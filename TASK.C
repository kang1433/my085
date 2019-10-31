


#include "task.h"


 
Task_Struct head;
Task_Struct *p_head = &head;

/*0: ��������ɹ�    1��ʧ��*/
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
	//����ָ���ƶ������ڵ�
	while (p_temp->p_next)
	{
		p_temp = p_temp->p_next;
	}
	p_temp->p_next = p_Task_Struct;	//�������������½ڵ�

	return 0;
}
  

void task_run(void) 
{
	Task_Struct *p_temp = p_head;
	do
	{
		if (p_temp->RunTag == 1)//�Ƿ�ʹ��
		{
			if (p_temp->CurTick == 0)//ʱ�䵽
			{
				p_temp->func();//ִ�нڵ��Ӧ����
				p_temp->CurTick = p_temp->TickTime; /*funcִ������������ʱ��� */     
			}
		}
		p_temp = p_temp->p_next;//�ƶ�����һ�ڵ�
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




/*���㷨ȡ��������L����ͷ��㣩�е�i��λ�õĽ��ָ��*/
Task_Struct *task_GetNode(u16 i)
{
	u16 j = 1;  				//��������ʼΪ1
	Task_Struct *p_temp = p_head;  //ͷ���ָ�븳��p
	if (i == 0)
	{
		return p_temp;  			//��i����0���򷵻�ͷ���
	}
	if (i < 1)
	{
		return NULL;  			//�� i ��Ч���򷵻� NULL
	}
	while((p_temp) && (j < i))
	{  
		p_temp = p_temp->p_next;//�ӵ�1����㿪ʼ�ң����ҵ�i�����
		j++;
	}
	return p_temp; 		//���ص�i������ָ�룬���i���ڱ�����p=NULL��ֱ�ӷ���p����
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

/*�ݹ麯���Ӻ���ǰ�����ִ�ӡ���ַ�*/
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