/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : stm32f10x_STLstartup.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/01/2007
* Description        : This file contains all calls to the routines to be
*                      executed at start.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/*Reviewed for IEC 60335-1 Compliance
* This module can be used for self tests according to Table H.11.12.7 of appendix R of the standard ;EN/IEC 60335-1. The implementation of the module has to be tested in each application.
* Reviewed by V.D.E. Testing and Certification Institute ; Merianstrasse 28 ; D-63069 Offenbach
* VDE-Ref:    5008057-9999-0001/91436 FG23/Scb
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "stm32f10x_STLlib.h"

#define ALLOC_GLOBALS
#include "stm32f10x_STLclassBvar.h"

#ifdef STL_VERBOSE_POR
  #include <stdio.h>
#endif  /* STL_VERBOSE_POR */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifdef STL_VERBOSE_POR
  static void STL_VerbosePORInit(void);
//  static void USART_Configuration(void);
#endif  /* STL_VERBOSE_POR */

static ErrorStatus RCC_Config64MHzOnHSI(void);
static ErrorStatus RCC_SwitchOffPLL(void);


/*******************************************************************************
* Function Name  : FailSafePOR
* Description    : Contains the Fail Safe routine executed in case of
*                  failure detected during one of the POR self-test routines
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FailSafePOR(void)
{
//  SysTick_ITConfig(DISABLE);
  #ifdef STL_VERBOSE_POR
  printf(" >>>>>>>>>> POR FailSafe Mode <<<<<<<<<<\n\r");
  #endif  /* STL_VERBOSE_POR */
  while(1)
  {
    NVIC_SystemReset();
  }
}

