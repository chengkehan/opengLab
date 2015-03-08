//
//  cgUtil.cpp
//  Base_on_cg
//
//  Created by jimCheng on 15/2/15.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <string.h>
#include "cgUtil.h"

void cgCheckError(const char* situation, CGcontext cgContext)
{
    CGerror error;
    const char *string = cgGetLastErrorString(&error);
    
    if (error != CG_NO_ERROR)
    {
        printf("cg error: %s: %s\n", situation, string);
        if (error == CG_COMPILER_ERROR)
        {
            printf("cg error: %s\n", cgGetLastListing(cgContext));
        }
        exit(1);
    }
}

char* cgCopyString(const char* str)
{
    if (str == nullptr)
    {
        return nullptr;
    }
    
    size_t strLength = strlen(str);
    char* newStr = (char*)malloc(sizeof(char) * (strLength + 1));
    if (newStr == nullptr)
    {
        cgPrintThenExit("Allocate memory fail when execute cgCopyString");
        return nullptr;
    }
    
    newStr[strLength] = '\0';
    memcpy(newStr, str, strLength);
    
    return newStr;
}