#include "AAA-include.h"
#define	YBS_DO_TIME 20000 //5S
#define LOW_AVOID_SPEED		200
#define	HIGH_AVOID_SPEED	600

s8 bump_gridx,bump_gridy;	//碰撞点的XY坐标信息

u8 Check_Xmax_Out(void)
{
	if((Gyro_Data.x_pos>motion1.xpos_start+RANGE_MAX)&(motion1.tgt_yaw==F_Angle_Const)) 	//qz modify 20190308
		{
//			stop_rap();
			return 1;
		}

	if((Gyro_Data.x_pos>motion1.xpos_max_area)&(motion1.tgt_yaw==F_Angle_Const))	//qz modify 20190308
		{
//			stop_rap();
			return 1;
		}
	return 0;
}

u8 Check_Xmin_Out(void)
{
	if((Gyro_Data.x_pos<motion1.xpos_start-RANGE_MIN)&(motion1.tgt_yaw==B_Angle_Const)) 		//qz add 20190308
		{
//			stop_rap();
			return 1;
		}

	if((Gyro_Data.x_pos<=motion1.xpos_min_area)&(motion1.tgt_yaw==B_Angle_Const))		//qz add 20190308
		{
//			stop_rap();
			return 1;
		}
	return 0;
}

u8 Check_Ymax_Out(void)
{
	if(Gyro_Data.y_pos>motion1.ypos_start+RANGE_MAX+10)
		{
//			stop_rap();
			return 1;
		}

	if(Gyro_Data.y_pos>motion1.ypos_max_area+10)
		{
//			stop_rap();
			return 1;
		}
	return 0;
}

u8 Check_Ymin_Out(void)
{
	if(Gyro_Data.y_pos<motion1.ypos_start-RANGE_MIN-10)
		{
			return 1;
		}

	if(Gyro_Data.y_pos<motion1.ypos_min_area-10)
		{
			return 1;
		}
	return 0;
}

u8 Check_OutofRange(void)
{
#if 0
	if(Gyro_Data.y_pos>motion1.ypos_start+RANGE_MAX+10)
		{
//			stop_rap();
			return BUMP_YOUTRANGE;
		}
	if(Gyro_Data.y_pos<motion1.ypos_start-RANGE_MIN-10)
		{
			return BUMP_YOUTRANGE;
		}
	if(Gyro_Data.y_pos>motion1.ypos_max_area+10)
		{
//			stop_rap();
			return BUMP_YOUTRANGE;
		}
	if(Gyro_Data.y_pos<motion1.ypos_min_area-10)
		{
			return BUMP_YOUTRANGE;
		}

	if((Gyro_Data.x_pos>motion1.xpos_start+RANGE_MAX)&(motion1.tgt_yaw==F_Angle_Const)) 	//qz modify 20190308
		{
//			stop_rap();
			return BUMP_XOUTRANGE;
		}
	if((Gyro_Data.x_pos<motion1.xpos_start-RANGE_MIN)&(motion1.tgt_yaw==B_Angle_Const)) 		//qz add 20190308
		{
//			stop_rap();
			return BUMP_XOUTRANGE;
		}
	
	if((Gyro_Data.x_pos>motion1.xpos_max_area)&(motion1.tgt_yaw==F_Angle_Const))	//qz modify 20190308
		{
//			stop_rap();
			return BUMP_XOUTRANGE;
		}
	if((Gyro_Data.x_pos<=motion1.xpos_min_area)&(motion1.tgt_yaw==B_Angle_Const))		//qz add 20190308
		{
//			stop_rap();
			return BUMP_XOUTRANGE;
		}
	return 0;
#else
	u8 xmax_out=0,xmin_out=0,ymax_out=0,ymin_out=0,data=0;
	u32 result=0;
	xmax_out=Check_Xmax_Out();
	xmin_out=Check_Xmin_Out();
	ymax_out=Check_Ymax_Out();
	ymin_out=Check_Ymin_Out();
	data=(xmax_out<<3)+(xmin_out<<2)+(ymax_out<<1)+ymin_out;

	switch(data)
		{
			case 10:
				result=BUMP_RF_OUT;
				break;
			case 9:
				result=BUMP_LF_OUT;
				break;
			case 6:
				result=BUMP_RB_OUT;
				break;
			case 5:
				result=BUMP_LB_OUT;
				break;
			case 8:
				result=BUMP_XMAX_OUT;
				break;
			case 4:
				result=BUMP_XMIN_OUT;
				break;
			case 2:
				result=BUMP_YMAX_OUT;
				break;
			case 1:
				result=BUMP_YMIN_OUT;
				break;
			default:
				result=0;
				break;
		}
	return result;
#endif
}

u8 Check_Already_Clean(void)
{
	u8 area_no1;
	s8 now_gridx,now_gridy,temp_gridx1,temp_gridx2;
	now_gridx=grid.x;now_gridy=grid.y;
	if(motion1.tgt_yaw==F_Angle_Const)
		{
			temp_gridx1=now_gridx+1;
			if(temp_gridx1>grid.x_area_max)
				temp_gridx1=grid.x_area_max;
			temp_gridx2=now_gridx+2;
			if(temp_gridx1>grid.x_area_max)
				temp_gridx1=grid.x_area_max;
			area_no1=Read_Coordinate_AreaNo(temp_gridx1,now_gridy);
			if((area_no1!=0)&(area_no1!=motion1.area_num))
				return BUMP_XMAX_OUT;
		}
	else
		{
			temp_gridx1=now_gridx-1;
			temp_gridx2=now_gridx-2;
			if(temp_gridx1<grid.x_area_min)
				temp_gridx1=grid.x_area_min;
			if(temp_gridx2<grid.x_area_min)
				temp_gridx2=grid.x_area_min;
			area_no1=Read_Coordinate_AreaNo(temp_gridx1,now_gridy);
			if((area_no1!=0)&(area_no1!=motion1.area_num))
				return BUMP_XMIN_OUT;
		}
	return 0;
}

u8 Read_Only_Collide(void)
{
	u8 data1;
	
	data1=Parse_BumpValue();
	switch (data1)
	{
		case BUMP_ONLY_LEFT:						 //左碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {
					 stop_rap();
					 mode.bump=BUMP_ONLY_LEFT;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.l_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_LEFT;
		 case BUMP_ONLY_LEFTMID:					 //左中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {
					 stop_rap();
					 mode.bump=BUMP_ONLY_LEFTMID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.l_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_LEFTMID;
		 case BUMP_LEFT_MID:						 //左左中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_LEFT_MID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.l_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_LEFT_MID;
		 case BUMP_ONLY_RIGHT:						 //右碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_ONLY_RIGHT;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.r_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_RIGHT;
		 case BUMP_ONLY_RIGHTMID:					 //右中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_ONLY_RIGHTMID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.r_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_RIGHTMID;
		 case BUMP_RIGHT_MID:						 //右右中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_RIGHT_MID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.r_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_RIGHT_MID;
		 case BUMP_MID: 							 //中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_MID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.m_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_MID;
	}
	return 0;
}

/*--------------------
输入参数：ir_enable
			ir_enable=1,加入墙检红外碰撞信息
			ir_enable=0,取消墙检红外碰撞信息
		  out_enable
		    1,加入区域边界出界信息
		    0,取消区域边界出界信息
输出参数：碰撞结果
0，没有碰撞
----------------------*/
u8 Read_Cliff(void)
{
	if(e_m.sign==FARN)
		{
			return BUMP_MID_CLIFF;
		}

	if(e_l.sign==FARN)
		{
			if(e_r.sign==FARN)
				return BUMP_MID_CLIFF;
			else
				return BUMP_LEFT_CLIFF;
		}

	if(e_r.sign==FARN)
		{
			return BUMP_RIGHT_CLIFF;
		}
	return 0;
}

u8 Read_Sweep_Bump(u8 ir_enable,u8 out_enable)
{
	u32 data1=0;
	static u32 find_seat_time=0;
	static bool find_seat_first=false;
#ifdef CLIFF_ENABLE			//ZDK屏蔽
	data1=Read_Cliff();
	if(data1)
		{
			if((mode.bump>BUMP_ALL_CLIFF)|(mode.bump==0))
				{
					stop_rap();
					mode.bump=data1;
					mode.step_bp=0;
					
#ifdef EARTH_IN_TIM2
					enable_pwm(L_BACK,1200);
					enable_pwm(R_BACK,1200);
					l_rap.ori=BACK;
					r_rap.ori=BACK;
#endif
				}
			return data1;
		}
#endif	 

	data1=Parse_BumpValue();
	switch (data1)
	{
		case BUMP_ONLY_LEFT:						 //左碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {
					 stop_rap();
					 mode.bump=BUMP_ONLY_LEFT;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.l_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_LEFT;
		 case BUMP_ONLY_LEFTMID:					 //左中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {
					 stop_rap();
					 mode.bump=BUMP_ONLY_LEFTMID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.l_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_LEFTMID;
		 case BUMP_LEFT_MID:						 //左左中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_LEFT_MID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.l_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_LEFT_MID;
		 case BUMP_ONLY_RIGHT:						 //右碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_ONLY_RIGHT;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.r_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_RIGHT;
		 case BUMP_ONLY_RIGHTMID:					 //右中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_ONLY_RIGHTMID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.r_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_ONLY_RIGHTMID;
		 case BUMP_RIGHT_MID:						 //右右中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_RIGHT_MID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.r_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_RIGHT_MID;
		 case BUMP_MID: 							 //中碰撞
			 if((!mode.bump_flag)&(mode.bump==0))
				 {							 
					 stop_rap();
					 mode.bump=BUMP_MID;
					 mode.bump_flag=true;
					 mode.step_bp=0;
					 Slam_Data.m_bump_flag=true;
					 mode.bump_time=giv_sys_time;
				 }
			 return BUMP_MID;
	}

	if((w_m.sign == NEAR)&(ir_enable))
		{
			if((mode.bump == 0)|(mode.bump==BUMP_SEAT))		//中墙检靠近墙
				{
					stop_rap();
					mode.bump=9;//W_M;
					mode.step_bp=0;
					mode.bump_time=giv_sys_time;
				}
			return 9;//W_M;
		}
	
#if 0	//QZ:原来位0	
	if((w_lm.sign == NEAR)&(ir_enable))
		{
			if((mode.bump == 0))		 //左中墙检靠近墙
				{
					stop_rap();
					mode.bump=LM_WALL_BUMP;//W_LM;
					mode.step_bp=0;
					bump_gridx=grid.x;
					bump_gridy=grid.y;
				}
			return LM_WALL_BUMP;//W_LM;
		}

	if((w_rm.sign == NEAR)&(ir_enable))
		{
			if((mode.bump == 0))		 //左中墙检靠近墙
				{
					stop_rap();
					mode.bump=RM_WALL_BUMP;//W_LM;
					mode.step_bp=0;
					bump_gridx=grid.x;
					bump_gridy=grid.y;
				}
			return RM_WALL_BUMP;//W_LM;
		}
#endif

#ifdef FREE_SKID_CHECK
	if(Check_Free_Sikd())
		{
			Slam_Data.skid_flag=1;
#ifdef SKID_REPORT_TIME
			Slam_Data.skid_report_time=giv_sys_time;
#endif
#ifdef FREE_SKID_ACTION
			stop_rap();
			mode.bump=0xff;
			mode.step_bp=0;
			mode.Info_Abort=1;
#endif
		}
#endif

#ifdef COMMANDER_SEAT_REPORT						//qz add 20180814
	if(r_hw.effectNear|r_hw.effectTopReal)
		{
			Slam_Data.ir_flag=true;
		}
#endif

#ifdef SLAM_FIND_SEAT
	if(SLAM_DOCK)
		{
			data1=ReadHwSign_My();
			if(data1&0xD1111)						//only top signal
				{
					if(mode.bump==0)
						{
							stop_rap();
							mode.bump=0xfe;
							mode.step_bp=0;
							mode.Info_Abort=1;
						}
				}
		}
#endif

#if 1
	if(out_enable)
		{
			data1=Check_OutofRange();
			if(data1)
				{
					//if(mode.bump<BUMP_OUTRANGE)
					if(mode.bump==0)
						{
							stop_rap();
							//mode.bump=BUMP_OUTRANGE;
							mode.bump=data1;
							mode.step_bp=0;
						}
					//return BUMP_OUTRANGE;
					return data1;
				}
		}
#endif

	data1=Check_Already_Clean();
	if(data1)
		{
			if(mode.bump==0)
				{
					stop_rap();
					TRACE("Check Already clean!!!\r\n");
					mode.bump=data1;
					TRACE("Force mode.bump=%d\r\n",mode.bump);
					mode.step_bp=0;
				}
			return data1;
		}

	//if((l_hw.effectTop|lm_hw.effectTop)|(rm_hw.effectTop|r_hw.effectTop))
	//if(lm_hw.effectTop&rm_hw.effectTop)
	if(top_time_sec>=10)
		{
			if(!find_seat_first)
				{
					find_seat_time=giv_sys_time;
					find_seat_first=true;
				}
			//if((find_seat_first)&(giv_sys_time-find_seat_time>10000))
				{
					if(mode.bump==0)
						{
							stop_rap();
							mode.bump=BUMP_SEAT;
							mode.step_bp=0;
							find_seat_first=false;
						}
				}
			return BUMP_SEAT;
		}
	else
		{
			find_seat_first=false;
		}
	return 0;		
}

void Init_First_Sweep(u8 start_seat)
{
	init_wallearth();
	enable_hwincept();
	Enable_wall();
	Enable_earth();
	Enable_Speed();

	mode.mode=SWEEP;
	mode.sub_mode=SWEEP_FIRST_INIT;
	mode.step=0;
	if(start_seat)
		{
			motion1.start_seat=true;
			Set_Seat_Grid();
		}
	else
		motion1.start_seat=false;
	motion1.pathpoint_ok=true;
	Set_AreaWorkTime(20);
}

