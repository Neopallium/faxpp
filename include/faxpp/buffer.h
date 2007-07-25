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

#ifndef __FAXPP__BUFFER_H
#define __FAXPP__BUFFER_H

#include "error.h"
#include "transcode.h"

/// Implementation of a resizing buffer
typedef struct Buffer_s {
  void *buffer;
  unsigned int length;
  void *cursor;
} Buffer;

typedef void (*BufferResizeCallback)(void *userData, Buffer *buffer, void *newBuffer);

TokenizerError init_buffer(Buffer *buffer, unsigned int initialSize);
TokenizerError free_buffer(Buffer *buffer);

#define reset_buffer(buf) (buf)->cursor = (buf)->buffer

TokenizerError resize_buffer(Buffer *buffer, unsigned int minSize,
                             BufferResizeCallback callback, void *userData);
TokenizerError buffer_append(Buffer *buffer, void *ptr, unsigned int len,
                             BufferResizeCallback callback, void *userData);
TokenizerError buffer_append_ch(Buffer *buffer, EncodeFunction encode, Char32 ch,
                                BufferResizeCallback callback, void *userData);

#endif
