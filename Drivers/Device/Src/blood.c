/**
  ******************************************************************************
  * @file    blood.c
  * @author  Deson
  * @version V1.0.0
  * @date    2019-12-28
  * @brief   Ѫ��ͼʾ�������򲿷�
  ******************************************************************************
  */
/*--Include-start-------------------------------------------------------------*/
#include "blood.h"
#include "MAX30100.h"
#include "algorithm.h"
#include "math.h"
#include "gui.h"
#include "ST7735.h"
#include "bmi160.h"
#include "bsp_usartx_CC2541.h"

uint16_t rawStep[1]= {0};

//rawStep[0]=0x00;
uint16_t setnumber2;

uint16_t setnumber;
uint8_t set_add_number;
uint8_t backup_number=0;

uint16_t backup_heart[100][3];
uint8_t backup_time[100][3];

//struct
//{
//	float 	Hp	;			//Ѫ�쵰��
//	float 	HpO2;			//����Ѫ�쵰��
//
//}g_BloodWave;//ѪҺ��������

BloodData g_blooddata = {0};					//ѪҺ���ݴ洢

/*Global data space ----------------------------------------------------------*/
uint16_t g_fft_index = 0;         	 	//fft��������±�

//struct compx s1[FFT_N+16];           	//FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����
struct compx s2[FFT_N];           	//FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����

struct
{
    float 	Hp	;			//Ѫ�쵰��
    float 	HpO2;			//����Ѫ�쵰��

} g_BloodWave; //ѪҺ��������


/*funcation start ------------------------------------------------------------*/


/*Sensor func -----------------------------------------------------------------*/
//����ѪҺ����
BloodData ffheart(void) {
    return g_blooddata;
}
uint16_t ffrawStep(void) {
    return rawStep[0];
}
//ѪҺ�����Ϣ����
void delay2_ms(uint16_t time)

{

    uint16_t i=0;

    while(time--)

    {

        i=12000;  //�Լ�����

        while(i--) ;

    }

}
void blood_data_update(void)
{

    uint16_t temp_num=0;
    uint16_t fifo_word_buff[1][2];
    __disable_irq();
    temp_num = max30100_Bus_Read(INTERRUPT_REG,max30100_WR_address);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
    __enable_irq();
    //��־λ��ʹ��ʱ ��ȡFIFO
//	printf("g_fft_index%d",g_fft_index);
//				printf("%d",temp_num);

    if (0x40&temp_num)

    {
        __disable_irq();
        max30100_FIFO_Read(0x07,fifo_word_buff,1); //read the hr and spo2 data form fifo in reg=0x05
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
        __enable_irq();
        //������д��fft���벢������


        if(g_fft_index < FFT_N+1)
        {

            s2[g_fft_index].real = fifo_word_buff[0][1];
            s2[g_fft_index].imag= 0;
            g_fft_index++;
        }
        //��Ϣ���±�־λ
        g_blooddata.update++;
    }

}
//ѪҺ��Ϣת��
void blood_data_translate(void)
{
    //������д�����
    if(g_fft_index>=FFT_N)
    {


        g_fft_index = 0;

        //���ݸ��±�־λ
        g_blooddata.display = 1;

        //���ٸ���Ҷ�任
//		FFT(s1);
        FFT(s2);

        //��ƽ��
        for(int i = 0; i < FFT_N; i++)
        {
            s2[i].real=sqrtf(s2[i].real*s2[i].real+s2[i].imag*s2[i].imag);
        }


        uint16_t s2_max_index = find_max_num_index(s2, 60);

        //���ʼ���
        g_blooddata.heart = s2_max_index*2.92;

        backup_heart[backup_number][0]=g_blooddata.heart;
        bmi160_getStep(rawStep);
        backup_heart[backup_number][1]=rawStep[0];
        backup_time[backup_number][0]=sdatestructureget.Date;
        backup_time[backup_number][1]=stimestructureget.Hours;
        backup_time[backup_number][2]=stimestructureget.Minutes;
				backup_number++;
        if(backup_number==99) {
            backup_number=0;
        }


        //״̬����
        g_blooddata.state = BLD_NORMAL;
//		}


    }
}

//ѪҺ���ݸ���
void blood_wave_update(void)
{
    static DC_FilterData hbodc = {.w = 0,.init = 0,.a = 0};

    float hboag = 0;

    float hboag_d = 0;

    //ǰ8����ƽ��ֵ
    for(int i = 0; i < 8; i++)
    {
        hboag += s2[g_fft_index - 8 + i].real;
    }

    //ֱ���˲�
    hboag_d = dc_filter(hboag,&hbodc) / 8;

    //�߶�����
    float hbohight = 0;

    hbohight  = (hboag_d / 7.5)+5;

    hbohight = (hbohight > 27) ? 27 : hbohight;
    hbohight = (hbohight < 0) ?  0 : hbohight;

    //�����ݷ�����ȫ��
    g_BloodWave.HpO2 = hbohight;
    //ѪҺ״̬���



}
void get_acc(uint8_t * str_acc,uint16_t number,uint16_t number2) {
    uint8_t mid;
    uint16_t kk=10000;

    for(uint8_t i=0; i<5; i++)
    {

        mid=number / (kk) % 10;
        kk=kk/10;
        mid=mid+48;
        str_acc[i]=mid;


    }

    str_acc[5]=0x2c;
    kk=10000;

    for(uint8_t j=1; j<6; j++) {
        mid=number2 / (kk) % 10;
        kk=kk/10;
        mid=mid+48;
        str_acc[5+j]=mid;
    }
    return ;



}
void tft_draw_wave_line(uint8_t p,uint8_t n,uint16_t h0,uint16_t h1,uint16_t color)
{
    uint16_t x = 1;										// x ��ʼ����
    uint16_t y = (p == 0) ? 122 : 89; // y ��ʼ����

    //������
    if((n + 1) < 132)
    {
        LCD_DrawLine((x + n + 2),y - 27,(x + n + 2),y + 1,GRED);
        LCD_DrawLine((x + n + 1),y - 27,(x + n + 1),y + 1,BLACK);
    }
    //������
    LCD_DrawLine((x + n),(y - h0),(x + n + 1),(y - h1),GRED);

}

