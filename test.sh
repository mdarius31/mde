cc --std=c99 -Wextra -Wall -Werror -g test.c -o test.x86_64
cc -P -E --std=c99 -Wextra -Wall -Werror test.c > temp.c.tmp

clang-format -i temp.c.tmp

valgrind -s --leak-check=full ./test.x86_64
