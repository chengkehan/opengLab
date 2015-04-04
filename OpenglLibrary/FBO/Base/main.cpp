// Block.cpp
// OpenGL SuperBible, Chapter 1
// Demonstrates an assortment of basic 3D concepts
// Program by Richard S. Wright Jr.

#include <GLTools.h>	// OpenGL toolkit
#include "glHeader.h"
#include "ShaderManager.h"
#include "Vector4.h"
#include "AttributeBindingLocation.h"
#include "util.h"

const char* projectName = "FBO";
ShaderManager shaderManager;

GLuint texture;
GLuint fbo;
GLuint depthRBO;
GLuint colorTexture;
GLint colorTextureWidth = 800;
GLint colorTextureHeight = 600;

void initRC();
void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();
void drawScreenQuard();

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    shaderManager.init(projectName);
    initRC();
}

void initRC()
{
    GLint width;
    GLint height;
    GLint components;
    GLenum format;
    GLbyte* bytes = gltReadTGABits("/Users/jimCheng/projects/OpenglLibrary/FBO/Base/Block6.tga", &width, &height, &components, &format);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
    free(bytes);
    
    GLint maxRboSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRboSize);
    if (maxRboSize < colorTextureWidth || maxRboSize < colorTextureHeight)
    {
        printThenExit("Max rbo size:%d.", maxRboSize);
        return;
    }
    
    glGenFramebuffers(1, &fbo);
    glGenRenderbuffers(1, &depthRBO);
    glGenTextures(1, &colorTexture);
    
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, colorTextureWidth, colorTextureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, colorTextureWidth, colorTextureHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
    
    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        printThenExit("fbo complete status fail.");
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ReleaseRC()
{
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, & depthRBO);
    glDeleteTextures(1, &colorTexture);
    glDeleteFramebuffers(1, &fbo);
}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    drawScreenQuard();
    
    // Flush drawing commands
    glutSwapBuffers();
}

GLuint compactAttributeVBO = 0;
GLuint compactIndexVBO = 0;
void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject()
{
    shaderManager.shader(SHADER_TYPE_IDENTITY_COLOR_TEXTURE)->setFloat4("color", 0, 1, 0, 1);
    shaderManager.shader(SHADER_TYPE_IDENTITY_COLOR_TEXTURE)->setTexture("texUnit", texture, 0);
    shaderManager.shader(SHADER_TYPE_IDENTITY_COLOR_TEXTURE)->enable();
    
    int numVertices = 3;
    
    if (compactAttributeVBO == 0)
    {
        // Attributes
        Vector4* attributes = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices * 3));
        // Position
        attributes[0] = Vector4(0, 0, 0, 1);
        attributes[1] = Vector4(1, 1, 0, 1);
        attributes[2] = Vector4(1, 0, 0, 1);
        // Color
        attributes[3] = Vector4(1, 0, 0, 1);
        attributes[4] = Vector4(0, 1, 0, 1);
        attributes[5] = Vector4(0, 0, 1, 1);
        // UV
        attributes[6] = Vector4(0, 0, 0, 0);
        attributes[7] = Vector4(1, 1, 0, 0);
        attributes[8] = Vector4(1, 0, 0, 0);
        
        glGenBuffers(1, &compactAttributeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, compactAttributeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * numVertices * 3, attributes, GL_STATIC_DRAW);
        free(attributes);
        
        // Indices
        GLushort* indices = static_cast<GLushort*>(malloc(sizeof(GLushort) * 3));
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        
        glGenBuffers(1, &compactIndexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compactIndexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 3, indices, GL_STATIC_DRAW);
        free(indices);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, compactAttributeVBO);
    glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_POSITION, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
    glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_POSITION);
    glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_COLOR, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(sizeof(Vector4) * numVertices));
    glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_COLOR);
    glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_TEXTURE0, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(sizeof(Vector4) * numVertices * 2));
    glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_TEXTURE0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compactIndexVBO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
}

GLuint screenQuardAttributeVBO = 0;
GLuint screenQuardIndexVBO = 0;
void drawScreenQuard()
{
    shaderManager.shader(SHADER_TYPE_IDENTITY_COLOR_TEXTURE)->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader(SHADER_TYPE_IDENTITY_COLOR_TEXTURE)->setTexture("texUnit", colorTexture, 0);
    shaderManager.shader(SHADER_TYPE_IDENTITY_COLOR_TEXTURE)->enable();

    int numVertices = 4;
    
    if (screenQuardAttributeVBO == 0)
    {
        // Attributes
        Vector4* attributes = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices * 3));
        // Position
        attributes[0] = Vector4(-1, -1, 0, 1);
        attributes[1] = Vector4(1, 1, 0, 1);
        attributes[2] = Vector4(1, -1, 0, 1);
        attributes[3] = Vector4(-1, 1, 0, 1);
        // Color
        attributes[4] = Vector4(1, 1, 1, 1);
        attributes[5] = Vector4(1, 1, 1, 1);
        attributes[6] = Vector4(1, 1, 1, 1);
        attributes[7] = Vector4(1, 1, 1, 1);
        // UV
        attributes[8] = Vector4(0, 0, 0, 0);
        attributes[9] = Vector4(1, 1, 0, 0);
        attributes[10] = Vector4(1, 0, 0, 0);
        attributes[11] = Vector4(0, 1, 0, 0);
        
        glGenBuffers(1, &screenQuardAttributeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, screenQuardAttributeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * numVertices * 3, attributes, GL_STATIC_DRAW);
        free(attributes);
        
        // Indices
        GLushort* indices = static_cast<GLushort*>(malloc(sizeof(GLushort) * 6));
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 3;
        indices[5] = 1;
        
        glGenBuffers(1, &screenQuardIndexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuardIndexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
        free(indices);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, screenQuardAttributeVBO);
    glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_POSITION, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
    glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_POSITION);
    glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_COLOR, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(sizeof(Vector4) * numVertices));
    glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_COLOR);
    glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_TEXTURE0, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(sizeof(Vector4) * numVertices * 2));
    glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_TEXTURE0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuardIndexVBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}


///////////////////////////////////////////////////////////////////////////////
// A normal ASCII key has been pressed.
// In this case, advance the scene when the space bar is pressed
void KeyPressFunc(unsigned char key, int x, int y)
{
    if (key == 27/*escape*/)
    {
        ReleaseRC();
        exit(0);
    }
    
    // Refresh the Window
    glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow(projectName);
    
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPressFunc);
    glutDisplayFunc(RenderScene);
    
    SetupRC();
    glutMainLoop();
    ReleaseRC();
    return 0;
}
