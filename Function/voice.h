#ifndef _VOICE_H
#define _VOICE_H
//#define  ONE_LINE_VOICE		1
//#define  OLD_VOICE		1
#define	NEW_VOICE	1

///////////////////����оƬ��������///////////////////////////
#define  VOICE_CLK		 GPIO_Pin_5		//PB NEW
#define	 VOICE_DATA		 GPIO_Pin_4		//PB NEW
#define  VOICE_GPIO		 GPIOB
#define	 VOICE_BUSY_PIN	 GPIO_Pin_3	//PA
#define	 VOICE_BUSY_GPIO GPIOB
#define  V_CLK_1		 GPIO_SetBits(VOICE_GPIO,VOICE_CLK)
#define  V_CLK_0	     GPIO_ResetBits(VOICE_GPIO,VOICE_CLK)
#define  V_DAT_1		 GPIO_SetBits(VOICE_GPIO,VOICE_DATA)
#define  V_DAT_0         GPIO_ResetBits(VOICE_GPIO,VOICE_DATA)

#ifdef  NEW_VOICE_IC
#define  ISVOICEBUSY    ! GPIO_ReadInputDataBit(VOICE_BUSY_GPIO,VOICE_BUSY_PIN)
#else
#define  ISVOICEBUSY     GPIO_ReadInputDataBit(VOICE_BUSY_GPIO,VOICE_BUSY_PIN)
#endif

#ifdef OLD_VOICE
#define		BASE_NUM					0
#define  	VOICE_SWEEPER_INIT	 		BASE_NUM
#define		VOICE_SWEEPER_OK			BASE_NUM+1
#define 	VOICE_WIFI_OK				BASE_NUM+2
#define		VOICE_WIFI_FAIL				BASE_NUM+3
#define		VOICE_WIFI_FIRST_BIND		BASE_NUM+4
#define 	VOICE_WIFI_BIND_OK			BASE_NUM+5
#define		VOICE_WIFI_BIND_FAIL		BASE_NUM+6
#define		VOICE_WIFI_DISBIND_OK		BASE_NUM+7
#define		VOICE_WIFI_DISBIND_FAIL		BASE_NUM+8
#define		VOICE_SWEEP_START			BASE_NUM+9
#define		VOICE_SWEEP_STOP			BASE_NUM+10
#define 	VOICE_DOCK_START			BASE_NUM+11
#define 	VOICE_DOCK_STOP				BASE_NUM+12
#define 	VOICE_CHARGING_START		BASE_NUM+13
#define		VOICE_CHARGING_FULL			BASE_NUM+14
#define		VOICE_DOCK_FAIL				BASE_NUM+15
#define		VOICE_ERROR_DUSTBOX_NONE	BASE_NUM+16
#define		VOICE_BOX_FULL				BASE_NUM+17
#define		VOICE_I_AM_HERE				BASE_NUM+18
#define		VOICE_POWER_LOW				BASE_NUM+19
#define		VOICE_UPGRADE_START			BASE_NUM+20
#define		VOICE_UPGRADE_OK			BASE_NUM+21
#define		VOICE_UPGRADE_FAIL			BASE_NUM+22

#define		VOICE_ERROR_LIFT			BASE_NUM+24			//Ϊ�������¾�����оƬ���޸�
#define		VOICE_ERROR_L_LIFT			BASE_NUM+24			//Ϊ�������¾�����оƬ���޸�
#define		VOICE_ERROR_R_LIFT			BASE_NUM+24			//Ϊ�������¾�����оƬ���޸�
#define		VOICE_LONG_PRESS			BASE_NUM+25			//����WIFI 3��ʱ�䵽

#define		VOICE_KEY_PRESS				BASE_NUM+27
#define		VOICE_VOLUME_2				BASE_NUM+28
#define		VOICE_VOLUME_3				BASE_NUM+29
#define		VOICE_VOLUME_4				BASE_NUM+26

#define 	VOICE_VOLUME_TEST			BASE_NUM+30
#define		VOICE_SWEEPER_RST			BASE_NUM+31
#define		VOICE_BOX_EXIST				BASE_NUM+32
#define		VOICE_ERROR_011				BASE_NUM+33
#define		VOICE_ERROR_012				BASE_NUM+34
#define		VOICE_ERROR_021				BASE_NUM+35
#define		VOICE_ERROR_031				BASE_NUM+36
#define		VOICE_ERROR_SLAM_TICK		BASE_NUM+37			//ERROR_041

