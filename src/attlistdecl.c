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

#include "tokenizer_states.h"
#include "char_classes.h"

#define SINGLE_CHAR_STATE(name, ch, next_stored_state, next_state, error) \
FAXPP_Error \
name(FAXPP_TokenizerEnv *env) \
{ \
  read_char(env); \
\
  switch(env->current_char) { \
  case (ch): \
    if((next_stored_state) != 0) env->stored_state = (next_stored_state); \
    env->state = (next_state); \
    next_char(env); \
    break; \
  LINE_ENDINGS \
  default: \
    next_char(env); \
    return (error); \
  } \
  return NO_ERROR; \
}

SINGLE_CHAR_STATE(attlistdecl_initial_state1, 'T', 0, attlistdecl_initial_state2, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(attlistdecl_initial_state2, 'T', 0, attlistdecl_initial_state3, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(attlistdecl_initial_state3, 'L', 0, attlistdecl_initial_state4, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(attlistdecl_initial_state4, 'I', 0, attlistdecl_initial_state5, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(attlistdecl_initial_state5, 'S', 0, attlistdecl_initial_state6, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(attlistdecl_initial_state6, 'T', attlistdecl_name_state1, ws_plus_state, INVALID_DTD_DECL)

FAXPP_Error
attlistdecl_name_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = attlistdecl_name_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ATTLISTDECL_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
attlistdecl_name_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = attlistdecl_content_state;
      env->state = ws_state;
      token_end_position(env);
      report_token(ATTLISTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->state = attlistdecl_content_state;
      token_end_position(env);
      report_token(ATTLISTDECL_NAME_TOKEN, env);
      // no next char
      token_start_position(env);
      return NO_ERROR;
    case ':':
      env->state = attlistdecl_name_seen_colon_state1;
      token_end_position(env);
      report_token(ATTLISTDECL_PREFIX_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ATTLISTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
attlistdecl_name_seen_colon_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = attlistdecl_name_seen_colon_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ATTLISTDECL_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
attlistdecl_name_seen_colon_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = attlistdecl_content_state;
      env->state = ws_state;
      token_end_position(env);
      report_token(ATTLISTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->state = attlistdecl_content_state;
      token_end_position(env);
      report_token(ATTLISTDECL_NAME_TOKEN, env);
      // no next char
      token_start_position(env);
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ATTLISTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
attlistdecl_content_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '>':
    base_state(env);
    report_token(ATTLISTDECL_CONTENT_TOKEN, env);
    break;
  LINE_ENDINGS
  default:
    break;
  }
  next_char(env);
  return NO_ERROR;
}

