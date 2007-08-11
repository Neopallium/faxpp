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
xml_decl_or_markup_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '?':
    env->state = xml_decl_or_pi_state1;
    next_char(env);
    token_start_position(env);
    break;
  case '!':
    env->state = comment_start_state1;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    env->state = (env)->start_element_name_state;
    env->seen_doc_element = 1;
    token_start_position(env);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_or_pi_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'x':
    env->state = xml_decl_or_pi_state2;
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = pi_name_start_state;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_or_pi_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'm':
    env->state = xml_decl_or_pi_state3;
    next_char(env);
    break;
  WHITESPACE:
    env->state = pi_ws_state;
    token_end_position(env);
    report_token(PI_NAME_TOKEN, env);
    next_char(env);
    break;
  case '?':
    env->state = pi_content_seen_question_state;
    token_end_position(env);
    report_token(PI_NAME_TOKEN, env);
    token_start_position(env);
    next_char(env);
    break;
  default:
    env->state = pi_name_x_state;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_or_pi_state3(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'l':
    env->state = xml_decl_or_pi_state4;
    next_char(env);
    break;
  WHITESPACE:
    env->state = pi_ws_state;
    token_end_position(env);
    report_token(PI_NAME_TOKEN, env);
    next_char(env);
    break;
  case '?':
    env->state = pi_content_seen_question_state;
    token_end_position(env);
    report_token(PI_NAME_TOKEN, env);
    token_start_position(env);
    next_char(env);
    break;
  default:
    env->state = pi_name_m_state;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_or_pi_state4(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    env->stored_state = xml_decl_version_state1;
    env->state = ws_state;
    token_end_position(env);
    next_char(env);
    break;
  case '?':
    env->state = xml_decl_seen_question_state;
    token_end_position(env);
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  default:
    env->state = pi_name_state;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_PI_NAME;
    break;
  }
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

SINGLE_CHAR_STATE(xml_decl_version_state1, 'v', 0, xml_decl_version_state2, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_version_state2, 'e', 0, xml_decl_version_state3, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_version_state3, 'r', 0, xml_decl_version_state4, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_version_state4, 's', 0, xml_decl_version_state5, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_version_state5, 'i', 0, xml_decl_version_state6, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_version_state6, 'o', 0, xml_decl_version_state7, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_version_state7, 'n', xml_decl_version_value_state1, equals_state, INVALID_CHAR_IN_XML_DECL)

FAXPP_Error
xml_decl_version_value_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '"':
    env->stored_state = xml_decl_version_value_quot_state;
    env->state = xml_decl_version_value_state2;
    next_char(env);
    token_start_position(env);
    break;
  case '\'':
    env->stored_state = xml_decl_version_value_apos_state;
    env->state = xml_decl_version_value_state2;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;  
}

SINGLE_CHAR_STATE(xml_decl_version_value_state2, '1', 0, xml_decl_version_value_state3, UNKNOWN_XML_VERSION)
SINGLE_CHAR_STATE(xml_decl_version_value_state3, '.', 0, xml_decl_version_value_state4, UNKNOWN_XML_VERSION)

FAXPP_Error
xml_decl_version_value_state4(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '0':
    retrieve_state(env);
    break;
  case '1':
    retrieve_state(env);
    env->ncname_start_char = NCNAME_START_CHAR11;
    env->ncname_char = NCNAME_CHAR11;
    env->non_restricted_char = NON_RESTRICTED_CHAR11;
    break;
  default:
    retrieve_state(env);
    return UNKNOWN_XML_VERSION;
  }
  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
