/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.1
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/*--------------中断优先级--------------------
AD DMA中断：								抢占优先级0，子优先级0			
外部中断5_9(用于回充红外):						抢占优先级0，子优先级0
外部中断10_15(用于碰撞):						抢占优先级0，子优先级0
TIM1输入捕获(用于中红外):						抢占优先级1，子优先级0(未使用)
TIM2溢出中断(用于时基):							抢占优先级1，子优先级0

USART1空闲中断(用于导航板接收):					抢占优先级3，子优先级0(取消)
USART1发送DMA中断(用于导航板数据上报):				抢占优先级4，子优先级0
USART2空闲中断(用于DEBUG接收):					抢占优先级3，子优先级0(取消)
USART2发送DMA中断(用于DEBUG发送):				抢占优先级5，子优先级0(取消)
USART3空闲中断(用于陀螺仪接收):					抢占优先级3，子优先级0(取消)
USART3发送DMA中断(用于陀螺仪控制):					抢占优先级5，子优先级0(取消)


外部中断10_15(用于STOP唤醒):					抢占优先级12，子优先级0
RTC闹钟中断(PP2取消):							抢占优先级12,子优先级0
----------------------------------------------*/



//=====================头文件====================================
#include "AAA-Include.h"
//=====================头文件====================================
#define  FENGRUI_FR1703

#ifdef	HW_INT_REC
#define BIT0_MIN_SAMPLE	14//14//12		//ms
#define BIT0_MAX_SAMPLE 16//16//17		//ms
#define BIT1_MIN_SAMPLE 6//6//5		//ms
#define BIT1_MAX_SAMPLE 9//8//9		//ms
#define START_FRAME_MIN	28
#define START_FRAME_MAX 32
#endif

static	 u8 r_step = 0;
static	 u8 l_step = 0;   


bool log_show;
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

void TIM1_UP_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	sys_time1_cnt++;		//10ms一次

	if(sys_time1_cnt%2==0)	//20ms
		{
			FanSpd_Cal();
		}

	if(sys_time1_cnt%4==0)	//40ms
		{
			FanSpd_PID();
		}
}

////////////////////////////////////////////////////////////////////////////////

#ifdef REMOTE
void Parse_Remote_Signa(INFR_DATA *hw_info)
{
	//延时200ms,防止遥控按键抖动，按一次按键大概发3-4次相同的码，时长约200ms左右
	if(giv_sys_time-remote_info.effect_time<2000)
		return;
	switch (hw_info->data)
		{
#if 0
			case REMOTE_SLEEP:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_SLEEP;
				remote_info.rece_ir=hw_info->rece_ir;
				break;
			case REMOTE_TIME:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_TIME;
				remote_info.rece_ir=hw_info->rece_ir;
				break;
			case REMOTE_SET:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_SET;
				remote_info.rece_ir=hw_info->rece_ir;
				break;
			case REMOTE_PREEN:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_PREEEN;
				remote_info.rece_ir=hw_info->rece_ir;
				break;

			case REMOTE_SPOT:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_SPOT;
				remote_info.rece_ir=hw_info->rece_ir;
				break;
#endif
			case REMOTE_DOCK:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_DOCK;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_flag&=REMOTE_KEYFLAG_DOCK;
				break;
			case REMOTE_GUIHUA:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_GUIHUA;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_flag&=REMOTE_KEYFLAG_AUTO;
				break;
			case REMOTE_FORWOARD:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_FORWORD;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_time=giv_sys_time;
				remote_info.key_flag&=REMOTE_KEYFLAG_FORWORD;
				break;
			case REMOTE_LEFT:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_LEFT;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_time=giv_sys_time;
				remote_info.key_flag&=REMOTE_KEYFLAG_LEFT;
				break;
			case REMOTE_START:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_START;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_flag&=REMOTE_KEYFLAG_START;
				break;
			case REMOTE_RIGHT:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_RIGHT;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_time=giv_sys_time;
				remote_info.key_flag&=REMOTE_KEYFLAG_RIGHT;
				break;
			case REMOTE_BACK:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_BACK;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_time=giv_sys_time;
				remote_info.key_flag&=REMOTE_KEYFLAG_BACK;
				break;
			case REMOTE_YBS:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_YBS;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_flag&=REMOTE_KEYFLAG_YBS;
				break;
			case REMOTE_FAN:
				remote_info.effect=true;
				remote_info.effect_time=giv_sys_time;
				remote_info.remote_key=REMOTE_KEY_FAN;
				remote_info.rece_ir=hw_info->rece_ir;
				remote_info.key_flag&=REMOTE_KEYFLAG_FAN;
				break;
		}
}
#endif



//======================================================================================
void TIM2_IRQHandler(void)	//	10K 中断
{ 
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update );
	giv_sys_time ++;
	if(mode.status)
		motion1.worktime++;
	
#ifdef FREE_SKID_INDEP_CHECK
	if(Free_Skid_Indep.check_flag)
		Free_Skid_Indep.check_start_time++;
#endif


	
	if((giv_sys_time % 2) == 0)
		{
			VOL_TEMP_ready = true;      //马达电流采样   
		}

	if((giv_sys_time %10) == 0)			//	1mS  1000HZ   实际500赫兹
		{   		
			action_earth_time=true;
			
		}
	if((giv_sys_time%20)==0)			//2ms
		{
			check_earth_time=true;
		}

	if((giv_sys_time%30)==0)
		{
#ifdef FAST_WALL_DET
			action_wall_time=true;
#endif
			earth_time=true;
		}

			
	if((giv_sys_time%50)==0)			//5ms
		{
//#ifndef  FAST_WALL_DET
			wall_time = true; 	
//#endif
			gyro_bios.check_flag=true;
		}
				
			
