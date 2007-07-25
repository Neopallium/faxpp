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

#include <string.h>
#include <stdlib.h>

#include <faxpp/xml_parser.h>
#include "tokenizer_states.h"

#define INITIAL_ATTRS_SIZE 4
#define INITIAL_NS_SIZE 6

/// Must be at least 4 for encoding sniffing
/// Must be a multiple of 4
#define READ_BUFFER_SIZE 16 * 1024

#define INITIAL_EVENT_BUFFER_SIZE 256
#define INITIAL_STACK_BUFFER_SIZE 1024

static TokenizerError nc_start_document_next_event(ParserEnv *env);
static TokenizerError nc_next_event(ParserEnv *env);

static TokenizerError wf_start_document_next_event(ParserEnv *env);
static TokenizerError wf_next_event(ParserEnv *env);

TokenizerError init_parser(ParserEnv *env, ParseMode mode, EncodeFunction encode)
{
  TokenizerError err;

  memset(env, 0, sizeof(ParserEnv));

  env->mode = mode;
  env->encode = encode;

  /* The next_event field is set in p_reset_parser() */

  env->max_attr_count = INITIAL_ATTRS_SIZE;
  env->attrs = (Attribute*)malloc(sizeof(Attribute) * INITIAL_ATTRS_SIZE);
  if(!env->attrs) return OUT_OF_MEMORY;

  err = init_buffer(&env->event_buffer, INITIAL_EVENT_BUFFER_SIZE);
  if(err != 0) return err;

  err = init_buffer(&env->stack_buffer, INITIAL_STACK_BUFFER_SIZE);
  if(err != 0) return err;

  return init_tokenizer(&env->tenv);
}

TokenizerError free_parser(ParserEnv *env)
{
  AttrValue *at;
  ElementInfo *el;
  NamespaceInfo *ns;

  if(env->attrs) free(env->attrs);

  while(env->av_dealloc) {
    at = env->av_dealloc;
    env->av_dealloc = at->dealloc_next;
    free(at);
  }

  while(env->element_info_stack) {
    el = env->element_info_stack;
    env->element_info_stack = el->prev;
    free(el);
  }

  while(env->namespace_stack) {
    ns = env->namespace_stack;
    env->namespace_stack = ns->prev;
    free(ns);
  }

  while(env->element_info_pool) {
    el = env->element_info_pool;
    env->element_info_pool = el->prev;
    free(el);
  }

  while(env->namespace_pool) {
    ns = env->namespace_pool;
    env->namespace_pool = ns->prev;
    free(ns);
  }

  if(env->read_buffer) free(env->read_buffer);

  free_buffer(&env->event_buffer);
  free_buffer(&env->stack_buffer);

  return free_tokenizer(&env->tenv);
}

static TokenizerError p_reset_parser(ParserEnv *env, int allocate_buffer)
{
  // Reset the stack buffer cursor
  reset_buffer(&env->stack_buffer);

  env->buffered_token = 0;

  env->tenv.result_token = &env->token;

  if(allocate_buffer && !env->read_buffer) {
    env->read_buffer = malloc(READ_BUFFER_SIZE);
    if(!env->read_buffer) return OUT_OF_MEMORY;
    env->read_buffer_length = READ_BUFFER_SIZE;
  }

  switch(env->mode) {
  case NO_CHECKS_PARSE_MODE:
    env->next_event = nc_start_document_next_event;
    break;
  case WELL_FORMED_PARSE_MODE:
    env->next_event = wf_start_document_next_event;
    break;
  }

  return NO_ERROR;
}

TokenizerError init_parse(ParserEnv *env, void *buffer, unsigned int length)
{
  TokenizerError err = p_reset_parser(env, /*allocate_buffer*/0);
  if(err != 0) return err;

  env->read = 0;
  env->read_user_data = 0;

  return init_tokenize(&env->tenv, buffer, length, env->encode);
}

static unsigned int p_file_read_callback(void *userData, void *buffer, unsigned int length)
{
  return fread(buffer, 1, length, (FILE*)userData);
}

TokenizerError init_parse_file(ParserEnv *env, FILE *file)
{
  return init_parse_callback(env, p_file_read_callback, (void*)file);
}

