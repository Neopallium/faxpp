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

#ifndef __FAXPP__EVENT_H
#define __FAXPP__EVENT_H

#include "text.h"

typedef enum {
  NO_EVENT,
  START_DOCUMENT_EVENT,      ///< version?, encoding?, standalone?
  END_DOCUMENT_EVENT,        ///<
  START_ELEMENT_EVENT,       ///< prefix?, uri?, name, attrs?
  SELF_CLOSING_ELEMENT_EVENT,///< prefix?, uri?, name, attrs?
  END_ELEMENT_EVENT,         ///< prefix?, uri?, name
  CHARACTERS_EVENT,          ///< value
  CDATA_EVENT,               ///< value
  IGNORABLE_WHITESPACE_EVENT,///< value
  COMMENT_EVENT,             ///< value
  PI_EVENT,                  ///< name, value?
  ENTITY_REFERENCE_EVENT,    ///< name
  DEC_CHAR_REFERENCE_EVENT,  ///< name
  HEX_CHAR_REFERENCE_EVENT   ///< name
} FAXPP_EventType;

typedef struct FAXPP_AttrValue_s {
  FAXPP_EventType type;
  FAXPP_Text value;

  struct FAXPP_AttrValue_s *next;

  /// private
  struct FAXPP_AttrValue_s *dealloc_next;
} FAXPP_AttrValue;

typedef struct FAXPP_Attribute_s {
  FAXPP_Text prefix;
  FAXPP_Text uri;
  FAXPP_Text name;

  FAXPP_AttrValue value;

  unsigned int xmlns_attr:1;

  unsigned int line;
  unsigned int column;
} FAXPP_Attribute;

typedef struct FAXPP_Event_s {
  FAXPP_EventType type;

  FAXPP_Text prefix;
  FAXPP_Text uri;
  FAXPP_Text name;

  FAXPP_Text value;

  unsigned int attr_count;
  FAXPP_Attribute *attrs; ///< Array of attributes

  FAXPP_Text version;
  FAXPP_Text encoding;
  FAXPP_Text standalone;

  unsigned int line;
  unsigned int column;
} FAXPP_Event;

const char *FAXPP_event_to_string(FAXPP_EventType type);

#endif
