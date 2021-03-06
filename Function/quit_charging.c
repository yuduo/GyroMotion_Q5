#include "AAA-Include.h"

void Init_Quit_Charging(u8 sweep_method)
{
	Init_Sweep_Pwm(PWM_SWEEP_MAX,PWM_SWEEP_PRESCALER);
	mode.last_mode=mode.mode;
	mode.last_sub_mode=mode.sub_mode;
	mode.mode=MODE_CEASE;
	mode.sub_mode=QUIT_CHARGING;

	Enable_wall();
	Enable_earth();
	enable_hwincept();
	Init_Action();
	Enable_Speed();
//	mode.step=0x01;
	mode.step=0x00;				//��ʼ��mode.step
	mode.time=giv_sys_time;
	mode.sweep_method=sweep_method;
	WriteWorkState();

	Sweep_Level_Set(SWEEP_LEVEL_STOP);
	//mode.Info_Abort=1;		//qz add 20180316:���ε��������� //qz mask 20180522
	mode.Info_Abort=0;

	CHECK_STATUS_FLAG=true;
	Init_Check_Status();

#ifdef DEBUG_Enter_Mode
	TRACE("Init Quit_Charge Complete!\r\n");
#endif

#ifdef UV
	Reset_UV();
#endif
	Close_Led();
	if(mode.low_power)
		{
			Send_Voice(VOICE_POWER_LOW);
			mode.step=0xf0;
			mode.time=giv_sys_time;
		}


}

void Do_Quit_Chargeing(void)
{
	static short tgt_angle=0,anti_tgt_angle;
	#if 0
	//if((power.charge_seat==1)&&(mode.step==0x00))
	if(mode.step==0)
		{

			mode.step++;
//			mode.Info_Abort=1;
		}
	#endif


	ACC_DEC_Curve();

	switch(mode.step)
		{
			case 0:
				if(giv_sys_time-mode.time<2000)
					return;
				tgt_angle=Gyro_Data.yaw;
				anti_tgt_angle=Get_Reverse_Angle(tgt_angle);

#ifdef TUYA_WIFI
				Reset_Map();
#endif
				mode.step++;
			case 0x01:
				Speed=HIGH_MOVE_SPEED;
				if(do_action(4,20*CM_PLUS))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 0x02:
				Speed=TURN_SPEED;
				do_action(2,360*Angle_1);
				if(Judge_Yaw_Reach(anti_tgt_angle,TURN_ANGLE_BIOS,1))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 3:
				Speed=HIGH_MOVE_SPEED;
				if(do_action_my(3,10*CM_PLUS,anti_tgt_angle))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 4:
				motion1.start_seat=true;
				switch(mode.sweep_method)
					{
						case SWEEP_METHOD_GUIHUA:									
							Init_First_Sweep(1);
							break;
						case SWEEP_METHOD_YBS:
							Reset_XY();
							//delay_ms(3000);
							Init_Right_YBS(1,1);
							break;
						case SWEEP_METHOD_SPOT:
							Reset_XY();
							//delay_ms(3000);
							Init_Sweep_Spot(SPOT_FROM_CHARGE);
							break;
					}
				break;
			case 0xf0:
				if(giv_sys_time-mode.time<30000)
					return;
				Init_Cease();
				break;
				
		}
}

