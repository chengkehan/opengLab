//
//  MemoryLog.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/27.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "MemoryLog.h"
#include <string.h>
#include <stdio.h>

/* PUBLIC */

MemoryLog::MemoryLog()
{
    tinyMemory.init(4);
}

MemoryLog::~MemoryLog()
{
    releaseItem(allocationRootItem);
    allocationRootItem = nullptr;
    
    releaseItem(releaseRootItem);
    releaseRootItem = nullptr;
}

bool MemoryLog::allocateMemoryLog(const char *file, unsigned int line)
{
    return addMemoryLog(&allocationRootItem, file, line);
}

bool MemoryLog::releaseMemoryLog(const char* file, unsigned int line)
{
    return addMemoryLog(&releaseRootItem, file, line);
}

void MemoryLog::printLog(bool listDetailLines, bool ignoreUnimportantInfo)
{
    MemoryLog_StatisticInfo* allocationMemoryStatisticInfo = createStstisticInfo(allocationRootItem);
    MemoryLog_StatisticInfo* releaseMemoryStatisticInfo = createStstisticInfo(releaseRootItem);
    
    if (allocationMemoryStatisticInfo == nullptr || allocationMemoryStatisticInfo->item == nullptr ||
        releaseMemoryStatisticInfo == nullptr || releaseMemoryStatisticInfo->item == nullptr)
    {
        printf("Memory log is empty, nothing print.\n");
    }
    else
    {
        printf("Memory log:\n");
        bool printSth = false;
        for (MemoryLog_StatisticInfo* i = allocationMemoryStatisticInfo; i != nullptr; i = i->siblingStatisticInfo)
        {
            int numAllocations = 0;
            int numReleases = 0;
            for (MemoryLog_StatisticInfo* j = i; j != nullptr; j = j->nextStatisticInfo)
            {
                ++numAllocations;
            }
            for (MemoryLog_StatisticInfo* l = releaseMemoryStatisticInfo; l != nullptr; l = l->siblingStatisticInfo)
            {
                if (strcmp(l->item->file, i->item->file) == 0)
                {
                    for (MemoryLog_StatisticInfo* m = l; m != nullptr; m = m->nextStatisticInfo)
                    {
                        ++numReleases;
                    }
                }
            }
            
            if(numAllocations != numReleases || !ignoreUnimportantInfo)
            {
                printSth = true;
                printf("   %sFile:%s. Allocations:%d. Releases:%d.\n", numAllocations == numReleases ? " " : "*", i->item->file, numAllocations, numReleases);
                
                if (listDetailLines)
                {
                    for (MemoryLog_StatisticInfo* j = i; j != nullptr; j = j->nextStatisticInfo)
                    {
                        printf("        Allocation:%s(%d)\n", j->item->file, j->item->line);
                    }
                    for (MemoryLog_StatisticInfo* l = releaseMemoryStatisticInfo; l != nullptr; l = l->siblingStatisticInfo)
                    {
                        if (strcmp(l->item->file, i->item->file) == 0)
                        {
                            for (MemoryLog_StatisticInfo* m = l; m != nullptr; m = m->nextStatisticInfo)
                            {
                                printf("        Release:%s(%d)\n", m->item->file, m->item->line);
                            }
                        }
                    }
                }
            }
        }
        if (!printSth)
        {
            printf("    Nothing to print.\n");
        }
    }
    
    releaseStatisticInfo(allocationMemoryStatisticInfo);
    releaseStatisticInfo(releaseMemoryStatisticInfo);
}

/* PRIVATE */