/*******************************************************************************
* Function Name  : STL_StartUp
* Description    : Contains the very first test routines executed right after
*                  the reset
* Input          : None
* Output         : Zero initialized RAM, CPU clock provided by the HSE
*                  Flash interface initialized, Systick timer ON (2ms timebase)
* Return         : None
*******************************************************************************/
void STL_StartUp(void)
{
  #ifdef STL_VERBOSE_POR
    /* NOTE: Depending on compiler optimization level, printf may not operate
    correctly. In this case, STL_StartUpCPUTest and STL_FullRamMarchC must be
    commented (this functions are destroying R4 and stack content) */
    STL_VerbosePORInit();
    printf("\n\n\r *******  Self Test Library Init  *******\n\r");
  #endif

  /*--------------------------------------------------------------------------*/
  /*------------------- CPU registers and Flags Self Test --------------------*/
  /*--------------------------------------------------------------------------*/
  /* WARNING: all registers destroyed when exiting this function (including
  preserved registers R4 to R11) and excluding stack point R13) */

  /* Initializes counter for control flow monitoring */
  CtrlFlowCnt = CPU_TEST_CALLER;
  CtrlFlowCntInv = 0xFFFFFFFFuL;

  if (STL_StartUpCPUTest() != CPUTEST_SUCCESS)
  {
    #ifdef STL_VERBOSE_POR
      printf("Start-up CPU Test Failure\n\r");
    #endif /* STL_VERBOSE_POR */
    FailSafePOR();
  }
  else  /* Test OK */
  {
    /* If else statement is not executed, it will be detected by control flow monitoring */
    CtrlFlowCntInv -= CPU_TEST_CALLER;
    #ifdef STL_VERBOSE_POR
      printf("Start-up CPU Test OK\n\r");
    #endif /* STL_VERBOSE_POR */
  }

  CtrlFlowCnt += WDG_TEST_CALLER;
//  STL_WDGSelfTest();
  CtrlFlowCntInv -= WDG_TEST_CALLER;

  /*--------------------------------------------------------------------------*/
  /*-------------------- Switch ON PLL for 64MHz operation--------------------*/
  /*--------------------------------------------------------------------------*/
  /* No Control flow check here (not safety critical) */
  /* Switch on the PLL to speed-up Flash and RAM tests */
  if (RCC_Config64MHzOnHSI() != SUCCESS)
  {
  #ifdef STL_VERBOSE_POR
    printf("PLL Switch on failure\n\r");
    FailSafePOR();
  #endif  /* STL_VERBOSE_POR */
  }

  #ifdef STL_VERBOSE_POR
    USART_Configuration();  // Re-init USART with modified clock setting
  #endif  /* STL_VERBOSE_POR */


  /*--------------------------------------------------------------------------*/
  /*--------------------- Invariable memory CRC check ------------------------*/
  /*--------------------------------------------------------------------------*/

  CtrlFlowCnt += CRC32_TEST_CALLER;
  {
      u32 TmpCRC;
    /* Initialize the internal 32-bit CRC generator */
    CRC_Init();
    /* Compute the 32-bit crc of the whole Flash */
    TmpCRC = CRC_CalcBlockCrc((uc32 *)ROM_START, (u32)ROM_SIZEinWORDS);
    /* Store the inverted least significant byte of the CRC in the peripheral */
    SetIDRegister(~((u8)TmpCRC));
    CtrlFlowCntInv -= CRC32_TEST_CALLER;
  }

  /* Regular 16-bit crc computation */
  CtrlFlowCnt += CRC16_TEST_CALLER;
  if(STL_crc16(CRC_INIT,(u8 *)ROM_START, ROM_SIZE) != REF_CRC16)
  {
    #ifdef STL_VERBOSE_POR
    printf(" FLASH 16-bit CRC Error at Start-up\n\r");
    #endif  /* STL_VERBOSE_POR */
    FailSafePOR();
  }
  else  /* Test OK */
  {
    /* If else statement is not executed, it will be detected by control flow monitoring */
    CtrlFlowCntInv -= CRC16_TEST_CALLER;
    #ifdef STL_VERBOSE_POR
      printf(" Start-up FLASH 16-bit CRC OK\n\r");
    #endif  /* STL_VERBOSE_POR */
  }


  /*--------------------------------------------------------------------------*/
  /*   Verify Control flow before RAM init (which clears Ctrl flow counters)  */
  /*--------------------------------------------------------------------------*/
  if (((CtrlFlowCnt ^ CtrlFlowCntInv) != 0xFFFFFFFFuL)
      ||(CtrlFlowCnt != CHECKPOINT1 ))
  {
     #ifdef STL_VERBOSE_POR
       printf("Control Flow Error Checkpoint 1\n\r");
     #endif  /* STL_VERBOSE_POR */
     FailSafePOR();
  }
  else
  {
   #ifdef STL_VERBOSE_POR
    printf("Control Flow Checkpoint 1 OK \n\r");
   #endif  /* STL_VERBOSE_POR */
  }

  /*--------------------------------------------------------------------------*/
  /* --------------------- Variable memory functional test -------------------*/
  /*--------------------------------------------------------------------------*/
  /* WARNING: Stack is zero-initialized when exiting from this routine */
  if (STL_FullRamMarchC() != SUCCESS)
  {
    #ifdef STL_VERBOSE_POR
      printf("RAM Test Failure\n\r");
    #endif  /* STL_VERBOSE_POR */
    FailSafePOR();
  }
  #ifdef STL_VERBOSE_POR
    printf(" Full RAM Test OK\n\r");
  #endif /* STL_VERBOSE_POR */

  /* Both CtrlFlowCnt and CtrlFlowCntInv are zeroed then re-initialized inside
  the test routine to have inverse values */

  /*--------------------------------------------------------------------------*/
  /*-- Store reference 32-bit CRC in RAM after RAM test (if not corrupted) ---*/
  /*--------------------------------------------------------------------------*/
  if ((GetIDRegister() ^ ((u8)GetCurrentCRC())) == 0xFF)
  {
    RefCrc32 = GetCurrentCRC();
    RefCrc32Inv = ~RefCrc32;
    #ifdef STL_VERBOSE_POR
      printf("Ref 32-bit CRC OK\n\r");
    #endif  /* STL_VERBOSE_POR */

  }
  else
  {
    #ifdef STL_VERBOSE_POR
      printf("Ref 32-bit CRC corrupted\n\r");
    #endif  /* STL_VERBOSE_POR */
    FailSafePOR();

  }

  /*--------------------------------------------------------------------------*/
  /*--------------- Switch Off PLL to test external clock source--------------*/
  /*--------------------------------------------------------------------------*/
  /* No Control flow check here (not safety critical) */
  if (RCC_SwitchOffPLL() != SUCCESS)
  {
  #ifdef STL_VERBOSE_POR
    printf("Switch back to HSI failed\n\r");
    FailSafePOR();
  #endif  /* STL_VERBOSE_POR */
  }

  #ifdef STL_VERBOSE_POR
    USART_Configuration();  // Re-init USART with modified clock setting
  #endif  /* STL_VERBOSE_POR */

  /*--------------------------------------------------------------------------*/
  /*----------------------- Clock Frequency Self Test ------------------------*/
  /*--------------------------------------------------------------------------*/
#if 0
  CtrlFlowCnt += CLOCK_TEST_CALLER;
  switch ( STL_ClockStartUpTest() )
  {
    case FREQ_OK:
      #ifdef STL_VERBOSE_POR
        /* Switch back to HSI to be sure to have a valid display */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
        printf("Clock frequency OK \n\r");
      #endif  /* STL_VERBOSE_POR */
      break;

    case LSI_START_FAIL:
       #ifdef STL_VERBOSE_POR
        printf("LSI start-up failure \n\r");
      #endif  /* STL_VERBOSE_POR */
      FailSafePOR();
      break;

    case HSE_START_FAIL:
      #ifdef STL_VERBOSE_POR
        printf("HSE start-up failure \n\r");
      #endif  /* STL_VERBOSE_POR */
      FailSafePOR();
      break;

    case HSI_HSE_SWITCH_FAIL:
      #ifdef STL_VERBOSE_POR
        printf("Clock switch failure \n\r");
      #endif  /* STL_VERBOSE_POR */
      FailSafePOR();
      break;

    case EXT_SOURCE_FAIL:
      #ifdef STL_VERBOSE_POR
        printf("Clock Source failure \n\r");
      #endif  /* STL_VERBOSE_POR */
      FailSafePOR();
      break;

    case TEST_ONGOING:
    default:
      #ifdef STL_VERBOSE_POR
        printf("Abnormal Clock Test routine termination \n\r");
      #endif  /* STL_VERBOSE_POR */
      FailSafePOR();
      break;
  }
  // Either switch back to HSI or start PLL on HSE asap
  CtrlFlowCntInv -= CLOCK_TEST_CALLER;
#endif
  /*--------------------------------------------------------------------------*/
  /* -----  Verify Control flow before Starting main program execution ------ */
  /*--------------------------------------------------------------------------*/

   CtrlFlowCnt += STACK_OVERFLOW_TEST;
   StackOverFlowPtrn[0] = 0xAAAAAAAAuL;
   StackOverFlowPtrn[1] = 0xBBBBBBBBuL;
   StackOverFlowPtrn[2] = 0xCCCCCCCCuL;
   StackOverFlowPtrn[3] = 0xDDDDDDDDuL;
   CtrlFlowCntInv -= STACK_OVERFLOW_TEST;

  /*--------------------------------------------------------------------------*/
  /* -----  Verify Control flow before Starting main program execution ------ */
  /*--------------------------------------------------------------------------*/
   if (((CtrlFlowCnt ^ CtrlFlowCntInv) != 0xFFFFFFFFuL)
      ||(CtrlFlowCnt != CHECKPOINT2 ))
   {
     #ifdef STL_VERBOSE_POR
       printf("Control Flow Error Checkpoint 2\n\r");
     #endif  /* STL_VERBOSE_POR */
     FailSafePOR();
   }
   #ifdef STL_VERBOSE_POR
    printf("Control Flow Checkpoint 2 OK \n\r");
   #endif  /* STL_VERBOSE_POR */

   GotoCompilerStartUp()
      ;

}


