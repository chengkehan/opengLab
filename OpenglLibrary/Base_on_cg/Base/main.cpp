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

/////////////////////////////////////////////////////////////////////////////////
// An assortment of needed classes
GLShaderManager		shaderManager;
ShaderManager myShaderManager;
GLMatrixStack modelMatrix;
GLMatrixStack		modelViewMatrix;
GLMatrixStack		projectionMatrix;
GLFrame				cameraFrame;
GLFrustum			viewFrustum;
GLBatch				cubeBatch;
GLBatch				floorBatch;
GLBatch				topBlock;
GLBatch				frontBlock;
GLBatch				leftBlock;

GLGeometryTransform	transformPipeline;
M3DMatrix44f		shadowMatrix;


// Keep track of effects step
int nStep = 0;

// Lighting data
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat lightSpecular[] = { 0.9f, 0.9f, 0.9f };
GLfloat vLightPos[] = { -8.0f, 20.0f, 100.0f, 1.0f };

enum TEXTURE
{
    TEXTURE_FLOOR = 0,
    TEXTURE_BLOCK1,
    TEXTURE_BLOCK2,
    TEXTURE_BLOCK3,
    TEXTURE_AMOUNT
};
GLuint textures[TEXTURE_AMOUNT];


///////////////////////////////////////////////////////////////////////////////
// Make a cube out of a batch of triangles. Texture coordinates and normals
// are also provided.
void MakeCube(GLBatch& cubeBatch)
{
    cubeBatch.Begin(GL_TRIANGLES, 36, 1);
    
    /////////////////////////////////////////////
    // Top of cube
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    
    ////////////////////////////////////////////
    // Bottom of cube
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    ///////////////////////////////////////////
    // Left side of cube
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    // Right side of cube
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    // Front and Back
    // Front
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    // Back
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.End();
}

