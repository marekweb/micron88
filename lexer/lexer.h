#pragma once
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  LEXER_STATE_OK,
  LEXER_STATE_EOF,
  LEXER_STATE_ERR,
} lexer_state_t;

struct lexer_t {
  const uint8_t *buf;
  size_t size;
  size_t current_offset;
  lexer_state_t state;
} ;

typedef struct lexer_t lexer_t;

struct token_t {
  uint8_t type;
  union {
    char *string;
    int64_t integer;
  } value;
} ;

typedef struct token_t token_t;

lexer_t *lexer_new(const uint8_t *buf, size_t size);
void lexer_free(lexer_t *lexer);
token_t lexer_next(lexer_t *lexer);
