/*******************************************************************************
* File Name          : timer.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "bsp_iocfg.h"
#include "lsys.h"
#include "stm32f10x_STLlib.h"
#include "stm32f10x_STLclassBvar.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u8 count = 0;
u32 t_count = 0;



/*******************************************************************************
* Function Name  : TIM4_Int_Init
* Description    : 
*                  
* Input          : arr: Automatic reload value
*                  psc: Pre clock divisor
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_Int_Init(u16 arr,u16 psc)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
        
        
        /** TIM2 **/

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����  
        
}


/*******************************************************************************
* Function Name  : TIM3_Int_Init
* Description    : 
*                  
* Input          : arr: Automatic reload value
*                  psc: Pre clock divisor
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_Int_Init(u16 arr,u16 psc)
{
#ifdef GEC_SF_MASTER
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


        /** TIM3 **/

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
#endif
        
      
        
							 
}

/*******************************************************************************
* Function Name  : TIM2_Int_Init
* Description    : 
*                  
* Input          : arr: Automatic reload value
*                  psc: Pre clock divisor
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Int_Init(u16 arr,u16 psc)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
        
        
        /** TIM2 **/

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����  
        
}

void TIM4_IRQHandler(void)   //TIM4�ж�
{
      if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
      {
        
          TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
          
//          PLUSE_OUT = !PLUSE_OUT;
#if 1         
          /* Verify TickCounter integrity */
          if ((TickCounter ^ TickCounterInv) == 0xFFFFFFFFuL)
          {
            TickCounter++;
            TickCounterInv = ~TickCounter;

            if (TickCounter >= SYSTICK_20ms_TB)
            {
                u32 RamTestResult;

              /* Reset timebase counter */
              TickCounter = 0;
              TickCounterInv = 0xFFFFFFFF;

              /* Set Flag read in main loop */
              TimeBaseFlag = 0xAAAAAAAA;
              TimeBaseFlagInv = 0x55555555;

//              if ((CurrentHSEPeriod ^ CurrentHSEPeriodInv) == 0xFFFFFFFFuL)
//              {
//                ISRCtrlFlowCnt += MEASPERIOD_ISR_CALLER;
//                CurrentHSEPeriod = STL_MeasurePeriod();
//                CurrentHSEPeriodInv = ~CurrentHSEPeriod;
//                ISRCtrlFlowCntInv -= MEASPERIOD_ISR_CALLER;
//              }
//              else  /* Class B Error on CurrentHSEPeriod */
//              {
//              #ifdef STL_VERBOSE
//                printf("\n\r Class B Error on CurrentHSEPeriod \n\r");
//              #endif  /* STL_VERBOSE */
//              }

              ISRCtrlFlowCnt += RAM_MARCHC_ISR_CALLER;
              RamTestResult = STL_TranspMarchC();
              ISRCtrlFlowCntInv -= RAM_MARCHC_ISR_CALLER;

        //      ISRCtrlFlowCnt += RAM_MARCHX_ISR_CALLER;
        //      RamTestResult = STL_TranspMarchX();
        //      ISRCtrlFlowCntInv -= RAM_MARCHX_ISR_CALLER;

              switch ( RamTestResult )
              {
                case TEST_RUNNING:
                  break;
                case TEST_OK:
                  #ifdef STL_VERBOSE
                    printf("\n\r Full RAM verified (Run-time)\n\r");
//                    GPIO_WriteBit(GPIOC, GPIO_Pin_7, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7)));
                  #endif  /* STL_VERBOSE */
                  break;
                case TEST_FAILURE:
                case CLASS_B_DATA_FAIL:
                default:
                  #ifdef STL_VERBOSE
                    printf("\n\r >>>>>>>>>>>>>>>>>>>  RAM Error (March C- Run-time check)\n\r");
                  #endif  /* STL_VERBOSE */
                  FailSafePOR();
                  break;
              } /* End of the switch */

              /* Do we reached the end of RAM test? */
              /* Verify 1st ISRCtrlFlowCnt integrity */
              if ((ISRCtrlFlowCnt ^ ISRCtrlFlowCntInv) == 0xFFFFFFFFuL)
              {
                if (RamTestResult == TEST_OK)
                {
                  if (ISRCtrlFlowCnt != RAM_TEST_COMPLETED)
                  {
                  #ifdef STL_VERBOSE
                    printf("\n\r Control Flow error (RAM test) \n\r");
                  #endif  /* STL_VERBOSE */
                  FailSafePOR();
                  }
                  else  /* Full RAM was scanned */
                  {
                    ISRCtrlFlowCnt = 0;
                    ISRCtrlFlowCntInv = 0xFFFFFFFF;
                  }
                } /* End of RAM completed if*/
              } /* End of control flow monitoring */
              else
              {
              #ifdef STL_VERBOSE
                printf("\n\r Control Flow error in ISR \n\r");
              #endif  /* STL_VERBOSE */
              FailSafePOR();
              }
            } /* End of the 20 ms timebase interrupt */
          }
          else  /* Class error on TickCounter */
          {
          #ifdef STL_VERBOSE
            printf("\n\r Class B Error on TickCounter\n\r");
          #endif  /* STL_VERBOSE */
          FailSafePOR();
          }          
          
          
          

#endif
        
      }
}


void TIM3_IRQHandler(void)  
{
      if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
      {
        
          TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 


        
      }
}






/******************************  END OF FILE  *********************************/



