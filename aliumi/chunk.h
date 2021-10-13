#ifndef aliumi_chunk_h
#define aliumi_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
	OP_CONSTANT,
	OP_NIL,
	OP_TRUE,
	OP_FALSE,
	OP_EQUAL,
	OP_NOT_EQUAL,
	OP_GREATER,
	OP_GREATER_EQUAL,
	OP_LESS,
	OP_LESS_EQUAL,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_NOT,
	OP_NEGATE,
	OP_RETURN,
} OpCode;

typedef struct {
	int capacity;
	int index;
	uint8_t* runLength;
	int* lineInfo;
} LineInfo;


typedef struct {
	int capacity;
	int count;
	uint8_t* code;
	LineInfo lineInfo;
	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

void addLine(Chunk* chunk, int line);
int getLine(Chunk* chunk, int offset);

#endif
