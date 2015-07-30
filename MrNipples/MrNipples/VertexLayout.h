#ifndef _VERTEXLAYOUT_H_
#define _VERTEXLAYOUT_H_

#include "3DLibs.h"

struct Vertex48
{
	XMFLOAT3	position;
	XMFLOAT3	normal;
	XMFLOAT2	texCoord;
	XMFLOAT3	tangent;
	float		padding;
};

struct Vertex32
{
	XMFLOAT3	position;
	XMFLOAT3	normal;
	XMFLOAT2	texCoord;
};
#endif