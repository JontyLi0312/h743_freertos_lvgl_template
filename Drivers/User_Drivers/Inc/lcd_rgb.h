#ifndef  __LCD_RGB_H
#define	__LCD_RGB_H

#include "stm32h7xx_hal.h"
#include "usart.h"
#include "sdram.h"  
#include "lcd_fonts.h"
#include <stdio.h>

// 1. LTDC时钟在 main.c 文件中 SystemClock_Config() 函数设置，例程默认为33MHz，
//    即刷新率在60帧左右，过高的时钟会加大对SDRAM的占用，容易造成花屏，并且屏
//    幕本身不支持这么高的刷新率，还会导致屏幕闪烁显示异常等，过低的时钟会导
//    致刷新率太低，屏幕会有明显的闪烁
// 
// 2. 只开单层显示，16位色、RGB888以及ARGB8888模式下，LTDC时钟可以设置为33M
//
// 3. 开双层显示，RGB565和ARGB1555模式下，LTDC时钟可以设置为33M
// 4. 开双层显示，RGB565和ARGB8888模式下，LTDC时钟最大只能设置为28M（不同的面板会有所差异，需要根据实际情况调整）
// 5. 开双层显示，RGB888和ARGB1555模式下，LTDC时钟最大只能设置为33M（不同的面板会有所差异，需要根据实际情况调整）
// 6. 开双层显示，RGB888和ARGB8888模式下，LTDC时钟最大只能设置为24M（不同的面板会有所差异，需要根据实际情况调整）
//
// 7. 综上，FK743M1核心板开启双层时，推荐使用 16位色+16位色 或者 24位色+16位色 的组合，这样可以确保屏幕工作在最佳的刷新率

#define 	LCD_NUM_LAYERS  1	//	定义显示的层数，743可驱动两层显示

#define	ColorMode_0   LTDC_PIXEL_FORMAT_RGB565   		//定义 layer0 的颜色格式
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_ARGB1555  
//#define	ColorMode_0    LTDC_PIXEL_FORMAT_ARGB4444  
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_RGB888
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_ARGB8888   


#if  LCD_NUM_LAYERS == 2	// 如果开启了双层，则在此处定义 layer1 的颜色格式			

//	#define	ColorMode_1   LTDC_PIXEL_FORMAT_RGB565   	
	#define	ColorMode_1   LTDC_PIXEL_FORMAT_ARGB1555  // ARGB1555仅支持一位透明色，即仅有透明和不透明两种状态
//	#define	ColorMode_1   LTDC_PIXEL_FORMAT_ARGB4444  // ARGB4444支持4位透明色，有16种透明状态
// #define	ColorMode_1   LTDC_PIXEL_FORMAT_RGB888   
//	#define	ColorMode_1   LTDC_PIXEL_FORMAT_ARGB8888   // ARGB8888支持8位透明色，有256种透明状态	

#endif




/*---------------------------------------------------------- 函数声明 -------------------------------------------------------*/
	
void MX_LTDC_Init(void);		// LTDC以及层初始化 

/*-------------------------------------------------------- LCD相关参数 -------------------------------------------------------*/

#define HBP  80	// 根据屏幕的手册进行设置
#define VBP  40
#define HSW  1
#define VSW  1
#define HFP  200
#define VFP  22


// FK743M1-IIT6 核心板 使用的是外部SDRAM作为显存，起始地址0xC0000000，大小为32MB
// 显存所需空间 = 分辨率 * 每个像素所占字节数，例如 800*480的屏，使用16位色（RGB565或者AEGB1555），需要显存 800*480*2 = 768000 字节

#define LCD_Width     		800						//	LCD的像素长度
#define LCD_Height    		480						//	LCD的像素宽度
#define LCD_MemoryAdd   	SDRAM_BANK_ADDR 		//	显存的起始地址  

	
#if ( ColorMode_0 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_0 ==LTDC_PIXEL_FORMAT_ARGB4444 )
	#define BytesPerPixel_0		2		//16位色模式每个像素占2字节
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888
	#define BytesPerPixel_0		3		//24位色模式每个像素占3字节
#else
	#define BytesPerPixel_0		4		//32位色模式每个像素占4字节
#endif	

#if LCD_NUM_LAYERS == 2

	#if ( ColorMode_1 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB4444 )
		#define BytesPerPixel_1		2	//16位色模式每个像素占2字节
	#elif ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888	
		#define BytesPerPixel_1		3	//24位色模式每个像素占3字节
	#else	
		#define BytesPerPixel_1		4	//32位色模式每个像素占4字节
	#endif	

	#define LCD_MemoryAdd_OFFSET   LCD_Width * LCD_Height * BytesPerPixel_0 	 //第二层的显存的偏移地址 

#endif
/*-------------------------------------------------------- LCD背光引脚 -------------------------------------------------------*/

#define  LCD_Backlight_PIN								GPIO_PIN_6								
#define	LCD_Backlight_PORT							GPIOH									
#define 	GPIO_LDC_Backlight_CLK_ENABLE        	__HAL_RCC_GPIOH_CLK_ENABLE()	 	

#define	LCD_Backlight_OFF		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_RESET);	// 关闭背光
#define 	LCD_Backlight_ON		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_SET);		// 开启背光


/*------------------------------------------------ LCD低2位引脚，用于旧面板识别 ----------------------------------------------*/

//// 旧面板识别修改，此处只对旧款5寸屏有用，也就是 RGB050M1  V1.0的版本
//// 旧款5寸屏只引出了18位的接口，低2位数据脚直接接地，如果检测到对应的引脚为低电平则可以确定接入的是旧款5寸屏
//// 其它面板无需理会此处，用户移植的时候可以直接删掉

#define  LTDC_R0_PIN						GPIO_PIN_13								
#define	LTDC_R0_PORT					GPIOG									

#define  LTDC_R1_PIN						GPIO_PIN_2								
#define	LTDC_R1_PORT					GPIOA	

#define  LTDC_G0_PIN						GPIO_PIN_5								
#define	LTDC_G0_PORT					GPIOE									

#define  LTDC_G1_PIN						GPIO_PIN_6								
#define	LTDC_G1_PORT					GPIOE		

#define  LTDC_B0_PIN						GPIO_PIN_14								
#define	LTDC_B0_PORT					GPIOG									

#define  LTDC_B1_PIN						GPIO_PIN_12								
#define	LTDC_B1_PORT					GPIOG		

	 
#endif //__LCD_RGB_H

