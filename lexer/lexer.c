#include <stdint.h>
#include <stdlib.h>
#include "lexer.h"

lexer_t *lexer_new(const uint8_t *buf, size_t size) {
    lexer_t *lexer = calloc(1, sizeof(lexer_t));
    lexer->buf = buf;
    lexer->size = size;
    return lexer;
}

void lexer_free(lexer_t *lexer) {
    free(lexer);
}

token_t lexer_next(lexer_t *lexer) {
    while (lexer->current_offset < lexer->size) {
        char c = lexer->buf[lexer->current_offset++];
        if (c == ' ') {
            lexer->current_offset++;
            continue;
        }

    }
    lexer->state = LEXER_STATE_EOF;
}
