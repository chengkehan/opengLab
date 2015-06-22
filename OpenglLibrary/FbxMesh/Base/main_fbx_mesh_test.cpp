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
#include "FbxModel.h"
#include <assert.h>
#include "FbxAnimation.h"
#include "MemoryLeakDetector.h"

MemoryLeakDetector memoryLeakDetector;

const char* projectName = "FbxMesh";
ShaderManager shaderManager;

GLuint texture;
Mesh mesh(MESH_RENDER_TYPE_TRIANGLES);

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    shaderManager.init(projectName);
    
    {
        GLint width;
        GLint height;
        GLint components;
        GLenum format;
        GLbyte* bytes = gltReadTGABits("/Users/jimCheng/resources/snail/DSGSY/DSG/Assets/Object/Act/NPC/34300011/34300011.tga", &width, &height, &components, &format);
        
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
        cameraFrame.MoveForward(-250.0f);
        cameraFrame.MoveUp(250.0f);
        cameraFrame.RotateLocalX(float(m3dDegToRad(20.0f)));
        mvpTransform.SetMatrixStacks(mvTransform, pTransform);
        
        FbxModel fbxModel;
        bool fbxModelLoaded = fbxModel.readFbxFromFile("/Users/jimCheng/resources/snail/DSGSY/DSG/Assets/Object/Act/NPC/34300011/34300011@TPOSE.FBX");
        
        printf("fbxModelLoaded:%s\n", fbxModelLoaded ? "yes" : "no");
        
        printf("numSubMeshes:%u\n", fbxModel.numSubMeshes());
        assert(fbxModel.numSubMeshes() == 1);
        mesh.setVertices(fbxModel.getSubMesh(0)->vertices->rawBuffer(), fbxModel.getSubMesh(0)->vertices->length());
        mesh.setIndices(fbxModel.getSubMesh(0)->indices->rawBuffer(), fbxModel.getSubMesh(0)->indices->length());
        mesh.setUV(fbxModel.getSubMesh(0)->uv->rawBuffer(), fbxModel.getSubMesh(0)->uv->length());
        mesh.setNormals(fbxModel.getSubMesh(0)->normals->rawBuffer(), fbxModel.getSubMesh(0)->normals->length());
        mesh.upload();
    }
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();
    
    // Flush drawing commands
    glutSwapBuffers();
    glutPostRedisplay();
}

void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject()
{
    M3DVector3f up = {0, 1, 0};
    M3DVector3f forward = {0, 0, -1};
    modelTransform.SetForwardVector(forward);
    modelTransform.SetUpVector(up);
    static float rotation = 0.0f;
    modelTransform.RotateLocalX(-1.57f);
    modelTransform.RotateLocalZ(rotation);
    rotation += 0.02f;
    M3DMatrix44f mModel;
    modelTransform.GetMatrix(mModel);
    
    mvTransform.PushMatrix();
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    mvTransform.MultMatrix(mCamera);
    mvTransform.MultMatrix(mModel);
    
    shaderManager.shader(SHADER_TYPE_MVP_TEXTURE_DIFFUSE)->setTexture("texUnit", texture, 0);
    shaderManager.shader(SHADER_TYPE_MVP_TEXTURE_DIFFUSE)->setFloat4x4("mvp", mvpTransform.GetModelViewProjectionMatrix());
    shaderManager.shader(SHADER_TYPE_MVP_TEXTURE_DIFFUSE)->setFloat4x4("mv", mvTransform.GetMatrix());
    shaderManager.shader(SHADER_TYPE_MVP_TEXTURE_DIFFUSE)->enable();
    
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
