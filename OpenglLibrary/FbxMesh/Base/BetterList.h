//
//  BetterList.h
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__BetterList__
#define __FbxMesh__BetterList__

#include "BetterListAllocator.h"

template<class T>
class BetterList
{
public:
    BetterList();
    BetterList(BetterListAllocator* allocator);
    BetterList(unsigned int capacity);
    BetterList(unsigned int capacity, BetterListAllocator* allocator);
    BetterList(unsigned int capacity, bool seekToEnd);
    BetterList(unsigned int capacity, bool seekToEnd, BetterListAllocator* allocator);
    ~BetterList();
    
    T& operator[](unsigned int index);
    bool toArray(T array[]);
    void clear();
    void release();
    bool add(T item);
    bool insert(unsigned int index, T item);
    bool contains(T item);
    bool remove(T item);
    bool removeAt(unsigned int index);
    T pop();
    unsigned int length();
    T* rawBuffer();
    BetterListAllocator* getAllocator();
    
private:
    T* buffer;
    unsigned int bufferSize;
    unsigned int size;
    BetterListAllocator* allocator;
    
private:
    BetterList<T>(const BetterList<T>&);
    const BetterList<T> operator=(const BetterList<T>&);
    
    bool AllocateMore();
    bool InitAllocate(unsigned int capacity);
    void releaseBuffer();
    unsigned int max(unsigned int a, unsigned int b);
    BetterListAllocator* currentAllocator();
};

#endif /* defined(__FbxMesh__BetterList__) */