#define		VOICE_ERROR_QJ_FIX			BASE_NUM+40			//ERROR_061
#define		VOICE_ERROR_QJ_FAIL			BASE_NUM+41			//ERROR_062
#define		VOICE_ERROR_ULTRA_FIX		BASE_NUM+42			//ERROR_071
#define		VOICE_ERROR_ULTRA_FAIL		BASE_NUM+43			//ERROR_072
#define		VOICE_ERROR_DJ_FIX			BASE_NUM+44			//ERROR_081
#define		VOICE_ERROR_DJ_FAIL			BASE_NUM+45			//ERROR_082
#define		VOICE_ERROR_BAT_FAIL		BASE_NUM+46			//ERROR_E09X
#define		VOICE_ERROR_FAN_OC			BASE_NUM+47			//ERROR_101
#define		VOICE_ERROR_FAN_FIX			BASE_NUM+48			//ERROR_102
#define		VOICE_ERROR_BUMP_FIX		BASE_NUM+49			//ERROR_111
#define		VOICE_ERROR_IR_FIX			BASE_NUM+50			//ERROR_121
#define		VOICE_ERROR_IR_FAIL			BASE_NUM+51			//ERROR_122
#define		VOICE_ERROR_L_RING			BASE_NUM+52			//Ϊ�������¾�����оƬ���޸�
#define		VOICE_ERROR_R_RING			BASE_NUM+52			//Ϊ�������¾�����оƬ���޸�����
#define		VOICE_ERROR_RING_OC			BASE_NUM+53			//ERROR_132
#define		VOICE_ERROR_SWEEP_FIX		BASE_NUM+54			//ERROR_133
#define		VOICE_ERROR_ZS_OC			BASE_NUM+55			//ERROR_141
#define		VOICE_ERROR_ZS_FIX			BASE_NUM+56			//ERROR_142
#define		VOICE_ERROR_BS_OC			BASE_NUM+57			//ERROR_151
#define		VOICE_ERROR_L_SB			BASE_NUM+58			//Ϊ�������¾�����оƬ���޸�
#define		VOICE_ERROR_R_SB			BASE_NUM+58			//Ϊ�������¾�����оƬ���޸ı�ɨ
#define		VOICE_ERROR_DC_EXIST		BASE_NUM+59			//�ε������
#endif

#ifdef NEW_VOICE
#ifdef NEW_VOICE_IC

#define  	VOICE_SWEEPER_INIT	 		1

#define		VOICE_SWEEP_START			2
#define		VOICE_SWEEP_STOP			3
#define		VOICE_SWEEP_DONE			4
#define 	VOICE_DOCK_START			5
#define 	VOICE_CHARGING_START		6
#define		VOICE_CHARGING_FULL			7
#define 	VOICE_WIFI_OK				8

#define		VOICE_KEY_PRESS				9
#define		VOICE_VOLUME_2				9
#define		VOICE_VOLUME_3				10
#define		VOICE_VOLUME_4				10
#define		VOICE_WIFI_RST				10

#define		VOICE_POWER_LOW				11

#define		VOICE_ERROR_DC_EXIST		12
#define		VOICE_ERROR_WARNING			12

#define		VOICE_ERROR_BUMP_FIX		13		//ERROR_111
#define		VOICE_ERROR_L_LIFT			13
#define		VOICE_ERROR_R_LIFT			13
#define		VOICE_ERROR_L_RING			13
#define		VOICE_ERROR_R_RING		13
#define		VOICE_ERROR_L_SB			13
#define		VOICE_ERROR_R_SB			13
#define		VOICE_ERROR_ZS_OC			13
#define		VOICE_ERROR_SWEEP_FIX		13

#define		VOICE_ERROR_GYRO			13
#define		VOICE_ERROR_L_WALL			13
#define		VOICE_ERROR_M_WALL			13
#define		VOICE_ERROR_R_WALL			13
#define		VOICE_ERROR_L_EARTH			13
#define		VOICE_ERROR_M_EARTH			13
#define		VOICE_ERROR_R_EARTH			13
#define		VOICE_ERROR_FAN_OC			13
#define		VOICE_ERROR_SLAM_TICK		13
#define		VOICE_ERROR_DIPAN_TICK		13
#define		VOICE_ERROR_BAT_FAIL		13
#define		VOICE_ERROR_VIDEO_FAIL		13
#define		VOICE_ERROR_BIRD_FAIL		13
#define		VOICE_ERROR_DANGER			13
#define		VOICE_ERROR_GYRO			13