#ifdef STL_VERBOSE_POR
/*******************************************************************************
* Function Name  : STL_VerbosePORInit
* Description    : Initializes the USART1 and few I/Os for test purposes
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STL_VerbosePORInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_AFIO, ENABLE);

  /* Configure PC.06 and PC.07 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_Configuration();

}


/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void USART_Configuration(void)
//{
//  USART_InitTypeDef USART_InitStructure;
//
//  USART_DeInit(USART1);
//
///* USART1 configuration ------------------------------------------------------*/
//  /* USART1 configured as follow:
//        - BaudRate = 115200 baud
//        - Word Length = 8 Bits
//        - One Stop Bit
//        - No parity
//        - Hardware flow control disabled (RTS and CTS signals)
//        - Receive and transmit enabled
//        - USART Clock disabled
//        - USART CPOL: Clock is active low
//        - USART CPHA: Data is captured on the middle
//        - USART LastBit: The clock pulse of the last data bit is not output to
//                         the SCLK pin
//  */
//  USART_InitStructure.USART_BaudRate = 115200uL;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//  USART_InitStructure.USART_Clock = USART_Clock_Disable;
//  USART_InitStructure.USART_CPOL = USART_CPOL_Low;
//  USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
//  USART_InitStructure.USART_LastBit = USART_LastBit_Disable;
//
//  USART_Init(USART1, &USART_InitStructure);
//
//  /* Enable USART1 */
//  USART_Cmd(USART1, ENABLE);
//}

