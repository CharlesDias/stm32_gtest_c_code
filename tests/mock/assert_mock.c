#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h> 

void assert_failed(uint8_t *file, unsigned int line)
{
   printf("assert_failed: file %s on line %lu\r\n", file, line);
   fflush(stdout);
   assert(false);
}

#ifdef __cplusplus
}
#endif