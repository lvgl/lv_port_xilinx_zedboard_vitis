#ifndef HR_GETTIME_H

#define HR_GETTIME_H

void init_timer( int xTimer );

/* Start-up the high-resolution timer. */
void vStartHighResolutionTimer( void );

/* Get the current time measured in uS. */
uint64_t ullGetHighResolutionTime( void );

#endif

