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

start_enum_state(PREFIX(START_ELEMENT_NAME_STATE));

  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      change_enum_state(env, PREFIX(START_ELEMENT_WS_STATE));
      return NO_ERROR;
    case '/':
      env->state = self_closing_element_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '>':
      env->state = start_element_end_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    case ':':
      change_enum_state(env, PREFIX(START_ELEMENT_NAME_SEEN_COLON_STATE));
      token_end_position(env);
      report_token(START_ELEMENT_PREFIX_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0) {
      return INVALID_CHAR_IN_ELEMENT_NAME;
    }
  }

start_enum_state(PREFIX(START_ELEMENT_NAME_SEEN_COLON_STATE));

  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default: 
    change_enum_state(env, PREFIX(START_ELEMENT_NAME_SEEN_COLON_STATE2));
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
  }
  // Fall through

start_enum_state(PREFIX(START_ELEMENT_NAME_SEEN_COLON_STATE2));

  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      change_enum_state(env, PREFIX(START_ELEMENT_WS_STATE));
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '/':
      env->state = self_closing_element_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '>':
      env->state = start_element_end_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
  }

start_enum_state(PREFIX(START_ELEMENT_MANDATORY_WS_STATE));

  END_CHECK;

  READ_CHAR;

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    // Fall through
  case '/':
  case '>':
    change_enum_state(env, PREFIX(START_ELEMENT_WS_STATE));
    break;
  default:
    change_enum_state(env, PREFIX(START_ELEMENT_WS_STATE));
    return EXPECTING_WHITESPACE;
  }
  // Fall through

start_enum_state(PREFIX(START_ELEMENT_WS_STATE));

  END_CHECK;

  READ_CHAR;

  switch(env->current_char) {
  WHITESPACE:
    goto NEXT_CHAR;
  case '/':
    env->state = self_closing_element_state;
    goto NEXT_CHAR_RETURN;
  case '>':
    change_enum_state(env, env->element_content_enum_state);
    env->nesting_level += 1;
    report_empty_token(START_ELEMENT_END_TOKEN, env);
    goto NEXT_CHAR_TOK_START_RETURN;
  case 'x':
    env->state = PREFIX(ns_name_state1);
    token_start_position(env);
    goto NEXT_CHAR_RETURN;
  default:
    DEFAULT_CASE;

    change_enum_state(env, PREFIX(ATTR_NAME_STATE));
    token_start_position(env);
    next_char(env);

    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ATTRIBUTE_NAME;

    goto_enum_state(PREFIX(ATTR_NAME_STATE));
  }

start_enum_state(PREFIX(ELEMENT_CONTENT_RSQUARE_STATE1));

  if(env->position >= env->buffer_end) {
    if(env->token.value.ptr) {
      token_end_position(env);
      if(env->token.value.len != 0) {
        report_token(CHARACTERS_TOKEN, env);
        return NO_ERROR;
      }
    }
    token_start_position(env);
    return PREMATURE_END_OF_BUFFER;
  }

  READ_CHAR;

  switch(env->current_char) {
  case ']':
    change_enum_state(env, PREFIX(ELEMENT_CONTENT_RSQUARE_STATE2));
    next_char(env);
    break;
  default:
    change_enum_state(env, env->element_content_enum_state);
    goto START;
  }
  // Fall through

start_enum_state(PREFIX(ELEMENT_CONTENT_RSQUARE_STATE2));

  if(env->position >= env->buffer_end) {
    if(env->token.value.ptr) {
      token_end_position(env);
      if(env->token.value.len != 0) {
        report_token(CHARACTERS_TOKEN, env);
        return NO_ERROR;
      }
    }
    token_start_position(env);
    return PREMATURE_END_OF_BUFFER;
  }

  READ_CHAR;

  switch(env->current_char) {
  case '>':
    change_enum_state(env, env->element_content_enum_state);
    next_char(env);
    return CDATA_END_IN_ELEMENT_CONTENT;;
  case ']':
    next_char(env);
    break;
  default:
    change_enum_state(env, env->element_content_enum_state);
    break;
  }
  goto START;

start_enum_state(PREFIX(ELEMENT_CONTENT_MARKUP_STATE));

  END_CHECK;

  READ_CHAR;

  switch(env->current_char) {
  case '?':
    env->state = pi_name_start_state;
    goto NEXT_CHAR_TOK_START_RETURN;
  case '!':
    env->state = cdata_or_comment_state;
    goto NEXT_CHAR_TOK_START_RETURN;
  case '/':
    next_char(env);
    token_start_position(env);
    change_enum_state(env, PREFIX(END_ELEMENT_NAME_STATE));
    goto_enum_state(PREFIX(END_ELEMENT_NAME_STATE));
  LINE_ENDINGS
  default:
    DEFAULT_CASE;

    change_enum_state(env, PREFIX(START_ELEMENT_NAME_STATE));
    token_start_position(env);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
    goto_enum_state(PREFIX(START_ELEMENT_NAME_STATE));
  }

start_enum_state(PREFIX(END_ELEMENT_NAME_STATE));

  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    change_enum_state(env, PREFIX(END_ELEMENT_NAME_STATE2));
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
  }
  // Fall through

start_enum_state(PREFIX(END_ELEMENT_NAME_STATE2));

  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      env->state = end_element_ws_state;
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '>':
      env->nesting_level -= 1;
      base_state(env);
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    case ':':
      change_enum_state(env, PREFIX(END_ELEMENT_NAME_SEEN_COLON_STATE));
      token_end_position(env);
      report_token(END_ELEMENT_PREFIX_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0) {
      return INVALID_CHAR_IN_ELEMENT_NAME;
    }
  }

start_enum_state(PREFIX(END_ELEMENT_NAME_SEEN_COLON_STATE));

  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    change_enum_state(env, PREFIX(END_ELEMENT_NAME_SEEN_COLON_STATE2));
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
    break;
  }
  // Fall through

start_enum_state(PREFIX(END_ELEMENT_NAME_SEEN_COLON_STATE2));

  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      env->state = end_element_ws_state;
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_RETURN;
    case '>':
      env->nesting_level -= 1;
      base_state(env);
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      goto NEXT_CHAR_TOK_START_RETURN;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
  }

