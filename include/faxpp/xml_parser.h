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

#ifndef __FAXPP__XML_PARSER_H
#define __FAXPP__XML_PARSER_H

#include <stdio.h>

#include "xml_tokenizer.h"
#include "event.h"
#include "buffer.h"

/*********************
 *
 *  Parser Environment
 *
 *********************/

typedef enum {
  NO_CHECKS_PARSE_MODE,
  WELL_FORMED_PARSE_MODE
} ParseMode;

typedef struct NamespaceInfo_s {
  Text prefix;
  Text uri;

  struct NamespaceInfo_s *prev;
} NamespaceInfo;

typedef struct ElementInfo_s {
  Text prefix;
  Text uri;
  Text name;

  NamespaceInfo *prev_ns;
  void *prev_stack_cursor;

  struct ElementInfo_s *prev;
} ElementInfo;

typedef struct ParserEnv_s ParserEnv;

/// Returns the number of bytes read - this will be less than "length" if the end of the input is reached
typedef unsigned int (*ReadCallback)(void *userData, void *buffer, unsigned int length);

struct ParserEnv_s {
  TokenizerError (*next_event)(ParserEnv *env);

  ParseMode mode;
  EncodeFunction encode;

  ReadCallback read;
  void *read_user_data;

  void *read_buffer;
  unsigned int read_buffer_length;

  TokenizerEnv tenv;
  Token token;
  unsigned int buffered_token:1;
  unsigned int start_doc_done:1;
  unsigned int null_terminate:1;

  unsigned int err_line;
  unsigned int err_column;

  Event event;

  unsigned int max_attr_count;
  Attribute *attrs;
  Attribute *current_attr;

  AttrValue *av_ptr;
  AttrValue *av_dealloc;

  ElementInfo *element_info_stack;
  NamespaceInfo *namespace_stack;

  ElementInfo *element_info_pool;
  NamespaceInfo *namespace_pool;

  Buffer event_buffer;
  Buffer stack_buffer;
};

/*********************
 *
 *  Parser Functions
 *
 *********************/

/// OUT_OF_MEMORY
TokenizerError init_parser(ParserEnv *env, ParseMode mode, EncodeFunction encode);
TokenizerError free_parser(ParserEnv *env);

/// UNSUPPORTED_ENCODING, OUT_OF_MEMORY
TokenizerError init_parse(ParserEnv *env, void *buffer, unsigned int length);
TokenizerError init_parse_file(ParserEnv *env, FILE *file);
TokenizerError init_parse_callback(ParserEnv *env, ReadCallback callback, void *userData);

TokenizerError next_event(ParserEnv *env);

#endif
