
#include "esc.h"
#include "includes.h"













/***************************************************************************************************
***************************************************************************************************/
//--------------------------------------------------------------------------------------------------
void run_key_check(void) //打包钥匙信号
{
  if((INPUT_PORT8 & 0x80) || (UPPER_INPUT_PORT2 & 0x08) || (LOWER_INPUT_PORT2 & 0x08)) //上行
  {
    CMD_FLAG6 |= 0x01;
  }
  else
  {
    CMD_FLAG6 &= ~0x01;
  }

  if((INPUT_PORT8 & 0x40) || (UPPER_INPUT_PORT2 & 0x10) || (LOWER_INPUT_PORT2 & 0x10)) //下行
  {
    CMD_FLAG6 |= 0x02;
  }
  else
  {
    CMD_FLAG6 &= ~0x02;
  }
 
	//
  if((UPPER_INPUT_PORT2 & 0x20) || (LOWER_INPUT_PORT2 & 0x20) || ((PGA_INPUT_MASK[0]&0x01) && (PGA_INPUT_BUFF[0]&0x01))) //检修上行
  {
    CMD_FLAG6 |= 0x04; 
  }
  else
  {
    CMD_FLAG6 &= ~0x04; 
  }

  if((UPPER_INPUT_PORT2 & 0x40) || (LOWER_INPUT_PORT2 & 0x40) || ((PGA_INPUT_MASK[0]&0x02) && (PGA_INPUT_BUFF[0]&0x02))) //检修下行
  {
    CMD_FLAG6 |= 0x08;
  }
  else
  {
    CMD_FLAG6 &= ~0x08;
  }
}

//------------------------------------------------------------------------------------- //远程开停梯
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
void inspect_state_check(void) //检修开关检测,置位允许检修检修
{
  static u16 inspect_key_on_tms=0,inspect_key_off_tms=0;

  //控制柜检修开关断开，检修模式
  if(!(INPUT_PORT4 & 0x04)) //外控，检修状态
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
  
	//控制模式确定，安全继电器闭合，检修开关闭合>>允许检修
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
void normal_state_check(void) //检修开关检测,置位允许检修检修
{
  static u16 sf_on_tms=0;
    
  //安全继电器闭合
  if(CMD_FLAG4 & CF4_SRL_State_On) //0x40
  {
    if(sf_on_tms<10000) sf_on_tms++;
  } 
  else
  {
    sf_on_tms=0;
  }      
    
  //控制模式确定，安全回路通，安全继电器闭合，非检修状态>>允许自动控制
  if((CMD_FLAG2&0x03) && (CMD_FLAG4==0x55) && (!(CMD_FLAG3&0x0f)) && (sf_on_tms>100))  
  {
		//辅助抱闸断开，无钥匙信号             
    if((CMD_FLAG7 & CF7_Aux_State_Off) && (!(CMD_FLAG6&0x0f)))                      
    {
      CMD_FLAG1 |= 0x01; //置位允许自动控制
    }
  }
  else
  {
    CMD_FLAG1 &= ~0x01;                  
  }
  
////////////////////////////////////////////////////////////////////////////////
/*
// 快慢速控制选择
// 本地选择 or 远程通信
// 如无远程控制，清除远程数据 
*/  
#ifdef ECT_01_MASTER	  
  //快速模式选择
  //无远程控制，清除远程控制，不主动存储
  if(!ParaBuff[59])               
  {
    pcRtDataBuff[2] = 0;
    pcRtDataBuff[3] = 0;
  }
 
  //快速模式  
  CMD_FLAG8 &= ~(CF8_Auto_Fast_Mode1 | CF8_Auto_Fast_Mode2);
  //有速度选档1，or，有远程快速2
  if((PGA_INPUT_BUFF[13]&0x01) || ((pcRtDataBuff[2]==0xf1) && (pcRtDataBuff[3]==0x01)))
  {
    CMD_FLAG8 |= CF8_Auto_Fast_Mode2;  //快速2
  }
  //有速度选档1，or，有远程快速1
  else if((PGA_INPUT_MASK[13]&0x01) || (pcRtDataBuff[2]==0xf1))
  {
    CMD_FLAG8 |= CF8_Auto_Fast_Mode1; //快速1 
  }
#endif    
  
////////////////////////////////////////////////////////////////////////////////

}
  
//--------------------------------------------------------------------------------------------------
void driver_check(void)
{
  //开梯前，辅助抱闸断开
  CMD_FLAG7 &= ~CF7_Aux_State_Off; 
  
  if((!(INPUT_PORT4&0x02))) //无辅助抱闸，或者，无反馈 (ParaBuff[7]) || 
  {
    CMD_FLAG7 |= CF7_Aux_State_Off;
  }

  //驱动主机信号  
  //安全继电器闭合，有上下行信号，有星三角信号
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

  //确认主机驱动转动
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
	
  //检修状态，恢复时间设为500ms
  if(CMD_FLAG3&0x01) delay_time=50; 
  
  if((!(INPUT_PORT8 & 0x01)) && (INPUT_PORT8_old&0x01)) //安全回路断开，下降沿
  {
    sf_train_open_tms = 0;                                                        
  }        
  if(sf_train_open_tms<10000) sf_train_open_tms++;   
  
  if(!(INPUT_PORT8 & 0x01))
  { 
    CMD_FLAG4 &= ~0x01; //安全回路实时状态  
    CMD_FLAG4 &= ~0x04; //断开安全继电器驱动 
		
		sf_train_close_tms=0;
  } 
  else
	{	
    CMD_FLAG4 |= 0x01; //安全回路实时状态  
    
		if(sf_train_close_tms < 10000) sf_train_close_tms++;
		
		if((sf_train_open_tms > delay_time) && (sf_train_close_tms>10)) //2s 
  	{
    	CMD_FLAG4 |= 0x04; //安全回路通，允许闭合安全继电器       
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

  run_key_check(); //打包钥匙信号

  //remote_onoff_check();//远程

  inspect_state_check(); //检修开关检测,置位允许检修检修
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
  //手自动
  CMD_FLAG2 |= (CMD_OMC_FLAG2&0x0c); 
  //快速模式
  CMD_FLAG8 |= (CMD_OMC_FLAG8&0xc0); 
  
#endif  

  System_Sequence |= 0x0800;
}

/***************************************************************************************************
***************************************************************************************************/
