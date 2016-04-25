
#include "esc.h"
#include "includes.h"













/***************************************************************************************************
***************************************************************************************************/
//--------------------------------------------------------------------------------------------------
void run_key_check(void) //���Կ���ź�
{
  if((INPUT_PORT8 & 0x80) || (UPPER_INPUT_PORT2 & 0x08) || (LOWER_INPUT_PORT2 & 0x08)) //����
  {
    CMD_FLAG6 |= 0x01;
  }
  else
  {
    CMD_FLAG6 &= ~0x01;
  }

  if((INPUT_PORT8 & 0x40) || (UPPER_INPUT_PORT2 & 0x10) || (LOWER_INPUT_PORT2 & 0x10)) //����
  {
    CMD_FLAG6 |= 0x02;
  }
  else
  {
    CMD_FLAG6 &= ~0x02;
  }
 
	//
  if((UPPER_INPUT_PORT2 & 0x20) || (LOWER_INPUT_PORT2 & 0x20) || ((PGA_INPUT_MASK[0]&0x01) && (PGA_INPUT_BUFF[0]&0x01))) //��������
  {
    CMD_FLAG6 |= 0x04; 
  }
  else
  {
    CMD_FLAG6 &= ~0x04; 
  }

  if((UPPER_INPUT_PORT2 & 0x40) || (LOWER_INPUT_PORT2 & 0x40) || ((PGA_INPUT_MASK[0]&0x02) && (PGA_INPUT_BUFF[0]&0x02))) //��������
  {
    CMD_FLAG6 |= 0x08;
  }
  else
  {
    CMD_FLAG6 &= ~0x08;
  }
}

//------------------------------------------------------------------------------------- //Զ�̿�ͣ��
void remote_onoff_check(void)
{
/*
  static u16 remote_cmd_tms = 0;

  CMD_FLAG6 &= ~0x30;

  if(remote_cmd_tms<1000) remote_cmd_tms++;

  if(REMOTE_COMM_CMD & REMOTE_COMM_CMD_UP)
  {
    if(remote_cmd_tms<600) CMD_FLAG6 |= 0x10;
  }
  else if(REMOTE_COMM_CMD & REMOTE_COMM_CMD_DOWN)
  {
    if(remote_cmd_tms<600) CMD_FLAG6 |= 0x20;
  }
  else
  {
    remote_cmd_tms = 0 ;
  }
*/
}

/***************************************************************************************************
***************************************************************************************************/
//--------------------------------------------------------------------------------------------------
void inspect_state_check(void) //���޿��ؼ��,��λ������޼���
{
  static u16 inspect_key_on_tms=0,inspect_key_off_tms=0;

  //���ƹ���޿��ضϿ�������ģʽ
  if(!(INPUT_PORT4 & 0x04)) //��أ�����״̬
  {
    inspect_key_on_tms=0;

    if(inspect_key_off_tms<20) //100ms
    {
      inspect_key_off_tms++;
    }
    else
    {
      CMD_FLAG3 |= 0x01;
    }
  }
  else
  {
    inspect_key_off_tms=0;

    if(inspect_key_on_tms<20) //100ms
    {
      inspect_key_on_tms++;
    }
    else
    {
      CMD_FLAG3 &= ~0x01;
    }
  }
  
	//����ģʽȷ������ȫ�̵����պϣ����޿��رպ�>>�������
  if((CMD_FLAG2&0x03) && (CMD_FLAG4==0x55) && (CMD_FLAG3&0x01)) //
  {
    CMD_FLAG3 |= 0x02;
  }
  else 
  {
    CMD_FLAG3 &= ~0x02;
  }
}

//--------------------------------------------------------------------------------------------------
void normal_state_check(void) //���޿��ؼ��,��λ������޼���
{
  static u16 sf_on_tms=0;
    
  //��ȫ�̵����պ�
  if(CMD_FLAG4 & CF4_SRL_State_On) //0x40
  {
    if(sf_on_tms<10000) sf_on_tms++;
  } 
  else
  {
    sf_on_tms=0;
  }      
    
  //����ģʽȷ������ȫ��·ͨ����ȫ�̵����պϣ��Ǽ���״̬>>�����Զ�����
  if((CMD_FLAG2&0x03) && (CMD_FLAG4==0x55) && (!(CMD_FLAG3&0x0f)) && (sf_on_tms>100))  
  {
		//������բ�Ͽ�����Կ���ź�             
    if((CMD_FLAG7 & CF7_Aux_State_Off) && (!(CMD_FLAG6&0x0f)))                      
    {
      CMD_FLAG1 |= 0x01; //��λ�����Զ�����
    }
  }
  else
  {
    CMD_FLAG1 &= ~0x01;                  
  }
  
////////////////////////////////////////////////////////////////////////////////
/*
// �����ٿ���ѡ��
// ����ѡ�� or Զ��ͨ��
// ����Զ�̿��ƣ����Զ������ 
*/  
#ifdef ECT_01_MASTER	  
  //����ģʽѡ��
  //��Զ�̿��ƣ����Զ�̿��ƣ��������洢
  if(!ParaBuff[59])               
  {
    pcRtDataBuff[2] = 0;
    pcRtDataBuff[3] = 0;
  }
 
  //����ģʽ  
  CMD_FLAG8 &= ~(CF8_Auto_Fast_Mode1 | CF8_Auto_Fast_Mode2);
  //���ٶ�ѡ��1��or����Զ�̿���2
  if((PGA_INPUT_BUFF[13]&0x01) || ((pcRtDataBuff[2]==0xf1) && (pcRtDataBuff[3]==0x01)))
  {
    CMD_FLAG8 |= CF8_Auto_Fast_Mode2;  //����2
  }
  //���ٶ�ѡ��1��or����Զ�̿���1
  else if((PGA_INPUT_MASK[13]&0x01) || (pcRtDataBuff[2]==0xf1))
  {
    CMD_FLAG8 |= CF8_Auto_Fast_Mode1; //����1 
  }
#endif    
  
////////////////////////////////////////////////////////////////////////////////

}
  
