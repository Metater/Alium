#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->encodedLineInfoCount = 0;
    chunk->encodedLineInfoCapacity = 0;
    chunk->encodedLineInfo = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(uint8_t, chunk->encodedLineInfo, chunk->encodedLineInfoCapacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }
    // Adjust the +1
    if (chunk->encodedLineInfoCapacity < chunk->encodedLineInfoCount + 1) {
        int oldCapacity = chunk->encodedLineInfoCapacity;
        chunk->encodedLineInfoCapacity = GROW_CAPACITY(oldCapacity);
        chunk->encodedLineInfo = GROW_ARRAY(uint8_t, chunk->encodedLineInfo, oldCapacity, chunk->encodedLineInfoCapacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->encodedLineInfo[chunk->count] = line;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int offset) {
    int remaining = 0;
    LineInfo* lineInfo = &chunk->lineInfo;
    for (size_t i = 0; i < lineInfo->count; i++) {
        remaining += lineInfo->runLength[i];
        if (remaining >= offset) return lineInfo->lineInfo[i];
    }
}
