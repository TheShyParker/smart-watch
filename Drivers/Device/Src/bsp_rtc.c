/**
  ******************************************************************************
  * 文件名程: bsp_rtc.c
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 板载调试串口底层驱动程序：默认使用USART1
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  *
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp_rtc.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 从串口调试助手获取数字值(把ASCII码转换为数字)
  * 输入参数: value 用户在超级终端中输入的数值
  * 返 回 值: 输入字符的ASCII码对应的数值
  * 说    明：本函数专用于RTC获取时间，若进行其它输入应用，要修改一下
  */


/**
  * 函数功能: 配置当前时间和日期
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void RTC_CalendarConfig(void)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef DateToUpdate;
#if 1
    /* 配置日期 */
    /* 设置日期：2015年10月4日 星期日 */
    DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
    DateToUpdate.Month = RTC_MONTH_JUNE;
    DateToUpdate.Date = 0x8;
    DateToUpdate.Year = 0x18;
    HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BCD);

    /* 配置时间 */
    /* 时钟时间：10:15:46 */
    sTime.Hours = 0x10;
    sTime.Minutes = 0x15;
    sTime.Seconds = 0x46;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
#else
    __IO uint32_t Tmp_YY = 0xFF, Tmp_MM = 0xFF, Tmp_DD = 0xFF, Tmp_HH = 0xFF, Tmp_MI = 0xFF, Tmp_SS = 0xFF;

    printf("=========================时间设置==================\n");
    printf("请输入年份: 20\n");
    while (Tmp_YY == 0xFF)
    {
        Tmp_YY = USART_Scanf(99);
    }
    printf("年份被设置为:  20%0.2d\n", Tmp_YY);

    DateToUpdate.Year = Tmp_YY;

    printf("请输入月份:  \n");
    while (Tmp_MM == 0xFF)
    {
        Tmp_MM = USART_Scanf(12);
    }
    printf("月份被设置为:  %d\n", Tmp_MM);

    DateToUpdate.Month= Tmp_MM;

    printf("请输入日期:  \n");
    while (Tmp_DD == 0xFF)
    {
        Tmp_DD = USART_Scanf(31);
    }
    printf("日期被设置为:  %d\n", Tmp_DD);
    DateToUpdate.Date= Tmp_DD;
    HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);

    printf("请输入时钟:  \n");
    while (Tmp_HH == 0xFF)
    {
        Tmp_HH = USART_Scanf(23);
    }
    printf("时钟被设置为:  %d\n", Tmp_HH );
    sTime.Hours= Tmp_HH;


    printf("请输入分钟:  \n");
    while (Tmp_MI == 0xFF)
    {
        Tmp_MI = USART_Scanf(59);
    }
    printf("分钟被设置为:  %d\n", Tmp_MI);
    sTime.Minutes= Tmp_MI;

    printf("请输入秒钟:  \n");
    while (Tmp_SS == 0xFF)
    {
        Tmp_SS = USART_Scanf(59);
    }
    printf("秒钟被设置为:  %d\n", Tmp_SS);
    sTime.Seconds= Tmp_SS;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
#endif

    /* 写入一个数值：0x32F1到RTC备份数据寄存器1 */
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F1);
}

/**
  * 函数功能: RTC实时时钟初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
/* RTC init function */
void MX_RTC_Init(void)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    /** Initialize RTC Only
    */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
//    Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */

    /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date
    */
    sTime.Hours = 0x9;
    sTime.Minutes = 0x25;
    sTime.Seconds = 0x0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
    {
//    Error_Handler();
    }
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = RTC_MONTH_JUNE;
    sDate.Date = 0x15;
    sDate.Year = 0x20;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
//    Error_Handler();
    }
    /** Enable the reference Clock input
    */
    if (HAL_RTCEx_SetRefClock(&hrtc) != HAL_OK)
    {
//    Error_Handler();
    }

}

/**
  * 函数功能: RTC实时时钟初始化
  * 输入参数: hrtc：RTC外设句柄指针
  * 返 回 值: 无
  * 说    明: 该函数供HAL库内部函数调用
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    if(hrtc->Instance==RTC)
    {
        /* To change the source clock of the RTC feature (LSE, LSI), You have to:
           - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
           - Enable write access using HAL_PWR_EnableBkUpAccess() function before to
             configure the RTC clock source (to be done once after reset).
           - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and
             __HAL_RCC_BACKUPRESET_RELEASE().
           - Configure the needed RTc clock source */
        /* 使能PWR(电源管理外设)时钟和使能获取备份域 */
        __HAL_RCC_PWR_CLK_ENABLE();
        HAL_PWR_EnableBkUpAccess();

        /* 使能备份时钟：备份寄存器 */
        __HAL_RCC_BKP_CLK_ENABLE();

        /* 配置外部低速时钟为RTC时钟源 */
        RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
        RCC_OscInitStruct.LSEState = RCC_LSE_ON;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

        /* 使能RTC外设时钟 */
        __HAL_RCC_RTC_ENABLE();
    }

}

/**
  * 函数功能: RTC实时时钟反初始化
  * 输入参数: hrtc：RTC外设句柄指针
  * 返 回 值: 无
  * 说    明: 该函数供HAL库内部函数调用
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{

    if(hrtc->Instance==RTC)
    {
        /* 禁用RTC时钟 */
        __HAL_RCC_RTC_DISABLE();

        /* 禁用PWR时钟和读取备份域 */
        HAL_PWR_DisableBkUpAccess();
        __HAL_RCC_PWR_CLK_DISABLE();

        /* 禁用备份时钟：备份寄存器 */
        __HAL_RCC_BKP_CLK_DISABLE();
    }
}


/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
