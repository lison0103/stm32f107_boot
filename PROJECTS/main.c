/*******************************************************************************
* File Name          : main.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : Main program body.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "initial_devices.h"
#include "includes.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//主控参数
u16 TimingDelay=0,System_Sequence=0,System_Check_Fault_Flag=0;

u8 Modbuff[5400]; 

u8 EscRTBuff[200],McRxBuff[1000];    

u16 *const ptModbuff  = (u16*)Modbuff; 

//------------------------------------------------------------------------------------------- ModBus
u8 *const pcMbRtccBuff = &Modbuff[50];  				//6字节

//modbus 
u8 *const pcEscErrorCodeBuff = &Modbuff[454];  	//5字节
u8 *const pcEscStateBuff = &Modbuff[460];	    	//4字节 
u8 *const pcEscAlarmBuff = &Modbuff[464];				//4字节
u8 *const pcEscErrorBuff = &Modbuff[468];				//32字节

//统计数组
u8 *const pcStatDataBuff = &Modbuff[500];  			
//记录故障
u16 *const ptErrorRecordFirstItem = (u16*)&Modbuff[530];

//系统参数
u8 *const ParaBuff = &Modbuff[1100];
u16 *const ptParaBuff = (u16*)&Modbuff[1100];

// 通信配置40676																	
//u16 *ptParaCommCgcBuff = (u16*)&Modbuff[1350];	//242字节
//u8 *pcParaCommCgcBuff = &Modbuff[1350];			//242字节

//多段速配置 50字节
u8 *const pcParaMultiSpeedBuff = &Modbuff[1600];	//50字节

//故障复位bit标志
u8 *const pcParaERB_DecodeBuff = &Modbuff[1660];		//32字节

//故障复位方式，人工复位252字节
u8 *const pcParaErrorResetBuff = &Modbuff[1700];		//252字节

//记录故障
u8 *const pcRtDataBuff = &Modbuff[2700];  
u8 *const pcRtRecordErrorCodeBuff = &Modbuff[2750];  

u8 *const pcErrorRecordBuff = &Modbuff[3000];   

//----------------------------------------------------------------------------------------- 实时数据
u16 *const ptEscRTBuff = 	(u16*)EscRTBuff;  

u8 *const pcInputBuff = 	&EscRTBuff[4];  
u8 *const pcCMDBuff = 		&EscRTBuff[20];  
u8 *const pcOutputBuff = 	&EscRTBuff[30];  
//u8 *const pcStateBuff = 	&EscRTBuff[60];  
u8 *const pcAlarmBuff = 	&EscRTBuff[64];  
u8 *const pcErrorBuff = 	&EscRTBuff[68];

//常数指针 const char* chrPtr
u32 *const plPowerOnTms = (u32*)&EscRTBuff[140];  

u8 *const pcPgaInputMask = &EscRTBuff[150];    
u8 *const pcPgaInputBuff = &EscRTBuff[170];  
u8 *const pcPgaOutputBuff = &EscRTBuff[190];  

//
u8 *const pcOMC_EscRTBuff = &McRxBuff[0];




/*******************************************************************************
* Function Name  : Task_Loop
* Description    : This function contains the task to be executed in a loop.               
* Input          : None                
* Output         : None
* Return         : None
*******************************************************************************/
void Task_Loop(void)
{
        
  
#ifdef GEC_CB_MAIN 
  
        /* self check */
        self_check_init();

      
#if   ETH_LWIP_TEST 

        /* ETH BSP init */
	ETH_BSP_Config();
        
	/* Initilaize the LwIP stack */
	LwIP_Init();

        
#if   TCP_CLIENT_TEST 
        /* tcp client , need to set the local ip / port and gateway */
	tcp_client_init();
#endif /* TCP_CLIENT_TEST */
      
        
#if   TCP_SERVER_TEST    
        /* tcp server , need to set the local ip / port and gateway */
        tcp_server_init();
#endif /* TCP_SERVER_TEST */
      
        
#if   MODBUS_TCP_TEST      
        /* modbus tcp , need to set the local ip / port and gateway */
        modbus_socket_init();
#endif /* MODBUS_TCP_TEST */
 
        
#ifdef USE_DHCP
        /* Start DHCPClient */
        lwip_dhcp_init();
#endif /* USE_DHCP */                	

        
        
#endif /* ETH_LWIP_TEST ------------------------------------------------------*/          
        
        
        
#if   MODBUS_RTU_TEST    
        /* modbus rtu , baud:115200 , device_id:0x0A */
        modbus_rtu_init();
#endif 

        
     
#if   USB_HOST_MASS_STORAGE_TEST   
      /* usb mass storage host mode test */
      usb_mass_storage_init();
#endif

              
#if   INPUT_RELAY_OUTPUT_AND_CAN_TEST       
      /* input output and can communication test */
        input_test_init();               
#endif        
        
           
#if   RX485_TEST      
        /* rx485 loopback test */
        rx485_test_init();
#endif   
         
        
#if   RTC_CLOCK_TEST
        /* use stm32 internal rtc clock */
        rtc_clock_init();								       
#endif                 
        
        /* led and ewdt task, the highest priority */
        led_ewdt_init();

        
	/* Start scheduler */
	vTaskStartScheduler();

              
        
#else
          /* in GEC-CB bootloader, enter usb mass storage host mode */
          /* wait 3s to update , overtime enter APP */
          usb_process();
          
#endif
        
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.             
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{        
 	        
        Bsp_Init();
        Task_Loop();
        
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/******************************  END OF FILE  *********************************/




	

