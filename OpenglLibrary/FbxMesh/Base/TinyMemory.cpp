//
//  TinyMemory.cpp
//  IsometricModel
//
//  Created by jimCheng on 14-10-6.
//  Copyright (c) 2014年 JimCheng. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "TinyMemory.h"

using namespace jcgame;

/* PRIVATE STATIC */
const unsigned int TinyMemory::BYTES_LEVELS[] =
{
    2,          4,          8,          16,         32,         64,         128,        256,        512,        1024/*1KB*/,
    2048,       4096,       9216,       18432,      36864,      73728,      147456,     294912,     589824,     1179648/*1.125MB*/,
    2359296,    4718592,    9437184,    18874368,   37748736/*36MB*/
};
const unsigned int TinyMemory::NUM_LEVELS = 25; // Keep pace with hard code in the header
const unsigned char TinyMemory::LEGAL_ALIGNMENTS[] = {2, 4, 8, 16, 32, 64};
const unsigned int TinyMemory::NUM_LEGAL_ALIGNMENT = 6;
const unsigned int TinyMemory::NUM_CELLS_PER_BLOCK = 100; // Keep pace with hard code in the header
const unsigned int TinyMemory::ENABLED_CELLS_PER_BLOCK[] =
{
    100,        100,        100,        100,        100,        100,        100,        100,        100,        100,
    100,        100,        50,         50,         30,         15,         10,         5,          2,          1,
    1,          1,          1,          1,          1
};

/* PUBLIC */

TinyMemory::TinyMemory() :
    alignment(0)
{
    // Do nothing
}

TinyMemory::~TinyMemory()
{
    if (!this->isInitialized())
    {
        return;
    }
    
    // Free all memory, destruct objects.
    for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
    {
        TinyMemory_Block* block = &this->blocks[i];
        while (block != nullptr)
        {
            if (block->data != nullptr)
            {
                free(block->data);
                block->data = nullptr;
            }
            block = block->nextBlock;
        }
    }
}

bool TinyMemory::init(unsigned char alignment)
{
    if (alignment == 0)
    {
        return false;
    }
    if (this->isInitialized())
    {
        return false;
    }
    if (!this->isLegalAlignment(alignment))
    {
        return false;
    }
    
    this->alignment = alignment;
    memset(this->blocks, 0, sizeof(TinyMemory_Block) * TinyMemory::NUM_LEVELS);
    
    return true;
}

