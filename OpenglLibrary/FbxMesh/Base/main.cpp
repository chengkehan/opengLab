// Block.cpp
// OpenGL SuperBible, Chapter 1
// Demonstrates an assortment of basic 3D concepts
// Program by Richard S. Wright Jr.

#include <GLTools.h>	// OpenGL toolkit
#include "glHeader.h"
#include "ShaderManager.h"
#include "Vector4.h"
#include "AttributeBindingLocation.h"
#include "Mesh.h"
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLGeometryTransform.h>

const char* projectName = "FbxMesh";
ShaderManager shaderManager;

GLuint texture;
Mesh mesh;

GLGeometryTransform mvpTransform;
GLMatrixStack mvTransform;
GLMatrixStack pTransform;
GLFrustum viewFrustum;
GLFrame cameraFrame;
GLFrame modelTransform;

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
    
    {
        cameraFrame.MoveForward(-10.0f);
        cameraFrame.MoveUp(6.0f);
        cameraFrame.RotateLocalX(float(m3dDegToRad(20.0f)));
        mvpTransform.SetMatrixStacks(mvTransform, pTransform);
        
        int numVertices = 8;
        
        // Vertices
        Vector3* vertices = static_cast<Vector3*>(malloc(sizeof(Vector3) * numVertices));
        vertices[0] = Vector3(1, 1, 1);
        vertices[1] = Vector3(1, -1, 1);
        vertices[2] = Vector3(-1, -1, 1);
        vertices[3] = Vector3(-1, 1, 1);
        vertices[4] = Vector3(1, 1, -1);
        vertices[5] = Vector3(1, -1, -1);
        vertices[6] = Vector3(-1, -1, -1);
        vertices[7] = Vector3(-1, 1, -1);
        
        //UV
        Vector2* uv = static_cast<Vector2*>(malloc(sizeof(Vector2) * numVertices));
        uv[0] = Vector2(1, 1);
        uv[1] = Vector2(1, 0);
        uv[2] = Vector2(0, 0);
        uv[3] = Vector2(0, 1);
        uv[4] = Vector2(1, 1);
        uv[5] = Vector2(1, 0);
        uv[6] = Vector2(0, 0);
        uv[7] = Vector2(0, 1);
        
        // Indices
        GLushort* indices = static_cast<GLushort*>(malloc(sizeof(GLushort) * 7 * 3));
        
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        
        indices[3] = 0;
        indices[4] = 2;
        indices[5] = 3;
        
        indices[6] = 4;
        indices[7] = 6;
        indices[8] = 5;
        
        indices[9] = 4;
        indices[10] = 7;
        indices[11] = 6;
        
        indices[12] = 4;
        indices[13] = 0;
        indices[14] = 3;
        
        indices[15] = 4;
        indices[16] = 3;
        indices[17] = 7;
        
        indices[18] = 4;
        indices[19] = 1;
        indices[20] = 0;
        
        mesh.setVertices(vertices, numVertices);
        mesh.setUV(uv, numVertices);
        mesh.setIndices(indices, 7 * 3);
        mesh.upload();
    }
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
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
    glutPostRedisplay();
}

void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject()
{
    modelTransform.RotateLocalY(0.03f);
    M3DMatrix44f mModel;
    modelTransform.GetMatrix(mModel);
    
    mvTransform.PushMatrix();
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    mvTransform.MultMatrix(mCamera);
    mvTransform.MultMatrix(mModel);
    
    shaderManager.shader(SHADER_TYPE_MVP_TEXTURE)->setTexture("texUnit", texture, 0);
    shaderManager.shader(SHADER_TYPE_MVP_TEXTURE)->setFloat4x4("mvp", mvpTransform.GetModelViewProjectionMatrix());
    shaderManager.shader(SHADER_TYPE_MVP_TEXTURE)->enable();
    
    mesh.draw();
    
    mvTransform.PopMatrix();
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
    else if(key == 32/*space*/)
    {
        static int polygonMode = 0;
        if (polygonMode == 0)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            polygonMode = -1;
        }
        ++polygonMode;
    }
    // Refresh the Window
//    glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(60.0f, float(w) / float(h), 1.0f, 500.0f);
    pTransform.LoadMatrix(viewFrustum.GetProjectionMatrix());
    mvTransform.LoadIdentity();
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