#define		VOICE_SWEEPER_OK			50

#define		BASE_NUM					20
#define 	VOICE_DOCK_STOP				BASE_NUM+10
#define		VOICE_POWER_NONE			BASE_NUM+47

#define		VOICE_WIFI_FAIL				BASE_NUM+3
#define		VOICE_WIFI_FIRST_BIND		BASE_NUM+4
#define 	VOICE_WIFI_BIND_OK			BASE_NUM+5
#define		VOICE_WIFI_DISBIND_OK		BASE_NUM+6


#define		VOICE_DOCK_FAIL				BASE_NUM+13
#define		VOICE_ERROR_DUSTBOX_NONE	BASE_NUM+14
#define		VOICE_BOX_FULL				BASE_NUM+15
#define		VOICE_I_AM_HERE				BASE_NUM+16

#define		VOICE_UPGRADE_START			BASE_NUM+18
#define		VOICE_UPGRADE_OK			BASE_NUM+19
#define		VOICE_UPGRADE_FAIL			BASE_NUM+20

#define		VOICE_ERROR_LIFT			BASE_NUM+21

#define 	VOICE_VOLUME_TEST			BASE_NUM+27
#define		VOICE_SWEEPER_RST			BASE_NUM+28
#define		VOICE_BOX_EXIST				BASE_NUM+29
#define		VOICE_STOP_CHARGE			BASE_NUM+31			//ֹͣ���

#define		VOICE_WIFI_BIND_FAIL		BASE_NUM+41
#define		VOICE_WIFI_DISBIND_FAIL		BASE_NUM+42
#define		VOICE_ERROR_GUIHUA			BASE_NUM+43
#define		VOICE_SLEEP_START			BASE_NUM+44
#define		VOICE_ENTER_CEASE			BASE_NUM+45
#define		VOICE_PREEN_SET_OK			BASE_NUM+46


#define		VOICE_PAUSE_SWEEP			BASE_NUM+62
#define		VOICE_CHANGE_POSITION		BASE_NUM+63
#define		VOICE_ENTER_FACTORY			BASE_NUM+80
#define		VOICE_EXIT_FACTORY			BASE_NUM+81

#else
#define		BASE_NUM					0
#define  	VOICE_SWEEPER_INIT	 		BASE_NUM
#define		VOICE_SWEEPER_OK			BASE_NUM+1
#define 	VOICE_WIFI_OK				BASE_NUM+2
#define		VOICE_WIFI_FAIL				BASE_NUM+3
#define		VOICE_WIFI_FIRST_BIND		BASE_NUM+4
#define 	VOICE_WIFI_BIND_OK			BASE_NUM+5

#define		VOICE_WIFI_DISBIND_OK		BASE_NUM+6
#define		VOICE_SWEEP_START			BASE_NUM+7
#define		VOICE_SWEEP_STOP			BASE_NUM+8
#define 	VOICE_DOCK_START			BASE_NUM+9
#define 	VOICE_DOCK_STOP				BASE_NUM+10
#define 	VOICE_CHARGING_START		BASE_NUM+11
#define		VOICE_CHARGING_FULL			BASE_NUM+12
#define		VOICE_DOCK_FAIL				BASE_NUM+13
#define		VOICE_ERROR_DUSTBOX_NONE	BASE_NUM+14
#define		VOICE_BOX_FULL				BASE_NUM+15
#define		VOICE_I_AM_HERE				BASE_NUM+16
#define		VOICE_POWER_LOW				BASE_NUM+17
#define		VOICE_UPGRADE_START			BASE_NUM+18
#define		VOICE_UPGRADE_OK			BASE_NUM+19
#define		VOICE_UPGRADE_FAIL			BASE_NUM+20

#define		VOICE_ERROR_LIFT			BASE_NUM+21
#define		VOICE_WIFI_RST				BASE_NUM+22			//����WIFI 3��ʱ�䵽

#define		VOICE_KEY_PRESS				BASE_NUM+23
#define		VOICE_VOLUME_2				BASE_NUM+24
#define		VOICE_VOLUME_3				BASE_NUM+25
#define		VOICE_VOLUME_4				BASE_NUM+26

