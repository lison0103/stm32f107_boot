
#include "esc.h"

#define   INPUT_AUX_BRAKE         (INPUT_PORT4&0x02)
#define   PARA_AUX_BRAKE_DIS      ParaBuff[7]
#define   PARA_STAR_RUN_TS        ParaBuff[8]

#define   CURRENT_MULTI_SPEED     EscRTBuff[29] //note

#ifdef ECT_01_MASTER 
#define AUTO_CMD_FLAG_DIFFER_CNT  ((u16)20)
#else
#define AUTO_CMD_FLAG_DIFFER_CNT  ((u16)50)
#endif

u16 RunStart_Check_Tms=0,RunStart_Star_Tms=0;

u16 smart_start_run_tms=0;

/******************************************************************************* 1 Կ��
*******************************************************************************/
void key_run_detect(void)
{
  static u16 key_on_tms=0,key_up_tms=0,key_down_tms=0;
  static u16 auto_cmd_differ_tms=0;

		
  if(CMD_FLAG6 & 0x03) //0x33
  {
    if(key_on_tms<5000) key_on_tms++;
	
		if(CMD_FLAG6 & 0x01) 
		{
			if(key_up_tms<5000) key_up_tms++;
		}		

		if(CMD_FLAG6 & 0x02) 
		{
			if(key_down_tms<5000) key_down_tms++;
		}		 
  }

	if(((CMD_FLAG1&0x03)==0x01) && (CMD_OMC_FLAG1&0x01))
	{
		//����ǰ���
		if((key_on_tms==150) && ((key_up_tms==150) || (key_down_tms==150)))
		{
			RunStart_Check_Tms = 1;
		}	
					
    //�����Զ����ƣ�ͬʱͣ��״̬
		if((!(CMD_FLAG6 & 0x03)) && (key_on_tms>=150) && (key_on_tms<600)) 
		{
			if(key_on_tms == key_up_tms)  //����
			{
				CMD_FLAG1 |= (1<<2)|(1<<1);
			}
			else if(key_on_tms == key_down_tms)   //����
			{
				CMD_FLAG1 |= (1<<3)|(1<<1); 
			}
	
			//�Զ����У�����ͣ�ݸ�����բ��ʱ
			#ifdef ECT_01_MASTER
			CMD_FLAG7 ^= CF7_Aux_Delay_State;
			#endif			
			CMD_FLAG7 |= 0x20;
		}	
	}			
	
  if(!(CMD_FLAG6 & 0x03)) key_on_tms = 0;	
	if(!(CMD_FLAG6 & 0x01)) key_up_tms=0;
	if(!(CMD_FLAG6 & 0x02)) key_down_tms=0;
	
  //----------------------------------------------------------------------------
  #ifdef REMOTE_RUN_EN
  if(((CMD_FLAG1&0x03) == 0x01) && (key_on_tms==200))
  {
    if(CMD_FLAG6 & 0x10) //Զ������
    {
      CMD_FLAG1 |= (1<<2)|(1<<1);
    }
    else if(CMD_FLAG6 & 0x20) //Զ������
    {
      CMD_FLAG1 |= (1<<3)|(1<<1);
    }
  }
  #endif

  //˫MCU����
  if(CMD_FLAG1&0x0E)
  {
    if((CMD_FLAG1&0x0f) != (CMD_OMC_FLAG1&0x0f))
    {
      if(auto_cmd_differ_tms > AUTO_CMD_FLAG_DIFFER_CNT)
      {
        CMD_FLAG1 &= 0x01;      
      } 
      else
      {
        auto_cmd_differ_tms++;      
      }  
    }
    else
    {
      auto_cmd_differ_tms = 0;
    }  
  }  
  else
  {
    auto_cmd_differ_tms = 0;
  }  
     
  //----------------------------------------------------------------------------
  if(!(CMD_FLAG1&0x01)) CMD_FLAG1 = 0; //������б�־
  if(!(CMD_OMC_FLAG1&0x01)) CMD_FLAG1 &= 0x01; //������б�־
}

