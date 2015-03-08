//
//  FileReader.h
//  Base_on_cg
//
//  Created by jimCheng on 15/2/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __Base_on_cg__FileReader__
#define __Base_on_cg__FileReader__

class FileReader
{
public:
    FileReader();
    ~FileReader();
    
    bool read(const char* path);
    const char* content();
    
private:
    FileReader(const FileReader&);
    const FileReader& operator=(const FileReader&);
    
    void releaseMemory();
    
private:
    char* m_content;
};

#endif /* defined(__Base_on_cg__FileReader__) */
