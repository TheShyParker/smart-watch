#ifndef __BLOOD_H
#define __BLOOD_H
#include "stdio.h"

#include "stm32f4xx_hal.h"
typedef enum
{
	BLD_NORMAL,		//����
	BLD_ERROR,		//������
	
}BloodState;//ѪҺ״̬

typedef struct
{
	uint16_t 		heart;		//��������
	float 			SpO2;			//Ѫ������
	BloodState	state;		//״̬
	uint8_t   	update;		//��Ϣ���±�־λ
	uint8_t   	display;	//���ݸ��±�־λ
}BloodData;
//BloodData g_blooddata = {0};					//ѪҺ���ݴ洢
extern uint8_t heat_INT;
extern uint8_t TIM3_data;
BloodData ffheart(void);
void blood_Setup(void);
void blood_Loop(int sport_data,uint8_t sent_data)
;uint16_t ffrawStep(void);
void tft_draw_windows(void);
extern RTC_DateTypeDef sdatestructureget;
extern RTC_TimeTypeDef stimestructureget;
#endif /*__BLOOD_H*/
