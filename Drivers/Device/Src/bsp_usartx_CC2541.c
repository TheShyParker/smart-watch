/**
  ******************************************************************************
  * �ļ�����: bsp_usartx.c
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ������ͨѶ
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  *
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp_usartx_CC2541.h"
#include "usart.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/

/* ��չ���� ------------------------------------------------------------------*/
extern uint8_t aRxBuffer;
extern uint8_t aRxBuffer2;



/**
  * ��������: ���ڷ���һ���ֽ�����
  * �������: ch���������ַ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Usart_SendByte(uint8_t ch )
{
    while(__HAL_UART_GET_FLAG(&husartx,UART_FLAG_TXE)==0); //ѭ������,ֱ���������
    /* ����һ���ֽ����ݵ�USART2 */
    HAL_UART_Transmit(&husartx,(uint8_t *)&ch,1,0xffff);

}

/**
  * ��������: ���ڷ���ָ�����ȵ��ַ���
  * �������: str���������ַ���������
  *           strlen:ָ���ַ�������
  * �� �� ֵ: ��
  * ˵    ������
  */
void Usart_SendStr_length(uint8_t *str,uint32_t strlen )
{
    unsigned int k=0;
    do
    {
        Usart_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
}

/**
  * ��������: ���ڷ����ַ�����ֱ�������ַ���������
  * �������: str���������ַ���������
  * �� �� ֵ: ��
  * ˵    ������
  */
void Usart_SendString(uint8_t *str)
{
    unsigned int k=0;
    do
    {
        Usart_SendByte(*(str + k) );
        k++;
    } while(*(str + k)!='\0');
}

//�жϻ��洮������
#define UART_BUFF_SIZE      100
__IO  uint16_t uart_p = 0;
__IO  uint16_t uart_p2 = 0;
uint8_t   uart_buff[UART_BUFF_SIZE];
uint8_t   uart_buff2[UART_BUFF_SIZE];
//uint8_t   uart_buff3[100000];

/**
  * ��������: �����жϻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
//	printf()
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

    if(UartHandle==&huart2) {
        if(uart_p<UART_BUFF_SIZE)
        {
            uart_buff[uart_p] =aRxBuffer;
            uart_p++;
            HAL_UART_Receive_IT(UartHandle,&aRxBuffer,1);

        }
        else
        {
            clean_rebuff(1);
        }
    }
    else {
        if(uart_p2<UART_BUFF_SIZE)
        {
            uart_buff2[uart_p2] =aRxBuffer2;
            uart_p2++;
            HAL_UART_Receive_IT(UartHandle,&aRxBuffer2,1);


        }
        else
        {
            clean_rebuff(2);
        }
    }
}

/**
  * ��������: ��ȡ���յ������ݺͳ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
char *get_rebuff(uint16_t *len)
{
    *len = uart_p;
    return (char *)&uart_buff;
}
char *get_rebuff2(uint16_t *len2)
{
    *len2 = uart_p2;
    return (char *)&uart_buff2;
}

/**
  * ��������: ��ջ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void clean_rebuff(int a)
{

    uint16_t i=UART_BUFF_SIZE;

    if(a==1) {


        uart_p = 0;
        while(i)
            uart_buff[--i]=0;
    }
    else {

        uart_p2 = 0;
        while(i)
            uart_buff2[--i]=0;
    }
}

uint8_t CC2541_Send_CMD(char* cmd)
{
    uint16_t len;
    char * redata;
    uint8_t retry=2;
    uint8_t i;
    while(retry--)
    {
        clean_rebuff(1);
        delay_ms(10);
        Usart_SendString((uint8_t*)cmd);
        delay_ms(100);
        for(i=0; i<20; i++)
        {
            redata=get_rebuff(&len);
            if(len>0)
            {
//                if(strstr(redata,"CS"))
//                {
                    return 1;
//                }

            }
            delay_ms(200);
        }
    }
    clean_rebuff(1);
    return 0 ;
}
uint8_t CC2541_Send_CMD_blue(char* cmd)
{
    uint16_t len;
    char * redata;
    uint8_t retry=2;
    uint8_t i;
    while(retry--)
    {
        clean_rebuff(1);
        delay_ms(10);
        Usart_SendString((uint8_t*)cmd);
        delay_ms(100);

        for(i=0; i<4; i++)
        {
            redata=get_rebuff(&len);
            if(len>0)
            {
                if(strstr(redata,"N"))
                {


                    clean_rebuff(1);
                    return 0;
                }
            }
            else {

                delay_ms(100);
            }
        }
    }
    clean_rebuff(1);
    return 1 ;
}
uint8_t CC2541_Send_CMD_power(char* cmd)
{
    uint16_t len;
    char * redata;
    uint8_t retry=2;
    uint8_t i;
    while(retry--)
    {
        clean_rebuff(1);
        delay_ms(10);
        Usart_SendString((uint8_t*)cmd);
        delay_ms(100);

//        for(i=0; i<1; i++)
//        {
        redata=get_rebuff(&len);
        if(len>0)
        {
            if(strstr(redata,"ADC"))
            {
                return 1;
            }
        }
    }
//        }

    clean_rebuff(1);
    return 0 ;
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
