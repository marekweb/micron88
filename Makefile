CC = clang
CFLAGS = -Wall -Wextra -Werror

LEXER_TARGET = test-lexer
LEXER_SOURCES = lexer/lexer.c lexer/lexer.test.c
LEXER_HEADERS = lexer/lexer.h

$(LEXER_TARGET): $(LEXER_SOURCES) $(LEXER_HEADERS)
	$(CC) $(CFLAGS) $(LEXER_SOURCES) -o $(LEXER_TARGET)

VM_TARGET = test-vm
VM_SOURCES = vm/vm.c tests/vm.test.c
VM_HEADERS = vm/vm.h

$(VM_TARGET): $(VM_SOURCES) $(VM_HEADERS)
	$(CC) $(CFLAGS) $(VM_SOURCES) -o $(VM_TARGET)

.PHONY: clean
clean:
	rm -f $(LEXER_TARGET) $(VM_TARGET)

.PHONY: run-test-lexer
run-test-lexer: $(LEXER_TARGET)
	./$(LEXER_TARGET)

.PHONY: run-test-vm
run-test-vm: $(VM_TARGET)
	./$(VM_TARGET)


.PHONY: test
test: run-test-lexer run-test-vm