//--------------------------------------------------------------------------------------------------
void driver_check(void)
{
  //����ǰ��������բ�Ͽ�
  CMD_FLAG7 &= ~CF7_Aux_State_Off; 
  
  if((!(INPUT_PORT4&0x02))) //�޸�����բ�����ߣ��޷��� (ParaBuff[7]) || 
  {
    CMD_FLAG7 |= CF7_Aux_State_Off;
  }

  //���������ź�  
  //��ȫ�̵����պϣ����������źţ����������ź�
  if(((CMD_OUTPUT_PORT&0x25)==0x25) && (CMD_OUTPUT_PORT&0x18) && (CMD_OUTPUT_PORT&0xc0))
  {
    CMD_FLAG8 |= CF8_ESC_Driver_On;
  }
  else
  {
    if(!(CMD_OUTPUT_PORT&0x1c))
    {
      CMD_FLAG8 &= ~CF8_ESC_Driver_On; 
    }
  }

  //ȷ����������ת��
  if((CMD_FLAG8 & CF8_ESC_Driver_On) && (CONTACT_FEEDBACK & 0x30) && (CONTACT_FEEDBACK & 0x0f))
  {
    CMD_FLAG8 |= CF8_ESC_Driver_Feedback;
  }
  else
  {
    if((!(CMD_FLAG8 & CF8_ESC_Driver_On)) && (!(CONTACT_FEEDBACK&0x30)))
    {
      CMD_FLAG8 &= ~CF8_ESC_Driver_Feedback;
    }
  }
}

/***************************************************************************************************
***************************************************************************************************/
void sf_check_string(void) 
{
  u16 delay_time = 300;
	
  static u8 INPUT_PORT8_old=0;               
  static u16 sf_train_open_tms=0,sf_train_close_tms=0;
	
  //����״̬���ָ�ʱ����Ϊ500ms
  if(CMD_FLAG3&0x01) delay_time=50; 
  
  if((!(INPUT_PORT8 & 0x01)) && (INPUT_PORT8_old&0x01)) //��ȫ��·�Ͽ����½���
  {
    sf_train_open_tms = 0;                                                        
  }        
  if(sf_train_open_tms<10000) sf_train_open_tms++;   
  
  if(!(INPUT_PORT8 & 0x01))
  { 
    CMD_FLAG4 &= ~0x01; //��ȫ��·ʵʱ״̬  
    CMD_FLAG4 &= ~0x04; //�Ͽ���ȫ�̵������� 
		
		sf_train_close_tms=0;
  } 
  else
	{	
    CMD_FLAG4 |= 0x01; //��ȫ��·ʵʱ״̬  
    
		if(sf_train_close_tms < 10000) sf_train_close_tms++;
		
		if((sf_train_open_tms > delay_time) && (sf_train_close_tms>10)) //2s 
  	{
    	CMD_FLAG4 |= 0x04; //��ȫ��·ͨ������պϰ�ȫ�̵���       
		}			
  }    
  
  //
  INPUT_PORT8_old = INPUT_PORT8;  
}     

/***************************************************************************************************
***************************************************************************************************/
void cmd_state_check_process(void)
{
  if(System_Sequence != 0x070f) System_Check_Fault_Flag |= 0x0008;
  
	sf_check_string(); 
		
  driver_check();

  run_key_check(); //���Կ���ź�

  //remote_onoff_check();//Զ��

  inspect_state_check(); //���޿��ؼ��,��λ������޼���
 	normal_state_check();
    
#ifdef ECT_01_MASTER	

  //pga_input_process->auto_manual_check
#else      
  STATE1 = 0;
  STATE2 = 0;
  STATE3 = 0;
  STATE4 = 0;
  
  CMD_FLAG2 &= ~0x0c; 
  CMD_FLAG8 &= ~0xc0; 
  //���Զ�
  CMD_FLAG2 |= (CMD_OMC_FLAG2&0x0c); 
  //����ģʽ
  CMD_FLAG8 |= (CMD_OMC_FLAG8&0xc0); 
  
#endif  

  System_Sequence |= 0x0800;
}

/***************************************************************************************************
***************************************************************************************************/
