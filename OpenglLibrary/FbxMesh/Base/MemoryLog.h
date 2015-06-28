//
//  MemoryLog.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/27.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__MemoryLog__
#define __FbxMesh__MemoryLog__

#include "TinyMemory.h"

typedef struct MemoryLog_Item
{
    char* file;
    unsigned int line;
    struct MemoryLog_Item* nextItem;
}
MemoryLog_Item;

typedef struct MemoryLog_StatisticInfo
{
    MemoryLog_Item* item;
    struct MemoryLog_StatisticInfo* nextStatisticInfo;
    struct MemoryLog_StatisticInfo* siblingStatisticInfo;
}
MemoryLog_StatisticInfo;

class MemoryLog
{
public:
    MemoryLog();
    ~MemoryLog();
    
    bool allocateMemoryLog(const char* file, unsigned int line);
    bool releaseMemoryLog(const char* file, unsigned int line);
    void printLog(bool detail);
    
private:
    MemoryLog(const MemoryLog&);
    MemoryLog& operator=(const MemoryLog&);
    
    jcgame::TinyMemory tinyMemory;
    MemoryLog_Item* allocationRootItem;
    MemoryLog_Item* releaseRootItem;
    
    bool addMemoryLog(MemoryLog_Item** rootItem, const char* file, unsigned int line);
    MemoryLog_StatisticInfo* createStstisticInfo(MemoryLog_Item* rootItem);
    void releaseStatisticInfo(MemoryLog_StatisticInfo* statisticInfo);
    void releaseItem(MemoryLog_Item* rootItem);
};

#endif /* defined(__FbxMesh__MemoryLog__) */
