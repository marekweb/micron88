CC=clang
OPTIONS="-Wall -Wextra -Werror -Wpedantic -std=c99"
$CC $OPTIONS -o test ./vm/*.c ./tests/test.c
