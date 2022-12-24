
#include "../vm/opcodes.h"
#include "../vm/vm.h"
#include <stdio.h>
#include <string.h>

#define assert_eq(a, b)                                                        \
  {                                                                            \
    if (a != b) {                                                              \
      printf("not ok %d - %s:%d: %s is %d (expected %d)\n", test_id, __FILE__, \
             __LINE__, #a, a, b);                                              \
      goto test_end;                                                           \
    }                                                                          \
  }

#define assert_vm(vm, _pc, _sp, _csp, _state, _err)                            \
  {                                                                            \
    assert_eq(vm->pc, _pc);                                                    \
    assert_eq(vm->sp, _sp);                                                    \
    assert_eq(vm->csp, _csp);                                                  \
    assert_eq(vm->state, _state);                                              \
    assert_eq(vm->err, _err);                                                  \
  }

#define assert_stack(vm, i, value) assert_eq(vm->stack[i], value)
#define assert_mem_8(vm, address, value)                                       \
  assert_eq(vm_read_mem_8(vm, address), value)
#define assert_mem_16(vm, address, value)                                      \
  assert_eq(vm_read_mem_16(vm, address), value)
#define assert_mem_32(vm, address, value)                                      \
  assert_eq(vm_read_mem_32(vm, address), value)

#define assert_vm_done(vm, pc, sp, csp)                                        \
  assert_vm(vm, pc, sp, csp, VM_STATE_HALT, VM_ERR_NONE)

#define assert_vm_err(vm, _pc, _sp, _csp, _err)                                \
  assert_vm(vm, _pc, _sp, _csp, VM_STATE_ERR, _err)

#define test_plan(n)                                                           \
  int test_id = 0;                                                             \
  printf("1..%d\n", n);

#define test_start()                                                           \
  __label__ test_end;                                                          \
  test_id++;

#define test_end()                                                             \
  printf("ok %d\n", test_id);                                                  \
  test_end:;

int main() {
  test_plan(6);
  {
    test_start();
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_8, 6, OP_CONST_8, 7, OP_ADD, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 6, 1, 0, VM_STATE_HALT, VM_ERR_NONE);
    vm_free(vm);
    test_end();
  }

  {
    test_start();
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_8, 6, OP_CONST_8, 7, OP_SUB, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 6, 1, 0, VM_STATE_HALT, VM_ERR_NONE);
    assert_stack(vm, 0, 1);
    vm_free(vm);
    test_end();
  }

  {
    test_start();
    vm *vm = vm_alloc();
    u8 program[] = {OP_ADD, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm_err(vm, 1, 0, 0, VM_ERR_STACK_UNDERFLOW);
    vm_free(vm);
    test_end();
  }

  {
    test_start();
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_ZERO, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 2, 1, 0, VM_STATE_HALT, VM_ERR_NONE);
    vm_free(vm);
    test_end();
  }

  {
    test_start();
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_8, 147, OP_CONST_8, 247, OP_STORE_8, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 6, 0, 0, VM_STATE_HALT, VM_ERR_NONE);
    assert_mem_8(vm, 247, 147);
    vm_free(vm);
    test_end();
  }

  {
    test_start();
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_16, 147,         100,    OP_CONST_8,
                    247,         OP_STORE_16, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 7, 0, 0, VM_STATE_HALT, VM_ERR_NONE);
    assert_mem_16(vm, 247, 147 + 256 * 100);
    vm_free(vm);
    test_end();
  }

  {
    test_start();
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_32, 147, 100,         44,     98,
                    OP_CONST_8,  247, OP_STORE_32, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 9, 0, 0, VM_STATE_HALT, VM_ERR_NONE);
    assert_mem_32(vm, 247,
                  147 + 256 * 100 + 256 * 256 * 44 + 256 * 256 * 256 * 98);
    vm_free(vm);
    test_end();
  }
    vm_run(vm);
}
