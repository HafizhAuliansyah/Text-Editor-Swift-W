#include "buffer.h"

void bufferAppend(outputBuffer *ob, const char *s, int len)
{
  char *to_append = (char*) realloc(ob->buffer, ob->len + len);
  if (to_append == NULL)
    return;
  memcpy(&to_append[ob->len], s, len);
  ob->buffer = to_append;
  ob->len += len;
}

void bufferFree(outputBuffer *ob)
{
  free(ob->buffer);
}
