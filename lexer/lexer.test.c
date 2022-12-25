#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

char input[] = "hello world 1234 alpha 9125 59681902";

#define assert_string(a, b) assert(strcmp(a, b) == 0)
#define print_token(token)                                                     \
  if (token.type == TOKEN_TYPE_STRING) {                                       \
    printf("token: type=%d, value=%s", token.type, token.value.string);        \
  } else if (token.type == TOKEN_TYPE_INTEGER) {                               \
    printf("token: type=%d, value=%d", token.type, token.value.integer);       \
  }

int main() {
  token_t tokens[100];
  int token_index = 0;
  size_t len = sizeof(input);
  lexer_t *lexer = lexer_new(input, len);
  while (1) {
    token_t token = lexer_next(lexer);
    printf("token: type=%d\n", token.type);

    tokens[token_index++] = token;
  }

  assert(tokens[0].type == TOKEN_TYPE_STRING);
  assert_string(tokens[0].value.string, "hello");
  assert(tokens[1].type == TOKEN_TYPE_STRING);
  assert_string(tokens[1].value.string, "world");
  assert(tokens[2].type == TOKEN_TYPE_INTEGER);
  assert(tokens[2].value.integer == 1234);
  assert(tokens[3].type == TOKEN_TYPE_STRING);
  assert_string(tokens[3].value.string, "alpha");
  assert(tokens[4].type == TOKEN_TYPE_INTEGER);
  assert(tokens[4].value.integer == 9125);
  assert(tokens[5].type == TOKEN_TYPE_INTEGER);
  assert(tokens[5].value.integer == 59681902);
  assert(tokens[6].type == TOKEN_TYPE_EOF);

  lexer_free(lexer);
}
