#include "main.h"
#include "jfif_parser.h"
#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FATFS fatfs;

uint8_t  *jpeg_image_sdr = (uint8_t *)(SDRAMM_BASE + 0x100000);

jfif_info_t jfif_info;

char* itoa(int num,char* str,int radix);

int main(void)
{
	uint32_t len;
	FRESULT  res;
	FIL filw, filr;
	FILINFO fileInfo;
	


	/*系统初始化*/
 	SystemInit();
	/*调试部分初始化*/
	SerialInit();
	printf("sys_start!\r\n");
	/*LCD RGB 应用初始化*/
	GPIO_Init(GPIOD,PIN9, 1, 1, 0, 0);
	IO_SET_H(GPIOD,PIN9);
	
	printf("Meory init success.\r\n");
	IC_LCD_Init();
	printf("KD028 init success.\r\n");
	RGBLCDInit();
	LCD_Start(LCD);
	swm_dma2d_init();
	
	//jpeg
	jpeg_outset_t jpeg_outset;
	JPEG_InitStructure JPEG_initStruct;
	
	JPEG_initStruct.DoneIEn = 0;
	JPEG_initStruct.ErrorIEn = 0;
	JPEG_Init(JPEG, &JPEG_initStruct);
	
	/*等待SD卡及FATFS初始化*/
	for(int i = 0; i < SystemCoreClock/10; i++) __NOP();	// 等待 SD 卡完成初始化
	
	//注册一个工作区
	if(f_mount(&fatfs,"sd:",1)==FR_OK)
	{
		
		printf("sd init success.\r\n");
	}
	else
	{
		printf("sd init fail.\r\n");
	}
	
  int num=0;
 	while(1==1)
 	{	
		char snum[]="0";
		char *sd="sd:";
		char *last=".jpg";
			
		
		itoa(num,snum,10);
		
		char *jpeg=(char *) malloc(strlen(sd) + strlen(snum)+strlen(last));
	  strcpy(jpeg, sd);
    strcat(jpeg, snum);	
		strcat(jpeg, last);	
		//strcpy (jpeg,snum);
		num++;
		if(num>9)
			num=0;
		//打开test.bmp
		printf("name=%s\r\n",jpeg);
		if(f_open(&filr,jpeg,FA_OPEN_EXISTING|FA_READ)==FR_OK)
		{;
			if(f_stat(jpeg, &fileInfo)==FR_OK) // 获取文件信息
			{
				if(f_read(&filr,jpeg_image_sdr, fileInfo.fsize, &len)!=FR_OK)//读取bmp到buf
				{
					printf("read ");
					printf("%s",jpeg);
					printf(" fail");
				}
			}
			else
			{
				printf("len=%d\r\n",len);
				printf("finfo %s fail\r\n",jpeg);
			}
			f_close(&filr);

		}
		else
		{
			printf("open ");
			printf("%s",jpeg);
			printf(" fail");
		}
	
		jfif_parse(jpeg_image_sdr,fileInfo.fsize, &jfif_info);
		jpeg_outset.format = JPEG_OUT_RGB565;
		jpeg_outset.dither = 0;
		jpeg_outset.RGBAddr = SDRAMM_BASE;
		JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
		while(JPEG_DecodeBusy(JPEG)) __NOP();
		//dma2d_test_show(&jpeg_outset);
		
		delay_ms(1000);
 	}
	
}

char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整
 
    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}