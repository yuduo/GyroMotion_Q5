#ifndef  __CEASE_H
#define  __CEASE_H
////////////////////////头文件////////////////////////////////////	   
#include "stm32f10x.h"										
#include "soft.h"

////////////////////////全局变量//////////////////////////////////	 
///////////////////////全局函数////////////////////////////////////	   
void Do_Cease(void);    //执行在待机状态的程序
void Init_Cease(void);
void Cease_No_Key(void);
void Cease_YaoKong_Manage(void);
void Init_VirtualSleep(void);
void Do_VirtualSleep(void);

#endif
