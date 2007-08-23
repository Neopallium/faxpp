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

#ifndef __FAXPP__XML_TOKENIZER_H
#define __FAXPP__XML_TOKENIZER_H

#include "buffer.h"
#include <faxpp/tokenizer.h>

/*********************
 *
 *  Tokenizer Environment
 *
 *********************/

typedef struct FAXPP_TokenizerEnv_s FAXPP_TokenizerEnv;
typedef FAXPP_Error (*FAXPP_StateFunction)(FAXPP_TokenizerEnv *env);

typedef enum {
  DEFAULT_START_ELEMENT_NAME_STATE,
  DEFAULT_START_ELEMENT_NAME_SEEN_COLON_STATE,
  DEFAULT_START_ELEMENT_NAME_SEEN_COLON_STATE2,
  DEFAULT_START_ELEMENT_MANDATORY_WS_STATE,
  DEFAULT_START_ELEMENT_WS_STATE,
  DEFAULT_ATTR_NAME_STATE,
  DEFAULT_ATTR_VALUE_START_STATE,
  DEFAULT_ATTR_VALUE_APOS_STATE,
  DEFAULT_ATTR_VALUE_QUOT_STATE,
  DEFAULT_ELEMENT_CONTENT_STATE,
  DEFAULT_ELEMENT_CONTENT_RSQUARE_STATE1,
  DEFAULT_ELEMENT_CONTENT_RSQUARE_STATE2,
  DEFAULT_ELEMENT_CONTENT_MARKUP_STATE,
  DEFAULT_END_ELEMENT_NAME_STATE,
  DEFAULT_END_ELEMENT_NAME_STATE2,
  DEFAULT_END_ELEMENT_NAME_SEEN_COLON_STATE,
  DEFAULT_END_ELEMENT_NAME_SEEN_COLON_STATE2,

  UTF8_START_ELEMENT_NAME_STATE,
  UTF8_START_ELEMENT_NAME_SEEN_COLON_STATE,
  UTF8_START_ELEMENT_NAME_SEEN_COLON_STATE2,
  UTF8_START_ELEMENT_MANDATORY_WS_STATE,
  UTF8_START_ELEMENT_WS_STATE,
  UTF8_ATTR_NAME_STATE,
  UTF8_ATTR_VALUE_START_STATE,
  UTF8_ATTR_VALUE_APOS_STATE,
  UTF8_ATTR_VALUE_QUOT_STATE,
  UTF8_ELEMENT_CONTENT_STATE,
  UTF8_ELEMENT_CONTENT_RSQUARE_STATE1,
  UTF8_ELEMENT_CONTENT_RSQUARE_STATE2,
  UTF8_ELEMENT_CONTENT_MARKUP_STATE,
  UTF8_END_ELEMENT_NAME_STATE,
  UTF8_END_ELEMENT_NAME_STATE2,
  UTF8_END_ELEMENT_NAME_SEEN_COLON_STATE,
  UTF8_END_ELEMENT_NAME_SEEN_COLON_STATE2,

  UTF16_START_ELEMENT_NAME_STATE,
  UTF16_START_ELEMENT_NAME_SEEN_COLON_STATE,
  UTF16_START_ELEMENT_NAME_SEEN_COLON_STATE2,
  UTF16_START_ELEMENT_MANDATORY_WS_STATE,
  UTF16_START_ELEMENT_WS_STATE,
  UTF16_ATTR_NAME_STATE,
  UTF16_ATTR_VALUE_START_STATE,
  UTF16_ATTR_VALUE_APOS_STATE,
  UTF16_ATTR_VALUE_QUOT_STATE,
  UTF16_ELEMENT_CONTENT_STATE,
  UTF16_ELEMENT_CONTENT_RSQUARE_STATE1,
  UTF16_ELEMENT_CONTENT_RSQUARE_STATE2,
  UTF16_ELEMENT_CONTENT_MARKUP_STATE,
  UTF16_END_ELEMENT_NAME_STATE,
  UTF16_END_ELEMENT_NAME_STATE2,
  UTF16_END_ELEMENT_NAME_SEEN_COLON_STATE,
  UTF16_END_ELEMENT_NAME_SEEN_COLON_STATE2
} FAXPP_TokenizerState;

struct FAXPP_TokenizerEnv_s {
  // TBD Mark end of buffer with EOF instead of using buffer_end - jpcs
  void *buffer;
  void *buffer_end;

  void *position;
  Char32 current_char;
  unsigned int char_len;

  unsigned int line;
  unsigned int column;

  unsigned int nesting_level;
  unsigned int do_encode:1;
  unsigned int seen_doc_element:1;
  unsigned int buffer_done:1;
  unsigned int normalize_attrs:1;

  FAXPP_DecodeFunction decode;
  FAXPP_EncodeFunction encode;

  FAXPP_Token result_token;
  FAXPP_Token token;

  FAXPP_Buffer token_buffer;
  void *token_position1;
  void *token_position2;

  FAXPP_StateFunction state;
  FAXPP_StateFunction stored_state;

  FAXPP_TokenizerState enum_state;

  FAXPP_TokenizerState start_element_name_enum_state;
  FAXPP_TokenizerState element_content_enum_state;

  uint8_t ncname_start_char;
  uint8_t ncname_char;
  uint8_t non_restricted_char;
};

#endif
