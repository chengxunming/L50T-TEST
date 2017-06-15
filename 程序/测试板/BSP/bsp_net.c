#include "bsp_net.h"
#include "socket.h"	// Just include one header for WIZCHIP

/* Private define ------------------------------------------------------------*/
#define DATA_BUF_SIZE    10

uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};	

uint8_t gDATABUF[DATA_BUF_SIZE];	
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
                            .ip = {192, 168, 1, 123},
                            .sn = {255,255,255,0},
                            .gw = {192, 168, 1, 1},
                            .dns = {0,0,0,0},
                            .dhcp = NETINFO_STATIC };
////////////////////////////////////////////////LINK  W5500/////////////////////////////////////////////////////////////////////

//W5500��λ���ų�ʼ�� 
static void W5500_RST_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	W5500_RESET_GPIO_CLK_ENABLE();
                         
	GPIO_InitStructure.GPIO_Pin = W5500_RESET_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W5500_RESET_GPIO, &GPIO_InitStructure);	
	
	GPIO_SetBits(W5500_RESET_GPIO, W5500_RESET_PIN);//Ĭ�ϸߵ�ƽ���͵�ƽ��λ��Ч
}
//SPI�ص�����ע��
static void W5500_RegisterFunction(void)
{
	// ע��W5500��SPI�ӿں��� 
	reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);	//ע���ٽ�������
	
#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
	reg_wizchip_cs_cbfunc(SPI2_CS_Select, SPI2_CS_Deselect);//ע��SPIƬѡ�źź���
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
	reg_wizchip_cs_cbfunc(SPI2_CS_Select, SPI2_CS_Deselect); 
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
	
	reg_wizchip_spi_cbfunc(SPI2_ReadByte, SPI2_WriteByte);	//ע���д����	
}
//��ʼ��оƬ����
static void W5500_ParametersConfiguration(void)
{
	uint8_t link_state;
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

	//WIZCHIP SOCKET��������ʼ��
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
	{
		while(1);
	}
	//PHY���������״̬���
	do{
		 ctlwizchip(CW_GET_PHYLINK, (void*)&link_state);
	}while(link_state == PHY_LINK_OFF);
}
//��ʼ���������
static void W5500_NetworkConfiguration(void)
{
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
}

//W5500 Ӳ����
void W5500_Reset(void)
{
	GPIO_ResetBits(W5500_RESET_GPIO, W5500_RESET_PIN);                   //�͵�ƽ
	delay_ms(50);	
	GPIO_SetBits(W5500_RESET_GPIO, W5500_RESET_PIN);                     //�ߵ�ƽ 
}
	
//W5500����
void W5500_Config(void)
{
	SPI2_Init();//��ʼ��SPI�ӿ�
	W5500_RST_GPIO_Init();//��ʼ����λ����
	
	W5500_RegisterFunction();
	W5500_ParametersConfiguration();
	W5500_NetworkConfiguration();
}
//����UDP����
void W5500_UDP_SocketCreat(uint8_t sn, uint16_t port)
{
	while(getSn_SR(sn)==SOCK_CLOSED)
	{
		socket(sn, Sn_MR_UDP, port, 0x00);
	}
}
//UDP�������ݻػ�����
int32_t W5500_UDP_LoopBack(uint8_t sn, uint16_t port)
{
	int32_t  ret;
	uint16_t size, sentsize;
	uint8_t  destip[4];
	uint16_t destport;
	uint8_t* buf;

	switch(getSn_SR(sn))//socket״̬
	{
		case SOCK_UDP :
			if((size = getSn_RX_RSR(sn)) > 0)
			{
				if(size > DATA_BUF_SIZE)size = DATA_BUF_SIZE;
				buf=(uint8_t*)malloc(size);//�����ڴ棬��Ž�������
				if(buf==NULL) return -1;
				ret = recvfrom(sn, buf, size, destip, (uint16_t*)&destport);
				if(ret <= 0)
				{
					return ret;
				}
				size = (uint16_t) ret;
				sentsize = 0;
				while(sentsize != size)
				{
					ret = sendto(sn, buf+sentsize, size-sentsize, destip, destport);
					if(ret < 0)
					{
						return ret;
					}
					sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
				}
				free(buf);//������ɣ��ͷ��ڴ�
			}
			break;
		case SOCK_CLOSED:
			if((ret = socket(sn, Sn_MR_UDP, port, 0x00)) != sn)//����UDP����
			{
				return ret;
			}
			break;
		default :
			break;
	}
	return 1;	
}

