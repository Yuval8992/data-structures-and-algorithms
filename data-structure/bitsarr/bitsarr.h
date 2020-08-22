#include <stddef.h> /*size_t*/

typedef size_t bitsarr_t ;

bitsarr_t BitsArrSetOn  (bitsarr_t barr, size_t idx);
bitsarr_t BitsArrSetOff (bitsarr_t barr, size_t idx);
bitsarr_t BitsArrSetAll    (bitsarr_t barr);
bitsarr_t BitsArrResetAll  (bitsarr_t barr);
bitsarr_t BitsArrSetBit    (bitsarr_t barr, size_t idx, int boolean_value);

int BitsArrIsOn    (bitsarr_t barr, size_t idx);
int BitsArrIsOff   (bitsarr_t barr, size_t idx);

bitsarr_t BitsArrFlipBit       (bitsarr_t barr, size_t idx);
bitsarr_t BitsArrRotateRNoLoop (bitsarr_t barr, size_t shifts);
bitsarr_t BitsArrRotateLNoLoop (bitsarr_t barr, size_t shifts);

bitsarr_t BitsArrMirror   (bitsarr_t barr);
char *BitsArrBitsToStr    (bitsarr_t barr, char *buff); 

size_t BitsArrCountOn     (bitsarr_t barr);
size_t BitsArrCountOff    (bitsarr_t barr);
size_t BitsArrCountOnWithLut(bitsarr_t barr);
size_t BitsArrCountOffWithLut(bitsarr_t barr);
size_t BitsArrMirrorWithLut(bitsarr_t barr);

