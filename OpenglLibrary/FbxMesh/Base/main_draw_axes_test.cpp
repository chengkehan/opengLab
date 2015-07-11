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
//#include "FbxAnimation.h"
#include "FbxSkeletons.h"
#include "MemoryLeakDetector.h"

MemoryLeakDetector memoryLeakDetector(false, false, false);

const char* projectName = "FbxMesh";
ShaderManager shaderManager;

Mesh mesh(MESH_RENDER_TYPE_LINES);

GLGeometryTransform mvpTransform;
GLMatrixStack mvTransform;
GLMatrixStack pTransform;
GLFrustum viewFrustum;
GLFrame cameraFrame;
GLFrame modelTransform;
GLFrame testFrame;

void drawTriangleWithVertexArray_ArrayOfCompactVertexBufferObject();

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    // Test Matrix
//    M3DMatrix44f testMatrix;
//    testFrame.SetForwardVector(0, 0, 1);
//    testFrame.TranslateLocal(1, 2, 3);
//    testFrame.RotateLocalX(45.0 * 3.1415926 / 180.0);
//    testFrame.GetMatrix(testMatrix);
//    printf("testMatrix------------------------------------------\n");
//    printf("%f  %f  %f  %f\n", testMatrix[0], testMatrix[4], testMatrix[8], testMatrix[12]);
//    printf("%f  %f  %f  %f\n", testMatrix[1], testMatrix[5], testMatrix[9], testMatrix[13]);
//    printf("%f  %f  %f  %f\n", testMatrix[2], testMatrix[6], testMatrix[10], testMatrix[14]);
//    printf("%f  %f  %f  %f\n", testMatrix[3], testMatrix[7], testMatrix[11], testMatrix[15]);
//    
//    FbxAMatrix fbxMat;
//    fbxMat.SetT(FbxVector4(1, 2, 3));
//    fbxMat.SetR(FbxVector4(45.0, 0, 0));
//    printf("fbxMat---------------------------------------------\n");
//    printf("%f  %f  %f  %f\n", fbxMat[0][0], fbxMat[1][0], fbxMat[2][0], fbxMat[3][0]);
//    printf("%f  %f  %f  %f\n", fbxMat[0][1], fbxMat[1][1], fbxMat[2][1], fbxMat[3][1]);
//    printf("%f  %f  %f  %f\n", fbxMat[0][2], fbxMat[1][2], fbxMat[2][2], fbxMat[3][2]);
//    printf("%f  %f  %f  %f\n", fbxMat[0][3], fbxMat[1][3], fbxMat[2][3], fbxMat[3][3]);

    
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    shaderManager.init(projectName);
    
    {
        //FbxAnimation fbxAnimation;
        //fbxAnimation.loadFbxFromFile("/Users/jimCheng/resources/snail/DSGSY/DSG/Assets/Object/Act/NPC/34300011/34300011@idle.FBX");
        
        FbxSkeletons fbxSkeletons;
//        fbxSkeletons.parseFromFile("/Users/jimCheng/resources/snail/DSGSY/DSG/Assets/Object/Act/NPC/34300011/34300011@idle.FBX");
        fbxSkeletons.parseFromFile("/Users/jimCheng/resources/snail/DSGSY/DSG/Assets/Object/Act/NPC/34300011/34300011@TPOSE.FBX");
//        fbxSkeletons.printTreeStruct();
//        printf("root bone name:%s\n", fbxSkeletons.getRootBone()->getName());
    }
    
    {
        cameraFrame.MoveForward(-250.0f);
        cameraFrame.MoveUp(200.0f);
        cameraFrame.RotateLocalX(float(m3dDegToRad(20.0f)));
        mvpTransform.SetMatrixStacks(mvTransform, pTransform);
        
        Vector3 vertices[] = {
            {0.0f, 0.0f, 0.0f}, {100.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f}, {0.0f, 100.0f, 0.0f},
            {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 100.0f}
        };
        mesh.setVertices(vertices, 6);
        
        Vector3 colors[] = {
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}
        };
        mesh.setColors(colors, 6);
        
        unsigned short indices[] = {0, 1, 2, 3, 4, 5};
        mesh.setIndices(indices, 6);
        
        mesh.setLineWidth(2.0f);
        mesh.upload();
    }
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void ReleaseRC()
{
    
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
    modelTransform.RotateLocalY(rotation);
    rotation += 0.005f;
    M3DMatrix44f mModel;
    modelTransform.GetMatrix(mModel);
    
    mvTransform.PushMatrix();
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    mvTransform.MultMatrix(mCamera);
    mvTransform.MultMatrix(mModel);
    
    shaderManager.shader(SHADER_TYPE_MVP_COLOR)->setFloat4x4("mvp", mvpTransform.GetModelViewProjectionMatrix());
    shaderManager.shader(SHADER_TYPE_MVP_COLOR)->enable();
    
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
