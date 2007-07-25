/*
 * Copyright 2007 Doxological Ltd.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdlib.h>

#include <faxpp/buffer.h>

TokenizerError init_buffer(Buffer *buffer, unsigned int initialSize)
{
  buffer->buffer = malloc(initialSize);
  if(!buffer->buffer) return OUT_OF_MEMORY;
  buffer->length = initialSize;
  buffer->cursor = buffer->buffer;
  return NO_ERROR;
}

TokenizerError free_buffer(Buffer *buffer)
{
  if(buffer->buffer) free(buffer->buffer);
  return NO_ERROR;
}

TokenizerError resize_buffer(Buffer *buffer, unsigned int minSize, BufferResizeCallback callback, void *userData)
{
  unsigned int newLength = buffer->length << 1;
  while(newLength < minSize) {
    newLength = newLength << 1;
  }

  void *newBuffer = realloc(buffer->buffer, newLength);
  if(!newBuffer) return OUT_OF_MEMORY;

  if(newBuffer != buffer->buffer) {
    if(callback) callback(userData, buffer, newBuffer);
    buffer->cursor += newBuffer - buffer->buffer;
    buffer->buffer = newBuffer;
  }

  buffer->length = newLength;

  return NO_ERROR;
}

TokenizerError buffer_append(Buffer *buffer, void *ptr, unsigned int len,
                             BufferResizeCallback callback, void *userData)
{
  TokenizerError err;
  if(buffer->cursor + len > buffer->buffer + buffer->length) {
    err = resize_buffer(buffer, (buffer->cursor + len) - buffer->buffer, callback, userData);
    if(err != 0) return err;
  }

  memcpy(buffer->cursor, ptr, len);
  buffer->cursor += len;

  return NO_ERROR;
}

TokenizerError buffer_append_ch(Buffer *buffer, EncodeFunction encode, Char32 ch,
                                BufferResizeCallback callback, void *userData)
{
  TokenizerError err;
  unsigned int len;
  while((len = encode(buffer->cursor, buffer->buffer + buffer->length, ch))
        == TRANSCODE_PREMATURE_END_OF_BUFFER) {
    err = resize_buffer(buffer, 0, callback, userData);
    if(err != 0) return err;
  }

  buffer->cursor += len;

  return NO_ERROR;
}

