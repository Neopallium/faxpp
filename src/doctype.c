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

FAXPP_Error
doctype_or_comment_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '-':
    env->state = comment_start_state2;
    break;
  case 'D':
    env->state = doctype_initial_state1;
    env->seen_doctype = 1;
    break;
  LINE_ENDINGS
  default:
    env->state = comment_content_state;
    token_start_position(env);
    next_char(env);
    return INVALID_START_OF_COMMENT;
  }
  next_char(env);
  return NO_ERROR;
}

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

SINGLE_CHAR_STATE(doctype_initial_state1, 'O', 0, doctype_initial_state2, INVALID_DOCTYPE_DECL)
SINGLE_CHAR_STATE(doctype_initial_state2, 'C', 0, doctype_initial_state3, INVALID_DOCTYPE_DECL)
SINGLE_CHAR_STATE(doctype_initial_state3, 'T', 0, doctype_initial_state4, INVALID_DOCTYPE_DECL)
SINGLE_CHAR_STATE(doctype_initial_state4, 'Y', 0, doctype_initial_state5, INVALID_DOCTYPE_DECL)
SINGLE_CHAR_STATE(doctype_initial_state5, 'P', 0, doctype_initial_state6, INVALID_DOCTYPE_DECL)
SINGLE_CHAR_STATE(doctype_initial_state6, 'E', doctype_name_state1, ws_plus_state, INVALID_DOCTYPE_DECL)

FAXPP_Error
doctype_name_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = doctype_name_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_DOCTYPE_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
doctype_name_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->state = doctype_after_name_state;
      token_end_position(env);
      report_token(DOCTYPE_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '[':
      env->state = doctype_internal_subset_state;
      token_end_position(env);
      report_token(DOCTYPE_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->state = doctype_end_state;
      token_end_position(env);
      report_token(DOCTYPE_NAME_TOKEN, env);
      // no next char
      return NO_ERROR;
    case ':':
      env->state = doctype_name_seen_colon_state1;
      token_end_position(env);
      report_token(DOCTYPE_PREFIX_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_DOCTYPE_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
doctype_name_seen_colon_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = doctype_name_seen_colon_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_DOCTYPE_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
doctype_name_seen_colon_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->state = doctype_after_name_state;
      token_end_position(env);
      report_token(DOCTYPE_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '[':
      env->state = doctype_internal_subset_state;
      token_end_position(env);
      report_token(DOCTYPE_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->state = doctype_end_state;
      token_end_position(env);
      report_token(DOCTYPE_NAME_TOKEN, env);
      // no next char
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_DOCTYPE_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
doctype_after_name_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    break;
  case 'S':
    env->stored_state = doctype_internal_subset_start_state;
    env->state = system_id_initial_state1;
    break;
/*   case 'P': */
/*     env->stored_state = doctype_internal_subset_start_state; */
/*     env->state = public_id_initial_state1; */
/*     break; */
  case '[':
    env->state = doctype_internal_subset_state;
    break;
  case '>':
    base_state(env);
    report_empty_token(DOCTYPE_END_TOKEN, env);
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  default:
    next_char(env);
    return INVALID_DOCTYPE_DECL;
  }
  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
doctype_internal_subset_start_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    break;
  case '[':
    env->state = doctype_internal_subset_state;
    break;
  case '>':
    base_state(env);
    report_empty_token(DOCTYPE_END_TOKEN, env);
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  default:
    next_char(env);
    return INVALID_DOCTYPE_DECL;
  }
  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
doctype_internal_subset_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ']':
    env->state = doctype_end_state;
    next_char(env);
    return NO_ERROR;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_DOCTYPE_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
doctype_end_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  case '>':
    base_state(env);
    report_empty_token(DOCTYPE_END_TOKEN, env);
    next_char(env);
    token_start_position(env);
    break;
  default:
    next_char(env);
    return INVALID_DOCTYPE_DECL;
  }
  return NO_ERROR;
}

SINGLE_CHAR_STATE(system_id_initial_state1, 'Y', 0, system_id_initial_state2, INVALID_SYSTEM_ID)
SINGLE_CHAR_STATE(system_id_initial_state2, 'S', 0, system_id_initial_state3, INVALID_SYSTEM_ID)
SINGLE_CHAR_STATE(system_id_initial_state3, 'T', 0, system_id_initial_state4, INVALID_SYSTEM_ID)
SINGLE_CHAR_STATE(system_id_initial_state4, 'E', 0, system_id_initial_state5, INVALID_SYSTEM_ID)
SINGLE_CHAR_STATE(system_id_initial_state5, 'M', 0, system_id_ws_state, INVALID_SYSTEM_ID)

FAXPP_Error
system_id_ws_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    env->state = system_literal_start_state;
    next_char(env);
    break;
  default:
    next_char(env);
    return EXPECTING_WHITESPACE;
  }
  return NO_ERROR;
}

FAXPP_Error
system_literal_start_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    return NO_ERROR;
  case '"':
    env->state = system_literal_quot_state;
    break;
  case '\'':
    env->state = system_literal_apos_state;
    break;
  default:
    next_char(env);
    return EXPECTING_SYSTEM_LITERAL;
  }
  next_char(env);
  token_start_position(env);
  return NO_ERROR;
}

FAXPP_Error
system_literal_apos_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case '\'':
      retrieve_state(env);
      token_end_position(env);
      report_token(SYSTEM_LITERAL_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    LINE_ENDINGS
    default:
      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0) {
        next_char(env);
        return RESTRICTED_CHAR;
      }
      break;
    }
    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
system_literal_quot_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case '"':
      retrieve_state(env);
      token_end_position(env);
      report_token(SYSTEM_LITERAL_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    LINE_ENDINGS
    default:
      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0) {
        next_char(env);
        return RESTRICTED_CHAR;
      }
      break;
    }
    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

