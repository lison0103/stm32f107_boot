
#include "includes.h"  

#include "delay.h"

#include "esc.h"

#define CMD_OMC_OUTPUT_PORT pcOMC_EscRTBuff[30] 

#define SF_DRV_INTERVAL	(u32)3600
#define AUX_DRV_INTERVAL (u32)3600

/***************************************************************************************************
***************************************************************************************************/
//----------------------------------------------------------------------------------- //������� F50 						
void en_open_train_check(void)
{
	u8 temp1=0,error_set_tms=10;
	static u8 open_train_tms=0,esc_running_tms=0;  
  
	temp1 = INPUT_PORT2 & 0x03;
	if((ParaBuff[5]) || (!(CMD_FLAG5&0x02))) temp1 |= 0x02;   //���� or ����δ���� 
  
	if(CMD_FLAG8 & CF8_ESC_Driver_On) // (MOTOR_DRIVER_ON)
	{
		if(esc_running_tms<150) 
		{
			open_train_tms = 0;	
			esc_running_tms++;	
		}
		error_set_tms = 3;
	}
	else
	{
		esc_running_tms = 0;
		error_set_tms = 20;
	}
   
	if(temp1 == 0x03)  
	{    
		open_train_tms=0;
	}    
	else //���� 
	{
		if(open_train_tms < error_set_tms) 
		{
			open_train_tms++;  
		}
		else
		{
			EN_ERROR1 |= 0x01;
		}              
	}  
}

//--------------------------------------------------------------------------- //�����բ��� F51/F52
void en_sub_motor_brake(void)
{
  u8 ErrorTemp=0,motor_break_mask = 0x03, motor_break_value = 0;

  static u16 motor_break_error1_tms=0,motor_break_error2_tms=0;
  
  if((ParaBuff[6]==1) || (CMD_FLAG3&0x01))   //�޵����բ��� || ����
  { 
    motor_break_error1_tms = 0; 
    motor_break_error2_tms = 0; 
  } 
  else
  { 
    motor_break_value = (INPUT_PORT3 & 0x30)>>4;  //�̶���բ�����
    if(pcPgaInputMask[8]&0xf0) //8����բ���     
    {
      motor_break_mask = 0xff; 
      motor_break_value |= (pcPgaInputBuff[8]&0xfc);
      
      // motor_break_value ^= 0xff;
    }  
    else if(pcPgaInputMask[8]&0x0c) //4����բ���
    {
      motor_break_mask = 0x0f;
      motor_break_value |= (pcPgaInputBuff[8]&0x0c);
      
      // motor_break_value ^= 0x0f;
    }   
    else
    {
      // motor_break_value ^= 0x03;
    }    
        
    if((CONTACT_FEEDBACK&0X30) && (CONTACT_FEEDBACK&0X0f))  //�Ӵ����򿪣���բ��
    {
      if(motor_break_value) 
      {
        if(motor_break_mask==0x03)
        {
          ErrorTemp = 0x02;  
        }  
        else if(motor_break_mask==0x0f)
        {
          if(motor_break_value&0x03)
          {  
            ErrorTemp = 0x02;
          }  
          else
          {
            ErrorTemp = 0x04;
          }  
        }  
        else if(motor_break_mask==0xff)
        {
          if(motor_break_value&0x0f)
          {  
            ErrorTemp = 0x02;
          }  
          else
          {
            ErrorTemp = 0x04;
          }  
        }  
      }  
    }  
    else //��բδ��
    {
      if(motor_break_value != motor_break_mask) 
      {
        if(motor_break_mask==0x03)
        {
          ErrorTemp = 0x02;
        }  
        else if(motor_break_mask==0x0f)
        {
          if((motor_break_value&0x03)!=0x03)
          {  
            ErrorTemp = 0x02;
          }  
          else
          {
            ErrorTemp = 0x04;
          }  
        }  
        else if(motor_break_mask==0xff)
        {
          if((motor_break_value&0x0f)!=0x0f)
          {  
            ErrorTemp = 0x02;
          }  
          else
          {
            ErrorTemp = 0x04;
          }  
        }  
      }  
    } 
    
    if(ErrorTemp&0x02)
    {
      if(motor_break_error1_tms>150)
      {
        EN_ERROR1 |= 0x02;  
      } 
      else
      {
        motor_break_error1_tms++;  
      }  
    }    
    else
    {
      motor_break_error1_tms=0;
    }    

    if(ErrorTemp&0x04)
    {
      if(motor_break_error2_tms>150)
      {
        EN_ERROR1 |= 0x04;  
      } 
      else
      {
        motor_break_error2_tms++;  
      }  
    }    
    else
    {
      motor_break_error2_tms=0;
    }    
  }           
}
   