TokenizerError init_parse_callback(ParserEnv *env, ReadCallback callback, void *userData)
{
  TokenizerError err = p_reset_parser(env, /*allocate_buffer*/1);
  if(err != 0) return err;

  env->read = callback;
  env->read_user_data = userData;

  unsigned int len = env->read(env->read_user_data, env->read_buffer, env->read_buffer_length);

  // TBD boolean for indicating this is the last buffer - jpcs
  return init_tokenize(&env->tenv, env->read_buffer, len, env->encode);
}

TokenizerError next_event(ParserEnv *env)
{
  return env->next_event(env);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void p_text_change_buffer(Buffer *buffer, void *newBuffer, Text *text)
{
  if(text->ptr >= buffer->buffer && text->ptr < (buffer->buffer + buffer->length)) {
    text->ptr += newBuffer - buffer->buffer;
  }
}

static void p_change_event_buffer(void *userData, Buffer *buffer, void *newBuffer)
{
  unsigned int i;
  AttrValue *atval;

  ParserEnv *env = (ParserEnv*)userData;

  p_text_change_buffer(buffer, newBuffer, &env->event.prefix);
  p_text_change_buffer(buffer, newBuffer, &env->event.uri);
  p_text_change_buffer(buffer, newBuffer, &env->event.name);
  p_text_change_buffer(buffer, newBuffer, &env->event.value);
  p_text_change_buffer(buffer, newBuffer, &env->event.version);
  p_text_change_buffer(buffer, newBuffer, &env->event.encoding);
  p_text_change_buffer(buffer, newBuffer, &env->event.standalone);

  for(i = 0; i < env->event.attr_count; ++i) {
    p_text_change_buffer(buffer, newBuffer, &env->event.attrs[i].prefix);
    p_text_change_buffer(buffer, newBuffer, &env->event.attrs[i].uri);
    p_text_change_buffer(buffer, newBuffer, &env->event.attrs[i].name);

    atval = &env->event.attrs[i].value;
    while(atval) {
      p_text_change_buffer(buffer, newBuffer, &atval->value);
      atval = atval->next;
    }
  }
}

static void p_change_stack_buffer(void *userData, Buffer *buffer, void *newBuffer)
{
  ParserEnv *env = (ParserEnv*)userData;

  ElementInfo *el = env->element_info_stack;
  while(el) {
    p_text_change_buffer(buffer, newBuffer, &el->prefix);
    p_text_change_buffer(buffer, newBuffer, &el->uri);
    p_text_change_buffer(buffer, newBuffer, &el->name);

    el->prev_stack_cursor += newBuffer - buffer->buffer;

    el = el->prev;
  }

  NamespaceInfo *ns = env->namespace_stack;
  while(ns) {
    p_text_change_buffer(buffer, newBuffer, &ns->prefix);
    p_text_change_buffer(buffer, newBuffer, &ns->uri);

    ns = ns->prev;
  }
}

#define p_move_text_to_event_buffer(env, text) \
{ \
  if((text)->ptr >= (env)->tenv.buffer && (text)->ptr < (env)->tenv.buffer_end) { \
    void *newPtr = (env)->event_buffer.cursor; \
    TokenizerError err = buffer_append(&(env)->event_buffer, (text)->ptr, (text)->len, p_change_event_buffer, (env)); \
    if((env)->null_terminate && err == 0) \
      err = buffer_append_ch(&(env)->event_buffer, (env)->encode, 0, p_change_event_buffer, (env)); \
    if(err != 0) return err; \
    (text)->ptr = newPtr; \
  } \
}

static TokenizerError p_read_more(ParserEnv *env)
{
  TokenizerError err;
  unsigned int len = 0;
  unsigned int i;
  AttrValue *atval;

  // Check if the partial token in the tokenizer needs copying to the token_buffer
  if(env->tenv.token.value.ptr >= env->tenv.buffer &&
     env->tenv.token.value.ptr < env->tenv.buffer_end) {
    void *token_start = env->tenv.token.value.ptr;

    // Find the length of the partial token
    unsigned int token_length = env->tenv.token.value.len;
    if(!token_length)
      token_length = env->tenv.position - env->tenv.token.value.ptr;

    // Re-position the token positions to point into the token_buffer
    reset_buffer(&env->tenv.token_buffer);
    env->tenv.token_position1 += env->tenv.token_buffer.cursor - token_start;
    env->tenv.token_position2 += env->tenv.token_buffer.cursor - token_start;
    env->tenv.token.value.ptr = env->tenv.token_buffer.cursor;
    env->tenv.token.value.len = token_length;

    err = buffer_append(&env->tenv.token_buffer, token_start, token_length,
                        change_token_buffer, &env->tenv);
    if(err != 0) return err;
  }

  // Copy any strings in the event which point to the old buffer
  // into the event_buffer
  p_move_text_to_event_buffer(env, &env->event.prefix);
  p_move_text_to_event_buffer(env, &env->event.uri);
  p_move_text_to_event_buffer(env, &env->event.name);
  p_move_text_to_event_buffer(env, &env->event.value);
  p_move_text_to_event_buffer(env, &env->event.version);
  p_move_text_to_event_buffer(env, &env->event.encoding);
  p_move_text_to_event_buffer(env, &env->event.standalone);

  for(i = 0; i < env->event.attr_count; ++i) {
    p_move_text_to_event_buffer(env, &env->event.attrs[i].prefix);
    p_move_text_to_event_buffer(env, &env->event.attrs[i].uri);
    p_move_text_to_event_buffer(env, &env->event.attrs[i].name);

    atval = &env->event.attrs[i].value;
    while(atval) {
      p_move_text_to_event_buffer(env, &atval->value);
      atval = atval->next;
    }
  }

  if(env->tenv.position < env->tenv.buffer_end) {
    // We're half way through a charcter, so we need to copy
    // the partial char to the begining of the buffer to keep
    // it for the next parse
    len = env->tenv.buffer_end - env->tenv.position;
    memmove(env->read_buffer, env->tenv.position, len);
  }

  len += env->read(env->read_user_data, env->read_buffer, env->read_buffer_length - len);

  return continue_tokenize(&env->tenv, env->read_buffer, len);
}

#define p_check_err(err, env) \
{ \
  if((err) != NO_ERROR) { \
    if((err) == PREMATURE_END_OF_BUFFER && (env)->read) { \
      (err) = p_read_more((env)); \
      if((err) != NO_ERROR) { \
        set_err_info_from_tokenizer((env)); \
        return (err); \
      } \
    } else { \
      set_err_info_from_tokenizer((env)); \
      return (err); \
    } \
  } \
}

#define BUF_SIZE 50

/* static void p_print_token(ParserEnv *env) */
/* { */
/*   char buf[BUF_SIZE + 1]; */
/*   if(env->token.value.ptr != 0) { */
/*     if(env->token.value.len > BUF_SIZE) { */
/*       strncpy(buf, env->token.value.ptr, BUF_SIZE - 3); */
/*       buf[BUF_SIZE - 3] = '.'; */
/*       buf[BUF_SIZE - 2] = '.'; */
/*       buf[BUF_SIZE - 1] = '.'; */
/*       buf[BUF_SIZE] = 0; */
/*     } */
/*     else { */
/*       strncpy(buf, env->token.value.ptr, env->token.value.len); */
/*       buf[env->token.value.len] = 0; */
/*     } */
/* #ifdef DEBUG */
/*     printf("%03d:%03d Token ID: %s, Token: \"%s\"\n", env->token.line, env->token.column, token_to_string(&env->token), buf); */
/* #else */
/*     printf("%03d:%03d Token ID: %d, Token: \"%s\"\n", env->token.line, env->token.column, env->token, buf); */
/* #endif */
/*   } */
/*   else { */
/* #ifdef DEBUG */
/*     printf("%03d:%03d Token ID: %s\n", env->token.line, env->token.column, token_to_string(&env->token)); */
/* #else */
/*     printf("%03d:%03d Token ID: %d\n", env->token.line, env->token.column, env->token); */
/* #endif */
/*   } */
/* } */

#define p_next_token(err, env) \
{ \
  if((env)->buffered_token) { \
    (env)->buffered_token = 0; \
  } else { \
    (env)->token.token = NO_TOKEN; \
    while((env)->token.token == NO_TOKEN) { \
      (err) = (env)->tenv.state(&(env)->tenv); \
      p_check_err((err), (env)); \
    } \
  } \
\
/*   p_print_token(env); */ \
}

#define p_set_text_from_text(text, o) \
{ \
  (text)->ptr = (o)->ptr; \
  (text)->len = (o)->len; \
}

#define p_copy_text_from_event(text, o, env) \
{ \
  /* Always copy the string to the stack_buffer, to avoid complications */ \
  /* when we have to swap buffers */ \
/*   if(((o)->ptr >= (env)->event_buffer.buffer && */ \
/*       (o)->ptr < ((env)->event_buffer.buffer + (env)->event_buffer.length)) || */ \
/*      (env)->null_terminate) { */ \
    (text)->ptr = (env)->stack_buffer.cursor; \
    (text)->len = (o)->len; \
    TokenizerError err = buffer_append(&(env)->stack_buffer, (o)->ptr, (o)->len, \
                                       p_change_stack_buffer, (env)); \
    if((env)->null_terminate && err == 0) \
      err = buffer_append_ch(&(env)->stack_buffer, (env)->encode, 0, p_change_stack_buffer, (env)); \
    if(err != 0) return err; \
/*   } else { */ \
/*     p_set_text_from_text((text), (o)); */ \
/*   } */ \
}

#define p_copy_text_from_token(text, env, useTokenBuffer) \
{ \
  /* TBD null terminate in tokenizer - jpcs */ \
  (text)->len = (env)->token.value.len; \
  if(((useTokenBuffer) || (env)->token.value.ptr != (env)->tenv.token_buffer.buffer) && !(env)->null_terminate) { \
    (text)->ptr = (env)->token.value.ptr; \
  } else { \
    (text)->ptr = (env)->event_buffer.cursor; \
    TokenizerError err = buffer_append(&(env)->event_buffer, (env)->token.value.ptr, (env)->token.value.len, \
                                       p_change_event_buffer, (env)); \
    if((env)->null_terminate && err == 0) \
      err = buffer_append_ch(&(env)->event_buffer, (env)->encode, 0, p_change_event_buffer, (env)); \
    if(err != 0) return err; \
  } \
}

#define p_set_location_from_token(env) \
{ \
  if((env)->event.line == 0) { \
    (env)->event.line = (env)->token.line; \
    (env)->event.column = (env)->token.column; \
  } \
}

static TokenizerError p_next_attr(ParserEnv *env)
{
  if(env->event.attr_count == env->max_attr_count) {
    env->max_attr_count = env->max_attr_count << 1;
    env->attrs = (Attribute*)realloc(env->attrs, sizeof(Attribute) * env->max_attr_count);
    if(!env->attrs) return OUT_OF_MEMORY;
  }
  env->event.attrs = env->attrs;
  env->current_attr = env->event.attrs + env->event.attr_count;
  env->event.attr_count += 1;

  memset(env->current_attr, 0, sizeof(Attribute));

  return NO_ERROR;
}

static AttrValue *p_next_attr_value(ParserEnv *env)
{
  AttrValue *atval;

  if(env->av_ptr) {
    atval = env->av_ptr;
    env->av_ptr = atval->dealloc_next;

    memset(atval, 0, sizeof(AttrValue));
    atval->dealloc_next = env->av_ptr;
  } else {
    atval = (AttrValue*)malloc(sizeof(AttrValue));
    if(!atval) return 0;

    memset(atval, 0, sizeof(AttrValue));
    atval->dealloc_next = env->av_dealloc;
    env->av_dealloc = atval;
  }

  return atval;
}

static void p_set_location_from_token_a(Attribute *attr, ParserEnv *env)
{
  if(attr->line == 0) {
    attr->line = env->token.line;
    attr->column = env->token.column;
  }
}

static TokenizerError p_set_attr_value(Attribute *attr, ParserEnv *env, EventType type)
{
  AttrValue *atval, *newatval;

  if(attr->value.value.ptr == 0) {
    p_copy_text_from_token(&attr->value.value, env, /*useTokenBuffer*/0);
    attr->value.type = type;
    return NO_ERROR;
  }

  newatval = p_next_attr_value(env);
  if(!newatval) return OUT_OF_MEMORY;

  p_copy_text_from_token(&newatval->value, env, /*useTokenBuffer*/0);
  newatval->type = type;

  /* Add newatval to the end of the linked list */
  atval = &attr->value;
  while(atval->next) atval = atval->next;
  atval->next = newatval;

  return NO_ERROR;
}

static void p_reset_event(ParserEnv *env)
{
  // Reset the attribute value store
  env->av_ptr = env->av_dealloc;

  // Reset the event buffer cursor
  reset_buffer(&env->event_buffer);

  // Clear the event
  env->event.type = NO_EVENT;

  env->event.prefix.ptr = 0;
  env->event.prefix.len = 0;
  env->event.uri.ptr = 0;
  env->event.uri.len = 0;
  env->event.name.ptr = 0;
  env->event.name.len = 0;
  env->event.value.ptr = 0;
  env->event.value.len = 0;

  env->event.attr_count = 0;

  env->event.version.ptr = 0;
  env->event.version.len = 0;
  env->event.encoding.ptr = 0;
  env->event.encoding.len = 0;
  env->event.standalone.ptr = 0;
  env->event.standalone.len = 0;

  env->event.line = 0;
}

static void set_err_info_from_tokenizer(ParserEnv *env)
{
  env->err_line = env->tenv.line;
  env->err_column = env->tenv.column;
}

static void set_err_info_from_event(ParserEnv *env)
{
  env->err_line = env->event.line;
  env->err_column = env->event.column;
}

static void set_err_info_from_attr(ParserEnv *env, const Attribute *attr)
{
  env->err_line = attr->line;
  env->err_column = attr->column;
}

static TokenizerError nc_start_document_next_event(ParserEnv *env)
{
  TokenizerError err = 0;

  p_reset_event(env);

  while(1) {
    p_next_token(err, env);

    switch(env->token.token) {
    case XML_DECL_VERSION_TOKEN:
      p_copy_text_from_token(&env->event.version, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case XML_DECL_ENCODING_TOKEN:
      // TBD invoke a callback function to change the transcoder
      p_copy_text_from_token(&env->event.encoding, env, /*useTokenBuffer*/0);
      break;
    case XML_DECL_STANDALONE_TOKEN:
      p_copy_text_from_token(&env->event.standalone, env, /*useTokenBuffer*/0);
      break;
    default:
      env->next_event = nc_next_event;
      env->buffered_token = 1;
      env->event.type = START_DOCUMENT_EVENT;
      return NO_ERROR;
    }
  }

  // Never happens
  return NO_ERROR;
}

static TokenizerError nc_next_event(ParserEnv *env)
{
  TokenizerError err = 0;

  p_reset_event(env);

  while(1) {
    p_next_token(err, env);

    switch(env->token.token) {
    case START_ELEMENT_PREFIX_TOKEN:
      p_copy_text_from_token(&env->event.prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case START_ELEMENT_NAME_TOKEN:
      p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case XMLNS_PREFIX_TOKEN:
      err = p_next_attr(env);
      if(err != 0) {
        set_err_info_from_tokenizer(env);
        return err;
      }

      p_copy_text_from_token(&env->current_attr->prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      env->current_attr->xmlns_attr = 1;
      break;
    case XMLNS_NAME_TOKEN:
      err = p_next_attr(env);
      if(err != 0) {
        set_err_info_from_tokenizer(env);
        return err;
      }

      p_copy_text_from_token(&env->current_attr->name, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      env->current_attr->xmlns_attr = 1;
      break;
    case ATTRIBUTE_PREFIX_TOKEN:
      err = p_next_attr(env);
      if(err != 0) {
        set_err_info_from_tokenizer(env);
        return err;
      }

      p_copy_text_from_token(&env->current_attr->prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      break;
    case ATTRIBUTE_NAME_TOKEN:
      if(!env->current_attr || env->current_attr->name.ptr != 0) {
        err = p_next_attr(env);
        if(err != 0) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      }
      p_copy_text_from_token(&env->current_attr->name, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      break;
    case ATTRIBUTE_VALUE_TOKEN:
      err = p_set_attr_value(env->current_attr, env, CHARACTERS_EVENT);
      if(err) {
        set_err_info_from_tokenizer(env);
        return err;
      }
      break;
    case START_ELEMENT_END_TOKEN:
      env->event.type = START_ELEMENT_EVENT;
      env->current_attr = 0;
      return NO_ERROR;
    case SELF_CLOSING_ELEMENT_TOKEN:
      env->event.type = SELF_CLOSING_ELEMENT_EVENT;
      env->current_attr = 0;
      return NO_ERROR;
    case END_ELEMENT_PREFIX_TOKEN:
      p_copy_text_from_token(&env->event.prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case END_ELEMENT_NAME_TOKEN:
      p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      env->event.type = END_ELEMENT_EVENT;
      return NO_ERROR;
    case CHARACTERS_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = CHARACTERS_EVENT;
      return NO_ERROR;
    case CDATA_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = CDATA_EVENT;
      return NO_ERROR;
    case IGNORABLE_WHITESPACE_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = IGNORABLE_WHITESPACE_EVENT;
      return NO_ERROR;
    case COMMENT_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = COMMENT_EVENT;
      return NO_ERROR;
    case PI_NAME_TOKEN:
      p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);

      p_next_token(err, env);

      if(env->token.token == PI_VALUE_TOKEN) {
        p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/0);
      } else {
        env->buffered_token = 1;
      }

      env->event.type = PI_EVENT;
      return NO_ERROR;
    case ENTITY_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value(env->current_attr, env, ENTITY_REFERENCE_EVENT);
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_location_from_token(env);
        env->event.type = ENTITY_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case DEC_CHAR_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value(env->current_attr, env, DEC_CHAR_REFERENCE_EVENT);
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_location_from_token(env);
        env->event.type = DEC_CHAR_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case HEX_CHAR_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value(env->current_attr, env, HEX_CHAR_REFERENCE_EVENT);
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_location_from_token(env);
        env->event.type = HEX_CHAR_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case END_OF_BUFFER_TOKEN:
      p_set_location_from_token(env);
      env->event.type = END_DOCUMENT_EVENT;
      return NO_ERROR;
    }
  }

  // Never happens
  return NO_ERROR;
}

#define p_compare_text(a, b) (((a)->len == (b)->len) ? memcmp((a)->ptr, (b)->ptr, (a)->len) : ((a)->len - (b)->len))

/* static int p_compare_text(const Text *a, const Text *b) */
/* { */
/*   int cmp = a->len - b->len; */
/*   if(cmp != 0) return cmp; */

/*   return memcmp(a->ptr, b->ptr, a->len); */
/* } */

static TokenizerError p_add_ns_info(ParserEnv *env, const Attribute *attr)
{
  NamespaceInfo *nsinfo = env->namespace_pool;

  if(nsinfo == 0) {
    nsinfo = (NamespaceInfo*)malloc(sizeof(NamespaceInfo));
    if(!nsinfo) return OUT_OF_MEMORY;
  }
  else {
    env->namespace_pool = nsinfo->prev;
  }

  memset(nsinfo, 0, sizeof(NamespaceInfo));
  nsinfo->prev = env->namespace_stack;
  env->namespace_stack = nsinfo;

  // TBD deal with entities in attribute values - jpcs
  p_copy_text_from_event(&nsinfo->uri, &(attr->value.value), env);

  if(attr->prefix.len != 0) {
    p_copy_text_from_event(&nsinfo->prefix, &attr->name, env);
  }

  return NO_ERROR;
}

#define p_find_ns_info(env, prefix, uri) ((env)->namespace_stack) ? p_find_ns_info_impl((env), (prefix), (uri)) : NO_ERROR

static TokenizerError p_find_ns_info_impl(const ParserEnv *env, const Text *prefix, Text *uri)
{
  const NamespaceInfo *nsinfo;

  nsinfo = env->namespace_stack;
  while(nsinfo != 0) {
    if(p_compare_text(prefix, &nsinfo->prefix) == 0) {
      p_set_text_from_text(uri, &nsinfo->uri);
      return NO_ERROR;
    }
    nsinfo = nsinfo->prev;
  }

  if(prefix->len == 0) {
    /* The default namespace is implicitly set to no namespace */
    return NO_ERROR;
  }

  return NO_URI_FOR_PREFIX;
}

static TokenizerError p_push_element(ParserEnv *env)
{
  ElementInfo *einfo = env->element_info_pool;

  if(einfo == 0) {
    einfo = (ElementInfo*)malloc(sizeof(ElementInfo));
    if(!einfo) return OUT_OF_MEMORY;
  }
  else {
    env->element_info_pool = einfo->prev; 
  }

  einfo->prev = env->element_info_stack;
  env->element_info_stack = einfo;

  /* Store the current place in the namespace stack */
  einfo->prev_ns = env->namespace_stack;

  /* Store the current place in the stack buffer */
  einfo->prev_stack_cursor = env->stack_buffer.cursor;

  einfo->uri.ptr = 0; einfo->uri.len = 0;
  p_copy_text_from_event(&einfo->prefix, &env->event.prefix, env);
  p_copy_text_from_event(&einfo->name, &env->event.name, env);

  return NO_ERROR;
}

static void p_pop_element(ParserEnv *env)
{
  ElementInfo *einfo;
  NamespaceInfo *ns;

  einfo = env->element_info_stack;

  /* Take the namespace_stack back to it's
     position before this element */
  while(env->namespace_stack != einfo->prev_ns) {
    ns = env->namespace_stack;
    env->namespace_stack = ns->prev;

    /* Put the NamespaceInfo object back in the pool */
    ns->prev = env->namespace_pool;
    env->namespace_pool = ns;
  }

  /* Return the stack_buffer back to it's state before this element */
  env->stack_buffer.cursor = einfo->prev_stack_cursor;

  env->element_info_stack = einfo->prev;

  /* Put the ElementInfo object back in the pool */
  einfo->prev = env->element_info_pool;
  env->element_info_pool = einfo;
}

static Char32 p_dec_char_ref_value(const Text *text, ParserEnv *env)
{
  Char32 ch, result = 0;

  void *text_ptr = text->ptr;
  void *text_end = text_ptr + text->len;

  while(text_ptr < text_end) {
    text_ptr += env->tenv.decode(text_ptr, text_end, &ch);
    result *= 10;
    if(ch <= '9' && ch >= '0')
      result += ch - '0';
  }

  return result;
}

static Char32 p_hex_char_ref_value(const Text *text, ParserEnv *env)
{
  Char32 ch, result = 0;

  void *text_ptr = text->ptr;
  void *text_end = text_ptr + text->len;

  while(text_ptr < text_end) {
    text_ptr += env->tenv.decode(text_ptr, text_end, &ch);
    result <<= 4;
    if(ch <= '9' && ch >= '0')
      result += ch - '0';
    else if(ch <= 'F' && ch >= 'A')
      result += ch - 'A' + 10;
    else if(ch <= 'f' && ch >= 'a')
      result += ch - 'a' + 10;
  }

  return result;
}

static TokenizerError wf_start_document_next_event(ParserEnv *env)
{
  TokenizerError err = nc_start_document_next_event(env);
  if(err != 0) return err;

  switch(env->event.type) {
  case START_DOCUMENT_EVENT:
    env->next_event = wf_next_event;
    break;
  default: break;
  }

  return err;
}

static TokenizerError wf_next_event(ParserEnv *env)
{
  int i, j;
  Attribute *attr, *attr2;
  AttrValue *attrVal;
  Char32 ch;

  TokenizerError err = nc_next_event(env);
  if(err != 0) return err;

  switch(env->event.type) {
  case START_DOCUMENT_EVENT:
    break;
  case END_DOCUMENT_EVENT:
    break;
  case START_ELEMENT_EVENT:
  case SELF_CLOSING_ELEMENT_EVENT:
    /* Store the element name */
    err = p_push_element(env);
    if(err != 0) {
      set_err_info_from_event(env);
      return err;
    }

    for(i = 0; i < env->event.attr_count; ++i) {
      attr = &env->event.attrs[i];

      /* Check that the character references are valid */
      for(attrVal = &attr->value; attrVal != 0; attrVal = attrVal->next) {
        switch(attrVal->type) {
        case DEC_CHAR_REFERENCE_EVENT:
          /* [WFC: Legal Character] */
          if((char_flags(p_dec_char_ref_value(&attrVal->value, env)) & NON_RESTRICTED_CHAR) == 0) {
            set_err_info_from_attr(env, attr);
            return RESTRICTED_CHAR;
          }
          break;
        case HEX_CHAR_REFERENCE_EVENT:
          /* [WFC: Legal Character] */
          if((char_flags(p_hex_char_ref_value(&attrVal->value, env)) & NON_RESTRICTED_CHAR) == 0) {
            set_err_info_from_attr(env, attr);
            return RESTRICTED_CHAR;
          }
          break;
        default: break;
        }
      }

      /* Check for namespace attributes */
      if(attr->xmlns_attr) {
        err = p_add_ns_info(env, attr);
        if(err != 0) {
          set_err_info_from_attr(env, attr);
          return err;
        }
      }
    }

    /* Resolve the element's URI */
    err = p_find_ns_info(env, &env->event.prefix, &env->event.uri);
    if(err != 0) {
      set_err_info_from_event(env);
      return err;
    }

    /* Copy the element's URI to the element stack */
    /* No need to use the stack_buffer, since the value already comes from there */
    p_set_text_from_text(&env->element_info_stack->uri, &env->event.uri);

    for(i = 0; i < env->event.attr_count; ++i) {
      attr = &env->event.attrs[i];
      /* Resolve the attributes' URIs */
      if(!attr->xmlns_attr && attr->prefix.len != 0) {
        err = p_find_ns_info(env, &attr->prefix, &attr->uri);
        if(err != 0) {
          set_err_info_from_attr(env, attr);
          return err;
        }
      }
    }

    for(i = 0; i < env->event.attr_count; ++i) {
      attr = &env->event.attrs[i];
      /* [WFC: Unique Att Spec] */
      /* Nested loop check for duplicate attributes */
      /* TBD make this a better algorithm? - jpcs */
      for(j = i + 1; j < env->event.attr_count; ++j) {
        attr2 = &env->event.attrs[j];
        if(attr2->xmlns_attr == attr->xmlns_attr &&
           p_compare_text(&attr2->name, &attr->name) == 0 &&
           p_compare_text(&attr2->uri, &attr->uri) == 0) {
          set_err_info_from_attr(env, attr);
          return DUPLICATE_ATTRIBUTES;
        }
      }
    }

    if(env->event.type == SELF_CLOSING_ELEMENT_EVENT) {
      /* Do this to remove the NamespaceInfo objects
         from the namespace stack */
      p_pop_element(env);
    }
    break;
  case END_ELEMENT_EVENT:
    /* [WFC: Element Type Match] */
    if(p_compare_text(&env->element_info_stack->name, &env->event.name) != 0 ||
       p_compare_text(&env->element_info_stack->prefix, &env->event.prefix) != 0) {
      set_err_info_from_event(env);
      err = ELEMENT_NAME_MISMATCH;
    }

    /* Copy the element's URI from the element stack */
    p_set_text_from_text(&env->event.uri, &env->element_info_stack->uri);

    p_pop_element(env);
    break;
  case CHARACTERS_EVENT:
    break;
  case CDATA_EVENT:
    break;
  case IGNORABLE_WHITESPACE_EVENT:
    break;
  case COMMENT_EVENT:
    break;
  case PI_EVENT:
    break;
  case ENTITY_REFERENCE_EVENT:
    /* [WFC: Entity Declared] */
    /* [WFC: Parsed Entity] */
    /* [WFC: No Recursion] */
    // TBD
    break;
  case DEC_CHAR_REFERENCE_EVENT:
    /* [WFC: Legal Character] */
    ch = p_dec_char_ref_value(&env->event.name, env);
    if((char_flags(ch) & NON_RESTRICTED_CHAR) == 0) {
      set_err_info_from_event(env);
      return RESTRICTED_CHAR;
    }

    env->event.value.ptr = env->event_buffer.cursor;
    err = buffer_append_ch(&env->event_buffer, env->encode, ch, p_change_event_buffer, (env));
    env->event.value.len = env->event_buffer.cursor - env->event.value.ptr;

    if(env->null_terminate && err == 0)
      err = buffer_append_ch(&env->event_buffer, env->encode, 0, p_change_event_buffer, (env));
    if(err != 0) return err;

    break;
  case HEX_CHAR_REFERENCE_EVENT:
    /* [WFC: Legal Character] */
    ch = p_hex_char_ref_value(&env->event.name, env);
    if((char_flags(ch) & NON_RESTRICTED_CHAR) == 0) {
      set_err_info_from_event(env);
      return RESTRICTED_CHAR;
    }


    env->event.value.ptr = env->event_buffer.cursor;
    err = buffer_append_ch(&env->event_buffer, env->encode, ch, p_change_event_buffer, (env));
    env->event.value.len = env->event_buffer.cursor - env->event.value.ptr;

    if(env->null_terminate && err == 0)
      err = buffer_append_ch(&env->event_buffer, env->encode, 0, p_change_event_buffer, (env));
    if(err != 0) return err;

    break;
  case NO_EVENT: break;
  }

  return err;
}

