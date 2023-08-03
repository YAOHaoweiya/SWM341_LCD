#ifndef __SCREEN_H__
#define __SCREEN_H__

/***********************************SPI GPIO�궨��************************************/
/* SWM34SVET6_Pin100 SPI�˿�ӳ��
 * ���ߣ�
			LCD              SWM341
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


/* ������ */
#if     0 /* �⺯�� */
    /** ��ȡ IO ����(�뱣֤��Ӧ IO Ϊ GPIO ����״̬, ���������Ч) */
    #define IO_GET(GPIOx, PINx)             GPIO_GetBits(GPIOx, PINx)
    /** IO �õ�(�뱣֤��Ӧ IO Ϊ GPIO ���״̬, ���������Ч) */
    #define IO_SET_L(GPIOx, PINx)           GPIO_AtomicClrBit(GPIOx, PINx)
    /** IO �ø�(�뱣֤��Ӧ IO Ϊ GPIO ���״̬, ���������Ч) */
    #define IO_SET_H(GPIOx, PINx)           GPIO_AtomicSetBit(GPIOx, PINx)

#else /* �Ĵ��� */
    /** ��ȡ IO ����(�뱣֤��Ӧ IO Ϊ GPIO ����״̬, ���������Ч) */
    #define IO_GET(GPIOx, PINx)             ( ( ( (GPIOx)->IDR ) >> (PINx) ) & 0x01 )
    /** IO �õ�(�뱣֤��Ӧ IO Ϊ GPIO ���״̬, ���������Ч) */
    #define IO_SET_L(GPIOx, PINx)           ( *( ( &( (GPIOx)->DATAPIN0 ) ) + (PINx) ) = 0 )
    /** IO �ø�(�뱣֤��Ӧ IO Ϊ GPIO ���״̬, ���������Ч) */
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

static void spi_gpio_config();
static void write_command (unsigned char y);
static void write_data(unsigned char w);
 void screen_init();

#endif 