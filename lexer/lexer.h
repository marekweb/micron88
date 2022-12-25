#pragma once
#include <stdint.h>
#include <stdlib.h>

#define TOKEN_TYPE_EOF 0
#define TOKEN_TYPE_WHITESPACE 1
#define TOKEN_TYPE_STRING 2
#define TOKEN_TYPE_INTEGER 3

typedef enum {
  LEXER_STATE_NONE,
  LEXER_STATE_OK,
  LEXER_STATE_EOF,
  LEXER_STATE_ERR,
} lexer_state_t;

struct lexer_t {
  uint8_t line; // TODO: assess limits of line and column
  uint8_t column;
  const uint8_t *buf;
  size_t size;
  size_t current_offset;
  lexer_state_t state;
};

typedef struct lexer_t lexer_t;

struct short_pascal_string_t {
  uint8_t length;
  uint8_t *buffer;
};

struct token_t {
  uint8_t type;
  uint8_t line; // TODO: assess limits of line and column, and keep it sync with
                // lexer.
  uint8_t column;
  union {
    struct {
      uint8_t length;
      uint8_t *buffer;
    } string;
    int32_t integer;
  } value;
};

typedef struct token_t token_t;

lexer_t *lexer_new(const uint8_t *buf, size_t size);
void lexer_free(lexer_t *lexer);
token_t lexer_next(lexer_t *lexer);
