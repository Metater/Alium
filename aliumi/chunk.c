#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk* chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;

    (&chunk->lineInfo)->capacity = 0;
    (&chunk->lineInfo)->count = 0;
    (&chunk->lineInfo)->runLength = NULL;
    (&chunk->lineInfo)->lineInfo = NULL;

    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    LineInfo* lineInfo = &chunk->lineInfo;
    FREE_ARRAY(uint8_t, lineInfo->runLength, lineInfo->capacity);
    FREE_ARRAY(int, lineInfo->lineInfo, lineInfo->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }
    LineInfo* lineInfo = &chunk->lineInfo;
    if (lineInfo->capacity < lineInfo->count + 1) {
        int oldCapacity = lineInfo->capacity;
        lineInfo->capacity = GROW_CAPACITY(oldCapacity);
        lineInfo->runLength = GROW_ARRAY(uint8_t, lineInfo->runLength, oldCapacity, lineInfo->capacity);
        lineInfo->lineInfo = GROW_ARRAY(int, lineInfo->lineInfo, oldCapacity, lineInfo->capacity);
    }

    chunk->code[chunk->count] = byte;
    addLine(chunk, line);
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}



void addLine(Chunk* chunk, int line) {
    LineInfo* lineInfo = &chunk->lineInfo;
    if (lineInfo->count > 0) {
        if (lineInfo->lineInfo[lineInfo->count] == line) {
            if (lineInfo->runLength[lineInfo->count] < 255) {
                lineInfo->runLength[lineInfo->count]++;
                printf("EEE%d\n", lineInfo->runLength[lineInfo->count]);
            }
            else {
                lineInfo->count++;
                lineInfo->runLength[lineInfo->count]++;
                lineInfo->lineInfo[lineInfo->count] = line;
            }
        }
        else {
            lineInfo->count++;
            lineInfo->runLength[lineInfo->count] = 0;
            lineInfo->lineInfo[lineInfo->count] = line;
        }
    }
    else {
        lineInfo->runLength[lineInfo->count] = 0;
        lineInfo->lineInfo[lineInfo->count] = line;
        lineInfo->count++;
    }
}

int getLine(Chunk* chunk, int offset) {
    int remaining = 0;
    LineInfo* lineInfo = &chunk->lineInfo;
    for (size_t i = 0; i < lineInfo->count; i++) {
        remaining += lineInfo->runLength[i] + 1;
        if (remaining >= offset) return lineInfo->lineInfo[i];
    }
    printf("error: line not found in chunk");
    exit(0);
}

