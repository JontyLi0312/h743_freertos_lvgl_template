/***
	************************************************************************************************
	*	@file  	lcd_pwm.c
	*	@version V1.0
	*  @date    2021-3-11
	*	@author  反客科技
	*	@brief   LCD背光pwm相关函数
   ************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32H743IIT6核心板 （型号：FK743M1-IIT6）
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*  1.PWM频率为2KHz
	*	2.HAL_TIM_MspPostInit用于初始化IO口，HAL_TIM_Base_MspInit用于开启时钟
	*
	************************************************************************************************
***/

#include "lcd_pwm.h"
#include "ltdc.h"
#include "stm32h7xx_hal_rcc.h"

TIM_HandleTypeDef htim12;  // TIM_HandleTypeDef 结构体变量

static uint16_t LCD_PwmPeriod = 500;  // 定时器重载值

/*************************************************************************************************
 *	函 数 名:	HAL_TIM_MspPostInit
 *	入口参数:	htim - TIM_HandleTypeDef结构体变量，即表示定义的TIM（句柄）
 *	返 回 值:	无
 *	函数功能:	初始化 TIM 相应的PWM口
 *	说    明:	初始化PWM用到的引脚
 *
 *************************************************************************************************/

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (htim->Instance == TIM12)
	{
		GPIO_LDC_Backlight_CLK_ENABLE;  // 开启背光引脚端口时钟

		GPIO_InitStruct.Pin = GPIO_PIN_6;             // 背光引脚
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       // 复用推挽输出
		GPIO_InitStruct.Pull = GPIO_NOPULL;           // 不上下拉
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // 低速模式
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM12;   // 复用

		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);  // 初始化IO口
	}
}

/*************************************************************************************************
 *	函 数 名:	LCD_PwmSetPulse
 *	入口参数:	pulse - PWM占空比，范围 0~100
 *	返 回 值:	无
 *	函数功能:	设置PWM占空比
 *	说    明:	无
 *************************************************************************************************/

void LCD_PwmSetPulse(uint8_t pulse)
{
	uint16_t compareValue;

	compareValue = pulse * LCD_PwmPeriod / 100;  // 根据占空比设置比较值

	TIM4->CCR4 = compareValue;  // 修改TIM4的通道4比较值
}

/*************************************************************************************************
 *	函 数 名:	LCD_PWMinit
 *	入口参数:	pulse - PWM占空比，范围 0~100
 *	返 回 值:	无
 *	函数功能:	初始化TIM4,配置PWM频率为2KHz
 *	说    明:	无
 *************************************************************************************************/

void LCD_PWMinit(uint8_t pulse)
{

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim12.Instance = TIM12;      // 定时器
	htim12.Init.Prescaler = 240;  // 预分频系数，此时定时器的计数频率为 1MKHz
	htim12.Init.CounterMode = TIM_COUNTERMODE_UP;  // 向上计数模式
	htim12.Init.Period = LCD_PwmPeriod - 1;        // 重载值499，即计数500次
	htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  // 时钟不分频
	htim12.Init.AutoReloadPreload =
		TIM_AUTORELOAD_PRELOAD_DISABLE;  // 控制寄存器 TIMx_CR1 的ARPE
										 // 位置0，即禁止自动重载寄存器进行预装载

	HAL_TIM_Base_Init(&htim12);  // 根据上面的参数，对TIM2进行初始化

	sClockSourceConfig.ClockSource =
		TIM_CLOCKSOURCE_INTERNAL;  // 选择内部时钟源
	HAL_TIM_ConfigClockSource(&htim12,
							  &sClockSourceConfig);  // 初始化配置时钟源

	HAL_TIM_PWM_Init(&htim12);  // 根据上面的参数，对TIM进行PWM初始化

	sMasterConfig.MasterOutputTrigger =
		TIM_TRGO_RESET;  // 触发输出选择，此时配置复位模式，即寄存器 TIMx_CR2 的
						 // MMS 为为000
	sMasterConfig.MasterSlaveMode =
		TIM_MASTERSLAVEMODE_DISABLE;  // 不使用从模式
	HAL_TIMEx_MasterConfigSynchronization(&htim12,
										  &sMasterConfig);  // 初始化配置

	sConfigOC.OCMode = TIM_OCMODE_PWM1;  // PWM模式1
	sConfigOC.Pulse =
		pulse * LCD_PwmPeriod / 100;  // 比较值250，重载为500，则占空比为50%
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;  // 有效状态为高电平
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;   // 禁止快速模式
	HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC,
							  TIM_CHANNEL_1);  // 初始化配置PWM

	HAL_TIM_MspPostInit(&htim12);               // 初始化IO口
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);  // 启动PWM
}

/*************************************************************************************************
 *	函 数 名:	HAL_TIM_Base_MspInit
 *	入口参数:	htim_base - TIM_HandleTypeDef结构体变量，即表示定义的TIM（句柄）
 *	返 回 值:	无
 *	函数功能:	开启时钟
 *	说    明:	无
 *
 *************************************************************************************************/

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
	if (htim_base->Instance == TIM12)
	{
		__HAL_RCC_GPIOH_CLK_ENABLE();  // 开启 TIM 时钟
	}
}
