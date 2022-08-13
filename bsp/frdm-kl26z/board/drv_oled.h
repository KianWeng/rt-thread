#ifndef __OLED_H
#define __OLED_H

//初始化oled
void rt_hw_oled_init(void);
// 写入单个字符 x y ：开始坐标（第几列和第几页） p需要写入的字符 type 	写入的字符类型 才cn、asc
void oled_write_font(rt_uint8_t x, rt_uint8_t y, char *p, rt_uint8_t type);  			
// 写入字符串  参数和写入单个字符一样 写入字符长度
void oled_write_string(rt_uint8_t x, rt_uint8_t y, char *p, rt_uint8_t len);
// 写入单个ascii码 x y：坐标chr需要写入的ascii码
void oled_write_str(rt_uint8_t x, rt_uint8_t y, rt_uint8_t chr);
//清屏
void oled_clear_screen(void);

#endif
