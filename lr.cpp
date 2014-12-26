#include "header.h"
#include "tools.h"

const int LOOP = 1000000;

int64_t buf[32768];

void *worker1(void *arg) {
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);

	for (int i = 0; i < 32768; i += 8) {
		buf[i] = i;
	}
}

void *worker2(void *arg) {
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(1, &mask);
	pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);

	int j;
	for (int i = 0; i < 32768; i += 8) {
		j = buf[i];
	}
	printf("%d\n", j);
}

int main() {
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, worker1, NULL);
	pthread_join(tid1, 0);
	double beg = dtime();
	pthread_create(&tid2, NULL, worker2, NULL);
	pthread_join(tid2, 0);
	printf("time cost: %lf\n", dtime() - beg);
	return 0;
}