//	if((giv_sys_time % 100) == 0)
	if((giv_sys_time % 100) == 0)		//	10mS
		{
			YBS_Check_Flag=true;
			key_time = true;          //按键时间	  
			rap_time = true;          //左右轮速度调节时间
			CHARGE_READY=true;		//qz add 20180522
			Rotate_Skid.time_flag=true;
			gyro_check_time=true;
			Cal_CoordinateXY();
			Record_Coordinate_Intime();
			rm_t_cnt_timeflag=true;
			lm_t_cnt_timeflag=true;
#ifdef UART_TWOBUF
			if(DMA1_Channel7->CNDTR==0)
				{
					if(buf1_full)
						{
							DMA_USART2_TX_Address((uint32_t)UART_BUF1,strlen((const char*)UART_BUF1));
							//buf1_full=false;
						}
					else if(buf2_full)
						{
							DMA_USART2_TX_Address((uint32_t)UART_BUF2,strlen((const char*)UART_BUF2));
							//buf2_full=false;
						}
					else if(buf3_full)
						{
							DMA_USART2_TX_Address((uint32_t)UART_BUF3,strlen((const char*)UART_BUF3));
							
							//DMA_USART2_TX_Address((uint32_t)UART_BUF3,buf3_cnt);							
							buf3_cnt=0;
							//buf3_full=false;
						}
					else
						{
							uart_dma_busy=false;
						}
				}
#endif		
		}			
		
		
			
	if((giv_sys_time % 200) == 0)				//20mS
		{
//			rap_time=true;
//			Coordinate_report_time=true;
			//Analysis_Coordinate();
#ifdef FAN_SPD_CTL
#ifndef FAN_SPD_TIM1
			FanSpd_Cal();
#endif
#endif
		}

	//qz add 20180316
	if((giv_sys_time%400)==0)
		{
#ifdef TUYA_WIFI
			wifi_time=true;
#endif
#ifdef FAN_SPD_CTL
			fanspd_flag=true;
#endif
		}
		
	if((giv_sys_time % 500) == 0)				//50 mS
		{
			led.led_time=true;
			gyro_comm_flag=true;
		}
		
	if((giv_sys_time % 1000) == 0)				//100ms			
		{
			CHECK_STATUS_TIME=true; 		//qz add 20180417
#ifndef  FAST_WALL_DET
			action_wall_time=true;
#endif
			spd_acc_flag=true;
			test_100ms_flag=true;
		}

	if((giv_sys_time%3000)==0)
		{
			led.quic_flag=true;
#ifdef TUYA_WIFI
			wifi_update_time=true;
#endif

		}
	if((giv_sys_time % 5000) == 0)				//500 mS
		{
			log_show = true;				//显示更新时间		
			led.halfsec_flag=true;
			Read_PwrSwitch_Status();
			test_500ms_flag=true;
			gyrotick_ok_timeflag=true;
		}
	
	if((giv_sys_time%10000)==0)
		{
			Sec=true;
			led.sec_flag=true;
			top_time_sec=top_time;
			top_time=0;
			test_1s_flag=true;
		}
	
	if((giv_sys_time%20000)==0)			//2s
		{
			watchdog_time = true;
		}

	if((giv_sys_time%30000)==0)
		{
			abort_shiftybs_flag=true;
		}
	
	if((giv_sys_time % 50000) == 0)     //5秒标志
		{
			Five_Sec  = true;	
		}

	if((giv_sys_time % 600000) == 0)	//分钟标志
		{
			gbv_minute = true;
#ifdef  RTC_8M_CORR
			rtc_8m_flag = FALSE;
#endif 
		}

	if((motion1.worktime%600000)==0)
		{
			if(mode.status)
				Gyro_Data.cal_flag=true;
			else
				Gyro_Data.cal_flag=false;
		}
#ifdef EARTH_IN_TIM2	
	Read_Earth_My();							//	读取地检
#endif
}
//======================================================================================
//======================================================================================






















//======================================================================================
//======================================================================================
/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


uint8_t ADC_test_counter;

//////void ADC1_2_IRQHandler(void)
//////{
////////////		ADC_test_counter++;
////////////		if((ADC_test_counter& 0x01) == 0x01)
////////////			{BIRD_PE14_1();}
////////////		else{BIRD_PE14_0();}
////////////	
//////		
////////////	//	 ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
////////////		 ADC_ready = true;
////////////		 if(((DMA1_Channel1->CNDTR)&0XF) != 0)
////////////				 {
////////////					 ReInitAd();
////////////				 }
////////////		 else
////////////				 {			 
//////			
//////	
//////		ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
//////	
//////	
////////////				 }

//////	
//////		 ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
//////}
//======================================================================================
//======================================================================================













/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void ADC_IRQHandler(void)
//{
// ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
// ADC_ready = true;
// if(((DMA1_Channel1->CNDTR)&0XF) != 0)
// {
//	 ReInitAd();
// }
// else
// {			 
//     ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
// }	   
// ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
//}


