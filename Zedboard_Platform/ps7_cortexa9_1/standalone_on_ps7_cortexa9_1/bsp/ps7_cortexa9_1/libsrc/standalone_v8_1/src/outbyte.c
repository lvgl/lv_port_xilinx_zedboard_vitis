#include "xparameters.h"
#ifdef __cplusplus
extern "C" {
#endif
void outbyte(char c); 

#ifdef __cplusplus
}
#endif 

#ifndef VERSAL_PLM
void outbyte(char c)
{
    (void) c;
}
#endif