//输入参数
//tgt_yaw：开始新区域清扫时正对的角度，
//X_ACC: 0:按X轴负清扫,1:X轴正方向,2:X轴双方向
//y_acc:开始新区域清扫时相对Y轴的方向，1：正增长方向，0：负增长方向，2：Y轴双方向
void Init_Init_Sweep(short tgt_yaw,u8 x_acc,u8 y_acc)
{
	delay_ms(100);
//	F_Angle=tgt_yaw;
//	B_Angle=Get_Reverse_Angle(F_Angle);
	TRACE("Enter Init_Init_Sweep!\r\n");

	Set_Motion_BackSweep(0);								//qz add 20190328
	Set_Motion_Acc(x_acc,y_acc);
	motion1.sweep_time++;
	motion1.exit_area_num=motion1.area_num;
	motion1.area_num=motion1.sweep_time;
	motion1.continue_checkstep=0;
	motion1.area_ok=false;
	motion1.ymax_ok=false;
	motion1.ymin_ok=false;
	motion1.xmax_ok=false;
	motion1.xmin_ok=false;
	
//	Del_All_Sweep_Bump_Node();
	grid.x_sweep_start=grid.x;
	grid.y_sweep_start=grid.y;

 	Cal_PosArea_Max();
	if(Add_AreaNode_End())
		{
			stop_rap();
			error_code=SEND_ERROR_NODEMALLOC;
			Init_Err();
			return;
		}
	Load_CurrNode_Info();
	Init_NormalSweep(tgt_yaw);		
	switch(motion1.y_acc)
		{
			case 0:
				Set_Motion_YDir(-1);
				break;
			case 1:
				Set_Motion_YDir(1);
				break;
			case 2:
				Set_Motion_YDir(0);
				break;
		}
	if(motion1.x_acc==2)
		motion1.repeat_sweep=true;		//第一次清扫，如果是X轴是双方向清扫，则设置重复扫一次，单方向的，则没有必要
	Set_AreaWorkTime(20);
}

void Do_FirstInit_Sweep(void)
{
	switch(mode.step)
		{
			case 0:
				Send_Voice(VOICE_SWEEP_START);
				Reset_XY();
				mode.time=giv_sys_time;
				Open_Led(1,0,1);
				mode.step++;
				break;

			case 1:
				if(giv_sys_time-mode.time<30000)
					return;
				
				Init_Coordinate();
				Get_Const_Angle();
				if(Init_PathPoint())
					{
						stop_rap();
						error_code=SEND_ERROR_PATHMALLOC;
						Init_Err();
						return;
					}
				//Delete_All_PathPoint();
				if(Creat_AreaNodeList())
					{
						stop_rap();
						error_code=SEND_ERROR_NODEMALLOC;
						Init_Err();
						return;
					}
				if(Init_BackHead())
					{
						stop_rap();
						error_code=SEND_ERROR_BACKMALLOC;
						Init_Err();
						return;
					}
				Set_Motion_BackSweep(0);								//qz add 20190328
				motion1.xpos_start=Gyro_Data.x_pos; 			//qz add 20190307
				motion1.ypos_start=Gyro_Data.y_pos; 			//qz add 20190307
				motion1.sweep_time=0;
				motion1.continue_checkstep=0;
				mode.status=1;
				TRACE("Enter Init_First_Sweep!\r\n");
				TRACE("F_Angle_Const=%d\r\n",F_Angle_Const);
				TRACE("B_Angle_Const=%d\r\n",B_Angle_Const);
				TRACE("L_Angle_Const=%d\r\n",L_Angle_Const);
				TRACE("R_Angle_Const=%d\r\n",R_Angle_Const);
				TRACE("LF_Angle_Const=%d\r\n",LF_Angle_Const);
				TRACE("LB_Angle_Const=%d\r\n",LB_Angle_Const);
				TRACE("RF_Angle_Const=%d\r\n",RF_Angle_Const);
				TRACE("RB_Angle_Const=%d\r\n",RB_Angle_Const);
				TRACE("X Range max=%dcm\r\n",motion1.xpos_start+RANGE_MAX);
				TRACE("X Range min=%dcm\r\n",motion1.xpos_start-RANGE_MIN);
				TRACE("GRID_MAX=%d\r\n",GRID_MAX);
				TRACE("GRID_MIN=%d\r\n",GRID_MIN);
				Cal_Grid_Pos();
				Init_Check_Status();
				Sweep_Level_Set(sweep_suction);
				Init_Init_Sweep(F_Angle_Const,2,1); 		//第一次清扫,X轴双方向,Y轴正向
			//	Del_All_Sweep_Bump_Node();
		}
}