//------------------------------------------------------------------------------- //������բ��� F53
void en_aux_brake_check(void)
{
  static u16 aux_brake_not_close_error_tms=0,aux_brake_not_open_error_tms=0; 
	
  static u16 relay_drv_test_delay_tms=0,relay_drv_test_cnt=0,relay_drv_error_cnt=0;
  static u32 relay_drv_check_interval=0;
    
  if(!ParaBuff[7]) //�и�����բ     
  { 
    //������բ��������û��
    if((CMD_OUTPUT_PORT & PORT_AUX) && (CMD_OMC_OUTPUT_PORT & PORT_AUX) && (!(INPUT_PORT4&0x02)))  
    {
      if(aux_brake_not_close_error_tms>150) //1.5s
      {
        EN_ERROR7 |= 0x80;    
      }  
      else
      {
        aux_brake_not_close_error_tms++;	 
      }
    }  
    else
    {
      aux_brake_not_close_error_tms = 0;	
    }
     
    if((!(CMD_OUTPUT_PORT&PORT_AUX)) && (INPUT_PORT4&0x02))  //������բճ�� 
    {
      if(aux_brake_not_open_error_tms>400) 
      {
        EN_ERROR7 |= 0x40;		
      }  
      else
      {
        aux_brake_not_open_error_tms++;	
      }
    }
    else
    {
      aux_brake_not_open_error_tms = 0;	
    }
		
		//��⸨����բ�����Ƿ�����
		if(relay_drv_test_delay_tms++>100) relay_drv_test_delay_tms=0;
		//��ȫ�̵����պϣ������쳣���
		if((CMD_OUTPUT_PORT & PORT_AUX) && (CMD_OMC_OUTPUT_PORT & PORT_AUX) && (relay_drv_check_interval++>AUX_DRV_INTERVAL) && (relay_drv_test_delay_tms==0))
		{ 
			//�����ߵ�ƽ������
			if(Get_AUX_Dvr_FB())
			{
				relay_drv_error_cnt++;
			}  
			
			//�̵����Ͽ�
			AUX_Relay_CMD(RELAY_OFF);
			TIM_delay(500); 
			
			//�����͵�ƽ������
			if(!(Get_AUX_Dvr_FB()))
			{
				relay_drv_error_cnt++;    
			}  
			
			relay_drv_test_cnt++;
			if(relay_drv_test_cnt>=3)
			{
				if(relay_drv_error_cnt>=3)
				{
					EN_ERROR1 |= 0x08;
				}  
				
				relay_drv_test_cnt = 0;
				relay_drv_error_cnt = 0;
				relay_drv_check_interval = 0;
			}        
			
			//�̵����պ�
			AUX_Relay_CMD(RELAY_ON);
		} 

  } 
  else 
  {
    if((INPUT_PORT4&0x02))
    {
      if(aux_brake_not_open_error_tms<100)
      {
        aux_brake_not_open_error_tms++;
      }
      else
      {
        EN_ERROR7 |= 0x40;	
      }
    }
    else
    {
      EN_ERROR7 &= ~0xc0;
      aux_brake_not_open_error_tms = 0;
    }
  } 
}

//------------------------------------------------------------------- //�ǰ��� 
void en_Combplate_check(void)     
{ 
  u8 combplate_temp=0; 
  u8 *pcpara; 
  static u16 Combplate_error_tms=0,Combplate_ok_tms=0; 
  
  pcpara = &ParaBuff[0];
  
  if(!(pcPgaInputMask[10]&0x05)) return;
  
  combplate_temp = pcPgaInputBuff[10] & 0x05; 
  if(pcpara[20]) combplate_temp = combplate_temp ^ 0x05;  //1���պϱ���(pcPgaInputBuff[10]&0x05)
  
  if(CMD_FLAG3&0x01) //����״̬
  {
    EN_ERROR7 &= ~0x05;   
    
    if(combplate_temp == 0x05)     
    {
      if(Combplate_error_tms>200)
      {
        EN_ERROR7 |= 0x10;  
      } 
      else
      {        
        Combplate_error_tms++;
      }
      
      Combplate_ok_tms=0; 
    }
    else
    {
      if(Combplate_ok_tms>300)
      {
        EN_ERROR7 &= ~0x10;    
      } 
      else
      {        
        Combplate_ok_tms++;
      }
        
      Combplate_error_tms=0;
    }      
  } 
  else //�Ǽ���
  {
    EN_ERROR7 &= ~0x10;  
        
    if(combplate_temp != 0x05)
    {
      if(Combplate_error_tms>50)
      {
        EN_ERROR7 &= ~0x05; 
        if(!(combplate_temp&0x01)) EN_ERROR7 |= 0x01;  
        if(!(combplate_temp&0x04)) EN_ERROR7 |= 0x04;  
      } 
      else
      {        
        Combplate_error_tms++;
      }
      
      Combplate_ok_tms=0; 
    }
    else
    {
      if(Combplate_ok_tms>300)
      {
        EN_ERROR7 &= ~0x05;   
      } 
      else
      {        
        Combplate_ok_tms++;
      }
      
      Combplate_error_tms=0;
    }      
  }    
} 

