/***
	************************************************************************************************************************************************************************************************
	*	@file  	lcd_rgb.c
	*	@version V1.0
	*  @date    2021-3-22
	*	@author  ���ͿƼ�
	*	@brief   ����RGB��ʾ��������ʾ	
   **********************************************************************************************************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H743IIT6���İ� ���ͺţ�FK743M1-IIT6�� + ����800*480�ֱ��ʵ�RGB��Ļ
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
>>>>> ��Ҫ˵����
	*
	*	1. FK743M1-IIT6 ���İ� ʹ�õ����ⲿSDRAM��Ϊ�Դ棬��ʼ��ַ0xC0000000����СΪ32MB
	*	2. �ڸ����������ʱ����Ļ����΢��������˸�����������󣬵ȴ�Ƭ�̻��������ϵ缴�ɻָ�����
	* 	3. LTDCʱ���� main.c �ļ���� SystemClock_Config()�������ã�����Ϊ33MHz����ˢ������60֡���ң����߻��߹��Ͷ��������˸
	* 
>>>>> ����˵����
	*
	*	1. �����ֿ�ʹ�õ���С�ֿ⣬���õ��˶�Ӧ�ĺ�����ȥȡģ���û����Ը����������������ɾ��
	*	2. ���������Ĺ��ܺ�ʹ�ÿ��Բο�������˵���Լ� lcd_test.c �ļ���Ĳ��Ժ���
	*
	*********************************************************************************************************************************************************************************************FANKE*****
***/

#include "lcd_rgb.h"

DMA2D_HandleTypeDef hdma2d;	// DMA2D���
LTDC_HandleTypeDef hltdc;		// LTDC���

static pFONT *LCD_Fonts;		// Ӣ������
static pFONT *LCD_CHFonts;		// ��������

//LCD��ز����ṹ��
struct	
{
	uint32_t Color;  				//	LCD��ǰ������ɫ
	uint32_t BackColor;			//	����ɫ
	uint32_t ColorMode;			// ��ɫ��ʽ
	uint32_t LayerMemoryAdd;	//	���Դ��ַ
	uint8_t  Layer; 				//	��ǰ��
	uint8_t  Direction;			//	��ʾ����
	uint8_t  BytesPerPixel;		// ÿ��������ռ�ֽ���
	uint8_t  ShowNum_Mode;		// ������ʾģʽ
}LCD;


/*************************************************************************************************
*	�� �� ��:	LCD_PanelModify
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	�����ʶ���޸ģ��˺���ֻ�Ծɿ�5�������ã�Ҳ���� RGB050M1  V1.0�İ汾
*	˵    ��:	��Ϊ�ɿ�5�������ֻ������18λ�Ľӿڣ�������Ҫ��Ӧ�İѵ�2λ�����ݽ�����͵�ƽ,
*					������Ļ�������
*************************************************************************************************/

