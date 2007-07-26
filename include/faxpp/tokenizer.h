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

#ifndef __FAXPP__TOKENIZER_H
#define __FAXPP__TOKENIZER_H

#include "error.h"
#include "transcode.h"
#include "token.h"

typedef struct FAXPP_TokenizerEnv_s FAXPP_Tokenizer;

FAXPP_Tokenizer *FAXPP_create_tokenizer();
void FAXPP_free_tokenizer(FAXPP_Tokenizer *tokenizer);

FAXPP_Error FAXPP_init_tokenize(FAXPP_Tokenizer *tokenizer, void *buffer,
                                unsigned int length, FAXPP_EncodeFunction encode);
FAXPP_Error FAXPP_continue_tokenize(FAXPP_Tokenizer *tokenizer, void *buffer,
                                    unsigned int length);

FAXPP_Error FAXPP_next_token(FAXPP_Tokenizer *tokenizer, FAXPP_Token *token);
unsigned int FAXPP_get_tokenizer_nesting_level(const FAXPP_Tokenizer *tokenizer);

unsigned int FAXPP_get_tokenizer_error_line(const FAXPP_Tokenizer *tokenizer);
unsigned int FAXPP_get_tokenizer_error_column(const FAXPP_Tokenizer *tokenizer);

#endif
