#ifndef aliumi_vm_h
#define aliumi_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
	Chunk* chunk;
	uint8_t* ip;
	Value stack[STACK_MAX];
	Value* stackTop;
} VM;

typedef enum {
	RUN_OK,
	RUN_RUNTIME_ERROR
} RunResult;

void initVM();
void freeVM();
RunResult run(Chunk* chunk);
void push(Value value);
Value pop();

#endif