#define 	VOICE_VOLUME_TEST			BASE_NUM+27
#define		VOICE_SWEEPER_RST			BASE_NUM+28
#define		VOICE_BOX_EXIST				BASE_NUM+29
#define		VOICE_ERROR_DC_EXIST		BASE_NUM+30			//�ε������
#define		VOICE_STOP_CHARGE			BASE_NUM+31			//ֹͣ���
#define		VOICE_ERROR_BUMP_FIX		BASE_NUM+32			//ERROR_111
#define		VOICE_ERROR_L_LIFT			BASE_NUM+33
#define		VOICE_ERROR_R_LIFT			BASE_NUM+34
#define		VOICE_ERROR_L_RING			BASE_NUM+35
#define		VOICE_ERROR_R_RING			BASE_NUM+36
#define		VOICE_ERROR_L_SB			BASE_NUM+37
#define		VOICE_ERROR_R_SB			BASE_NUM+38
#define		VOICE_ERROR_ZS_OC			BASE_NUM+39
#define		VOICE_ERROR_SWEEP_FIX		BASE_NUM+40
#define		VOICE_WIFI_BIND_FAIL		BASE_NUM+41
#define		VOICE_WIFI_DISBIND_FAIL		BASE_NUM+42
#define		VOICE_ERROR_GUIHUA			BASE_NUM+43
#define		VOICE_SLEEP_START			BASE_NUM+44
#define		VOICE_ENTER_CEASE			BASE_NUM+45
#define		VOICE_PREEN_SET_OK			BASE_NUM+46
#define		VOICE_POWER_NONE			BASE_NUM+47
#define		VOICE_ERROR_GYRO			BASE_NUM+48
#define		VOICE_ERROR_L_WALL			BASE_NUM+49
#define		VOICE_ERROR_M_WALL			BASE_NUM+50
#define		VOICE_ERROR_R_WALL			BASE_NUM+51
#define		VOICE_ERROR_L_EARTH			BASE_NUM+52
#define		VOICE_ERROR_M_EARTH			BASE_NUM+53
#define		VOICE_ERROR_R_EARTH			BASE_NUM+54
#define		VOICE_ERROR_FAN_OC			BASE_NUM+55
#define		VOICE_ERROR_SLAM_TICK		BASE_NUM+56
#define		VOICE_ERROR_DIPAN_TICK		BASE_NUM+57
#define		VOICE_ERROR_BAT_FAIL		BASE_NUM+58
#define		VOICE_ERROR_VIDEO_FAIL		BASE_NUM+59
#define		VOICE_ERROR_BIRD_FAIL		BASE_NUM+60
#define		VOICE_ERROR_DANGER			BASE_NUM+61
#define		VOICE_PAUSE_SWEEP			BASE_NUM+62
#define		VOICE_CHANGE_POSITION		BASE_NUM+63
#define		VOICE_SWEEP_DONE			BASE_NUM+79
#define		VOICE_ENTER_FACTORY			BASE_NUM+80
#define		VOICE_EXIT_FACTORY			BASE_NUM+81
   #endif
#endif

#define		VOICE_LEVEL_1				0XE0
#define		VOICE_LEVEL_2				0XE1
#define		VOICE_LEVEL_3				0XE2
#define		VOICE_LEVEL_4				0XE3
#define		VOICE_LEVEL_5				0XE4
#define		VOICE_LEVEL_6				0XE5
#define		VOICE_LEVEL_7				0XE6
#define		VOICE_LEVEL_8				0XE7
#define		VOICE_LEVEL_9				0XE8
#define		VOICE_LEVEL_10				0XE9
#define		VOICE_LEVEL_11				0XEA
#define		VOICE_LEVEL_12				0XEB
#define		VOICE_LEVEL_13				0XEC
#define		VOICE_LEVEL_14				0XED
#define		VOICE_LEVEL_15				0XEE
#define		VOICE_LEVEL_16				0XEF


extern u8 voice_level;

void Send_Voice(u8 addr);
void delay_100us(u32 delay_time);
void Voice_Handle(void);
void voice_usart(u8 data);
u8 Init_Voice_Head(void);
void Init_Voice(void);
void Del_AllNode(void);
void Set_Voice_Level(u8 data);
void Write_Voice_Level(u8 data);
u8 Read_Voice_Level(void);
void Del_All_VoiceNode(void);

#endif