/******************************************************************************* 2 ����
*******************************************************************************/
void inspect_run_detect(void)
{
  if(CMD_FLAG3&0x03)
  {
    CMD_FLAG3 &= ~0x0c; //����������б�־

    if(CMD_FLAG6 & 0x04) //��������
    {
      CMD_FLAG3 |= (1<<2);
    }
    else if(CMD_FLAG6 & 0x08) //��������
    {
      CMD_FLAG3 |= (1<<3);
    }
  }
  else
  {
    CMD_FLAG3 &= ~0x0c; //����������б�־
  }
}

/******************************************************************************* 4 ���м��
*******************************************************************************/
void start_check(void)
{
  static u16 start_check_error_cnt=0;

  if(RunStart_Check_Tms<5) start_check_error_cnt=0;

  CMD_OUTPUT_PORT &= ~PORT_UDC;
  CMD_OUTPUT_PORT &= ~PORT_DYC;
  CMD_OUTPUT_PORT &= ~PORT_UP;
  CMD_OUTPUT_PORT &= ~PORT_DOWN;
  CMD_OUTPUT_PORT &= ~PORT_D;
  CMD_OUTPUT_PORT &= ~PORT_Y;

  if(RunStart_Check_Tms<80) //������
  {
    CMD_OUTPUT_PORT |= PORT_UDC;
    CMD_OUTPUT_PORT |= PORT_DYC;
  }
  else if(RunStart_Check_Tms>100 && RunStart_Check_Tms<170) //����&����
  {
    CMD_OUTPUT_PORT |= PORT_UP;
    CMD_OUTPUT_PORT |= PORT_Y;
  }
  else if(RunStart_Check_Tms>190 && RunStart_Check_Tms<260) //����&������
  {
    CMD_OUTPUT_PORT |= PORT_DOWN;
    CMD_OUTPUT_PORT |= PORT_D;
  }

//------------------------------------------------------------------------- ���
  //CONTACT_MASK
  if((CONTACT_FEEDBACK))
  {
    start_check_error_cnt++;

    if(start_check_error_cnt>10)
    {
      EN_ERROR5 |= 0x08;
    }
  }
}

/******************************************************************************* 5
*******************************************************************************/
//------------------------------------------------------------------------------ //������
void dy_running(void)
{
#ifdef ECT_01_MASTER 	
  static u8 cmd_flag8_old=0,dy_change_tms=0;  
  u16 i;
#endif
	
  if(RunStart_Star_Tms<10000) //����ʱ��
  {
    RunStart_Star_Tms++;
  }

  CMD_OUTPUT_PORT &= ~PORT_UDC;
  CMD_OUTPUT_PORT &= ~PORT_DYC;
  CMD_OUTPUT_PORT &= ~PORT_UP;
  CMD_OUTPUT_PORT &= ~PORT_DOWN;
  CMD_OUTPUT_PORT &= ~PORT_Y;
  CMD_OUTPUT_PORT &= ~PORT_D;

  if(CMD_FLAG1&0x20) //��Ъ
  {
    RunStart_Star_Tms=0;
  }
  else //�Ǽ�Ъ                                                          
  {
    CMD_OUTPUT_PORT |= PORT_UDC;
    CMD_OUTPUT_PORT |= PORT_DYC;

    if(CMD_FLAG1&0x04) //����
    {
      CMD_OUTPUT_PORT |= PORT_UP;
    }
    else if(CMD_FLAG1&0x08) //����
    {
      CMD_OUTPUT_PORT |= PORT_DOWN;
    }
		
#ifdef ECT_01_MASTER 	
    if((CMD_FLAG1&0x10) && (PARA_STAR_RUN_TS)) //����,ʵΪ��������, ���������л�����
    {
      CMD_FLAG8 |= CF8_DY_State_Star;

      i = PARA_STAR_RUN_TS;
      i = i*200;
      if(RunStart_Star_Tms > i) RunStart_Star_Tms = i-100; // 
    }
    else //����,��������,����������
    {
      i = PARA_STAR_RUN_TS;
      i = i*200;

      if(PARA_STAR_RUN_TS && (RunStart_Star_Tms<i))  // >= 3
      {
        CMD_FLAG8 |= CF8_DY_State_Star;
      }
      else //������(ֱ������)����
      {
        CMD_FLAG8 |= CF8_DY_State_Delta;
      }
    }
#else
		CMD_FLAG8 |= CF8_DY_State_Star|CF8_DY_State_Delta;
#endif 		
  }

//------------------------------------------------------------------------------
#ifdef ECT_01_MASTER 	
  //���������л�,�Ѿ�����100ms����
  if((cmd_flag8_old&0x30) != (CMD_FLAG8&0x30))
  {
    dy_change_tms = 0;
  }

  cmd_flag8_old = CMD_FLAG8; 

  if(dy_change_tms<16)
  {
    dy_change_tms++;
    CMD_FLAG8 &= ~(CF8_DY_State_Delta | CF8_DY_State_Star);
  }
#endif
	
//------------------------------------------------------------------------------
  if((!PARA_AUX_BRAKE_DIS) && (!INPUT_AUX_BRAKE)) //������բδ��
  {
    RunStart_Star_Tms=0;

    CMD_OUTPUT_PORT &= ~PORT_Y;
    CMD_OUTPUT_PORT &= ~PORT_D;
    CMD_OUTPUT_PORT &= ~PORT_DYC;
    
    CMD_FLAG8 &= ~0x30;
  }

	if(CMD_FLAG8 & CF8_DY_State_Star) CMD_OUTPUT_PORT |= PORT_Y;
	if(CMD_FLAG8 & CF8_DY_State_Delta) CMD_OUTPUT_PORT |= PORT_D;
}

