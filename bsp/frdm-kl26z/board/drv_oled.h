#ifndef __OLED_H
#define __OLED_H

//��ʼ��oled
void rt_hw_oled_init(void);
// д�뵥���ַ� x y ����ʼ���꣨�ڼ��к͵ڼ�ҳ�� p��Ҫд����ַ� type 	д����ַ����� ��cn��asc
void oled_write_font(rt_uint8_t x, rt_uint8_t y, char *p, rt_uint8_t type);  			
// д���ַ���  ������д�뵥���ַ�һ�� д���ַ�����
void oled_write_string(rt_uint8_t x, rt_uint8_t y, char *p, rt_uint8_t len);
// д�뵥��ascii�� x y������chr��Ҫд���ascii��
void oled_write_str(rt_uint8_t x, rt_uint8_t y, rt_uint8_t chr);
//����
void oled_clear_screen(void);

#endif
