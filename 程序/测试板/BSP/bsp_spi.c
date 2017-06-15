#include "bsp_spi.h"

//SPI2引脚初始化
static void SPI2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SPI2_GPIO_CLK_ENABLE();
    //SCK,MISO,MOSI                          
	GPIO_InitStructure.GPIO_Pin = SPI2_CLK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);//初始化GPIOB

    //片选引脚
    GPIO_InitStructure.GPIO_Pin = SPI2_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(SPI2_GPIO,SPI2_CS_PIN);
}
//SPI2初始化
void SPI2_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI2_CLK_ENABLE();
	SPI2_GPIO_Init();
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设

}
//SPI2写一字节数据
void SPI2_WriteByte(uint8_t TxData)
{               
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
    SPI_I2S_ReceiveData(SPI2);      
}
//SPI2读一字节数据
uint8_t SPI2_ReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
	SPI_I2S_SendData(SPI2, 0xFF); //Write a dummy byte
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
    return (uint8_t)SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据                           
}
//SPI2片选使能
void SPI2_CS_Select(void)
{
    GPIO_ResetBits(SPI2_GPIO,SPI2_CS_PIN);
}
//SPI2片选失能
void SPI2_CS_Deselect(void)
{
    GPIO_SetBits(SPI2_GPIO,SPI2_CS_PIN);
}

/**
  * @brief  进入临界区
  * @retval None
  */
void SPI_CrisEnter(void)
{
	__set_PRIMASK(1);//关中断
}
/**
  * @brief  退出临界区
  * @retval None
  */
void SPI_CrisExit(void)
{
	__set_PRIMASK(0);//开中断
}



