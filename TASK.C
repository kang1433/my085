


#include "task.h"


 
Task_Struct head;
Task_Struct *p_head = &head;

/*0: 添加任务成功    1：失败*/
u8 task_add(u16 id,u16 NeedTime,void(*func)(void),u8 En) 
{
	Task_Struct *p_Task_Struct = (Task_Struct *)malloc(sizeof(Task_Struct));
	Task_Struct *p_temp = p_head;
	if (p_Task_Struct == NULL)
	{
		return 1;
	}
	else
	{
		memset(p_Task_Struct, 0x00, sizeof(Task_Struct));
	} 
	p_Task_Struct->RunTag = En;
	p_Task_Struct->Id = id;
	p_Task_Struct->TickTime = NeedTime;
	p_Task_Struct->CurTick = NeedTime;
	p_Task_Struct->func = func;
	p_Task_Struct->p_next = NULL;
	//链表指针移动到最后节点
	while (p_temp->p_next)
	{
		p_temp = p_temp->p_next;
	}
	p_temp->p_next = p_Task_Struct;	//链表最后加入了新节点

	return 0;
}
  

void task_run(void) 
{
	Task_Struct *p_temp = p_head;
	do
	{
		if (p_temp->RunTag == 1)//是否使能
		{
			if (p_temp->CurTick == 0)//时间到
			{
				p_temp->func();//执行节点对应任务
				p_temp->CurTick = p_temp->TickTime; /*func执行完后更新启动时间点 */     
			}
		}
		p_temp = p_temp->p_next;//移动到下一节点
	} while (p_temp);
}

 
void task_TickCut(void)
{
	Task_Struct *p_temp = p_head;
	do
	{
		if (p_temp->CurTick > 0)
		{
			p_temp->CurTick--;
		}
		p_temp = p_temp->p_next;
	} while (p_temp);
}




/*本算法取出单链表L（带头结点）中第i个位置的结点指针*/
Task_Struct *task_GetNode(u16 i)
{
	u16 j = 1;  				//计数，初始为1
	Task_Struct *p_temp = p_head;  //头结点指针赋给p
	if (i == 0)
	{
		return p_temp;  			//若i等于0，则返回头结点
	}
	if (i < 1)
	{
		return NULL;  			//若 i 无效，则返回 NULL
	}
	while((p_temp) && (j < i))
	{  
		p_temp = p_temp->p_next;//从第1个结点开始找，查找第i个结点
		j++;
	}
	return p_temp; 		//返回第i个结点的指针，如果i大于表长，p=NULL，直接返回p即可
}



//int find_char(char **strings, char value)
//{
//	char *string;
//	while((string = *strings++)!= NULL)
//	{
//		while(*string != '\0')
//		{
//			if(string++ == value)
//				return 1;
//		}
//	}
//	return 0;
//}

/*递归函数从后往前将数字打印成字符*/
//void binary_to_ascii(u32 value)
//{
//	u32 quotient;
//	quotient = value/10;
//	if(quotient != 0)
//		binary_to_ascii(quotient);
//	printf(value % 10 + '0');
//}

//float average(int32 n_values,...)
//{
//	va_list	var_arg;
//	int32 count;
//	float sum=0;
//	va_start(var_arg,n_values);
//	for(count = 0;count < n_values;count +=1)
//	{
//		sum += va_arg(var_arg,int32);
//	}
//	va_end(var_arg);
//	return sum/n_values;
//}


//**----end