void EXTI0_IRQHandler(void)
{
	//左轮速度脉冲外部中断函数
	if(EXTI_GetITStatus(EXTI_Line_LSPD)!=RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line_LSPD);
			switch (l_step)
				{
					case 0:
						if(Read_L_Speed() == Bit_RESET)   //Bit_RESET = 0
							{
								if((giv_sys_time - l_ring.dq_time) > 1)
									{
										if(ring_cnt_flag)
											{
												l_ring.length++; 
												l_ring.speed_length++;		//qz add 20180203
												l_ring.dq_time = giv_sys_time;
												if(l_rap.ori == FRONT)
													{
														l_ring.all_length++;
													}
		//										else
												else if(l_rap.ori==BACK)
													{
														l_ring.all_length--;
													}
											}
									}
									l_step = 1; 
							}
						break;
					case 1:
						if(Read_L_Speed() != Bit_RESET)
							{
								if((giv_sys_time - l_ring.dq_time) > 1)
									{
										if(ring_cnt_flag)
											{
												l_ring.length++; 
												l_ring.speed_length++;		//qz add 20180203
												if(l_rap.ori == FRONT)
													{
														l_ring.all_length++;
													}
		//										else
												else if(l_rap.ori==BACK)
													{
														l_ring.all_length--;
													}
											}
																
										l_ring.dq_time = giv_sys_time;
									}
									l_step = 0; 
							}
						break;
					default :
						l_step = 0;
				 }
		}
}
/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
#ifdef SPEED_EXTI
	if(EXTI_GetITStatus(EXTI_Line_RSPD)!=RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line_RSPD);
			switch (r_step)
				{
					case 0:
						if(Read_R_Speed() == Bit_RESET)   //Bit_RESET = 0
							{
								if((giv_sys_time - r_ring.dq_time) > 1)
									{
										if(ring_cnt_flag)
											{
												r_ring.length++; 
												r_ring.speed_length++;		//qz add 20180203
												if(r_rap.ori == FRONT)
													{
														r_ring.all_length++;
													}
		//										else
												else if(r_rap.ori==BACK)
													{
														r_ring.all_length--;
													}
											}
										r_ring.dq_time = giv_sys_time;
									}
									r_step = 1; 
							}
						break;
					case 1:
						if(Read_R_Speed() != Bit_RESET)
							{
								if((giv_sys_time - r_ring.dq_time) > 1)
									{
										if(ring_cnt_flag)
											{
												r_ring.length++; 
												r_ring.speed_length++;		//qz add 20180203
												if(r_rap.ori == FRONT)
													{
														r_ring.all_length++;
													}
		//										else
												else if(r_rap.ori==BACK)
													{
														r_ring.all_length--;
													}
											}						
										r_ring.dq_time = giv_sys_time;
									}
									r_step = 0; 
							}
						break;
					default :
						r_step = 0;
						 }
		}
#endif
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
				{
						ExitInput = true;
						EXTI_ClearITPendingBit(EXTI_Line2);	  
				}
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	
}

//======================================================================================
//======================================================================================

//右轮速度脉冲外部中断函数
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line4);
			fanspd_cnt++;
		}
}



/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
#ifdef IR_CORRECT
	static u32 r_high_time,l_high_time,rm_high_time,rb_high_time,lm_high_time,lb_high_time;
