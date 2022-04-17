
#include "buffer.h"
void bufferAppend(outputBuffer *ob, const char *s, int len)
{
  char *new = realloc(ob->b, ob->len + len);
  if (new == NULL)
    return;
  memcpy(&new[ob->len], s, len);
  ob->b = new;
  ob->len += len;
}

void bufferFree(outputBuffer *ob)
{
  free(ob->b);
}
