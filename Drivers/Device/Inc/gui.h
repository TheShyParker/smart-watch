#ifndef __GUI_H
#define __GUI_H

#include "stm32f4xx_hal.h"

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);//ָ�����������ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//��ָ��λ�û�һ����
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//��ָ��λ�û�һ����
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//��ָ��λ�û�һ������
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//��ָ��λ�û�һ��Բ
uint8_t get_number_c(uint16_t numeber,uint8_t w);

uint32_t mypow(uint8_t m,uint8_t n);//����
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��������
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��λС������

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,uint8_t p);//��ʾͼƬ

void Show_top(uint16_t x,uint16_t y,uint16_t length,uint16_t width,uint16_t back,uint16_t color,uint8_t p);
void Show_roll(uint16_t x,uint16_t y,uint16_t length,uint16_t width,uint16_t ud,uint16_t up_down) ;
void Word48(uint16_t x,uint16_t y,uint16_t color,uint8_t p);
void Word23(uint16_t x,uint16_t y,uint16_t color,uint16_t p);
void Refresh(uint8_t p) ;
void Word23_sig(uint16_t x,uint16_t y,uint16_t color,uint16_t p);
uint8_t get_number_c(uint16_t numeber,uint8_t w);
void LCD_ShowPicture_action_foot(uint16_t x,uint16_t y,uint16_t length,uint16_t width,uint8_t p);
void LCD_ShowPicture_action_cal(uint16_t x,uint16_t y,uint16_t length,uint16_t width,uint8_t p);
void Cal_page(void);
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	
#define ORG         	 0xE2DC	  

#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#endif






