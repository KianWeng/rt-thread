#include <rtthread.h>
#include <MKL26Z4.h>

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "drv_oled_font.h"

static void OLED_SCLK(rt_uint8_t x)
{
	if(x == 1)
		GPIO_WritePinOutput (BOARD_LCD_D0_GPIO, BOARD_LCD_D0_PIN, 1);
	else
		GPIO_WritePinOutput (BOARD_LCD_D0_GPIO, BOARD_LCD_D0_PIN, 0);
}

static void OLED_SDIN(rt_uint8_t x)
{
	if(x == 0)
		GPIO_WritePinOutput (BOARD_LCD_D1_GPIO, BOARD_LCD_D1_PIN, 0);
	else
		GPIO_WritePinOutput (BOARD_LCD_D1_GPIO, BOARD_LCD_D1_PIN, 1);
}

static void OLED_RES(rt_uint8_t x)
{
	if(x == 1)
		GPIO_WritePinOutput (BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 1);
	else
		GPIO_WritePinOutput (BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 0);
}

static void OLED_DC(rt_uint8_t x)
{
	if(x == 1)
		GPIO_WritePinOutput (BOARD_LCD_DC_GPIO, BOARD_LCD_DC_PIN, 1);
	else
		GPIO_WritePinOutput (BOARD_LCD_DC_GPIO, BOARD_LCD_DC_PIN, 0);
}

// void OLED_CS( uint8_t x )
// {
// 	if( x==1 )
// 		GPIO_SetBits( GPIOD, GPIO_Pin_6 );
// 	else
// 		GPIO_ResetBits( GPIOD, GPIO_Pin_6 );
// }

/* 写一个命令 高位先行 */
static void OLED_WR_Cmd(rt_uint8_t cmd)
{
	rt_uint8_t i, k;
	/* 拉低片选CS，写命令拉低DC */
	// OLED_CS(0); //CS管脚一直为低
	OLED_DC(0);
	
	for(i = 0; i < 8; i++)
	{
		/* 时钟线，上升沿有效 */
		OLED_SCLK(0);
		k = cmd & (0x80);
		OLED_SDIN(k);
		OLED_SCLK(1);
		cmd <<= 1;
	}
	// OLED_CS(1);
	OLED_DC(1);	
}

/* 写一个数据 高位先行 */
static void OLED_WR_Data(rt_uint8_t data)
{
	rt_uint8_t i, k;
	/* 拉低片选CS，写数据拉高DC */
	// OLED_CS(0);
	OLED_DC(1);
	
	for(i = 0; i < 8; i++)
	{
		/* 时钟线，上升沿有效 */
		OLED_SCLK(0);
		k = data & (0x80);
		OLED_SDIN(k);
		OLED_SCLK(1);
		data <<= 1;
	}
	// OLED_CS(1);
	OLED_DC(1);
}

void oled_clear_screen(void)
{
	rt_uint32_t i;
	
	OLED_WR_Cmd(0x20);
	OLED_WR_Cmd(0x00);
	OLED_WR_Cmd(0x21);
	OLED_WR_Cmd(0x00);
	OLED_WR_Cmd(0xFF);
	OLED_WR_Cmd(0x22);
	OLED_WR_Cmd(0x00);
	OLED_WR_Cmd(0x07);
	
	for(i=0; i < 1024; i++)
	{
		OLED_WR_Data(0x00);	
	}
}

/*
 *	写入单个字符 x y ：开始坐标（第几列和第几页） p需要写入的字符 type 	写入的字符类型 才cn、asc
 */
void oled_write_font(rt_uint8_t x, rt_uint8_t y, char *p, rt_uint8_t type)
{
	rt_uint8_t bulk, i;  

	if(type) 
		bulk = 0x0f;
	else 
		bulk = 0x07;

	OLED_WR_Cmd(0x20);
	OLED_WR_Cmd(0x00);
	OLED_WR_Cmd(0x21);
	OLED_WR_Cmd(x);
	OLED_WR_Cmd(x + bulk);
	OLED_WR_Cmd(0x22);
	OLED_WR_Cmd(y);
	OLED_WR_Cmd(y + 0x01);
	
	for(i = 0; i < (2 * bulk + 1); i++)
	{
		OLED_WR_Data(p[i]);
	}
}


/*
 *	写入单个ascii码 x y：坐标chr需要写入的ascii码
 */
void oled_write_str(rt_uint8_t x, rt_uint8_t y, rt_uint8_t chr)
{
	rt_uint8_t i;

	chr = chr - ' ';
	OLED_WR_Cmd(0x20);
	OLED_WR_Cmd(0x00);
	OLED_WR_Cmd(0x21);
	OLED_WR_Cmd(x);
	OLED_WR_Cmd(x+0x07);
	OLED_WR_Cmd(0x22);
	OLED_WR_Cmd(y);
	OLED_WR_Cmd(y+0x01);
	
	for(i = 0; i < 16; i++)
	{
		OLED_WR_Data(asc2_1608[chr][i]);		
	}
	
}


