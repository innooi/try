#include "tools.h"

string NumToStr(uint32_t x) {
	string tmp = "";
	if(x == 0) return tmp = "0";
	while(x != 0){
		tmp += x % 10;
		x /= 10;
	}
	string ret(tmp.rbegin(), tmp.rend());
	return ret;
}

uint32_t StrToNum(char *str) {
	uint32_t ret = 0;
	while(str) {
		ret *= 10;
		ret += (*str - '0');
	}
	return ret;
}

inline uint32_t Hash(uint32_t key) {
	
	return 0;	
}

double dtime() {
    double tseconds = 0.0;
    struct timeval mytime;
    gettimeofday(&mytime,(struct timezone*)0);
    tseconds = (double)(mytime.tv_sec + mytime.tv_usec*1.0e-6);
    return( tseconds );
}
