
#include "../vm/opcodes.h"
#include "../vm/vm.h"
#include <stdio.h>
#include <string.h>

#define assert_eq(a, b)                                                        \
  {                                                                            \
    if (a != b) {                                                              \
      ("%s:%d: %s is %d (expected %d)\n", __FILE__, __LINE__, #a, a, b);       \
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

#define assert_vm_done(vm, pc, sp, csp)                                        \
  assert_vm(vm, pc, sp, csp, VM_STATE_HALT, VM_ERR_NONE)

#define assert_vm_err(vm, _pc, _sp, _csp, _err)                                \
  assert_vm(vm, _pc, _sp, _csp, VM_STATE_ERR, _err)

int main() {
  {
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_8, 6, OP_CONST_8, 7, OP_ADD, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 6, 1, 0, VM_STATE_HALT, VM_ERR_NONE);
    vm_free(vm);
  }

  {
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_8, 6, OP_CONST_8, 7, OP_SUB, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 6, 1, 0, VM_STATE_HALT, VM_ERR_NONE);
    assert_stack(vm, 0, 1);
    vm_free(vm);
  }

  {
    vm *vm = vm_alloc();
    u8 program[] = {OP_ADD, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm_err(vm, 1, 0, 0, VM_ERR_STACK_UNDERFLOW);
    vm_free(vm);
  }

  {
    vm *vm = vm_alloc();
    u8 program[] = {OP_CONST_ZERO, OP_HALT};
    memcpy(vm->memory, program, sizeof(program));
    vm_run(vm);
    assert_vm(vm, 2, 1, 0, VM_STATE_HALT, VM_ERR_NONE);
    vm_free(vm);
  }
}
