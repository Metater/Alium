#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "vm.h"
#include "debug.h"

static void resetStack(VM* vm) {
    vm->stackTop = vm->stack;
}

static void runtimeError(VM* vm, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm->ip - vm->chunk->code - 1;
    int line = getLine(vm->chunk, instruction);
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack(vm);
}

void initVM(VM* vm) {
    resetStack(vm);
}

void freeVM(VM* vm) {
}

void push(VM* vm, Value value) {
    *(vm->stackTop) = value;
    vm->stackTop++;
}

Value pop(VM* vm) {
    vm->stackTop--;
    return *(vm->stackTop);
}

static Value peek(VM* vm, int distance) {
    return vm->stackTop[-1 - distance];
}

static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

RunResult run(VM* vm, Chunk* chunk) {
	vm->chunk = chunk;
	vm->ip = vm->chunk->code;
    #define READ_BYTE() (*(vm->ip)++)
    #define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
    #define BINARY_OP(valueType, op) \
		do { \
		if (!IS_NUMBER(peek(vm, 0)) || !IS_NUMBER(peek(vm, 1))) { \
		    runtimeError(vm, "Operands must be numbers."); \
			return RUN_RUNTIME_ERROR; \
		} \
		double b = AS_NUMBER(pop(vm)); \
		double a = AS_NUMBER(pop(vm)); \
		push(vm, valueType(a op b)); \
		} while (false)

    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (Value* slot = vm->stack; slot < vm->stackTop; slot++) {
                printf("[ ");
                printValue(*slot);
                printf(" ]");
            }
            printf("\n");
            disassembleInstruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
        #endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(vm, constant);
                break;
            }
            case OP_NIL:   push(vm, NIL_VAL); break;
            case OP_TRUE:  push(vm, BOOL_VAL(true)); break;
            case OP_FALSE: push(vm, BOOL_VAL(false)); break;
            // Fix later NaN is always false
            case OP_EQUAL: {
                Value b = pop(vm);
                Value a = pop(vm);
                push(vm, BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_NOT_EQUAL: {
                Value b = pop(vm);
                Value a = pop(vm);
                push(vm, BOOL_VAL(!valuesEqual(a, b)));
                break;
            }
            case OP_GREATER:       BINARY_OP(BOOL_VAL, > ); break;
            case OP_GREATER_EQUAL: BINARY_OP(BOOL_VAL, >= ); break;
            case OP_LESS:          BINARY_OP(BOOL_VAL, < ); break;
            case OP_LESS_EQUAL:    BINARY_OP(BOOL_VAL, <= ); break;
            case OP_ADD:      BINARY_OP(NUMBER_VAL, +); break;
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, / ); break;
            case OP_NOT:
                push(vm, BOOL_VAL(isFalsey(pop(vm))));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(vm, 0))) {
                    runtimeError(vm, "Operand must be a number.");
                    return RUN_RUNTIME_ERROR;
                }
                push(vm, NUMBER_VAL(-AS_NUMBER(pop(vm))));
                break;
            case OP_RETURN: {
                printValue(pop(vm));
                printf("\n");
                return RUN_OK;
            }
        }
    }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}