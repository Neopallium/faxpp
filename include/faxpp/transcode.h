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

#ifndef __FAXPP__TRANSCODE_H
#define __FAXPP__TRANSCODE_H

#include <stdint.h>

typedef uint32_t Char32;

#define TRANSCODE_ERROR 1000
#define TRANSCODE_PREMATURE_END_OF_BUFFER 1001
#define TRANSCODE_BAD_ENCODING 1002

/// Returns the length of the char, unless it is
/// bigger than TRANSCODE_ERROR, in which case it is
/// an error code
typedef unsigned int (*FAXPP_DecodeFunction)
     (void *buffer, void *buffer_end, Char32 *ch);

/// Returns the length of the char, unless it is
/// bigger than TRANSCODE_ERROR, in which case it is
/// an error code
typedef unsigned int (*FAXPP_EncodeFunction)
     (void *buffer, void *buffer_end, Char32 ch);

const char *FAXPP_decode_to_string(FAXPP_DecodeFunction t);
const char *FAXPP_encode_to_string(FAXPP_EncodeFunction t);

unsigned int FAXPP_utf8_decode(void *buffer, void *buffer_end, Char32 *ch);
unsigned int FAXPP_utf16_le_decode(void *buffer, void *buffer_end, Char32 *ch);
unsigned int FAXPP_utf16_be_decode(void *buffer, void *buffer_end, Char32 *ch);
unsigned int FAXPP_utf16_native_decode(void *buffer, void *buffer_end, Char32 *ch);
unsigned int FAXPP_ucs4_le_decode(void *buffer, void *buffer_end, Char32 *ch);
unsigned int FAXPP_ucs4_be_decode(void *buffer, void *buffer_end, Char32 *ch);
unsigned int FAXPP_ucs4_native_decode(void *buffer, void *buffer_end, Char32 *ch);

unsigned int FAXPP_utf8_encode(void *buffer, void *buffer_end, Char32 ch);
unsigned int FAXPP_utf16_native_encode(void *buffer, void *buffer_end, Char32 ch);

#endif
