// Block.cpp
// OpenGL SuperBible, Chapter 1
// Demonstrates an assortment of basic 3D concepts
// Program by Richard S. Wright Jr.

#include <GLTools.h>	// OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLGeometryTransform.h>

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include "ShaderManager.h"
#include "Vector3.h"
#include "Vector4.h"

ShaderManager shaderManager;
const char* projectName = "SimpleShape";

GLBatch shape0;

void drawTriangleWithConstantVertexAttribute();
void drawTriangleWithVertexArray_StructOfArray();
void drawTriangleWithVertexArray_ArrayOfCompactStruct();
void drawTriangleWithVertexArray_ArrayOfInterleavedStruct();
void drawTriangleWithVertexArray_VertexBufferObjectOfArray();
void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();
void drawTriangleWithVertexArray_ArrayOfInterleavedVertexBufferObject();

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    shaderManager.init(projectName);
}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
//    drawTriangleWithConstantVertexAttribute();
//    drawTriangleWithVertexArray_StructOfArray();
//    drawTriangleWithVertexArray_ArrayOfCompactStruct();
//    drawTriangleWithVertexArray_ArrayOfInterleavedStruct();
//    drawTriangleWithVertexArray_VertexBufferObjectOfArray();
    drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();
//    drawTriangleWithVertexArray_ArrayOfInterleavedVertexBufferObject();
    
    // Flush drawing commands
    glutSwapBuffers();
}

void drawTriangleWithConstantVertexAttribute()
{
    shaderManager.useShader(SHADER_TYPE_IDENTITY_COLOR);
    shaderManager.shader()->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader()->enable();
    
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertexAttrib3f(0, 0, 0, 0);
    glColor3f(0, 1, 0);
    glVertexAttrib3f(0, 1, 1, 0);
    glColor3f(0, 0, 1);
    glVertexAttrib3f(0, 1, 0, 0);
    glEnd();
}

Vector3* vertices = nullptr;
Vector4* colors = nullptr;
void drawTriangleWithVertexArray_StructOfArray()
{
    shaderManager.useShader(SHADER_TYPE_IDENTITY_COLOR);
    shaderManager.shader()->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader()->enable();
    
    int POSITION_INDEX = 0;
    int COLOR_INDEX = 3;
    int numVertices = 3;
    
    // Vertices
    if(vertices == nullptr)
    {
        vertices = static_cast<Vector3*>(malloc(sizeof(Vector3) * numVertices));
        vertices[0] = Vector3(0, 0, 0);
        vertices[1] = Vector3(1, 1, 0);
        vertices[2] = Vector3(1, 0, 0);
        glVertexAttribPointer(POSITION_INDEX, 3/*xyz*/, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(POSITION_INDEX);
    }
    
    // Colors
    if (colors == nullptr)
    {
        colors = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices));
        colors[0] = Vector4(1, 0, 0, 1);
        colors[1] = Vector4(0, 1, 0, 1);
        colors[2] = Vector4(0, 0, 1, 1);
        glVertexAttribPointer(COLOR_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, colors);
        glEnableVertexAttribArray(COLOR_INDEX);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // Free memory must be after draw command
//    free(vertices);
//    vertices = nullptr;
//    free(colors);
//    colors = nullptr;
}

Vector4* compactAttributes = nullptr;
void drawTriangleWithVertexArray_ArrayOfCompactStruct()
{
    shaderManager.useShader(SHADER_TYPE_IDENTITY_COLOR);
    shaderManager.shader()->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader()->enable();
    
    int POSITION_INDEX = 0;
    int COLOR_INDEX = 3;
    int numVertices = 3;
    
    if (compactAttributes == nullptr)
    {
        compactAttributes = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices * 2));
        // Position
        compactAttributes[0] = Vector4(0, 0, 0, 1);
        compactAttributes[1] = Vector4(1, 1, 0, 1);
        compactAttributes[2] = Vector4(1, 0, 0, 1);
        // Color
        compactAttributes[3] = Vector4(1, 0, 0, 1);
        compactAttributes[4] = Vector4(0, 1, 0, 1);
        compactAttributes[5] = Vector4(0, 0, 1, 1);
        //
        glVertexAttribPointer(POSITION_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, compactAttributes);
        glEnableVertexAttribArray(POSITION_INDEX);
        glVertexAttribPointer(COLOR_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, compactAttributes + numVertices);
        glEnableVertexAttribArray(COLOR_INDEX);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Vector4* interleavedAttributes = nullptr;
void drawTriangleWithVertexArray_ArrayOfInterleavedStruct()
{
    shaderManager.useShader(SHADER_TYPE_IDENTITY_COLOR);
    shaderManager.shader()->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader()->enable();
    
    int POSITION_INDEX = 0;
    int COLOR_INDEX = 3;
    int numVertices = 3;
    
    if (interleavedAttributes == nullptr)
    {
        interleavedAttributes = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices * 2));
        // Position
        interleavedAttributes[0] = Vector4(0, 0, 0, 1);
        interleavedAttributes[2] = Vector4(1, 1, 0, 1);
        interleavedAttributes[4] = Vector4(1, 0, 0, 1);
        // Color
        interleavedAttributes[1] = Vector4(1, 0, 0, 1);
        interleavedAttributes[3] = Vector4(0, 1, 0, 1);
        interleavedAttributes[5] = Vector4(0, 0, 1, 1);
        //
        glVertexAttribPointer(POSITION_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, sizeof(Vector4) * 2, interleavedAttributes);
        glEnableVertexAttribArray(POSITION_INDEX);
        glVertexAttribPointer(COLOR_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, sizeof(Vector4) * 2, interleavedAttributes + 1);
        glEnableVertexAttribArray(COLOR_INDEX);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint positionVBO = 0;