//----------------------------------------------------------------------------------- //��Ƶ���� F57
void en_vvvf_check(void)
{
  static u16 vvvf_on_tms=0,vvvf_check_error_tms=0,vvvf_check_ok_tms=0;
    
  if(CMD_FLAG2 & 0x02) //��Ƶ����
  {
    if(vvvf_on_tms<500)
    {
      vvvf_on_tms++; 
    } 
    else 
    {  
      if(INPUT_PORT4 & 0x01) 
      { 
        //��ƵOK>3s���ָ�
        if(vvvf_check_ok_tms>500) 
        {  
          EN_ERROR1 &= ~0x80; 
        }  
        else 
        {
          vvvf_check_ok_tms++;
        }  
        vvvf_check_error_tms=0; 
      }     
      else   //��Ƶ����>0.5s������
      {
        if(vvvf_check_error_tms>50) 
        {  
          EN_ERROR1 |= 0x80;
        }  
        else
        {
          vvvf_check_error_tms++;
        }  
        vvvf_check_ok_tms=0;
      }    
    }  
  } 
  else
  {
    EN_ERROR1 &= ~0x80;
    vvvf_on_tms = 0;
  }  
}

//----------------------------------------------------------------------------------- ���м̵���ճ��
void en_run_relay_check(void)
{
  static u16 sf_relay_on_tms=0;
/*
//�Ǽ���״̬����ȫ�̵����պ�ʱ��<0.5,�Ӵ�����Ӧ�ñպϣ�����Ӧ���з���  
*/  
  if((CMD_OUTPUT_PORT&PORT_SF) && (!(CMD_FLAG3&0x01)))
	{
		if(sf_relay_on_tms<50)            
		{
			sf_relay_on_tms++;       
			
			if(CONTACT_FEEDBACK)             
			{
				EN_ERROR13 |= 0x80;            	 					
			}
		}
	}
	else    	    
	{
	   	sf_relay_on_tms=0;
	}		
}

