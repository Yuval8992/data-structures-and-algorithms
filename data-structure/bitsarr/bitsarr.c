#include <assert.h> /*assert*/

#include "bitsarr.h"

#define NUM_BITS_IN_CHAR 8
#define BUFF_SIZE (sizeof(bitsarr_t) * NUM_BITS_IN_CHAR)
#define ASCII_CONVERT 48
#define UNUSED(x) (void)(x)

/**************************BitsArrSetOn***************************/
bitsarr_t BitsArrSetOn(bitsarr_t barr, size_t idx)
{
	return (barr | ((size_t)1 << idx)) ;
}

/**************************BitsArrSetOff**************************/
bitsarr_t BitsArrSetOff(bitsarr_t barr, size_t idx)
{
	return (barr & (~(size_t)0 ^ ((size_t)1 << idx)));
}

/**************************BitsArrSetAll**************************/
bitsarr_t BitsArrSetAll(bitsarr_t barr)
{
	barr = ~0;
	return barr;
}

/**************************BitsArrResetAll**************************/
bitsarr_t BitsArrResetAll(bitsarr_t barr)
{
	barr = 0;
	return barr;
}

/**************************BitsArrSetBit**************************/
bitsarr_t BitsArrSetBit(bitsarr_t barr, size_t idx, int boolean_value)
{
	if (boolean_value == 0)
	{
		return (BitsArrSetOff(barr, idx));
	}
	else
	{
		return (BitsArrSetOn(barr, idx));		
	}
}

/**************************BitsArrIsOn**************************/
int BitsArrIsOn(bitsarr_t barr, size_t idx)
{
	return (!!(barr & (1 << idx)));
}

/**************************BitsArrIsOff**************************/
int BitsArrIsOff(bitsarr_t barr, size_t idx)
{
	return (!(barr & (1 << idx)));
}

/**************************BitsArrFlipBit**************************/
bitsarr_t BitsArrFlipBit(bitsarr_t barr, size_t idx)
{
	return (barr ^ (1 << idx));
}

/**************************BitsArrRotateRNoLoop**************************/
bitsarr_t BitsArrRotateRNoLoop(bitsarr_t barr, size_t shifts)
{
	return ((barr >> (shifts % BUFF_SIZE)) | (barr << ((BUFF_SIZE - shifts) % BUFF_SIZE)));
}

/**************************BitsArrRotateLNoLoop**************************/
bitsarr_t BitsArrRotateLNoLoop(bitsarr_t barr, size_t shifts)
{
	return (((barr << (shifts % BUFF_SIZE)) | (barr >> (BUFF_SIZE - (shifts % BUFF_SIZE)))));
}

/**************************BitsArrMirror**************************/
bitsarr_t BitsArrMirror(bitsarr_t barr)
{
	size_t temp = 0;
	size_t i = 0;
	
	for (i = 0; i < BUFF_SIZE; ++i)
	{
		temp <<= 1;
		temp |= (barr & 1);
		barr >>= 1;
	}

	return temp;
}

/**************************BitsArrCountOn**************************/
size_t BitsArrCountOn(bitsarr_t barr)
{
	size_t count = 0;

	while (barr != 0)
	{
		barr &= (barr - 1);
		++count;
	}
	
	return count;
}

/**************************BitsArrCountOff**************************/
size_t BitsArrCountOff(bitsarr_t barr)
{
	return (BUFF_SIZE - BitsArrCountOn(barr));
}

/**************************BitsArrBitsToStr**************************/
char *BitsArrBitsToStr(bitsarr_t barr, char *buff)
{
	size_t i = 0;

	assert(buff != NULL);

	buff += BUFF_SIZE;
	*buff = '\0';

	for (i = 0; i < BUFF_SIZE; ++i)
	{
		--buff;
		*buff = (barr & 1) + ASCII_CONVERT;
		barr >>= 1;
	}

	return buff;
}

/**************************BitsArrCountOnWithLut**************************/
size_t BitsArrCountOnWithLut(bitsarr_t barr)
{
	int array[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	size_t count = 0;

	while (barr != 0)
	{
		count += array[barr & 15];
		barr >>= 4;
	}
	
	return count;
}

/**************************BitsArrCountOffWithLut**************************/
size_t BitsArrCountOffWithLut(bitsarr_t barr)
{
	return (BUFF_SIZE - BitsArrCountOnWithLut(barr));
}

/**************************BitsArrMirrorWithLut**************************/
size_t BitsArrMirrorWithLut(bitsarr_t barr)
{
	int array[] = {0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};
	bitsarr_t mirror_barr = 0;
	size_t i = 0;
	for (i = 0; i < BUFF_SIZE / 4; ++i)
	{
		mirror_barr |= array[barr & 15];
		mirror_barr <<= 4;
		barr >>= 4;
	}
	
	return mirror_barr;
}
