xml_decl_version_value_quot_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '"':
    env->state = xml_decl_encoding_ws_state;
    token_end_position(env);
    report_token(XML_DECL_VERSION_TOKEN, env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return UNKNOWN_XML_VERSION;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_version_value_apos_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    env->state = xml_decl_encoding_ws_state;
    token_end_position(env);
    report_token(XML_DECL_VERSION_TOKEN, env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return UNKNOWN_XML_VERSION;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_encoding_ws_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    env->stored_state = xml_decl_encoding_state1;
    env->state = ws_state;
    next_char(env);
    break;
  case '?':
    env->state = xml_decl_seen_question_state;
    token_start_position(env);
    next_char(env);
    break;
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_encoding_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  case '?':
    env->state = xml_decl_seen_question_state;
    token_start_position(env);
    next_char(env);
    break;
  case 's':
    env->state = xml_decl_standalone_state2;
    next_char(env);
    break;
  case 'e':
    env->state = xml_decl_encoding_state2;
    next_char(env);
    break;
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

SINGLE_CHAR_STATE(xml_decl_encoding_state2, 'n', 0, xml_decl_encoding_state3, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_encoding_state3, 'c', 0, xml_decl_encoding_state4, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_encoding_state4, 'o', 0, xml_decl_encoding_state5, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_encoding_state5, 'd', 0, xml_decl_encoding_state6, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_encoding_state6, 'i', 0, xml_decl_encoding_state7, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_encoding_state7, 'n', 0, xml_decl_encoding_state8, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_encoding_state8, 'g', xml_decl_encoding_value_state1, equals_state, INVALID_CHAR_IN_XML_DECL)

FAXPP_Error
xml_decl_encoding_value_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '"':
    env->stored_state = xml_decl_encoding_value_quot_state;
    env->state = xml_decl_encoding_value_state2;
    next_char(env);
    token_start_position(env);
    break;
  case '\'':
    env->stored_state = xml_decl_encoding_value_apos_state;
    env->state = xml_decl_encoding_value_state2;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;  
}

////////////////////////////////////////////////////////////////////////////////////////////////////

FAXPP_Error
xml_decl_encoding_value_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':           case 'J': case 'K': case 'L': case 'M':
  case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':           case 'V': case 'W': case 'X': case 'Y': case 'Z':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':           case 'j': case 'k': case 'l': case 'm':
  case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':           case 'v': case 'w': case 'x': case 'y': case 'z':
    retrieve_state(env);
    break;
  case 'U':
  case 'u':
    env->state = xml_decl_encoding_value_utf_state1;
    break;
  case 'I':
  case 'i':
    env->state = xml_decl_encoding_value_iso_state1;
    break;
  case '"':
  case '\'':
    retrieve_state(env);
    return INVALID_ENCODING_VALUE;
  LINE_ENDINGS
  default:
    retrieve_state(env);
    next_char(env);
    return INVALID_ENCODING_VALUE;
  }
  next_char(env);
  return NO_ERROR;
}

#define ENCODING_VALUE_STATE1(name, char1, next_state) \
FAXPP_Error \
name(FAXPP_TokenizerEnv *env) \
{ \
  read_char(env); \
\
  switch(env->current_char) { \
  case (char1): \
    env->state = (next_state); \
    next_char(env); \
    break; \
  default: \
    retrieve_state(env); \
    break; \
  } \
  return NO_ERROR; \
}

#define ENCODING_VALUE_STATE2(name, char1, char2, next_state) \
FAXPP_Error \
name(FAXPP_TokenizerEnv *env) \
{ \
  read_char(env); \
\
  switch(env->current_char) { \
  case (char1): \
  case (char2): \
    env->state = (next_state); \
    next_char(env); \
    break; \
  default: \
    retrieve_state(env); \
    break; \
  } \
  return NO_ERROR; \
}

ENCODING_VALUE_STATE2(xml_decl_encoding_value_utf_state1, 'T', 't', xml_decl_encoding_value_utf_state2)
ENCODING_VALUE_STATE2(xml_decl_encoding_value_utf_state2, 'F', 'f', xml_decl_encoding_value_utf_state3)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_utf_state3, '-',      xml_decl_encoding_value_utf_state4)

