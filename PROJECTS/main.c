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
#ifdef GEC_CB_MAIN 
#include "esc.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef GEC_CB_MAIN 

u8 testmode = 0;

/* ESC */
u8 Modbuff[5000];
u8 EscRTBuff[200];
u8 *const pcEscErrorBuff = &Modbuff[3000];
u16 *const ptErrorRecordFirstItem = (u16*)&Modbuff[2950];//2
u16 *const ptErrorRecordCounter = (u16*)&Modbuff[2952];//2
u8 *const pcEscErrorCodeBuff = &Modbuff[504];//2
u8 *const pcMbRtccBuff = &Modbuff[50];//6
u16 *const pcStateBuff = (u16*)&Modbuff[460];
u32 *const plPowerOnTms = (u32*)&Modbuff[140];//4  
u16 *const pt_SysBuff = (u16*)&Modbuff[1100];
#endif

/*******************************************************************************
* Function Name  : Start_Task
* Description    : This function contains the task to be started.               
* Input          : None                
* Output         : None
* Return         : None
*******************************************************************************/
void Start_Task(void)
{
        
  
#ifdef GEC_CB_MAIN 
  
        /* self check */
        self_check_init();

      
#if   ETH_LWIP_TEST 

        /* ETH BSP init */
	if(!ETH_BSP_Config())
        {
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
            
        }
        
#endif /* ETH_LWIP_TEST ------------------------------------------------------*/          
        
        
#if     MODBUS_LOCAL_DDU_TEST
        /* modbus loacal ddu test */
        modbus_local_ddu_init();
        
#elif   MODBUS_RTU_TEST    
        /* modbus rtu , baud:115200 , device_id:0x0A */
        modbus_rtu_init();
        
#elif   RX485_TEST      
        /* rx485 loopback test */
        rx485_test_init();         
        
#endif 

        
     
#if   USB_HOST_MASS_STORAGE_TEST   
      /* usb mass storage host mode test */
      usb_mass_storage_init();
#endif

              
#if   INPUT_RELAY_OUTPUT_AND_CAN_TEST       
      /* input output and can communication test */
        input_test_init();   
#endif        
              
        
#if   RTC_CLOCK_TEST
        /* use stm32 internal rtc clock */
        rtc_clock_init();								       
#endif              
        
#if     COMM_DISPLAY_BOARD_TEST
         /* communication with display board */
        comm_db_init();
#endif
        
        /* led and ewdt task, the highest priority */
        led_ewdt_init();

        
	/* Start scheduler */
	vTaskStartScheduler();

              
        
#else
          /* in GEC-CB bootloader, enter usb mass storage host mode */
          /* wait 1s to update , overtime enter APP */
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
#ifdef GEC_CB_BOOTLOADER    
    u16 i; 
    
    /* Power up delay */
    for( i = 0; i < 10000; i++ );   
#endif
    
    Initial_Device();
    Start_Task();
        
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




	

