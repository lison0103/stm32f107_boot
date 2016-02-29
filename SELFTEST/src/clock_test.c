/**
 * Company	: ThyssenKrupp PDC
 * @file	: clock_test.c
 * @purpose     :
 * @brief	: c based clock structure test POST and BIST clock structure test 
 *                  IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 5-May-2014
 * @author	: Paul
*/

#include "iec61508.h"
#include "ram_test.h"
#include "clock_test.h"
#include "stm32f10x_tim.h"

ClockTest_t ClockTest;  /* Clock/Timer-test requires fixed data */

/*!
 * @brief    internal function to reset all test structures
 */
/*! @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#if defined (__ICCARM__)
    #pragma section="IEC61508_code"
#else
__attribute__((section(".IEC61508_code")))
#endif
/*! @endcond (DO_NOT_INCLUDE_WITH_DOXYGEN) */
void IEC61508_resetClockTest(uint32_t ratio, uint32_t tolerance)
{
  /* reset counter */
  if (ratio == 0)
  {
    ClockTest.timerCounter = 0;
    ClockTest.baseTicks    = 0;
    ClockTest.result       = IEC61508_testInProgress + 1;
  }
  else
  {
    ClockTest.Ratio        = ratio;
    ClockTest.Tolerance    = tolerance;
    ClockTest.timerCounter = 0;
    ClockTest.baseTicks    = 0;
  }
}

/*!
 */
/*! @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#if defined (__ICCARM__)
    #pragma section="IEC61508_code"
#else
__attribute__((section(".IEC61508_code")))
#endif
/*! @endcond (DO_NOT_INCLUDE_WITH_DOXYGEN) */
void IEC61508_initClockTest(uint32_t ratio, uint32_t tolerance)
{
  /* setup parameter */
  IEC61508_resetClockTest(ratio, tolerance);

  //����TIM����
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  //����TIM1ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_DeInit(TIM2);
  //�����ڲ�ʱ�Ӹ�TIM2�ṩʱ��Դ
  TIM_InternalClockConfig(TIM2);
  TIM_TimeBaseStructure.TIM_Period=2000;        //�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) ARR��ֵ
  //�ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
  TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);    //ʱ��Ԥ��Ƶ��   ���磺ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1)
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);       //�������жϱ�־
  //TIM_PrescalerConfig(TIM2,0x8C9F,TIM_PSCReloadMode_Immediate); //ʱ�ӷ�Ƶϵ��36000�����Զ�ʱ��ʱ��Ϊ2K
  TIM_ARRPreloadConfig(TIM2, DISABLE);       //��ֹARRԤװ�ػ�����
  //����TIM2���ж�
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

  NVIC_InitTypeDef NVIC_InitStructure;
  //ѡ���жϷ���1
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //ѡ��TIM2���ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
  //��ռʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  //��Ӧʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //ʹ���ж�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM2, ENABLE);           //����ʱ��
  
  
#if (USE_SYSTICK == 1)
/*! @cond USE_SYSTICK */

  /* Setup SysTick Timer for 10 msec interrupts  */
  /* set reload register */
  /* set Priority for Cortex-M3 System Interrupts */
  /* Load the SysTick Counter Value */
  /* Enable SysTick IRQ and SysTick Timer */

/*! @endcond */
#elif (USE_TIMER0)
/*! @cond USE_TIMER0 */
/* start timer0 */
/* Enable Timer Clock */
  /* Configure TIMER from HFPERCLK */
  /* 32K ticks interrupts */
  /* Clear pending interrupt */
  /* enable interrupt */
/*! @endcond */
#elif (USE_TIMER1)
/*! @cond USE_TIMER1 */
/* start timer1 */
/* TODO : implement timer1 setup here */
/*! @endcond */
#endif
}

