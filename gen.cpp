#include "header.h"
#include "RandNum.h"
#include "omp.h"

uint32_t req[16777216];
uint8_t vec[16777216];

int main() {
	RandNum rn;
	FILE *fout = fopen("data.in", "wb");
#pragma omp parallel for
	for (int i = 0; i < 16777216; ++i) {
		req[i] = rn.randUL(4000000000);
		//printf("%d\n", i);
	}
	fwrite(req, 16777216 * sizeof(uint32_t), 1, fout);
	fclose(fout);

	fout = fopen("vec.in", "wb");
#pragma omp parallel for
	for (int i = 0; i < 16777216; ++i) {
		vec[i] = rn.randUL(2);
	}
	fwrite(vec, 16777216 * sizeof(uint8_t), 1, fout);
	fclose(fout);	
	return 0;
}