bool TinyMemory::init(unsigned char alignment, unsigned int reservedBlocks)
{
    if (this->init(alignment))
    {
        if (reservedBlocks > 0)
        {
            for (int indexOfBytesLevel = 0; indexOfBytesLevel < TinyMemory::NUM_LEVELS; ++indexOfBytesLevel)
            {
                TinyMemory_Block* block = nullptr;
                for (int reservedBlockIndex = 0; reservedBlockIndex < reservedBlocks; ++reservedBlockIndex)
                {
                    block = this->newBlock(block, indexOfBytesLevel);
                    if (block == nullptr)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool TinyMemory::isInitialized()
{
    return this->alignment != 0;
}

void* TinyMemory::allocateMemory(unsigned long numBytes)
{
    if (!this->isInitialized())
    {
        return nullptr;
    }
    // The size of bytes to allocate is out of ability
    if (numBytes > TinyMemory::BYTES_LEVELS[TinyMemory::NUM_LEVELS - 1])
    {
        assert(false);
        return nullptr;
    }
    
    int indexOfBytesLevel = this->getIndexOfBytesLevel(numBytes);
    if (indexOfBytesLevel == -1)
    {
        return nullptr;
    }
    
    TinyMemory_Block* block = this->getBlock(indexOfBytesLevel);
    if (block == nullptr)
    {
        return nullptr;
    }
    assert(block->numFreeCells > 0);
    
    --block->numFreeCells;
    unsigned int freeIndex = block->freeList[block->numFreeCells];
    block->freeList[block->numFreeCells] = UINT32_MAX;
    assert(isFreeCell(block, freeIndex));
    setFreeCell(block, freeIndex, false);
    
    char* allocPtr = &block->data[(TinyMemory::BYTES_LEVELS[indexOfBytesLevel] + this->alignment) * freeIndex];
    char* alignedPtr = this->alignMemory(allocPtr);
    
    return alignedPtr;
}

void* TinyMemory::allocateZeroMemory(unsigned long numBytes)
{
    void* ptr = this->allocateMemory(numBytes);
    if (ptr != nullptr)
    {
        char* offsetPtr = (char*)ptr - 1;
        unsigned char offset = *offsetPtr;
        char* rawPtr = (char*)ptr - offset;
        int indexOfBytesLevel = this->getIndexOfBytesLevel(numBytes);
        assert(indexOfBytesLevel != -1);
        memset(rawPtr, 0, TinyMemory::BYTES_LEVELS[indexOfBytesLevel] + this->alignment);
        *offsetPtr = offset;
    }
    return ptr;
}

bool TinyMemory::freeMemory(void *ptr)
{
    if (!this->isInitialized())
    {
        return false;
    }
    if (ptr == nullptr)
    {
        return true;
    }
    
    char* offsetPtr = (char*)ptr - 1;
    char* rawPtr = (char*)ptr - *offsetPtr;
    for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
    {
        TinyMemory_Block* block = &this->blocks[i];
        while (block != nullptr)
        {
            if (block->data != nullptr)
            {
                if (rawPtr >= block->data && rawPtr < block->tail)
                {
                    unsigned long offset = rawPtr - block->data;
                    unsigned int realCellSize = TinyMemory::BYTES_LEVELS[i] + this->alignment;
                    // Check the pointer is the header address of a cell
                    if (offset % realCellSize != 0)
                    {
                        printf("Free a undefined pointer %p. Maybe the pointer address has an offset.\n", ptr);
                        assert(false);
                        return false;
                    }
                    
                    unsigned long index = offset / realCellSize;
                    if (this->isFreeCell(block, (unsigned int)index))
                    {
                        printf("Free the same pointer more than once.\n");
                        assert(false);
                        return false;
                    }
                    
                    block->freeList[block->numFreeCells] = (unsigned int)index;
                    setFreeCell(block, (unsigned int)index, true);
                    ++block->numFreeCells;
                    return true;
                }
            }
            block = block->nextBlock;
        }
    }
    
    printf("Free a undefined pointer %p. Maybe the pointer is not allocated from the TinyMemory.\n", ptr);
    assert(false);
    return false;
}

void TinyMemory::cleanup()
{
    if (!this->isInitialized())
    {
        return;
    }
    
    for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
    {
        TinyMemory_Block* block = &this->blocks[i];
        while (block != nullptr)
        {
            if (block->data != nullptr)
            {
                if (block->numFreeCells == TinyMemory::ENABLED_CELLS_PER_BLOCK[block->indexOfBytesLevel])
                {
                    TinyMemory_Block* nextBlock = block->nextBlock;
                    free(block->data);
                    memset(block, 0, sizeof(TinyMemory_Block));
                    block->nextBlock = nextBlock;
                }
            }
            block = block->nextBlock;
        }
    }
}

void TinyMemory::debugPrint()
{
    if (!this->isInitialized())
    {
        printf("TinyMemory was not been initialized\n");
    }
    else
    {
        printf("TinyMemory:(%d)\n", TinyMemory::NUM_CELLS_PER_BLOCK);
        for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
        {
            TinyMemory_Block* block = &this->blocks[i];
            this->debugPrintBlock(block, 1);
        }
        printf("\n");
    }
}

bool TinyMemory::hasUnreleasedMemory()
{
    if (!isInitialized())
    {
        return false;
    }
    
    for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
    {
        TinyMemory_Block* block = &this->blocks[i];
        assert(block != nullptr);
        if(block->data != nullptr && block->numFreeCells != TinyMemory::ENABLED_CELLS_PER_BLOCK[block->indexOfBytesLevel])
        {
            return true;
        }
    }
    
    return false;
}

unsigned int TinyMemory::bytesUsed()
{
    unsigned int bytes = 0;
    for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
    {
        TinyMemory_Block* block = &this->blocks[i];
        unsigned int bytesOfCell = TinyMemory::BYTES_LEVELS[i];
        unsigned int totalCells = TinyMemory::ENABLED_CELLS_PER_BLOCK[i];
        while (block != nullptr)
        {
            if (block->data != nullptr)
            {
                bytes += (totalCells - block->numFreeCells) * bytesOfCell;
            }
            block = block->nextBlock;
        }
    }
    return bytes;
}

unsigned int TinyMemory::bytesReservedUnused()
{
    unsigned int bytes = 0;
    for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
    {
        TinyMemory_Block* block = &this->blocks[i];
        unsigned int bytesOfCell = TinyMemory::BYTES_LEVELS[i];
        while (block != nullptr)
        {
            if (block->data != nullptr)
            {
                bytes += block->numFreeCells * bytesOfCell;
            }
            block = block->nextBlock;
        }
    }
    return bytes;
}

/* PRIVATE */

int TinyMemory::getIndexOfBytesLevel(unsigned long numBytes)
{
    assert(numBytes <= TinyMemory::BYTES_LEVELS[TinyMemory::NUM_LEVELS - 1]);
    
    for (int i = 0; i < TinyMemory::NUM_LEVELS; ++i)
    {
        if (numBytes <= TinyMemory::BYTES_LEVELS[i])
        {
            return i;
        }
    }
    return -1;
}

TinyMemory_Block* TinyMemory::getBlock(unsigned int indexOfBytesLevel)
{
    assert(indexOfBytesLevel < TinyMemory::NUM_LEVELS);
    
    TinyMemory_Block* block = &this->blocks[indexOfBytesLevel];
    if (block->data == nullptr)
    {
        if (initBlock(block, indexOfBytesLevel))
        {
            return block;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        while (true)
        {
            if (block->numFreeCells > 0)
            {
                return block;
            }
            else
            {
                if (block->nextBlock == nullptr)
                {
                    return this->newBlock(block, indexOfBytesLevel);
                }
                else
                {
                    block = block->nextBlock;
                }
            }
        }
        return nullptr;
    }
}

TinyMemory_Block* TinyMemory::newBlock(TinyMemory_Block *prevBlock, unsigned int indexOfBytesLevel)
{
    assert(indexOfBytesLevel < TinyMemory::NUM_LEVELS);
    assert(prevBlock == nullptr || prevBlock->nextBlock == nullptr);
    
    if (prevBlock == nullptr)
    {
        return this->getBlock(indexOfBytesLevel);
    }
    else
    {
        TinyMemory_Block* nextBlock = (TinyMemory_Block*)malloc(sizeof(TinyMemory_Block));
        if(nextBlock == nullptr)
        {
            return nullptr;
        }
        else
        {
            nextBlock->data = nullptr;
            if (this->initBlock(nextBlock, indexOfBytesLevel))
            {
                prevBlock->nextBlock = nextBlock;
                return nextBlock;
            }
            else
            {
                free(nextBlock);
                return nullptr;
            }
        }
    }
}

bool TinyMemory::initBlock(TinyMemory_Block *block, unsigned int indexOfBytesLevel)
{
    assert(block != nullptr);
    assert(block->data == nullptr);
    assert(indexOfBytesLevel < TinyMemory::NUM_LEVELS);
    
    memset(block, 0, sizeof(TinyMemory_Block));
    
    size_t numBytes = (TinyMemory::BYTES_LEVELS[indexOfBytesLevel] + this->alignment) * TinyMemory::ENABLED_CELLS_PER_BLOCK[indexOfBytesLevel];
    block->data = (char*)malloc(numBytes);
    if (block->data == nullptr)
    {
        return false;
    }
    
    block->tail = block->data + numBytes;
    block->numFreeCells = TinyMemory::ENABLED_CELLS_PER_BLOCK[indexOfBytesLevel];
    block->indexOfBytesLevel = indexOfBytesLevel;
    
    for (int i = 0; i < block->numFreeCells; ++i)
    {
        block->freeList[i] = i;
        block->states[i] = false;
    }
    
    return true;
}

char* TinyMemory::alignMemory(char *memory)
{
    assert(memory != nullptr);
    
    unsigned long address = (unsigned long)memory;
    unsigned long mask = address & (this->alignment - 1);
    unsigned long offset = this->alignment - mask;
    char* alignedMemory = memory + offset;
    char* offsetPtr = alignedMemory - 1;
    *offsetPtr = offset;
    
    return alignedMemory;
}

bool TinyMemory::isLegalAlignment(unsigned char alignment)
{
    for (int i = 0; i < TinyMemory::NUM_LEGAL_ALIGNMENT; ++i)
    {
        if (TinyMemory::LEGAL_ALIGNMENTS[i] == alignment)
        {
            return true;
        }
    }
    return false;
}

bool TinyMemory::isFreeCell(TinyMemory_Block *block, unsigned int index)
{
    assert(block != nullptr);
    assert(index < TinyMemory::ENABLED_CELLS_PER_BLOCK[block->indexOfBytesLevel]);
    assert(block->data != nullptr);
    
    return !block->states[index];
}

void TinyMemory::setFreeCell(TinyMemory_Block *block, unsigned int index, bool isFree)
{
    assert(block != nullptr);
    assert(index < TinyMemory::ENABLED_CELLS_PER_BLOCK[block->indexOfBytesLevel]);
    assert(block->data != nullptr);
    
    block->states[index] = !isFree;
}

void TinyMemory::debugPrintBlock(TinyMemory_Block *block, unsigned int depth)
{
    assert(block != nullptr);
    assert(block->indexOfBytesLevel < TinyMemory::NUM_LEVELS);
    
    this->debugPrintTabs(depth);
    printf("Block %d %d:", TinyMemory::BYTES_LEVELS[block->indexOfBytesLevel], TinyMemory::ENABLED_CELLS_PER_BLOCK[block->indexOfBytesLevel]);
    if (block->data == nullptr)
    {
        printf("Not initialized");
    }
    else
    {
        printf("FreeList:");
        for (int i = 0; i < block->numFreeCells; ++i)
        {
            printf("%d,", block->freeList[i]);
        }
    }
    printf("\n");
    
    if (block->nextBlock)
    {
        this->debugPrintBlock(block->nextBlock, depth + 1);
    }
}

void TinyMemory::debugPrintTabs(unsigned int depth)
{
    for (int i = 0; i < depth; ++i)
    {
        printf("    ");
    }
}