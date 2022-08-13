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

/* дһ������ ��λ���� */
static void OLED_WR_Cmd(rt_uint8_t cmd)
{
	rt_uint8_t i, k;
	/* ����ƬѡCS��д��������DC */
	// OLED_CS(0); //CS�ܽ�һֱΪ��
	OLED_DC(0);
	
	for(i = 0; i < 8; i++)
	{
		/* ʱ���ߣ���������Ч */
		OLED_SCLK(0);
		k = cmd & (0x80);
		OLED_SDIN(k);
		OLED_SCLK(1);
		cmd <<= 1;
	}
	// OLED_CS(1);
	OLED_DC(1);	
}

/* дһ������ ��λ���� */
static void OLED_WR_Data(rt_uint8_t data)
{
	rt_uint8_t i, k;
	/* ����ƬѡCS��д��������DC */
	// OLED_CS(0);
	OLED_DC(1);
	
	for(i = 0; i < 8; i++)
	{
		/* ʱ���ߣ���������Ч */
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
 *	д�뵥���ַ� x y ����ʼ���꣨�ڼ��к͵ڼ�ҳ�� p��Ҫд����ַ� type 	д����ַ����� ��cn��asc
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
 *	д�뵥��ascii�� x y������chr��Ҫд���ascii��
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
 *	д���ַ���  ������д�뵥���ַ�һ�� д���ַ�����
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
 *	OLED ��ʼ������
 */
void rt_hw_oled_init(void)
{
	// OLED_CS(1);
	OLED_DC(1);

	/* ��λ */
	OLED_RES(0);
	rt_thread_delay(RT_TICK_PER_SECOND);
	OLED_RES(1);

	/* ��ʼд���ʼ������ */
	// OLED_WR_Cmd(0xAE);//�ر���ʾ
	// OLED_WR_Cmd(0xD5);//����ʱ�ӷ�Ƶ����
	// OLED_WR_Cmd(80);
	
	// OLED_WR_Cmd(0xA8);//��������·��
	// OLED_WR_Cmd(0x3F);//·��Ĭ��0x3F��1/64��
	
	// OLED_WR_Cmd(0xD3);//������ʾƫ��
	// OLED_WR_Cmd(0x00);//ƫ��Ĭ��Ϊ0
	
	// OLED_WR_Cmd(0x40);//������ʾ��ʼ��[5:0]
	
	// OLED_WR_Cmd(0x8D);//��ɱ�����
	// OLED_WR_Cmd(0x14);//bit2������/�ر�
	
	// OLED_WR_Cmd(0x20);//�����ڴ��ַģʽ
	// OLED_WR_Cmd(0x02);//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	
	// OLED_WR_Cmd(0xA1);//���ض�������,bit0:0,0->0;1,0->127;
	// OLED_WR_Cmd(0xC0);//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	// OLED_WR_Cmd(0xDA);//����COMӲ����������
	// OLED_WR_Cmd(0x12);//[5:4]����
	
	// OLED_WR_Cmd(0x81);//�Աȶ�����
	// OLED_WR_Cmd(0xEF);//Ĭ��0x7F����Χ1~255��Խ��Խ����
	
	// OLED_WR_Cmd(0xD9);//����Ԥ�������
	// OLED_WR_Cmd(0xF1);//[3:0],PHASE 1;[7:4],PHASE 2;
	
	// OLED_WR_Cmd(0xDB);//����VCOMH ��ѹ����
	// OLED_WR_Cmd(0x30);//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	
	// OLED_WR_Cmd(0xA4);//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	// OLED_WR_Cmd(0xA6);//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	
	// OLED_WR_Cmd(0xAF);//������ʾ

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
	
	/* �������� */
	oled_clear_screen();
}
