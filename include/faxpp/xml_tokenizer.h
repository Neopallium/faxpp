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

#include "token.h"
#include "transcode.h"
#include "error.h"
#include "buffer.h"

/*********************
 *
 *  Tokenizer Environment
 *
 *********************/

typedef struct TokenizerEnv_s TokenizerEnv;
typedef TokenizerError (*StateFunction)(TokenizerEnv *env);

struct TokenizerEnv_s {
  // TBD Mark end of buffer with EOF instead of using buffer_end - jpcs
  void *buffer;
  void *buffer_end;

  void *position;
  Char32 current_char;
  unsigned int char_len;

  unsigned int line;
  unsigned int column;

  unsigned int nesting_level;
  unsigned int seen_doc_element:1;

  DecodeFunction decode;
  EncodeFunction encode;

  Token *result_token;
  Token token;

  Buffer token_buffer;
  void *token_position1;
  void *token_position2;

  StateFunction state;
  StateFunction stored_state;

  StateFunction start_element_name_state;
  StateFunction element_content_state;
};

/*********************
 *
 *  Tokenizer Functions
 *
 *********************/

TokenizerError init_tokenizer(TokenizerEnv *env);
TokenizerError free_tokenizer(TokenizerEnv *env);

TokenizerError init_tokenize(TokenizerEnv *env, void *buffer,
                             unsigned int length, EncodeFunction encode);
TokenizerError continue_tokenize(TokenizerEnv *env, void *buffer,
                                 unsigned int length);
TokenizerError next_token(TokenizerEnv *env, Token *token);

#endif
