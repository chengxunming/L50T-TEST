#include "sys.h"

//�ж����ȼ�������
void NVIC_GroupingConfiguration(void)
{

    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_7);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}
