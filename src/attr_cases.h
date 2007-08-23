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

// This file needs to have a number of macros defined before it is included

start_enum_state(PREFIX(ATTR_NAME_STATE));

  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      env->state = PREFIX(attr_equals_state);
      token_end_position(env);
      report_token(ATTRIBUTE_NAME_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '=':
      change_enum_state(env, PREFIX(ATTR_VALUE_START_STATE));
      token_end_position(env);
      report_token(ATTRIBUTE_NAME_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case ':':
      env->state = PREFIX(attr_name_seen_colon_state);
      token_end_position(env);
      report_token(ATTRIBUTE_PREFIX_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ATTRIBUTE_NAME;
  }

start_enum_state(PREFIX(ATTR_VALUE_START_STATE));

  END_CHECK;

  READ_CHAR;

  switch(env->current_char) {
  WHITESPACE:
    goto NEXT_CHAR;
  case '"':
    change_enum_state(env, PREFIX(ATTR_VALUE_QUOT_STATE));
    next_char(env);
    token_start_position(env);
    goto_enum_state(PREFIX(ATTR_VALUE_QUOT_STATE));
  case '\'':
    change_enum_state(env, PREFIX(ATTR_VALUE_APOS_STATE));
    next_char(env);
    token_start_position(env);
    goto_enum_state(PREFIX(ATTR_VALUE_APOS_STATE));
  default:
    DEFAULT_CASE;

    next_char(env);
    return INVALID_CHAR_IN_ATTRIBUTE;
  }

start_enum_state(PREFIX(ATTR_VALUE_APOS_STATE));

  while(1) {
    if(env->position >= env->buffer_end) {
      if(env->token.value.ptr) {
        token_end_position(env);
        if(env->token.value.len != 0) {
          report_token(ATTRIBUTE_VALUE_TOKEN, env);
          return NO_ERROR;
        }
      }
      token_start_position(env);
      return PREMATURE_END_OF_BUFFER;
    }

    READ_CHAR;

    switch(env->current_char) {
    case '\'':
      change_enum_state(env, PREFIX(START_ELEMENT_MANDATORY_WS_STATE));
      token_end_position(env);
      report_token(ATTRIBUTE_VALUE_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '&':
      store_state(env);
      env->state = reference_state;
      token_end_position(env);
      report_token(ATTRIBUTE_VALUE_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    case '<':
      next_char(env);
      return INVALID_CHAR_IN_ATTRIBUTE;
    LINE_ENDINGS
    case '\t':
      if(env->normalize_attrs) {
        // Move the token to the buffer, to normalize it
        FAXPP_Error err = FAXPP_tokenizer_release_buffer(env, 0);
        if(err != NO_ERROR) return err;
        env->current_char = ' ';
      }
      break;
    default:
      DEFAULT_CASE;

      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0)
        goto NEXT_CHAR_RESTRICTED_CHAR_ERROR;
      break;
    }
    next_char(env);
  }

start_enum_state(PREFIX(ATTR_VALUE_QUOT_STATE));

  while(1) {
    if(env->position >= env->buffer_end) {
      if(env->token.value.ptr) {
        token_end_position(env);
        if(env->token.value.len != 0) {
          report_token(ATTRIBUTE_VALUE_TOKEN, env);
          return NO_ERROR;
        }
      }
      token_start_position(env);
      return PREMATURE_END_OF_BUFFER;
    }

    READ_CHAR;

    switch(env->current_char) {
    case '"':
      change_enum_state(env, PREFIX(START_ELEMENT_MANDATORY_WS_STATE));
      token_end_position(env);
      report_token(ATTRIBUTE_VALUE_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '&':
      store_state(env);
      env->state = reference_state;
      token_end_position(env);
      report_token(ATTRIBUTE_VALUE_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    case '<':
      next_char(env);
      return INVALID_CHAR_IN_ATTRIBUTE;
    LINE_ENDINGS
    case '\t': {
      if(env->normalize_attrs) {
        // Move the token to the buffer, to normalize it
        FAXPP_Error err = FAXPP_tokenizer_release_buffer(env, 0);
        if(err != NO_ERROR) return err;
        env->current_char = ' ';
      }
      break;
    }
    default:
      DEFAULT_CASE;

      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0)
        goto NEXT_CHAR_RESTRICTED_CHAR_ERROR;
      break;
    }
    next_char(env);
  }
