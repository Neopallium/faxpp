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
      env->state = internal_subset_state;
      env->internal_subset = 1;
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
      env->state = internal_subset_state;
      env->internal_subset = 1;
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
  case 'P':
    env->stored_state = doctype_internal_subset_start_state;
    env->state = public_id_initial_state1;
    break;
  case '[':
    env->state = internal_subset_state;
    env->internal_subset = 1;
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
    env->state = system_literal_start_state;
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

SINGLE_CHAR_STATE(public_id_initial_state1, 'U', 0, public_id_initial_state2, INVALID_PUBLIC_ID)
SINGLE_CHAR_STATE(public_id_initial_state2, 'B', 0, public_id_initial_state3, INVALID_PUBLIC_ID)
SINGLE_CHAR_STATE(public_id_initial_state3, 'L', 0, public_id_initial_state4, INVALID_PUBLIC_ID)
SINGLE_CHAR_STATE(public_id_initial_state4, 'I', 0, public_id_initial_state5, INVALID_PUBLIC_ID)
SINGLE_CHAR_STATE(public_id_initial_state5, 'C', 0, public_id_ws_state, INVALID_PUBLIC_ID)

FAXPP_Error
public_id_ws_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    env->state = pubid_literal_start_state;
    next_char(env);
    break;
  default:
    env->state = pubid_literal_start_state;
    return EXPECTING_WHITESPACE;
  }
  return NO_ERROR;
}

FAXPP_Error
pubid_literal_start_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    return NO_ERROR;
  case '"':
    env->state = pubid_literal_quot_state;
    break;
  case '\'':
    env->state = pubid_literal_apos_state;
    break;
  default:
    next_char(env);
    return EXPECTING_PUBID_LITERAL;
  }
  next_char(env);
  token_start_position(env);
  return NO_ERROR;
}

FAXPP_Error
pubid_literal_apos_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case '\'':
      env->state = system_id_ws_state;
      token_end_position(env);
      report_token(PUBID_LITERAL_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    // [13]   	PubidChar	   ::=   	#x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
    LINE_ENDINGS
    // A-Z
               case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    case 0x58: case 0x59: case 0x5A:
    // a-z
               case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
    case 0x78: case 0x79: case 0x7A:
    // 0-9
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
    case '9': 
    case ' ': case '-': case '(': case ')': case '+': case ',': case '.': case '/': case ':':
    case '=': case '?': case ';': case '!': case '*': case '#': case '@': case '$': case '_':
    case '%':
      // Valid PubidChar
      break;
    default:
      next_char(env);
      return INVALID_CHAR_IN_PUBID_LITERAL;
    }
    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
pubid_literal_quot_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case '"':
      env->state = system_id_ws_state;
      token_end_position(env);
      report_token(PUBID_LITERAL_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    // [13]   	PubidChar	   ::=   	#x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
    LINE_ENDINGS
    // A-Z
               case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    case 0x58: case 0x59: case 0x5A:
    // a-z
               case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
    case 0x78: case 0x79: case 0x7A:
    // 0-9
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
    case '9': 
    case ' ': case '-': case '(': case ')': case '+': case ',': case '.': case '/': case ':':
    case '=': case '?': case ';': case '!': case '*': case '#': case '@': case '$': case '_':
    case '%': case '\'':
      // Valid PubidChar
      break;
    default:
      next_char(env);
      return INVALID_CHAR_IN_PUBID_LITERAL;
    }
    next_char(env);
  }

  // Never happens
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
    env->state = internal_subset_state;
    env->internal_subset = 1;
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
internal_subset_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ']':
    env->state = doctype_end_state;
    env->internal_subset = 0;
    break;
  WHITESPACE:
    break;
  case '%':
    store_state(env);
    env->state = parameter_entity_reference_state;
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  case '<':
    env->state = internal_subset_markup_state;
    break;
  default:
    next_char(env);
    return INVALID_DOCTYPE_DECL;
  }

  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
internal_subset_state_en(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    break;
  case '%':
    store_state(env);
    env->state = parameter_entity_reference_state;
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  case '<':
    env->state = internal_subset_markup_state;
    break;
  default:
    next_char(env);
    return INVALID_DOCTYPE_DECL;
  }

  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
internal_subset_markup_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '?':
    env->state = pi_name_start_state;
    break;
  case '!':
    env->state = internal_subset_decl_state;
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_DTD_DECL;
  }

  next_char(env);
  token_start_position(env);
  return NO_ERROR;
}

FAXPP_Error
internal_subset_decl_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '-':
    env->state = comment_start_state2;
    break;
  case 'E':
    env->state = elementdecl_or_entitydecl_state;
    break;
  case 'A':
    env->state = attlistdecl_initial_state1;
    break;
  case 'N':
    env->state = notationdecl_initial_state1;
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

FAXPP_Error
external_subset_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    break;
  case '%':
    store_state(env);
    env->state = parameter_entity_reference_state;
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  case '<':
    env->state = external_subset_markup_state;
    break;
  default:
    next_char(env);
    return INVALID_DOCTYPE_DECL;
  }

  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
external_subset_markup_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '?':
    env->state = pi_name_start_state;
    break;
  case '!':
    env->state = external_subset_decl_state;
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_DTD_DECL;
  }

  next_char(env);
  token_start_position(env);
  return NO_ERROR;
}

FAXPP_Error
external_subset_decl_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '-':
    env->state = comment_start_state2;
    break;
/*   // TBD conditional sections - jpcs */
/*   case '[': */
/*     break; */
  case 'E':
    env->state = elementdecl_or_entitydecl_state;
    break;
  case 'A':
    env->state = attlistdecl_initial_state1;
    break;
  case 'N':
    env->state = notationdecl_initial_state1;
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

