CC=clang
OPTIONS="-Wall -Wextra -Werror -Wpedantic -std=c99"
$CC $OPTIONS -o main ./vm/*.c
