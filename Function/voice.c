#include "AAA-include.h"
#include "stdlib.h"

#define	VOICELIST_START 		1
#define VOICE_LEN sizeof(struct node)

#ifdef VOICE_LIST
u32 voice_time=0;
u8 voice_level;
bool voice_ok=true;

struct node
{
	u8 data;
	struct node* next;
};

struct node *voice_head;

u8 Init_Voice_Head(void)
{	
	if(voice_head!=NULL)
		{
#ifdef DEBUG_VOICE
			TRACE("voice head is exist!!\r\n");
#endif
			if(voice_head->next!=NULL)
				{
					Del_All_VoiceNode();
				}
			return 0;
		}
	voice_head=(struct node*)malloc(VOICE_LEN);
	if(voice_head==NULL)
		{
#ifdef DEBUG_VOICE
			TRACE("voice head malloc fail!!\r\n");
#endif
			return 1;
		}
#ifdef DEBUG_VOICE
	TRACE("voice head init ok!!\r\n");
#endif
	voice_head->next=NULL;
	return 0;
}


void Del_Node(void)
{
	if(voice_head->next==NULL)
		return;
#ifndef VOICELIST_START		//最新的语音在List最后，所以先删除第一个语音信息（voice_head后面）
	struct node *p=voice_head;
	p=p->next;
	voice_head->next=p->next;
	free(p);
#else						//最新的语音在list最前，所以先删除最后一个语音信息。
	struct node *p=voice_head,*q;
	while(p->next!=NULL)
		{
			q=p;
			p=p->next;
		}
	q->next=NULL;
	free(p);
	
#endif
}

void Del_All_VoiceNode(void)
{
	if(voice_head->next==NULL)
		return;
	struct node *p,*q;
	while(voice_head->next!=NULL)
		{
			p=voice_head;
			while(p->next!=NULL)
				{
					q=p;
					p=p->next;
				}
			free(p);
			q->next=NULL;
		}
#ifdef DEBUG_VOICE
	TRACE("Del all voicenode success!!!\r\n");	
#endif
}

#ifdef ONE_LINE_VOICE
u8 Voice_Driver(u8 data)
{
	u8 temp_addr=data;
	u8 i=0;
	V_CLK_0;
//	delay_100us(50);
	if(giv_sys_time-voice_time<50)		//delay 5ms
		return 0;
	for(i=0;i<8;i++)
		{
			if(temp_addr&0x01)
				{
					V_CLK_1;
					delay_100us(6);
					V_CLK_0;
					delay_100us(2);
				}
			else
				{
					V_CLK_1;
					delay_100us(2);
					V_CLK_0;
					delay_100us(6);
				}
			temp_addr>>=1;
		}
	V_CLK_1;
	return 1;
}
#endif

u8 Voice_Driver(u8 data)
{
	u8 temp_addr=data;
#ifdef  NEW_VOICE_IC
	if (temp_addr >	13)  
		return  1;
		// temp_addr = 9; //测试用 
		V_CLK_1;					//拉低时钟
		if(giv_sys_time-voice_time<50)
			return 0;
		V_CLK_0;			
		for(int i=0;i<temp_addr;i++)
			{
				delay_100us(3);
				V_DAT_1;
				delay_100us(3);
				V_DAT_0;	
			}
	return 1;							 
#else
	V_DAT_1;
	V_CLK_0;					//拉低时钟
	if(giv_sys_time-voice_time<50)
		return 0;
 	for(int i=0;i<8;i++)
		{
			V_CLK_0;			//拉低时钟

			if(temp_addr&0x01)	//准备数据
				V_DAT_1;
			else
				V_DAT_0;
			temp_addr>>=1;

			delay_100us(3);		//延时300us
			V_CLK_1;			//拉高时钟
			delay_100us(3);		//延时300us
				
		}

//	V_CLK_0;
//	delay_100us(3);
	V_CLK_1;
	V_DAT_1;
	return 1;
#endif	
}

