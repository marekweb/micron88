#include "lexer.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Convert a string of digits to an integer. Used for parsing integers. Takes a
 * buffer length.
 */
uint64_t atoin(const uint8_t *buf, uint8_t length) {
  uint64_t result = 0;
  uint8_t i;

  if (length == 0) {
    return 0;
  }

  for (i = 0; i < length; i++) {
    result = result * 10 + (buf[i] - '0');
  }

  return result;
}

lexer_t *lexer_new(const uint8_t *buf, size_t size) {
  lexer_t *lexer = calloc(1, sizeof(lexer_t));
  lexer->buf = buf;
  lexer->size = size;
  return lexer;
}

void lexer_free(lexer_t *lexer) { free(lexer); }

/*
 * Get next character without advancing current offset.
 */
uint8_t peek(lexer_t *lexer) {
  if (lexer->current_offset >= lexer->size) {
    lexer->state = LEXER_STATE_EOF;
    return 0;
  }
  return lexer->buf[lexer->current_offset];
}

/*
 * Get next character and advance current offset.
 */
uint8_t next(lexer_t *lexer) {
  uint8_t c = peek(lexer);
  lexer->current_offset++;
  if (c == '\n') {
    lexer->line++;
    lexer->column = 0;
  } else {
    lexer->column++;
  }
  return c;
}

uint8_t back(lexer_t *lexer) {
  lexer->current_offset--;
  return lexer->buf[lexer->current_offset];
}

token_t lexer_next(lexer_t *lexer) {
  uint8_t c = next(lexer);
  if (c == 0 && lexer->state == LEXER_STATE_EOF) {
    token_t token = {
        .type = TOKEN_TYPE_EOF,
        .line = lexer->line,
        .column = lexer->column,
    };
    return token;
  }
  if (isalpha(c)) {
    int token_start = lexer->current_offset - 1;
    while (1) {
      c = next(lexer);
      if (!isalnum(c)) {
        back(lexer);
        break;
      }
    }
    token_t token = {
        .type = TOKEN_TYPE_IDENTIFIER,
        .line = lexer->line,
        .column = lexer->column,
        .value.string = lexer_extract_string(
            lexer, token_start, lexer->current_offset - token_start)};
    return token;
  }
  if (isdigit(c)) {
    int token_start = lexer->current_offset - 1;
    while (1) {
      c = next(lexer);
      if (!isdigit(c)) {
        back(lexer);
        break;
      }
    }
    token_t token = {.type = TOKEN_TYPE_INTEGER_LITERAL,
                     .line = lexer->line,
                     .column = lexer->column,
                     .value.integer =
                         atoin(lexer->buf + token_start,
                               lexer->current_offset - token_start)};
    return token;
  }
  if (isspace(c)) {
    int token_start = lexer->current_offset - 1;
    while (1) {
      c = next(lexer);
      if (!isspace(c)) {
        back(lexer);
        break;
      }
    }

    if (lexer->skip_whitespace) {
      return lexer_next(lexer);
    }

    token_t token = {
        .type = TOKEN_TYPE_WHITESPACE,
        .line = lexer->line,
        .column = lexer->column,
        .value.string = lexer_extract_string(
            lexer, token_start, lexer->current_offset - token_start)};
    return token;
  }
  if (c == '"') {
    // TODO: handle escape sequences in strings
    int token_start = lexer->current_offset - 1;
    while (1) {
      c = next(lexer);
      if (c == '"') {
        break;
      }
    }
    token_t token = {
        .type = TOKEN_TYPE_STRING_LITERAL,
        .line = lexer->line,
        .column = lexer->column,
        .value.string = lexer_extract_string(
            lexer, token_start, lexer->current_offset - token_start)};
    return token;
  }

  switch (c) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '.':
  case '=':
  case ';':
  case '(':
  case ')':
    return (token_t){
        .type = TOKEN_TYPE_SPECIAL,
        .line = lexer->line,
        .column = lexer->column,
        .value.integer = c,
    };
  }

  // TODO: handle other tokens
  // For now, just return EOF for unknown characters, but we print the unknown
  // character to signal that we still need to implement handling for it. c == 0
  // is the null terminator, so it's EOF.
  if (c != 0) {
    printf("Unknown character: [%c] (%d)\n", c, c);
  }
  return (token_t){
      .type = TOKEN_TYPE_EOF,
      .line = lexer->line,
      .column = lexer->column,
  };
}

/* Allocate a new string from the source code buffer */
char *lexer_extract_string(lexer_t *lexer, int start, int length) {
  char *token_string = malloc(length + 1);
  memcpy(token_string, lexer->buf + start, length);
  token_string[length] = '\0';
  return token_string;
}

struct token_list_item_t {
  token_t *token;
  struct token_list_item_t *next, *prev;
};

struct token_list_t {
  struct token_list_item_t *head, *tail;
};