//------------------------------------------------------------------------------ //smart vf
void smart_vf_running(void)
{
/*  
  static u8 smart_ctr_flag=0,cmd_old=0;
  static u16 smart_change_tms=0;
  
  CMD_OUTPUT_PORT &= ~PORT_UDC;
  CMD_OUTPUT_PORT &= ~PORT_DYC;
  CMD_OUTPUT_PORT &= ~PORT_UP;
  CMD_OUTPUT_PORT &= ~PORT_DOWN;
  CMD_OUTPUT_PORT &= ~PORT_Y;
  CMD_OUTPUT_PORT &= ~PORT_D;
  RELAY_OUTPUT_Q5 &= ~0x07;
  
  if(smart_start_run_tms<10000) smart_start_run_tms++; 
  if(smart_start_run_tms==1) smart_ctr_flag = 0;
  
  if(CMD_FLAG1&0x20) //��Ъ
  {
    smart_start_run_tms=0;
    smart_change_tms = 0;
  }
  else //if(!(CMD_FLAG1&0x20)) //�Ǽ�Ъ
  { 
    CMD_OUTPUT_PORT |= PORT_UDC;
//    CMD_OUTPUT_PORT |= PORT_DYC;
    
    if(CMD_FLAG1&0x04) //����
    {
      CMD_OUTPUT_PORT |= PORT_UP;
    }
    else if(CMD_FLAG1&0x08)
    {
      CMD_OUTPUT_PORT |= PORT_DOWN;
    }
   
    //֮ǰ����ֹͣ
    if(smart_ctr_flag==0) 
    {
      RELAY_OUTPUT_Q5 |= 0x04;
      
      if(CMD_FLAG1&0x10) //����
      {
        //��Ƶ���� 
        smart_ctr_flag = 1;
        PGAOUTDATA[0] |= 0x01;
      } 
      else
      {
        //��Ƶ���� 
        smart_ctr_flag = 2;
        PGAOUTDATA[0] |= 0x02;
      }  
      smart_change_tms = 1;
    } 
    else 
    { 
      if((CMD_FLAG1&0x10) && (!(cmd_old&0x10))) //ת����
      {
        smart_change_tms = 1;
        smart_ctr_flag = 3;
      }
      else if((!(CMD_FLAG1&0x10)) && (cmd_old&0x10) || (smart_ctr_flag==2)) //ת����
      {
        smart_change_tms = 1;
        smart_ctr_flag = 22;
      }  

      if(CMD_FLAG1&0x10) //����
      {
        PGAOUTDATA[0] |= 0x01;
        
        if(smart_ctr_flag==3)
        {
          if(smart_change_tms<1000) smart_change_tms++;
          if((EscRTBuff[106]<=50) && (smart_change_tms>10)) 
          {
            smart_ctr_flag = 1;
          }  
        } 
        else
        { 
          RELAY_OUTPUT_Q5 |= 0x04;
          
        }  
      } 
      else //����
      {
        if(smart_ctr_flag==22)
        {
          RELAY_OUTPUT_Q5 |= 0x04;
          PGAOUTDATA[0] |= 0x02;
          
          if(smart_change_tms++>1000)
          {
            smart_ctr_flag=5;
            smart_change_tms = 0;
          }  
        } 
        else if(smart_ctr_flag==5)
        {
          if(smart_change_tms++>10)
          {
            smart_ctr_flag = 6;
          }  
        }  
        else  
        {
          RELAY_OUTPUT_Q5 |= 0x01;
          PGAOUTDATA[0] &= ~0x02;
        }  
      }  
    }
    
    if(ESC_CONTROL_MODE==4)
    {
      CMD_OUTPUT_PORT |= PORT_DYC;
      CMD_OUTPUT_PORT |= PORT_D;  
    }  
  }  
  
//------------------------------------------------------------------------------
  cmd_old = CMD_FLAG1;
*/  
}