#endif  /* STL_VERBOSE_POR */

/*******************************************************************************
* Function Name  : RCC_Config64MHzOnHSI
* Description    : Switch on the PLL to have the maximum avialable speed on
*                  internal high speed oscillator
* Input          : None
* Output         : None
* Return         : ErrorStatus = {ERROR; SUCCESS}
*******************************************************************************/
ErrorStatus RCC_Config64MHzOnHSI(void)
{
    ErrorStatus Result = ERROR;
    u32 TimeOut = PLL_LOCK_TIMEOUT;

  /* HCLK = SYSCLK */
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  /* PCLK2 = HCLK */
  RCC_PCLK2Config(RCC_HCLK_Div1);
  /* PCLK1 = HCLK/2 */
  RCC_PCLK1Config(RCC_HCLK_Div2);

  /* Flash 2 wait state */
  FLASH_SetLatency(FLASH_Latency_2);
  /* Enable Prefetch Buffer */
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

  /* PLLCLK = 4MHz * 16 = 64 MHz */
//  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);

  /* Wait till PLL is ready */
  do
  {
    TimeOut--;
  }
  while((RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) && (TimeOut != 0));

  if (TimeOut == 0)
  {
    Result = ERROR;     /* Clock switch failure */
  }
  else
  {
    /* Set timeout for clock switch */
    TimeOut = CLOCK_SWITCH_TIMEOUT;

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    do
    {
      TimeOut--;
    }
    while((RCC_GetSYSCLKSource() != 0x08) && (TimeOut != 0));

    if (TimeOut == 0)
    {
      Result = ERROR;     /* Clock switch failure */
    }
    else
    {
      Result = SUCCESS;
    }
  }

  return (Result);

}

/*******************************************************************************
* Function Name  : RCC_SwitchOffPLL
* Description    : Switch off the PLL to be able to measure external clock
*                  source frequency
* Input          : None
* Output         : None
* Return         : ErrorStatus = {ERROR; SUCCESS}
*******************************************************************************/
ErrorStatus RCC_SwitchOffPLL(void)
{
    ErrorStatus Result = ERROR;
    u32 TimeOut = CLOCK_SWITCH_TIMEOUT;
    /* Even if product runs at 64MHz, timeout is still much higher than the
    few clock cycles required for clock switch */

  /* Select HSI as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  /* Wait till HSI is used again as a system clock source */
  do
  {
    TimeOut--;
  }
  while ((RCC_GetSYSCLKSource() != 0x00) && (TimeOut != 0));

  if (TimeOut == 0)
  {
    Result = ERROR;     /* Clock switch failure */
  }
  else
  {
    /* We can safely switch off the PLL */
    RCC_PLLCmd(DISABLE);
    Result = SUCCESS;
  }

  return (Result);


}


