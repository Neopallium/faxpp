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

#ifndef __FAXPP__ERROR_H
#define __FAXPP__ERROR_H

/*********************
 *
 *  Errors
 *
 *********************/

#define NO_ERROR 0
#define DOUBLE_DASH_IN_COMMENT 1
#define PREMATURE_END_OF_BUFFER 2
#define INVALID_START_OF_COMMENT 3
#define INVALID_CHAR_IN_START_ELEMENT 4
#define INVALID_CHAR_IN_ATTRIBUTE 5
#define INVALID_CHAR_IN_END_ELEMENT 6
#define NON_WHITESPACE_OUTSIDE_DOC_ELEMENT 7
#define BAD_ENCODING 8
#define UNSUPPORTED_ENCODING 9
#define ADDITIONAL_DOCUMENT_ELEMENT 10
#define INVALID_CHAR_IN_PI_NAME 11
#define INVALID_PI_NAME_OF_XML 12
#define INVALID_CHAR_IN_ELEMENT_NAME 13
#define INVALID_CHAR_IN_ATTRIBUTE_NAME 14
#define RESTRICTED_CHAR 15
#define INVALID_CHAR_IN_ENTITY_REFERENCE 16
#define INVALID_CHAR_IN_CHAR_REFERENCE 17
#define INVALID_CHAR_IN_XML_DECL 18
#define EXPECTING_EQUALS 19
#define EXPECTING_WHITESPACE 20
#define UNKNOWN_XML_VERSION 21
#define OUT_OF_MEMORY 22
#define ELEMENT_NAME_MISMATCH 23
#define NO_URI_FOR_PREFIX 24
#define DUPLICATE_ATTRIBUTES 25

typedef unsigned int TokenizerError;

const char *err_to_string(TokenizerError err);

#endif
