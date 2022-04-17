#ifndef buffer_H
#define buffer_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*** append buffer ***/
struct outputBuffer
{
  char *buffer;
  int len;
};

#define OUTPUT_INIT \
  {               \
    NULL, 0       \
  }

void bufferAppend(outputBuffer *ob, const char *s, int len);

void bufferFree(outputBuffer *ob);
#endif
