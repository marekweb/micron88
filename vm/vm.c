#include "vm.h"
#include "opcodes.h"
#include <stdio.h>
#include <stdlib.h>

vm *vm_alloc() { return calloc(1, sizeof(vm)); }

void vm_free(vm *vm) { free(vm); }

void vm_err(vm *vm, u8 err) {
  vm->err = err;
  vm->state = VM_STATE_ERR;
  longjmp(vm->jmp, 1);
}

u8 vm_read_mem_8(vm *vm, u32 addr) {
  if (addr > VM_MEMORY_SIZE) {
    vm_err(vm, VM_ERR_OOB);
    return 0;
  }
  return vm->memory[addr];
}

u16 vm_read_mem_16(vm *vm, u32 addr) {
  if (addr + 1 > VM_MEMORY_SIZE) {
    vm_err(vm, VM_ERR_OOB);
    return 0;
  }
  u16 *mem = (u16 *)vm->memory;
  return mem[addr];
}

u32 vm_read_mem_32(vm *vm, u32 addr) {
  if (addr + 3 > VM_MEMORY_SIZE) {
    vm_err(vm, VM_ERR_OOB);
    return 0;
  }
  return vm->memory[addr];
}

u8 vm_read_pc_8(vm *vm) { return vm_read_mem_8(vm, vm->pc++); }
u16 vm_read_pc_16(vm *vm) { return vm_read_mem_16(vm, vm->pc += 2); }
u32 vm_read_pc_32(vm *vm) { return vm_read_mem_32(vm, vm->pc += 4); }

u32 vm_stack_peek(vm *vm) {
  if (vm->sp == 0) {
    vm_err(vm, VM_ERR_STACK_UNDERFLOW);
    return 0;
  }
  return vm->stack[vm->sp - 1];
}

u32 vm_stack_pop(vm *vm) {
  if (vm->sp == 0) {
    vm_err(vm, VM_ERR_STACK_UNDERFLOW);
    return 0;
  }
  return vm->stack[--vm->sp];
}

void vm_stack_push(vm *vm, i32 value) {
  if (vm->sp == VM_STACK_SIZE - 1) {
    vm_err(vm, VM_ERR_STACK_OVERFLOW);
    return;
  }
  vm->stack[vm->sp++] = value;
}

void vm_exec(vm *vm) {
  u8 opcode = vm_read_pc_8(vm);
  u32 arg;

  switch (opcode) {
  case OP_HALT:
    vm->state = VM_STATE_HALT;
    break;

  case OP_CONST_ZERO:
    vm_stack_push(vm, 0);
    break;

  case OP_CONST_8:
    arg = vm_read_pc_8(vm);
    vm_stack_push(vm, arg);
    break;

  case OP_CONST_16:
    arg = vm_read_pc_16(vm);
    vm_stack_push(vm, arg);
    break;

  case OP_CONST_32:
    arg = vm_read_pc_32(vm);
    vm_stack_push(vm, arg);
    break;

  case OP_DROP:
    vm_stack_pop(vm);
    break;

  case OP_DUP:
    vm_stack_push(vm, vm_stack_peek(vm));
    break;

  case OP_ADD: {
    i32 value1 = vm_stack_pop(vm);
    i32 value2 = vm_stack_pop(vm);
    vm_stack_push(vm, value1 + value2);
    break;
  }

  case OP_SUB: {
    i32 value1 = vm_stack_pop(vm);
    i32 value2 = vm_stack_pop(vm);
    vm_stack_push(vm, value1 - value2);
    break;
  }

  case OP_MUL: {
    i32 value1 = vm_stack_pop(vm);
    i32 value2 = vm_stack_pop(vm);
    vm_stack_push(vm, value1 * value2);
    break;
  }

  case OP_DIV: {
    i32 value1 = vm_stack_pop(vm);
    i32 value2 = vm_stack_pop(vm);
    vm_stack_push(vm, value1 / value2);
    break;
  }

  case OP_MOD: {
    i32 value1 = vm_stack_pop(vm);
    i32 value2 = vm_stack_pop(vm);
    vm_stack_push(vm, value1 % value2);
    break;
  }

  case OP_EQ: {
    i32 value1 = vm_stack_pop(vm);
    i32 value2 = vm_stack_pop(vm);
    vm_stack_push(vm, value1 == value2);
    break;
  }

  case OP_NEQ: {
    i32 value1 = vm_stack_pop(vm);
    i32 value2 = vm_stack_pop(vm);
    vm_stack_push(vm, value1 != value2);
    break;
  }
  }
}

void vm_run(vm *vm) {
  vm->state = VM_STATE_RUNNING;
  if (setjmp(vm->jmp) == 1) {
    return;
  }

  while (vm->state == VM_STATE_RUNNING) {
    vm_exec(vm);
  }
}