//���Ʋ���ͼ��
void tft_draw_wave(void)
{
    static uint16_t hist_n  = 0;	//ͬ����ʷ���

//	static uint16_t hbhist_h  = 0;//Ѫ�쵰����ʷ��ֵ
    static uint16_t hbohist_h = 0;//����Ѫ�쵰����ʷ��ֵ

    //������
//	tft_draw_wave_line(1, hist_n, hbhist_h , g_BloodWave.Hp, 0x8FF0);
    __disable_irq();
    tft_draw_wave_line(0, hist_n, hbohist_h, g_BloodWave.HpO2, 0xFC1F);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
    __enable_irq();

    //�ƶ��±�
    hist_n = (hist_n < 131) ? (hist_n + 1) : 0;

    //������ʷ
//	hbhist_h = g_BloodWave.Hp;
    hbohist_h = g_BloodWave.HpO2;
}

//����Ѫ������ˢ��
void tft_draw_hrsp(int sport_data)
{
    uint16_t len3;
    char * redata3;
    uint16_t sent_buf[2];
    uint16_t set_data[2];
    delay_ms(100);
    printf("fist%d",sport_data);
    delay_ms(100);
    for(uint8_t i=0; i<backup_number; i++) {
        printf("D%d,%d,%d,%d,%d",backup_heart[i][0],backup_heart[i][1], backup_time[i][0],backup_time[i][1],backup_time[i][2]);
    delay_ms(100);
    }
		 printf("stop");

}



//tft��ʾˢ��
void tft_display_update(int sport_data,uint8_t sent_data)
{
    //ͼ�����

    if(g_blooddata.update >= 8)
    {
        //���ͼ����±�־λ
        g_blooddata.update = 0;
        if(!sent_data) {
            blood_wave_update();
            tft_draw_wave();
        }



    }

    //ת��������ݸ���
    if(g_blooddata.display >= 1)
    {
        //������±�־λ
//        g_blooddata.display = 0;
			                if(!sent_data) {
        __disable_irq();
        LCD_Fill(49,172,85,220,BLACK);
        Word48(49,172,RED,g_blooddata.heart);
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
        __enable_irq();}
			if(sent_data) {

								        __disable_irq();
        tft_draw_hrsp(sport_data);
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
        __enable_irq();
        }
//
    }
}
/*Beep and LED func -------------------------------------------------------*/

//LED�ͷ�����״̬����


/*Setup and loop func -----------------------------------------------------*/

void blood_Setup(void)
{
    //��ʼ��ѪҺ��Ϣ
    g_blooddata.heart = 0;
    g_blooddata.SpO2 = 0;
    g_blooddata.display = 1;

//	//���ƴ���
//
//	//��ʼ��ָʾ�ƺͷ�����
}

void blood_Loop(int sport_data,uint8_t sent_data)
{
    g_blooddata.update=0;
    g_fft_index=0;
    __disable_irq();
//		    LCD_Fill(49,172,85,220,BLACK);
//				max30100_Bus_Write(REG_LED1_PA,0xFF,MAX30102_Device_address);//�������ʴ�����
    max30100_Bus_Write(REG_LED2_PA,0x33,MAX30102_Device_address);// Choose value for ~ 10mA for LED2
	if(!sent_data){    Word48(49,172,RED,g_blooddata.heart);
}
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
    __enable_irq();
    while(1) {
        //ѪҺ��Ϣ��ȡ
        while(g_blooddata.display ==0) {
            if(0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||TIM3_data==1||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)) {
                break;
            }
            blood_data_update();
            if(0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||TIM3_data==1||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)) {
                break;
            }
            //ѪҺ��Ϣת��
            blood_data_translate();
            if(0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||TIM3_data==1||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)) {
                break;
            }
            //tft��ʾˢ��
            tft_display_update(sport_data,sent_data);
						if(sent_data){
						if(g_blooddata.display !=0){
							                break;
						}
					}
        }
				
        g_blooddata.display = 0;
        if(0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||TIM3_data==1||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||0 == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||sent_data) {
            break;
        }
    }
    __disable_irq();
    max30100_Bus_Write(REG_LED2_PA,0,MAX30102_Device_address);//�ر����ʴ�����
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
    __enable_irq();
    heat_INT=0;


}




