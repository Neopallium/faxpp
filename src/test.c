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
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <faxpp/xml_tokenizer.h>

#define MSECS_IN_SECS 1000000

unsigned long getTime()
{
  struct timeval timev;
  gettimeofday(&timev, 0);

  return (timev.tv_sec * MSECS_IN_SECS) + timev.tv_usec;
}

#define BUF_SIZE 50

int
main(int argc, char **argv)
{
  int fd;
  long length;
  void *xml;
  int i;
  char buf[BUF_SIZE + 1];
  unsigned long startTime;

  if(argc < 2) {
    printf("Too few arguments\n");
    exit(-1);
  }

  TokenizerEnv env;
  Token token;

  TokenizerError err = init_tokenizer(&env);
  if(err != NO_ERROR) {
    printf("ERROR: %s\n", err_to_string(err));
    exit(1);
  }

  for(i = 1; i < argc; ++i) {

    startTime = getTime();

    fd = open(argv[i], O_RDONLY);
    if(fd == -1) {
      printf("Open failed: %d\n", errno);
      exit(1);
    }

    length = lseek(fd, 0, SEEK_END);
/*     printf("Arg: %s\nLength: %d\n", argv[1], length); */

    xml = mmap(0, length, PROT_READ, MAP_SHARED, fd, 0);
    if(xml == MAP_FAILED) {
      printf("Mmap failed: %d\n", errno);
      exit(1);
    }

    err = init_tokenize(&env, xml, length, utf8_encode);
    if(err != NO_ERROR) {
      printf("ERROR: %s\n", err_to_string(err));
      exit(1);
    }

/*     printf("Char set: %s\n", transcode_to_string(env.transcode)); */

    err = next_token(&env, &token);
    while(token.token != END_OF_BUFFER_TOKEN) {
      if(err != NO_ERROR) {
        printf("%03d:%03d ERROR: %s\n", env.line, env.column, err_to_string(err));
        if(err == PREMATURE_END_OF_BUFFER ||
           err == BAD_ENCODING) break;
      }
/*       else if(token.value != 0) { */
/*         if(token.length > BUF_SIZE) { */
/*           strncpy(buf, token.value, BUF_SIZE - 3); */
/*           buf[BUF_SIZE - 3] = '.'; */
/*           buf[BUF_SIZE - 2] = '.'; */
/*           buf[BUF_SIZE - 1] = '.'; */
/*           buf[BUF_SIZE] = 0; */
/*         } */
/*         else { */
/*           strncpy(buf, token.value, token.length); */
/*           buf[token.length] = 0; */
/*         } */
/*         printf("%03d:%03d Token ID: %s, Token: \"%s\"\n", token.line, token.column, token_to_string(&token), buf); */
/*       } */
/*       else */
/*         printf("%03d:%03d Token ID: %s\n", token.line, token.column, token_to_string(&token)); */
      err = next_token(&env, &token);
    }

    printf("Time taken: %gms\n", ((double)(getTime() - startTime) / MSECS_IN_SECS * 1000));
  }

  free_tokenizer(&env);

  return 0;
}