void Sweep_Bump_Action(u8 ir_enable,u8 out_enable)
{
	u8 m=0;
	static u8 turn_dir=0,turn_angle=0,cliff_time=0;
	static short tgt_angle=0;
	s8 now_gridx,now_gridy,last_gridy,next_gridy,ydir;
	now_gridx=grid.x;now_gridy=grid.y;
	ydir=Read_Motion_YDir();

	if(ydir>0)
		{
			last_gridy=bump_gridy-1;
			next_gridy=bump_gridy+1;
		}
	if(ydir<0)
		{
			last_gridy=bump_gridy+1;
			next_gridy=bump_gridy-1;
		}
	
	m=Read_Sweep_Bump(ir_enable,out_enable);
	switch(mode.bump)
		{
			case BUMP_MID_CLIFF:
			case BUMP_LEFT_CLIFF:
			case BUMP_RIGHT_CLIFF:
				switch(mode.step_bp)
					{
						case 0:
							Set_Coordinate_Wall(now_gridx,now_gridy);
							mode.step_bp++;
							cliff_time=0;
							break;
						case 1:
							Speed=TOP_MOVE_SPEED;
							if(do_action(4,CLIFF_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									if(!Read_Cliff())
										{
											mode.step_bp++;
											return;
										}
									else
										{
											Set_Coordinate_Wall(now_gridx,now_gridy);
											cliff_time++;
										}
									if(cliff_time>3)
										{
											error_code=ERROR_LIFT;
											Send_Voice(VOICE_ERROR_DANGER);
											Init_Err();
										}
								}
							if(!Read_Cliff())
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 2:
							if((motion1.repeat_sweep)&(!Read_Motion_BackSweep()))
								{
									TRACE("call this in %d %s\r\n",__LINE__,__func__);
									Init_Pass2Sweep();
									motion1.repeat_sweep=false;
									return;
								}
							if(!Read_LeftRight())		//需要准备左沿边
								{
									turn_dir=2;			//左沿边，碰撞向右转
								}
							else						//需要准备右沿边
								{
									turn_dir=1;			//右沿边，碰撞向左转
								}
							mode.step_bp++;
						case 3:
							Speed=TURN_SPEED;
							if(do_action(turn_dir,90*Angle_1))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 4:
							mode.bump=0;
							mode.step_bp=0;
							mode.bump_flag=false;
							if(!Read_LeftRight())		//需要准备左沿边
								{Init_Sweep_LeftYBS(1);}
							else
								{Init_Sweep_RightYBS(1);}
							mode.bump_time=giv_sys_time;
							break;
					}
				break;
			case 9:
			case BUMP_MID:
				switch(mode.step_bp)
					{
						case 0:
							mode.bump_time=giv_sys_time;
							mode.step_bp++;
							bump_gridy=grid.y;
							bump_gridx=grid.x;
							break;
						case 1:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							Set_Coordinate_Wall(now_gridx, now_gridy);
							if(mode.bump==90)
								mode.step_bp++;
							else
								mode.step_bp+=2;
							break;
						case 2:
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
									mode.bump_time=giv_sys_time;
								}
							break;
						case 3:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							if(Analysis_NeedBack(grid.y))		//判断回扫条件
								{
									mode.step_bp=10;
								}
							else
								{
									mode.step_bp++;
								}
							break;
						case 4:
							if((motion1.repeat_sweep)&(!Read_Motion_BackSweep()))
								{
									TRACE("mode.bump=%d\r\n",mode.bump);
									TRACE("call this in %d %s\r\n",__LINE__,__func__);
									Init_Pass2Sweep();
									motion1.repeat_sweep=false;
									return;
								}
							if(Read_Motion_BackSweep())
								{
									if(Analysis_StopBack_InBump(ydir,now_gridx,now_gridy))
										{
											TRACE("Stop Back in %d %s\r\n",__LINE__,__func__);
											Init_Stop_BackSweep();
											return;
										}
								}
							if(!Read_LeftRight())		//需要准备左沿边
								{
									turn_dir=2;			//左沿边，碰撞向右转
								}
							else						//需要准备右沿边
								{
									turn_dir=1;			//右沿边，碰撞向左转
								}
							mode.step_bp++;
							break;
						case 5:
							Speed=TURN_SPEED;
							if(do_action(turn_dir,90*Angle_1))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 6:
							mode.bump=0;
							mode.step_bp=0;
							mode.bump_flag=false;
							if(!Read_LeftRight())		//需要准备左沿边
								{Init_Sweep_LeftYBS(1);}
							else
								{Init_Sweep_RightYBS(1);}
							mode.bump_time=giv_sys_time;
							break;
						case 10:						//准备回扫进入
							Add_BackInfo();
							Set_Motion_BackSweep(1);	//设置回扫标志
							Save_Abort_Data();
							Change_LeftRight();
							Set_Motion_YDir_Reverse();
							mode.step_bp=4;
							//Logout_Area_Coordinate();
							//while(1);
							break;
						case 20:
							if(giv_sys_time-mode.bump_time<100)
								return;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							mode.step_bp++;
							break;
						case 21:
							Speed=TURN_SPEED;
							if(do_action(turn_dir,turn_angle*Angle_1))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 22:
							Speed=MID_MOVE_SPEED;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							if(do_action(3,5*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							if((m>=BUMP_ONLY_LEFT)&(m<=BUMP_MID))
								{
									stop_rap();
									mode.bump=m;
									mode.step_bp=0;
									
								}
							if((now_gridy==last_gridy)&(Judge_GridYPOS_Nearby_Reach(bump_gridy)))
								{
									stop_rap();
									TRACE("last_gridy=%d now_gridy=%d\r\n",last_gridy,now_gridy);
									//Init_Sweep_LeftYBS(1);
									motion1.tgt_yaw=motion1.anti_tgt_yaw;
									motion1.repeat_sweep=false;
									Init_Pass2Sweep();
								}
							break;
						case 23:
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							if(turn_dir==2)
								enable_rap_no_length(FRONT,REVOLUTION_SPEED_LOW,FRONT,REVOLUTION_SPEED_HIGH);
							else
								enable_rap_no_length(FRONT,REVOLUTION_SPEED_HIGH,FRONT,REVOLUTION_SPEED_LOW);
							if(Judge_Yaw_Reach(motion1.tgt_yaw,TURN_ANGLE_BIOS))
								{
									stop_rap();
									TRACE("Angle repeat in %s %d\r\n",__func__,__LINE__);
									mode.bump=0;
									mode.step_bp=0;
									mode.bump_flag=false;
									mode.step=0;
									grid.y_straight_start=grid.y;
									grid.x_straight_start=grid.x;
									mode.bump_time=giv_sys_time;
								}
							if((now_gridy==last_gridy)&(Judge_GridYPOS_Nearby_Reach(bump_gridy)))
								{
									stop_rap();
									TRACE("last_gridy=%d now_gridy=%d\r\n",last_gridy,now_gridy);
									//Init_Sweep_LeftYBS(1);
									motion1.tgt_yaw=motion1.anti_tgt_yaw;
									motion1.repeat_sweep=false;
									Init_Pass2Sweep();
								}
							if((m>=BUMP_ONLY_LEFT)&(m<=BUMP_MID))
								{
									stop_rap();
									TRACE("bump again in %s %d\r\n",__func__,__LINE__);
									mode.bump=m;
									mode.step_bp=0;
								}
							break;
					}
				break;
				
			case BUMP_ONLY_LEFT:
			case BUMP_ONLY_LEFTMID:
			case BUMP_LEFT_MID:
			case LM_WALL_BUMP:
				switch(mode.step_bp)
					{
						case 0:
							mode.bump_time=giv_sys_time;
							mode.step_bp++;
							bump_gridy=grid.y;
							bump_gridx=grid.x;
							break;
						case 1:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							if(mode.bump==LM_WALL_BUMP)
								mode.step_bp+=2;
							else
								mode.step_bp++;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							break;
						case 2:
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
									mode.bump_time=giv_sys_time;
								}
							break;
						case 3:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							if(Analysis_NeedBack(grid.y))		//判断回扫条件
								{
									mode.step_bp=10;
								}
							else if((!Read_Motion_BackSweep())&(Analysis_LastYClean()))
								{
									TRACE("Last Y has clean,prepare to LeftYBS!!!\r\n");
									turn_dir=2;
									if(mode.bump==BUMP_ONLY_LEFT)
										turn_angle=20;
									else
										turn_angle=40;
									mode.step_bp=20;
									mode.bump_time=giv_sys_time;
								}
							else
								{
									mode.step_bp++;
								}
							break;
						case 4:
							if(motion1.repeat_sweep&(!Read_Motion_BackSweep()))
								{
									TRACE("call this in %d %s\r\n",__LINE__,__func__);
									Init_Pass2Sweep();
									motion1.repeat_sweep=false;
									return;
								}
							if(Read_Motion_BackSweep())
								{
									if(Analysis_StopBack_InBump(ydir,now_gridx,now_gridy))
										{
											TRACE("Stop Back in %d %s\r\n",__LINE__,__func__);
											Init_Stop_BackSweep();
											return;
										}
								}
							if(!Read_LeftRight())		//需要准备左沿边
								{
									turn_dir=2;			//左沿边，碰撞向右转
									if(mode.bump==BUMP_ONLY_LEFT)
										turn_angle=20;
									else
										turn_angle=40;
								}
							else						//需要准备右沿边
								{
									turn_dir=1;			//右沿边，碰撞向左转
									if(mode.bump==BUMP_ONLY_LEFT)
										turn_angle=120;
									else
										turn_angle=90;
								}
							mode.step_bp++;
							break;
						case 5:
							Speed=TURN_SPEED;
							if(do_action(turn_dir,turn_angle*Angle_1))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 6:
							if(!Read_LeftRight())		//需要准备左沿边
								{
									Init_Sweep_LeftYBS(1);
								}
							else					
								{
									Init_Sweep_RightYBS(1);
								}
							mode.bump=0;
							mode.step_bp=0;
							mode.bump_flag=false;
							mode.bump_time=giv_sys_time;
							break;
						case 10:
							Add_BackInfo();
							Set_Motion_BackSweep(1);
							Save_Abort_Data();
							Change_LeftRight();
							Set_Motion_YDir_Reverse();
							mode.step_bp=4;
							break;
							
						case 19:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									turn_dir=2;
									turn_angle=30;
									mode.step_bp++;
									mode.bump_time=giv_sys_time;
								}
							break;							
						case 20:
							if(giv_sys_time-mode.bump_time<100)
								return;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							mode.step_bp++;
							break;
						case 21:
							Speed=TURN_SPEED;
							if(do_action(turn_dir,turn_angle*Angle_1))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 22:
							Speed=MID_MOVE_SPEED;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							if(do_action(3,5*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							if((m>=BUMP_ONLY_LEFT)&(m<=BUMP_MID))
								{
									stop_rap();
									TRACE("bump again at stepbp22\r\n");
//									mode.bump=m;
									mode.step_bp=19;
									mode.bump_time=giv_sys_time;
								}
							
							if(Judge_GridYPOS_Nearby_Reach(bump_gridy))
								{
									if(now_gridy==last_gridy)
										{
											TRACE("last_gridy=%d now_gridy=%d at stepbp22\r\n",last_gridy,now_gridy,__func__);
										}
									else
										{
											TRACE("next_gridy=%d now_gridy=%d at stepbp22\r\n",next_gridy,now_gridy,__func__);
										}
									stop_rap();
									TRACE("last_gridy=%d now_gridy=%d at stepbp23\r\n",last_gridy,now_gridy,__func__);
									motion1.tgt_yaw=motion1.anti_tgt_yaw;
									motion1.repeat_sweep=false;
									Init_Pass2Sweep();
									return;
								}
							break;
						case 23:
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							enable_rap_no_length(FRONT,REVOLUTION_SPEED_LOW,FRONT,REVOLUTION_SPEED_HIGH);
							if(Judge_Yaw_Reach(motion1.tgt_yaw,TURN_ANGLE_BIOS))
								{	
									stop_rap();
									mode.bump=0;
									mode.step_bp=0;
									mode.bump_flag=false;
									mode.step=0;
									grid.y_straight_start=grid.y;
									grid.x_straight_start=grid.x;
									mode.bump_time=giv_sys_time;
									motion1.repeat_sweep=false;
									TRACE("Angle repeat at stepbp 23\r\n");
								}
							
							if(now_gridy==last_gridy)
								{
									motion1.repeat_sweep=false;
									if(Judge_GridYPOS_Nearby_Reach(bump_gridy))
										{
											stop_rap();
											TRACE("last_gridy=%d now_gridy=%d at stepbp23\r\n",last_gridy,now_gridy,__func__);
											//Init_Sweep_RightYBS(1);
											motion1.tgt_yaw=motion1.anti_tgt_yaw;
											motion1.repeat_sweep=false;
											Init_Pass2Sweep();
											return;
										}
								}
							if(now_gridy==next_gridy)
								{
									if(Judge_GridYPOS_Nearby_Reach(bump_gridy))
										{
											stop_rap();
											TRACE("next_gridy=%d now_gridy=%d at stepbp23\r\n",next_gridy,now_gridy);
											//Init_Sweep_RightYBS(1);
											motion1.tgt_yaw=motion1.anti_tgt_yaw;
											motion1.repeat_sweep=true;
											Init_Pass2Sweep();
											return;
										}
								}
							if((m>=BUMP_ONLY_LEFT)&(m<=BUMP_MID))
								{
									stop_rap();
									TRACE("bump again at stepbp23\r\n");
//									mode.bump=m;
//									mode.step_bp=0;
									mode.step_bp=19;
									mode.bump_time=giv_sys_time;
								}
							break;
					}
				break;
			case BUMP_ONLY_RIGHT:
			case BUMP_ONLY_RIGHTMID:
			case BUMP_RIGHT_MID:
			case RM_WALL_BUMP:
				switch(mode.step_bp)
					{
						case 0:
							mode.bump_time=giv_sys_time;
							mode.step_bp++;
							bump_gridy=grid.y;
							bump_gridx=grid.x;
							break;
						case 1:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							if(mode.bump==LM_WALL_BUMP)
								mode.step_bp+=2;
							else
								mode.step_bp++;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							break;
						case 2:
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 3:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							if(Analysis_NeedBack(grid.y))		//判断回扫条件
								{
									mode.step_bp=10;
								}
							else if((!Read_Motion_BackSweep())&(Analysis_LastYClean()))
								{
									TRACE("Last Y has clean,prepare to RightYBS!!!\r\n");
									turn_dir=1;
									if(mode.bump==BUMP_ONLY_RIGHT)
										turn_angle=20;
									else
										turn_angle=40;
									mode.step_bp=20;
									mode.bump_time=giv_sys_time;
								}
							else
								{
									mode.step_bp++;
								}
							break;
						case 4:
							if(motion1.repeat_sweep&(!Read_Motion_BackSweep()))
								{
									TRACE("call this in %d %s\r\n",__LINE__,__func__);
									Init_Pass2Sweep();
									motion1.repeat_sweep=false;
									return;
								}
							if(Read_Motion_BackSweep())
								{
									if(Analysis_StopBack_InBump(ydir,now_gridx,now_gridy))
										{
											TRACE("Stop Back in %d %s\r\n",__LINE__,__func__);
											Init_Stop_BackSweep();
											return;
										}
								}
							if(!Read_LeftRight())		//需要准备左沿边
								{
									turn_dir=2; 		//左沿边，碰撞向右转
									if(mode.bump==BUMP_ONLY_RIGHT)
										turn_angle=120;
									else
										turn_angle=90;
								}
							else						//需要准备右沿边
								{
									turn_dir=1; 		//右沿边，碰撞向左转
									if(mode.bump==BUMP_ONLY_RIGHT)
										turn_angle=20;
									else
										turn_angle=40;
								}
							mode.step_bp++;
							break;
						case 5:
							Speed=TURN_SPEED;
							if(do_action(turn_dir,turn_angle*Angle_1))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 6:
							if(!Read_LeftRight())		//需要准备左沿边
								{
									Init_Sweep_LeftYBS(1);
								}
							else					
								{
									Init_Sweep_RightYBS(1);
								}
							mode.bump=0;
							mode.step_bp=0;
							mode.bump_flag=false;
							mode.bump_time=giv_sys_time;
							break;
						case 10:
							Add_BackInfo();
							Set_Motion_BackSweep(1);		//设置回扫标志
							Save_Abort_Data();
							Change_LeftRight();
							Set_Motion_YDir_Reverse();
							mode.step_bp=4;
							break;
						case 19:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									turn_dir=1;
									turn_angle=30;
									mode.step_bp++;
									mode.bump_time=giv_sys_time;
								}
							break;
						case 20:
							if(giv_sys_time-mode.bump_time<100)
								return;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							mode.step_bp++;
							break;
						case 21:
							Speed=TURN_SPEED;
							if(do_action(turn_dir,turn_angle*Angle_1))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 22:
							Speed=MID_MOVE_SPEED;
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							if(do_action(3,5*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							if((m>=BUMP_ONLY_LEFT)&(m<=BUMP_MID))
								{
									stop_rap();
									TRACE("bump again at stepbp22\r\n");
//									mode.bump=m;
									mode.step_bp=19;
									mode.bump_time=giv_sys_time;
								}
							if(Judge_GridYPOS_Nearby_Reach(bump_gridy))
								{
									if(now_gridy==last_gridy)
										{
											TRACE("last_gridy=%d now_gridy=%d at stepbp22\r\n",last_gridy,now_gridy,__func__);
										}
									else
										{
											TRACE("next_gridy=%d now_gridy=%d at stepbp22\r\n",next_gridy,now_gridy,__func__);
										}
									stop_rap();
									TRACE("last_gridy=%d now_gridy=%d at stepbp23\r\n",last_gridy,now_gridy,__func__);
									//Init_Sweep_RightYBS(1);
									motion1.tgt_yaw=motion1.anti_tgt_yaw;
									motion1.repeat_sweep=false;
									Init_Pass2Sweep();
									return;
								}
							break;
						case 23:
							Set_Coordinate_WallClean(now_gridx,now_gridy);
							enable_rap_no_length(FRONT,REVOLUTION_SPEED_HIGH,FRONT,REVOLUTION_SPEED_LOW);

							if(Judge_Yaw_Reach(motion1.tgt_yaw,TURN_ANGLE_BIOS))
								{	
									stop_rap();
									TRACE("Angle repeat at stepbp 23\r\n");
									mode.bump=0;
									mode.step_bp=0;
									mode.bump_flag=false;
									mode.step=0;
									grid.y_straight_start=grid.y;
									grid.x_straight_start=grid.x;
									mode.bump_time=giv_sys_time;
								}
							
							if(now_gridy==last_gridy)
								{
									motion1.repeat_sweep=false;
									if(Judge_GridYPOS_Nearby_Reach(bump_gridy))
										{
											stop_rap();
											TRACE("last_gridy=%d now_gridy=%d at stepbp23\r\n",last_gridy,now_gridy,__func__);
											//Init_Sweep_RightYBS(1);
											motion1.tgt_yaw=motion1.anti_tgt_yaw;
											motion1.repeat_sweep=false;
											Init_Pass2Sweep();
											return;
										}
								}
							if(now_gridy==next_gridy)
								{
									if(Judge_GridYPOS_Nearby_Reach(bump_gridy))
										{
											stop_rap();
											TRACE("next_gridy=%d now_gridy=%d at stepbp23\r\n",next_gridy,now_gridy);
											//Init_Sweep_RightYBS(1);
											motion1.tgt_yaw=motion1.anti_tgt_yaw;
											motion1.repeat_sweep=true;
											Init_Pass2Sweep();
											return;
										}
								}
							if((m>=BUMP_ONLY_LEFT)&(m<=BUMP_MID))
								{
									stop_rap();
									TRACE("bump again at stepbp23\r\n");
//									mode.bump=m;
									mode.step_bp=19;
									mode.bump_time=giv_sys_time;
								}
							break;
							
								
							
					}
				break;
			case BUMP_XOUTRANGE:
			case BUMP_YOUTRANGE:
			case BUMP_XMAX_OUT:
			case BUMP_XMIN_OUT:
			case BUMP_YMAX_OUT:
			case BUMP_YMIN_OUT:
			case BUMP_RF_OUT:
			case BUMP_LF_OUT:
			case BUMP_RB_OUT:
			case BUMP_LB_OUT:
				switch(mode.step_bp)
					{
						case 0:
							stop_rap();
							mode.bump_time=giv_sys_time;
//							mode.step_bp++;
							mode.step_bp=50;
							break;
						case 50:
							switch(mode.bump)
								{
									case BUMP_XOUTRANGE:
										tgt_angle=F_Angle_Const;
										mode.step_bp++;
										break;
									case BUMP_XMIN_OUT:
										tgt_angle=B_Angle_Const;
										mode.step_bp++;
										break;
									case BUMP_YMAX_OUT:
									case BUMP_RF_OUT:
									case BUMP_RB_OUT:
										tgt_angle=R_Angle_Const;
										mode.step_bp++;
										break;
									case BUMP_YMIN_OUT:
									case BUMP_LF_OUT:
									case BUMP_LB_OUT:
										tgt_angle=L_Angle_Const;
										mode.step_bp++;
										break;
									default:
										mode.step_bp=1;
										break;
								}
							break;
						case 51:
							turn_dir=Get_TurnDir(tgt_angle);
							Speed=TURN_SPEED;
							do_action(turn_dir,360*Angle_1);
							if(Judge_Yaw_Reach(tgt_angle,TURN_ANGLE_BIOS))
								{
									stop_rap();
									mode.step_bp=1;
									mode.bump_time=giv_sys_time;
								}
							break;
						case 1:
							if(giv_sys_time-mode.bump_time<100)
								return;
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,5*CM_PLUS))
								{
									stop_rap();
									m=Check_OutofRange();
									if(m)
										{
											mode.bump=m;
											mode.step_bp=0;
											return;
										}
									else
										{
											mode.step_bp++;
											return;
										}
								}
							if(!Check_OutofRange())
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 2:
							if(Analysis_NeedBack(grid.y))		//判断回扫条件
								{
									mode.step_bp=10;
								}
							else
								{
									mode.step_bp++;
								}
							break;
						case 3:
							if(motion1.repeat_sweep&(!Read_Motion_BackSweep()))
								{
									TRACE("call this in %d %s\r\n",__LINE__,__func__);
									Init_Pass2Sweep();
									motion1.repeat_sweep=false;
									return;
								}
							if(Read_Motion_BackSweep())
								{
									if(Analysis_StopBack_InBump(ydir,now_gridx,now_gridy))
										{
											TRACE("Stop Back in %d %s\r\n",__LINE__,__func__);
											Init_Stop_BackSweep();
											return;
										}
								}
							if(((now_gridy==grid.y_area_max)&(ydir>0))|((now_gridy==grid.y_area_min)&(ydir<0)))
								{
									Area_Check(0);
									return;
								}
							
							if(!Read_LeftRight())		//需要准备左沿边
								{
									if(motion1.tgt_yaw==F_Angle_Const)
										tgt_angle=R_Angle_Const;
									else
										tgt_angle=L_Angle_Const;
								}
							else					
								{
									if(motion1.tgt_yaw==F_Angle_Const)
										tgt_angle=L_Angle_Const;
									else
										tgt_angle=R_Angle_Const;
								}
//							turn_dir=Get_TurnDir(tgt_angle);
							TRACE("tgt_yaw=%d in %s %d \r\n",tgt_angle,__func__,__LINE__);
							mode.step_bp++;
							break;
						case 4:
							Speed=TURN_SPEED;
							turn_dir=Get_TurnDir(tgt_angle);
							if(do_action(turn_dir,720*Angle_1))
								{
									stop_rap();
									//mode.step_bp++;
								}
							if(Judge_Yaw_Reach(tgt_angle,TURN_ANGLE_BIOS))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 5:
							Speed=MID_MOVE_SPEED;
							if(do_action(3,30*CM_PLUS))
								{
									stop_rap();
									mode.step_bp=7;
								}
							if((m!=0)&(m<BUMP_OUTRANGE))
								{	
									stop_rap();
									mode.step_bp=12;
									return;
								}
							//if(grid.y!=grid.y_straight_start)
							if((grid.y!=grid.y_straight_start)&(Judge_GridYPOS_Nearby_Reach(grid.y_straight_start)))
								{
									stop_rap();
									mode.step_bp=7;
								}
							break;
						case 6:
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 7:
							if((Check_OutofRange())==BUMP_YOUTRANGE)
								{	
									mode.step_bp=6;
									return;
								}
							TRACE("Out of Range action complete!\r\n");
							TRACE("Prepare to Pass to Sweep!\r\n");
							Init_Pass2Sweep();
							break;
						case 10:
							Add_BackInfo();
							Set_Motion_BackSweep(1);		//设置回扫标志
							Save_Abort_Data();
							Change_LeftRight();
							Set_Motion_YDir_Reverse();
							mode.step_bp=3;
							break;
						case 12:
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 13:
							if(!Read_LeftRight())		//需要准备左沿边
								{
									Init_Sweep_LeftYBS(1);
								}
							else					
								{
									Init_Sweep_RightYBS(1);
								}
							mode.bump=0;
							mode.step_bp=0;
							mode.bump_flag=false;
							mode.bump_time=giv_sys_time;
							break;
					}
				break;
				
			case BUMP_SEAT:
				switch(mode.step_bp)
					{
						case 0:
							Set_Coordinate_Seat(now_gridx,now_gridy);
							Set_Coordinate_Wall(now_gridx,now_gridy);
							if(motion1.repeat_sweep)
								{
									TRACE("call this in %d %s\r\n",__LINE__,__func__);
									Init_Pass2Sweep();
									motion1.repeat_sweep=false;
								}
							else
								{
									mode.step_bp++;
								}
							break;
						case 1:
							if(Read_Motion_BackSweep())
								{
									if(Analysis_StopBack_InBump(ydir,now_gridx,now_gridy))
										{
											TRACE("Stop Back in %d %s\r\n",__LINE__,__func__);
											Init_Stop_BackSweep();
											return;
										}
								}
							if(!Read_LeftRight())		//需要准备左沿边
								{
									if(motion1.tgt_yaw==F_Angle_Const)
										tgt_angle=R_Angle_Const;
									else
										tgt_angle=L_Angle_Const;
								}
							else					
								{
									if(motion1.tgt_yaw==F_Angle_Const)
										tgt_angle=L_Angle_Const;
									else
										tgt_angle=R_Angle_Const;
								}
							mode.step_bp++;
						case 2:
							Speed=TURN_SPEED;
							turn_dir=Get_TurnDir(tgt_angle);
							if(do_action(turn_dir,720*Angle_1))
								{
									stop_rap();
									//mode.step_bp++;
								}
							if(Judge_Yaw_Reach(tgt_angle,TURN_ANGLE_BIOS))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 3:
							Speed=MID_MOVE_SPEED;
							if(do_action(3,30*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							if((m>0)&(m!=BUMP_SEAT))
								{	
									stop_rap();
									mode.step_bp=12;
									return;
								}
							//if(grid.y!=grid.y_straight_start)
							if((now_gridy!=grid.y_straight_start)&(Judge_GridYPOS_Nearby_Reach(grid.y_straight_start)))
								{
									stop_rap();
									Set_Coordinate_Seat(now_gridx,now_gridy);
									Set_Coordinate_Wall(now_gridx,now_gridy);
									mode.step_bp++;
								}
							break;
						case 4:
							TRACE("call this in %d %s\r\n",__LINE__,__func__);
							Init_Pass2Sweep();
							break;
						case 12:
							Speed=BUMP_BACK_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									mode.step_bp++;
								}
							break;
						case 13:
							if(!Read_LeftRight())		//需要准备左沿边
								{
									Init_Sweep_LeftYBS(1);
								}
							else					
								{
									Init_Sweep_RightYBS(1);
								}
							mode.bump=0;
							mode.step_bp=0;
							mode.bump_flag=false;
							mode.bump_time=giv_sys_time;
							break;
					}
		}
}

void Init_NormalSweep(short tgt_yaw)
{
	mode.last_mode=mode.mode;		//qz add 20180205
	mode.last_sub_mode=mode.sub_mode;
	/******初始化显示***********/
		
	/******初始化设置的值********************/
	clr_ram();
//	ReInitAd();

	Enable_earth();
	Enable_wall();
	enable_hwincept();				//允许红外接收电源
	Enable_Speed(); 				//允许速度发送
#if 0
	if(DOCK_SWEEP)
		Sweep_Level_Set(DOCK_SWEEP_LEVEL);
	else
		Sweep_Level_Set(sweep_suction);
#endif		 
	Init_Action();
	
	mode.mode = SWEEP;			
	mode.sub_mode = NORMAL_SWEEP;
	mode.step=0;
	mode.time=giv_sys_time;
	mode.bump = 0;
	mode.step_bp = 0;
	mode.bump_flag=0;
	mode.Info_Abort=0;				//qz add 20180919
	mode.All_Info_Abort=0;			//qz add 20180919

	mode.status=1;
//		WriteWorkState();

	w_l.on=0;
	w_r.on=0;
	w_rm.on=0;
	w_lm.on=0;
#ifdef DEBUG_Enter_Mode
	TRACE("Init NORMAL SWEEP Mode Complete!\r\n");
#endif
	//初始化检测的条件
	CHECK_STATUS_FLAG=true; 	//qz add 20180725:如果机器处于休眠时,接到控制命令会直接进入此状态,因此需要打开检测开关
//	Init_Check_Status();//qz add 20180425
#ifdef FREE_SKID_CHECK
	Enable_Free_Skid_Check();		//打开万向轮检测
#endif
	memset(USART1_RX_BUF,0,sizeof(USART1_RX_BUF)/sizeof(USART1_RX_BUF[0])); 	//清除接收缓存qz add 20180703
	
	if(dis_err_code==DIS_ERROR_DOCK_FAIL)		//qz add 20180710
		{
			dis_err_code=0;
			error_code=0;
		}

	motion1.tgt_yaw=tgt_yaw;
	motion1.anti_tgt_yaw=Get_Reverse_Angle(tgt_yaw);
	
	Assign_LeftRight();
	
	motion1.leakon=false;
	grid.x_straight_start=grid.x;
	grid.y_straight_start=grid.y;

//	Sweep_Level_Set(sweep_suction);
	
	TRACE("motion1.tgt_yaw=%d\r\n",motion1.tgt_yaw);
	TRACE("motion1.anti_tgt_yaw=%d\r\n",motion1.anti_tgt_yaw);
	TRACE("grid.x_straight_start=%d\r\n",grid.x_straight_start);
	TRACE("grid.y_straight_start=%d\r\n",grid.y_straight_start);

	delay_ms(500);
}

void Do_NormalSweep(void)
{	
	s8 now_gridx,now_gridy,ydir;
	static u8 turn_dir=0;
	
	now_gridx=grid.x;now_gridy=grid.y;
	ydir=Read_Motion_YDir();
	
	ACC_DEC_Curve();
	u8 abnormal=Read_Protect();
	if(mode.abnormity)
		{
			Action_Protect_My(abnormal);
#ifdef FREE_SKID_INDEP_CHECK
			Free_Skid_Indep.check_flag=false;
#endif

#ifdef YBS_DIS_RESTORE
			Disable_Rotate_Angle();
#endif
			mode.speed_up=false;		//qz add 20181225

			return;
		}
	Sweep_Bump_Action(1,1);

	clr_all_hw_effect();
	
	if(mode.bump)
		return;
	if((mode.sub_mode!=NORMAL_SWEEP))		//因为在Sweep_Bump_Action()中可能会切换模式到PASS2SWEEP
		return;
	switch (mode.step)
		{
			case 0:
				if(giv_sys_time-mode.bump_time<200)
					return;
				mode.time=giv_sys_time;
				mode.step++;
			break;
			case 1:
				Speed=HIGH_MOVE_SPEED;
				//do_action(3,FARAWAY);
				if(do_action_my(3,FARAWAY,motion1.tgt_yaw));
				if((w_m.high_sign==NEAR)|(w_r.high_sign==NEAR)|(w_l.high_sign==NEAR)|(w_lm.high_sign==NEAR)|(w_rm.high_sign==NEAR))
					{
						r_rap.rap=HIGH_MOVE_SPEED;
						l_rap.rap=HIGH_MOVE_SPEED;
						mode.time=giv_sys_time;
					}
				else if(giv_sys_time-mode.time>10000)
					{
						if((grid.x>=grid.x_area_max-1)|(grid.x<=grid.x_area_min+1))
							{
								l_rap.rap=HIGH_MOVE_SPEED;
								r_rap.rap=HIGH_MOVE_SPEED;
							}
						else
							{
								r_rap.rap=TOP_MOVE_SPEED;
								l_rap.rap=TOP_MOVE_SPEED;
							}
					}
				if((motion1.tgt_yaw==B_Angle_Const)&(motion1.start_seat)&(motion1.area_num<=2))
					{
						if((now_gridx<=-1)&(now_gridx>=-3))
							{
								if((now_gridy<=2)&(now_gridy>=-2))
									{
										stop_rap();
										TRACE("motion in Seat Area!!!\r\n");
										mode.step++;
									}
							}
					}
				break;
			case 2:
				if(ydir>0)
					{
						if(now_gridy+1>grid.y_area_max)
							{
								Area_Check(0);
								return;
							}
					}
				else if(ydir<0)
					{
						if(now_gridy-1<grid.y_area_min)
							{
								Area_Check(0);
								return;
							}
					}
				if(motion1.repeat_sweep)
					{
						TRACE("call this in %d %s\r\n",__LINE__,__func__);
						Init_Pass2Sweep();
						motion1.repeat_sweep=false;
						return;
					}
				if(!Read_LeftRight())		//需要准备左沿边
					{
						turn_dir=2; //左沿边，碰撞向右转
					}
				else				//需要准备右沿边
					{
						turn_dir=1; //右沿边，碰撞向左转
					}
				mode.step++;
				break;
			case 3:
				Speed=TURN_SPEED;
				if(do_action(turn_dir,100*Angle_1))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 4:
				Speed=HIGH_MOVE_SPEED;
				if(do_action(3,HORIZON_LENGTH*CM_PLUS))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 5:
				//Init_Back_Sweep();
				TRACE("call this in %d %s\r\n",__LINE__,__func__);
				Init_Pass2Sweep();
				break;
			//qz add 20190307 X坐标超出4M范围的措施
		}
}

void Init_Back_Sweep(short tgt_yaw)
{
	u8 data1;
	mode.last_mode=mode.mode;		//qz add 20180205
	mode.last_sub_mode=mode.sub_mode;
	/******初始化显示***********/
		
	/******初始化设置的值********************/
	clr_ram();
//	ReInitAd();

	Enable_earth();
	Enable_wall();
	enable_hwincept();				//允许红外接收电源
	Enable_Speed(); 				//允许速度发送
#if 0
	if(DOCK_SWEEP)
		Sweep_Level_Set(DOCK_SWEEP_LEVEL);
	else
		Sweep_Level_Set(sweep_suction);
#endif		 
	Init_Action();
	
	mode.mode = SWEEP;			
	mode.sub_mode = BACK_SWEEP;
	mode.step=0;
	mode.time=giv_sys_time;
	mode.bump = 0;
	mode.step_bp = 0;
	mode.bump_flag=0;
	mode.Info_Abort=0;				//qz add 20180919
	mode.All_Info_Abort=0;			//qz add 20180919

	mode.status=1;
//		WriteWorkState();

	w_l.on=0;
	w_r.on=0;
	w_rm.on=0;
	w_lm.on=0;
#ifdef DEBUG_Enter_Mode
	TRACE("Init BACK SWEEP Mode Complete!\r\n");
#endif
	//初始化检测的条件
	CHECK_STATUS_FLAG=true; 	//qz add 20180725:如果机器处于休眠时,接到控制命令会直接进入此状态,因此需要打开检测开关
//	Init_Check_Status();//qz add 20180425
#ifdef FREE_SKID_CHECK
	Enable_Free_Skid_Check();		//打开万向轮检测
#endif
	memset(USART1_RX_BUF,0,sizeof(USART1_RX_BUF)/sizeof(USART1_RX_BUF[0])); 	//清除接收缓存qz add 20180703
	
	if(dis_err_code==DIS_ERROR_DOCK_FAIL)		//qz add 20180710
		{
			dis_err_code=0;
			error_code=0;
		}

	motion1.tgt_yaw=tgt_yaw;
	motion1.anti_tgt_yaw=Get_Reverse_Angle(tgt_yaw);
	
	Assign_LeftRight();
	
	grid.x_straight_start=grid.x;
	grid.y_straight_start=grid.y;
	grid.y_back_last=grid.y;
	TRACE("motion1.tgt_yaw=%d\r\n",motion1.tgt_yaw);
	TRACE("motion1.anti_tgt_yaw=%d\r\n",motion1.anti_tgt_yaw);
	data1=Analysis_StopBack(motion1.tgt_yaw);
	if(data1==1)
		{
			{
				TRACE("Analysis Stop back sweep!\r\n");
				TRACE("Quit Back Sweep in %s\r\n",__func__);
				Init_Stop_BackSweep();
			}
		}
	else if(data1==2)
		{
			mode.step=2;
		}

	delay_ms(500);
}

void Do_BackSweep(void)
{
	s8 now_gridx,now_gridy,ydir;
	static u8 turn_dir;
	
	now_gridx=grid.x;now_gridy=grid.y;
	ydir=Read_Motion_YDir();
	
	ACC_DEC_Curve();
	clr_all_hw_effect();
	u8 abnormal=Read_Protect();
	if(mode.abnormity)
		{
			Action_Protect_My(abnormal);
#ifdef FREE_SKID_INDEP_CHECK
			Free_Skid_Indep.check_flag=false;
#endif

#ifdef YBS_DIS_RESTORE
			Disable_Rotate_Angle();
#endif
			mode.speed_up=false;		//qz add 20181225

			return;
		}
	Sweep_Bump_Action(0,1);
	if(mode.bump)
		return;
	if((mode.sub_mode!=BACK_SWEEP))		//因为在Sweep_Bump_Action()中可能会切换模式到PASS2SWEEP
		return;
	switch (mode.step)
		{
			case 0:
				mode.time=giv_sys_time;
				mode.step++;
			break;
			case 1:
				Speed=HIGH_MOVE_SPEED;
				//do_action(3,FARAWAY);
				if(do_action_my(3,FARAWAY,motion1.tgt_yaw));
				if((w_m.high_sign==NEAR)|(w_r.high_sign==NEAR)|(w_l.high_sign==NEAR)|(w_lm.high_sign==NEAR)|(w_rm.high_sign==NEAR))
					{
						r_rap.rap=HIGH_MOVE_SPEED;
						l_rap.rap=HIGH_MOVE_SPEED;
						mode.time=giv_sys_time;
					}
				else if(giv_sys_time-mode.time>10000)
					{
						if((now_gridx>=grid.x_area_max-1)|(now_gridx<=grid.x_area_min+1))
							{
								l_rap.rap=HIGH_MOVE_SPEED;
								r_rap.rap=HIGH_MOVE_SPEED;
							}
						else
							{
								r_rap.rap=TOP_MOVE_SPEED;
								l_rap.rap=TOP_MOVE_SPEED;
							}
					}

				
				if((motion1.tgt_yaw==F_Angle_Const))
					{
						if((Read_Coordinate_Clean(now_gridx+1,now_gridy))&(Read_Coordinate_Clean(now_gridx+2,now_gridy)))
						//if((Read_Coordinate_Clean(now_gridx+1,now_gridy))&(now_gridx+1<=GRID_MAX))
							{
								stop_rap();
								TRACE("Now Grid.x=%d y=%d\r\n",now_gridx,now_gridy);
								TRACE("coor[%d][%d] has been clean,goto next!!!\r\n",now_gridy,now_gridx+1);
								TRACE("coor[%d][%d] has been clean,goto next!!!\r\n",now_gridy,now_gridx+2);
								mode.step++;
							}
					}

				if(motion1.tgt_yaw==B_Angle_Const)
					{
						if((Read_Coordinate_Clean(now_gridx-1,now_gridy))&(Read_Coordinate_Clean(now_gridx-2,now_gridy)))
						//if((Read_Coordinate_Clean(now_gridx-1,now_gridy))&(now_gridx-1>=GRID_MIN))
							{
								stop_rap();
								TRACE("Now Grid.x=%d y=%d\r\n",now_gridx,now_gridy);
								TRACE("coor[%d][%d] has been clean,goto next!!!\r\n",now_gridy,now_gridx+1);
								TRACE("coor[%d][%d] has been clean,goto next!!!\r\n",now_gridy,now_gridx+2);
								mode.step++;
							}
					}

				#if 0
				//if((coordinate[grid.y][grid.x].clean))
				if(Read_Coordinate_Clean(grid.x,grid.y))
					{
						TRACE("BACK OFF!!!\\r\n::");
						while(1);
					}
				#endif
				//qz add end
				break;
			//qz add 20190307 X坐标超出4M范围的措施
			case 2:
				if(ydir>0)
					{
						if(now_gridy+1>grid.y_area_max)
							{
								TRACE("STOP BACK in %s %d\r\n",__func__,__LINE__);
								Init_Stop_BackSweep();
								return;
							}
					}
				else if(ydir<0)
					{
						if(now_gridy-1<grid.y_area_min)
							{
								TRACE("STOP BACK in %s %d\r\n",__func__,__LINE__);
								Init_Stop_BackSweep();
								return;
							}
					}
				
				if(!Read_LeftRight())		//需要准备左沿边
					{
						turn_dir=2; //左沿边，碰撞向右转
					}
				else				//需要准备右沿边
					{
						turn_dir=1; //右沿边，碰撞向左转
					}
				mode.step++;
				break;
			case 3:
				Speed=TURN_SPEED;
				if(do_action(turn_dir,100*Angle_1))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 4:
				Speed=HIGH_MOVE_SPEED;
				if(do_action(3,HORIZON_LENGTH*CM_PLUS))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 5:
				//Init_Back_Sweep();
				TRACE("call this in %d %s\r\n",__LINE__,__func__);
				Init_Pass2Sweep();
				break;
			//qz add end
				
			case 0xff:
				break;
					
		}
}

void Pass2Sweep_Bump_Action(void)
{
	Read_Only_Collide();
	switch (mode.bump)
		{
			case BUMP_ONLY_LEFT:
			case BUMP_ONLY_LEFTMID:
			case BUMP_LEFT_MID:
			case BUMP_ONLY_RIGHT:
			case BUMP_ONLY_RIGHTMID:
			case BUMP_RIGHT_MID:
			case BUMP_MID:
				switch (mode.step_bp)
					{
						case 0:
							stop_rap();
							mode.bump_time=giv_sys_time;
							mode.step_bp++;
							break;
						case 1:
							if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
								return;
							Speed=TURN_SPEED;
							if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
								{
									stop_rap();
									mode.bump=0;
									mode.bump_flag=false;
									mode.step_bp=0;
									mode.step=0;
									mode.bump_time=giv_sys_time;
								}
							break;
					}
		}
}

void Init_Pass2Sweep(void)
{
	mode.last_mode=mode.mode;		//qz add 20180205
	mode.last_sub_mode=mode.sub_mode;
	/******初始化显示***********/
		
	/******初始化设置的值********************/
	clr_ram();
//	ReInitAd();

	Enable_earth();
	Enable_wall();
	enable_hwincept();				//允许红外接收电源
	Enable_Speed(); 				//允许速度发送
	Init_Action();
	
	mode.mode = SWEEP;			
	mode.sub_mode = PASS2SWEEP;
	mode.step=0;
	mode.time=giv_sys_time;
	mode.bump = 0;
	mode.step_bp = 0;
	mode.bump_flag=0;
	mode.Info_Abort=0;				//qz add 20180919
	mode.All_Info_Abort=0;			//qz add 20180919

	mode.status=1;
	WriteWorkState();

	w_l.on=0;
	w_r.on=0;
	w_rm.on=0;
	w_lm.on=0;
#ifdef DEBUG_Enter_Mode
	TRACE("Init PASS2SWEEP Mode Complete!\r\n");
#endif
	//初始化检测的条件
	CHECK_STATUS_FLAG=true; 	//qz add 20180725:如果机器处于休眠时,接到控制命令会直接进入此状态,因此需要打开检测开关
//	Init_Check_Status();//qz add 20180425
#ifdef FREE_SKID_CHECK
	Enable_Free_Skid_Check();		//打开万向轮检测
#endif
	memset(USART1_RX_BUF,0,sizeof(USART1_RX_BUF)/sizeof(USART1_RX_BUF[0])); 	//清除接收缓存qz add 20180703
	
	if(dis_err_code==DIS_ERROR_DOCK_FAIL)		//qz add 20180710
		{
			dis_err_code=0;
			error_code=0;
		}

}


void Do_Pass2Sweep(void)
{
	u8 turn_dir=0;
	short tgt_angle=0;
//	YBS_Comm_Rap_My();
	ACC_DEC_Curve();
	clr_all_hw_effect();
	u8 abnormal=Read_Protect();
	if(mode.abnormity)
		{
			Action_Protect_My(abnormal);
#ifdef FREE_SKID_INDEP_CHECK
			Free_Skid_Indep.check_flag=false;
#endif

#ifdef YBS_DIS_RESTORE
			Disable_Rotate_Angle();
#endif
			mode.speed_up=false;		//qz add 20181225

			return;
		}

	Pass2Sweep_Bump_Action();
	if(mode.bump)
		return;
	if(mode.sub_mode!=PASS2SWEEP)
		return;
	switch (mode.step)
		{
			case 0:
				stop_rap();
				if(giv_sys_time-mode.time>5000)
					{
						{
							if(!Read_LeftRight())		//需要准备左沿边
								{
									mode.step=10;
									return;
								}
							else						//当前右沿边，左转。tgt_yaw为B_Anlge,需要转向到F_Angle;
								{
									mode.step=20;
									return;
								}
						}
					}
				break;
			case 10:
				Speed=TURN_SPEED;
				if(do_action(2,720*Angle_1));
				if(motion1.tgt_yaw==F_Angle_Const)
					tgt_angle=B_Angle_Const;
				else
					tgt_angle=F_Angle_Const;
				if(Judge_Yaw_Reach(tgt_angle,TURN_ANGLE_BIOS))
					{
						stop_rap();
						TRACE("Gyro_Data.yaw=%d turn_angle=%d\r\n",Gyro_Data.yaw,tgt_angle);
						TRACE("yaw Reach!\r\n");
						mode.step=0xF0;
					}
				break;
			case 20:
				Speed=TURN_SPEED;
				if(do_action(1,720*Angle_1));
				if(motion1.tgt_yaw==F_Angle_Const)
					tgt_angle=B_Angle_Const;
				else
					tgt_angle=F_Angle_Const;
				if(Judge_Yaw_Reach(tgt_angle,TURN_ANGLE_BIOS))
					{
						stop_rap();
						mode.step=0xF0;
					}
				break;
			case 0xF0:
				if(motion1.tgt_yaw==F_Angle_Const)
					{
						tgt_angle=B_Angle_Const;
					}
				else
					{	
						tgt_angle=F_Angle_Const;
					}
				if(!Read_Motion_BackSweep())
					{
						if(!motion1.leakon)
							Init_NormalSweep(tgt_angle);
						else
							Init_LeakSweep(tgt_angle);
					}
				else
					{
						if(!motion1.leakon)
							Init_Back_Sweep(tgt_angle);
						else
							Init_Leak_BackSweep(tgt_angle);
					}
				break;
		}
}

void Continue_Sweep(void)
{
	/******初始化显示***********/
			
	/******初始化设置的值********************/
	clr_ram();
	//	ReInitAd();
	
	Enable_earth();
	Enable_wall();
	enable_hwincept();				//允许红外接收电源
	Enable_Speed(); 				//允许速度发
	
	mode.mode=SWEEP;
#if 0
	if(Read_Motion_BackSweep())
		mode.sub_mode=BACK_SWEEP;
	else
		mode.sub_mode=NORMAL_SWEEP;
#endif
	TRACE("mode.last_sub_mode=0x%x\r\n",mode.last_sub_mode);
	mode.sub_mode=mode.last_sub_mode;
	mode.step=0;
	mode.bump=0;
	mode.step_bp=0;
	mode.abnormity=0;
	mode.step_abn=0;
	mode.bump_flag=0;
	delay_ms(500);
	TRACE("Continue SWEEP!\r\n");
	grid.x_straight_start=grid.x;
	grid.y_straight_start=grid.y;
}

void Init_Stop_BackSweep(void)
{
	mode.last_mode=mode.mode;		//qz add 20180205
	mode.last_sub_mode=mode.sub_mode;
	/******初始化显示***********/
			
	/******初始化设置的值********************/
	clr_ram();
	//	ReInitAd();
	
	Enable_earth();
	Enable_wall();
	enable_hwincept();				//允许红外接收电源
	Enable_Speed(); 				//允许速度发
	
	mode.mode=SWEEP;
	mode.sub_mode=STOP_BACKSWEEP;
	mode.step=0;
	mode.bump=0;
	mode.step_bp=0;
	mode.abnormity=0;
	mode.step_abn=0;
	mode.bump_flag=0;
	mode.time=giv_sys_time;

	mode.status=1;
	while(giv_sys_time-mode.time<5000);
	delay_ms(500);
	TRACE("Init Stop Backsweep complete!!!\r\n");
	Load_Abort_Data();		//然后再装在中断之前的数据
	TRACE("grid.x_abort=%d y_abort=%d\r\n",grid.x_abort,grid.y_abort);
}

void StopBack_Bump_Action(void)
{
	u8 m=0;
	static bool turn_flag=false;
	static u8 turn_dir=0;
	static u16 ypos_dif=0;
	short ypos,tgt_ypos;
	
	tgt_ypos=Return_GridYPos_Point(grid.y_abort);
	ypos=Gyro_Data.y_pos;
	m=Read_Sweep_Bump(0,0);
	//if(Judge_Ypos_Reach(motion1.ypos_abort,5))
	//if(Judge_Ypos_Reach(tgt_ypos,POS_BIOS))
	if(Judge_GridYPOS_Reach(grid.y_abort,0))
		{
			if(mode.step==3)
				{
					stop_rap();
					TRACE("Approach Y in %s\r\n",__func__);
					mode.bump=0;
					mode.bump_flag=false;
					mode.step_bp=0;
					mode.step++;
					return;
				}
		}
	if(mode.bump==0)
		return;
	switch(mode.step_bp)
		{
			case 0:
				mode.bump_time=giv_sys_time;
				mode.step_bp++;
				turn_flag=false;
			break;
			case 1:
				if(giv_sys_time-mode.bump_time<BUMP_TIME_DELAY)
					return;
				Speed=BUMP_BACK_SPEED;
				if(do_action(4,BUMP_BACK_LENGTH*CM_PLUS))
					{
						stop_rap();
						mode.step_bp++;
					}
				break;
			case 2:
				if((mode.step!=3)&(mode.step!=7))
					{
						mode.step=0;
						mode.bump=0;
						mode.bump_flag=false;
						mode.step_bp=0;
						mode.bump_time=giv_sys_time;
					}
				else if(mode.step==3)			//沿Y轴移动
					{
						mode.step_bp=3;			
					}
				else							//沿X轴移动
					{
						mode.step_bp=7;						
					}
				break;
			case 3:
#if 0
				if(motion1.tgt_yaw==L_Angle_Const)
					{
						if(xpos>motion1.xpos_abort)
							{
								turn_dir=1;
							}
						else
							{
								turn_dir=2;
							}
					}
				else
					{
						if(xpos>motion1.xpos_abort)
							{
								turn_dir=2;
							}
						else
							{
								turn_dir=1;
							}
					}
#else
				if(motion1.tgt_yaw==L_Angle_Const)
					{
						turn_dir=2;
					}
				else
					{
						turn_dir=1;
					}
#endif
				//ypos_dif=abs(motion1.ypos_abort-ypos);
				ypos_dif=abs(tgt_ypos-ypos);
				mode.step_bp++;
				break;
			case 4:
				Speed=TURN_SPEED;
				if(do_action(turn_dir,40*Angle_1))
					{
						stop_rap();
						mode.step_bp++;
					}
				break;
			case 5:
				Speed=MID_MOVE_SPEED;
				if(do_action(3,5*CM_PLUS))
					{
						stop_rap();
						mode.step_bp++;
					}
				if(m)
					{
						stop_rap();
						mode.step_bp=8;
						mode.bump_time=giv_sys_time;
					}
				break;
			case 6:
				if(turn_dir==1)
					{
						enable_rap_no_length(FRONT,REVOLUTION_SPEED_HIGH,FRONT,REVOLUTION_SPEED_LOW);
					}
				else
					{
						enable_rap_no_length(FRONT,REVOLUTION_SPEED_LOW,FRONT,REVOLUTION_SPEED_HIGH);
					}
				#if 0
				if(Judge_Yaw_Reach(motion1.tgt_yaw,TURN_ANGLE_BIOS))
					{
						stop_rap();
						mode.bump=0;
						mode.step_bp=0;
						mode.bump_flag=false;
						mode.step=3;
					}
				#endif
				if(m)
					{
						stop_rap();
						mode.step_bp=8;
						mode.bump_time=giv_sys_time;
					}
				break;
			case 8:
				if(giv_sys_time-mode.bump_time<1000)
					return;
				Speed=MID_MOVE_SPEED;
				if(do_action(4,2*CM_PLUS))
					{
						stop_rap();
						mode.step_bp=4;
						//if(abs(motion1.ypos_abort-ypos)>ypos_dif+25)
						if(abs(tgt_ypos-ypos)>ypos_dif+25)
							{
								if(!turn_flag)
									{
										if(turn_dir==1)
											turn_dir=2;
										else
											turn_dir=1;
										turn_flag=true;
									}
							}
					}
				break;
				
			case 7:
				m=mode.bump;
				Init_NormalSweep(motion1.tgt_yaw);
				mode.bump=m;
				mode.step_bp=0;
				break;
		}
}


void Do_Stop_BackSweep(void)
{
	short ypos,tgt_xpos,tgt_ypos;
	static u8 turn_dir=0;
	static short tgt_yaw=0;
	ypos=Gyro_Data.y_pos;
	tgt_xpos=Return_GridXPos_Point(grid.x_abort);tgt_ypos=Return_GridYPos_Point(grid.y_abort);

	ACC_DEC_Curve();
	
	clr_all_hw_effect();
	u8 abnormal=Read_Protect();
	
	if(mode.abnormity)
		{
			Action_Protect_My(abnormal);
#ifdef FREE_SKID_INDEP_CHECK
			Free_Skid_Indep.check_flag=false;
#endif

#ifdef YBS_DIS_RESTORE
			Disable_Rotate_Angle();
#endif
			mode.speed_up=false;		//qz add 20181225

			return;
		}

	
	StopBack_Bump_Action();
	if((Judge_Pos_Reach(tgt_xpos,tgt_ypos))&(mode.step<4))
		{
			stop_rap();
			TRACE("pos reached in %s\r\n",__func__);
			TRACE("goto mode.step 4\r\n");
			mode.step=4;
			mode.bump=0;
			mode.step_bp=0;
		}
	if(mode.bump)
		return;
	
	switch(mode.step)
		{	
			case 0:
				if(giv_sys_time-mode.bump_time<200)
					return;
				if(ypos+3<tgt_ypos)			//当前Y坐标小于中断坐标，需要转到R_ANGLE_CONST
					{
						tgt_yaw=R_Angle_Const;
					}
				else if(ypos>tgt_ypos+3)			//当前Y坐标大于中断坐标，需要转到R_ANGLE_CONST
					{
						tgt_yaw=L_Angle_Const;
					}
				else
					{
						TRACE("now ypos=%d tgt_ypos=%d\r\n",ypos,tgt_ypos);
						TRACE("goto mode step 4!!\r\n");
						mode.step=4;
						return;
					}
				TRACE("tgt_yaw=%d\r\n",tgt_yaw);
				mode.step++;
				break;
			case 1:
//				turn_dir= Get_TurnDir(motion1.tgt_yaw);
				mode.step++;
				break;
			case 2:
				Speed=TURN_SPEED;
				turn_dir= Get_TurnDir(tgt_yaw);
				do_action(turn_dir,360*Angle_1);
				if(Judge_Yaw_Reach(tgt_yaw,TURN_ANGLE_BIOS))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 3:
				Speed=HIGH_MOVE_SPEED;
				do_action_my(3,FARAWAY,tgt_yaw);
				//if(motion1.tgt_yaw==L_Angle_Const)

				if(Judge_GridYPOS_Reach(grid.y_abort, tgt_yaw))
					{
						stop_rap();
						mode.step++;
						TRACE("Approch grid.y_abort in %s %d\r\n",__func__,__LINE__);
					}
				break;
			case 4:
				//Set_Motion_BackSweep(0);	//清空回扫标志
				TRACE("grid.y_abort=%d x_abort=%d\r\n",grid.y_abort,grid.x_abort);
				TRACE("grid.y=%d x=%d\r\n",grid.y,grid.x);
				TRACE("Stop BackSweep action complete!\r\n");
				TRACE("Prepare enter to Normal Sweep and then Load Data!\r\n");
				mode.step++;
				break;
			case 5:
//				turn_dir=Get_TurnDir(motion1.tgt_yaw);
				mode.step++;
				break;
			case 6:
				Speed=TURN_SPEED;
				turn_dir=Get_TurnDir(motion1.tgt_yaw);
				if(do_action(turn_dir,720*Angle_1))
					{
						stop_rap();
						mode.step=6;
					}
				if(Judge_Yaw_Reach(motion1.tgt_yaw,TURN_ANGLE_BIOS))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 7:
				if(Analysis_Back_Leak())
					{
						Add_BackInfo();
						Set_Motion_BackSweep(1);	//设置回扫标志
//						Save_Abort_Data();
						Change_LeftRight();
						Set_Motion_YDir_Reverse();
						Init_Shift_Point1(0);
						return;
					}
				if(motion1.back_sweep)
					{
						if(!motion1.leakon)
							Init_Back_Sweep(motion1.tgt_yaw);
						else
							Init_Leak_BackSweep(motion1.tgt_yaw);
					}
				else
					{
						if(!motion1.leakon)
							Init_NormalSweep(motion1.tgt_yaw);		//先进入正常直行清扫模式
						else
							Init_LeakSweep(motion1.tgt_yaw);
					}
				break;
		}
}

void Init_Pass2Init(short tgt_yaw,u8 y_acc,u8 x_acc)
{
	mode.last_mode=mode.mode;		//qz add 20180205
	mode.last_sub_mode=mode.sub_mode;
	/******初始化显示***********/
		
	/******初始化设置的值********************/
	clr_ram();
//	ReInitAd();

	Enable_earth();
	Enable_wall();
	enable_hwincept();				//允许红外接收电源
	Enable_Speed(); 				//允许速度发送
	Init_Action();
	
	mode.mode = PASS2INIT;			
	mode.sub_mode = PASS2INIT;
	mode.step=0;
	mode.time=giv_sys_time;
	mode.bump = 0;
	mode.step_bp = 0;
	mode.bump_flag=0;
	mode.Info_Abort=0;				//qz add 20180919
	mode.All_Info_Abort=0;			//qz add 20180919

	mode.status=1;
	WriteWorkState();

	w_l.on=0;
	w_r.on=0;
	w_rm.on=0;
	w_lm.on=0;
#ifdef DEBUG_Enter_Mode
	TRACE("Init PASS2INIT Mode Complete!\r\n");
#endif
	//初始化检测的条件
	CHECK_STATUS_FLAG=true; 	//qz add 20180725:如果机器处于休眠时,接到控制命令会直接进入此状态,因此需要打开检测开关
//	Init_Check_Status();//qz add 20180425
#ifdef FREE_SKID_CHECK
	Enable_Free_Skid_Check();		//打开万向轮检测
#endif
	memset(USART1_RX_BUF,0,sizeof(USART1_RX_BUF)/sizeof(USART1_RX_BUF[0])); 	//清除接收缓存qz add 20180703
	
	if(dis_err_code==DIS_ERROR_DOCK_FAIL)		//qz add 20180710
		{
			dis_err_code=0;
			error_code=0;
		}


	motion1.tgt_yaw=tgt_yaw;
	motion1.y_acc=y_acc;
	motion1.x_acc=x_acc;
}

void Do_Pass2Init(void)
{
	static u8 turn_dir;
//	YBS_Comm_Rap_My();
	ACC_DEC_Curve();
	clr_all_hw_effect();
	u8 abnormal=Read_Protect();
	if(mode.abnormity)
		{
			Action_Protect_My(abnormal);
#ifdef FREE_SKID_INDEP_CHECK
			Free_Skid_Indep.check_flag=false;
#endif

#ifdef YBS_DIS_RESTORE
			Disable_Rotate_Angle();
#endif
			mode.speed_up=false;		//qz add 20181225

			return;
		}
	
	Pass2Sweep_Bump_Action();
	if(mode.bump)
		return;
	switch(mode.step)
		{
			case 0:
//				turn_dir=Get_TurnDir(motion1.tgt_yaw);
				mode.step++;
				break;
			case 1:
				Speed=TURN_SPEED;
				turn_dir=Get_TurnDir(motion1.tgt_yaw);
				do_action(turn_dir,360*Angle_1);
				if(Judge_Yaw_Reach(motion1.tgt_yaw,TURN_ANGLE_BIOS))
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 2:
				Init_Init_Sweep(motion1.tgt_yaw,motion1.x_acc,motion1.y_acc);
				break;
		}
}

void Init_Sweep_RightYBS(u8 avoid_staright)
{
	mode.last_mode=mode.mode;
	mode.last_sub_mode=mode.sub_mode;
    /******初始化显示***********/
	
	clr_ram();
	Enable_earth();						//打开地检
	Enable_wall();						//打开墙检
	enable_hwincept();					//允许红外接收电源
	Enable_Speed();					//打开速度检测
#if 0
	if(DOCK_SWEEP)
		Sweep_Level_Set(DOCK_SWEEP_LEVEL);
	else
		Sweep_Level_Set(sweep_suction);
#endif	 
	Init_Action();
	//	ReInitAd();
	//clr_all_hw_struct();				//清零回充信号标志	//qz modify 20181210 effect-->struct	//qz mask 20181215
	WriteWorkState();					//功能：保存工作状态
	

	mode.mode = SWEEP;			
	mode.sub_mode = YBS_SUB_RIGHT;		
	mode.step_bp = 0;
	mode.bump = 0;
	mode.Info_Abort=0;				//qz add 20180919
	mode.All_Info_Abort=0;			//qz add 20180919
	mode.status=1;
	mode.time=giv_sys_time;
	if(avoid_staright==0)
		{
			mode.step = 0x88;//QZ:原来为0x88;
		}
	else
		{
			mode.step = 0;
		}
#ifdef FREE_SKID_CHECK
	Enable_Free_Skid_Check();			//打开万向轮检测	
#endif
#ifdef ROTATE_SKID_CHECK	
	Enable_Rotate_Skid_Check(0);
#endif

	while(giv_sys_time-mode.time<1000);
	motion1.xpos_ybs_start=Gyro_Data.x_pos;
	motion1.ypos_ybs_start=Gyro_Data.y_pos;
	grid.x_ybs_start=grid.x;
	grid.y_ybs_start=grid.y;
#ifdef DEBUG_Enter_Mode
	TRACE("Init Sweep Right YBS Mode Complete!\r\n");
#endif

	//初始化检测的条件
//	Init_Check_Status();//qz add 20180425
//	mode.fun=YBS_read_bump1;
#ifdef UV
	//if((mode.status)&(!SLAM_DOCK))		//qz add 20180902
	//	Set_UV();
	//else									//qz add 20180910,经过调查发现,沿边时UV灯打开以后,会对右红外发现充电座信号产生严重影响,导致不能发现回充座，所以先关掉UV
		Reset_UV();
#endif

	YBS_DISTANCE=YBS_DISTANCE_CONST;

	TRACE("motion1.ybs_start_xpos=%d ypos=%d\r\n",motion1.xpos_ybs_start,motion1.ypos_ybs_start);
	motion1.continue_checkstep=0;			//qz add 20190328
	YBS_check_base=false;
}

void Init_Sweep_LeftYBS(u8 avoid_staright)
{
	mode.last_mode=mode.mode;
	mode.last_sub_mode=mode.sub_mode;
	/******初始化显示***********/
	
	clr_ram();
	Enable_earth(); 					//打开地检
	Enable_wall();						//打开墙检
	enable_hwincept();					//允许红外接收电源
	Enable_Speed(); 				//打开速度检测
#if 0
	if(DOCK_SWEEP)
		Sweep_Level_Set(DOCK_SWEEP_LEVEL);
	else
		Sweep_Level_Set(sweep_suction);
#endif
	 
	Init_Action();
	//	ReInitAd();
	clr_all_hw_effect();				//清零回充信号标志
	WriteWorkState();					//功能：保存工作状态
	

	mode.mode = SWEEP;			
	mode.sub_mode = YBS_SUB_LEFT;		
	mode.step_bp = 0;
	mode.bump = 0;
	mode.Info_Abort=0;				//qz add 20180919
	mode.All_Info_Abort=0;			//qz add 20180919
	mode.time=giv_sys_time;
	mode.status=1;
	if(avoid_staright==0)
		mode.step = 0x88;//QZ:原来为0x88;
	else
		mode.step=0x40;
	Enable_Free_Skid_Check();			//打开万向轮检测 

#ifdef ROTATE_SKID_CHECK	
	Enable_Rotate_Skid_Check(0);
#endif

	while(giv_sys_time-mode.time<1000);
	motion1.xpos_ybs_start=Gyro_Data.x_pos;
	motion1.ypos_ybs_start=Gyro_Data.y_pos;
	grid.x_ybs_start=grid.x;
	grid.y_ybs_start=grid.y;
#ifdef DEBUG_Enter_Mode
	TRACE("Init Sweep Left YBS Mode Complete!\r\n");
#endif
	TRACE("motion1.ybs_start_xpos=%d ypos=%d\r\n",motion1.xpos_ybs_start,motion1.ypos_ybs_start);
	//初始化检测的条件
//	Init_Check_Status();//qz add 20180425
//	mode.fun=YBS_read_bump1;
	motion1.continue_checkstep=0;		//qz add 20190328
	YBS_check_base=false;
}

u8 YBS_AbortFor_Sweep(void)
{
	s8 temp_data1,now_gridx,now_gridy;
	short xpos,ypos;
	s8 ydir=Read_Motion_YDir();
	xpos=Gyro_Data.x_pos;ypos=Gyro_Data.y_pos;
	now_gridx=grid.x;now_gridy=grid.y;	

	//if(ypos+20<motion1.ypos_ybs_start)				//沿边y坐标小于起始沿边y坐标15cm
	if(Judge_GridYPOS_Nearby_Reach(grid.y_straight_start)&(grid.y<grid.y_straight_start))
		{
			if(ydir>0)		//沿Y轴正方向清扫
				{
					if(Read_Motion_BackSweep())
						{
							stop_rap();
							TRACE("Quit Now backsweep in %s\r\n",__func__);
							Init_Stop_BackSweep();
							return 1;
						}
					else
						{
							stop_rap();
							TRACE("motion1.x_ybs=%d\r\n",motion1.xpos_ybs_start);
							TRACE("motion1.y_ybs=%d\r\n",motion1.ypos_ybs_start);
							TRACE("Gyro.x_pos=%d\r\n",xpos);
							TRACE("Gyro.y_pos=%d\r\n",ypos);
							TRACE("back check!!!\r\n");
							//Logout_Area_Coordinate();
							//y坐标回溯超过20cm时，开启左沿边
							Area_Check(1);
							return 1;
//							Logout_Area_Coordinate();
							//while(1);
						}
				}
			else if(ydir<0)							//沿Y坐标负向方向
				{
					if(abs(now_gridx-grid.x_straight_start)<=1)
						{
							if((motion1.tgt_yaw==F_Angle_Const)&(abs(grid.x_straight_start-grid.x_area_min)<=1))
								{
									return 0;
								}
							else if((motion1.tgt_yaw==B_Angle_Const)&(abs(grid.x_straight_start-grid.x_area_max)<=1))
								{
									return 0;
								}
							YBS_check_base=true;
						}
					stop_rap();
					TRACE("motion1.y_pos_ybs=%d\r\n",motion1.ypos_ybs_start);
					TRACE("y_pos=%d\r\n",ypos);
					TRACE("ypos<motion1_ypos+20!\r\n");
					//if(((mode.step==0x11)|(mode.step==0x51))&(Judge_Yaw_Reach(motion1.tgt_yaw,6000)))
					if(((mode.step==0x11)|(mode.step==0x51)))
						{
							if((now_gridx>grid.x_area_min)&(now_gridx<grid.x_area_max))

								{
									TRACE("Need Repeat!\r\n");
									motion1.repeat_sweep=true;
								}
						}
					else
						{
							TRACE("No Need Repeat!\r\n");
							motion1.repeat_sweep=false;
						}
						Init_Pass2Sweep();
					return 1;
				}
			else
				{
					Set_Motion_YDir(-1);
					Init_Pass2Sweep();
					return 1;
				}
		}
	//else if(ypos>motion1.ypos_ybs_start+20)		//沿边y坐标大于起始沿边y坐标15cm
	else if(Judge_GridYPOS_Nearby_Reach(grid.y_straight_start)&(grid.y>grid.y_straight_start))
		{
			if(ydir>0)		//沿Y轴正方向清扫
				{
					if(abs(now_gridx-grid.x_straight_start)<=1)
						{
							if((motion1.tgt_yaw==F_Angle_Const)&(abs(grid.x_straight_start-grid.x_area_min)<=1))
								{
									return 0;
								}
							else if((motion1.tgt_yaw==B_Angle_Const)&(abs(grid.x_straight_start-grid.x_area_max)<=1))
								{
									return 0;
								}								
							YBS_check_base=true;
						}
					stop_rap();
					TRACE("motion1.y_pos_ybs=%d\r\n",motion1.ypos_ybs_start);
					TRACE("y_pos=%d\r\n",ypos);
					TRACE("ypos>motion1_ypos+20!\r\n");
					//if(((mode.step==0x11)|(mode.step==0x51))&(Judge_Yaw_Reach(motion1.tgt_yaw,6000)))
					if(((mode.step==0x11)|(mode.step==0x51)))		//如果接近于最大区域，则不用考虑重扫
						{
							if((now_gridx>grid.x_area_min)&(now_gridx<grid.x_area_max))

								{
									TRACE("Need Repeat!\r\n");
									motion1.repeat_sweep=true;
								}
						}
					else
						{
							TRACE("No Need Repeat!\r\n");
							motion1.repeat_sweep=false;
						}
						Init_Pass2Sweep();
					return 1;
				}
			else if(ydir<0)
				{
					if(Read_Motion_BackSweep())
						{
							stop_rap();
							TRACE("Quit Now backsweep in %s\r\n",__func__);
							Init_Stop_BackSweep();
							return 1;
						}
					else
						{
							stop_rap();
							TRACE("motion1.x_ybs=%d\r\n",motion1.xpos_ybs_start);
							TRACE("motion1.y_ybs=%d\r\n",motion1.ypos_ybs_start);
							TRACE("Gyro.x_pos=%d\r\n",xpos);
							TRACE("Gyro.y_pos=%d\r\n",ypos);
							TRACE("back check!!!\r\n");
							//Logout_Area_Coordinate();
							//y坐标回溯超过20cm时，开启左沿边
							Area_Check(1);
							return 1;
//							Logout_Area_Coordinate();
							//while(1);
						}
				}
			else
				{
					Set_Motion_YDir(1);
						Init_Pass2Sweep();
					return 1;
				}
		}

	if(((mode.bump==BUMP_YMAX_OUT)&(ydir>0))|((mode.bump==BUMP_YMIN_OUT)&(ydir<0)))
		{			
			stop_rap();
			TRACE("ydir=%d mode.bump=%d \r\n",ydir,mode.bump);
			TRACE("stop for Area Check!!!\r\n");
			Area_Check(1);
			return 1;
		}

	//判断沿边时是否绕过障碍，绕过则继续直行清扫
	if((Parse_ContinueInYBS())&(mode.step<0xD0))		
		{
			stop_rap();
			mode.step=0xD0;
			TRACE("Gyro.yaw=%d motion.tgt_yaw=%d\r\n",Gyro_Data.yaw,motion1.tgt_yaw);	
			TRACE("Round Obstacles!!!\r\n");
			if((Read_Motion_BackSweep())&(mode.last_mode==NORMAL_SWEEP))					//如果刚刚判断需要回扫,当前还是正常直行状态
				{
					TRACE("It should be BackSweep,but Round Obstacles,");
					TRACE("So temproray to NormalSweep!\r\n");
					Set_Motion_BackSweep(0);										//绕过障碍后，先取消回扫，正常直行
					Load_Abort_Data();		//然后再装在中断之前的数据
				}
			return 1;
		}

	if(YBS_check_base)
		{
			if((now_gridx==grid.x_sweep_start)&(now_gridy==grid.y_sweep_start))
				{
					stop_rap();
					TRACE("YBS check base!!!\r\n");
					//y坐标回溯超过20cm时，开启左沿边
					YBS_check_base=false;
					Area_Check(0);
					return 1;
				}
		}

#if 1
	if((giv_sys_time-mode.time>250000)&(mode.last_sub_mode!=BACK_SWEEP)&(mode.last_sub_mode!=LEAK_BACKSWEEP))
		{
			if((grid.x==grid.x_ybs_start)&(grid.y==grid.y_ybs_start))
				{
					stop_rap();
					TRACE("The Motion ybs reach the orignal point!!!\r\n");
					motion1.area_ok=true;
					Set_CurrNode_LeakInfo(motion1.area_ok);
					Area_Check(0);
				}
		}
#endif

	return 0;
}


void Sweep_YBS(void)
{
//  static u8 piv_out;  //机器是否向外展开，1为向外展开，0为向里缩小
//  static u8 piv_left; //机器是否向左转，1为向左转，0为向右转
	u8 temp_data1=0;
	u8 abnormal;
	u32 uin32;
	static u8 turn_dir=0;

#if 1		
#ifdef DC_NOBAT_RUN
	if((power.charge_dc)&(!dc_nobat_run))
#else
	if(power.charge_dc)
#endif
		{
			stop_rap();
			uin32 = giv_sys_time;
			while(giv_sys_time - uin32 < 20000)
				{
					AutoReadKey();
					judge_charge();
				}
				 
#ifdef DC_NOBAT_RUN
			if((power.charge_dc)&(!dc_nobat_run))
#else
			if(power.charge_dc)
#endif
				{
					 Init_Chargeing(DC_CHARGING);
					 return;
				}					
		}
#endif

#ifdef PITCH_SPEEDUP
  	if(Gyro_Pitch_Speedup())
  		{
			mode.speed_up=true;
  		}
	else
		mode.speed_up=false;
#endif

//	YBS_comm_rap();	
//	if(Gyro_Data.straight)
		ACC_DEC_Curve();
//	else
//		YBS_Comm_Rap_My();

	clr_all_hw_effect();			//qz add 20181210

	if(mode.sub_mode==YBS_SUB_RIGHT)
		YBS_Check_corner();
	else
		YBS_Left_Check_corner();
	
//	YBS_Wall_Count();		//qz add 20180902

	abnormal=Read_Protect();
	if(mode.abnormity)
		{
			Action_Protect_My(abnormal);
#ifdef FREE_SKID_INDEP_CHECK
			Free_Skid_Indep.check_flag=false;
#endif

#ifdef YBS_DIS_RESTORE
			Disable_Rotate_Angle();
#endif
			mode.speed_up=false;		//qz add 20181225

			return;
		}

	if(YBS_AbortFor_Sweep())
		return;
	
	
	if((mode.sub_mode==YBS_SUB_RIGHT))				//	RIGHT
		{
			YBS_Right_Bump(1);
		}		
	else if((mode.sub_mode == YBS_SUB_LEFT))		//	LEFT
		{
			YBS_Left_Bump(1);
		}
	else
		return;
			
	if(mode.bump != 0)		//	有碰撞需要处理，返回d
		{
#if 0
			Wall_lost_counter = 0;
			if(mode.step>=0x88)
				{
					if(mode.sub_mode==YBS_SUB_RIGHT)
					{
						//mode.step=0x00;			//qz mask 20180910
					}
					else
						mode.step=0x40;
				}
#endif

#ifdef ROTATE_SKID_CHECK
			Disable_Rotate_Skid_Check();
#endif

#ifdef YBS_DIS_RESTORE
			Disable_Rotate_Angle();
#endif

#ifdef FREE_SKID_INDEP_CHECK
			Free_Skid_Indep.check_flag=false;
#endif
			return;
		}
	
	//qz add 20180228
#ifdef	FREE_SKID_CHECK
	if(Check_Free_Sikd())
		{
			Slam_Data.skid_flag=1;
#ifdef SKID_REPORT_TIME
			Slam_Data.skid_report_time=giv_sys_time;
#endif
#ifdef FREE_SKID_ACTION
			stop_rap();
			mode.step=0xB0;
#endif
		}
#endif

#if 0
	#ifdef ROTATE_SKID_CHECK
	if(Check_Rotate_Skid())
		{
			Slam_Data.skid_flag=1;
#ifdef ROTATE_SKID_ACTION
			stop_rap();
			Disable_Rotate_Skid_Check();
			
			mode.step=0xC0;
#endif
			
		}
	#endif
#endif
	//qz add end
	
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	
	switch (mode.step)	//step路径执行的步骤
		{
			case 0x88:
#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif
#ifndef YBS_START_RIGHT_ANGLE
				enable_rap_no_length(FRONT, 3500, FRONT, 3000);	//qz modify 20180703:走斜线
#else
				Speed=FAST_MOVE_SPEED;//2000							
				if(do_action(3,100*CM_PLUS))		//直行1m
					{
						stop_rap();
						mode.step++;
					}
#ifdef	ROTATE_SKID_CHECK
				Disable_Rotate_Skid_Check();
#endif
				break;
			//qz add 20180801
			case 0x89:
#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=false;
#endif
				Speed=FAST_MOVE_SPEED;
				if(mode.sub_mode==YBS_SUB_RIGHT)
					temp_data1=2;
				else
					temp_data1=1;
				if(do_action(temp_data1,90*Angle_1))			//右转90度
					{
						stop_rap();
						mode.step++;
					}
				break;
			case 0x8A:
#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif
				Speed=FAST_MOVE_SPEED;//2000
				if(do_action(3,100*CM_PLUS))		//直行5m
					{
						stop_rap();
						mode.step=0x88;
					}
#endif					
#ifdef	ROTATE_SKID_CHECK
				Disable_Rotate_Skid_Check();
#endif
				break;
					
			case 0:
				if(mode.sub_mode==YBS_SUB_LEFT)
					{
						mode.step=0x40;
						return;
					}
				if(giv_sys_time-mode.bump_time<200)
					return;
				Speed=HIGH_MOVE_SPEED;		//800//2000
				forward(0xFF812345);
				mode.step = 1;
				//qz add 20180316
#ifdef ROTATE_SKID_CHECK
				Disable_Rotate_Skid_Check();
#endif

#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif				
				break;
			
			case 1:
				//YBS_Check_corner(); 	//QZ:利用w_r的difference，计算distance，每次得到一个新的YBS_Wall_Distance
				//YBS_Distance_Check(0);
				Wall_Comm_Rap();
				if(YBS_Wall_Distance < 80)		//80	//	第一次找到墙
				//if(YBS_Wall_Distance<CONST_DIS+YBS_DISTANCE)
					{
						mode.step = 2;
#ifdef	YBS_Straight_FAST
						YBS_Straight_Time=giv_sys_time;
#endif

#ifdef YBS_DIS_RESTORE			//准备检查里程计算出的机器角度
						Enable_Rotate_Angle();
#endif
						return;
					}
				//QZ ADD
				//if(YBS_Wall_Distance>140)	//140
				else
					{
						mode.step=3;
					}
				
#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif
				break;
							
			case 2:
				//YBS_Check_corner(); 					
				//YBS_Distance_Check(0);
				Wall_Comm_Rap();
				if(YBS_Wall_Distance > 80)  			//	彻底丢失墙壁    有可能出现拐角//80  //140
				//if(YBS_Wall_Distance>=CONST_DIS+YBS_DISTANCE)
					{
						mode.step = 3;

#ifdef YBS_DIS_RESTORE		//出现空旷区域，停止检测里程计角度
						Disable_Rotate_Angle();
#endif
					}

#ifdef YBS_DIS_RESTORE		//检查里程计计算的角度，如果角度大于8度，恢复YBS_DISTANCE_CONST
				Check_Rotate_Angle();
				if((rotate_angle.rot_angle>(12.0))&(YBS_DISTANCE>YBS_DISTANCE_CONST))	//qz modify 20180902:8.0-->6.0-->12.0
					{
						YBS_DISTANCE=YBS_DISTANCE_CONST;
					}
#endif


#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif
				break;
			
			case 3:
//					Wall_lost_Start_Pos = r_rap.pulse;							//	向前走一点		//qz mask 20180203
				Wall_lost_Start_Pos=l_ring.all_length;
				r_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;
				l_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;

				mode.step = 4;	 
				break;
			case 4: 
				r_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;
				l_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;
				if((l_ring.all_length-Wall_lost_Start_Pos)>=3*CM_PLUS)
				{
					//QZ:ADD
					stop_rap();

					//qz add
					if(YBS_Wall_Distance<CONST_DIS+YBS_DISTANCE)
						{
							mode.step=0x00;
							return;
						}
					//qz add end
					mode.step = 0x10;
				}

#if 0
				//qz add 20190107
				if((YBS_Continue_Time>0)&(YBS_Wall_Distance>160)) 	
					{
						stop_rap();
						mode.step=0xf0;
						Init_Pass2Sweep();
						return;
					}
				//qz add end
#endif
				break;
			case 0x10: 	  	
				Wall_lost_Start_Pos = l_ring.all_length;							//	旋转 
				r_rap.rap =YBS_LOST_CORNER_TURN_SPD_R;	//QZ:原来为300,350		
				l_rap.rap = YBS_LOST_CORNER_TURN_SPD ;
				l_rap.sign=1;
				r_rap.sign=1;
				l_rap.ori=FRONT;
				r_rap.ori=FRONT;
				mode.step = 0x11;
				YBS_DISTANCE=YBS_DISTANCE_CONST;		//qz add 20810803
				lost_turn_time=giv_sys_time;
				//qz add 20180316
#ifdef ROTATE_SKID_CHECK
				Enable_Rotate_Skid_Check(1);
#endif

#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=false;
#endif
				break;
			case 0x11:
				YBS_Check_corner();

				//QZ ADD
				r_rap.rap =YBS_LOST_CORNER_TURN_SPD_R;	//QZ:原来为300,350		
				l_rap.rap = YBS_LOST_CORNER_TURN_SPD ;
					
				l_rap.ori=FRONT;
				l_rap.sign=1;
				r_rap.ori=FRONT;
				r_rap.sign=1;

				if(giv_sys_time-lost_turn_time>10000)			//qz add 20180902 1.5s qz modify 20181201 1.5s--->1.0s
					{
						YBS_DISTANCE=YBS_DISTANCE_CONST;
					}
				if((YBS_Wall_Distance <= (YBS_DISTANCE + 20))|(w_m.high_sign==NEAR)|(w_rm.high_sign==NEAR))//||(M_WALL_DIS<100)||(RM_WALL_DIS<100))				//	第二次找到墙	//qz modify 20181225
					{
						//QZ:ADD
						stop_rap();
					//	mode.step = 0x12;
						//qz add
						r_rap.rap=HIGH_MOVE_SPEED;
						l_rap.rap=HIGH_MOVE_SPEED;
						l_rap.ori=FRONT;
						l_rap.sign=1;
						r_rap.ori=FRONT;
						r_rap.sign=1;
						mode.step =0x01;
					}
				break;
			case 0x12:   							//	停止旋转
				mode.step = 0x00;
				break;
			//--------------------------------------------------------
							

			
			//--------------------------------------------------------
			//--------------------------------------------------------
			case 0x40:  
				if(giv_sys_time-mode.bump_time<200)
					return;
				Speed=HIGH_MOVE_SPEED; 	//2000
				forward(0xFF812345);
				mode.step = 0x41;				
				//qz add 20180316
#ifdef ROTATE_SKID_CHECK
				Disable_Rotate_Skid_Check();
#endif
			
#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif
								
				break;
			case 0x41:  
				YBS_Left_Check_corner(); 	//QZ:利用w_r的difference，计算distance，每次得到一个新的YBS_Wall_Distance
				//YBS_Left_Distance_Check();
				Wall_Comm_Rap();
				if(YBS_Wall_Distance < 80)		//80	//	第一次找到墙
					{
						mode.step = 0x42;
#ifdef	YBS_Straight_FAST
						YBS_Straight_Time=giv_sys_time;
#endif
			
#ifdef YBS_DIS_RESTORE		//准备检查里程计算出的机器角度
						Enable_Rotate_Angle();
#endif
					}
					//QZ ADD
				if(YBS_Wall_Distance>140)	//140
					{
						mode.step=0x43;
					}
								
#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif
								
								
				break;
			case 0x42:
				YBS_Left_Check_corner(); 					
				//YBS_Left_Distance_Check();
				Wall_Comm_Rap();
				if(YBS_Wall_Distance > 140) 			//	彻底丢失墙壁	  有可能出现拐角//80
						{
							mode.step = 0x43;

#ifdef YBS_DIS_RESTORE		//出现空旷区域，停止检测里程计角度
							Disable_Rotate_Angle();
#endif

						}

#ifdef YBS_DIS_RESTORE		//检查里程计计算的角度，如果角度大于8度，恢复YBS_DISTANCE_CONST
				Check_Rotate_Angle();
				if((rotate_angle.rot_angle<(-8.0))&(YBS_DISTANCE>YBS_DISTANCE_CONST))
					{
						YBS_DISTANCE=YBS_DISTANCE_CONST;
					}
#endif


#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=true;
#endif
				break;
			//--------------------------------------------------------
			//--------------------------------------------------------
			case 0x43:
				Wall_lost_Start_Pos=r_ring.all_length;
				r_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;
				l_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;

				mode.step = 0x44;	 
				break;
			case 0x44:
				r_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;
				l_rap.rap = YBS_LOST_CORNER_FORWARD_SPEED;
				if((r_ring.all_length-Wall_lost_Start_Pos)>=3*CM_PLUS)
				{
					//QZ:ADD
					stop_rap();
					mode.step = 0x50;
				}

				//qz add
				if(YBS_Wall_Distance<80)
					{
						mode.step=0x40;
					}
				//qz add end

#if 0
				//qz add 20190107
				if((YBS_Continue_Time>0)&(YBS_Wall_Distance>160)) 	
					{
						stop_rap();
						mode.step=0xf0;
						Init_Pass2Sweep();
						return;
					}
				//qz add end
#endif
				break;
			case 0x50:
				Wall_lost_Start_Pos = r_ring.all_length;							//	旋转 
				r_rap.rap 	=YBS_LOST_CORNER_TURN_SPD;	//QZ:原来为300,350		
				l_rap.rap 	=YBS_LOST_CORNER_TURN_SPD_R ;
				l_rap.sign	=1;
				r_rap.sign	=1;
				l_rap.ori	=FRONT;
				r_rap.ori	=FRONT;
				mode.step 	= 0x51;
				YBS_DISTANCE=YBS_DISTANCE_CONST;		//qz add 20810803

				//qz add 20180316
#ifdef ROTATE_SKID_CHECK
				Enable_Rotate_Skid_Check(1);
#endif

#ifdef FREE_SKID_INDEP_CHECK
				Free_Skid_Indep.check_flag=false;
#endif
				break;
			case 0x51:
				YBS_Left_Check_corner();

				//QZ ADD
				r_rap.rap =YBS_LOST_CORNER_TURN_SPD;	//QZ:原来为300,350		
				l_rap.rap = YBS_LOST_CORNER_TURN_SPD_R ;
				
				l_rap.ori=FRONT;
				l_rap.sign=1;
				r_rap.ori=FRONT;
				r_rap.sign=1;

#if 0			//取消底盘孤岛判断
				if(l_ring.all_length-Wall_lost_Start_Pos>(angle720+angle360))
					{
						stop_rap();
						mode.step=0x88;//0x12
#ifdef FREE_SKID_CHECK
						Enable_Free_Skid_Check();
#endif
#ifdef YBS_BLACK
						YBS_DISTANCE=30;
#endif
						return;
					}
									//QZ ADD END
#endif 
				if((YBS_Wall_Distance < (YBS_DISTANCE + 20))||(w_rm.sign==NEAR)||(w_m.high_sign==NEAR)|(w_lm.high_sign==NEAR))//||(M_WALL_DIS<100)||(RM_WALL_DIS<100))				//	第二次找到墙
						{
							//QZ:ADD
							stop_rap();
					//		mode.step = 0x12;
							//qz add
							r_rap.rap=HIGH_MOVE_SPEED;
							l_rap.rap=HIGH_MOVE_SPEED;
							l_rap.ori=FRONT;
							l_rap.sign=1;
							r_rap.ori=FRONT;
							r_rap.sign=1;
							mode.step =0x41;
						}
				break;
								
			//绕过障碍后的处理过程
  			case 0xD0:
				if(giv_sys_time-mode.time<5000)
					return;
				mode.step++;
//				turn_dir=Get_TurnDir(motion1.tgt_yaw);
				TRACE("now_angle=%d turn_dir=%d\r\n",Gyro_Data.yaw,turn_dir);
				break;
			case 0xD1:
				Speed=TURN_SPEED;
				turn_dir=Get_TurnDir(motion1.tgt_yaw);
				if(do_action(turn_dir,720*Angle_1))
					{	
						stop_rap();
					}
				if(Judge_Yaw_Reach(motion1.tgt_yaw,TURN_ANGLE_BIOS))
					{	
						stop_rap();
						mode.step++;
					}
				break;
			case 0xD2:
				Continue_Sweep();
				break;
				//	不停继续转圈
			default:
				mode.step=0x88;
				break;
		}	//	end of      switch (mode.step)	//step路径执行的步骤
}



