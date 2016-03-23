/*******************************************************************************
* File Name          : malloc.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "malloc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//内存参数设定.
#define MEM_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM_MAX_SIZE			13*1024           				        //最大管理内存 13K
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	                                //内存表大小

/* Private variables ---------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//设定为4字节对齐  
__no_init u8 membase[MEM_MAX_SIZE]; //内部SRAM内存池   
#pragma pack(pop)//恢复对齐状态
/*内存池(4字节对齐)*/
//__align(4) u8 membase[MEM_MAX_SIZE];			//SRAM内存池
/*内存管理表*/
u16 memmapbase[MEM_ALLOC_TABLE_SIZE];			//SRAM内存池MAP
/*内存管理参数*/	   
const u32 memtblsize=MEM_ALLOC_TABLE_SIZE;		//内存表大小
const u32 memblksize=MEM_BLOCK_SIZE;			//内存分块大小
const u32 memsize=MEM_MAX_SIZE;					//内存总大小

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
	mmem_init,			//内存初始化
	mem_perused,		//内存使用率
	membase,			//内存池
	memmapbase,			//内存管理状态表
	0,  				//内存管理未就绪
}; 



/*******************************************************************************
* Function Name  : mymemcpy
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 	   
//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  


/*******************************************************************************
* Function Name  : mymemset
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	


/*******************************************************************************
* Function Name  : mmem_init
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//内存管理初始化  
void mmem_init(void)  
{  
    mymemset(mallco_dev.memmap, 0,memtblsize*2);//内存状态表数据清零  
	mymemset(mallco_dev.membase, 0,memsize);	//内存池所有数据清零  
	mallco_dev.memrdy=1;						//内存管理初始化OK  
}  

/*******************************************************************************
* Function Name  : mem_perused
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//获取内存使用率
//返回值:使用率(0~100)
u8 mem_perused(void)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize;i++)  
    {  
        if(mallco_dev.memmap[i])used++; 
    } 
    return (used*100)/(memtblsize);  
}  


/*******************************************************************************
* Function Name  : mmem_malloc
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 mmem_malloc(u32 size)  
{  
    signed long offset=0;  
    u16 nmemb;	//需要的内存块数  
	u16 cmemb=0;//连续空内存块数
    u32 i;  
    if(!mallco_dev.memrdy)mallco_dev.init();	//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;				//不需要分配
    nmemb=size/memblksize;  					//获取需要分配的连续内存块数
    if(size%memblksize)nmemb++;  
    for(offset=memtblsize-1;offset>=0;offset--)	//搜索整个内存控制区  
    {     
		if(!mallco_dev.memmap[offset])cmemb++;	//连续空内存块数增加
		else cmemb=0;							//连续内存块清零
		if(cmemb==nmemb)						//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  				//标注内存块非空 
            {  
                mallco_dev.memmap[offset+i]=nmemb;  
            }  
            return (offset*memblksize);			//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  


/*******************************************************************************
* Function Name  : mmem_free
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//释放内存(内部调用) 
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 mmem_free(u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy)//未初始化,先执行初始化
	{
		mallco_dev.init();    
        return 1;//未初始化  
    }  
    if(offset<memsize)//偏移在内存池内. 
    {  
        int index=offset/memblksize;		//偏移所在内存块号码  
        int nmemb=mallco_dev.memmap[index];	//内存块数量
        for(i=0;i<nmemb;i++)  				//内存块清零
        {  
            mallco_dev.memmap[index+i]=0;  
        }
        return 0;  
    }else return 2;//偏移超区了.  
}  


/*******************************************************************************
* Function Name  : myfree
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//释放内存(外部调用) 
//ptr:内存首地址 
void myfree(void *ptr)  
{  
	u32 offset;  
    if(ptr==NULL)return;//地址为0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase;  
    mmem_free(offset);	//释放内存     
}  


/*******************************************************************************
* Function Name  : mymalloc
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//分配内存(外部调用)
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u32 size)  
{  
    u32 offset;  									      
	offset=mmem_malloc(size);  	   				   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase+offset);  
}  


/*******************************************************************************
* Function Name  : myrealloc
* Description    : None
*                  
* Input          : appxaddr:应用程序的起始地址
*                  appbuf:应用程序CODE.
*                  appsize:应用程序大小(字节).
* Output         : None
* Return         : None
*******************************************************************************/ 
//重新分配内存(外部调用)
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(void *ptr,u32 size)  
{  
    u32 offset;  
    offset=mmem_malloc(size);  
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase+offset),ptr,size);	//拷贝旧内存内容到新内存   
        myfree(ptr);  											  	//释放旧内存
        return (void*)((u32)mallco_dev.membase+offset);  			//返回新内存首地址
    }  
}


/******************************  END OF FILE  *********************************/