#endif
	static u32 l_hw_time=0,lm_hw_time=0,lb_hw_time=0,r_hw_time=0,rm_hw_time=0,rb_hw_time=0;
	u32 sample_time;
	 /////////////右红外中断///////////////
	if(EXTI_GetITStatus(EXTI_Line_RHW)!=RESET) 	 
		{
			if(Read_R_HW())		 //上升沿
				{
					if(r_hw.start)
						{
							sample_time=giv_sys_time-r_hw_time;
							if((sample_time>=BIT0_MIN_SAMPLE)&&(sample_time<=BIT0_MAX_SAMPLE))		 //bit '0'
								{
									r_hw.tempdata<<=1;
									r_hw.tempdata|=0;
									r_hw.bitnumber++;
								}
							else if((sample_time>=BIT1_MIN_SAMPLE)&&(sample_time<=BIT1_MAX_SAMPLE))  //bit '1'
								{
									r_hw.tempdata<<=1;
									r_hw.tempdata|=1;
									r_hw.bitnumber++;
								}

#ifdef INTERFERENCE_ACTION
							//qz add 20180823，抗干扰
							else
								{
									r_hw.tempdata=0;
									r_hw.start=0;
									r_hw.bitnumber=0;
								}
#endif
							if(r_hw.bitnumber==8)
								{
									r_hw.data=r_hw.tempdata;
									r_hw.tempdata=0;
									r_hw.start=0;
									switch (r_hw.data)
										{
											case RIGHT_SIGN:
												r_hw.effectRight=1;
												r_hw.effect_timeRight=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
											break;
											case LEFT_SIGN:
												r_hw.effectLeft=1;
												r_hw.effect_timeLeft=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
											break;
											case LEFT_MID_SIGN:
												r_hw.effectMidLeft=1;
												r_hw.effect_timeMidLeft=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
											break;
											case RIGHT_MID_SIGN:
												r_hw.effectMidRight=1;
												r_hw.effect_timeMidRight=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
											break;
											case NEAR_SIGN:
												r_hw.effectNear=1;
												r_hw.effect_timeNear=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
											break;
											case TOP_SIGN:
												r_hw.effectTop=1;
												r_hw.effect_timeTop=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
												top_time++;
											break;
											case MID_SIGN:
												r_hw.effectMid=1;
												r_hw.effect_timeMid=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
											break;
#ifdef DOCK_NEAR						
											case 0x55:
												r_hw.effectNear=1;
												r_hw.effect_timeNear=giv_sys_time;
												r_hw.effect=1;
												r_hw.effect_time=giv_sys_time;
											break;
#endif
#ifdef REMOTE		
											default:
												Parse_Remote_Signa(&r_hw);
											break;
#endif

										}
								}
						}
					else								 //检测引导码
						{
							sample_time=giv_sys_time-r_hw_time;
							if((sample_time>=START_FRAME_MIN)&&(sample_time<=START_FRAME_MAX))
								{
									r_hw.start=1;
									r_hw.bitnumber=0;
									r_hw.tempdata=0;
								}
						}
#ifdef IR_CORRECT
					r_high_time=giv_sys_time;
#endif
				}
			else										 //下降沿
				{
#ifdef IR_CORRECT
					if(giv_sys_time-r_high_time>2)
#endif
						r_hw_time=giv_sys_time;
					
				}
			EXTI_ClearITPendingBit(EXTI_Line_RHW);
		}

	if(EXTI_GetITStatus(EXTI_Line_RMHW)!=RESET)		 
		{
			if(Read_RM_HW())		 //上升沿
				{
					if(rm_hw.start)
						{
							sample_time=giv_sys_time-rm_hw_time;
							if((sample_time>=BIT0_MIN_SAMPLE)&&(sample_time<=BIT0_MAX_SAMPLE)) 		 //bit '0'
								{
									rm_hw.tempdata<<=1;
									rm_hw.tempdata|=0;
									rm_hw.bitnumber++;
								}
							else if((sample_time>=BIT1_MIN_SAMPLE)&&(sample_time<=BIT1_MAX_SAMPLE))	 //bit '1'
								{
									rm_hw.tempdata<<=1;
									rm_hw.tempdata|=1;
									rm_hw.bitnumber++;
								}

#ifdef INTERFERENCE_ACTION
							//qz add 20180823，抗干扰
							else
								{
									rm_hw.tempdata=0;
									rm_hw.start=0;
									rm_hw.bitnumber=0;
								}
#endif
							if(rm_hw.bitnumber==8)
								{
									rm_hw.data=rm_hw.tempdata;
									rm_hw.tempdata=0;
									rm_hw.start=0;
									switch (rm_hw.data)
										{
											case RIGHT_SIGN:
												rm_hw.effectRight=1;
												rm_hw.effect_timeRight=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
											break;
											case LEFT_SIGN:
												rm_hw.effectLeft=1;
												rm_hw.effect_timeLeft=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
											break;
											case LEFT_MID_SIGN:
												rm_hw.effectMidLeft=1;
												rm_hw.effect_timeMidLeft=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
											break;
											case RIGHT_MID_SIGN:
												rm_hw.effectMidRight=1;
												rm_hw.effect_timeMidRight=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
											break;
											case NEAR_SIGN:
												rm_hw.effectNear=1;
												rm_hw.effect_timeNear=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
											break;
											case TOP_SIGN:
												rm_hw.effectTop=1;
												rm_hw.effect_timeTop=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
												top_time++;
											break;
											case MID_SIGN:
												rm_hw.effectMid=1;
												rm_hw.effect_timeMid=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
											break;
#ifdef DOCK_NEAR						
											case 0x55:
												rm_hw.effectNear=1;
												rm_hw.effect_timeNear=giv_sys_time;
												rm_hw.effect=1;
												rm_hw.effect_time=giv_sys_time;
											break;
#endif
#ifdef REMOTE		
											default:
												Parse_Remote_Signa(&rm_hw);
											break;
#endif

										}
								}
						}
					else								 //检测引导码
						{
							sample_time=giv_sys_time-rm_hw_time;
							if((sample_time>=START_FRAME_MIN)&&(sample_time<=START_FRAME_MAX))
								{
									rm_hw.start=1;
									rm_hw.bitnumber=0;
									rm_hw.tempdata=0;
								}
						}
#ifdef IR_CORRECT
					rm_high_time=giv_sys_time;
#endif
				}
			else										 //下降沿
				{
#ifdef IR_CORRECT
					if(giv_sys_time-rm_high_time>2)
#endif
					rm_hw_time=giv_sys_time;
				}
			EXTI_ClearITPendingBit(EXTI_Line_RMHW);
		}

	if(EXTI_GetITStatus(EXTI_Line_RBHW)!=RESET) 	 
		{
			if(Read_RB_HW())		 //上升沿
				{
					if(rb_hw.start)
						{
							sample_time=giv_sys_time-rb_hw_time;
							if((sample_time>=BIT0_MIN_SAMPLE)&&(sample_time<=BIT0_MAX_SAMPLE))		 //bit '0'
								{
									rb_hw.tempdata<<=1;
									rb_hw.tempdata|=0;
									rb_hw.bitnumber++;
								}
							else if((sample_time>=BIT1_MIN_SAMPLE)&&(sample_time<=BIT1_MAX_SAMPLE))  //bit '1'
								{
									rb_hw.tempdata<<=1;
									rb_hw.tempdata|=1;
									rb_hw.bitnumber++;
								}

#ifdef INTERFERENCE_ACTION
							//qz add 20180823，抗干扰
							else
								{
									rb_hw.tempdata=0;
									rb_hw.start=0;
									rb_hw.bitnumber=0;
								}
#endif
							if(rb_hw.bitnumber==8)
								{
									rb_hw.data=rb_hw.tempdata;
									rb_hw.tempdata=0;
									rb_hw.start=0;
									switch (rb_hw.data)
										{
											case RIGHT_SIGN:
												rb_hw.effectRight=1;
												rb_hw.effect_timeRight=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
											case LEFT_SIGN:
												rb_hw.effectLeft=1;
												rb_hw.effect_timeLeft=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
											case LEFT_MID_SIGN:
												rb_hw.effectMidLeft=1;
												rb_hw.effect_timeMidLeft=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
											case RIGHT_MID_SIGN:
												rb_hw.effectMidRight=1;
												rb_hw.effect_timeMidRight=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
											case NEAR_SIGN:
												rb_hw.effectNear=1;
												rb_hw.effect_timeNear=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
											case TOP_SIGN:
												rb_hw.effectTop=1;
												rb_hw.effect_timeTop=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
											case MID_SIGN:
												rb_hw.effectMid=1;
												rb_hw.effect_timeMid=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
#ifdef DOCK_NEAR						
											case 0x55:
												rb_hw.effectNear=1;
												rb_hw.effect_timeNear=giv_sys_time;
												rb_hw.effect=1;
												rb_hw.effect_time=giv_sys_time;
											break;
#endif
#ifdef REMOTE		
											default:
												Parse_Remote_Signa(&rb_hw);
											break;
#endif

										}
								}
						}
					else								 //检测引导码
						{
							sample_time=giv_sys_time-rb_hw_time;
							if((sample_time>=START_FRAME_MIN)&&(sample_time<=START_FRAME_MAX))
								{
									rb_hw.start=1;
									rb_hw.bitnumber=0;
									rb_hw.tempdata=0;
								}
						}
#ifdef IR_CORRECT
					rb_high_time=giv_sys_time;
#endif
				}
			else										 //下降沿
				{
#ifdef IR_CORRECT
					if(giv_sys_time-rb_high_time>2)
#endif
					rb_hw_time=giv_sys_time;
				}
			EXTI_ClearITPendingBit(EXTI_Line_RBHW);
		}
	 
	if(EXTI_GetITStatus(EXTI_Line_LHW)!=RESET)	 
		{
			if(Read_L_HW()) 	 //上升沿
				{
					if(l_hw.start)
						{
							sample_time=giv_sys_time-l_hw_time;
							if((sample_time>=BIT0_MIN_SAMPLE)&&(sample_time<=BIT0_MAX_SAMPLE))		 //bit '0'
								{
									l_hw.tempdata<<=1;
									l_hw.tempdata|=0;
									l_hw.bitnumber++;
								}
							else if((sample_time>=BIT1_MIN_SAMPLE)&&(sample_time<=BIT1_MAX_SAMPLE))  //bit '1'
								{
									l_hw.tempdata<<=1;
									l_hw.tempdata|=1;
									l_hw.bitnumber++;
								}

#ifdef INTERFERENCE_ACTION
							//qz add 20180823，抗干扰
							else
								{
									l_hw.tempdata=0;
									l_hw.start=0;
									l_hw.bitnumber=0;
								}
#endif
							if(l_hw.bitnumber==8)
								{
									l_hw.data=l_hw.tempdata;
									l_hw.tempdata=0;
									l_hw.start=0;
									switch (l_hw.data)
										{
											case RIGHT_SIGN:
												l_hw.effectRight=1;
												l_hw.effect_timeRight=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
											break;
											case LEFT_SIGN:
												l_hw.effectLeft=1;
												l_hw.effect_timeLeft=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
											break;
											case LEFT_MID_SIGN:
												l_hw.effectMidLeft=1;
												l_hw.effect_timeMidLeft=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
											break;
											case RIGHT_MID_SIGN:
												l_hw.effectMidRight=1;
												l_hw.effect_timeMidRight=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
											break;
											case NEAR_SIGN:
												l_hw.effectNear=1;
												l_hw.effect_timeNear=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
											break;
											case TOP_SIGN:
												l_hw.effectTop=1;
												l_hw.effect_timeTop=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
												top_time++;
											break;
											case MID_SIGN:
												l_hw.effectMid=1;
												l_hw.effect_timeMid=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
											break;
#ifdef DOCK_NEAR						
											case 0x55:
												l_hw.effectNear=1;
												l_hw.effect_timeNear=giv_sys_time;
												l_hw.effect=1;
												l_hw.effect_time=giv_sys_time;
											break;
#endif
#ifdef REMOTE		
											default:
												Parse_Remote_Signa(&l_hw);
											break;
#endif

										}
								}
						}
					else								 //检测引导码
						{
							sample_time=giv_sys_time-l_hw_time;
							if((sample_time>=START_FRAME_MIN)&&(sample_time<=START_FRAME_MAX))
								{
									l_hw.start=1;
									l_hw.bitnumber=0;
									l_hw.tempdata=0;
								}
						}
#ifdef IR_CORRECT
					l_high_time=giv_sys_time;
#endif
				}
			else										 //下降沿
				{
#ifdef IR_CORRECT
					if(giv_sys_time-l_high_time>2)
#endif
					l_hw_time=giv_sys_time;
				}
			EXTI_ClearITPendingBit(EXTI_Line_LHW);
		}

	if(EXTI_GetITStatus(EXTI_Line_LMHW)!=RESET) 	 
		{
			if(Read_LM_HW())		 //上升沿
				{
					if(lm_hw.start)
						{
							sample_time=giv_sys_time-lm_hw_time;
							if((sample_time>=BIT0_MIN_SAMPLE)&&(sample_time<=BIT0_MAX_SAMPLE))		 //bit '0'
								{
									lm_hw.tempdata<<=1;
									lm_hw.tempdata|=0;
									lm_hw.bitnumber++;
								}
							else if((sample_time>=BIT1_MIN_SAMPLE)&&(sample_time<=BIT1_MAX_SAMPLE))  //bit '1'
								{
									lm_hw.tempdata<<=1;
									lm_hw.tempdata|=1;
									lm_hw.bitnumber++;
								}

#ifdef INTERFERENCE_ACTION
							//qz add 20180823，抗干扰
							else
								{
									lm_hw.tempdata=0;
									lm_hw.start=0;
									lm_hw.bitnumber=0;
								}
#endif
							if(lm_hw.bitnumber==8)
								{
									lm_hw.data=lm_hw.tempdata;
									lm_hw.tempdata=0;
									lm_hw.start=0;
									switch (lm_hw.data)
										{
											case RIGHT_SIGN:
												lm_hw.effectRight=1;
												lm_hw.effect_timeRight=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
											break;
											case LEFT_SIGN:
												lm_hw.effectLeft=1;
												lm_hw.effect_timeLeft=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
											break;
											case LEFT_MID_SIGN:
												lm_hw.effectMidLeft=1;
												lm_hw.effect_timeMidLeft=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
											break;
											case RIGHT_MID_SIGN:
												lm_hw.effectMidRight=1;
												lm_hw.effect_timeMidRight=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
											break;
											case NEAR_SIGN:
												lm_hw.effectNear=1;
												lm_hw.effect_timeNear=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
											break;
											case TOP_SIGN:
												lm_hw.effectTop=1;
												lm_hw.effect_timeTop=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
												top_time++;
											break;
											case MID_SIGN:
												lm_hw.effectMid=1;
												lm_hw.effect_timeMid=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
											break;
#ifdef DOCK_NEAR						
											case 0x55:
												lm_hw.effectNear=1;
												lm_hw.effect_timeNear=giv_sys_time;
												lm_hw.effect=1;
												lm_hw.effect_time=giv_sys_time;
											break;
#endif
#ifdef REMOTE		
											default:
												Parse_Remote_Signa(&lm_hw);
											break;
#endif

										}
								}
						}
					else								 //检测引导码
						{
							sample_time=giv_sys_time-lm_hw_time;
							if((sample_time>=START_FRAME_MIN)&&(sample_time<=START_FRAME_MAX))
								{
									lm_hw.start=1;
									lm_hw.bitnumber=0;
									lm_hw.tempdata=0;
								}
						}
#ifdef IR_CORRECT
					lm_high_time=giv_sys_time;
#endif
				}
			else										 //下降沿
				{
#ifdef IR_CORRECT
					if(giv_sys_time-lm_high_time>2)
#endif
					lm_hw_time=giv_sys_time;
				}
			EXTI_ClearITPendingBit(EXTI_Line_LMHW);
		}

	if(EXTI_GetITStatus(EXTI_Line_LBHW)!=RESET) 	 
		{
			if(Read_LB_HW())		 //上升沿
				{
					if(lb_hw.start)
						{
							sample_time=giv_sys_time-lb_hw_time;
							if((sample_time>=BIT0_MIN_SAMPLE)&&(sample_time<=BIT0_MAX_SAMPLE))		 //bit '0'
								{
									lb_hw.tempdata<<=1;
									lb_hw.tempdata|=0;
									lb_hw.bitnumber++;
								}
							else if((sample_time>=BIT1_MIN_SAMPLE)&&(sample_time<=BIT1_MAX_SAMPLE))  //bit '1'
								{
									lb_hw.tempdata<<=1;
									lb_hw.tempdata|=1;
									lb_hw.bitnumber++;
								}

#ifdef INTERFERENCE_ACTION
							//qz add 20180823，抗干扰
							else
								{
									lb_hw.tempdata=0;
									lb_hw.start=0;
									lb_hw.bitnumber=0;
								}
#endif
							if(lb_hw.bitnumber==8)
								{
									lb_hw.data=lb_hw.tempdata;
									lb_hw.tempdata=0;
									lb_hw.start=0;
									switch (lb_hw.data)
										{
											case RIGHT_SIGN:
												lb_hw.effectRight=1;
												lb_hw.effect_timeRight=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
											case LEFT_SIGN:
												lb_hw.effectLeft=1;
												lb_hw.effect_timeLeft=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
											case LEFT_MID_SIGN:
												lb_hw.effectMidLeft=1;
												lb_hw.effect_timeMidLeft=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
											case RIGHT_MID_SIGN:
												lb_hw.effectMidRight=1;
												lb_hw.effect_timeMidRight=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
											case NEAR_SIGN:
												lb_hw.effectNear=1;
												lb_hw.effect_timeNear=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
											case TOP_SIGN:
												lb_hw.effectTop=1;
												lb_hw.effect_timeTop=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
											case MID_SIGN:
												lb_hw.effectMid=1;
												lb_hw.effect_timeMid=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
#ifdef DOCK_NEAR						
											case 0x55:
												lb_hw.effectNear=1;
												lb_hw.effect_timeNear=giv_sys_time;
												lb_hw.effect=1;
												lb_hw.effect_time=giv_sys_time;
											break;
#endif
#ifdef REMOTE		
											default:
												Parse_Remote_Signa(&lb_hw);
											break;
#endif

										}
								}
						}
					else								 //检测引导码
						{
							sample_time=giv_sys_time-lb_hw_time;
							if((sample_time>=START_FRAME_MIN)&&(sample_time<=START_FRAME_MAX))
								{
									lb_hw.start=1;
									lb_hw.bitnumber=0;
									lb_hw.tempdata=0;
								}
						}
#ifdef IR_CORRECT
					lb_high_time=giv_sys_time;
#endif
				}
			else										 //下降沿
				{
#ifdef IR_CORRECT
					if(giv_sys_time-lb_high_time>2)
#endif
					lb_hw_time=giv_sys_time;
				}
			EXTI_ClearITPendingBit(EXTI_Line_LBHW);
		}
}
//======================================================================================
//======================================================================================











 



