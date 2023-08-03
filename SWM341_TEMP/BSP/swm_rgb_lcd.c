#include "main.h"

uint16_t *LCD_Buffer = (uint16_t *)SDRAMM_BASE;

static void MemoryInit(void)
{
	SDRAM_InitStructure SDRAM_InitStruct;
	
	PORT_Init(PORTM, PIN13, PORTM_PIN13_SDR_CLK, 0);
	PORT_Init(PORTM, PIN14, PORTM_PIN14_SDR_CKE, 0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_SDR_WE,   0);
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_SDR_CAS,  0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_SDR_RAS,  0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_SDR_CS,  0);
	PORT_Init(PORTE, PIN15, PORTE_PIN15_SDR_BA0, 0);
	PORT_Init(PORTE, PIN14, PORTE_PIN14_SDR_BA1, 0);
	PORT_Init(PORTN, PIN14, PORTN_PIN14_SDR_A0,  0);
	PORT_Init(PORTN, PIN13, PORTN_PIN13_SDR_A1,  0);
	PORT_Init(PORTN, PIN12, PORTN_PIN12_SDR_A2,  0);
	PORT_Init(PORTN, PIN11, PORTN_PIN11_SDR_A3,  0);
	PORT_Init(PORTN, PIN10, PORTN_PIN10_SDR_A4,  0);
	PORT_Init(PORTN, PIN9,  PORTN_PIN9_SDR_A5,   0);
	PORT_Init(PORTN, PIN8,  PORTN_PIN8_SDR_A6,   0);
	PORT_Init(PORTN, PIN7,  PORTN_PIN7_SDR_A7,   0);
	PORT_Init(PORTN, PIN6,  PORTN_PIN6_SDR_A8,   0);
	PORT_Init(PORTN, PIN3,  PORTN_PIN3_SDR_A9,   0);
	PORT_Init(PORTN, PIN15, PORTN_PIN15_SDR_A10, 0);
	PORT_Init(PORTN, PIN2,  PORTN_PIN2_SDR_A11,  0);
	PORT_Init(PORTM, PIN15, PORTM_PIN15_SDR_A12, 0);
	PORT_Init(PORTE, PIN7,  PORTE_PIN7_SDR_D0,   1);
	PORT_Init(PORTE, PIN6,  PORTE_PIN6_SDR_D1,   1);
	PORT_Init(PORTE, PIN5,  PORTE_PIN5_SDR_D2,   1);
	PORT_Init(PORTE, PIN4,  PORTE_PIN4_SDR_D3,   1);
	PORT_Init(PORTE, PIN3,  PORTE_PIN3_SDR_D4,   1);
	PORT_Init(PORTE, PIN2,  PORTE_PIN2_SDR_D5,   1);
	PORT_Init(PORTE, PIN1,  PORTE_PIN1_SDR_D6,   1);
	PORT_Init(PORTE, PIN0,  PORTE_PIN0_SDR_D7,   1);
	PORT_Init(PORTE, PIN8,  PORTE_PIN8_SDR_D8,   1);
	PORT_Init(PORTE, PIN9,  PORTE_PIN9_SDR_D9,   1);
	PORT_Init(PORTE, PIN10, PORTE_PIN10_SDR_D10, 1);
	PORT_Init(PORTE, PIN11, PORTE_PIN11_SDR_D11, 1);
	PORT_Init(PORTE, PIN12, PORTE_PIN12_SDR_D12, 1);
	PORT_Init(PORTE, PIN13, PORTE_PIN13_SDR_D13, 1);
	PORT_Init(PORTC, PIN14, PORTC_PIN14_SDR_D14, 1);
	PORT_Init(PORTC, PIN15, PORTC_PIN15_SDR_D15, 1);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_SDR_LDQM, 0);
	PORT_Init(PORTM, PIN12, PORTM_PIN12_SDR_UDQM,0);
	
	SDRAM_InitStruct.Size = SDRAM_SIZE_8MB;
	SDRAM_InitStruct.ClkDiv = SDRAM_CLKDIV_1;
	SDRAM_InitStruct.CASLatency = SDRAM_CASLATENCY_2;
	SDRAM_InitStruct.TimeTRP  = SDRAM_TRP_3;
	SDRAM_InitStruct.TimeTRCD = SDRAM_TRCD_3;
	SDRAM_InitStruct.TimeTRFC  = SDRAM_TRFC_15;
	SDRAM_Init(&SDRAM_InitStruct);
}


