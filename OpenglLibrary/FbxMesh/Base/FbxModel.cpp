//
//  FbxModel.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <assert.h>
#include "FbxModel.h"
#include "Common.h"
#include "Memory.h"

/* PUBLIC */

FbxModel::FbxModel()
{
    // Do nothing
}

FbxModel::~FbxModel()
{
    release();
}

bool FbxModel::readFbxFromFile(const char *fbxFilePath)
{
    release();
    
    FbxManager* fbxManager = nullptr;
    FbxScene* fbxScene = nullptr;
    bool fbxResult;
    InitializeSdkObjects(fbxManager, fbxScene);
    
    fbxResult = LoadScene(fbxManager, fbxScene, fbxFilePath);
    if (!fbxResult)
    {
        FBXSDK_printf("Load fbx fail:%s\n", fbxFilePath);
    }
    else
    {
        FbxGeometryConverter fbxGeometryConverter(fbxManager);
        fbxGeometryConverter.Triangulate(fbxScene, true);
        
        FbxNode* fbxNode = fbxScene->GetRootNode();
        if (fbxNode != nullptr)
        {
            for (int i = 0; i < fbxNode->GetChildCount(); ++i)
            {
                processFbxNode(fbxNode->GetChild(i));
            }
        }
    }
    
    DestroySdkObjects(fbxManager, fbxResult);

    return fbxResult;
}

unsigned int FbxModel::numSubMeshes()
{
    return subMeshes.length();
}

FbxModelSubMesh* FbxModel::getSubMesh(unsigned int index)
{
    if (index >= subMeshes.length())
    {
        return nullptr;
    }
    return subMeshes[index];
}

/* PRIVATE */

void FbxModel::processFbxNode(FbxNode *fbxNode)
{
    if (fbxNode == nullptr)
    {
        return;
    }
    
    if (fbxNode->GetNodeAttribute() == nullptr)
    {
        FBXSDK_printf("Null node attribute\n");
    }
    else
    {
        FbxNodeAttribute::EType fbxNodeAttributeType = fbxNode->GetNodeAttribute()->GetAttributeType();
        switch (fbxNodeAttributeType)
        {
            case FbxNodeAttribute::eMesh :
            {
                processFbxMesh(fbxNode);
                break;
            }
            default:
                break;
        }
    }
    
    for (int i = 0; i < fbxNode->GetChildCount(); ++i)
    {
        processFbxNode(fbxNode->GetChild(i));
    }
}