//======================================================================================
//======================================================================================
void EXTI9_5_IRQHandler(void)
{
	u8 i=0;

	 /////////////左碰撞中断///////////////
	if(EXTI_GetFlagStatus(EXTI_Line_LBUMP)!=RESET)			//左碰撞
		{
			EXTI_ClearITPendingBit(EXTI_Line_LBUMP);
				{
#ifdef RING_PWM_CTL
					i=0;
					while(i<10)
						{
							i++;
						}
					if(!Read_L_Bump())
						{
							bump_value|=L_BUMP_BIT;
							l_bump.key=0;
						}
					if(!Read_R_Bump())
						{
							bump_value|=R_BUMP_BIT;
							r_bump.key=0;
						}
#if 0
					if(!Read_RM_Bump())
						{
							bump_value|=RM_BUMP_BIT;
							rm_bump.key=0;
						}
					if(!Read_LM_Bump())
						{
	
							bump_value|=LM_BUMP_BIT;
							lm_bump.key=0;
						}
#endif
					if(bump_value)
						{
							Close_Bump_Exit();
						}
#else
					for(i=0;i<3;i++)
						{
							if(!Read_L_Bump())
							{
								if(i==2)
								{
									bump_value|=L_BUMP_BIT;
									l_bump.key=0;
								}
							}
							else
							{
								break;
							}
						}
					if(bump_value)
						{
							Close_Bump_Exit();
						}
#endif
				}
		}

	 ////////////右碰撞中断///////////////////
	 if(EXTI_GetFlagStatus(EXTI_Line_RBUMP)!=RESET) 		//右碰撞
		{
			EXTI_ClearITPendingBit(EXTI_Line_RBUMP);
				{
#ifdef RING_PWM_CTL
					i=0;
					while(i<10)
						{
							i++;
						}
					if(!Read_L_Bump())
						{
							bump_value|=L_BUMP_BIT;
							l_bump.key=0;
						}
					if(!Read_R_Bump())
						{
							bump_value|=R_BUMP_BIT;
							r_bump.key=0;
						}
#if 0
					if(!Read_RM_Bump())
						{
							bump_value|=RM_BUMP_BIT;
							rm_bump.key=0;
						}
					if(!Read_LM_Bump())
						{
	
							bump_value|=LM_BUMP_BIT;
							lm_bump.key=0;
						}
#endif
					if(bump_value)
						{
							Close_Bump_Exit();
						}
#else
					for(i=0;i<3;i++)
						{
							if(!Read_R_Bump())
							{
								if(i==2)
								{
									bump_value|=R_BUMP_BIT;
									r_bump.key=0;
								}
							}
							else
							{
								break;
							}
						}
					if(bump_value)
						{
							Close_Bump_Exit();
						}
#endif
				}
		}

	 if(EXTI_GetFlagStatus(EXTI_Line8)!=RESET) 		
		{
			EXTI_ClearITPendingBit(EXTI_Line8);
	 	}
}
//======================================================================================
//======================================================================================




















