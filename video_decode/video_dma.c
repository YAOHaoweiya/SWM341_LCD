
#include "SWM341.h"
uint8_t frameup=0;          //avi一帧结束标志
uint8_t dac_dma=1;          //dac dma传输完成标志


void dac_init(void)
{
    PORT_Init(PORTD, PIN2, PORTD_PIN2_DAC_OUT, 0);
    DAC_Init(DAC, DAC_FORMAT_MSB12B); //DHR高12位传输给DOR
    /* 测试设置 */
    SYS->DACCR &= ~(0x0F << 2);
    SYS->DACCR |= ~(0x14 << 2);

    DAC_Open(DAC);
}


void dac_dma_init(uint32_t addr, uint32_t count)
{
    DMA_InitStructure DMA_initStruct;

    DMA_initStruct.Mode = DMA_MODE_SINGLE;
    DMA_initStruct.Unit = DMA_UNIT_HALFWORD;
    DMA_initStruct.Count = count;
    DMA_initStruct.SrcAddr = addr;
    DMA_initStruct.SrcAddrInc = 1;
    DMA_initStruct.DstAddr = (uint32_t)&DAC->DHR;
    DMA_initStruct.DstAddrInc = 0;
    DMA_initStruct.Priority = DMA_PRI_LOW;
    DMA_initStruct.INTEn = 1;

    DMA_initStruct.Handshake = DMA_EXHS_TIMR0;

    DMA_CH_Init(DMA_CH0, &DMA_initStruct);
    DMA_CH_Open(DMA_CH0);
}


void dac_deinit(void)
{
    DMA_CH_Close(DMA_CH0);
    DAC_Close(DAC);
    TIMR_Stop(TIMR0);
}


void JPEG_Handler(void)
{
    if(JPEG->IR & JPEG_IR_IFERROR_Msk)
    {
        JPEG->IR |= 1 << JPEG_IR_ICERROR_Pos;
        JPEG->CR |= 1 << JPEG_CR_RESET_Pos;
    }
}

void BTIMR0_Handler(void)
{
    TIMR_INTClr(BTIMR0);
    frameup = 1;
}

void DMA_Handler(void)
{
    if (DMA_CH_INTStat(DMA_CH0,DMA_IT_DONE))
    {
        DMA_CH_INTClr(DMA_CH0,DMA_IT_DONE);
        dac_dma = 1;
    }
}