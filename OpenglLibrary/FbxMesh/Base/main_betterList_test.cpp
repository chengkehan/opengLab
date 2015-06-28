//
//  main_betterList_test.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <stdio.h>
#include "BetterList.cpp"
#include "MemoryLeakDetector.h"
#include "Memory.h"

MemoryLeakDetector memoryLeakDetector;

class Base
{
public:
    Base(){ printf("Base Constructor\n"); }
    virtual ~Base() { printf("Base Destructor\n"); }
};

class ClassA : public Base
{
public:
    ClassA() { printf("ClassA Constructor\n"); }
    ~ClassA() { printf("ClassA Destructor\n"); }
};

class ClassB
{
public:
    ClassB() { printf("ClassB Constructor\n"); }
    ~ClassB() { printf("ClassB Destructor\n"); }
};

void test1()
{
    ClassA* a = Memory_NewHeapObject(ClassA);
    Memory_DeleteHeapObject(a);
}

void test0()
{
    BetterList<unsigned short>* list = Memory_NewHeapObject(BetterList<unsigned short>);
    
    for (int i = 0; i < 33; ++i)
    {
        list->add(i);
    }
    
    Memory_DeleteHeapObject(list);
}

int main(int argc, char* argv[])
{
    printf("Test BetterList\n");
    
    test0();
//    test1();
    
    return 0;
}