//==================================================================================
/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
//uint32 alr;
  if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
		{
	 // 	  ExitInput	= TRUE;
				RTC_ClearITPendingBit(RTC_IT_ALR);
				EXTI_ClearITPendingBit(EXTI_Line17);
		}
}
//==================================================================================
#ifdef 	RTC_8M_CORR	
unsigned int   second_old_time   = 0;
#endif
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC)!=RESET)
		{
#ifdef 	RTC_8M_CORR	
			rtc_8m_mode  ++;
			if( rtc_8m_mode == END_CORR)	 
				second_time = giv_sys_time -second_old_time ; 
			if( rtc_8m_mode == BEGIN_CORR_2)	  
				second_old_time = giv_sys_time;					
#endif			
			RTC_GetCounter();
			RTC_ClearITPendingBit(RTC_IT_SEC);
		}			
}


#ifdef   NEW_Q55_BOARD_1113   
#ifdef TUYA_WIFI
void USART2_IRQHandler(void)
{
#if 1
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //
	{ 
#ifdef TUYA_WIFI
		uart_receive_input((uint8_t)USART_ReceiveData(USART2));
#endif
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	/* 溢出 */
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)  
		{ 
			USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR 
			USART_ReceiveData(USART2); //读DR 
		} 
#else
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_IDLE)!=RESET)
		{
			USART_ClearITPendingBit(USART1, USART_IT_IDLE);
			Res=USART1->SR;
			Res=USART1->DR;
			Res=Res;
			DMA_Cmd(DMA1_Channel5,DISABLE);
			//	UART1.RevLength=USART1_RX_SIZE;
			//	UART1.RevLength=DMA_GetCurrDataCounter(DMA1_Channel5);
			UART1.RevLength=USART1_RX_SIZE-DMA_GetCurrDataCounter(DMA1_Channel5);
			DMA_SetCurrDataCounter(DMA1_Channel5,USART1_RX_SIZE);

			DMA_Cmd(DMA1_Channel5, DISABLE);
			UART1.Rece_Done=true;	
		}
