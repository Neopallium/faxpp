#include "tokenizer_states.h"

TokenizerError
comment_start_state1(TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '-':
    env->state = comment_start_state2;
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = comment_content_state;
    token_start_position(env);
    next_char(env);
    return INVALID_START_OF_COMMENT;
  }
  return NO_ERROR;
}

TokenizerError
comment_start_state2(TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '-':
    env->state = comment_content_state;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    env->state = comment_content_state;
    token_start_position(env);
    next_char(env);
    return INVALID_START_OF_COMMENT;
  }
  return NO_ERROR;
}

TokenizerError
comment_content_state(TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case '-':
      env->state = comment_content_seen_dash_state;
      env->token_position1 = env->token_buffer.cursor ? env->token_buffer.cursor : env->position;
      next_char(env);
      return NO_ERROR;
    LINE_ENDINGS
      break;
    }

    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

TokenizerError
comment_content_seen_dash_state(TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '-':
    env->state = comment_content_seen_dash_twice_state;
    env->token_position2 = env->token_position1;
    env->token_position1 = env->token_buffer.cursor ? env->token_buffer.cursor : env->position;
    break;
  LINE_ENDINGS
  default:
    env->state = comment_content_state;
    break;
  }

  next_char(env);
  return NO_ERROR;
}

TokenizerError
comment_content_seen_dash_twice_state(TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '>':
    base_state(env);
    env->token_buffer.cursor = 0;
    env->token.value.len = env->token_position2 - env->token.value.ptr;
    report_token(COMMENT_TOKEN, env);
    next_char(env);
    token_start_position(env);
    break;
  case '-':
    env->token_position2 = env->token_position1;
    env->token_position1 = env->token_buffer.cursor ? env->token_buffer.cursor : env->position;
    next_char(env);
    return DOUBLE_DASH_IN_COMMENT;
  LINE_ENDINGS
  default:
    env->state = comment_content_state;
    next_char(env);
    return DOUBLE_DASH_IN_COMMENT;
  }
  return NO_ERROR;
}