bool MemoryLog::addMemoryLog(MemoryLog_Item **rootItem, const char *file, unsigned int line)
{
    if (file == nullptr)
    {
        return false;
    }
    
    size_t strLength = strlen(file);
    if (strLength == 0)
    {
        return false;
    }
    
    MemoryLog_Item* newItem = (MemoryLog_Item*)tinyMemory.allocateMemory(sizeof(MemoryLog_Item));
    newItem->file = (char*)tinyMemory.allocateMemory((unsigned int)(strLength + 1));
    memcpy(newItem->file, file, strLength);
    newItem->file[strLength] = '\0';
    newItem->line = line;
    newItem->nextItem = nullptr;
    
    if (*rootItem == nullptr)
    {
        *rootItem = newItem;
    }
    else
    {
        MemoryLog_Item* nextItemOfRoot = (*rootItem)->nextItem;
        (*rootItem)->nextItem = newItem;
        newItem->nextItem = nextItemOfRoot;
    }
    
    return true;

}

MemoryLog_StatisticInfo* MemoryLog::createStstisticInfo(MemoryLog_Item *rootItem)
{
    MemoryLog_StatisticInfo* statisticInfo = (MemoryLog_StatisticInfo*)tinyMemory.allocateMemory(sizeof(MemoryLog_StatisticInfo));
    statisticInfo->item = nullptr;
    statisticInfo->nextStatisticInfo = nullptr;
    statisticInfo->siblingStatisticInfo = nullptr;
    
    MemoryLog_Item* item = rootItem;
    while (item != nullptr)
    {
        if (statisticInfo->item == nullptr)
        {
            statisticInfo->item = item;
        }
        else
        {
            bool isMatched = false;
            for (MemoryLog_StatisticInfo* i = statisticInfo; i != nullptr; i = i->siblingStatisticInfo)
            {
                for (MemoryLog_StatisticInfo* j = i; j != nullptr; j = j->nextStatisticInfo)
                {
                    if (strcmp(j->item->file, item->file) == 0)
                    {
                        isMatched = true;
                        MemoryLog_StatisticInfo* newStatisticInfo = (MemoryLog_StatisticInfo*)tinyMemory.allocateMemory(sizeof(MemoryLog_StatisticInfo));
                        newStatisticInfo->item = item;
                        newStatisticInfo->nextStatisticInfo = nullptr;
                        newStatisticInfo->siblingStatisticInfo = nullptr;
                        MemoryLog_StatisticInfo* nextStatisticInfoOfJ = j->nextStatisticInfo;
                        j->nextStatisticInfo = newStatisticInfo;
                        newStatisticInfo->nextStatisticInfo = nextStatisticInfoOfJ;
                        break;
                    }
                }
                if (isMatched)
                {
                    break;
                }
            }
            if (!isMatched)
            {
                MemoryLog_StatisticInfo* newStatisticInfo = (MemoryLog_StatisticInfo*)tinyMemory.allocateMemory(sizeof(MemoryLog_StatisticInfo));
                newStatisticInfo->item = item;
                newStatisticInfo->nextStatisticInfo = nullptr;
                newStatisticInfo->siblingStatisticInfo = nullptr;
                MemoryLog_StatisticInfo* siblingStatisticInfo = statisticInfo->siblingStatisticInfo;
                statisticInfo->siblingStatisticInfo = newStatisticInfo;
                newStatisticInfo->siblingStatisticInfo = siblingStatisticInfo;
            }
            
        }
        item = item->nextItem;
    }

    return statisticInfo;
}

void MemoryLog::releaseStatisticInfo(MemoryLog_StatisticInfo *statisticInfo)
{
    if (statisticInfo == nullptr)
    {
        return;
    }
    
    MemoryLog_StatisticInfo* nextStatisticInfo = statisticInfo->nextStatisticInfo;
    MemoryLog_StatisticInfo* siblingStatisticInfo = statisticInfo->siblingStatisticInfo;
    statisticInfo->item = nullptr;
    
    tinyMemory.freeMemory(statisticInfo);
    
    releaseStatisticInfo(nextStatisticInfo);
    releaseStatisticInfo(siblingStatisticInfo);
}

void MemoryLog::releaseItem(MemoryLog_Item *rootItem)
{
    MemoryLog_Item* item = rootItem;
    while (item != nullptr)
    {
        MemoryLog_Item* nextItem = item->nextItem;
        tinyMemory.freeMemory(item->file);
        tinyMemory.freeMemory(item);
        item = nextItem;
    }
}
