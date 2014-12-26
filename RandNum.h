#pragma    once
#ifndef    RANDNUM_H
#define    RANDNUM_H
#include "header.h"

#ifndef    INCLUDE_IOSTREAM
#define    INCLUDE_IOSTREAM
#include   <iostream>
#endif
#ifndef    INCLUDE_CTIME
#define    INCLUDE_CTIME
#include   <ctime>
#endif
#ifndef    INCLUDE_CMATH
#define    INCLUDE_CMATH
#include   <cmath>
#endif
#ifndef    INCLUDE_CSTDLIB
#define    CSTDLIB
#include   <cstdlib>
#endif
#ifndef    UL
#define    UL unsigned long
#endif
class	RandNum
{
	public:
		RandNum(time_t* init=NULL);
		int Rand(int N); // 基本的随机数生成函数0~N-1
		~RandNum();
		long randLong(long upBound); //大的随机数生产函数，均匀0~upBound-1，当使用long long 类型时可生成范围会更大（只要该一下类型就好）
		UL randUL(UL upBound);
		uint32_t randU32(uint32_t upBound);
	private:
		UL randTemp(int exp);//随机数生成函数0~10^exp-1,这是作为private 成员，是生成其他大随机数的基础
};
#endif    /*RANDNUM_H*/
