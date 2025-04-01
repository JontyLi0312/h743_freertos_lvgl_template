/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    ltdc.h
 * @brief   This file contains all the function prototypes for
 *          the ltdc.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LTDC_H__
#define __LTDC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

	/* USER CODE BEGIN Includes */

	typedef struct _pFONT
	{
		const uint8_t *ptable;
		uint16_t Width;
		uint16_t Height;
		uint16_t Sizes;
		uint16_t Table_Rows;
	} pFONT;

#define LCD_Backlight_PIN             GPIO_PIN_6
#define LCD_Backlight_PORT            GPIOH
#define GPIO_LDC_Backlight_CLK_ENABLE __HAL_RCC_GPIOH_CLK_ENABLE()
#define LCD_Backlight_OFF                                                      \
	HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_RESET);
#define LCD_Backlight_ON                                                       \
	HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_SET);
	/* USER CODE END Includes */

	extern LTDC_HandleTypeDef hltdc;

	/* USER CODE BEGIN Private defines */

	/* USER CODE END Private defines */

	void MX_LTDC_Init(void);

	/* USER CODE BEGIN Prototypes */

	/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __LTDC_H__ */
