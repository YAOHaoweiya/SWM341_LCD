#ifndef __SWM341_CORDIC_H__
#define __SWM341_CORDIC_H__

#define CORDIC_PI	3.141592653589793

void CORDIC_Init(CORDIC_TypeDef * CORDICx);


/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Sin()
* 功能说明:	使用CORDIC模块计算sin
* 输    入: uint32_t radian		要计算sin值的角度，以弧度为单位，取值范围0.01~1.56；但需要将弧度值乘以16384作为参数调用此函数
* 输    出: 无
* 注意事项: 需要将弧度值乘以16384作为参数调用此函数
******************************************************************************************************************************************/
static __INLINE void CORDIC_Sin(uint32_t radian)
{
	CORDIC->INPUT = radian;
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (1 << CORDIC_CMD_SINCOS_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Sin_IsDone()
* 功能说明:	CORDIC模块sin运算是否完成
* 输    入: 无
* 输    出: uint32_t		0 sin运算未完成    1 sin运算已完成，可取出运算结果
* 注意事项: 
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Sin_IsDone(void)
{
	return (CORDIC->SIN & CORDIC_SIN_DONE_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Sin_Result()
* 功能说明:	取出CORDIC模块sin运算结果
* 输    入: 无
* 输    出: uint32_t		sin运算结果，Result[15:14]表示整数部分，Result[13:0]表示小数部分
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Sin_Result(void)
{
	return CORDIC->SIN & (CORDIC_SIN_F_Msk | CORDIC_SIN_I_Msk);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Cos()
* 功能说明:	使用CORDIC模块计算cos
* 输    入: uint32_t radian		要计算cos值的角度，以弧度为单位，取值范围0.01~1.56；但需要将弧度值乘以16384作为参数调用此函数
* 输    出: 无
* 注意事项: 需要将弧度值乘以16384作为参数调用此函数
******************************************************************************************************************************************/
static __INLINE void CORDIC_Cos(uint32_t radian)
{
	CORDIC->INPUT = radian;
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (1 << CORDIC_CMD_SINCOS_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Cos_IsDone()
* 功能说明:	CORDIC模块cos运算是否完成
* 输    入: 无
* 输    出: uint32_t		0 cos运算未完成    1 cos运算已完成，可取出运算结果
* 注意事项: 
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Cos_IsDone(void)
{
	return (CORDIC->COS & CORDIC_COS_DONE_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Cos_Result()
* 功能说明:	取出CORDIC模块cos运算结果
* 输    入: 无
* 输    出: uint32_t		cos运算结果，Result[15:14]表示整数部分，Result[13:0]表示小数部分
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Cos_Result(void)
{
	return CORDIC->COS & (CORDIC_COS_F_Msk | CORDIC_COS_I_Msk);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Arctan()
* 功能说明:	使用CORDIC模块计算arctan
* 输    入: uint32_t input		取值范围0.05~10000，但需要将其乘以16384作为参数调用此函数
* 输    出: 无
* 注意事项: 需要将待计算值乘以16384作为参数调用此函数
******************************************************************************************************************************************/
static __INLINE void CORDIC_Arctan(uint32_t input)
{
	if((input > 819) && (input <= 8192))
	{
		CORDIC->INPUT = input * 2;
		
		CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (0 << CORDIC_CMD_SINCOS_Pos) | (0 << CORDIC_CMD_RANGE_Pos);
	}
	else if((input > 8192) && (input <= 32768))
	{
		CORDIC->INPUT = input;
		
		CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (0 << CORDIC_CMD_SINCOS_Pos) | (1 << CORDIC_CMD_RANGE_Pos);
	}
	else if((input > 32768) && (input < 10000*16384))
	{
		CORDIC->INPUT = 32768*16384/input;
		
		CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (0 << CORDIC_CMD_SINCOS_Pos) | (2 << CORDIC_CMD_RANGE_Pos);
	}
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Arctan_IsDone()
* 功能说明:	CORDIC模块arctan运算是否完成
* 输    入: 无
* 输    出: uint32_t		0 arctan运算未完成    1 arctan运算已完成，可取出运算结果
* 注意事项: 
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Arctan_IsDone(void)
{
	return (CORDIC->ARCTAN & CORDIC_ARCTAN_DONE_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Arctan_Result()
* 功能说明:	取出CORDIC模块arctan运算结果
* 输    入: 无
* 输    出: uint32_t		arctan运算结果，为弧度值，Result[15:14]表示整数部分，Result[13:0]表示小数部分
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Arctan_Result(void)
{
	return CORDIC->ARCTAN & (CORDIC_ARCTAN_F_Msk | CORDIC_ARCTAN_I_Msk);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Mul()
* 功能说明:	使用CORDIC模块计算乘法
* 输    入: uint16_t mul1	乘数1
*			uint16_t mul2	乘数2
* 输    出: 无
* 注意事项: 需要将乘数乘以16384作为参数调用此函数
******************************************************************************************************************************************/
static __INLINE void CORDIC_Mul(uint16_t mul1, uint16_t mul2)
{
	CORDIC->INPUT = mul1 | (mul2 << 16);
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (3 << CORDIC_CMD_MULDIV_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Mul_IsDone()
* 功能说明:	CORDIC模块乘法运算是否完成
* 输    入: 无
* 输    出: uint32_t		0 乘法运算未完成    1 乘法运算已完成，可取出运算结果
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Mul_IsDone(void)
{
	return (CORDIC->SIN & CORDIC_SIN_DONE_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Mul_Result()
* 功能说明:	取出CORDIC模块乘法运算结果
* 输    入: 无
* 输    出: uint32_t		乘法运算结果，Result[15:14]表示整数部分，Result[13:0]表示小数部分
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Mul_Result(void)
{
	return CORDIC->SIN & (CORDIC_SIN_F_Msk | CORDIC_SIN_I_Msk);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Div()
* 功能说明:	使用CORDIC模块计算除法
* 输    入: uint16_t dividend	被除数
*			uint16_t divisor	除数
* 输    出: 无
* 注意事项: 需要将除数和被除数值乘以16384作为参数调用此函数
******************************************************************************************************************************************/
static __INLINE void CORDIC_Div(uint16_t dividend, uint16_t divisor)
{
	CORDIC->INPUT = divisor | (dividend << 16);
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (2 << CORDIC_CMD_MULDIV_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Div_IsDone()
* 功能说明:	CORDIC模块除法运算是否完成
* 输    入: 无
* 输    出: uint32_t		0 除法运算未完成    1 除法运算已完成，可取出运算结果
* 注意事项: 
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Div_IsDone(void)
{
	return (CORDIC->ARCTAN & CORDIC_ARCTAN_DONE_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Div_Result()
* 功能说明:	取出CORDIC模块除法运算结果
* 输    入: 无
* 输    出: uint32_t		除法运算结果，为弧度值，Result[15:14]表示整数部分，Result[13:0]表示小数部分
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Div_Result(void)
{
	return CORDIC->ARCTAN & (CORDIC_ARCTAN_F_Msk | CORDIC_ARCTAN_I_Msk);
}

#endif //__SWM341_CORDIC_H__
