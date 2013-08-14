#include <sys/time.h>
#include <string.h>

long
timeofday_diff(struct timeval *b, struct timeval *a) {
    struct timeval diff;
    memset(&diff, 0, sizeof(diff));
    
    diff.tv_sec  = a->tv_sec  - b->tv_sec - 1;
    diff.tv_usec = a->tv_usec - b->tv_usec + 1000000L;
    if (diff.tv_usec >= 1000000L) {                           
        diff.tv_usec -= 1000000L;
        diff.tv_sec++;
    }

    return (long)(diff.tv_sec * 1000 + diff.tv_usec / 1000);
}
