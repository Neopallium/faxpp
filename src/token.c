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

#include <faxpp/token.h>

#ifdef DEBUG
const char *
token_to_string(Token *token)
{
  switch(token->token) {
  case START_ELEMENT_NAME_TOKEN:
    return "START_ELEMENT_NAME_TOKEN";
  case START_ELEMENT_END_TOKEN:
    return "START_ELEMENT_END_TOKEN";
  case SELF_CLOSING_ELEMENT_TOKEN:
    return "SELF_CLOSING_ELEMENT_TOKEN";
  case END_ELEMENT_NAME_TOKEN:
    return "END_ELEMENT_NAME_TOKEN";
  case ATTRIBUTE_NAME_TOKEN:
    return "ATTRIBUTE_NAME_TOKEN";
  case ATTRIBUTE_VALUE_TOKEN:
    return "ATTRIBUTE_VALUE_TOKEN";
  case CHARACTERS_TOKEN:
    return "CHARACTERS_TOKEN";
  case COMMENT_TOKEN:
    return "COMMENT_TOKEN";
  case PI_NAME_TOKEN:
    return "PI_NAME_TOKEN";
  case PI_VALUE_TOKEN:
    return "PI_VALUE_TOKEN";
  case IGNORABLE_WHITESPACE_TOKEN:
    return "IGNORABLE_WHITESPACE_TOKEN";
  case END_OF_BUFFER_TOKEN:
    return "END_OF_BUFFER_TOKEN";
  case START_ELEMENT_PREFIX_TOKEN:
    return "START_ELEMENT_PREFIX_TOKEN";
  case END_ELEMENT_PREFIX_TOKEN:
    return "END_ELEMENT_PREFIX_TOKEN";
  case ATTRIBUTE_PREFIX_TOKEN:
    return "ATTRIBUTE_PREFIX_TOKEN";
  case ENTITY_REFERENCE_TOKEN:
    return "ENTITY_REFERENCE_TOKEN";
  case DEC_CHAR_REFERENCE_TOKEN:
    return "DEC_CHAR_REFERENCE_TOKEN";
  case HEX_CHAR_REFERENCE_TOKEN:
    return "HEX_CHAR_REFERENCE_TOKEN";
  case CDATA_TOKEN:
    return "CDATA_TOKEN";
  case XML_DECL_VERSION_TOKEN:
    return "XML_DECL_VERSION_TOKEN";
  case XML_DECL_ENCODING_TOKEN:
    return "XML_DECL_ENCODING_TOKEN";
  case XML_DECL_STANDALONE_TOKEN:
    return "XML_DECL_STANDALONE_TOKEN";
  case XMLNS_PREFIX_TOKEN:
    return "XMLNS_PREFIX_TOKEN";
  case XMLNS_NAME_TOKEN:
    return "XMLNS_NAME_TOKEN";
  }

  return "NO_TOKEN";
}
#endif
