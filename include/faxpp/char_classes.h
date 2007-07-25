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

#ifndef __FAXPP__CHAR_CLASSES_H
#define __FAXPP__CHAR_CLASSES_H

#include <stdint.h>

#define NCNAME_START_CHAR   0x01
#define NCNAME_CHAR         0x02
#define NAME_START_CHAR     0x04
#define NAME_CHAR           0x08
#define WHITESPACE_CHAR     0x10
#define NON_RESTRICTED_CHAR 0x20

typedef uint32_t Char32;

const char *char_flags_to_string(unsigned int flags);

#define char_flags(ch) ((ch) < 256) ? char_flags_256[(ch)] : char_flags_impl((ch))

unsigned int char_flags_impl(Char32 c);
const uint8_t char_flags_256[256];

#endif
