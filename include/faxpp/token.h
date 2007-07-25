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

#ifndef __FAXPP__TOKEN_H
#define __FAXPP__TOKEN_H

#define NO_TOKEN 0

#define XML_DECL_VERSION_TOKEN 1
#define XML_DECL_ENCODING_TOKEN 2
#define XML_DECL_STANDALONE_TOKEN 3

#define START_ELEMENT_PREFIX_TOKEN 4
#define START_ELEMENT_NAME_TOKEN 5
#define START_ELEMENT_END_TOKEN 6
#define SELF_CLOSING_ELEMENT_TOKEN 7

#define XMLNS_PREFIX_TOKEN 8
#define XMLNS_NAME_TOKEN 9

#define ATTRIBUTE_PREFIX_TOKEN 10
#define ATTRIBUTE_NAME_TOKEN 11
#define ATTRIBUTE_VALUE_TOKEN 12

#define END_ELEMENT_PREFIX_TOKEN 13
#define END_ELEMENT_NAME_TOKEN 14

#define CHARACTERS_TOKEN 15
#define CDATA_TOKEN 16
#define IGNORABLE_WHITESPACE_TOKEN 17

#define COMMENT_TOKEN 18

#define PI_NAME_TOKEN 19
#define PI_VALUE_TOKEN 20

#define ENTITY_REFERENCE_TOKEN 21
#define DEC_CHAR_REFERENCE_TOKEN 22
#define HEX_CHAR_REFERENCE_TOKEN 23

#define END_OF_BUFFER_TOKEN 99

typedef struct Text_s {
  void *ptr;
  unsigned int len;
} Text;

typedef struct Token_s {
  unsigned int token;
  Text value;
  unsigned int line;
  unsigned int column;
} Token;

#ifdef DEBUG
const char *token_to_string(Token *token);
#endif

#endif
