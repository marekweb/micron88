#pragma once
#include <setjmp.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t i32;
typedef uint32_t u32;

#define VM_STACK_SIZE 64
#define VM_MEMORY_SIZE 8 * 1024

#define VM_ERR_NONE 0
#define VM_ERR_OOB 1
#define VM_ERR_DIV_ZERO 2
#define VM_ERR_INVALID_OPCODE 3
#define VM_ERR_STACK_UNDERFLOW 4
#define VM_ERR_STACK_OVERFLOW 5

#define VM_STATE_RUNNING 0
#define VM_STATE_HALT 1
#define VM_STATE_ERR 2

struct vm {
  u32 pc;
  u8 sp;
  u8 csp;
  u8 err;   // see VM_ERR_* constants
  u8 state; // see VM_STATE_* constants
  jmp_buf jmp;
  i32 stack[VM_STACK_SIZE];
  i32 call_stack[VM_STACK_SIZE];
  u8 memory[VM_MEMORY_SIZE];
};

typedef struct vm vm;

vm *vm_alloc();
void vm_free(vm *vm);
void vm_err(vm *vm, u8 err);
void vm_stack_push(vm *vm, i32 value);
i32 vm_stack_pop(vm *vm);
i32 vm_stack_peek(vm *vm);
void vm_exec(vm *vm);
void vm_run(vm *vm);
void vm_debug(vm *vm, char *message);
void vm_debug_stack(vm *vm);
void vm_dump(vm *vm);
void vm_dump_json(vm *vm);
