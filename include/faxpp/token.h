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

#include "text.h"

typedef enum {
  NO_TOKEN  = 0,

  XML_DECL_VERSION_TOKEN,
  XML_DECL_ENCODING_TOKEN,
  XML_DECL_STANDALONE_TOKEN,

  START_ELEMENT_PREFIX_TOKEN,
  START_ELEMENT_NAME_TOKEN,
  START_ELEMENT_END_TOKEN,
  SELF_CLOSING_ELEMENT_TOKEN,

  XMLNS_PREFIX_TOKEN,
  XMLNS_NAME_TOKEN,

  ATTRIBUTE_PREFIX_TOKEN,
  ATTRIBUTE_NAME_TOKEN,
  ATTRIBUTE_VALUE_TOKEN,

  END_ELEMENT_PREFIX_TOKEN,
  END_ELEMENT_NAME_TOKEN,

  CHARACTERS_TOKEN,
  CDATA_TOKEN,
  IGNORABLE_WHITESPACE_TOKEN,

  COMMENT_TOKEN,

  PI_NAME_TOKEN,
  PI_VALUE_TOKEN,

  ENTITY_REFERENCE_TOKEN,
  DEC_CHAR_REFERENCE_TOKEN,
  HEX_CHAR_REFERENCE_TOKEN,

  END_OF_BUFFER_TOKEN = 99
} FAXPP_TokenType;

typedef struct FAXPP_Token_s {
  FAXPP_TokenType token;
  FAXPP_Text value;
  unsigned int line;
  unsigned int column;
} FAXPP_Token;

const char *FAXPP_token_to_string(FAXPP_Token *token);

#endif
