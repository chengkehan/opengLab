//
//  cgUtil.h
//  Base_on_cg
//
//  Created by jimCheng on 15/2/15.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __Base_on_cg__cgUtil__
#define __Base_on_cg__cgUtil__

#include <stdio.h>
#include <stdlib.h>
#include "Cg/cg.h"
#include "Cg/cgGL.h"

#define cgPrintThenExit(...) printf(__VA_ARGS__);exit(1);
#define cgPrint(...) printf(__VA_ARGS__);

void cgCheckError(const char* situation, CGcontext cgContext);

char* cgCopyString(const char* str);

#endif /* defined(__Base_on_cg__cgUtil__) */
