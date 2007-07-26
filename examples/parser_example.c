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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#include <faxpp/parser.h>

#define BUFFER_SIZE 10
#define MSECS_IN_SECS 1000000

unsigned long getTime()
{
  struct timeval timev;
  gettimeofday(&timev, 0);

  return (timev.tv_sec * MSECS_IN_SECS) + timev.tv_usec;
}

void
output_text(const FAXPP_Text *text, FILE *stream)
{
  char *buffer = (char*)text->ptr;
  char *buffer_end = buffer + text->len;

  while(buffer < buffer_end) {
    putc(*buffer++, stream);
  }
}

#define SHOW_URIS 0

void
output_event(const FAXPP_Event *event, FILE *stream)
{
  int i;
  FAXPP_AttrValue *atval;

  switch(event->type) {
  case START_DOCUMENT_EVENT:
    if(event->version.ptr != 0) {
      fprintf(stream, "<?xml version=\"");
      output_text(&event->version, stream);
      if(event->encoding.ptr != 0) {
        fprintf(stream, "\" encoding=\"");
        output_text(&event->encoding, stream);
      }
      if(event->standalone.ptr != 0) {
        fprintf(stream, "\" standalone=\"");
        output_text(&event->standalone, stream);
      }
      fprintf(stream, "\"?>");
    }
    break;
  case END_DOCUMENT_EVENT:
    break;
  case START_ELEMENT_EVENT:
  case SELF_CLOSING_ELEMENT_EVENT:
    fprintf(stream, "<");
#if SHOW_URIS
    if(event->uri.ptr != 0) {
      fprintf(stream, "{");
      output_text(&event->uri, stream);
      fprintf(stream, "}");
    } else
#endif
    if(event->prefix.ptr != 0) {
      output_text(&event->prefix, stream);
      fprintf(stream, ":");
    }
    output_text(&event->name, stream);

    for(i = 0; i < event->attr_count; ++i) {
      fprintf(stream, " ");
#if SHOW_URIS
      if(event->attrs[i].uri.ptr != 0) {
        fprintf(stream, "{");
        output_text(&event->attrs[i].uri, stream);
        fprintf(stream, "}");
      } else
#endif
      if(event->attrs[i].prefix.ptr != 0) {
        output_text(&event->attrs[i].prefix, stream);
        fprintf(stream, ":");
      }
      output_text(&event->attrs[i].name, stream);
      fprintf(stream, "=\"");

      atval = &event->attrs[i].value;
      while(atval) {
        switch(atval->type) {
        case CHARACTERS_EVENT:
          output_text(&atval->value, stream);
          break;
        case ENTITY_REFERENCE_EVENT:
          fprintf(stream, "&");
          output_text(&atval->value, stream);
          fprintf(stream, ";");
          break;
        case DEC_CHAR_REFERENCE_EVENT:
          fprintf(stream, "&#");
          output_text(&atval->value, stream);
          fprintf(stream, ";");
          break;
        case HEX_CHAR_REFERENCE_EVENT:
          fprintf(stream, "&#x");
          output_text(&atval->value, stream);
          fprintf(stream, ";");
          break;
        default:
          break;
        }
        atval = atval->next;
      }

      fprintf(stream, "\"");
    }

    if(event->type == SELF_CLOSING_ELEMENT_EVENT)
      fprintf(stream, "/>");
    else
      fprintf(stream, ">");
    break;
  case END_ELEMENT_EVENT:
    fprintf(stream, "</");
#if SHOW_URIS
    if(event->uri.ptr != 0) {
      fprintf(stream, "{");
      output_text(&event->uri, stream);
      fprintf(stream, "}");
    } else
#endif
    if(event->prefix.ptr != 0) {
      output_text(&event->prefix, stream);
      fprintf(stream, ":");
    }
    output_text(&event->name, stream);
    fprintf(stream, ">");
    break;
  case CHARACTERS_EVENT:
    output_text(&event->value, stream);
    break;
  case CDATA_EVENT:
    fprintf(stream, "<![CDATA[");
    output_text(&event->value, stream);
    fprintf(stream, "]]>");
    break;
  case IGNORABLE_WHITESPACE_EVENT:
    output_text(&event->value, stream);
    break;
  case COMMENT_EVENT:
    fprintf(stream, "<!--");
    output_text(&event->value, stream);
    fprintf(stream, "-->");
    break;
  case PI_EVENT:
    fprintf(stream, "<?");
    output_text(&event->name, stream);
    if(event->value.ptr != 0) {
      fprintf(stream, " ");
      output_text(&event->value, stream);
    }
    fprintf(stream, "?>");
    break;
  case ENTITY_REFERENCE_EVENT:
    fprintf(stream, "&");
    output_text(&event->name, stream);
    fprintf(stream, ";");
    break;
  case DEC_CHAR_REFERENCE_EVENT:
    fprintf(stream, "&#");
    output_text(&event->name, stream);
    fprintf(stream, ";");
    break;
  case HEX_CHAR_REFERENCE_EVENT:
    fprintf(stream, "&#x");
    output_text(&event->name, stream);
    fprintf(stream, ";");
    break;
  case NO_EVENT:
    break;
  }
}

int
main(int argc, char **argv)
{
  FAXPP_Error err;
  int i;
  unsigned long startTime;
  FILE *file;

  if(argc < 2) {
    printf("Too few arguments\n");
    exit(-1);
  }

  FAXPP_Parser *parser = FAXPP_create_parser(WELL_FORMED_PARSE_MODE, FAXPP_utf8_encode);
  if(parser == 0) {
    printf("ERROR: out of memory\n");
    exit(1);
  }

  for(i = 1; i < argc; ++i) {

    startTime = getTime();

    file = fopen(argv[i], "r");
    if(file == 0) {
      printf("Open failed: %s\n", strerror(errno));
      exit(1);
    }

    err = FAXPP_init_parse_file(parser, file);
    if(err != NO_ERROR) {
      printf("ERROR: %s\n", FAXPP_err_to_string(err));
      exit(1);
    }

    while((err = FAXPP_next_event(parser)) == 0) {
      output_event(FAXPP_get_current_event(parser), stdout);

      if(FAXPP_get_current_event(parser)->type == END_DOCUMENT_EVENT)
        break;
    }

    if(err != NO_ERROR) {
      printf("%03d:%03d ERROR: %s\n", FAXPP_get_error_line(parser),
             FAXPP_get_error_column(parser), FAXPP_err_to_string(err));
    }

    fclose(file);

    printf("Time taken: %gms\n", ((double)(getTime() - startTime) / MSECS_IN_SECS * 1000));
  }

  FAXPP_free_parser(parser);

  return 0;
}
