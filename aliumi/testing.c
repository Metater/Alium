#include <stdio.h>

#include "testing.h"
#include "chunk.h"

void testAddLine() {
    int verify[1024];

    Chunk chunk;
    initChunk(&chunk);

    for (size_t i = 0; i < 24; i++)
    {
        int line = 420 + i;
        verify[i] = line;
        writeChunk(&chunk, OP_RETURN, line);
    }
    for (size_t i = 0; i < 1000; i++)
    {
        int line = 420;
        verify[i + 24] = line;
        writeChunk(&chunk, OP_RETURN, line);
    }

    for (size_t i = 0; i < 1024; i++)
    {
        if (getLine(&chunk, i) != verify[i])
        {
            printf("Test Failed: Chunk add line, on index %lli\n", i);
        }
    }
    printf("Test Success: Chunk add line\n");

    freeChunk(&chunk);
}