//------------------------------------------------------------------------------- //�Ӵ����̵��� F63
void en_contact_check(void)
{
  u16 i=0,j=0;

  static u16 ContactError_old1, ContactError_old2;
  static u16 contact_check_error_tms1=0,contact_check_error_tms2=0;
  static u16 contact_error_same_tms1=0,contact_error_same_tms2=0;
  
  if(ESC_MODE_RELAY & RELAY_BIT_DS) //����������Ӵ���
  {
    if(!(INPUT_PORT2 & 0x80)) i |= 0x0010; //���պ�
  }
  else
  {
    if(INPUT_PORT2 & 0x80) i |= 0x0020; //���Ͽ�
  }  

  if(ESC_MODE_RELAY & RELAY_BIT_VI) //��Ƶ����
  {
    if(!(INPUT_PORT2 & 0x20)) i |= 0x0040; //���պ�
  }
  else
  {
    if(INPUT_PORT2 & 0x20) i |= 0x0080; //���Ͽ�
  }  

  if(ESC_MODE_RELAY & RELAY_BIT_VO) //��Ƶ���
  {
    if(!(INPUT_PORT2 & 0x40)) i |= 0x0100; //���պ�
  }
  else
  {
    if(INPUT_PORT2 & 0x40) i |= 0x0200; //���Ͽ�
  }  
  
  //������ɳ�ʼ��ǰor�Ǹ��Ͽ���
	if((!(CMD_FLAG5 & 0x02)) || ((PARA_CONTROL_MODE!=2)&&(PARA_CONTROL_MODE!=4)))
  {
    i = 0; 
  }    
                  
//������ 
  if((CONTACT_MASK&0x30)==0x10)    
  {    
    if((CMD_OUTPUT_PORT & PORT_UP)||(CMD_OUTPUT_PORT & PORT_DOWN)) //�����нӴ���
    {
      if(!(CONTACT_FEEDBACK & 0x10)) i |= 0x0001; //���պ�
    } 
    else
    {
      if(CONTACT_FEEDBACK & 0x10) i |= 0x0002; //���Ͽ�    
    } 
  }                       
  else //if((CONTACT_MASK&0x30)==0x30) //�����нӴ����ֿ�    
  { 
    if(CMD_OUTPUT_PORT & PORT_UP)
    {
      //�Ӵ����պ�,�ջ��޷��� 
      if(!(CONTACT_FEEDBACK & 0x10)) j |= 0x0001;
    }
    else
    {
      //�Ӵ����Ͽ�,�ջ�����
      if(CONTACT_FEEDBACK & 0x10) j |= 0x0002; 
    }    
    
    if(CMD_OUTPUT_PORT & PORT_DOWN)
    {
      //�Ӵ����պ�,�ջ��޷���
      if(!(CONTACT_FEEDBACK & 0x20)) j |= 0x0004;
    }
    else          
    {
      //�Ӵ����Ͽ�,�ջ�����
      if(CONTACT_FEEDBACK & 0x20) j |= 0x0008; 
    }      
  }    
               
  //������ 1           
  if((CONTACT_MASK&0x05)==0x01)    
  {          
    if(PARA_CONTROL_MODE==1) //����Ƶ
    {
      if((CMD_OUTPUT_PORT & PORT_D) || (CMD_OUTPUT_PORT & PORT_Y)) //�����ǽӴ���
      {
        if(!(CONTACT_FEEDBACK & 0x01)) i |= 0x1000; //���պ� 
      } 
      else
      {
        if(CONTACT_FEEDBACK & 0x01) i |= 0x2000;  //���Ͽ�
      }
    }
    else //������ or ���Ͽ���
    {   
      if((CMD_OUTPUT_PORT & PORT_D) || (CMD_OUTPUT_PORT & PORT_Y)) //�����ǽӴ���
      {
        if(!(CONTACT_FEEDBACK & 0x01)) i |= 0x0004; //���պ� 
      } 
      else
      {
        if(CONTACT_FEEDBACK & 0x01) i |= 0x0008;  //���Ͽ�
      }
    }      
  }
  else //������1���ֿ�
  {
    if(PARA_CONTROL_MODE==1) //����Ƶ
    {
      if(CMD_FLAG8 & CF8_DY_State_Star)// (CMD_OUTPUT_PORT & PORT_Y)
      {
        //�Ӵ����պ�,�ջ��޷���
        if(!(CONTACT_FEEDBACK & 0x01)) i |= 0x1000;
      }
      else
      {
        //�Ӵ����Ͽ�,�ջ�����
        if(CONTACT_FEEDBACK & 0x01) i |= 0x2000;
      }        
      
      if(CMD_FLAG8 & CF8_DY_State_Delta)// (CMD_OUTPUT_PORT & PORT_D)
      {
        //�Ӵ����պ�,�ջ��޷���
        if(!(CONTACT_FEEDBACK & 0x04)) j |= 0x1000;
      }
      else
      {
        //�Ӵ����Ͽ�,�ջ�����
        if(CONTACT_FEEDBACK & 0x04) j |= 0x2000;
      }   
    }  
    else //������ or ���Ͽ���
    {  
      if(CMD_FLAG8 & CF8_DY_State_Star)// (CMD_OUTPUT_PORT & PORT_Y)
      {
        //�Ӵ����պ�,�ջ��޷���
        if(!(CONTACT_FEEDBACK & 0x01)) j |= 0x0010;
      }
      else
      {
        //�Ӵ����Ͽ�,�ջ�����
        if(CONTACT_FEEDBACK & 0x01) j |= 0x0020;
      }        
      
      if(CMD_FLAG8 & CF8_DY_State_Delta)// (CMD_OUTPUT_PORT & PORT_D)
      {
        //�Ӵ����պ�,�ջ��޷���
        if(!(CONTACT_FEEDBACK & 0x04)) j |= 0x0040;
      }
      else
      {
        //�Ӵ����Ͽ�,�ջ�����
        if(CONTACT_FEEDBACK & 0x04) j |= 0x0080;
      }   
    }
  }  
                        
	//������ 2,˫��         
  if((!PARA_DRIVER_MODE) && (PARA_CONTROL_MODE!=1))               
  {  
    if((CONTACT_MASK&0x0a)==0x02) //����            
    {          
      if(CMD_FLAG8 & (CF8_DY_State_Star | CF8_DY_State_Delta))// ((CMD_OUTPUT_PORT & PORT_D) || (CMD_OUTPUT_PORT & PORT_Y)) //�����ǽӴ���
      {
        if(!(CONTACT_FEEDBACK & 0x02)) i |= 0x0400; //���պ�
      } 
      else
      {
        if(CONTACT_FEEDBACK & 0x02) i |= 0x0800;  //���Ͽ�
      } 
    }
    else
    {
      if(CMD_FLAG8 & CF8_DY_State_Star)// (CMD_OUTPUT_PORT & PORT_Y)
      {
        //�Ӵ����պ�,�ջ��޷���
        if(!(CONTACT_FEEDBACK & 0x02)) j |= 0x0100;
      }
      else
      {
        //�Ӵ����Ͽ�,�ջ�����
        if(CONTACT_FEEDBACK & 0x02) j |= 0x0200; 
      }        
    
      if(CMD_FLAG8 & CF8_DY_State_Delta)// (CMD_OUTPUT_PORT & PORT_D)
      {
        //�Ӵ����պ�,�ջ��޷���
        if(!(CONTACT_FEEDBACK & 0x08)) j |= 0x0400; 
      }
      else
      {
        //�Ӵ����Ͽ�,�ջ�����
        if(CONTACT_FEEDBACK & 0x08) j |= 0x0800; 
      }        
    }  
  }
  
//��ʱ  
  if(i)
  {    
    if(i==ContactError_old1) 
    {
      if(contact_error_same_tms1<1000) contact_error_same_tms1++;
    }  
    else
    {
      contact_error_same_tms1=0;
    }      
      
    if((contact_check_error_tms1>200) || (contact_error_same_tms1>80))
    {
      EN_ERROR8 |= (u8)((i&0x03)<<6);
      EN_ERROR9 |= (u8)(i>>2);
      EN_ERROR10 |= (u8)(i>>10);
    }
    else
    {
      contact_check_error_tms1++;
    }    
  }
  else
  {     
    contact_check_error_tms1 = 0; 
    contact_error_same_tms1 = 0;
  }                 
  
  if((j==ContactError_old2) && j)
  {    
    if(j==ContactError_old2) 
    {
      if(contact_error_same_tms2<1000) contact_error_same_tms2++;
    }  
    else
    {
      contact_error_same_tms2=0;
    }      

    if((contact_check_error_tms2>200) || (contact_error_same_tms2>80))
    {
      EN_ERROR11 |= (u8)((j&0x3f)<<2);
      EN_ERROR12 |= (u8)(j>>6);  
    }
    else    
    { 
      contact_check_error_tms2++;
    }    
  }
  else
  {     
    contact_check_error_tms2 = 0; 
    contact_error_same_tms2 = 0;
  }                 
                  
  ContactError_old1 = i;
  ContactError_old2 = j;
}

