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
elementdecl_or_entitydecl_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'L':
    env->state = elementdecl_initial_state1;
    next_char(env);
    break;
  case 'N':
    env->state = entitydecl_initial_state1;
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_DTD_DECL;
  }
  return NO_ERROR;
}

#define SINGLE_CHAR_STATE_RETURN(name, ch, next_stored_state, next_state, error, return_token) \
FAXPP_Error \
name(FAXPP_TokenizerEnv *env) \
{ \
  read_char(env); \
\
  switch(env->current_char) { \
  case (ch): \
    if((next_stored_state) != 0) env->stored_state = (next_stored_state); \
    env->state = (next_state); \
    if((return_token) != NO_TOKEN) { report_empty_token((return_token), env); } \
    next_char(env); \
    break; \
  LINE_ENDINGS \
  default: \
    next_char(env); \
    return (error); \
  } \
  return NO_ERROR; \
}

#define SINGLE_CHAR_STATE(name, ch, next_stored_state, next_state, error) SINGLE_CHAR_STATE_RETURN(name, ch, next_stored_state, next_state, error, NO_TOKEN)

SINGLE_CHAR_STATE(elementdecl_initial_state1, 'E', 0, elementdecl_initial_state2, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(elementdecl_initial_state2, 'M', 0, elementdecl_initial_state3, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(elementdecl_initial_state3, 'E', 0, elementdecl_initial_state4, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(elementdecl_initial_state4, 'N', 0, elementdecl_initial_state5, INVALID_DTD_DECL)
SINGLE_CHAR_STATE(elementdecl_initial_state5, 'T', elementdecl_name_state1, ws_plus_state, INVALID_DTD_DECL)

FAXPP_Error
elementdecl_name_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = elementdecl_name_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
elementdecl_name_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = elementdecl_content_state;
      env->state = ws_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case ':':
      env->state = elementdecl_name_seen_colon_state1;
      token_end_position(env);
      report_token(ELEMENTDECL_PREFIX_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
elementdecl_name_seen_colon_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = elementdecl_name_seen_colon_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
elementdecl_name_seen_colon_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = elementdecl_content_state;
      env->state = ws_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
elementdecl_content_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'E':
    env->state = elementdecl_empty_state1;
    break;
  case 'A':
    env->state = elementdecl_any_state1;
    break;
  case '(':
    env->nesting_level += 1;
    env->stored_state = elementdecl_mixed_or_children_state;
    env->state = ws_state;
    report_empty_token(ELEMENTDECL_LPAR_TOKEN, env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_ELEMENTDECL_CONTENT;
  }
  next_char(env);
  return NO_ERROR;
}

SINGLE_CHAR_STATE(elementdecl_empty_state1, 'M', 0, elementdecl_empty_state2, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE(elementdecl_empty_state2, 'P', 0, elementdecl_empty_state3, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE(elementdecl_empty_state3, 'T', 0, elementdecl_empty_state4, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE_RETURN(elementdecl_empty_state4, 'Y', elementdecl_end_state, ws_state, INVALID_ELEMENTDECL_CONTENT, ELEMENTDECL_EMPTY_TOKEN)

SINGLE_CHAR_STATE(elementdecl_any_state1, 'N', 0, elementdecl_any_state2, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE_RETURN(elementdecl_any_state2, 'Y', elementdecl_end_state, ws_state, INVALID_ELEMENTDECL_CONTENT, ELEMENTDECL_ANY_TOKEN)

FAXPP_Error
elementdecl_mixed_or_children_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '#':
    env->state = elementdecl_pcdata_state1;
    break;
  default:
    env->state = elementdecl_cp_name_state1;
    // No next_char
    return NO_ERROR;
  }
  next_char(env);
  return NO_ERROR;

}

FAXPP_Error
elementdecl_cp_name_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '(':
    env->nesting_level += 1;
    env->stored_state = elementdecl_cp_name_state1;
    env->state = ws_state;
    report_empty_token(ELEMENTDECL_LPAR_TOKEN, env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = elementdecl_cp_name_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
elementdecl_cp_name_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = elementdecl_cp_separator_or_end_state;
      env->state = ws_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '?':
    case '*':
    case '+':
      env->state = elementdecl_cp_cardinality_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      // No next_char
      return NO_ERROR;
    case '|':
    case ',':
    case ')':
      env->state = elementdecl_cp_separator_or_end_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      // No next_char
      return NO_ERROR;
    case ':':
      env->state = elementdecl_cp_name_seen_colon_state1;
      token_end_position(env);
      report_token(ELEMENTDECL_PREFIX_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
elementdecl_cp_name_seen_colon_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = elementdecl_cp_name_seen_colon_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
elementdecl_cp_name_seen_colon_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = elementdecl_cp_separator_or_end_state;
      env->state = ws_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '?':
    case '*':
    case '+':
      env->state = elementdecl_cp_cardinality_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      // No next_char
      return NO_ERROR;
    case '|':
    case ',':
    case ')':
      env->state = elementdecl_cp_separator_or_end_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      // No next_char
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
elementdecl_cp_cardinality_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  if(env->nesting_level == 0)
    env->stored_state = elementdecl_end_state;
  else
    env->stored_state = elementdecl_cp_separator_or_end_state;
  env->state = ws_state;

  switch(env->current_char) {
  case '?':
    report_empty_token(ELEMENTDECL_QUESTION_TOKEN, env);
    break;
  case '*':
    report_empty_token(ELEMENTDECL_STAR_TOKEN, env);
    break;
  case '+':
    report_empty_token(ELEMENTDECL_PLUS_TOKEN, env);
    break;
  default:
    // No next_char
    return NO_ERROR;
  }

  next_char(env);
  return NO_ERROR;  
}

FAXPP_Error
elementdecl_cp_separator_or_end_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '|':
    env->stored_state = elementdecl_cp_name_state1;
    env->state = ws_state;
    report_empty_token(ELEMENTDECL_BAR_TOKEN, env);
    break;
  case ',':
    env->stored_state = elementdecl_cp_name_state1;
    env->state = ws_state;
    report_empty_token(ELEMENTDECL_COMMA_TOKEN, env);
    break;
  case ')':
    env->nesting_level -= 1;
    env->state = elementdecl_cp_cardinality_state;
    report_empty_token(ELEMENTDECL_RPAR_TOKEN, env);
    break;
  default:
    next_char(env);
    return INVALID_ELEMENTDECL_CONTENT;
  }

  next_char(env);
  return NO_ERROR;  
}

SINGLE_CHAR_STATE(elementdecl_pcdata_state1, 'P', 0, elementdecl_pcdata_state2, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE(elementdecl_pcdata_state2, 'C', 0, elementdecl_pcdata_state3, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE(elementdecl_pcdata_state3, 'D', 0, elementdecl_pcdata_state4, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE(elementdecl_pcdata_state4, 'A', 0, elementdecl_pcdata_state5, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE(elementdecl_pcdata_state5, 'T', 0, elementdecl_pcdata_state6, INVALID_ELEMENTDECL_CONTENT)
SINGLE_CHAR_STATE_RETURN(elementdecl_pcdata_state6, 'A', elementdecl_pcdata_end_or_names_state1, ws_state, INVALID_ELEMENTDECL_CONTENT, ELEMENTDECL_PCDATA_TOKEN)

FAXPP_Error
elementdecl_pcdata_end_or_names_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ')':
    env->nesting_level -= 1;
    env->state = elementdecl_pcdata_optional_star_state;
    report_empty_token(ELEMENTDECL_RPAR_TOKEN, env);
    break;
  case '|':
    env->stored_state = elementdecl_pcdata_name_state1;
    env->state = ws_state;
    report_empty_token(ELEMENTDECL_BAR_TOKEN, env);
    break;
  default:
    next_char(env);
    return INVALID_ELEMENTDECL_CONTENT;
  }
  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
elementdecl_pcdata_optional_star_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '*':
    report_empty_token(ELEMENTDECL_STAR_TOKEN, env);
    next_char(env);
    // Fall through
  default:
    env->stored_state = elementdecl_end_state;
    env->state = ws_state;
    // No next_char
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
elementdecl_pcdata_end_or_names_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ')':
    env->nesting_level -= 1;
    env->state = elementdecl_pcdata_star_state;
    report_empty_token(ELEMENTDECL_RPAR_TOKEN, env);
    break;
  case '|':
    env->stored_state = elementdecl_pcdata_name_state1;
    env->state = ws_state;
    report_empty_token(ELEMENTDECL_BAR_TOKEN, env);
    break;
  default:
    next_char(env);
    return INVALID_ELEMENTDECL_CONTENT;
  }
  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
elementdecl_pcdata_star_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  env->stored_state = elementdecl_end_state;
  env->state = ws_state;

  switch(env->current_char) {
  case '*':
    report_empty_token(ELEMENTDECL_STAR_TOKEN, env);
    next_char(env);
    break;
  default:
    next_char(env);
    return INVALID_ELEMENTDECL_CONTENT;
  }
  return NO_ERROR;
}

FAXPP_Error
elementdecl_pcdata_name_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = elementdecl_pcdata_name_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
elementdecl_pcdata_name_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = elementdecl_pcdata_end_or_names_state2;
      env->state = ws_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case ')':
      env->state = elementdecl_pcdata_end_or_names_state2;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      // No next_char
      return NO_ERROR;
    case ':':
      env->state = elementdecl_pcdata_name_seen_colon_state1;
      token_end_position(env);
      report_token(ELEMENTDECL_PREFIX_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
elementdecl_pcdata_name_seen_colon_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = elementdecl_pcdata_name_seen_colon_state2;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
elementdecl_pcdata_name_seen_colon_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->stored_state = elementdecl_pcdata_end_or_names_state2;
      env->state = ws_state;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case ')':
      env->state = elementdecl_pcdata_end_or_names_state2;
      token_end_position(env);
      report_token(ELEMENTDECL_NAME_TOKEN, env);
      // No next_char
      return NO_ERROR;
    default:
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENTDECL_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
elementdecl_end_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '>':
    base_state(env);
    report_empty_token(ELEMENTDECL_END_TOKEN, env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_ELEMENTDECL_CONTENT;
  }
  next_char(env);
  return NO_ERROR;
}