//------------------------------------------------------------------------------ //��Ƶ����
void vf_running(void)
{
  CMD_OUTPUT_PORT &= ~PORT_UDC;
  CMD_OUTPUT_PORT &= ~PORT_DYC;
  CMD_OUTPUT_PORT &= ~PORT_UP;
  CMD_OUTPUT_PORT &= ~PORT_DOWN;
  CMD_OUTPUT_PORT &= ~PORT_Y;
  CMD_OUTPUT_PORT &= ~PORT_D;

  if(!(CMD_FLAG1&0x20)) //�Ǽ�Ъ
  {
    CMD_OUTPUT_PORT |= PORT_UDC;
    CMD_OUTPUT_PORT |= PORT_DYC;

    if(CMD_FLAG1&0x04) //����
    {
      CMD_OUTPUT_PORT |= PORT_UP;
    }
    else if(CMD_FLAG1&0x08)
    {
      CMD_OUTPUT_PORT |= PORT_DOWN;
    }

    if(PARA_CONTROL_MODE>1) //���Ͽ���
    {
			CMD_FLAG8 |= CF8_DY_State_Delta;
    }
    else //����Ƶ
    {
			CMD_FLAG8 |= CF8_DY_State_Star | CF8_DY_State_Delta;
    }

    if(CMD_FLAG1&0x10) //����
    {
      PGAOUTDATA[0] |= 0x02;
    }
    else //����
    {
      if(ParaBuff[34] && CURRENT_MULTI_SPEED) //��ʱ��������� note
      {
        PGAOUTDATA[0] |= ((CURRENT_MULTI_SPEED-1) & 0x07);
      }
      else
      {
        //���ӿ��ƿ���2�����ߣ�Զ��ѡ�����2
        if(CMD_FLAG8 & CF8_Auto_Fast_Mode2) // ((pcPgaInputBuff[13]&0x01) ||  ) //����1
        {
          PGAOUTDATA[0] |= 0x03;
        }
        else //���٣���׼
        {
          PGAOUTDATA[0] |= 0x01;
        }
      }
    }
  }
  
  //----------------------------------------------------------------------------
  if((!PARA_AUX_BRAKE_DIS) && (!INPUT_AUX_BRAKE)) //������բδ��
  {
    CMD_OUTPUT_PORT &= ~PORT_DYC;
    CMD_OUTPUT_PORT &= ~PORT_Y;
    CMD_OUTPUT_PORT &= ~PORT_D;
    
		CMD_FLAG8 &= ~(CF8_DY_State_Delta | CF8_DY_State_Star);
  }

	if(CMD_FLAG8 & CF8_DY_State_Star) CMD_OUTPUT_PORT |= PORT_Y;
	if(CMD_FLAG8 & CF8_DY_State_Delta) CMD_OUTPUT_PORT |= PORT_D;
}