/***************************************************************************************************
***************************************************************************************************/
void sf_check_signal_inspection(void) 
{
  static u16 sf_train_open_tms=0,sf_train_close_tms=0;    
  static u16 isp_uds_off_tms=0,isp_uds_on_tms=0;    
	static u16 isp_uds_error_tms=0,isp_check_sfc_en=0;
  
  static u16 isp_enable_tms=0;
  
  //�м����������ź�
  if(CMD_FLAG6&0x0c)
  {
    if(isp_uds_on_tms<10000) isp_uds_on_tms++;
    isp_uds_off_tms = 0;
    
    //���޿�ʼ����ʱ����ȫ��·�պ�ʱ��>5s
    if(isp_check_sfc_en && (sf_train_close_tms>500))
    {
      EN_ERROR6 |= 0x20;
    }  
      
    isp_check_sfc_en = 0;  
  } 
  else
  {
    if(isp_uds_off_tms<10000) isp_uds_off_tms++;
    isp_uds_on_tms = 0;
    
    if(isp_uds_off_tms>200) isp_check_sfc_en=1;
  }                                        
  
  //��ȫ��·ͨ
  if(INPUT_PORT8 & 0x01)        
  {
    if(sf_train_close_tms<10000) sf_train_close_tms++;
    sf_train_open_tms=0; 
  } 
  else                      
  {
    if(sf_train_open_tms<10000) sf_train_open_tms++;
    sf_train_close_tms = 0;
  }   
    
  // �м����źţ�����ȫ��·���պ�   
  if((CMD_FLAG6&0x0c) && (!(INPUT_PORT8&0x01)))  
  {
    if(isp_uds_error_tms>300) 
    {
      EN_ERROR6 |= 0x20;
    }    
    else
    {
      isp_uds_error_tms++;
    }  
  } 
  else
  {
    isp_uds_error_tms = 0;
  }  

  //��ȫ�̵�������պ�ʱ���ʱ
  if(CMD_FLAG4 & CF4_EN_SRL_Enable) // 0x08
  {
    if(isp_enable_tms<1000) isp_enable_tms++;
  }
  else
  {
    isp_enable_tms=0;
  }  
  
  //
  if(((POWER_ON_TMS<500) || ((isp_enable_tms>10) && (isp_enable_tms<200))) && (isp_uds_on_tms>10))
  {
    EN_ERROR6 |= 0x20;
  }
}