/*******************************************************************************
* Function Name  : STL_WDGSelfTest
* Description    : Verifies the watchdog by forcing watchdog resets
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STL_WDGSelfTest(void)
{
  #ifdef STL_VERBOSE_POR
    if (RCC_GetFlagStatus(RCC_FLAG_PINRST) == SET) printf("Pin reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) == SET) printf("POR reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET) printf("SW reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) == SET) printf("LP reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) printf("IWDG reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) printf("WWDG reset \r\n");
  #endif /* STL_VERBOSE_POR */

  /* In one of the 4 conditions below, start watchdogs test */
  if ((RCC_GetFlagStatus(RCC_FLAG_PORRST) == SET)   /* Power on reset */
   || (RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET)   /* or Software reset */
   || (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) == SET)  /* or Low Power reset */
   || ((RCC_GetFlagStatus(RCC_FLAG_PINRST) == SET)  /* or triggered by */
    && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == RESET)  /* external pin solely */
    && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == RESET)))
  {
    #ifdef STL_VERBOSE_POR
      printf("... Power-on or software reset, testing IWDG ... \r\n");
    #endif  /* STL_VERBOSE_POR */

    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    /* IWDG clock: 32KHz(LSI) / 4 = 8KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_4);
    /* Set counter reload value to 1 (125�s */
    IWDG_SetReload(1);
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    /* Enable IWDG (LSI automatically enabled by HW) */
    IWDG_Enable();

    RCC_ClearFlag();        /* Clear all flags before reuming test */
    /* Wait for an independant watchdog reset */
    while(1);
  }
  else  /* Watchdog test or software reset triggered by application failure */
  {
    /* If WWDG only was set, re-start the complete test (indicates a reset triggered by safety routines */
    if ((RCC_GetFlagStatus(RCC_FLAG_PINRST)  == SET)
     && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
     && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == RESET))
    {
      RCC_ClearFlag();
      #ifdef STL_VERBOSE_POR
        printf("... WWDG reset, re-start WDG test ... \r\n");
      #endif  /* STL_VERBOSE_POR */
      NVIC_SystemReset();
    }
    else  /* If IWDG only was set, continue the test with WWDG test*/
    {
      if ((RCC_GetFlagStatus(RCC_FLAG_PINRST)  == SET)
       && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == RESET)
       && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET))
      { /* If IWDG only was set, test WWDG*/
        #ifdef STL_VERBOSE_POR
          printf("... IWDG reset from test or application, testing WWDG\r\n");
        #endif  /* STL_VERBOSE_POR */

        /* Enable WWDG clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
        /* WWDG clock counter = (PCLK1/4096)/8 = 976 Hz */
        WWDG_SetPrescaler(WWDG_Prescaler_1);
        /* Set Window close to timeout */
        WWDG_SetWindowValue(0x40);
        /* Enable WWDG and set counter to 0x7F to force directly a reset */
        WWDG_Enable(0x7F);
        while(1);

      }
      else  /* If both flags are set, means that watchdog test is completed */
      {
        if ((RCC_GetFlagStatus(RCC_FLAG_PINRST)  == SET)
         && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
         && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET))
        {
          RCC_ClearFlag();
          #ifdef STL_VERBOSE_POR
            printf("... WWDG reset, WDG test completed ... \r\n");
          #endif  /* STL_VERBOSE_POR */
        }
        else  /* Unexpected Flag configuration, re-start WDG test */
        {
          RCC_ClearFlag();
          #ifdef STL_VERBOSE_POR
            printf("...Unexpected Flag configuration, re-start WDG test... \r\n");
          #endif  /* STL_VERBOSE_POR */
          NVIC_SystemReset();
        } /* End of Unexpected Flag configuration */
      } /* End of normal test sequence */
    } /* End of partial WDG test (IWDG test done) */
  } /* End of part where 1 or 2 Watchdog flags are set */
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/