FAXPP_Error
xml_decl_encoding_value_utf_state4(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '1':
    env->state = xml_decl_encoding_value_utf16_state1;
    next_char(env);
    break;
  case '8':
    env->state = xml_decl_encoding_value_utf8_state;
    next_char(env);
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_encoding_value_utf8_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    if(env->stored_state == xml_decl_encoding_value_apos_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF8_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  case '"':
    if(env->stored_state == xml_decl_encoding_value_quot_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF8_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

ENCODING_VALUE_STATE1(xml_decl_encoding_value_utf16_state1, '6', xml_decl_encoding_value_utf16_state2)

FAXPP_Error
xml_decl_encoding_value_utf16_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    if(env->stored_state == xml_decl_encoding_value_apos_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF16_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  case '"':
    if(env->stored_state == xml_decl_encoding_value_quot_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF16_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  case 'B':
  case 'b':
    env->state = xml_decl_encoding_value_utf16be_state1;
    next_char(env);
    break;
  case 'L':
  case 'l':
    env->state = xml_decl_encoding_value_utf16le_state1;
    next_char(env);
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

ENCODING_VALUE_STATE2(xml_decl_encoding_value_utf16be_state1, 'E', 'e', xml_decl_encoding_value_utf16be_state2)

FAXPP_Error
xml_decl_encoding_value_utf16be_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    if(env->stored_state == xml_decl_encoding_value_apos_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF16BE_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  case '"':
    if(env->stored_state == xml_decl_encoding_value_quot_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF16BE_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

ENCODING_VALUE_STATE2(xml_decl_encoding_value_utf16le_state1, 'E', 'e', xml_decl_encoding_value_utf16le_state2)

FAXPP_Error
xml_decl_encoding_value_utf16le_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    if(env->stored_state == xml_decl_encoding_value_apos_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF16LE_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  case '"':
    if(env->stored_state == xml_decl_encoding_value_quot_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UTF16LE_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

// ISO-10646-UCS-4
ENCODING_VALUE_STATE2(xml_decl_encoding_value_iso_state1,  'S', 's', xml_decl_encoding_value_iso_state2)
ENCODING_VALUE_STATE2(xml_decl_encoding_value_iso_state2,  'O', 'o', xml_decl_encoding_value_iso_state3)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_iso_state3,  '-',      xml_decl_encoding_value_iso_state4)

FAXPP_Error
xml_decl_encoding_value_iso_state4(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '1':
    env->state = xml_decl_encoding_value_ucs4_state1;
    next_char(env);
    break;
  case '8':
    env->state = xml_decl_encoding_value_iso_8859_1_state1;
    next_char(env);
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state1,  '1',      xml_decl_encoding_value_ucs4_state2)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state2,  '0',      xml_decl_encoding_value_ucs4_state3)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state3,  '6',      xml_decl_encoding_value_ucs4_state4)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state4,  '4',      xml_decl_encoding_value_ucs4_state5)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state5,  '6',      xml_decl_encoding_value_ucs4_state6)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state6,  '-',      xml_decl_encoding_value_ucs4_state7)
ENCODING_VALUE_STATE2(xml_decl_encoding_value_ucs4_state7,  'U', 'u', xml_decl_encoding_value_ucs4_state8)
ENCODING_VALUE_STATE2(xml_decl_encoding_value_ucs4_state8,  'C', 'c', xml_decl_encoding_value_ucs4_state9)
ENCODING_VALUE_STATE2(xml_decl_encoding_value_ucs4_state9,  'S', 's', xml_decl_encoding_value_ucs4_state10)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state10, '-',      xml_decl_encoding_value_ucs4_state11)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_ucs4_state11, '4',      xml_decl_encoding_value_ucs4_state12)

FAXPP_Error
xml_decl_encoding_value_ucs4_state12(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    if(env->stored_state == xml_decl_encoding_value_apos_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UCS4_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  case '"':
    if(env->stored_state == xml_decl_encoding_value_quot_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_UCS4_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

ENCODING_VALUE_STATE1(xml_decl_encoding_value_iso_8859_1_state1,  '8',      xml_decl_encoding_value_iso_8859_1_state2)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_iso_8859_1_state2,  '5',      xml_decl_encoding_value_iso_8859_1_state3)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_iso_8859_1_state3,  '9',      xml_decl_encoding_value_iso_8859_1_state4)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_iso_8859_1_state4,  '-',      xml_decl_encoding_value_iso_8859_1_state5)
ENCODING_VALUE_STATE1(xml_decl_encoding_value_iso_8859_1_state5,  '1',      xml_decl_encoding_value_iso_8859_1_state6)

FAXPP_Error
xml_decl_encoding_value_iso_8859_1_state6(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    if(env->stored_state == xml_decl_encoding_value_apos_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_ISO_8859_1_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  case '"':
    if(env->stored_state == xml_decl_encoding_value_quot_state) {
      env->state = xml_decl_standalone_ws_state;
      token_end_position(env);
      report_token(XML_DECL_ENCODING_ISO_8859_1_TOKEN, env);
      next_char(env);
    }
    else {
      retrieve_state(env);
    }
    break;
  default:
    retrieve_state(env);
    break;
  }
  return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

FAXPP_Error
xml_decl_encoding_value_quot_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '"':
    env->state = xml_decl_standalone_ws_state;
    token_end_position(env);
    report_token(XML_DECL_ENCODING_TOKEN, env);
    break;
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
  case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
  case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case '_': case '-':
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_ENCODING_VALUE;
  }
  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
xml_decl_encoding_value_apos_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    env->state = xml_decl_standalone_ws_state;
    token_end_position(env);
    report_token(XML_DECL_ENCODING_TOKEN, env);
    break;
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
  case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
  case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case '_': case '-':
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_ENCODING_VALUE;
  }
  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
