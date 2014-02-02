//
//  CSpriteEx.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-27.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CSpriteEx.h"

static class CSpriteExRegister
{
public:
    CSpriteExRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "SpriteEx", CSpriteExLoader::loader());
    }
} __reg;



void CSpriteEx::prepareSpriteEx()
{
    m_sQuadOri = _quad;
}



//沿Y轴旋转degree角度
void CSpriteEx::rotateY(float degree)
{
    static float fRadSeed = 3.14159f/180.0f;
    
    //创建个旋转矩阵
    kmMat4 kMat;
    kmMat4Identity(&kMat);
    kmMat4RotationY(&kMat, degree*fRadSeed);
    
    Vertex3F* v[4] = {&_quad.bl.vertices, &_quad.br.vertices, &_quad.tl.vertices, &_quad.tr.vertices};
    Vertex3F* vOri[4] = {&m_sQuadOri.bl.vertices, &m_sQuadOri.br.vertices, &m_sQuadOri.tl.vertices, &m_sQuadOri.tr.vertices};
    
    //向量矩阵相乘
    for(int i = 0; i < 4; ++i) {
        float x = kMat.mat[0]*vOri[i]->x + kMat.mat[4]*vOri[i]->y + kMat.mat[8]*vOri[i]->z + kMat.mat[12];
        float y = kMat.mat[1]*vOri[i]->x + kMat.mat[5]*vOri[i]->y + kMat.mat[9]*vOri[i]->z + kMat.mat[13];
        float z = kMat.mat[2]*vOri[i]->x + kMat.mat[6]*vOri[i]->y + kMat.mat[10]*vOri[i]->z + kMat.mat[14];
        
        v[i]->x = x;
        v[i]->y = y;
        v[i]->z = z;
    }
}



void CSpriteEx::rotateX(float degree)
{
    static float fRadSeed = 3.14159f/180.0f;
    
    //创建个旋转矩阵
    kmMat4 kMat;
    kmMat4Identity(&kMat);
    kmMat4RotationX(&kMat, degree*fRadSeed);
    
    Vertex3F* v[4] = {&_quad.bl.vertices, &_quad.br.vertices, &_quad.tl.vertices, &_quad.tr.vertices};
    Vertex3F* vOri[4] = {&m_sQuadOri.bl.vertices, &m_sQuadOri.br.vertices, &m_sQuadOri.tl.vertices, &m_sQuadOri.tr.vertices};
    
    //向量矩阵相乘
    for(int i = 0; i < 4; ++i) {
        float x = kMat.mat[0]*vOri[i]->x + kMat.mat[4]*vOri[i]->y + kMat.mat[8]*vOri[i]->z + kMat.mat[12];
        float y = kMat.mat[1]*vOri[i]->x + kMat.mat[5]*vOri[i]->y + kMat.mat[9]*vOri[i]->z + kMat.mat[13];
        float z = kMat.mat[2]*vOri[i]->x + kMat.mat[6]*vOri[i]->y + kMat.mat[10]*vOri[i]->z + kMat.mat[14];
        
        v[i]->x = x;
        v[i]->y = y;
        v[i]->z = z;
    }
}