/***************************************************************************************************
***************************************************************************************************/
void en_safety_relay_check(void)
{
  static u16 sf_relay_error_tms=0;
  static u16 relay_drv_test_delay_tms=0,relay_drv_test_cnt=0,relay_drv_error_cnt=0;
  static u32 relay_drv_check_interval=0;
	
  //��ȫ�̵��������Ͽ������ߣ���ȫ��·��ͨ���ջ����ߵ�ƽ
  if(((!(CMD_OUTPUT_PORT&PORT_SF)) || (!(CMD_OUTPUT_PORT2&PORT_SF_COMM)) || (!(INPUT_PORT8&0x01))) && (!(INPUT_PORT8&0x04)))
  {
    if(sf_relay_error_tms<1000) sf_relay_error_tms++; 
		
  	//��ȫ�̵����쳣ʱ��>500ms    
  	if(sf_relay_error_tms>50) EN_ERROR5 |= 0x04; 
  }
  //��ȫ�̵��������պϣ�ͬʱ��ȫ��·ͨ���ջ����ߵ�ƽ 
  else if((CMD_OUTPUT_PORT&PORT_SF) && (CMD_OUTPUT_PORT2&PORT_SF_COMM) && (INPUT_PORT8&0x01) && (INPUT_PORT8&0x04))
  {
    if(sf_relay_error_tms<1000) sf_relay_error_tms++;
		
  	//��ȫ�̵����쳣ʱ��>2000ms    
  	if(sf_relay_error_tms>200) EN_ERROR5 |= 0x04; 
  }  
  else
  {
    sf_relay_error_tms=0;
  }    
  
  if(relay_drv_test_delay_tms++>100) relay_drv_test_delay_tms=0;
  //��ȫ�̵����պϣ������쳣���
  if((CMD_FLAG4&0x40) && (relay_drv_check_interval++>SF_DRV_INTERVAL) && (relay_drv_test_delay_tms==0))
  { 
    //�����ߵ�ƽ������
    if(Get_SF_Dvr_FB())
    {
      relay_drv_error_cnt++;
    }  
    
    //��ȫ�̵����Ͽ�
    SF_Relay_CMD(RELAY_OFF);
		TIM_delay(500); 
		
    //�����͵�ƽ������
    if(!(Get_SF_Dvr_FB()))
    {
      relay_drv_error_cnt++;    
    }  
    
    relay_drv_test_cnt++;
    if(relay_drv_test_cnt>=3)
    {
      if(relay_drv_error_cnt>=3)
      {
        EN_ERROR5 |= 0x04;
      }  
      
      relay_drv_test_cnt = 0;
      relay_drv_error_cnt = 0;
      relay_drv_check_interval = 0;
    }        
    
    //��ȫ�̵����պ�
    SF_Relay_CMD(RELAY_ON);
  } 
} 

/***************************************************************************************************
Կ���źż��
***************************************************************************************************/
void en_key_check(void)
{
  static u32 auto_running_tms=0,key_on_tms=0,key_off_tms=0,key_stop_tms=0;
  static u32 signal_error_tms=0;
  
//------------------------------------------------------------------------------
//Կ��ճ��
  if(CMD_FLAG6 & 0x03) //��Կ���ź�
  {
    if(key_on_tms<1000) 
    {
      key_on_tms++;
    }
    else
    {
      EN_ERROR1 |= 0x40; //Կ��ճ��
    }  

    key_off_tms=0;
  }
  else 
  {  
    if(key_off_tms<200) 
    {
      key_off_tms++;
    }
    else
    {
      EN_ERROR1 &= ~0x40;  
    }
    
    key_on_tms=0;
  }

//------------------------------------------------------------------------------
  if(CMD_FLAG1 & 0x0c) //�������У�Կ��ͣ�� 
  {
    if(auto_running_tms<6000) auto_running_tms++; //1����
  }
  else
  {
    auto_running_tms=0;
  }    
  
  if((key_on_tms==300) && (auto_running_tms>1000)) //����10s��3s��Կ���ź�
  {
    EN_ERROR4 |= 0x20; 
  }  
  
  if(EN_ERROR4 & 0x20)
  {
    if(key_stop_tms++>200) EN_ERROR4 &= ~0x20;
  }  
  else
  {
    key_stop_tms=0; 
  }  

//-------------------------------------------------- //Կ���ź�/�����ź��쳣��� 
  //�Ǽ���״̬���м����ź�
  if((!(CMD_FLAG3&0x01)) && (CMD_FLAG6&0x0c))
  {
    if(signal_error_tms<100)
    {
      signal_error_tms++;
    }
    else
    {
      EN_ERROR6 |= 0x80;
    } 
  } 
//����״̬,��Կ���ź�  
  else if((CMD_FLAG3&0x01) && (CMD_FLAG6&0x03))
  {
    if(signal_error_tms<100)
    {
      signal_error_tms++;
    }
    else
    {
      EN_ERROR6 |= 0x40;
    } 
  }
  else
  {    
    signal_error_tms=0;
  }  
}

/***************************************************************************************************
***************************************************************************************************/


















