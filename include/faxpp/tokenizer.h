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

#ifndef __FAXPP__TOKENIZER_H
#define __FAXPP__TOKENIZER_H

#include "error.h"
#include "transcode.h"
#include "token.h"

/**
 * \struct FAXPP_Tokenizer
 * The tokenizer structure. Details of the structure are private.
 *
 * \see tokenizer.h
 */

/// The tokenizer structure. Details of the structure are private.
typedef struct FAXPP_TokenizerEnv_s FAXPP_Tokenizer;

/**
 * Creates a tokenizer object
 *
 * \return A pointer to the tokenizer object, or 0 if out of memory.
 *
 * \relatesalso FAXPP_Tokenizer
 */
FAXPP_Tokenizer *FAXPP_create_tokenizer();
/**
 * Frees a tokenizer object
 *
 * \param tokenizer The tokenizer to free
 *
 * \relatesalso FAXPP_Tokenizer
 */
void FAXPP_free_tokenizer(FAXPP_Tokenizer *tokenizer);

/**
 * Initialize the tokenizer to tokenize the given buffer, returning strings
 * encoded using the given encoding function.
 *
 * \param tokenizer The tokenizer to initialize
 * \param buffer A pointer to the start of the buffer to tokenize
 * \param length The length of the given buffer
 * \param encode The encoding function to use when encoding token values
 *
 * \retval UNSUPPORTED_ENCODING If the encoding sniffing algorithm cannot recognize
 * the encoding of the buffer
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Tokenizer
 */
FAXPP_Error FAXPP_init_tokenize(FAXPP_Tokenizer *tokenizer, void *buffer,
                                unsigned int length, FAXPP_EncodeFunction encode);

/**
 * Instructs the tokenizer to release any dependencies it has on it's current buffer.
 *
 * This is typically called on recieving a PREMATURE_END_OF_BUFFER error, before
 * using FAXPP_continue_tokenize() to provide a new buffer. In this case, the buffer data
 * between *buffer_position and the end of the buffer need to be copied into the start of
 * the new buffer.
 *
 * \param tokenizer
 * \param[out] buffer_position Set to a pointer in the current buffer that the tokenizer
 * has tokenized up to
 *
 * \retval OUT_OF_MEMORY
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Tokenizer
 */
FAXPP_Error FAXPP_tokenizer_release_buffer(FAXPP_Tokenizer *tokenizer, void **buffer_position);

/**
 * Provides a new buffer for the tokenizer to continue tokenizing.
 *
 * FAXPP_tokenizer_release_buffer() should have been called before this,
 * and the remaining data in the old buffer transferred to the new one.
 * 
 * \param tokenizer The tokenizer to initialize
 * \param buffer A pointer to the start of the buffer to tokenize
 * \param length The length of the given buffer
 *
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Tokenizer
 */
FAXPP_Error FAXPP_continue_tokenize(FAXPP_Tokenizer *tokenizer, void *buffer,
                                    unsigned int length);

/**
 * Reads the next token from the buffer, placing the information for it
 * into the current token.
 * 
 * \param tokenizer
 *
 * \retval DOUBLE_DASH_IN_COMMENT
 * \retval PREMATURE_END_OF_BUFFER
 * \retval INVALID_START_OF_COMMENT
 * \retval INVALID_CHAR_IN_START_ELEMENT
 * \retval INVALID_CHAR_IN_ATTRIBUTE
 * \retval INVALID_CHAR_IN_END_ELEMENT
 * \retval NON_WHITESPACE_OUTSIDE_DOC_ELEMENT
 * \retval BAD_ENCODING
 * \retval UNSUPPORTED_ENCODING
 * \retval ADDITIONAL_DOCUMENT_ELEMENT
 * \retval INVALID_CHAR_IN_PI_NAME
 * \retval INVALID_PI_NAME_OF_XML
 * \retval INVALID_CHAR_IN_ELEMENT_NAME
 * \retval INVALID_CHAR_IN_ATTRIBUTE_NAME
 * \retval RESTRICTED_CHAR
 * \retval INVALID_CHAR_IN_ENTITY_REFERENCE
 * \retval INVALID_CHAR_IN_CHAR_REFERENCE
 * \retval INVALID_CHAR_IN_XML_DECL
 * \retval EXPECTING_EQUALS
 * \retval EXPECTING_WHITESPACE
 * \retval UNKNOWN_XML_VERSION
 * \retval OUT_OF_MEMORY
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Tokenizer
 */
FAXPP_Error FAXPP_next_token(FAXPP_Tokenizer *tokenizer);

/**
 * Returns the current token produced by the tokenizer when FAXPP_next_token() was called.
 * 
 * \param tokenizer
 * \return The current token
 *
 * \relatesalso FAXPP_Tokenizer
 */
const FAXPP_Token *FAXPP_get_current_token(const FAXPP_Tokenizer *tokenizer);

/**
 * Returns the current element nesting level in the XML document.
 * 
 * \param tokenizer
 * \return The current nesting level
 *
 * \relatesalso FAXPP_Tokenizer
 */
unsigned int FAXPP_get_tokenizer_nesting_level(const FAXPP_Tokenizer *tokenizer);

/**
 * Returns the line that the current error occured on.
 * 
 * \param tokenizer
 * \return The line number
 *
 * \relatesalso FAXPP_Tokenizer
 */
unsigned int FAXPP_get_tokenizer_error_line(const FAXPP_Tokenizer *tokenizer);

/**
 * Returns the column that the current error occured on.
 * 
 * \param tokenizer
 * \return The column number
 *
 * \relatesalso FAXPP_Tokenizer
 */
unsigned int FAXPP_get_tokenizer_error_column(const FAXPP_Tokenizer *tokenizer);

#endif
