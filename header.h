#pragma once 

#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <queue>
#include <map>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <assert.h>
#include <iostream>
#include <ifaddrs.h>

using namespace std;

#define BUFF_SIZE 131072
#define UL unsigned long
#define HWT_NUM 236
#define CACHE_SIZE (128 * 1024)
#define FILL_RATE (0.2)

struct req_msg {
	uint32_t idx;
	int rwd;
};