/***************************************************************************************************
***************************************************************************************************/
//�����ж�
void en_error_check(void)
{
	u16 i=0,sf_signal_error=0; 
	static u16 en_no_error_tms=1000,en_error_tms=1000,sf_signal_error_tms=0; 
  static u16 sf_on_tms=0;
  
  //////////////////////////////////////////////////////////////////////////////
  //���������
  CMD_FLAG4 &= ~CF4_EN_Error;
	
  for(i=6;i<10;i++)
	{
		if(pcErrorBuff[i]) CMD_FLAG4 |= CF4_EN_Error;		
	}		
	
	for(i=16;i<32;i++)
	{
		if(pcErrorBuff[i]) CMD_FLAG4 |= CF4_EN_Error;	
	}		
  
#ifdef ECT_01_MASTER	
	for(i=0;i<32;i++)
  {  
		if(pcRtRecordErrorCodeBuff[i]) CMD_FLAG4 |= CF4_EN_Error;		
  }   
#endif	
  
  //////////////////////////////////////////////////////////////////////////////
  //��ȫ��·ͨ�������õ����쳣
	CMD_FLAG4 &= ~CF4_EN_SC_Error;
  
  for(i=0;i<6;i++)
  {    
    if(pcErrorBuff[i])  sf_signal_error = 1;
  }      

  for(i=10;i<16;i++)
  {    
    if(pcErrorBuff[i])  sf_signal_error = 1;
  }   
  
  if(CMD_FLAG4 & CF4_SC_State_On)
  {
    if(sf_on_tms<0xffff) sf_on_tms++;
  } 
  else
  {
    sf_on_tms = 0;
  }
  
  //��ȫ��·�պϣ������ⰲȫ��·����
  if(sf_signal_error && (sf_on_tms>1000)) // 0x01 
  {
    if(sf_signal_error_tms > 200) 
    {
      CMD_FLAG4 |= CF4_EN_SC_Error;  // 0x02 �����ⰲȫ��·����
    }
    else 
    {
      sf_signal_error_tms++;
    }    
  } 
  else
  {
    sf_signal_error_tms = 0;
  }    
  
  //////////////////////////////////////////////////////////////////////////////
  //
  if(en_no_error_tms<10000) en_no_error_tms++;
  if(en_error_tms<10000) en_error_tms++;  

  //�����⵽����
  if((CMD_FLAG4 & (CF4_EN_SC_Error | CF4_EN_Error)) || (CMD_OMC_FLAG4 & (CF4_EN_SC_Error | CF4_EN_Error)))
  { 
    en_no_error_tms = 0;
    CMD_FLAG4 &= ~CF4_EN_SRL_Enable;
  }
  else
  {
    //�����������             
		if(!( CMD_FLAG4 & CF4_EN_SRL_Enable ))  //��ȫ�̵����պϲ�����
		{
			if((en_no_error_tms>100) && (en_error_tms>400))
    	{
      	CMD_FLAG4 |= CF4_EN_SRL_Enable; 											     
    	}	 					           
		}   
		else  //����պϣ���ȫ�̵�������ʱ������
		{
			en_error_tms = 0;	
		}	       
  }    
}

/***************************************************************************************************
***************************************************************************************************/
//���ϸ�λ
void reset_error(void) 
{
	u16 i;
  static u16 enErrorTms=0,db_reset_tms=0,KeyResetOnTms=0,PinResetOnTms=0;

  u8 reset_en=0;

  if(CMD_FLAG4 & CF4_EN_Error) // �������
  {
    if(enErrorTms < 10000) enErrorTms++;
  }  
  else
  {
    enErrorTms = 0;                        
  } 
	
  //��ʾ�尴����λָ�t=30ʱ��λ
#ifdef ECT_01_MASTER
  i = 30;
  if(CMD_FLAG10 & 0x80)
#else
  i = 5;  
  if(CMD_OMC_FLAG10 & 0x80)
#endif
  { 
    if(db_reset_tms++>100) CMD_FLAG10 &= ~0x80; 
    
    if((enErrorTms>300) && (db_reset_tms==i)) 
    {
      reset_en=1;
    }
  } 
  else
  {
    db_reset_tms = 0; 
  }  
      
  //��Կ���ź�,����Կ�׸�λ 
  if((CMD_FLAG6 & 0x03) && (!ParaBuff[33])) 
  {
    if(KeyResetOnTms < 10000)                       
    {
      KeyResetOnTms++; 
    }  
  }   
  else              
  {  
		if((enErrorTms>1000) && (KeyResetOnTms>500) && (KeyResetOnTms<1000))
		{
			reset_en=1;
		}			
		    
    KeyResetOnTms=0;
  }
  
  if(INPUT_PORT4&0x20)
  {
    if(PinResetOnTms < 1000) PinResetOnTms++;
  }
  else
  {  
		if((enErrorTms>500) && (PinResetOnTms>100) && (PinResetOnTms<500))
		{
			reset_en=1;
		}			
		
    PinResetOnTms = 0; 
  }
	
	if( reset_en )
  {    
		for(i=0;i<32;i++)
		{
			pcErrorBuff[i] = 0;
#ifdef ECT_01_MASTER	
			pcRtRecordErrorCodeBuff[i] = 0;
#endif
		}		
/*		
		for(i=16;i<32;i++)
		{
			pcErrorBuff[i] = 0;
#ifdef ECT_01_MASTER	
			pcRtRecordErrorCodeBuff[i] = 0;
#endif
		}		
*/		
		CMD_FLAG4 &= ~0x80;
        
#ifdef ECT_01_MASTER	
    EEP_Struct_RtBuff.flag=0xf1;
#endif
	}  
}