void RGBLCDInit(void)
{
	LCD_InitStructure LCD_initStruct;
	
	MemoryInit();

	/* R- 8 bit : [ 0 ~ 7 ] */
	GPIO_Init(GPIOC, PIN4, 1, 0, 0, 0);
	GPIO_Init(GPIOC, PIN5, 1, 0, 0, 0);
	GPIO_Init(GPIOC, PIN8, 1, 0, 0, 0);
	GPIO_AtomicClrBit(GPIOC, PIN4);
	GPIO_AtomicClrBit(GPIOC, PIN5);
	GPIO_AtomicClrBit(GPIOC, PIN8);
	/* R- 5 bit : [ 3 ~ 7 ] */
	PORT_Init(PORTC, PIN9, PORTC_PIN9_LCD_R3, 0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_LCD_R4, 0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_LCD_R5, 0);
	PORT_Init(PORTC, PIN12, PORTC_PIN12_LCD_R6, 0);
	PORT_Init(PORTC, PIN13, PORTC_PIN13_LCD_R7, 0);

	/* G- 8 bit : [ 0 ~ 7 ] */
	GPIO_Init(GPIOA, PIN12, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN13, 1, 0, 0, 0);
	GPIO_AtomicClrBit(GPIOA, PIN12);
	GPIO_AtomicClrBit(GPIOA, PIN13);
	/* G- 6 bit : [ 2 ~ 7 ] */
	PORT_Init(PORTA, PIN14, PORTA_PIN14_LCD_G2, 0);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_LCD_G3, 0);
	PORT_Init(PORTC, PIN0, PORTC_PIN0_LCD_G4, 0);
	PORT_Init(PORTC, PIN1, PORTC_PIN1_LCD_G5, 0);
	PORT_Init(PORTC, PIN2, PORTC_PIN2_LCD_G6, 0);
	PORT_Init(PORTC, PIN3, PORTC_PIN3_LCD_G7, 0);

	/* B- 8 bit : [ 0 ~ 7 ] */
	GPIO_Init(GPIOB, PIN1, 1, 0, 0, 0);
	GPIO_Init(GPIOB, PIN11, 1, 0, 0, 0);
	GPIO_Init(GPIOB, PIN13, 1, 0, 0, 0);
	GPIO_AtomicClrBit(GPIOB, PIN1);
	GPIO_AtomicClrBit(GPIOB, PIN11);
	GPIO_AtomicClrBit(GPIOB, PIN13);
	/* B- 5 bit : [ 3 ~ 7 ] */
	PORT_Init(PORTB, PIN15, PORTB_PIN15_LCD_B3, 0);
	PORT_Init(PORTA, PIN2, PORTA_PIN2_LCD_B4, 0);
	PORT_Init(PORTA, PIN9, PORTA_PIN9_LCD_B5, 0);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_LCD_B6, 0);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_LCD_B7, 0);


	PORT_Init(PORTB, PIN2,  PORTB_PIN2_LCD_VSYNC, 0);
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_LCD_HSYNC, 0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_LCD_DEN,   0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_LCD_DCLK,  0);

//	PORT_Init(PORTM, PIN8, PORTM_PIN8_LCD_HSYNC, 0);
//	PORT_Init(PORTM, PIN11, PORTM_PIN11_LCD_DEN, 0);


	LCD_initStruct.ClkDiv = 6;
	LCD_initStruct.Format = LCD_FMT_RGB565;
	LCD_initStruct.HnPixel = LCD_HDOT;
	LCD_initStruct.VnPixel = LCD_VDOT;
	LCD_initStruct.Hfp = IC_Hfp;
	LCD_initStruct.Hbp = IC_Hbp;
	LCD_initStruct.Vfp = IC_Vfp;
	LCD_initStruct.Vbp = IC_Vbp;
	LCD_initStruct.HsyncWidth = IC_HsyncWidth;
	LCD_initStruct.VsyncWidth = IC_VsyncWidth;
	LCD_initStruct.DataSource = (uint32_t)LCD_Buffer;
	LCD_initStruct.Background = 0xFFFF;
	LCD_initStruct.SampleEdge =PCLK_EDGE;	// LCD_SAMPLE_FALL LCD_SAMPLE_RISE
	LCD_initStruct.IntEOTEn = 1;
	LCD_Init(LCD, &LCD_initStruct);
}
void LCD_Handler(void)
{
	LCD_INTClr(LCD);
	
	LCD_Start(LCD);
}