GLuint colorVBO = 0;
GLuint indexVBO = 0;
void drawTriangleWithVertexArray_VertexBufferObjectOfArray()
{
    shaderManager.useShader(SHADER_TYPE_IDENTITY_COLOR);
    shaderManager.shader()->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader()->enable();
    
    if (positionVBO == 0)
    {
        int POSITION_INDEX = 0;
        int COLOR_INDEX = 3;
        int numVertices = 3;
        
        // Position
        Vector4* positions = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices));
        positions[0] = Vector4(0, 0, 0, 1);
        positions[1] = Vector4(1, 1, 0, 1);
        positions[2] = Vector4(1, 0, 0, 1);
        glGenBuffers(1, &positionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * numVertices, positions, GL_STATIC_DRAW);
        free(positions);
        glVertexAttribPointer(POSITION_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
        glEnableVertexAttribArray(POSITION_INDEX);
        
        // Color
        Vector4* colors = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices));
        colors[0] = Vector4(1, 0, 0, 1);
        colors[1] = Vector4(0, 1, 0, 1);
        colors[2] = Vector4(0, 0, 1, 1);
        glGenBuffers(1, &colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * numVertices, colors, GL_STATIC_DRAW);
        free(colors);
        glVertexAttribPointer(COLOR_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
        glEnableVertexAttribArray(COLOR_INDEX);
        
        // Indices
        GLushort* indices = static_cast<GLushort*>(malloc(sizeof(GLushort) * 3));
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        
        glGenBuffers(1, &indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 3, indices, GL_STATIC_DRAW);
        free(indices);
    }
    
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
}

GLuint compactAttributeVBO = 0;
GLuint compactIndexVBO = 0;
void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject()
{
    shaderManager.useShader(SHADER_TYPE_IDENTITY_COLOR);
    shaderManager.shader()->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader()->enable();
    
    if (compactAttributeVBO == 0)
    {
        int POSITION_INDEX = 0;
        int COLOR_INDEX = 3;
        int numVertices = 3;
        
        // Attributes
        Vector4* attributes = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices * 2));
        // Position
        attributes[0] = Vector4(0, 0, 0, 1);
        attributes[1] = Vector4(1, 1, 0, 1);
        attributes[2] = Vector4(1, 0, 0, 1);
        // Color
        attributes[3] = Vector4(1, 0, 0, 1);
        attributes[4] = Vector4(0, 1, 0, 1);
        attributes[5] = Vector4(0, 0, 1, 1);
        
        glGenBuffers(1, &compactAttributeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, compactAttributeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * numVertices * 2, attributes, GL_STATIC_DRAW);
        free(attributes);
        
        glVertexAttribPointer(POSITION_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
        glEnableVertexAttribArray(POSITION_INDEX);
        glVertexAttribPointer(COLOR_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(sizeof(Vector4) * numVertices));
        glEnableVertexAttribArray(COLOR_INDEX);
        
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

GLuint interleavedAttributeVBO = 0;
GLuint interleavedIndexVBO = 0;
void drawTriangleWithVertexArray_ArrayOfInterleavedVertexBufferObject()
{
    shaderManager.useShader(SHADER_TYPE_IDENTITY_COLOR);
    shaderManager.shader()->setFloat4("color", 1, 1, 1, 1);
    shaderManager.shader()->enable();
    
    if (interleavedAttributeVBO == 0)
    {
        int numVertices = 3;
        int POSITION_INDEX = 0;
        int COLOR_INDEX = 3;
        
        Vector4* attributes = static_cast<Vector4*>(malloc(sizeof(Vector4) * numVertices * 2));
        // Position
        attributes[0] = Vector4(0, 0, 0, 1);
        attributes[2] = Vector4(1, 1, 0, 1);
        attributes[4] = Vector4(1, 0, 0, 1);
        // Color
        attributes[1] = Vector4(1, 0, 0, 1);
        attributes[3] = Vector4(0, 1, 0, 1);
        attributes[5] = Vector4(0, 0, 1, 1);
        
        glGenBuffers(1, &interleavedAttributeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, interleavedAttributeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * numVertices * 2, attributes, GL_STATIC_DRAW);
        free(attributes);
        
        glVertexAttribPointer(POSITION_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, sizeof(Vector4) * 2, reinterpret_cast<const GLvoid*>(0));
        glEnableVertexAttribArray(POSITION_INDEX);
        glVertexAttribPointer(COLOR_INDEX, 4/*xyzw*/, GL_FLOAT, GL_FALSE, sizeof(Vector4) * 2, reinterpret_cast<const GLvoid*>(sizeof(Vector4)));
        glEnableVertexAttribArray(COLOR_INDEX);
        
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
// Release all resources
void ReleaseResources()
{
    
}

///////////////////////////////////////////////////////////////////////////////
// A normal ASCII key has been pressed.
// In this case, advance the scene when the space bar is pressed
void KeyPressFunc(unsigned char key, int x, int y)
{
    if(key == 32/* space */)
    {
        
    }
    else if(key == 27/* esc */)
    {
        ReleaseResources();
        exit(1);
    }
    else if(key == 97/* a */)
    {
        
    }
    else if(key == 100/* d */)
    {
        
    }
    else if(key == 115/* s */)
    {
        
    }
    else if(key == 119/* w */)
    {
        
    }
    else
    {
        printf("key press %d\n", key);
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
    return 0;
}
