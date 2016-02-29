/**
 * Company	: ThyssenKrupp PDC
 * @file	: main.c
 * @purpose
 * @brief	: main code for applications for IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 26-March-2014
 * @author	: Paul
*/

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "check_instruction.h"
#include "check_instruction_2.h"
#include "cpu_reg_test.h"
#include "ram_test.h"
#include "flash_test.h"
#include "wdt_test.h"
#include "clock_test.h"
#include "pc_test.h"
#include "config_test.h"
#include "flag_test.h"


#include "self_test.h"

//GPIO_InitTypeDef GPIO_InitStructure;

extern IEC61508_InstCheck_struct InstCheckPOST_struct;
extern IEC61508_InstCheck_struct InstCheckBIST_struct;

extern CPUreg_struct CPUregTestPOST_struct;

/* Private function prototypes -----------------------------------------------*/

int self_test(void)
{
 
  //unsigned int un;
  //un=(*( u16 *)0x1FFFF7E0);   //���������Ĵ���
  //type_testResult_t result = IEC61508_testFailed;                 
  volatile type_testResult_t result = IEC61508_testFailed;   /* variable is located in the stack */
//  STM_EVAL_LEDInit(LED1);

  result = flag_test();
  if (result != IEC61508_testPassed)
  {
    while(1);                            /* remains if FLAG test fails */
  }
#if 0  
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_Configuration();//HSE - 8MHz
  RCC_GetClocksFreq(&RCC_Clocks); 
  if (RCC_Clocks.SYSCLK_Frequency !=8000000)
    while(1);

  RCC_Config(); //PLL - 72MHz
  RCC_GetClocksFreq(&RCC_Clocks); 
  if (RCC_Clocks.SYSCLK_Frequency !=72000000)
    while(1);
  
   /* Configure PA.00 in interrupt mode */
  EXTI0_Config();
  /* Generate software interrupt: simulate a falling edge applied on EXTI0 line */
  EXTI_GenerateSWInterrupt(EXTI_Line0);//�жϺ����м����־ IEC61508_testPassed���жϳɹ�����
 
  result = IEC61508_PCTest_POST();
  if (result != IEC61508_testPassed)
  {
    while(1);                                      /* remains if PC test fails */
  }

  /* clock/rtc test */
 /* Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.
    
    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

    4. To adjust the SysTick time base, use the following formula:
                            
         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
    
       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
   */ 
 if (SysTick_Config(SystemCoreClock / 100))  // 1/100s = 10ms
  { 
    /* Capture error */ 
    while (1);
  }

  RCC_cfg();
  //STM_EVAL_LEDInit(LED1);
  //Delay(0x1FFFFF);
  IEC61508_initClockTest(100, 5);                      /* init clock test */
  //result = IEC61508_Clocktest_PollHandler();           /* check evidence */
  
  //while(1);
  
  //��Ϊ�������Ź�ʹ�õ���LSI��������ó���������ʱ��ʹʱ��Դ�ȶ�:
  /* LSI������*/
  RCC_LSICmd(ENABLE);//��LSI
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);    //�ȴ�ֱ��LSI�ȶ� 

 /* Configure the system clocks */
  RCC_Configuration();
//  STM_EVAL_LEDInit(LED1);
  Delay(0x1FFFFF);
 
/*WWDG**************************/
  if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)   
  {
    //GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
   /*��������ĸ�λ��־λ�У� RCC_FLAG_PINRST, RCC_FLAG_PORRST, RCC_FLAG_SFTRST, 
     RCC_FLAG_IWDGRST,  RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST */
    result = IEC61508_testPassed;
    //RCC_ClearFlag();
    //WWDG_DeInit();
  }
  else
  {
   //GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
    result = IEC61508_testFailed;
    WWDG_Configuration();
  }
  /* Infinite loop */
  if (result != IEC61508_testPassed)
  {
//    STM_EVAL_LEDOn(LED1);
    while(1)                                      /* remains until WDOG test passes */
    { 
      //�ȴ�mcu��IWDG��λ
      /* Set WWDG counter value to 0x70 ���ü�����ֵΪ0x7f*/ 
      //WWDG_SetCounter(0x7f); 
      /* Clear EWI flag������ڻ����жϱ�־λ */
      //WWDG_ClearFlag();
    }
  }  
   
