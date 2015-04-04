//
//  main.cpp
//  ReadFBX
//
//  Created by jimCheng on 15/4/2.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <iostream>
#include "fbxsdk.h"
#include "Common.h"

void processFbxNode(FbxNode* fbxNode);
void processFbxMesh(FbxNode* fbxNode);
void processVertexPosition(FbxMesh* fbxMesh);
void processPolygon(FbxMesh* fbxMesh);

int main(int argc, const char * argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    
    FbxManager* fbxManager = nullptr;
    FbxScene* fbxScene = nullptr;
    bool fbxResult;
    InitializeSdkObjects(fbxManager, fbxScene);
    
    const char* fbxFilePath = "/Users/jimCheng/resources/snail/DSGSY/DSG/Assets/Object/Act/Hero/30000004/30000004_40@Tpose.FBX";
    fbxResult = LoadScene(fbxManager, fbxScene, fbxFilePath);
    if (!fbxResult)
    {
        FBXSDK_printf("Load fbx fail:%s\n", fbxFilePath);
    }
    else
    {
        FBXSDK_printf("Load fbx success.\n");
        
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
    
    return 0;
}

void processFbxNode(FbxNode* fbxNode)
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

void processFbxMesh(FbxNode* fbxNode)
{
    FbxMesh* fbxMesh = dynamic_cast<FbxMesh*>(fbxNode->GetNodeAttribute());
    FBXSDK_printf("Mesh name:%s\n", fbxNode->GetName());
    processVertexPosition(fbxMesh);
    processPolygon(fbxMesh);
}

void processVertexPosition(FbxMesh* fbxMesh)
{
    int numVertices = fbxMesh->GetControlPointsCount();
    FbxVector4* vertices = fbxMesh->GetControlPoints();
    FBXSDK_printf("    Vertex Position:(%d)\n", numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        FbxVector4* vertex = &vertices[i];
        FBXSDK_printf("        %d: %f, %f, %f\n", i, vertex->mData[0], vertex->mData[1], vertex->mData[2]);
    }
}

void processPolygon(FbxMesh* fbxMesh)
{
    int numPolygons = fbxMesh->GetPolygonCount();
    FbxVector4* vertices = fbxMesh->GetControlPoints();
    FBXSDK_printf("    Polygon:(%d)\n", numPolygons);
    int vertexId = 0;
    for (int i = 0; i < numPolygons; ++i)
    {
        int polygonSize = fbxMesh->GetPolygonSize(i);
        FBXSDK_printf("        %d:(%d)\n", i, polygonSize);
        for (int j = 0; j < polygonSize; ++j)
        {
            int vertexIndex = fbxMesh->GetPolygonVertex(i, j);
            FBXSDK_printf("            %d:\n", j);
            FbxVector4* vertex = &vertices[vertexIndex];
            FBXSDK_printf("                pos:%f, %f, %f\n", vertex->mData[0], vertex->mData[1], vertex->mData[2]);
            
            for(int k = 0; k < fbxMesh->GetElementNormalCount(); ++k)
            {
                FbxGeometryElementNormal* normal = fbxMesh->GetElementNormal(k);
                FBXSDK_printf("                normal:");
                if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                {
                    switch (normal->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            FbxVector4 vertex = normal->GetDirectArray().GetAt(vertexId);
                            FBXSDK_printf("%f, %f, %f\n", vertex.mData[0], vertex.mData[1], vertex.mData[2]);
                            break;
                        }
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id = normal->GetIndexArray().GetAt(vertexId);
                            FbxVector4 vertex = normal->GetDirectArray().GetAt(id);
                            FBXSDK_printf("%f, %f, %f\n", vertex.mData[0], vertex.mData[1], vertex.mData[2]);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            
            for (int k = 0; k < fbxMesh->GetElementUVCount(); ++k)
            {
                FbxGeometryElementUV* uv = fbxMesh->GetElementUV(k);
                FBXSDK_printf("                uv:");
                
                switch (uv->GetMappingMode())
                {
                    case FbxGeometryElement::eByControlPoint:
                        switch (uv->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            {
                                FbxVector2 uvVector = uv->GetDirectArray().GetAt(vertexIndex);
                                FBXSDK_printf("%f, %f\n", uvVector.mData[0], uvVector.mData[1]);
                                break;
                            }
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = uv->GetIndexArray().GetAt(vertexIndex);
                                FbxVector2 uvVector = uv->GetDirectArray().GetAt(id);
                                FBXSDK_printf("%f, %f\n", uvVector.mData[0], uvVector.mData[1]);
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                        
                    case FbxGeometryElement::eByPolygonVertex:
                    {
                        int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
                        switch (uv->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                FbxVector2 uvVector = uv->GetDirectArray().GetAt(lTextureUVIndex);
                                FBXSDK_printf("%f, %f\n", uvVector.mData[0], uvVector.mData[1]);
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

            
            ++vertexId;
        }
    }
}
