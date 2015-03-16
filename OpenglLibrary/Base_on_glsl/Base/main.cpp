// Block.cpp
// OpenGL SuperBible, Chapter 1
// Demonstrates an assortment of basic 3D concepts
// Program by Richard S. Wright Jr.

#include <GLTools.h>	// OpenGL toolkit
#include "glHeader.h"
#include "ShaderManager.h"
#include "Vector4.h"
#include "AttributeBindingLocation.h"

const char* projectName = "Base_on_glsl";
ShaderManager shaderManager;

GLuint texture;

void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    
    shaderManager.init(projectName);
    
    {
        GLint width;
        GLint height;
        GLint components;
        GLenum format;
        GLbyte* bytes = gltReadTGABits("/Users/jimCheng/projects/OpenglLibrary/Base_on_glsl/Base/Block6.tga", &width, &height, &components, &format);
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
        glBindTexture(GL_TEXTURE_2D, 0);
        free(bytes);
    }
}

void ReleaseRC()
{
    glDeleteTextures(1, &texture);
}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();
    
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
    
    if (compactAttributeVBO == 0)
    {
        int numVertices = 3;
        
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
        
        glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_POSITION, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
        glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_POSITION);
        glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_COLOR, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(sizeof(Vector4) * numVertices));
        glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_COLOR);
        glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_TEXTURE0, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(sizeof(Vector4) * numVertices * 2));
        glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_TEXTURE0);
        
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
    
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
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
