#include "sys.h"

//中断优先级组配置
void NVIC_GroupingConfiguration(void)
{

    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_7);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}
