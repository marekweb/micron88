#include "lexer.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int spseq(const short_pascal_string_t a, char *b) {
  for (int i = 0; i < a.length; i++) {
    if (a.buffer[i] != b[i] || b[i] == 0) {
      return 0;
    }
  }
  return 1;
}

#define assert_token_identifier(token, s)                                      \
  assert(token.type == TOKEN_TYPE_IDENTIFIER);                                 \
  assert(spseq(token.value.string, s));
#define assert_token_string_literal(token, s)                                  \
  assert(token.type == TOKEN_TYPE_STRING_LITERAL);                             \
  assert(spseq(token.value.string, s));
#define assert_token_integer(token, i)                                         \
  assert(token.type == TOKEN_TYPE_INTEGER_LITERAL);                            \
  assert(token.value.integer == i);
#define assert_token_whitespace(token)                                         \
  assert(token.type == TOKEN_TYPE_WHITESPACE);
#define assert_token_special(token, c)                                         \
  assert(token.type == TOKEN_TYPE_SPECIAL);                                    \
  assert(token.value.integer == c);

#define print_token(token)                                                     \
  if (token.type == TOKEN_TYPE_IDENTIFIER) {                                   \
    printf("token: type=%d, value=%.*s at %d:%d\n", token.type,                \
           token.value.string.length, token.value.string.buffer, token.line,   \
           token.column);                                                      \
  } else if (token.type == TOKEN_TYPE_STRING_LITERAL) {                        \
    printf("token: type=%d, value=%.*s at %d:%d\n", token.type,                \
           token.value.string.length, token.value.string.buffer, token.line,   \
           token.column);                                                      \
  } else if (token.type == TOKEN_TYPE_INTEGER_LITERAL) {                       \
    printf("token: type=%d, value=%d\n", token.type, token.value.integer);     \
  } else if (token.type == TOKEN_TYPE_WHITESPACE) {                            \
    printf("token: type=%d (whitespace)\n", token.type);                       \
  }

void tokenize(const uint8_t *buf, size_t size, token_t *tokens) {
  lexer_t *lexer = lexer_new(buf, size);
  token_t token;
  int token_index = 0;
  do {
    token = lexer_next(lexer);
    tokens[token_index++] = token;
    print_token(token);
  } while (token.type != TOKEN_TYPE_EOF);
  lexer_free(lexer);
}

int main() {
  {
    uint8_t input[] = "hello world\n\n1234 alpha \n 9125\t\t59681902\n\n";

    token_t tokens[100];
    size_t len = sizeof(input);
    lexer_t *lexer = lexer_new(input, len);
    tokenize(input, len, tokens);

    assert_token_identifier(tokens[0], "hello");
    assert_token_whitespace(tokens[1]);
    assert_token_identifier(tokens[2], "world");
    assert_token_whitespace(tokens[3]);
    assert_token_integer(tokens[4], 1234);
    assert_token_whitespace(tokens[5]);
    assert_token_identifier(tokens[6], "alpha");
    assert_token_whitespace(tokens[7]);
    assert_token_integer(tokens[8], 9125);
    assert_token_whitespace(tokens[9]);
    assert_token_integer(tokens[10], 59681902);
    assert_token_whitespace(tokens[11]);
    assert(tokens[12].type == TOKEN_TYPE_EOF);

    lexer_free(lexer);
  }

  {
    uint8_t input[] = "value = 500 + 100;\nresult = value * 2;\n.value;\n";
    token_t tokens[100];
    size_t len = sizeof(input);
    lexer_t *lexer = lexer_new(input, len);
    tokenize(input, len, tokens);

    assert_token_identifier(tokens[0], "value");
    assert_token_whitespace(tokens[1]);
    assert_token_special(tokens[2], '=');
    assert_token_whitespace(tokens[3]);
    assert_token_integer(tokens[4], 500);
    assert_token_whitespace(tokens[5]);
    assert_token_special(tokens[6], '+');
    assert_token_whitespace(tokens[7]);
    assert_token_integer(tokens[8], 100);
    assert_token_special(tokens[9], ';');
    assert_token_whitespace(tokens[10]);
    assert_token_identifier(tokens[11], "result");
    assert_token_whitespace(tokens[12]);
    assert_token_special(tokens[13], '=');
    assert_token_whitespace(tokens[14]);
    assert_token_identifier(tokens[15], "value");
    assert_token_whitespace(tokens[16]);
    assert_token_special(tokens[17], '*');
    assert_token_whitespace(tokens[18]);
    assert_token_integer(tokens[19], 2);
    assert_token_special(tokens[20], ';');
    assert_token_whitespace(tokens[21]);
    assert_token_special(tokens[22], '.');
    assert_token_identifier(tokens[23], "value");
    assert_token_special(tokens[24], ';');
    assert_token_whitespace(tokens[25]);
    assert(tokens[26].type == TOKEN_TYPE_EOF);

    lexer_free(lexer);
  }

  {
    uint8_t input[] = "message = \"hello world\";\n";
    token_t tokens[100];
    size_t len = sizeof(input);
    lexer_t *lexer = lexer_new(input, len);
    tokenize(input, len, tokens);

    assert_token_identifier(tokens[0], "message");
    assert_token_whitespace(tokens[1]);
    assert_token_special(tokens[2], '=');
    assert_token_whitespace(tokens[3]);
    assert_token_string_literal(tokens[4], "\"hello world\"");
    assert_token_special(tokens[5], ';');
    assert_token_whitespace(tokens[6]);
    assert(tokens[7].type == TOKEN_TYPE_EOF);

    lexer_free(lexer);
  }
}
