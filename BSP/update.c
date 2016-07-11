/*******************************************************************************
* File Name          : update.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains app bin update functions.
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
* Description    : Checks whether a file exists
*                  
* Input          : filename: file name string
*                  
* Output         : None
* Return         : 0 exist
*******************************************************************************/
u8 isFileExist(char *filename)
{
  FIL* fp;
  FRESULT res = FR_NO_FILE;
  
  fp = (FIL*)mymalloc(sizeof(FIL));	
  if(fp != NULL)
  {      
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
* Function Name  : CopyFile
* Description    : Copy a new file.
*                  
* Input          : readfilename: Copied files name string
                   newfilename: New file name string.
*                  
* Output         : None
* Return         : 0 success
*******************************************************************************/
u8 CopyFile(char *readfilename, char *newfilename)
{
  FIL* fp1;
  FIL* fp2;
  FRESULT res = FR_NO_FILE;
    u8 *tempbuf;
    u16 bread;
    u32 offx=0;
  
  fp1 = (FIL*)mymalloc(sizeof(FIL));	
  fp2 = (FIL*)mymalloc(sizeof(FIL));	
  tempbuf = mymalloc(1024);
  
  
  if(fp1 != NULL && fp2 != NULL && tempbuf != NULL)
  {
    
      res = f_open(fp1,readfilename,FA_READ);
      
      res = f_open(fp2,newfilename,FA_CREATE_NEW | FA_WRITE);  
      printf("\r\n open res = %d \r\n",res);
      
        while(res==FR_OK)
        {
          res = f_read(fp1,tempbuf,1024,(UINT *)&bread);		
          if(res!=FR_OK)break;					
          res = f_write(fp2,tempbuf,bread,&offx);	  
          offx+=bread;
          if(bread!=1024)
          {           
                break;					
          }
        }            
        
       f_close(fp1); 
       f_close(fp2); 
        
      myfree(fp1);    
      myfree(fp2);
      myfree(tempbuf);
  }  
    
  return res;
  
}

/*******************************************************************************
* Function Name  : UpdateApp
* Description    : Update app bin file.
*                  
* Input          : filename: new app bin file name string.
*                  
* Output         : None
* Return         : 0 success
*******************************************************************************/
u8 UpdateApp(char *filename)
{
    FIL* fp;
    FRESULT res = FR_NOT_ENOUGH_CORE;
    u8 *tempbuf;
    u16 bread;
    u32 offx=0;
    u8  flag = 0;
    
    fp = (FIL*)mymalloc(sizeof(FIL));	
    tempbuf = mymalloc(4096);	
    
    if(fp != NULL && tempbuf != NULL)
    {      
        res = f_open(fp,filename,FA_READ);    
        printf("\r\n open res = %d \r\n",res);
        
//        INTX_DISABLE();
        while(res==FR_OK)
        {
          EWDT_TOOGLE();
          res=f_read(fp,tempbuf,4096,(UINT *)&bread);			
          if(res!=FR_OK)break;					
          if(0 == flag)
          {
              flag = 1;
              if(((*(vu32*)(tempbuf + 4))&0xFF000000)!=0x08000000)
              {
                  res = FR_INVALID_OBJECT;
                  break;             
              }
          }
          iap_write_appbin(FLASH_APP1_ADDR + offx,tempbuf,4096);	  
          offx+=bread;
          if(bread!=4096)break;					
        }      
//        INTX_ENABLE();
    }
    
    printf("\r\n update res = %d \r\n",res);
    
    f_close(fp);	   
    myfree(fp);
    myfree(tempbuf);
    
    return res;  
}

/*******************************************************************************
* Function Name  : DeleteFile
* Description    : Delete a file.
*                  
* Input          : filename: deleted file name string.
*                  
* Output         : None
* Return         : 0£ºsuccess
*******************************************************************************/
u8 DeleteFile(char *filename)
{
    FRESULT res;
    
    res = f_unlink(filename);
    
    return res;
}

/*******************************************************************************
* Function Name  : ReadDir
* Description    : Read a file directory
*                  
* Input          : path: directory  str: input a string
*                  
* Output         : str: return the name of the upgrade file found
* Return         : 0 success
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
    res = f_opendir(&dir,(const TCHAR*)path); 
    if (res == FR_OK) 
    {	
//      printf("\r\n"); 
      while(1)
      {
        res = f_readdir(&dir, &fileinfo);                   
        if (res != FR_OK || fileinfo.fname[0] == 0) break;  
        //if (fileinfo.fname[0] == '.') continue;             
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
        if(flag == 2)//version upgrade
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
        else if(flag == 3)//force upgrade
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