#endif
}

#endif

#else //NEW_Q55_BOARD_1113   



#ifdef TUYA_WIFI
void USART1_IRQHandler(void)
{
#if 1
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //
	{ 
#ifdef TUYA_WIFI
		uart_receive_input((uint8_t)USART_ReceiveData(USART1));
#endif
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	/* 溢出 */
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)  
		{ 
			USART_ClearFlag(USART1,USART_FLAG_ORE); //读SR 
			USART_ReceiveData(USART1); //读DR 
		} 
#else
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_IDLE)!=RESET)
		{
			USART_ClearITPendingBit(USART1, USART_IT_IDLE);
			Res=USART1->SR;
			Res=USART1->DR;
			Res=Res;
			DMA_Cmd(DMA1_Channel5,DISABLE);
			//	UART1.RevLength=USART1_RX_SIZE;
			//	UART1.RevLength=DMA_GetCurrDataCounter(DMA1_Channel5);
			UART1.RevLength=USART1_RX_SIZE-DMA_GetCurrDataCounter(DMA1_Channel5);
			DMA_SetCurrDataCounter(DMA1_Channel5,USART1_RX_SIZE);

			DMA_Cmd(DMA1_Channel5, DISABLE);
			UART1.Rece_Done=true;	
		}
#endif
}
#endif
#endif //  NEW_Q55_BOARD_1113   

