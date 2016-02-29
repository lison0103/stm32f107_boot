/**
 * Company	: ThyssenKrupp PDC
 * @file	: wdt_test.c
 * @purpose     :
 * @brief	: c based WDT test 
 *                  POST and BIST WDT test for IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 28-April-2014
 * @author	: Paul
*/

#include "stm32f10x.h"
//#include "platform_config.h"
#include "wdt_test.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"


//#include "stm32f10x_exti.h"
//#include "stm32f10x_wwdg.h"
//#include "stm32f10x_flash.h"

/*! @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#if defined (__ICCARM__)
    #pragma section="IEC61508_code"
#else
__attribute__((section(".IEC61508_code")))
#endif
/*! @endcond (DO_NOT_INCLUDE_WITH_DOXYGEN) */
type_testResult_t IEC61508_initWDT(void)
{
  type_testResult_t result = IEC61508_testFailed;
  //�������Ź���ʼ��
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   //�����Ĵ�����д
  IWDG_SetPrescaler(IWDG_Prescaler_32);           //40Kʱ��32��Ƶ
  IWDG_SetReload(349);                            //��������ֵ
  IWDG_ReloadCounter();                           //����������
  IWDG_Enable();                                  //�������Ź�
  return result;
}

/*! @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#if defined (__ICCARM__)
    #pragma section="IEC61508_code"
#else
__attribute__((section(".IEC61508_code")))
#endif
/*! @endcond (DO_NOT_INCLUDE_WITH_DOXYGEN) */
void IEC61508_Refresh_WDT(void)
{
  /* If a previous clearing is being synchronized to LF domain, then there
   * is no point in waiting for it to complete before clearing over again.
   * This avoids stalling the core in the typical use case where some idle loop
   * keeps clearing the watchdog. */
  //if (SyncWDOGCMD())
  //{
  //  return;
  //}

  //WDOG->CMD = WDOG_CMD_CLEAR;
}

/*! @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#if defined (__ICCARM__)
    #pragma section="IEC61508_code"
#else
__attribute__((section(".IEC61508_code")))
#endif
/*! @endcond (DO_NOT_INCLUDE_WITH_DOXYGEN) */
type_testResult_t IEC61508_Force_WDT_Reset(void)
{
  type_testResult_t result = IEC61508_testFailed;
  /* no debugger allowed here */
  result = IEC61508_initWDT();

  return result;
}

/***************************/
void WWDG_Configuration(void)
{
    /* �������ڿ��Ź�ʱ�� */
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
//   /* Deinitialize the WWDG registers������ WWDG�Ĵ�������Ϊȱʡֵ  */ 
//   ����ײ㺯�����Կ�������ʼ���رմ��ڿ��Ź�ʱ��
//   WWDG_DeInit();
   /* Set WWDG prescaler to 8 ���� WWDG Ԥ��Ƶֵ ��PCLK1/4096/8=1098.6 */ 
   WWDG_SetPrescaler(WWDG_Prescaler_8);
   /* Set WWDG window value to 0x41ָ���Ĵ���ֵ���ò���ȡֵ������ 0x40 �� 0x7F֮�䡣  */ 
   WWDG_SetWindowValue(0x41); 
  /* Enable WWDG and set counter value to 0x7F ʹ�ܴ��ڿ��Ź������ѿ��Ź���������ֵ��Ϊ0x7f*/
    WWDG_Enable(0x7f);
   /* Clear EWI flag ������ڻ����жϱ�־λ */ 
   WWDG_ClearFlag();  
   /* Enable WWDG Early wakeup interrupt ʹ�� WWDG ���ڻ����жϣ�EWI��  */ 
   WWDG_EnableIT();  
}


/*!
 * @}
 */

/************************************** EOF *********************************/
