#include "SWM341.h"
#include "KD034_ST7701S.h"

#include "swm_delay.h"

#if defined(KD034_LCD)

/****************************************************************************************************************************************** 
* 函数名称: spi_gpio_config()
* 功能说明:	spi gpio引脚配置
* 输    入: 无
* 输    出: 无
* 接线：
			KD028_LCD        SWM341
				SDI-------------PN4
				SCL-------------PN5
				 CS-------------PB0
			RESET-------------PD1
* 注意事项: 无
******************************************************************************************************************************************/
static void spi_gpio_config()
{
	/*spi gpio端口配置	（gpio端口，gpio引脚，1输出，1上拉使能，0下拉使能，开漏使能）*/
	GPIO_Init(LCD_GPIO_CS, LCD_PIN_CS, 1, 1, 0, 0);
	GPIO_Init(LCD_GPIO_SCK, LCD_PIN_SCK, 1, 1, 0, 0);
  GPIO_Init(LCD_GPIO_SDA, LCD_PIN_SDA, 1, 1, 0, 0);
  GPIO_Init(LCD_GPIO_RST, LCD_PIN_RST, 1, 1, 0, 0);
	
	H_CS();
  H_SCK();
  H_SDA();
}
/****************************************************************************************************************************************** 
* 函数名称: write_command (unsigned char y)
* 功能说明:	9bit spi 命令
* 输    入: y 命令数据
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/	
static void write_command (unsigned char y) // (uchar y,uchar x)
{
  unsigned char i;
	L_CS();
	L_SCK();
	L_SDA();
	H_SCK();
	for(i=0;i<8;i++)
	{
		L_SCK();      
		if (y&0x80)
		{
			H_SDA();
	  }
		else
		{
			L_SDA();
		}
		H_SCK();
		y=y<<1;
	}
	H_CS();
}
/****************************************************************************************************************************************** 
* 函数名称: write_data(unsigned char w)
* 功能说明:	9bit spi 数据
* 输    入: w 数据
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/	
static void write_data(unsigned char w) // (uchar w, uchar v)
{
  unsigned char i;
	L_CS();
	L_SCK();
	H_SDA();
	H_SCK();
	for(i=0;i<8;i++)
	{
	L_SCK();
		if (w&0x80)
		{
			H_SDA();
		}
		else
		{
			L_SDA();
		}
	 H_SCK();
	 w=w<<1;
	}
	H_CS();
}
/****************************************************************************************************************************************** 
* 函数名称: 	lcd_spi_init()
* 功能说明:	通过SPI初始化LCD为RGB888
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/	
static void lcd_spi_init()
{
	H_RST();
	delay_ms(1);
	L_RST();
	delay_ms(10);
	H_RST();
	delay_ms(120);
 //**************************************  
write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x13);

write_command(0xEF);
write_data(0x08);

write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x10);

write_command(0xC0);
write_data(0x3B);
write_data(0x00);

write_command(0xC1);
write_data(0x10);
write_data(0x0C);

write_command(0xC2);////Inversion selection
write_data(0x31); //31-2dot ,37-Column 
write_data(0x0A);

write_command(0xC3); //SET RGB MODE
write_data(0x02); //02-DE MODE ,82-HV MODE	 PCLK N
write_data(0x00);
write_data(0x00);

write_command(0xCC);
write_data(0x10);

write_command(0xCD);
write_data(0x08);
//**********GAMMA SET***************//
write_command(0xB0);
write_data(0x40);
write_data(0x0E);
write_data(0x58);
write_data(0x0E);
write_data(0x12);
write_data(0x08);
write_data(0x0C);
write_data(0x09);
write_data(0x09);
write_data(0x27);
write_data(0x07);
write_data(0x18);
write_data(0x15);
write_data(0x78);
write_data(0x26);
write_data(0xC7);

write_command(0xB1);
write_data(0x40);
write_data(0x13);
write_data(0x5B);
write_data(0x0D);
write_data(0x11);
write_data(0x06);
write_data(0x0A);
write_data(0x08);
write_data(0x08);
write_data(0x26);
write_data(0x03);
write_data(0x13);
write_data(0x12);
write_data(0x79);
write_data(0x28);
write_data(0xC9);
/*-----------------------------End Gamma Setting------------------------------*/
/*------------------------End Display Control setting-------------------------*/
/*-----------------------------Bank0 Setting  End-----------------------------*/
/*-------------------------------Bank1 Setting--------------------------------*/
/*--------------------- Power Control Registers Initial ----------------------*/
write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x11);

