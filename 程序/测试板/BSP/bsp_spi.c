#include "bsp_spi.h"

//SPI2���ų�ʼ��
static void SPI2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SPI2_GPIO_CLK_ENABLE();
    //SCK,MISO,MOSI                          
	GPIO_InitStructure.GPIO_Pin = SPI2_CLK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);//��ʼ��GPIOB

    //Ƭѡ����
    GPIO_InitStructure.GPIO_Pin = SPI2_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(SPI2_GPIO,SPI2_CS_PIN);
}
//SPI2��ʼ��
void SPI2_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI2_CLK_ENABLE();
	SPI2_GPIO_Init();
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

}
//SPI2дһ�ֽ�����
void SPI2_WriteByte(uint8_t TxData)
{               
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    SPI_I2S_ReceiveData(SPI2);      
}
//SPI2��һ�ֽ�����
uint8_t SPI2_ReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	SPI_I2S_SendData(SPI2, 0xFF); //Write a dummy byte
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    return (uint8_t)SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����                           
}
//SPI2Ƭѡʹ��
void SPI2_CS_Select(void)
{
    GPIO_ResetBits(SPI2_GPIO,SPI2_CS_PIN);
}
//SPI2Ƭѡʧ��
void SPI2_CS_Deselect(void)
{
    GPIO_SetBits(SPI2_GPIO,SPI2_CS_PIN);
}

/**
  * @brief  �����ٽ���
  * @retval None
  */
void SPI_CrisEnter(void)
{
	__set_PRIMASK(1);//���ж�
}
/**
  * @brief  �˳��ٽ���
  * @retval None
  */
void SPI_CrisExit(void)
{
	__set_PRIMASK(0);//���ж�
}



