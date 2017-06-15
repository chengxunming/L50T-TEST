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

//W5500复位引脚初始化 
static void W5500_RST_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	W5500_RESET_GPIO_CLK_ENABLE();
                         
	GPIO_InitStructure.GPIO_Pin = W5500_RESET_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W5500_RESET_GPIO, &GPIO_InitStructure);	
	
	GPIO_SetBits(W5500_RESET_GPIO, W5500_RESET_PIN);//默认高电平，低电平复位有效
}
//SPI回调函数注册
static void W5500_RegisterFunction(void)
{
	// 注册W5500的SPI接口函数 
	reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);	//注册临界区函数
	
#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
	reg_wizchip_cs_cbfunc(SPI2_CS_Select, SPI2_CS_Deselect);//注册SPI片选信号函数
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
	reg_wizchip_cs_cbfunc(SPI2_CS_Select, SPI2_CS_Deselect); 
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
	
	reg_wizchip_spi_cbfunc(SPI2_ReadByte, SPI2_WriteByte);	//注册读写函数	
}
//初始化芯片参数
static void W5500_ParametersConfiguration(void)
{
	uint8_t link_state;
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

	//WIZCHIP SOCKET缓存区初始化
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
	{
		while(1);
	}
	//PHY物理层连接状态检查
	do{
		 ctlwizchip(CW_GET_PHYLINK, (void*)&link_state);
	}while(link_state == PHY_LINK_OFF);
}
//初始化网络参数
static void W5500_NetworkConfiguration(void)
{
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
}

//W5500 硬重启
void W5500_Reset(void)
{
	GPIO_ResetBits(W5500_RESET_GPIO, W5500_RESET_PIN);                   //低电平
	delay_ms(50);	
	GPIO_SetBits(W5500_RESET_GPIO, W5500_RESET_PIN);                     //高电平 
}
	
//W5500配置
void W5500_Config(void)
{
	SPI2_Init();//初始化SPI接口
	W5500_RST_GPIO_Init();//初始化复位引脚
	
	W5500_RegisterFunction();
	W5500_ParametersConfiguration();
	W5500_NetworkConfiguration();
}
//创建UDP连接
void W5500_UDP_SocketCreat(uint8_t sn, uint16_t port)
{
	while(getSn_SR(sn)==SOCK_CLOSED)
	{
		socket(sn, Sn_MR_UDP, port, 0x00);
	}
}
//UDP连接数据回环测试
int32_t W5500_UDP_LoopBack(uint8_t sn, uint16_t port)
{
	int32_t  ret;
	uint16_t size, sentsize;
	uint8_t  destip[4];
	uint16_t destport;
	uint8_t* buf;

	switch(getSn_SR(sn))//socket状态
	{
		case SOCK_UDP :
			if((size = getSn_RX_RSR(sn)) > 0)
			{
				if(size > DATA_BUF_SIZE)size = DATA_BUF_SIZE;
				buf=(uint8_t*)malloc(size);//分配内存，存放接收数据
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
				free(buf);//处理完成，释放内存
			}
			break;
		case SOCK_CLOSED:
			if((ret = socket(sn, Sn_MR_UDP, port, 0x00)) != sn)//创建UDP连接
			{
				return ret;
			}
			break;
		default :
			break;
	}
	return 1;	
}