void RCC_cfg()
{
  //�������״̬����
  ErrorStatus HSEStartUpStatus;
  //��RCC�Ĵ�����������ΪĬ��ֵ
  RCC_DeInit();
  //���ⲿ����ʱ�Ӿ���
  RCC_HSEConfig(RCC_HSE_ON);
  //�ȴ��ⲿ����ʱ�Ӿ�����
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    //����AHBʱ��(HCLK)Ϊϵͳʱ��
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    //���ø���AHBʱ��(APB2)ΪHCLKʱ��
    RCC_PCLK2Config(RCC_HCLK_Div1);
    //���õ���AHBʱ��(APB1)ΪHCLK��2��Ƶ
    RCC_PCLK1Config(RCC_HCLK_Div2);
    //����FLASH������ʱ
    FLASH_SetLatency(FLASH_Latency_2);
    //ʹ��Ԥȡָ����
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    //����PLLʱ�ӣ�ΪHSE��9��Ƶ 8MHz * 9 = 72MHz
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    //ʹ��PLL
    RCC_PLLCmd(ENABLE);
    //�ȴ�PLL׼������
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    //����PLLΪϵͳʱ��Դ
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    //�ж�PLL�Ƿ���ϵͳʱ��
    while(RCC_GetSYSCLKSource() != 0x08);
  }
   //����TIM2��ʱ��
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  //����GPIO��ʱ��
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}

void RCC_HSI_Cfg()
{
  RCC_DeInit();  //������RCC�Ĵ�������Ϊȱʡֵ
  RCC_HSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY)==RESET);  //�ȴ�HSI����
  
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  RCC_PCLK2Config(RCC_HCLK_Div1);
  RCC_PCLK1Config(RCC_HCLK_Div2);
  
  FLASH_SetLatency(FLASH_Latency_2);
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  while(RCC_GetSYSCLKSource() != 0x00);  //0x00: HSI ��Ϊϵͳʱ��

}
/*!
 * This function represents the part of the IEC61508 Class B clock test which has to be executed within the main loop.
 * It checks several thing:
 * @arg If the clock test timer interrupts are triggered
 * @arg If the clock test RTC interrupt is triggered
 * @arg or, If in any of the two interrupts an error was detected...
 *
 * This function must be called once inside the main loop.
 *
 * @attention For this function, it is necessary to estimate the count how often this function could be called. This is important to find
 * valid threshold values, which are used to test timer and RTC interrupt occurrence.
 *
 * @return passed or failed. See \link #testResult_enum testResult_t \endlink.
 */
/*! @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#if (defined (__ICCARM__))
    #pragma section="IEC61508_code"
#else
__attribute__((section(".IEC61508_code")))
#endif
/*! @endcond (DO_NOT_INCLUDE_WITH_DOXYGEN) */
type_testResult_t IEC61508_Clocktest_PollHandler(void)
{
  type_testResult_t result = IEC61508_testFailed;
  uint32_t     absolutUP, absolutLOW, intermediate;
  ClockTest.result = IEC61508_testInProgress;

  /* wait on RTC flag if running */
    //ClockTest.baseTicks++;
  /* calculate tolerance band extremes */
  intermediate = ClockTest.Ratio * ClockTest.Tolerance / 100;
  if (intermediate > (0x80000000 - 1))
  {
    ClockTest.result = IEC61508_testFailed;
    return(result);
  }
  else
  {
    /* absolutLOW must not be negative */
    if (ClockTest.Ratio >= intermediate)
    {
      absolutLOW = ClockTest.Ratio - intermediate;
    }
    else
    {
      absolutLOW = 0;
    }
    absolutUP = intermediate + ClockTest.Ratio;
  }
  /* calculate average counter/baseTicks */
  intermediate = ClockTest.timerCounter / ClockTest.baseTicks;
  if ((intermediate > absolutLOW) && (intermediate < absolutUP))
  {
    result = IEC61508_testPassed;
    /* reset the counters */
    IEC61508_resetClockTest(0, 0);
    /* stop Interrupts */
#if (USE_SYSTICK == 1)
/*! @cond USE_SYSTICK */
    /* Disable SysTick IRQ and SysTick Timer */

  /* Disable the SysTick Counter ��ֹSysTick ������*/
  //SysTick_CounterCmd(SysTick_Counter_Disable);
  /* Clear the SysTick Counter ����SysTick ������*/
  //SysTick_CounterCmd(SysTick_Counter_Clear);
    
/*! @endcond */
#elif (USE_TIMER0)
/*! @cond USE_TIMER0 */
    //if (TIMER0->CMD != TIMER_CMD_START)
    //{
    //  ClockTest.result = IEC61508_testFailed;
    //  return(IEC61508_testFailed);
    //}
    ClockTest.result = IEC61508_testPassed;
    /* disable interrupt */
    //NVIC->ICER[((uint32_t)(TIMER0_IRQn) >> 5)] = (1 << ((uint32_t)(TIMER0_IRQn) & 0x1F));
    /* Clear pending interrupt */
    //NVIC->ICPR[((uint32_t)(TIMER0_IRQn) >> 5)] = (1 << ((uint32_t)(TIMER0_IRQn) & 0x1F));
    //TIMER0->IFC                                = TIMER_IEN_OF;
    //TIMER0->IEN                               &= ~TIMER_IEN_OF;
/*! @endcond */
#elif (USE_TIMER1)
/*! @cond USE_TIMER1 */
/* TODO : implement timer1 stop here */
/*! @endcond */
#endif
  }

  return(result);
}

