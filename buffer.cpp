#include "buffer.h"
#include "output.h"

void bufferAppend(outputBuffer *ob, const char *s, int len)
{
  char *to_append = (char *)realloc(ob->buffer, ob->len + len);
  if (to_append == NULL)
  {
    setMessage("buffer kosong");
    return;
  }

  memcpy(&to_append[ob->len], s, len);
  ob->buffer = to_append;
  ob->len += len;
  
}

void bufferFree(outputBuffer *ob)
{
  free(ob->buffer);
}