void FbxModel::processFbxMesh(FbxNode *fbxNode)
{
    FbxMesh* fbxMesh = dynamic_cast<FbxMesh*>(fbxNode->GetNodeAttribute());
    
    // Check the max number of vertices
    int numVertices = fbxMesh->GetControlPointsCount();
    if (numVertices > 65535)
    {
        printf("There are more than 65535 vertices in a subMesh. The mesh is ignored.");
        return;
    }
    FbxModelSubMesh* subMesh = Memory_NewHeapObject(FbxModelSubMesh);
    subMeshes.add(subMesh);
    
    // Position
    FbxVector4* vertices = fbxMesh->GetControlPoints();
    subMesh->vertices = Memory_NewHeapObject(BetterList<Vector3>, numVertices, true);
    subMesh->uv = Memory_NewHeapObject(BetterList<Vector2>, numVertices, true);
    subMesh->normals = Memory_NewHeapObject(BetterList<Vector3>, numVertices, true);
    for (int i = 0; i < numVertices; ++i)
    {
        FbxVector4* fbxVertex = &vertices[i];
        Vector3& vertex = (*subMesh->vertices)[i];
        vertex.x = fbxVertex->mData[0];
        vertex.y = fbxVertex->mData[1];
        vertex.z = fbxVertex->mData[2];
    }
    
    // If uv or normal is not exists, delete the corresponding data-list.
    bool uvExists = false;
    bool normalExists = false;
    
    // Attributes
    int numPolygons = fbxMesh->GetPolygonCount();
    subMesh->indices = Memory_NewHeapObject(BetterList<unsigned short>);
    for (int i = 0; i < numPolygons; ++i)
    {
        int polygonSize = fbxMesh->GetPolygonSize(i);
        // The polygon must be a triangle.
        assert(polygonSize == 3);
        for (int j = 0; j < polygonSize; ++j)
        {
            int vertexId = i * 3/*triangle*/ + j;
            
            // Indices
            int vertexIndex = fbxMesh->GetPolygonVertex(i, j);
            subMesh->indices->add(vertexIndex);
            
            // Normals
            for(int k = 0; k < fbxMesh->GetElementNormalCount(); ++k)
            {
                FbxGeometryElementNormal* normal = fbxMesh->GetElementNormal(k);
                if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                {
                    switch (normal->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            FbxVector4 fbxNormal = normal->GetDirectArray().GetAt(vertexId);
                            Vector3& normal = (*subMesh->normals)[vertexIndex];
                            normal.x = fbxNormal.mData[0];
                            normal.y = fbxNormal.mData[1];
                            normal.z = fbxNormal.mData[2];
                            normalExists = true;
                            break;
                        }
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id = normal->GetIndexArray().GetAt(vertexId);
                            FbxVector4 fbxNormal = normal->GetDirectArray().GetAt(id);
                            Vector3& normal = (*subMesh->normals)[vertexIndex];
                            normal.x = fbxNormal.mData[0];
                            normal.y = fbxNormal.mData[1];
                            normal.z = fbxNormal.mData[2];
                            normalExists = true;
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            
            // UV
            for (int k = 0; k < fbxMesh->GetElementUVCount(); ++k)
            {
                FbxGeometryElementUV* uv = fbxMesh->GetElementUV(k);
                switch (uv->GetMappingMode())
                {
                    case FbxGeometryElement::eByControlPoint:
                    {
                        switch (uv->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            {
                                FbxVector2 fbxUV = uv->GetDirectArray().GetAt(vertexIndex);
                                Vector2& uv = (*subMesh->uv)[vertexIndex];
                                uv.x = fbxUV.mData[0];
                                uv.y = fbxUV.mData[1];
                                uvExists = true;
                                break;
                            }
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = uv->GetIndexArray().GetAt(vertexIndex);
                                FbxVector2 fbxUV = uv->GetDirectArray().GetAt(id);
                                Vector2& uv = (*subMesh->uv)[vertexIndex];
                                uv.x = fbxUV.mData[0];
                                uv.y = fbxUV.mData[1];
                                uvExists = true;
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    }
                    case FbxGeometryElement::eByPolygonVertex:
                    {
                        int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
                        switch (uv->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                FbxVector2 fbxUV = uv->GetDirectArray().GetAt(lTextureUVIndex);
                                Vector2& uv = (*subMesh->uv)[vertexIndex];
                                uv.x = fbxUV.mData[0];
                                uv.y = fbxUV.mData[1];
                                uvExists = true;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                        break;
                        
                    case FbxGeometryElement::eByPolygon:
                    case FbxGeometryElement::eAllSame:
                    case FbxGeometryElement::eNone:
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    // If uv or normal is not exists, delete the corresponding data-list.
    if (!uvExists)
    {
        Memory_DeleteHeapObject(subMesh->uv);
        subMesh->uv = nullptr;
    }
    if (!normalExists)
    {
        Memory_DeleteHeapObject(subMesh->normals);
        subMesh->normals = nullptr;
    }
}

void FbxModel::release()
{
    releaseSubMeshes();
}

void FbxModel::releaseSubMeshes()
{
    unsigned int numSubMeshes = subMeshes.length();
    for (int i = 0; i < numSubMeshes; ++i)
    {
        FbxModelSubMesh* subMesh = subMeshes[i];
        if (subMesh->vertices != nullptr)
        {
            Memory_DeleteHeapObject(subMesh->vertices);
            subMesh->vertices = nullptr;
        }
        if (subMesh->uv != nullptr)
        {
            Memory_DeleteHeapObject(subMesh->uv);
            subMesh->uv = nullptr;
        }
        if (subMesh->normals != nullptr)
        {
            Memory_DeleteHeapObject(subMesh->normals);
            subMesh->normals = nullptr;
        }
        if (subMesh->indices != nullptr)
        {
            Memory_DeleteHeapObject(subMesh->indices);
            subMesh->indices = nullptr;
        }
        Memory_DeleteHeapObject(subMeshes[i]);
    }
    subMeshes.release();
}