/*******************************************************************************
*******************************************************************************/
//------------------------------------------------------------------------------ //����
void inspect_run(void)
{
#ifdef ECT_01_MASTER  
  static u8 CMD_FLAG8_old=0,dy_change_tms=0;
  u16 i=0;
#endif
  if(RunStart_Star_Tms<10000)
  {
    RunStart_Star_Tms++;
  }

  CMD_OUTPUT_PORT &= ~PORT_UDC;
  CMD_OUTPUT_PORT &= ~PORT_DYC;
  CMD_OUTPUT_PORT &= ~PORT_UP;
  CMD_OUTPUT_PORT &= ~PORT_DOWN;
  CMD_OUTPUT_PORT &= ~PORT_Y;
  CMD_OUTPUT_PORT &= ~PORT_D;

  CMD_OUTPUT_PORT |= PORT_UDC;
  CMD_OUTPUT_PORT |= PORT_DYC;

  if(CMD_FLAG3&0x04) //��������
  {
    CMD_OUTPUT_PORT |= PORT_UP;
  }
  else if(CMD_FLAG3&0x08) //��������
  {
    CMD_OUTPUT_PORT |= PORT_DOWN;
  }

  if(CMD_FLAG2&0x01) //�����ǿ���ģʽ
  {
#ifdef ECT_01_MASTER     
    if(PARA_STAR_RUN_TS)
    {
      i = PARA_STAR_RUN_TS;
      i = i*200;

      if(RunStart_Star_Tms<i)
      {
        CMD_FLAG8 |= CF8_DY_State_Star;
      }
      else //if(auto_star_run_tms>(i+30))
      {
        CMD_FLAG8 |= CF8_DY_State_Delta;
      }
    }
    else
    {
      CMD_FLAG8 |= CF8_DY_State_Delta;
    }

    //���������л�,�Ѿ�����100ms����
    if((CMD_FLAG8_old&0x30) != (CMD_FLAG8&0x30))
    {
      dy_change_tms = 0;
    }

    CMD_FLAG8_old = CMD_FLAG8;

    if(dy_change_tms<16)
    {
      dy_change_tms++;
      CMD_FLAG8 &= ~(CF8_DY_State_Delta | CF8_DY_State_Star);
    }
#else
  CMD_FLAG8 |= CF8_DY_State_Delta | CF8_DY_State_Star;      
#endif
  }
  else if(CMD_FLAG2&0x02) //��Ƶ����ģʽ
  {
    if(PARA_CONTROL_MODE>1) //���Ͽ��� 2014.6.7
    {
      CMD_FLAG8 |= CF8_DY_State_Delta;
    }
    else //����Ƶ
    {
      CMD_FLAG8 |= CF8_DY_State_Delta | CF8_DY_State_Star;    
    }
  }
  else
  {
    CMD_OUTPUT_PORT &= ~PORT_UDC;
    CMD_OUTPUT_PORT &= ~PORT_DYC;
    CMD_OUTPUT_PORT &= ~PORT_UP;
    CMD_OUTPUT_PORT &= ~PORT_DOWN;
    CMD_OUTPUT_PORT &= ~PORT_Y;
    CMD_OUTPUT_PORT &= ~PORT_D;
  }

//------------------------------------------------------------------------------
  if((!PARA_AUX_BRAKE_DIS) && (!INPUT_AUX_BRAKE)) //������բδ��
  {
    RunStart_Star_Tms=0;

    CMD_OUTPUT_PORT &= ~PORT_DYC;

		CMD_FLAG8 &= ~(CF8_DY_State_Delta | CF8_DY_State_Star);
  }
	
	if(CMD_FLAG8 & CF8_DY_State_Star) CMD_OUTPUT_PORT |= PORT_Y;
	if(CMD_FLAG8 & CF8_DY_State_Delta) CMD_OUTPUT_PORT |= PORT_D;
}

