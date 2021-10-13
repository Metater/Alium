#include <stdio.h>
#include <stdio.h>
#include <time.h>

#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"
#include "testing.h"

int main(int argc, const char* argv[]) {
	clock_t start, end;
	double cpu_time_used;

	start = clock();

	VM vm;
	initVM(&vm);

	Chunk chunk;
	initChunk(&chunk);

	int constant = addConstant(&chunk, NUMBER_VAL(1.2));
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant, 123);

	constant = addConstant(&chunk, NUMBER_VAL(3.4));
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant, 123);

	writeChunk(&chunk, OP_ADD, 123);

	constant = addConstant(&chunk, NUMBER_VAL(5.6));
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant, 123);

	writeChunk(&chunk, OP_DIVIDE, 123);
	writeChunk(&chunk, OP_NEGATE, 123);

	writeChunk(&chunk, OP_RETURN, 123);

	run(&vm, &chunk);
	freeVM(&vm);
	freeChunk(&chunk);

	// tests
	//testAddLine();

	end = clock();

	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Total execution time: %f\n", cpu_time_used);
	return 0;
}