write_command(0xB0);
write_data(0x6D);//30
/*--------------------------------Vcom Setting--------------------------------*/
write_command(0xB1);
write_data(0x38);//VOCM
/*------------------------------End Vcom Setting------------------------------*/
write_command(0xB2);
write_data(0x81);//84

write_command(0xB3);
write_data(0x80);

write_command(0xB5);
write_data(0x4E);//4E

write_command(0xB7);
write_data(0x85);

write_command(0xB8);
write_data(0x20);

write_command(0xC1);
write_data(0x78);

write_command(0xC2);
write_data(0x78);

write_command(0xD0);
write_data(0x88);
/*--------------------End Power Control Registers Initial --------------------*/
//********GIP SET********************///
write_command(0xE0);
write_data(0x00);
write_data(0x00);
write_data(0x02);

write_command(0xE1);
write_data(0x06);
write_data(0x30);
write_data(0x08);
write_data(0x30);
write_data(0x05);
write_data(0x30);
write_data(0x07);
write_data(0x30);
write_data(0x00);
write_data(0x33);
write_data(0x33);

write_command(0xE2);
write_data(0x11);
write_data(0x11);
write_data(0x33);
write_data(0x33);
write_data(0xF4);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0xF4);
write_data(0x00);
write_data(0x00);
write_data(0x00);

write_command(0xE3);
write_data(0x00);
write_data(0x00);
write_data(0x11);
write_data(0x11);

write_command(0xE4);
write_data(0x44);
write_data(0x44);

write_command(0xE5);
write_data(0x0D);
write_data(0xF5);
write_data(0x30);
write_data(0xF0);
write_data(0x0F);
write_data(0xF7);
write_data(0x30);
write_data(0xF0);
write_data(0x09);
write_data(0xF1);
write_data(0x30);
write_data(0xF0);
write_data(0x0B);
write_data(0xF3);
write_data(0x30);
write_data(0xF0);

write_command(0xE6);
write_data(0x00);
write_data(0x00);
write_data(0x11);
write_data(0x11);

write_command(0xE7);
write_data(0x44);
write_data(0x44);

write_command(0xE8);
write_data(0x0C);
write_data(0xF4);
write_data(0x30);
write_data(0xF0);
write_data(0x0E);
write_data(0xF6);
write_data(0x30);
write_data(0xF0);
write_data(0x08);
write_data(0xF0);
write_data(0x30);
write_data(0xF0);
write_data(0x0A);
write_data(0xF2);
write_data(0x30);
write_data(0xF0);

write_command(0xE9);
write_data(0x36);
write_data(0x01);

write_command(0xEB);
write_data(0x00);
write_data(0x01);
write_data(0xE4);
write_data(0xE4);
write_data(0x44);
write_data(0x88);
write_data(0x40);

write_command(0xED);
write_data(0xFF);
write_data(0x45);
write_data(0x67);
write_data(0xFA);
write_data(0x01);
write_data(0x2B);
write_data(0xCF);
write_data(0xFF);
write_data(0xFF);
write_data(0xFC);
write_data(0xB2);
write_data(0x10);
write_data(0xAF);
write_data(0x76);
write_data(0x54);
write_data(0xFF);

write_command(0xEF);
write_data(0x10);
write_data(0x0D);
write_data(0x04);
write_data(0x08);
write_data(0x3F);
write_data(0x1F);

write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x00);

write_command(0x3A);
write_data(0x70);

write_command(0x11);
delay_ms(120);

write_command(0x29);
delay_ms(25);
write_command(0x35);
write_data(0x00);
}

/****************************************************************************************************************************************** 
* 函数名称: 	KD028LCD_Init(void)
* 功能说明:	通过 SPI 通讯初始化屏幕KD028LCD，RGB88
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/	
void IC_LCD_Init(void)
{
	delay_init();
	spi_gpio_config();//模拟spi gpio配置
	lcd_spi_init();//先使用spi配置LCD
}
//*******************************************
void EnterSleep (void)
{
    write_command(0x28);
    delay_ms(10);
    write_command(0x10);
  
  }

//*********************************************************
void ExitSleep (void)

 {
    write_command(0x11);
    delay_ms(120);
    write_command(0x29);

   }
#endif //defined(KD034_LCD)
