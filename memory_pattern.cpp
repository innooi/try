#include "header.h"
#include "tools.h"
#include "RandNum.h"
#include "treap.h"
#include "memory_pattern.h"

#define ITER_TIMES 50000

RandNum *pRN;

uint32_t coreNum;
uint32_t privateCacheLineNum;
uint32_t dataLineNum;
uint32_t perWrite;
Treap<uint32_t, CacheState> *privateCache;
CacheState *overallState;
uint32_t *overallCount;
uint32_t *overallOwner;

uint32_t *privateModNum;
uint32_t *privateExcNum;
uint32_t *privateShaNum;
uint32_t *privateInvNum;

uint32_t privateHitNum;
uint32_t overallRefNum;
uint32_t remoteHitNum;

uint32_t RFONum;
uint32_t evictNum;

uint32_t GetAdd() {
	return (uint32_t) pRN->randLong(dataLineNum);
}

uint32_t RandLine(uint32_t BOUND) {
	return (uint32_t) pRN->randLong(BOUND);
}

uint8_t ROW() {
	uint32_t p = (uint32_t) pRN->randLong(1000);
	if (p < perWrite) return 1;
	return 0;
}

/*
   An Eviction opearation
   returned value = 0 when no cache line need to be evicted
   = 1 when an valid cache line is evicted
   */
uint32_t Evict(uint32_t	coreID) {
	uint32_t nVal = privateCache[coreID].size();
	//need an eviction
	if (nVal == privateCacheLineNum) {
		uint32_t randLine = RandLine(nVal) + 1;
		Treap<uint32_t, CacheState>::Node *getNode = 
			privateCache[coreID]._kth(privateCache[coreID].root, randLine);
		uint32_t key = getNode->key;
		CacheState val = getNode->val;
		switch (val) {
			case MOD:
				privateModNum[coreID] -= 1;
				break;
			case EXC:
				privateExcNum[coreID] -= 1;
				break;
			case SHA:
				privateShaNum[coreID] -= 1;
				break;
			case INV:
				//privateInvNum[coreID] -= 1;
				break;
		}
		privateCache[coreID].del(key);

		overallCount[key] -= 1;
		if (overallCount[key] == 0) {
			overallState[key] = INV;
		}
		return 1;
	}
	return 0;
}

/*
   Invalid a cache line over all cores
   2ed parameter -1 means sending a invalid broadcast to all cores
   */
void Invalid(uint32_t fromCoreId, uint32_t toCoreID, uint32_t addr)
{
	if (toCoreID == -1) {
		//puts("happend");
		for (uint32_t i = 0; i < coreNum; ++i) {
			if (overallCount[addr] == 0) {
				break;
			}
			if (i == fromCoreId) {
				continue;
			}
			if (privateCache[i].in(addr)) {
				CacheState val = privateCache[i][addr];
				switch (val) {
					case MOD:
						privateModNum[i] -= 1;
						break;
					case EXC:
						privateExcNum[i] -= 1;
						break;
					case SHA:
						privateShaNum[i] -= 1;
						break;
					case INV:
						//privateInvNum[i] -= 1;
						break;
				}
				//privateInvNum[i] += 1;

				privateCache[i].del(addr);
				overallCount[addr] -= 1;
			}
		}
	}
	else {
		CacheState val = privateCache[toCoreID][addr];
		switch (val) {
			case MOD:
				privateModNum[toCoreID] -= 1;
				break;
			case EXC:
				privateExcNum[toCoreID] -= 1;
				break;
			case SHA:
				privateShaNum[toCoreID] -= 1;
				break;
			case INV:
				//privateInvNum[toCoreID] -= 1;
				break;
		}
		//privateInvNum[toCoreID] += 1;
		privateCache[toCoreID].del(addr);
	}
}