/*IWDG**************************/
  /* Check if the system has resumed from IWDG reset */
 if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* IWDGRST flag set ��������λ����������ͨ�����˴β��ٳ�ʼ������Ҳ�����*/
    result = IEC61508_testPassed;
    /* Clear reset flags */
    RCC_ClearFlag();
  }
  else
  {
    /* IWDGRST flag is not set */
    /*! @cond TEST_POST_WDOG ��ʼ������ʱ����δ���δ������λ�����ù���ʼ�����򣬼��*/
    result = IEC61508_Force_WDT_Reset();           /* test WDOG with reset */
  }

  if (result != IEC61508_testPassed)
  {
//    STM_EVAL_LEDOn(LED1);
    while(1)                                      /* remains until WDOG test passes */
    { 
      //�ȴ�mcu��IWDG��λ
      //IWDG_ReloadCounter();  //KRд��0x5555 ���¿�ʼ���������ø�λ
    }
  }  
 
/******/  
  result = IEC61508_testFailed;
  result = ((type_testResult_t)IEC61508_FLASHtest_POST());            /* test FLASH content */
  result = ((type_testResult_t)IEC61508_FLASHtest_POST2());            /* test FLASH content */
  /* TODO : insert correct CRC value to line 80 in iec61508_flash_test.h */
  if (result != IEC61508_testPassed)
  {
         while (1);                    /* remains if Flash content is not consistent */
  }

  /* Flash test */
  result = IEC61508_FLASHtest_BIST (FLASH_CRC_Restart);
  result = IEC61508_FLASHtest_BIST (0);
  
  
/******/
  
  /* Do the IEC61508 CPU register tests */   //ok
  _CPUregTestPOST();
  if (CPUregTestPOST_struct.testPassed == IEC61508_testFailed)
  {
      /* POST CPU register test failed */
      while (1);
  }
 
   //Check_Instruction_Set();
  //perip_64bitaccess();
  /* Do the IEC61508 instruction tests */
  if (iec61508_InstCheck_POST() == IEC61508_testFailed)
  {
      /* POST instruction test failed */  /*waitting for WATCHDOG to reset*/
      while (1);
  }
  
    /* test results */  //no ok  ->move to after Instruction Set check, then ok
  result = ((type_testResult_t)IEC61508_RAMtest_BIST (0x20000100, 0x10));
  if (result != IEC61508_testPassed)
  {
        while (1);
  }
  
#endif
  //no ok  RAM area 0x20000D78 there are progam, must use BIST test, data must be save before testing 
  //type_testResult_t result = IEC61508_testFailed;                 
  //result = ((type_testResult_t)IEC61508_RAMtest(CORE_RAM_BASE_ADDR, IEC61508_RAM_SIZE, BLOCK_SEL_CORE));
  //if (result != IEC61508_testPassed)
  //{
  //      while (1);
  //}

//  STM_EVAL_LEDInit(LED1);
//  STM_EVAL_LEDInit(LED2);
//  while(1){
//    STM_EVAL_LEDOn(LED2);
//    /* Turn on LED1 */
//    STM_EVAL_LEDOff(LED1);
//    Delay(0x3FFFFF);
//
//    STM_EVAL_LEDOn(LED1);
//    STM_EVAL_LEDOff(LED2);
//    Delay(0x3FFFFF);
//  }
  
  return result;
} 

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

///**
//  * @brief  Configures LED GPIO.
//  * @param  Led: Specifies the Led to be configured. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  * @retval None
//  */
//void STM_EVAL_LEDInit(Led_TypeDef Led)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  
//  /* Enable the GPIO_LED Clock */
//  RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);
//
//  /* Configure the GPIO_LED pin */
//  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//
//  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
//}
//
///**
//  * @brief  Turns selected LED On.
//  * @param  Led: Specifies the Led to be set on. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  * @retval None
//  */
//void STM_EVAL_LEDOn(Led_TypeDef Led)
//{
//  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];     
//}
//
///**
//  * @brief  Turns selected LED Off.
//  * @param  Led: Specifies the Led to be set off. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  * @retval None
//  */
//void STM_EVAL_LEDOff(Led_TypeDef Led)
//{
//  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];  
//}
//
///**
//  * @brief  Toggles the selected LED.
//  * @param  Led: Specifies the Led to be toggled. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  * @retval None
//  */
//void STM_EVAL_LEDToggle(Led_TypeDef Led)
//{
//  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
//}

/************************************** EOF *********************************/