/*!
 * This function is intended to be called once inside the custom RTC interrupt service handler.
 * It can be used as service handler by itself. The result can be evaluated by checking the
 * counters values are reseted after this.
 *
 * This function tests the timer time frame and in this case the CPU frequency.
 * Also, this function checks if the main loop function was called.
 */
/*! @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#if defined (__ICCARM__)
    #pragma section="IEC61508_code"
#else
__attribute__((section(".IEC61508_code")))
#endif
/*! @endcond (DO_NOT_INCLUDE_WITH_DOXYGEN) */
void IEC61508_Clocktest_RTCHandler(void)
{
  uint32_t absolutUP, absolutLOW, intermediate;
  ClockTest.result = IEC61508_testInProgress;
  /* TODO: call this vector if RTC is used with interrupt */
  /* check comp0 interrupt */
  //if (RTC->IF & RTC_IF_COMP0)
  //{
    ClockTest.baseTicks++;
  //  RTC->IFC = RTC_IFC_COMP0;               /* clear the interrupt flag */
    /* calculate tolerance band extremes */
    intermediate = ClockTest.Ratio * ClockTest.Tolerance / 100;

      /* absolutLOW must not be negative */
      if (ClockTest.Ratio >= intermediate)
      {
        absolutLOW = ClockTest.Ratio - intermediate;
      }
      else
      {
        absolutLOW = 0;
      }
      absolutUP = intermediate + ClockTest.Ratio;

      /* calculate average counter/baseTicks */
    intermediate = ClockTest.timerCounter / ClockTest.baseTicks;
      if ((intermediate > absolutLOW) && (intermediate < absolutUP))
      {
        /* reset the counters */
        IEC61508_resetClockTest(0, 0);
        /* stop Interrupts */
#if (USE_SYSTICK == 1)
/*! @cond USE_SYSTICK */
        /* Disable SysTick IRQ and SysTick Timer */
        SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
/*! @endcond */
#elif (USE_TIMER0)
/*! @cond USE_TIMER0 */
      if (TIMER0->CMD != TIMER_CMD_START)
      {
        ClockTest.result = IEC61508_testFailed;
        return;
      }
      ClockTest.result = IEC61508_testPassed;
      /* disable interrupt */
      NVIC->ICER[((uint32_t)(TIMER0_IRQn) >> 5)] = (1 << ((uint32_t)(TIMER0_IRQn) & 0x1F));
      /* Clear pending interrupt */
      NVIC->ICPR[((uint32_t)(TIMER0_IRQn) >> 5)] = (1 << ((uint32_t)(TIMER0_IRQn) & 0x1F));
      TIMER0->IFC                                = TIMER_IEN_OF;
      TIMER0->IEN                               &= ~TIMER_IEN_OF;
/*! @endcond */
#elif (USE_TIMER1)
/*! @cond USE_TIMER1 */
/* TODO : implement timer1 stop here */
/*! @endcond */
#endif
    //}
  }
}


/*!
 * @}
 */

/************************************** EOF *********************************/
