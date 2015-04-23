//
//  main_betterList_test.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <stdio.h>
#include "BetterList.cpp"

class ClassA
{
public:
    int value;
};

template <class T>
void printBetterList(BetterList<T>& betterList, const char* printFormat)
{
    for (int i = 0; i < betterList.length(); ++i)
    {
        printf(printFormat, betterList[i]);
    }
    printf("\n");
}

void test0()
{
    BetterList<int> list;
    for (int i = 0; i < 100; ++i)
    {
        list.add(i);
    }
    printBetterList(list, "%d ");
}

void test1()
{
    BetterList<ClassA*> list;
    for (int i = 0; i < 100; ++i)
    {
        list.add(new ClassA());
        list[i]->value = i;
    }
    
    for (int i = 0; i < list.length(); ++i)
    {
        printf("%d ", list[i]->value);
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    printf("Test BetterList\n");
    
//    test0();
    test1();
    
    return 0;
}