void	LCD_PanelModify(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	

	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;				//	���ģʽ
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;							//	��������
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;				// �ٶȵȼ�2M
	
	GPIO_InitStruct.Pin 		= LTDC_R0_PIN; 			//	R0
	HAL_GPIO_Init(LTDC_R0_PORT, &GPIO_InitStruct);		
	
	GPIO_InitStruct.Pin 		= LTDC_R1_PIN; 			//	R1
	HAL_GPIO_Init(LTDC_R1_PORT, &GPIO_InitStruct);		
	
	GPIO_InitStruct.Pin 		= LTDC_G0_PIN; 			//	G0
	HAL_GPIO_Init(LTDC_G0_PORT, &GPIO_InitStruct);		
	
	GPIO_InitStruct.Pin 		= LTDC_G1_PIN; 			//	G1
	HAL_GPIO_Init(LTDC_G1_PORT, &GPIO_InitStruct);	
	
	GPIO_InitStruct.Pin 		= LTDC_B0_PIN; 			//	B0
	HAL_GPIO_Init(LTDC_B0_PORT, &GPIO_InitStruct);		

	GPIO_InitStruct.Pin 		= LTDC_B1_PIN; 			//	B1
	HAL_GPIO_Init(LTDC_B1_PORT, &GPIO_InitStruct);			
	
	// �ɿ�5����ֻ������18λ�Ľӿڣ��������ݽ��ǽӵص�
	// �����Ҫ��LTDC��Ӧ�ĵ�2λ���ݽ�����͵�ƽ
	
	HAL_GPIO_WritePin(LTDC_R0_PORT, LTDC_R0_PIN, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(LTDC_R1_PORT, LTDC_R1_PIN, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(LTDC_G0_PORT, LTDC_G0_PIN, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(LTDC_G1_PORT, LTDC_G1_PIN, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(LTDC_B0_PORT, LTDC_B0_PIN, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(LTDC_B1_PORT, LTDC_B1_PIN, GPIO_PIN_RESET);				
}

/*************************************************************************************************
*	�� �� ��:	HAL_LTDC_MspInit
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	��ʼ��LTDC���ŵ�IO��
*	˵    ��:	�ᱻ HAL_LTDC_Init() ��������			
*************************************************************************************************/

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
	uint8_t Modify_Flag = 0;	// �����ʶ���־λ
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hltdc->Instance==LTDC)
	{
		__HAL_RCC_LTDC_CLK_ENABLE();	// ʹ��LTDCʱ��

		__HAL_RCC_GPIOE_CLK_ENABLE();	// ʹ��IO��ʱ��
		__HAL_RCC_GPIOI_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		GPIO_LDC_Backlight_CLK_ENABLE;	// ʹ�ܱ�������IO��ʱ��

/*------------------------------LTDC GPIO Configuration--------------------------
		
    PG13    ------> LTDC_R0		PE5     ------> LTDC_G0			PG14    ------> LTDC_B0
    PA2     ------> LTDC_R1      PE6     ------> LTDC_G1       PG12    ------> LTDC_B1
    PH8     ------> LTDC_R2      PH13    ------> LTDC_G2       PD6     ------> LTDC_B2
    PH9     ------> LTDC_R3      PH14    ------> LTDC_G3       PA8     ------> LTDC_B3
    PH10    ------> LTDC_R4      PH15    ------> LTDC_G4       PI4     ------> LTDC_B4
    PH11    ------> LTDC_R5	   PI0     ------> LTDC_G5       PI5     ------> LTDC_B5
    PH12    ------> LTDC_R6      PI1     ------> LTDC_G6       PI6     ------> LTDC_B6
    PG6     ------> LTDC_R7	   PI2     ------> LTDC_G7       PI7     ------> LTDC_B7
	 
    PG7     ------> LTDC_CLK	
    PF10    ------> LTDC_DE	 
    PI9     ------> LTDC_VSYNC
    PI10    ------> LTDC_HSYNC
--*/	 

/*----------------------------�����ʶ��---------------------------------*/
	

		GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;		//	����ģʽ
		GPIO_InitStruct.Pull 	= GPIO_NOPULL;				//	��������
		
		GPIO_InitStruct.Pin 		= LTDC_R0_PIN; 			//	R0
		HAL_GPIO_Init(LTDC_R0_PORT, &GPIO_InitStruct);		

		GPIO_InitStruct.Pin 		= LTDC_G0_PIN; 			//	G0
		HAL_GPIO_Init(LTDC_G0_PORT, &GPIO_InitStruct);		

		GPIO_InitStruct.Pin 		= LTDC_B0_PIN; 			//	B0
		HAL_GPIO_Init(LTDC_B0_PORT, &GPIO_InitStruct);		


		// �����ʶ���޸ģ��˴�ֻ�Ծɿ�5�������ã�Ҳ���� RGB050M1  V1.0�İ汾
		// �ɿ�5����ֻ������18λ�Ľӿڣ���2λ���ݽ�ֱ�ӽӵأ������Ӧ������Ϊ�͵�ƽ�����ȷ��������Ǿɿ�5����
		// ��������������˴����û���ֲ��ʱ�����ֱ��ɾ��
		if(   (HAL_GPIO_ReadPin(LTDC_R0_PORT,LTDC_R0_PIN) == 0) \
			&& (HAL_GPIO_ReadPin(LTDC_G0_PORT,LTDC_G0_PIN) == 0) \
			&& (HAL_GPIO_ReadPin(LTDC_B0_PORT,LTDC_B0_PIN) == 0) )	
		{

			Modify_Flag	= 1;	// �����⵽�˾ɿ�5����������λ��־λ	
		}

/*-----------------------------------------------------------------------*/
		

		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1
								  |GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
								  |GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
								  |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_12|GPIO_PIN_13
								  |GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF13_LTDC;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		// ��ʼ����������
		
		GPIO_InitStruct.Pin 		= LCD_Backlight_PIN;				// ��������
		GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;			// �������ģʽ
		GPIO_InitStruct.Pull 	= GPIO_NOPULL;						// ��������
		GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;			// �ٶȵȼ���
		HAL_GPIO_Init(LCD_Backlight_PORT, &GPIO_InitStruct);	// ��ʼ��

		LCD_Backlight_OFF;	// �ȹرձ������ţ���ʼ��LTDC֮���ٿ���
		
// �����ʶ���޸ģ��˴�ֻ�Ծɿ�5�������ã�Ҳ���� RGB050M1  V1.0�İ汾
// �ɿ�5����ֻ������18λ�Ľӿڣ���2λ���ݽ�ֱ�ӽӵأ������Ӧ������Ϊ�͵�ƽ�����ȷ��������Ǿɿ�5����
// ��������������˴����û���ֲ��ʱ�����ֱ��ɾ��

		if( Modify_Flag == 1 )
		{
			LCD_PanelModify();	// ����Ӧ�ĵ�2λ��������͵�ƽ������Ƭ�����Ļ�ܸ�
		}		
		
	}
}


/*************************************************************************************************
*	�� �� ��:	MX_LTDC_Init
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	��ʼ��LTDC���ŵ�IO�ڡ�ȫ�ֲ����������õ�
*	˵    ��:	��			
*************************************************************************************************/

void MX_LTDC_Init(void)
{
	LTDC_LayerCfgTypeDef pLayerCfg = {0};		// layer0 ��ز���
	__HAL_RCC_DMA2D_CLK_ENABLE();					// ʹ��DMA2Dʱ��

	hltdc.Instance 		 = LTDC;
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;		// �͵�ƽ��Ч
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;     // �͵�ƽ��Ч
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;     // �͵�ƽ��Ч��Ҫע����ǣ��ܶ���嶼�Ǹߵ�ƽ��Ч������743��Ҫ���óɵ͵�ƽ����������ʾ
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;    // ����ʱ���ź�
  
	hltdc.Init.HorizontalSync 		= HSW - 1;									// ������Ļ���ò�������
	hltdc.Init.VerticalSync 		= VSW	-1 ;
	hltdc.Init.AccumulatedHBP		= HBP + HSW -1;
	hltdc.Init.AccumulatedVBP 		= VBP + VSW -1;
	hltdc.Init.AccumulatedActiveW = LCD_Width  + HSW + HBP -1;
	hltdc.Init.AccumulatedActiveH = LCD_Height + VSW + VBP -1;
	hltdc.Init.TotalWidth 			= LCD_Width  + HSW + HBP + HFP - 1; 
	hltdc.Init.TotalHeigh 			= LCD_Height + VSW + VBP + VFP - 1;
	
	hltdc.Init.Backcolor.Red 		= 0;	// ��ʼ����ɫ��R	
	hltdc.Init.Backcolor.Green 	= 0;  // ��ʼ����ɫ��G	
	hltdc.Init.Backcolor.Blue 		= 0;	// ��ʼ����ɫ��B
	
	HAL_LTDC_Init(&hltdc);	// ��ʼ��LTDC����

/*---------------------------------- layer0 ��ʾ���� --------------------------------*/

	pLayerCfg.WindowX0 			= 0;										// ˮƽ���
	pLayerCfg.WindowX1 			= LCD_Width;							// ˮƽ�յ�
	pLayerCfg.WindowY0 			= 0;										// ��ֱ���
	pLayerCfg.WindowY1 			= LCD_Height;							// ��ֱ�յ�
	pLayerCfg.ImageWidth 		= LCD_Width;                     // ��ʾ������
	pLayerCfg.ImageHeight 		= LCD_Height;                    // ��ʾ����߶�	
	pLayerCfg.PixelFormat 		= ColorMode_0;							// ��ɫ��ʽ	

// ���� layer0 �ĺ㶨͸���ȣ�����д�� LTDC_LxCACR �Ĵ��� 
//	��Ҫע����ǣ����������ֱ���������� layer0 ��͸���ȣ���������Ϊ255����͸�� 
	pLayerCfg.Alpha 				= 255;									// ȡֵ��Χ0~255��255��ʾ��͸����0��ʾ��ȫ͸��

// ���� layer0 �Ĳ���ϵ��������д�� LTDC_LxBFCR �Ĵ��� 
// �ò����������� layer0 �� �ײ㱳�� ֮�����ɫ���ϵ�������㹫ʽΪ ��
// ��Ϻ����ɫ =  BF1 * layer0����ɫ + BF2 * �ײ㱳������ɫ
// ��� layer0 ʹ����͸��ɫ����������ó� LTDC_BLENDING_FACTOR1_PAxCA �� LTDC_BLENDING_FACTOR2_PAxCA������ARGB�е�Aͨ����������
//	����Ľ��ܿ��Բ��� �ο��ֲ���� LTDC_LxBFCR �Ĵ����Ľ���
	pLayerCfg.BlendingFactor1 	= LTDC_BLENDING_FACTOR1_CA;		// ���ϵ��1
	pLayerCfg.BlendingFactor2 	= LTDC_BLENDING_FACTOR2_CA;      // ���ϵ��2
	
	pLayerCfg.FBStartAdress 	= LCD_MemoryAdd;                 // �Դ��ַ

// ���� layer0 �ĳ�ʼĬ����ɫ������A,R,G,B ��ֵ ������д�� LTDC_LxDCCR �Ĵ��� 
	pLayerCfg.Alpha0 				= 0;			// ��ʼ��ɫ��A
	pLayerCfg.Backcolor.Blue 	= 0;        //	��ʼ��ɫ��R
	pLayerCfg.Backcolor.Green 	= 0;        //	��ʼ��ɫ��G
	pLayerCfg.Backcolor.Red 	= 0;			//	��ʼ��ɫ��B 
  
	HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0);		// ����layer0

#if ( ( ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888 )||( ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB8888 ) ) // �ж��Ƿ�ʹ��24λ����32λɫ

// ��Ϊ743ÿ��ͨ���ĵ�λ���ǲ���α�����������������������ɫ���������޷�������ʾ24λ����32λɫ

	HAL_LTDC_EnableDither(&hltdc); // ������ɫ����
	
#endif


/*---------------------------------- layer1 ��ʾ���� --------------------------------*/

#if ( LCD_NUM_LAYERS == 2 )	//���������˫��
  
	LTDC_LayerCfgTypeDef pLayerCfg1 = {0};
  
	pLayerCfg1.WindowX0 				= 0;							// ˮƽ���
	pLayerCfg1.WindowX1 				= LCD_Width;				// ˮƽ�յ�
	pLayerCfg1.WindowY0 				= 0;							// ��ֱ���
	pLayerCfg1.WindowY1 				= LCD_Height;				// ��ֱ�յ�
	pLayerCfg1.ImageWidth 			= LCD_Width;         	// ��ʾ������
	pLayerCfg1.ImageHeight 			= LCD_Height;        	// ��ʾ����߶�		
	pLayerCfg1.PixelFormat 			= ColorMode_1;				// ��ɫ��ʽ��layer1 Ӧ����Ϊ����͸��ɫ�ĸ�ʽ������ARGB8888��ARGB1555

// ���� layer1 �ĺ㶨͸���ȣ�����д�� LTDC_LxCACR �Ĵ��� 
//	��Ҫע����ǣ����������ֱ���������� layer1 ��͸���ȣ���������Ϊ255����͸�� 
	pLayerCfg1.Alpha 					= 255;			// ȡֵ��Χ0~255��255��ʾ��͸����0��ʾ��ȫ͸��
	
// ���� layer1 �Ĳ���ϵ��������д�� LTDC_LxBFCR �Ĵ��� 
// �ò����������� layer1 �� (layer0+������֮�����ɫ���ϵ�������㹫ʽΪ ��
// ��Ϻ����ɫ =  BF1 * layer1����ɫ + BF2 * (layer0+������Ϻ����ɫ��
// ��� layer1 ʹ����͸��ɫ����������ó� LTDC_BLENDING_FACTOR1_PAxCA �� LTDC_BLENDING_FACTOR2_PAxCA������ARGB�е�Aͨ����������
//	����Ľ��ܿ��Բ��� �ο��ֲ���� LTDC_LxBFCR �Ĵ����Ľ���
	pLayerCfg1.BlendingFactor1 	= LTDC_BLENDING_FACTOR1_PAxCA;			// ���ϵ��1
	pLayerCfg1.BlendingFactor2 	= LTDC_BLENDING_FACTOR2_PAxCA;      	// ���ϵ��2
	
	pLayerCfg1.FBStartAdress 		= LCD_MemoryAdd + LCD_MemoryAdd_OFFSET; // �Դ��ַ
	

// ����layer1 �ĳ�ʼĬ����ɫ������A,R,G,B ��ֵ ������д�� LTDC_LxDCCR �Ĵ��� 
	pLayerCfg1.Alpha0 				= 0;				// ��ʼ��ɫ��A
	pLayerCfg1.Backcolor.Red 		= 0;				//	��ʼ��ɫ��R
	pLayerCfg1.Backcolor.Green 	= 0;           //	��ʼ��ɫ��G
	pLayerCfg1.Backcolor.Blue 		= 0;           //	��ʼ��ɫ��B 
	
	HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1);	// ��ʼ�� layer1 ������

	#if ( ( ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888 )||( ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB8888 ) ) // �ж��Ƿ�ʹ��24λ����32λɫ

	// ��Ϊ743ÿ��ͨ���ĵ�λ���ǲ���α�����������������������ɫ���������޷�������ʾ24λ����32λɫ
	
		HAL_LTDC_EnableDither(&hltdc); // ������ɫ����
		
	#endif

#endif

	HAL_LTDC_ProgramLineEvent(&hltdc, 0 );			// �������жϣ���0��
	HAL_NVIC_SetPriority(LTDC_IRQn, 0xE, 0);		// �������ȼ�
	HAL_NVIC_EnableIRQ(LTDC_IRQn);					// ʹ���ж�	

// LTDC�ڳ�ʼ��֮���ϵ��˲�����һ�����ݵİ�����
//	��ʹһ��ʼ�ͽ������������Ͳ�����Ļ���������õ����������ǻ����������
//	�����Ҫ����������󣬿����ڳ�ʼ�����֮�󣬽���һ�����ݵ���ʱ�ٴ򿪱���
//
//	HAL_Delay(200);	// ��ʱ200ms

	LCD_Backlight_ON;							// ��������	

}

/**************************************************************************************************************************************************************************************************************************************************************************FANKE***/
// ʵ��ƽ̨������ STM32H743���İ�
//