int main(int argc, char **argv) {
	coreNum 			= argc > 1 ? atoi(argv[1]) : 59;
	privateCacheLineNum = argc > 2 ? atoi(argv[2]) : 8192;
	dataLineNum 		= argc > 3 ? atoi(argv[3]) : 483328;
	perWrite 			= argc > 4 ? atoi(argv[4]) : 330;

	cout << coreNum << endl;
	cout << privateCacheLineNum << endl;
	cout << dataLineNum << endl;
	cout << perWrite << endl;

	pRN = new RandNum();

	overallState = (CacheState *) malloc(dataLineNum * sizeof(CacheState));
	overallCount = (uint32_t *) malloc(dataLineNum * sizeof(uint32_t));
	overallOwner = (uint32_t *) malloc(dataLineNum * sizeof(uint32_t));

	for (uint32_t i = 0; i < dataLineNum; ++i) {
		overallState[i] = INV;
		overallCount[i] = 0;
		overallOwner[i] = -1;
	}

	privateCache = new Treap<uint32_t, CacheState>[coreNum]; 

	for (int i = 0; i < coreNum; ++i) {
		privateCache[i].clear();
	}

	privateModNum = (uint32_t *) calloc(privateCacheLineNum, sizeof(uint32_t));
	privateExcNum = (uint32_t *) calloc(privateCacheLineNum, sizeof(uint32_t));
	privateShaNum = (uint32_t *) calloc(privateCacheLineNum, sizeof(uint32_t));
	//privateInvNum = (uint32_t *) calloc(privateCacheLineNum, sizeof(uint32_t));

	privateHitNum = 0;
	overallRefNum = 0;
	remoteHitNum = 0;

	RFONum = 0;
	evictNum = 0;

	double beg_time = dtime();

	printf("M\tE\tS\tI\tSUM\n");

	for (uint32_t iter = 0; iter < ITER_TIMES; ++iter) {
		for (uint32_t i = 0; i < coreNum; ++i) {
			/*
			   printf("%u\t%u\t%u\t%u\t%u\n", privateModNum[0], privateExcNum[0],
			   privateShaNum[0], 
			   privateCacheLineNum - 
			   (privateModNum[0]+privateExcNum[0]+privateShaNum[0]),
			   privateModNum[0]+privateExcNum[0]+privateShaNum[0]);
			   */
			/*
			   printf ("%u\t%u\t%u\n", 
			   privateCacheInv[0].size(), 
			   privateCacheVal[0].size(), 
			   privateCacheInv[0].size() + privateCacheVal[0].size());
			   */
			uint32_t add = GetAdd();
			uint8_t row = ROW();
			overallRefNum += 1;
			//cout << add << " " << overallState[add] << endl;
			switch (overallState[add]) {
				case MOD:
					//read a modified cache line
					if (row == 0) {
						//private cache read miss, remote mod cache read hit
						if (privateCache[i].in(add) != true) {
							RFONum += 1;
							if ( Evict(i) == 1 ) {
								evictNum += 1;
							}

							privateShaNum[i] += 1;
							privateShaNum[overallOwner[add]] += 1;
							privateModNum[overallOwner[add]] -= 1;

							privateCache[i][add] = SHA;
							privateCache[overallOwner[add]][add] = SHA;
							overallState[add] = SHA;
							overallCount[add] += 1;

							remoteHitNum += 1;
						}
						//hit
						else {
							privateHitNum += 1;
						}
					}
					//write a modified cache line
					else {
						/*
						   private cache write miss, 
						   a RFO operation on remote cache
						   */
						if (privateCache[i].in(add) != true) {
							RFONum += 1;
							if ( Evict(i) == 1 ) {
								evictNum += 1;
							}
							Invalid(i, overallOwner[add], add);
							privateModNum[i] += 1;

							privateCache[i][add] = MOD;
							overallOwner[add] = i;
							overallState[add] = MOD;
							overallCount[add] = 1;

							remoteHitNum += 1;
						}
						//hit
						else {
							privateHitNum += 1;
						}
					}
					break;

				case EXC:
					//read an exclusive cache line
					if (row == 0) {
						//private cache read miss, remote exc cache read hit
						if (privateCache[i].in(add) != true) {
							RFONum += 1;
							if ( Evict(i) == 1 ) {
								evictNum += 1;
							}
							privateShaNum[i] += 1;
							privateShaNum[overallOwner[add]] += 1;
							privateExcNum[overallOwner[add]] -= 1;

							privateCache[i][add] = SHA;
							privateCache[overallOwner[add]][add] = SHA;
							overallState[add] = SHA;
							overallCount[add] += 1;

							remoteHitNum += 1;

						}
						//hit
						else {
							privateHitNum += 1;
						}
					}
					else //write an exclusive cache line
					{
						//private cache read hit
						if (privateCache[i].in(add) == true) {
							privateModNum[i] += 1;
							privateExcNum[i] -= 1;

							privateCache[i][add] = MOD;
							overallOwner[add] = i;
							overallState[add] = MOD;
							overallCount[add] = 1;

							privateHitNum += 1;
						}
						/*
						   private cache read miss, 
						   a RFO opearation on remote cache
						   */
						else {
							RFONum += 1;
							if ( Evict(i) == 1 ) {
								evictNum += 1;
							}
							Invalid(i, overallOwner[add], add);
							privateModNum[i] += 1;

							privateCache[i][add] = MOD;
							overallOwner[add] = i;
							overallState[add] = MOD;
							overallCount[add] = 1;

							remoteHitNum += 1;
						}
					}
					break;

				case SHA:
					//read a share cache line
					if (row == 0) {
						//private cache miss
						if (privateCache[i].in(add) != true) {
							RFONum += 1;
							if ( Evict(i) == 1 ) {
								evictNum += 1;
							}
							privateShaNum[i] += 1;

							privateCache[i][add] = SHA;
							overallCount[add] += 1;

							remoteHitNum += 1;
						}
						//hit
						else {
							privateHitNum += 1;
						}
					}
					//write a share cache line
					else {
						//private cache hit, invalids others share cache lines
						if (privateCache[i].in(add) == true) {
							Invalid(i, -1, add);
							privateShaNum[i] -= 1;
							privateModNum[i] += 1;

							privateCache[i][add] = MOD;
							overallOwner[add] = i;
							overallState[add] = MOD;
							overallCount[add] = 1;

							privateHitNum += 1;
						}
						/*
						   private cache miss, a RFO operation to owner and 
						   invalids others share cache line
						   */
						else {
							RFONum += 1;
							if ( Evict(i) == 1 ) {
								evictNum += 1;
							}
							Invalid(i, -1, add);
							privateModNum[i] += 1;

							privateCache[i][add] = MOD;
							overallOwner[add] = i;
							overallState[add] = MOD;
							overallCount[add] = 1;

							remoteHitNum += 1;
						}
					}
					break;

				case INV: //an overvall cache miss
					RFONum += 1;
					if ( Evict(i) == 1 ) {
						evictNum += 1;
					}
					if (row == 0) {
						privateExcNum[i] += 1;
					}
					else{
						privateModNum[i] += 1;
					}

					privateCache[i][add] = (row == 0 ? EXC : MOD);
					overallOwner[add] = i;
					overallState[add] = (row == 0 ? EXC : MOD);
					overallCount[add] = 1;

					break;
			}
		}
	}
	printf("time cost: %lf\n", dtime() - beg_time);


	printf("M\tE\tS\tI\tSUM\n");
	for (uint32_t i = 0; i < coreNum; ++i) {
		printf("%u\t%u\t%u\t%u\n", privateModNum[i], privateExcNum[i],
				privateShaNum[i], privateModNum[i]+privateExcNum[i]+privateShaNum[i]);
	}

	printf("private hit ratio: %lf, remote hit ratio: %lf, miss ratio: %lf\n", 
			1.0 * privateHitNum / overallRefNum, 1.0 * remoteHitNum / overallRefNum, 
			1.0 * (overallRefNum - privateHitNum - remoteHitNum) / overallRefNum);

	printf("RFONum = %u,\t evictNum = %u,\t ratio = %lf\n", RFONum, evictNum, 1.0 * evictNum / RFONum);

	return 0;
}
