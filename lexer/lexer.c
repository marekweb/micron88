#include "lexer.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#define transition(old_state, condition, new_state)                            \
  if (lexer->state == old_state && condition) {                                \
    lexer->state = new_state;                                                  \
    continue;                                                                  \
  }

lexer_t *lexer_new(const uint8_t *buf, size_t size) {
  lexer_t *lexer = calloc(1, sizeof(lexer_t));
  lexer->buf = buf;
  lexer->size = size;
  return lexer;
}

void lexer_free(lexer_t *lexer) { free(lexer); }

uint8_t peek(lexer_t *lexer) {
  if (lexer->current_offset >= lexer->size) {
    lexer->state = LEXER_STATE_EOF;
    return 0;
  }
  return lexer->buf[lexer->current_offset];
}
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
        .type = TOKEN_TYPE_STRING,
        .line = lexer->line,
        .column = lexer->column,
        .value.string.length = lexer->current_offset - token_start,
        .value.string.buffer = (uint8_t *)lexer->buf + token_start,
    };
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
    token_t token = {
        .type = TOKEN_TYPE_INTEGER,
        .line = lexer->line,
        .column = lexer->column,
        .value.integer = atoi((char *)lexer->buf + token_start),
    };
    return token;
  }
  if (isblank(c)) {
    int token_start = lexer->current_offset - 1;
    while (1) {
      c = next(lexer);
      if (!isblank(c)) {
        back(lexer);
        break;
      }
    }
    token_t token = {
        .type = TOKEN_TYPE_WHITESPACE,
        .line = lexer->line,
        .column = lexer->column,
        .value.string.length = lexer->current_offset - token_start,
        .value.string.buffer = (uint8_t *)lexer->buf + token_start,
    };
    return token;
  }
}
