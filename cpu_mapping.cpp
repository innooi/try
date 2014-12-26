#include "cpu_mapping.h"

int getNextHWT(){

//#ifdef MIC //on Xeon Phi 5110p with 60 Cores and 240 Hardware Threads
	static int id = 1;
	int ret = id;
	id += 4;
	if (id > 236) {
		if (id % 4 == 1) {
			id = 2;
		}
		else if (id % 4 == 2) {
			id = 3;
		}
		else if (id % 4 == 3) {
			id = 4;
		}
		else if (id % 4 == 0) {
			id = 4;
		}
	}
	return ret;
/*
#else //on 2 x E5 2620v2 with 12 Cores and 24 Hardware Threads
	static int id = 0;
	id %= 24;
	return (id++);

#endif
*/
}