/*
 *	写入字符串  参数和写入单个字符一样 写入字符长度
 */
void oled_write_string(rt_uint8_t x, rt_uint8_t y, char *p, rt_uint8_t len)
{
	
	rt_uint8_t str_x = x, i;

	for(i = 0; i < len; i++)
	{
		rt_uint8_t place = p[i];
		oled_write_str(str_x, y, place);
		str_x = str_x + 0x08;
	}
	
}

/*
 *	OLED 初始化工作
 */
void rt_hw_oled_init(void)
{
	// OLED_CS(1);
	OLED_DC(1);

	/* 复位 */
	OLED_RES(0);
	rt_thread_delay(RT_TICK_PER_SECOND);
	OLED_RES(1);

	/* 开始写入初始化命令 */
	// OLED_WR_Cmd(0xAE);//关闭显示
	// OLED_WR_Cmd(0xD5);//设置时钟分频因子
	// OLED_WR_Cmd(80);
	
	// OLED_WR_Cmd(0xA8);//设置驱动路数
	// OLED_WR_Cmd(0x3F);//路数默认0x3F（1/64）
	
	// OLED_WR_Cmd(0xD3);//设置显示偏移
	// OLED_WR_Cmd(0x00);//偏移默认为0
	
	// OLED_WR_Cmd(0x40);//设置显示开始行[5:0]
	
	// OLED_WR_Cmd(0x8D);//电荷泵设置
	// OLED_WR_Cmd(0x14);//bit2，开启/关闭
	
	// OLED_WR_Cmd(0x20);//设置内存地址模式
	// OLED_WR_Cmd(0x02);//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	
	// OLED_WR_Cmd(0xA1);//段重定义设置,bit0:0,0->0;1,0->127;
	// OLED_WR_Cmd(0xC0);//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	// OLED_WR_Cmd(0xDA);//设置COM硬件引脚配置
	// OLED_WR_Cmd(0x12);//[5:4]配置
	
	// OLED_WR_Cmd(0x81);//对比度设置
	// OLED_WR_Cmd(0xEF);//默认0x7F（范围1~255，越大越亮）
	
	// OLED_WR_Cmd(0xD9);//设置预充电周期
	// OLED_WR_Cmd(0xF1);//[3:0],PHASE 1;[7:4],PHASE 2;
	
	// OLED_WR_Cmd(0xDB);//设置VCOMH 电压倍率
	// OLED_WR_Cmd(0x30);//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	
	// OLED_WR_Cmd(0xA4);//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	// OLED_WR_Cmd(0xA6);//设置显示方式;bit0:1,反相显示;0,正常显示
	
	// OLED_WR_Cmd(0xAF);//开启显示

	OLED_WR_Cmd(0xAE);//--turn off oled panel
	OLED_WR_Cmd(0x00);//---set low column address
	OLED_WR_Cmd(0x10);//---set high column address
	OLED_WR_Cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Cmd(0x81);//--set contrast control register
	OLED_WR_Cmd(0xCF);// Set SEG Output Current Brightness
	OLED_WR_Cmd(0xA1);//--Set SEG/Column Mapping     0xa0???? 0xa1??
	OLED_WR_Cmd(0xC8);//Set COM/Row Scan Direction   0xc0???? 0xc8??
	OLED_WR_Cmd(0xA6);//--set normal display
	OLED_WR_Cmd(0xA8);//--set multiplex ratio(1 to 64)
	OLED_WR_Cmd(0x3f);//--1/64 duty
	OLED_WR_Cmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Cmd(0x00);//-not offset
	OLED_WR_Cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Cmd(0xD9);//--set pre-charge period
	OLED_WR_Cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Cmd(0xDA);//--set com pins hardware configuration
	OLED_WR_Cmd(0x12);
	OLED_WR_Cmd(0xDB);//--set vcomh
	OLED_WR_Cmd(0x40);//Set VCOM Deselect Level
	OLED_WR_Cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Cmd(0x00);//
	OLED_WR_Cmd(0x8D);//--set Charge Pump enable/disable
	OLED_WR_Cmd(0x14);//--set(0x10) disable
	OLED_WR_Cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Cmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Cmd(0xAF);
	OLED_WR_Cmd(0x21);	
	OLED_WR_Cmd(0x00);
	OLED_WR_Cmd(0xFF);
	OLED_WR_Cmd(0x22);	
	OLED_WR_Cmd(0x00);
	OLED_WR_Cmd(0x07);
	
	/* 清屏函数 */
	oled_clear_screen();
}
