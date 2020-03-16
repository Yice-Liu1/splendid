#include "main.h"

void LostCounterFeed(LostCounter_t *lc)
{
	*lc=0;
}

void LostCounterCount(LostCounter_t *lc, uint32_t os_ticks)
{
	*lc+=os_ticks;
}

uint8_t LostCounterOverflowCheck(LostCounter_t lc, uint32_t threshold)
{
	if(lc<threshold) return 0;
	
	else          return 1;
}