/******************************************************************************* 8
*******************************************************************************/
void aux_brake_process(void)
{
  static u16 aux_break_off_tms=0,aux_break_on_cnt=0;//
  u16  aux_break_off_delay_tms=0;

  CMD_FLAG7 &= ~CF7_Aux_CmdOff_Enable;	 //���Э���Ͽ�������־

  aux_break_off_delay_tms = ParaBuff[42];
  if(aux_break_off_delay_tms>6 || aux_break_off_delay_tms<3) aux_break_off_delay_tms = 5;
  aux_break_off_delay_tms = 200*aux_break_off_delay_tms;

  if(aux_break_off_tms<8000) aux_break_off_tms++;

	//��������
  if((CMD_FLAG7 & CF7_Aux_Output_Enable) && ((CMD_OUTPUT_PORT & PORT_UP)||(CMD_OUTPUT_PORT & PORT_DOWN))) 
  {
    CMD_OUTPUT_PORT |= PORT_AUX;
    aux_break_off_tms = 0;
  }

  if((EN_ERROR1&0x01) || (EN_ERROR6&0x17) || (EN_ERROR8&0x02)) //�����Ͽ�������բ
  {
    CMD_OUTPUT_PORT &= ~PORT_AUX;
    CMD_FLAG7 |= CF7_Aux_CmdOff_Enable;
  }
  else if(CMD_OMC_FLAG7 & CF7_Aux_CmdOff_Enable) //Эͬ�Ͽ�������բ
  {
    CMD_OUTPUT_PORT &= ~PORT_AUX;      
  }
  else
  {
#ifdef ECT_01_MASTER
    if((!(CMD_FLAG7 & CF7_Aux_Delay_State)) && (!(CMD_FLAG1&0x20)) && (!(CMD_FLAG3&0x01)))
#else
    if((CMD_OMC_FLAG7 & CF7_Aux_Delay_State) && (!(CMD_FLAG1&0x20)) && (!(CMD_FLAG3&0x01)))
#endif
    {
      aux_break_off_delay_tms += 1200;
    }

    if(aux_break_off_tms>aux_break_off_delay_tms) //�Ͽ�������բ
    {
      CMD_OUTPUT_PORT &= ~PORT_AUX;
    }
  }

  if(CMD_OUTPUT_PORT & PORT_AUX)
  {
    if(aux_break_on_cnt<20)
    {
      aux_break_on_cnt++;
      CMD_OUTPUT_PORT &= ~PORT_AUX;
    }
  }
  else
  {
    aux_break_on_cnt = 0;
  }
}

/*******************************************************************************
*******************************************************************************/
void run_process(void)
{
	
  if(System_Sequence != 0x1f3f) System_Check_Fault_Flag |= 0x0020;
  
  //
  
  CMD_FLAG7 &= ~CF7_Aux_Output_Enable;

//------------------------------------------------------------------------------
  //���������м̵���
  PGAOUTDATA[0] &= ~0x07;
  //Modbus7xxBuff[28] = 0;

  //�����ǽӴ��������־
  CMD_FLAG8 &= ~(CF8_DY_State_Delta | CF8_DY_State_Star);

//------------------------------------------------------------------------------
  key_run_detect();                         // 2 ��ȡԿ���������ź�

  inspect_run_detect();                     // 3 ��ȡ�����������ź�

	//��������
	if(((CMD_FLAG1&0x01)==0x01) && (RunStart_Check_Tms))
	{
		if(RunStart_Check_Tms<300)              //1.5s
    {
      RunStart_Check_Tms++;
      start_check();                        // 4 �������
    }
		else
		{
			RunStart_Check_Tms = 0;       
		}	
	}	
  else if(((CMD_FLAG1&0x03)==0x03) && (CMD_FLAG1&0x0C))
  {
		CMD_FLAG7 |= CF7_Aux_Output_Enable;    

    if((CMD_FLAG2&0x03) == 0x01)                    // 5 ������
    {
      dy_running(); 
    }
    else if((CMD_FLAG2&0x03) == 0x02)               // 6 ��Ƶ
    {
      vf_running(); 
    }
    else if((CMD_FLAG2&0x03) == 0x03)               //  
    {
      smart_vf_running(); 
    }  
  }
  else if(((CMD_FLAG3&0x03)==0x03) && (CMD_FLAG3&0x0C))
  {
    CMD_FLAG7 |= CF7_Aux_Output_Enable;
		
    inspect_run();                          //  ��������
  }
  else  //��(����)���У�ͣ��
  {
    RunStart_Check_Tms = 0;    
    RunStart_Star_Tms = 0;  
    smart_start_run_tms = 0;

    CMD_OUTPUT_PORT &= ~PORT_UDC;
    CMD_OUTPUT_PORT &= ~PORT_DYC;

    CMD_OUTPUT_PORT &= ~PORT_UP;
    CMD_OUTPUT_PORT &= ~PORT_Y;

    CMD_OUTPUT_PORT &= ~PORT_DOWN;
    CMD_OUTPUT_PORT &= ~PORT_D;
    
    //Smart mode����Ƶ/��Ƶ����Ӵ����Ͽ�
    if(ESC_CONTROL_MODE>2)
    {  
      RELAY_OUTPUT_Q5 &= ~0x07;
    }
  }

  aux_brake_process();                      // 8 ������բ����

  //
  
  System_Sequence |= 0x2000;
}

/*******************************************************************************
*******************************************************************************/