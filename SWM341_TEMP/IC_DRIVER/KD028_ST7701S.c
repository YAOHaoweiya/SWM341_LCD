#include "SWM341.h"
#include "KD028_ST7701S.h"

#include "swm_delay.h"


#if defined(KD028_LCD)
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
//***************************************************************//LCD SETING
write_command(0xFF);   //PAGE3
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x13);

write_command(0xEF);
write_data(0x08);

write_command(0xFF);//PAGE01
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x10);

write_command(0xC0); //Display Line Setting   
write_data(0x4F);
write_data(0x00);

write_command(0xC1); //Porch Control  
write_data(0x10);
write_data(0x02);

write_command(0xC2);// Inversion set  
write_data(0x31); //31 2-DOT 37-Column
write_data(0x02);

write_command(0xCC);
write_data(0x10);

write_command(0xB0);//Positive Voltage Gamma Control 
write_data(0x00);
write_data(0x10);
write_data(0x17);
write_data(0x0D);
write_data(0x11);
write_data(0x06);
write_data(0x05);
write_data(0x08);
write_data(0x07);
write_data(0x1F);
write_data(0x04);
write_data(0x11);
write_data(0x0E);
write_data(0x29);
write_data(0x30);
write_data(0x1F);

write_command(0xB1); //Negative Voltage Gamma Control   
write_data(0x00);
write_data(0x0D);
write_data(0x14);
write_data(0x0E);
write_data(0x11);
write_data(0x06);
write_data(0x04);
write_data(0x08);
write_data(0x08);
write_data(0x20);
write_data(0x05);
write_data(0x13);
write_data(0x13);
write_data(0x26);
write_data(0x30);
write_data(0x1F);
 //*******power set********//
//PAGE1
write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x11);

write_command(0xB0);//Vop Amplitude setting  
write_data(0x65); //Vop

write_command(0xB1);//VCOM amplitude setting  
write_data(0x7C);

write_command(0xB2); //VGH Voltage setting  
write_data(0x87);//VGH=+15V

write_command(0xB3);//TEST Command Setting   
write_data(0x80);

write_command(0xB5); //VGL Voltage setting   
write_data(0x49); //VGL-10V

write_command(0xB7);//Power Control 1  
write_data(0x85);

write_command(0xB8); //Power Control 2    
write_data(0x20);  //AVDD=6.6 & AVCL=-4.6 

write_command(0xC1);//Source pre_drive timing set1
write_data(0x78);

write_command(0xC2);//Source EQ2 Setting  
write_data(0x78);

write_command(0xD0); //Source EQ2 Setting 
write_data(0x88);

write_command(0xEE);
write_data(0x42);
  //*********GIP SET*************//
write_command(0xE0);
write_data(0x00);
write_data(0x00);
write_data(0x02);

write_command(0xE1);
write_data(0x04);
write_data(0xA0);
write_data(0x06);
write_data(0xA0);
write_data(0x05);
write_data(0xA0);
write_data(0x07);
write_data(0xA0);
write_data(0x00);
write_data(0x44);
write_data(0x44);

write_command(0xE2);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x00);

write_command(0xE3);
write_data(0x00);
write_data(0x00);
write_data(0x22);
write_data(0x22);

write_command(0xE4);
write_data(0x44);
write_data(0x44);

write_command(0xE5);
write_data(0x0C);
write_data(0x90);
write_data(0xA0);
write_data(0xA0);
write_data(0x0E);
write_data(0x92);
write_data(0xA0);
write_data(0xA0);
write_data(0x08);
write_data(0x8C);
write_data(0xA0);
write_data(0xA0);
write_data(0x0A);
write_data(0x8E);
write_data(0xA0);
write_data(0xA0);

write_command(0xE6);
write_data(0x00);
write_data(0x00);
write_data(0x22);
write_data(0x22);

write_command(0xE7);
write_data(0x44);
write_data(0x44);

write_command(0xE8);
write_data(0x0D);
write_data(0x91);
write_data(0xA0);
write_data(0xA0);
write_data(0x0F);
write_data(0x93);
write_data(0xA0);
write_data(0xA0);
write_data(0x09);
write_data(0x8D);
write_data(0xA0);
write_data(0xA0);
write_data(0x0B);
write_data(0x8F);
write_data(0xA0);
write_data(0xA0);

write_command(0xEB);
write_data(0x00);
write_data(0x00);
write_data(0xE4);
write_data(0xE4);
write_data(0x44);
write_data(0x00);
write_data(0x00);

write_command(0xED);
write_data(0xFF);
write_data(0xF5);
write_data(0x47);
write_data(0x6F);//
write_data(0x0B);
write_data(0xA1);
write_data(0xAB);
write_data(0xFF);
write_data(0xFF);
write_data(0xBA);
write_data(0x1A);
write_data(0xB0);
write_data(0xF6);//
write_data(0x74);
write_data(0x5F);
write_data(0xFF);

write_command(0xEF);
write_data(0x08);
write_data(0x08);
write_data(0x08);
write_data(0x40);
write_data(0x3F);
write_data(0x64);

write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x00);

write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x13);

write_command(0xE6);
write_data(0x7C);

write_command(0xE8);
write_data(0x00);
write_data(0x0E);

write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x00);

write_command(0x11);
 delay_ms(120);

write_command(0xFF);
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x13);

write_command(0xE8);
write_data(0x00);
write_data(0x0C);
 delay_ms(10);
write_command(0xE8);
write_data(0x00);
write_data(0x00);

write_command(0xFF); //PAGE 0
write_data(0x77);
write_data(0x01);
write_data(0x00);
write_data(0x00);
write_data(0x00);
   																	
write_command(0x11);     
delay_ms(120); 

write_command(0xE8);     
write_data(0x00);   
write_data(0x00);   

write_command(0xFF);     
write_data(0x77);   
write_data(0x01);   
write_data(0x00);   
write_data(0x00);   
write_data(0x00);   

write_command(0x3A); 
write_data(0x70); //24 bit

write_command(0x29);     
 delay_ms(25);  
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

#endif //defined(KD028_LCD)