/////////////////////////////////////////////////////////////////////////////
// Make the floor, just the verts and texture coordinates, no normals
void MakeFloor(GLBatch& floorBatch)
{
    GLfloat x = 5.0f;
    GLfloat y = -1.0f;
    
    floorBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
    floorBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    floorBatch.Vertex3f(-x, y, x);
    
    floorBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    floorBatch.Vertex3f(x, y, x);
    
    floorBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    floorBatch.Vertex3f(x, y, -x);
    
    floorBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    floorBatch.Vertex3f(-x, y, -x);
    floorBatch.End();
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    GLbyte *pBytes;
    GLint nWidth, nHeight, nComponents;
    GLenum format;
    
    shaderManager.InitializeStockShaders();
    myShaderManager.init();
    
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2.5f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
    cameraFrame.MoveForward(-15.0f);
    cameraFrame.MoveUp(6.0f);
    cameraFrame.RotateLocalX(float(m3dDegToRad(20.0f)));
    
    MakeCube(cubeBatch);
    MakeFloor(floorBatch);
    
    // Make top
    topBlock.Begin(GL_TRIANGLE_FAN, 4, 1);
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 0.0f, 0.0f);
    topBlock.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 1.0f, 0.0f);
    topBlock.Vertex3f(1.0f, 1.0f, 1.0f);
    
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 1.0f, 1.0f);
    topBlock.Vertex3f(1.0f, 1.0f, -1.0f);
    
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 0.0f, 1.0f);
    topBlock.Vertex3f(-1.0f, 1.0f, -1.0f);
    topBlock.End();
    
    // Make Front
    frontBlock.Begin(GL_TRIANGLE_FAN, 4, 1);
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 0.0f, 0.0f);
    frontBlock.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 1.0f, 0.0f);
    frontBlock.Vertex3f(1.0f, -1.0f, 1.0f);
    
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 1.0f, 1.0f);
    frontBlock.Vertex3f(1.0f, 1.0f, 1.0f);
    
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 0.0f, 1.0f);
    frontBlock.Vertex3f(-1.0f, 1.0f, 1.0f);
    frontBlock.End();
    
    // Make left
    leftBlock.Begin(GL_TRIANGLE_FAN, 4, 1);
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 0.0f, 0.0f);
    leftBlock.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 1.0f, 0.0f);
    leftBlock.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 1.0f, 1.0f);
    leftBlock.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 0.0f, 1.0f);
    leftBlock.Vertex3f(-1.0f, 1.0f, -1.0f);
    leftBlock.End();
    
    // Create shadow projection matrix
    GLfloat floorPlane[] = { 0.0f, 1.0f, 0.0f, 1.0f};
    m3dMakePlanarShadowMatrix(shadowMatrix, floorPlane, vLightPos);
    
    // Load up four textures
    glGenTextures(TEXTURE_AMOUNT, textures);
    
    // Wood floor
    pBytes = gltReadTGABits("/Users/jimCheng/tutorial/OpenglBlueBook5/Src/Chapter01/Block/floor.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_FLOOR]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // One of the block faces
    pBytes = gltReadTGABits("/Users/jimCheng/tutorial/OpenglBlueBook5/Src/Chapter01/Block/Block4.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BLOCK1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Another block face
    pBytes = gltReadTGABits("/Users/jimCheng/tutorial/OpenglBlueBook5/Src/Chapter01/Block/Block5.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BLOCK2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Yet another block face
    pBytes = gltReadTGABits("/Users/jimCheng/tutorial/OpenglBlueBook5/Src/Chapter01/Block/Block6.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BLOCK3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}




///////////////////////////////////////////////////////////////////////////////
// Render the block
void RenderBlock(bool isMirror)
{
    GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat vWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    switch(nStep)
    {
            // Wire frame
        case 0:
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            myShaderManager.useShader(SHADER_TYPE_FLAT);
            myShaderManager.shader()->setFloat4x4("mvpMatrix", transformPipeline.GetModelViewProjectionMatrix());
            myShaderManager.shader()->setFloat4("color", vRed[0], vRed[1], vRed[2], vRed[3]);
            myShaderManager.shader()->enable();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            // Draw the cube
            cubeBatch.Draw();
            break;
            
            // Wire frame, but not the back side... we also want the block to be in the stencil buffer
        case 1:
            myShaderManager.useShader(SHADER_TYPE_FLAT);
            myShaderManager.shader()->setFloat4x4("mvpMatrix", transformPipeline.GetModelViewProjectionMatrix());
            myShaderManager.shader()->setFloat4("color", vRed[0], vRed[1], vRed[2], vRed[3]);
            myShaderManager.shader()->enable();
            
            // Draw solid block in stencil buffer
            // Back face culling prevents the back sides from showing through
            // The stencil pattern is used to mask when we draw the floor under it
            // to keep it from showing through.
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_NEVER, 0, 0);
            glStencilOp(GL_INCR, GL_INCR, GL_INCR);
            cubeBatch.Draw();
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glDisable(GL_STENCIL_TEST);
            
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
            // Draw the front side cube
            cubeBatch.Draw();
            break;
            
            // Solid
        case 2:
            myShaderManager.useShader(SHADER_TYPE_FLAT);
            myShaderManager.shader()->setFloat4x4("mvpMatrix", transformPipeline.GetModelViewProjectionMatrix());
            myShaderManager.shader()->setFloat4("color", vRed[0], vRed[1], vRed[2], vRed[3]);
            myShaderManager.shader()->enable();
            
            // Draw the cube
            cubeBatch.Draw();
            break;
            
            // Lit
        case 3:
            myShaderManager.useShader(SHADER_TYPE_DIFFUSE);
            myShaderManager.shader()->setFloat4x4("mvpMatrix", transformPipeline.GetModelViewProjectionMatrix());
            myShaderManager.shader()->setFloat4x4("mMatrix", modelMatrix.GetMatrix());
            myShaderManager.shader()->setFloat4("color", vRed[0], vRed[1], vRed[2], vRed[3]);
            myShaderManager.shader()->setFloat4("lightDir", 0, 1, 1, 0);
            myShaderManager.shader()->enable();
            
            // Draw the cube
            cubeBatch.Draw();
            break;
            
            // Textured & Lit
        case 4:
        case 5:
        default:
            if (isMirror)
            {
                myShaderManager.useShader(SHADER_TYPE_TEXTURE);
            }
            else
            {
                myShaderManager.useShader(SHADER_TYPE_DIFFUSE_TEXTURE);
            }
            myShaderManager.shader()->setFloat4x4("mvpMatrix", transformPipeline.GetModelViewProjectionMatrix());
            if (!isMirror)
            {
                myShaderManager.shader()->setFloat4x4("mMatrix", modelMatrix.GetMatrix());
                myShaderManager.shader()->setFloat4("lightDir", 0, 1, 1, 0);
            }
            myShaderManager.shader()->setFloat4("color", vWhite[0], vWhite[1], vWhite[2], vWhite[3]);
            
            myShaderManager.shader()->setTexture("mainTex", textures[TEXTURE_BLOCK1]);
            myShaderManager.shader()->enable();
            topBlock.Draw();
            myShaderManager.shader()->setTexture("mainTex", textures[TEXTURE_BLOCK2]);
            myShaderManager.shader()->enable();
            frontBlock.Draw();
            myShaderManager.shader()->setTexture("mainTex", textures[TEXTURE_BLOCK3]);
            myShaderManager.shader()->enable();
            leftBlock.Draw();
            
            break;
    }
    
    // Put everything back
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_STENCIL_TEST);
}

///////////////////////////////////////////////////////////////////////////////
// Render the floor
void RenderFloor(void)
{
    GLfloat vBrown [] = { 0.55f, 0.292f, 0.09f, 1.0f};
    GLfloat vFloor[] = { 1.0f, 1.0f, 1.0f, 0.8f };
    
    switch(nStep)
    {
            // Wire frame
        case 0:
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBrown);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            break;
            
            // Wire frame, but not the back side.. and only where stencil == 0
        case 1:
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, 0, 0xff);
            
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBrown);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
            
            // Solid
        case 2:
        case 3:
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBrown);
            break;
            
            // Textured
        case 4:
        case 5:
        default:
            glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_FLOOR]);
            shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vFloor, 0);
            break;
    }
    
    // Draw the floor
    floorBatch.Draw();
    
    // Put everything back
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_STENCIL_TEST);
}