void Voice_Handle(void)
{
	static u8 step=0;
	if(voice_head->next==NULL)
		return;
	struct node *p;
#if 0
	p=voice_head->next;
#else
	p=voice_head;
	while(p->next!=NULL)
		p=p->next;
#endif


	switch (step)
		{
		case 0:
			voice_time=giv_sys_time;
			step++;
#ifdef USE_VOICE_BUSY
			step=2;
#endif
		if((p->data==VOICE_KEY_PRESS)|(p->data==VOICE_CHARGING_START)|(p->data==VOICE_STOP_CHARGE)|(p->data==VOICE_VOLUME_TEST))			//按键语音,打断当前语音,即刻发送
			step=3;
		if((p->data==VOICE_VOLUME_TEST))
			step=3;
		break;
		case 1:
			if(Voice_Driver(0xF3))				//发送0xF3
				{
					step++;
					voice_time=giv_sys_time;
				}
		break;
		case 2:
			if(giv_sys_time-voice_time>20)		//延时2ms
				{
#ifdef USE_VOICE_BUSY
					if(!ISVOICEBUSY)
						return;
#endif
					step++;
					voice_time=giv_sys_time;
				}
		break;
		case 3:
			if(Voice_Driver(p->data))			//发送语音地址
				{
					step++;
					voice_time=giv_sys_time;
				}
		break;
		case 4:
			if(giv_sys_time-voice_time>40)		//延时4ms
				{
					step=8;
					voice_time=giv_sys_time;
				}
		break;
#if 1
		case 5:
			if(Voice_Driver(0xF8))				//发送0xF3
				{
					step++;
					voice_time=giv_sys_time;
				}
		break;
		case 6:
			if(giv_sys_time-voice_time>20)		//延时2ms
				{
					step++;
					voice_time=giv_sys_time;
				}
		break;
		case 7:
			if(Voice_Driver(100))				//静音1000ms
				{
					step++;
					voice_time=giv_sys_time;					
				}
		break;
#endif
		case 8:
#ifdef USE_VOICE_BUSY
			Del_Node();
			voice_time=giv_sys_time;
			step=0;
#else
			if((p->data==VOICE_KEY_PRESS)|(p->data==VOICE_STOP_CHARGE)|(p->data==VOICE_CHARGING_START)\
				|(p->data==VOICE_VOLUME_TEST))		//按键语音,即刻删除返回,无需延时
				{
					Del_Node();					
					voice_time=giv_sys_time;
					step=0;
				}
			else
				{								//非语音按键,删除地址并延时
					Del_Node();
					step++;
				}
#endif
		break;
		case 9:
			if(giv_sys_time-voice_time>15000)		//10ms
				{
					step=0;
					voice_time=giv_sys_time;
				}
			break;
		}
		return;
}

void Init_Voice(void)
{
	if(Init_Voice_Head())
		{
			voice_ok=false;
			return;
		}
	voice_ok=true;
	voice_level=Read_Voice_Level();
#ifdef DEBUG_INIT
	TRACE("voice_level=0x%x\r\n",voice_level);
#endif
	if((voice_level<VOICE_LEVEL_1)|(voice_level>VOICE_LEVEL_16))
		{
			voice_level=VOICE_LEVEL_13;
#ifdef DEBUG_INIT
	TRACE("voice_level =%d is not good,rewrite!\r\n",voice_level);
#endif
		}
	Set_Voice_Level(voice_level);
#ifdef DEBUG_INIT
	TRACE("Voice init OK!\r\n");
#endif
}

void Set_Voice_Level(u8 data)
{
	voice_time=giv_sys_time;
	while(!Voice_Driver(data));
	Write_Voice_Level(data);
}

void Write_Voice_Level(u8 data)
{
#ifndef DS1307
	PWR_BackupAccessCmd(ENABLE);
	BKP_WriteBackupRegister(VOICE_BACKUP_BKP, (u16)(data));
	PWR_BackupAccessCmd(DISABLE);
#else
	if(DS1307_Writeu8data(VALUE_BACKUP_ADDR, data))
		{
#ifdef DEBUG_Enter_Mode
			TRACE("Write DS1307 fail!\r\n");
#endif
		}
#endif
}

u8 Read_Voice_Level(void)
{
	u8 data=0;
#ifndef DS1307
	data=BKP_ReadBackupRegister(VOICE_BACKUP_BKP);
#else

	if(DS1307_Readu8data(VALUE_BACKUP_ADDR, &data))
		{
#ifdef DEBUG_Enter_Mode
			TRACE("Write DS1307 fail!\r\n");
#endif
		}
#endif
	return data;
}
#endif

void delay_100us(u32 delay_time)
{
	u32 time=giv_sys_time;
	while(giv_sys_time-time<delay_time);
}
void Send_Voice(u8 data)
{
#ifdef NEW_VOICE_IC
	if(data>13)
		return;
#endif

#ifdef VOICE_LIST						//使用语音LIST发出语音
	if(!voice_ok)
		return;
#ifndef VOICELIST_START		//将新添加的语音放在语音list的最后
	struct node *p,*q;
	p=(struct node*)malloc(VOICE_LEN);
	
	if(p==NULL)
		return;
	
	q=voice_head;
	while(q->next!=NULL)
		q=q->next;
	q->next=p;
	p->data=data;
	p->next=NULL;
#else						//将新添加的语音放在语音list的最前（voice_head后面）
	struct node *p=NULL;
	p=(struct node*)malloc(VOICE_LEN);

	if(p==NULL)
		return;

	p->next=voice_head->next;
	voice_head->next=p;
	p->data=data;
#endif
#else						//不使用LIST，直接发语音
#ifdef DEBUG_VOICE
	TRACE("vd=%d\r\n",data);
#endif
	V_CLK_1;
	delay_us(100);
	V_CLK_0;
	delay_us(100);
	for(int i=0;i<data;i++)
		{
			delay_100us(1);
			V_DAT_1;
			delay_100us(1);
			V_DAT_0;	
		}
	V_CLK_0;
	V_DAT_0;
	delay_us(100);
#endif
}