void USART3_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_IDLE)!=RESET)
		{
			USART_ClearITPendingBit(USART3, USART_IT_IDLE);
			Res=USART3->SR;
			Res=USART3->DR;
			Res=Res;
			DMA_Cmd(DMA1_Channel3,DISABLE);
			//UART2.RevLength=23-DMA_GetCurrDataCounter(DMA1_Channel6);
			UART3.RevLength=USART3_RX_SIZE-DMA_GetCurrDataCounter(DMA1_Channel3);
			DMA_SetCurrDataCounter(DMA1_Channel3,USART3_RX_SIZE);

			Uart3_Communication_II();
			Cal_xy();
			UART3.Rece_Done=true;
		}
}

//ADC DMA传输完成中断
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1))
		{
			ADC_Cmd(ADC1,DISABLE);
			ADC_SoftwareStartConvCmd(ADC1, DISABLE);  
			DMA_Cmd(DMA1_Channel1, DISABLE);


			DMA_SetCurrDataCounter(DMA1_Channel1, 256);
			ADC_Cmd(ADC1,ENABLE);
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			DMA_Cmd(DMA1_Channel1,ENABLE);
			DMA_ClearITPendingBit(DMA1_IT_TC1);
			
		}
}

//DMA1_Channel4即USART1的DMA传输中断函数  QZ ADD
void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_FLAG_TC4))
		{
			//DMA_ClearFlag(DMA1_FLAG_TC4);
			DMA_ClearITPendingBit(DMA1_FLAG_TC4);
			DMA_Cmd(DMA1_Channel4,DISABLE);
			UART1.Trans_Busy=false;
		}
}

//DMA1_Channel7即USART2的DMA传输中断函数  QZ ADD
void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_FLAG_TC7))
		{
			//DMA_ClearFlag(DMA1_FLAG_TC4);
			DMA_ClearITPendingBit(DMA1_FLAG_TC7);
			DMA_Cmd(DMA1_Channel7,DISABLE);
#ifdef UART_TWOBUF
			if(dma_buf_num==1)
				buf1_full=false;
			else if(dma_buf_num==2)
				buf2_full=false;
			else if(dma_buf_num==3)
				buf3_full=false;
			
			if(buf1_full)
				{
					DMA_USART2_TX_Address((uint32_t)UART_BUF1,strlen((const char*)UART_BUF1));
					//buf1_full=false;
				}
			else if(buf2_full)
				{
					DMA_USART2_TX_Address((uint32_t)UART_BUF2,strlen((const char*)UART_BUF2));
					//buf2_full=false;
				}
			else if(buf3_full)
				{
					DMA_USART2_TX_Address((uint32_t)UART_BUF3,strlen((const char*)UART_BUF3));
					
					//DMA_USART2_TX_Address((uint32_t)UART_BUF3,buf3_cnt);							
					buf3_cnt=0;
					//buf3_full=false;
				}
			else
				{
					uart_dma_busy=false;
				}
#endif
#ifdef   NEW_Q55_BOARD_1113   
   UART2.Trans_Busy=false;
#else
UART1.Trans_Busy=false;

#endif			
		}
}


