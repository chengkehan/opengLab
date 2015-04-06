//
//  BetterList.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "BetterList.h"

/* PUBLIC */

template<class T>
BetterList<T>::BetterList() :
    buffer(nullptr), bufferSize(0), size(0)
{
    // Do nothing
}

template<class T>
BetterList<T>::~BetterList()
{
    releaseBuffer();
}

template<class T>
T& BetterList<T>::operator[](unsigned int index)
{
    if (index >= size)
    {
        return nullptr;
    }
    return buffer[index];
}

template<class T>
bool BetterList<T>::toArray(T *array)
{
    if (array == nullptr)
    {
        return false;
    }
    if (size == 0)
    {
        return true;
    }
    for (int i = 0; i < size; ++i)
    {
        array[i] = buffer[i];
    }
    return true;
}

template<class T>
void BetterList<T>::clear()
{
    size = 0;
}

template<class T>
void BetterList<T>::release()
{
    releaseBuffer();
    size = 0;
}

template<class T>
bool BetterList<T>::add(T item)
{
    if (size == bufferSize)
    {
        if(!AllocateMore())
        {
            return false;
        }
    }
    buffer[size++] = item;
    return true;
}

template<class T>
bool BetterList<T>::insert(unsigned int index, T item)
{
    if (size == bufferSize)
    {
        if(!AllocateMore())
        {
            return false;
        }
    }
    
    if (index < size)
    {
        for (int i = size; i > index; --i) buffer[i] = buffer[i - 1];
        buffer[index] = item;
        ++size;
        return true;
    }
    else
    {
        return add(item);
    }
}

template<class T>
bool BetterList<T>::contains(T item)
{
    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == item)
        {
            return true;
        }
    }
    return false;
}

template<class T>
bool BetterList<T>::remove(T item)
{
    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == item)
        {
            --size;
            for (int b = i; b < size; ++b) buffer[b] = buffer[b + 1];
            return true;
        }
    }
    return false;
}

template<class T>
bool BetterList<T>::removeAt(unsigned int index)
{
    if (index < size)
    {
        --size;
        for (int b = index; b < size; ++b) buffer[b] = buffer[b + 1];
        return true;
    }
    return false;
}

template<class T>
T BetterList<T>::pop()
{
    if (size != 0)
    {
        return buffer[--size];
    }
    return nullptr;
}

/* PRIVATE */

template<class T>
bool BetterList<T>::AllocateMore()
{
    unsigned int newBufferSize = max(bufferSize << 1, 32);
    T* newBuffer = new T[newBufferSize];
    if (newBuffer == nullptr)
    {
        return false;
    }
    toArray(newBuffer);
    releaseBuffer();
    buffer = newBuffer;
    bufferSize = newBufferSize;
    return true;
}

template<class T>
void BetterList<T>::releaseBuffer()
{
    if (buffer != nullptr)
    {
        delete[] buffer;
        buffer = nullptr;
    }
    bufferSize = 0;
}

template<class T>
unsigned int BetterList<T>::max(unsigned int a, unsigned int b)
{
    return a > b ? a : b;
}
