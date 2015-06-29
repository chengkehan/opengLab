//
//  TinyMemory.h
//  IsometricModel
//
//  Created by jimCheng on 14-10-6.
//  Copyright (c) 2014年 JimCheng. All rights reserved.
//

#ifndef __IsometricModel__TinyMemory__
#define __IsometricModel__TinyMemory__

namespace jcgame
{
    // A block of memory
    // The block will be divided into many same small size cells to use.
    typedef struct TinyMemory_Block
    {
        // Link to the next block
        struct TinyMemory_Block* nextBlock;
        // Memory data
        char* data;
        // The tail pointer of the memory data
        char* tail;
        // The number of unused cells
        unsigned int numFreeCells;
        // All indices of unused cells
        unsigned int freeList[100]; // TinyMemory::NUM_CELLS_PER_BLOCK(hard code here)
        // The status of cell, in using(true) or free(false).
        bool states[100]; // TinyMemory::NUM_CELLS_PER_BLOCK(hard code here)
        // Index of BYTES_LEVELS
        unsigned int indexOfBytesLevel;
    }
    TinyMemory_Block;
    
    // The manager for all TinyMemory_Blocks
    class TinyMemory
    {
    public:
        TinyMemory();
        ~TinyMemory();
        
        // Initialize with memory address alignment
        bool init(unsigned char alignment);
        // Initialize with memory address alignment and some reserved memory blocks 
        bool init(unsigned char alignment, unsigned int reservedBlocks);
        // Whether the object has been initialized
        bool isInitialized();
        // Allocate memory with how many bytes you want
        void* allocateMemory(unsigned int numBytes);
        // Allocate memory and all byte will be set with zero
        void* allocateZeroMemory(unsigned int numBytes);
        // Free memory
        bool freeMemory(void* ptr);
        // Destroy all unused TinyMemory_Blocks
        void cleanup();
        // Print the inner struct of TinyMemory for debug
        void debugPrint();
        // Check whether there is any unreleased data remain in memory.
        // You can use this function to check memory leak while the program exit.
        bool hasUnreleasedMemory();
        // The number of bytes that has been allocated to application to use.
        unsigned int bytesUsed();
        // The number of free bytes.
        unsigned int bytesReservedUnused();
        
    private:
        // Define the increment of each block's cell
        static const unsigned int BYTES_LEVELS[];
        // The size of BYTES_LEVELS
        static const unsigned int NUM_LEVELS;
        // The legal memory address alignment
        static const unsigned char LEGAL_ALIGNMENTS[];
        // The size of LEGAL_ALIGNMENT
        static const unsigned int NUM_LEGAL_ALIGNMENT;
        // The number of cells in one memory block
        static const unsigned int NUM_CELLS_PER_BLOCK;
        // How many cells of block is enabled.
        // It must be less than or equal to NUM_CELLS_PER_BLOCK.
        // The length of this array must be equal to NUM_LEVELS.
        static const unsigned int ENABLED_CELLS_PER_BLOCK[];
        
    private:
        TinyMemory(const TinyMemory&);
        const TinyMemory& operator=(const TinyMemory&);
        
        // The number of bytes will map to which index of BYTES_LEVELS
        int getIndexOfBytesLevel(unsigned int numBytes);
        // Get memory block with index of BYTES_LEVELS
        TinyMemory_Block* getBlock(unsigned int indexOfLevel);
        // New memory block
        TinyMemory_Block* newBlock(TinyMemory_Block* prevBlock, unsigned int indexOfLevel);
        // Initialize memory block
        bool initBlock(TinyMemory_Block* block, unsigned int indexOfLevel);
        // Get a aligned memory address
        char* alignMemory(char* memory);
        // Whether is legal memory address alignment
        bool isLegalAlignment(unsigned char alignment);
        // Whether the cell is in using or free at the index
        bool isFreeCell(TinyMemory_Block* block, unsigned int index);
        // Set the cell is in using or free at the index
        void setFreeCell(TinyMemory_Block* block, unsigned index, bool isFree);
        // Print the inner struct of TinyMemory_Block for debug
        void debugPrintBlock(TinyMemory_Block* block, unsigned int depth);
        // Print tabs with depth
        void debugPrintTabs(unsigned int depth);
        
        TinyMemory_Block blocks[25]; // TinyMemory::NUM_LEVELS(hard code here)
        // The alignment of memory address that allocate from this manager
        unsigned char alignment;
    };
}

#endif /* defined(__IsometricModel__TinyMemory__) */
