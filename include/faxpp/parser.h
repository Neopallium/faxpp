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

#ifndef __FAXPP__PARSER_H
#define __FAXPP__PARSER_H

#include <stdio.h>

#include "error.h"
#include "transcode.h"
#include "event.h"

typedef struct FAXPP_ParserEnv_s FAXPP_Parser;

typedef enum {
  NO_CHECKS_PARSE_MODE,
  WELL_FORMED_PARSE_MODE
} FAXPP_ParseMode;

/// Returns the number of bytes read - this will be less than "length" if the end of the input is reached
typedef unsigned int (*FAXPP_ReadCallback)(void *userData, void *buffer, unsigned int length);

FAXPP_Parser *FAXPP_create_parser(FAXPP_ParseMode mode, FAXPP_EncodeFunction encode);
void FAXPP_free_parser(FAXPP_Parser *parser);

void FAXPP_set_null_terminate(FAXPP_Parser *parser, unsigned int boolean);
void FAXPP_set_encode(FAXPP_Parser *parser, FAXPP_EncodeFunction encode);

/// UNSUPPORTED_ENCODING, OUT_OF_MEMORY
FAXPP_Error FAXPP_init_parse(FAXPP_Parser *parser, void *buffer, unsigned int length);
FAXPP_Error FAXPP_init_parse_file(FAXPP_Parser *parser, FILE *file);
FAXPP_Error FAXPP_init_parse_callback(FAXPP_Parser *parser, FAXPP_ReadCallback callback, void *userData);

FAXPP_Error FAXPP_next_event(FAXPP_Parser *parser);
const FAXPP_Event *FAXPP_get_current_event(const FAXPP_Parser *parser);

FAXPP_Error FAXPP_lookup_namespace_uri(const FAXPP_Parser *parser, const FAXPP_Text *prefix, FAXPP_Text *uri);

unsigned int FAXPP_get_error_line(const FAXPP_Parser *parser);
unsigned int FAXPP_get_error_column(const FAXPP_Parser *parser);

#endif
