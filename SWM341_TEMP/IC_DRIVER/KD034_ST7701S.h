#ifndef __KD034_ST7701S_H__
#define __KD034_ST7701S_H__

/***********************************LCD参数-宏定义************************************/
#define IC_VsyncWidth 10
#define IC_HsyncWidth 8
#define IC_Vbp 60
#define IC_Vfp 40
#define IC_Hbp 56
#define IC_Hfp 20
#define PCLK_EDGE LCD_SAMPLE_FALL

#define LCD_HDOT	480		// 水平点数
#define LCD_VDOT	480	// 垂直点数
/***********************************SPI GPIO宏定义************************************/
/* SWM34SVET6_Pin100 SPI端口映射
 * 接线：
			KD028_LCD        SWM341
				SDI-------------PN4
				SCL-------------PN5
				 CS-------------PB0
			RESET-------------PD1
*/
#define LCD_GPIO_RST            GPIOD
#define LCD_PIN_RST             PIN1

#define LCD_GPIO_CS             GPIOB
#define LCD_PIN_CS              PIN0

#define LCD_GPIO_SCK            GPION
#define LCD_PIN_SCK             PIN5

#define LCD_GPIO_SDA            GPION
#define LCD_PIN_SDA             PIN4
/* 操作宏 */
#if     0 /* 库函数 */
    /** 获取 IO 输入(须保证对应 IO 为 GPIO 输入状态, 否则操作无效) */
    #define IO_GET(GPIOx, PINx)             GPIO_GetBits(GPIOx, PINx)
    /** IO 置低(须保证对应 IO 为 GPIO 输出状态, 否则操作无效) */
    #define IO_SET_L(GPIOx, PINx)           GPIO_AtomicClrBit(GPIOx, PINx)
    /** IO 置高(须保证对应 IO 为 GPIO 输出状态, 否则操作无效) */
    #define IO_SET_H(GPIOx, PINx)           GPIO_AtomicSetBit(GPIOx, PINx)

#else /* 寄存器 */
    /** 获取 IO 输入(须保证对应 IO 为 GPIO 输入状态, 否则操作无效) */
    #define IO_GET(GPIOx, PINx)             ( ( ( (GPIOx)->IDR ) >> (PINx) ) & 0x01 )
    /** IO 置低(须保证对应 IO 为 GPIO 输出状态, 否则操作无效) */
    #define IO_SET_L(GPIOx, PINx)           ( *( ( &( (GPIOx)->DATAPIN0 ) ) + (PINx) ) = 0 )
    /** IO 置高(须保证对应 IO 为 GPIO 输出状态, 否则操作无效) */
    #define IO_SET_H(GPIOx, PINx)           ( *( ( &( (GPIOx)->DATAPIN0 ) ) + (PINx) ) = 1 )

#endif

#define H_RST()         IO_SET_H(LCD_GPIO_RST, LCD_PIN_RST)
#define L_RST()         IO_SET_L(LCD_GPIO_RST, LCD_PIN_RST)

#define H_CS()          IO_SET_H(LCD_GPIO_CS, LCD_PIN_CS)
#define L_CS()          IO_SET_L(LCD_GPIO_CS, LCD_PIN_CS)

#define H_SCK()         IO_SET_H(LCD_GPIO_SCK, LCD_PIN_SCK)
#define L_SCK()         IO_SET_L(LCD_GPIO_SCK, LCD_PIN_SCK)

#define H_SDA()         IO_SET_H(LCD_GPIO_SDA, LCD_PIN_SDA)
#define L_SDA()         IO_SET_L(LCD_GPIO_SDA, LCD_PIN_SDA)		
/**************************************************************************************************/

/***********************************RGB GPIO宏定义**************************************************/
    #define LCD_GPIO_RST                GPIOD
    #define LCD_PIN_RST                 PIN1

    #define LCD_GPIO_BL                 GPIOD
    #define LCD_PIN_BL                  PIN9

    #define LCD_PORT_BL                 PORTD
    #define LCD_PORT_FUNC_BL            PORTD_PIN9_PWM1B
    #define LCD_PWMN_BL                 PWM1
    #define LCD_PWMN_MSK_BL             PWM1_MSK
/**************************************************************************************************/




void IC_LCD_Init(void);
void EnterSleep (void);
void ExitSleep (void);

#endif //__KD034_ST7701S_H__
