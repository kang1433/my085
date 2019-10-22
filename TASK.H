#ifndef _TASK_H_
#define _TASK_H_
#include "include.h"

typedef struct Task_Struct
{
	u8 RunTag;		//表示任务是否在运行
	u16 Id;
	u16 TickTime;
	volatile u16 CurTick;
	void(*func) (void);			//任务的函数指针
	struct Task_Struct * p_next;
}Task_Struct;

extern Task_Struct head;
extern Task_Struct *p_head;



u8 task_add(u16 id,u16 NeedTime,void(*func)(void),u8 En);
void task_run(void) ;
void task_TickCut(void);
Task_Struct *task_GetNode(u16 i);




#endif
