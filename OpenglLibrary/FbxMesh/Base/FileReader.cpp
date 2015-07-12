//
//  FileReader.cpp
//  Base_on_cg
//
//  Created by jimCheng on 15/2/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "FileReader.h"
#include "util.h"
#include "Memory.h"

/* PUBLIC */

FileReader::FileReader() :
    m_content(nullptr)
{
    // Do nothing
}

FileReader::~FileReader()
{
    releaseMemory();
}

bool FileReader::read(const char *path)
{
    if (path == nullptr)
    {
        printThenExit("FileReader::read input parameter is null");
        return false;
    }
    
    FILE* file = fopen(path, "rb");
    if (file == nullptr)
    {
        printThenExit("File is not exists. %s.\n", path);
        return false;
    }
    
    int seekResult = fseek(file, 0, SEEK_END);
    if (seekResult != 0)
    {
        printThenExit("Seek to end of file error. %s\n", path);
        return false;
    }
    
    long fileSize = ftell(file);
    if (fileSize == -1L)
    {
        printThenExit("Get size of file fail. %s\n", path);
        return false;
    }
    
    rewind(file);
    
    releaseMemory();
    m_content = (char*)Memory_MallocHeapBlock(sizeof(char) * (fileSize + 1));
    
    if (m_content == nullptr)
    {
        printThenExit("Allocate memory fail for reading file. %s\n", path);
        return false;
    }
    m_content[sizeof(char) * fileSize] = '\0';
    
    size_t readFileResult = fread(m_content, 1, fileSize, file);
    if (readFileResult != fileSize)
    {
        releaseMemory();
        printThenExit("Read file fail. %s\n", path);
        return false;
    }
    
    fclose(file);
    
    return true;
}

const char* FileReader::content()
{
    return m_content;
}

/* PRIVATE */

void FileReader::releaseMemory()
{
    Memory_FreeHeapBlock(m_content);
    m_content = nullptr;
}