xml_decl_standalone_ws_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    env->state = xml_decl_standalone_state1;
    next_char(env);
    break;
  case '?':
    env->state = xml_decl_seen_question_state;
    token_start_position(env);
    next_char(env);
    break;
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_standalone_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  case '?':
    env->state = xml_decl_seen_question_state;
    token_start_position(env);
    next_char(env);
    break;
  case 's':
    env->state = xml_decl_standalone_state2;
    next_char(env);
    break;
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

SINGLE_CHAR_STATE(xml_decl_standalone_state2, 't', 0, xml_decl_standalone_state3, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state3, 'a', 0, xml_decl_standalone_state4, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state4, 'n', 0, xml_decl_standalone_state5, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state5, 'd', 0, xml_decl_standalone_state6, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state6, 'a', 0, xml_decl_standalone_state7, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state7, 'l', 0, xml_decl_standalone_state8, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state8, 'o', 0, xml_decl_standalone_state9, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state9, 'n', 0, xml_decl_standalone_state10, INVALID_CHAR_IN_XML_DECL)
SINGLE_CHAR_STATE(xml_decl_standalone_state10, 'e', xml_decl_standalone_value_state1, equals_state, INVALID_CHAR_IN_XML_DECL)

FAXPP_Error
xml_decl_standalone_value_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '"':
    env->stored_state = xml_decl_standalone_value_quot_state;
    env->state = xml_decl_standalone_value_state2;
    next_char(env);
    token_start_position(env);
    break;
  case '\'':
    env->stored_state = xml_decl_standalone_value_apos_state;
    env->state = xml_decl_standalone_value_state2;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;  
}

FAXPP_Error
xml_decl_standalone_value_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'y':
    env->state = xml_decl_standalone_yes_state1;
    break;
  case 'n':
    env->state = xml_decl_standalone_no_state;
    break;
  LINE_ENDINGS
  default:
    retrieve_state(env);
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  next_char(env);
  return NO_ERROR;
}

SINGLE_CHAR_STATE(xml_decl_standalone_yes_state1, 'e', 0, xml_decl_standalone_yes_state2, INVALID_CHAR_IN_XML_DECL)

FAXPP_Error
xml_decl_standalone_yes_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 's':
    retrieve_state(env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    retrieve_state(env);
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_standalone_no_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'o':
    retrieve_state(env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    retrieve_state(env);
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_standalone_value_quot_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '"':
    env->stored_state = xml_decl_question_state;
    env->state = ws_state;
    token_end_position(env);
    report_token(XML_DECL_STANDALONE_TOKEN, env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_standalone_value_apos_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '\'':
    env->stored_state = xml_decl_question_state;
    env->state = ws_state;
    token_end_position(env);
    report_token(XML_DECL_STANDALONE_TOKEN, env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_question_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  case '?':
    env->state = xml_decl_seen_question_state;
    token_start_position(env);
    next_char(env);
    break;
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

FAXPP_Error
xml_decl_seen_question_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '>':
    env->state = initial_misc_state;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_CHAR_IN_XML_DECL;
  }
  return NO_ERROR;
}

