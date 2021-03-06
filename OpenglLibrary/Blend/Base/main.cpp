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

ShaderManager shaderManager;
const char* projectName = "Blend";

GLBatch triangleBatch;
GLBatch triangleBatch2;

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    shaderManager.init(projectName);
    
    {
        GLfloat vertices[] = { -0.5f, 0.0f, 0.0f,
                                0.5f, 0.0f, 0.0f,
                                0.0f, 0.5f, 0.0f };
        triangleBatch.Begin(GL_TRIANGLES, 3);
        triangleBatch.CopyVertexData3f(vertices);
        triangleBatch.End();
    }
    
    {
        float offset = 0.2f;
        GLfloat vertices[] = { -0.5f + offset, 0.0f + offset, 0.0f,
                                0.5f + offset, 0.0f + offset, 0.0f,
                                0.0f + offset, 0.5f + offset, 0.0f };
        triangleBatch2.Begin(GL_TRIANGLES, 3);
        triangleBatch2.CopyVertexData3f(vertices);
        triangleBatch2.End();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    shaderManager.useShader(SHADER_TYPE_IDENTITY);
    shaderManager.shader()->setFloat4("color", 1.0f, 0.0f, 0.0f, 0.5f);
    shaderManager.shader()->enable();
    triangleBatch.Draw();
    
    shaderManager.useShader(SHADER_TYPE_IDENTITY);
    shaderManager.shader()->setFloat4("color", 0.0f, 0.0f, 1.0f, 0.5f);
    shaderManager.shader()->enable();
    triangleBatch2.Draw();
    
    glDisable(GL_BLEND);
    
    // Flush drawing commands
    glutSwapBuffers();
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
