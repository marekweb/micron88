#pragma once
#include <stdint.h>

#define TOKEN_TYPE_EOF 0
#define TOKEN_TYPE_WHITESPACE 1
#define TOKEN_TYPE_IDENTIFIER 2
#define TOKEN_TYPE_STRING_LITERAL 2
#define TOKEN_TYPE_INTEGER_LITERAL 3
#define TOKEN_TYPE_SPECIAL 4

struct short_pascal_string_t {
  uint8_t length;
  uint8_t *buffer;
};

typedef struct short_pascal_string_t short_pascal_string_t;

struct token_t {
  uint8_t type;
  uint8_t line; // TODO: assess limits of line and column, and keep it sync with
                // lexer.
  uint8_t column;
  union {
    short_pascal_string_t string;
    int32_t integer;
  } value;
};
typedef struct token_t token_t;
