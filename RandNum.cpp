#include   "RandNum.h"
#ifndef    INCLUDE_ALGORITHM
#define    INCLUDE_ALGORITHM
#include   <algorithm>
#endif
#define    LENGTH 100000000
RandNum::RandNum(time_t* init /* =NULL */)
{ 
	srand(time(init)); // set the seed;
}
RandNum::~RandNum(){};
int RandNum::Rand(int N)
{// the basic random number generate funcion
	int BOUND=RAND_MAX-(RAND_MAX%N); // BOUND = k*N 是N的整数倍
	int re=rand();
	while(re>BOUND) // 如果大于BOUND那么重新生成随机数
		re=rand();
	return re%N;
}
UL RandNum::randTemp(int exp)
{
	UL re=0;
	while(exp>=4) //这里的4也可以换为其他的数字，但不要使得10^exp超过RAND_MAX就好，太小的话可能会有点慢
	{
		re*=pow(10,4);
		re+=Rand(10000);
		exp-=4;
	}
	re=re*(pow(10,exp))+Rand(pow(10,exp)); // 最后一个可能是0,1,2,3而不一定是4，所以需要特殊处理使得服从密度为1/pow(10,exp)的均匀分布;
	return re;
}
long RandNum::randLong(long upBound)
{
	int mtg=ceil(log10(upBound*1.0)); 
	long suN=pow(10,mtg);   // 简单地计算其上界
	long BOUND=suN-(suN%upBound);
	long re=randTemp(mtg);       
	//-------------------------类似于myrand()中的处理
	while(re>BOUND)
		re=randTemp(mtg);
	return re%upBound;
}
UL RandNum::randUL(UL upBound)
{
	int mtg=ceil(log10(upBound*1.0)); 
	UL suN=pow(10,mtg);   // 简单地计算其上界
	UL BOUND=suN-(suN%upBound);
	UL re=randTemp(mtg);       
	//-------------------------类似于myrand()中的处理
	while(re>BOUND)
		re=randTemp(mtg);
	return re%upBound;
}

uint32_t RandNum::randU32(uint32_t upBound)
{
	int mtg=ceil(log10(upBound*1.0)); 
	uint32_t suN=pow(10,mtg);   // 简单地计算其上界
	uint32_t BOUND=suN-(suN%upBound);
	uint32_t re=randTemp(mtg);       
	//-------------------------类似于myrand()中的处理
	puts("ssb");
	while(re>BOUND)
		re=randTemp(mtg);
	return re%upBound;
}
