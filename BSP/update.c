/*******************************************************************************
* File Name          : update.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "update.h"
#include "ewdt.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
* Function Name  : isFileExist
* Description    : �ж��ļ��Ƿ����
*                  
* Input          : �ļ����ַ���
*                  
* Output         : None
* Return         : 0 ����
*******************************************************************************/
u8 isFileExist(char *filename)
{
  FIL* fp;
  FRESULT res = FR_NO_FILE;
  
  fp = (FIL*)mymalloc(sizeof(FIL));	//�����ڴ�
  if(fp != NULL)
  {
    
//      res = f_open(fp,"0:DU.bin",FA_READ);
      
      res = f_open(fp,filename,FA_READ);  
      printf("\r\n open res = %d \r\n",res);
      
      if(res==FR_OK)
      {
	 f_close(fp);						
      }
      myfree(fp);      
  }  
    
  return res;
  
}

/*******************************************************************************
* Function Name  : CreateFile
* Description    : 
*                  
* Input          : �ļ����ַ���
*                  
* Output         : None
* Return         : 0 success
*******************************************************************************/
u8 CreateFile(char *readfilename, char *createfilename)
{
  FIL* fp1;
  FIL* fp2;
  FRESULT res = FR_NO_FILE;
    u8 *tempbuf;
    u16 bread;
    u32 offx=0;
  
  fp1 = (FIL*)mymalloc(sizeof(FIL));	//�����ڴ�
  fp2 = (FIL*)mymalloc(sizeof(FIL));	//�����ڴ�
  tempbuf = mymalloc(1024);
  
  
  if(fp1 != NULL && fp2 != NULL && tempbuf != NULL)
  {
    
      res = f_open(fp1,readfilename,FA_READ);
      
      res = f_open(fp2,createfilename,FA_CREATE_NEW | FA_WRITE);  
      printf("\r\n open res = %d \r\n",res);
      
        while(res==FR_OK)//��ѭ��ִ��
        {
          res = f_read(fp1,tempbuf,1024,(UINT *)&bread);		//��ȡ����	
          if(res!=FR_OK)break;					//ִ�д���

          res = f_write(fp2,tempbuf,bread,&offx);
//          iap_write_appbin(FLASH_APP1_ADDR + offx,tempbuf,4096);//����FLASH���� 	  
          offx+=bread;
          if(bread!=1024)
          {
//            	u32 total,free;
//                u8 res=0;
//	 
//                res=exf_getfree("0:",&total,&free);
//        	if(res==0)
//                {	   
//                        total = total>>10;
//                        free  = free>>10;
//
//                        f_write(fp2,tempbuf,bread,&offx);
//	    	
//                } 
                
                
                break;					//������.
          }
        }       
      
      
//      if(res==FR_OK)
//      {         
//	 f_close(fp);						
//      }
        
       f_close(fp1); 
       f_close(fp2); 
        
      myfree(fp1);    
      myfree(fp2);
  }  
    
  return res;
  
}

/*******************************************************************************
* Function Name  : UpdateApp
* Description    : ����APP
*                  
* Input          : �ļ����ַ���
*                  
* Output         : None
* Return         : 0 ���³ɹ�
*******************************************************************************/
u8 UpdateApp(char *filename)
{
    FIL* fp;
    FRESULT res = FR_NOT_ENOUGH_CORE;
    u8 *tempbuf;
    u16 bread;
    u32 offx=0;
    u8  flag = 0;
    
    fp = (FIL*)mymalloc(sizeof(FIL));	//�����ڴ�
    tempbuf = mymalloc(4096);	//����4096���ֽڿռ�
    
    if(fp != NULL && tempbuf != NULL)
    {      
        res = f_open(fp,filename,FA_READ);    
        printf("\r\n open res = %d \r\n",res);
        
//        INTX_DISABLE();//�ر������ж�
        while(res==FR_OK)//��ѭ��ִ��
        {
          EWDT_TOOGLE();
          res=f_read(fp,tempbuf,4096,(UINT *)&bread);		//��ȡ����	
          if(res!=FR_OK)break;					//ִ�д���
          if(0 == flag)
          {
              flag = 1;
              if(((*(vu32*)(tempbuf + 4))&0xFF000000)!=0x08000000)
              {
                  res = FR_INVALID_OBJECT;
                  break;             
              }
          }
          iap_write_appbin(FLASH_APP1_ADDR + offx,tempbuf,4096);//����FLASH���� 	  
          offx+=bread;
          if(bread!=4096)break;					//������.
        }      
//        INTX_ENABLE();//�������ж�
    }
    
    printf("\r\n update res = %d \r\n",res);
    
    f_close(fp);	   
    myfree(fp);
    myfree(tempbuf);
    
    return res;  
}

/*******************************************************************************
* Function Name  : DeleteFile
* Description    : ɾ�������̼�
*                  
* Input          : �ļ����ַ���
*                  
* Output         : None
* Return         : 0��ɾ���ɹ�
*******************************************************************************/
u8 DeleteFile(char *filename)
{
    FRESULT res;
    
    res = f_unlink(filename);
    
    return res;
}

/*******************************************************************************
* Function Name  : ReadDir
* Description    : ��ȡĿ¼�µ��ļ�
*                  
* Input          : path:Ŀ¼  str:����һ���ַ����������ҵ��������ļ���
*                  
* Output         : None
* Return         : 0 �ɹ�
*******************************************************************************/
extern u8 App_Version[2];
u8 ReadDir(u8 * path, char str[])
{
    FRESULT res;
    char *fn;   /* This function is assuming non-Unicode cfg. */
    char updatestr[20] = "DU_V1_0.bin";
    char forceupdatestr[10] = "DU.bin";
    u8 flag = 0;
#if _USE_LFN
    fileinfo.lfsize = _MAX_LFN * 2 + 1;
    fileinfo.lfname = mymalloc(fileinfo.lfsize);
#endif	
    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK) 
    {	
//      printf("\r\n"); 
      while(1)
      {
        res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
        //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
#if _USE_LFN
        fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        fn = fileinfo.fname;
#endif	                                              /* It is a file. */
        if(fn[0] == forceupdatestr[0] && fn[1] == forceupdatestr[1] && fn[2] == forceupdatestr[2] && 
           fn[3] == forceupdatestr[3] && fn[4] == forceupdatestr[4] && fn[5] == forceupdatestr[5] && 
           fn[6] == forceupdatestr[6])
        {
            flag = 3;
        }
        else
        {
            for(u8 i = 0; i < 11; i++)
            {
                if(fn[i] != updatestr[i])
                {
                    if(i == 4 || i == 6)
                    {
                        flag = 2;
                    }
                    else
                    {
                        flag = 1;
                        break;
                    }
                }
            }
        }
        if(flag == 2)//�汾������
        {
            flag = 0;
            if(App_Version[0] < fn[4] || (App_Version[0] <= fn[4] && App_Version[1] < fn[6]))
            {
                App_Version[0] = fn[4];
                App_Version[1] = fn[6];

                sprintf(str, "%s%s",path,fn);
                break;
            }
        }
        else if(flag == 3)//ǿ�����������Ѱ汾�ż�¼Ϊ1.0
        {
                App_Version[0] = '1';
                App_Version[1] = '0';

                sprintf(str, "%s%s",path,fn);
                break;        
        }
      } 
    }	  
    myfree(fileinfo.lfname);
    return res;	  
}



/******************************  END OF FILE  *********************************/
