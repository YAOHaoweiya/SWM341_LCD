#ifndef __SWM_DELAY_H__
#define __SWM_DELAY_H__


/* delay macros */
#define STEP_DELAY_MS                    50
/* delay variable */
static __IO uint32_t fac_us;
static __IO uint32_t fac_ms;

/* delay function */
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_sec(uint16_t sec);



#endif //__SWM_DELAY_H__