/***************************************************************************************************
***************************************************************************************************/
void SF_Relay_Output(void)
{
  u32 *OMC_checkTms = (u32*)&pcOMC_EscRTBuff[140];
#ifdef ECT_01_MASTER	  
  u32 *checkTms = (u32*)&EscRTBuff[140];
  u32 sfton=300,sftoff=600,sfauxton=400,sfauxtoff=1000;
#else
  u32 *checkTms = (u32*)&pcOMC_EscRTBuff[140];
  u32 sfton=700,sftoff=1000,sfauxton=300,sfauxtoff=900;
#endif

  /* ��ȫ��·�Ͽ������߿���ʱ�����>12s����λ��ȫ�̵����Լ���ɱ�־ */
  if((!(CMD_FLAG5&0x04)) && ((POWER_ON_TMS>1200) || (OMC_checkTms[0]>1200) || (!(CMD_FLAG4 & CF4_SC_State_On))))
  {
    CMD_FLAG5 |= 0x04;  
  }
  
	//�����޹���
	if((CMD_FLAG4&0x1f)==(CF4_SC_State_On | CF4_SC_SRL_Enable | CF4_EN_SRL_Enable)) //
  {   
    if((CMD_OMC_FLAG4&0x1f)==(CF4_SC_State_On | CF4_SC_SRL_Enable | CF4_EN_SRL_Enable)) //
    {
      if(CMD_FLAG5 & 0x04) 
      {
        CMD_OUTPUT_PORT |= PORT_SF;  
        CMD_OUTPUT_PORT2 |= PORT_SF_COMM;  
        
        //��ȫ�̵����պ�״̬
        CMD_FLAG4 |= CF4_SRL_State_On; 
        CMD_FLAG4 &= ~CF4_SRL_State_Off;     
        
        //������ʼ�����
        CMD_FLAG5 |= 0x04;         
      } 
      else //������� 
      {    
        if((checkTms[0]>sfton) && (checkTms[0]<sftoff)) //checkTms[0]
        {
          CMD_OUTPUT_PORT |= PORT_SF;
          CMD_FLAG4 |= CF4_SRL_State_On;
          CMD_FLAG4 &= ~CF4_SRL_State_Off;
        }  
        
        if((checkTms[0]>sfauxton) && (checkTms[0]<sfauxtoff)) //checkTms[0]
        {
          CMD_OUTPUT_PORT2 |= PORT_SF_COMM;
        }	
      }  
    }  
	}
}

/***************************************************************************************************
***************************************************************************************************/
void en_check(void)
{
  if(System_Sequence != 0x0307) System_Check_Fault_Flag |= 0x0004;
  
  //���ϸ�λ
	reset_error();

  //////////////////////////////////////////////////////////////////////////////
	
  //������ɳ�ʼ��
	if(CMD_FLAG5 & 0x02)
	{		
  	#ifdef ECT_01_MASTER	
  		pga_input_process(); 
		#else      
      sf_chain_check();		
  	#endif
	}
  
  //////////////////////////////////////////////////////////////////////////////
  
  //�����������
	en_open_train_check();

  //�ǰ���
  en_Combplate_check();     
  
  //��Ƶ���ϼ��
  en_vvvf_check();
  
  //������нӴ��������̵����Ƿ�ճ��  
  en_run_relay_check();     

  //�����բ���
	en_sub_motor_brake();     

  //������բ���
	en_aux_brake_check();
  
  //�Ӵ������
	en_contact_check();
	
  //�����źż�� 
	sf_check_signal_inspection();  

  //��ȫ�̵������ 
	en_safety_relay_check(); 

  //����Կ�׼��
  en_key_check();
                      
  //////////////////////////////////////////////////////////////////////////////
  //�ж��Ƿ��������Լ����
  if(EN_ERROR_SYS1)
  {
    EN_ERROR4 |= 0x80; 
  } 
  
  //����洢������
  if(EN_ERROR_SYS2 & 0x01) EN_ERROR5 |= 0x01;
  //�����������
  //if(EN_ERROR_SYS2 & 0x02) EN_ERROR5 |= 0x02;
    
  
  //////////////////////////////////////////////////////////////////////////////
	en_error_check();                                              
 
  CMD_FLAG4 |= CF4_SRL_State_Off; //��λ��ȫ�̵����Ͽ���־
  CMD_FLAG4 &= ~CF4_SRL_State_On; //�����ȫ�̵����պϱ�־
	CMD_OUTPUT_PORT &= ~PORT_SF;
	CMD_OUTPUT_PORT2 &= ~PORT_SF_COMM;
	
  //����ʱ��>2s������պϰ�ȫ�̵���
  if(POWER_ON_TMS>200) SF_Relay_Output();
  
  EWDT_TOOGLE();
  
  System_Sequence |= 0x0400;
}

/***************************************************************************************************
***************************************************************************************************/