///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    modelViewMatrix.PushMatrix();
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.MultMatrix(mCamera);
    
    // Reflection step... draw cube upside down, the floor
    // blended on top of it
    if(nStep == 5) {
        glDisable(GL_CULL_FACE);
        modelViewMatrix.PushMatrix();
        modelViewMatrix.Scale(1.0f, -1.0f, 1.0f);
        modelViewMatrix.Translate(0.0f, 2.0f, 0.0f);
        modelViewMatrix.Rotate(35.0f, 0.0f, 1.0f, 0.0f);
        RenderBlock(true);
        modelViewMatrix.PopMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        RenderFloor();
        glDisable(GL_BLEND);
    }
    
    
    modelViewMatrix.PushMatrix();
    modelMatrix.PushMatrix();
    
    // Draw normally
    modelViewMatrix.Rotate(35.0f, 0.0f, 1.0f, 0.0f);
    modelMatrix.Rotate(35.0f, 0.0f, 1.0f, 0.0f);
    RenderBlock(false);
    modelMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();
    
    
    // If not the reflection pass, draw floor last
    if(nStep != 5)
        RenderFloor();
    
    
    modelViewMatrix.PopMatrix();
    
    
    // Flush drawing commands
    glutSwapBuffers();
}


///////////////////////////////////////////////////////////////////////////////
// A normal ASCII key has been pressed.
// In this case, advance the scene when the space bar is pressed
void KeyPressFunc(unsigned char key, int x, int y)
{
    float cameraMoveSpeed = 0.3f;
    if(key == 32)
    {
        nStep++;
        printf("step:%d\n", nStep);
        if(nStep > 5)
            nStep = 0;
    }
    else if(key == 97)
    {
        cameraFrame.MoveRight(cameraMoveSpeed);
    }
    else if(key == 100)
    {
        cameraFrame.MoveRight(-cameraMoveSpeed);
    }
    else if(key == 115)
    {
        cameraFrame.MoveForward(-cameraMoveSpeed);
    }
    else if(key == 119)
    {
        cameraFrame.MoveForward(cameraMoveSpeed);
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
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    modelViewMatrix.LoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Effects Demo");
    
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
    glDeleteTextures(TEXTURE_AMOUNT,textures);
    return 0;
}
