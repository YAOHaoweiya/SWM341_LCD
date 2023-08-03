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
	


	/*ϵͳ��ʼ��*/
 	SystemInit();
	/*���Բ��ֳ�ʼ��*/
	SerialInit();
	printf("sys_start!\r\n");
	/*LCD RGB Ӧ�ó�ʼ��*/
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
	
	/*�ȴ�SD����FATFS��ʼ��*/
	for(int i = 0; i < SystemCoreClock/10; i++) __NOP();	// �ȴ� SD ����ɳ�ʼ��
	
	//ע��һ��������
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
		//��test.bmp
		printf("name=%s\r\n",jpeg);
		if(f_open(&filr,jpeg,FA_OPEN_EXISTING|FA_READ)==FR_OK)
		{;
			if(f_stat(jpeg, &fileInfo)==FR_OK) // ��ȡ�ļ���Ϣ
			{
				if(f_read(&filr,jpeg_image_sdr, fileInfo.fsize, &len)!=FR_OK)//��ȡbmp��buf
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
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
    unsigned unum;//���Ҫת���������ľ���ֵ,ת�������������Ǹ���
    int i=0,j,k;//i����ָʾ�����ַ�����Ӧλ��ת��֮��i��ʵ�����ַ����ĳ��ȣ�ת����˳��������ģ��������������k����ָʾ����˳��Ŀ�ʼλ��;j����ָʾ����˳��ʱ�Ľ�����
 
    //��ȡҪת���������ľ���ֵ
    if(radix==10&&num<0)//Ҫת����ʮ�����������Ǹ���
    {
        unum=(unsigned)-num;//��num�ľ���ֵ����unum
        str[i++]='-';//���ַ�����ǰ������Ϊ'-'�ţ�����������1
    }
    else unum=(unsigned)num;//����numΪ����ֱ�Ӹ�ֵ��unum
 
    //ת�����֣�ע��ת�����������
    do
    {
        str[i++]=index[unum%(unsigned)radix];//ȡunum�����һλ��������Ϊstr��Ӧλ��ָʾ������1
        unum/=radix;//unumȥ�����һλ
 
    }while(unum);//ֱ��unumΪ0�˳�ѭ��
 
    str[i]='\0';//���ַ���������'\0'�ַ���c�����ַ�����'\0'������
 
    //��˳���������
    if(str[0]=='-') k=1;//����Ǹ��������Ų��õ������ӷ��ź��濪ʼ����
    else k=0;//���Ǹ�����ȫ����Ҫ����
 
    char temp;//��ʱ��������������ֵʱ�õ�
    for(j=k;j<=(i-1)/2;j++)//ͷβһһ�Գƽ�����i��ʵ�����ַ����ĳ��ȣ��������ֵ�ȳ�����1
    {
        temp=str[j];//ͷ����ֵ����ʱ����
        str[j]=str[i-1+k-j];//β����ֵ��ͷ��
        str[i-1+k-j]=temp;//����ʱ������ֵ(��ʵ����֮ǰ��ͷ��ֵ)����β��
    }
 
    return str;//����ת������ַ���
 
}