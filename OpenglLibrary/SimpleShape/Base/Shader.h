//
//  Shader.h
//  Base_on_cg
//
//  Created by jimCheng on 15/2/15.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __Base_on_cg__Shader__
#define __Base_on_cg__Shader__

#include <string>
#include <map>
#include <list>
#include "Cg/cg.h"
#include "Cg/cgGL.h"

class Shader
{
public:
    Shader(const char* name);
    ~Shader();
    
    bool init(CGcontext cgContext, const char* vertexProgramMainEntry, const char* fragmentProgramMainEntry);
    
    bool loadVertexProgramFromFile(const char* path);
    bool loadFragmentProgramFromFile(const char* path);
    bool loadVertexAndFragmentProgramFromFile(const char* path);
    bool loadVertexProgram(const char* program);
    bool loadFragmentProgram(const char* program);
    bool loadVertexAndFragmentProgram(const char* program);
    
    void enable();
    void disable();
    
    void setFloat(const char* parameterName, float value);
    void setFloat2(const char* parameterName, float x, float y);
    void setFloat4(const char* parameterName, float x, float y, float z, float w);
    void setFloat4x4(const char* parameterName, const float float4x4[16]);
    void setTexture(const char* parameterName, GLuint texture);
    
private:
    class EnabledTexture
    {
    public:
        std::string parameterName;
        CGparameter cgParameter;
    };
    
private:
    typedef std::map<std::string, CGparameter> cgParametersMapping_type;
    typedef std::map<std::string, CGparameter>::iterator cgParametersMapping_type_iterator;
    typedef std::map<std::string, CGparameter>::value_type cgParametersMapping_type_valuePair;
    typedef std::list<EnabledTexture*> enabledTextures_type;
    typedef std::list<EnabledTexture*>::iterator enabledTextures_type_iterator;
private:
    Shader(const Shader&);
    const Shader& operator=(const Shader&);
    
    void release();
    void releaseVertexProgram();
    void releaseFragmentProgram();
    bool loadProgramFromFile(const char* path, bool vertexOrFragment);
    bool loadProgram(const char* program, bool vertexOrFragment);
    CGparameter getCGparameter(const char* parameterName);
    
private:
    CGcontext cgContext;
    CGprogram cgVertexProgram;
    CGprogram cgFragmentProgram;
    CGprofile cgVertexProfile;
    CGprofile cgFragmentProfile;
    std::string vertexProgramMainEntry;
    std::string fragmentProgramMainEntry;
    cgParametersMapping_type cgParametersMapping;
    std::string name;
    enabledTextures_type enabledTextures;
};

#endif /* defined(__Base_on_cg__Shader__) */
