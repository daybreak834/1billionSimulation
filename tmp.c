#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// faster random function from https://stackoverflow.com/questions/26237419/faster-than-rand
// but changed range from [0, 32767] to [0, UINT_MAX]
unsigned int g_seed;
inline void fast_srand(int seed) { g_seed = seed; }
inline unsigned int fast_rand(void) { return g_seed = 214013*g_seed+2531011; }

// bitcounting magic from http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive
unsigned int bitcount(unsigned int v){
	v = v - ((v >> 1) & 0x55555555);
	v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
	return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

int main(){
	clock_t start = clock(), diff;
	fast_srand(time(NULL));
	int Ones;
	int rolls = 1000000000;
	int maxOnes = 0;

	for(;Ones < 177 && rolls; rolls--){
		Ones = 0;
		// for each 231 cycle two 231 bit numbers are generated
		// each bit in position 1 to 231 represents the outcome
		// two numbers are needed to simulate a 1/4th chance (0 and 0, 0 and 1, 1 and 0 and 1 and 1) 
		// if the nth-position in both numbers is 1 paralysis took effect that turn
		Ones += bitcount(fast_rand() & fast_rand());
		Ones += bitcount(fast_rand() & fast_rand());
		Ones += bitcount(fast_rand() & fast_rand());
		Ones += bitcount(fast_rand() & fast_rand());
		Ones += bitcount(fast_rand() & fast_rand());
		Ones += bitcount(fast_rand() & fast_rand());
		Ones += bitcount(fast_rand() & fast_rand());
		Ones += bitcount((fast_rand() & fast_rand()) & 127);	// 231 is not devisible by 32 so these are the last 7 bit

		maxOnes = maxOnes < Ones ? Ones : maxOnes;
	}
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

	printf("Higest Ones Roll: %d\n",maxOnes);
	printf("Number of Roll Sessions: %d\n",1000000000 - rolls);
}