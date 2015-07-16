#ifndef _3DLIBS_H_
#define _3DLIBS_H_
///---------------------------------------------------
#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define PI (3.14159265358979323846f)

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <fstream>
#include <vector>

using namespace DirectX;
///---------------------------------------------------

struct PerInstanceData // 80 byte
{
	XMFLOAT4X4  world;
	XMFLOAT4	color;
};

struct PerFrameData // 144 byte
{
	XMFLOAT4X4  view;
	XMFLOAT4X4  projection;
	XMFLOAT3	eyePosition;
	float		padding;
};

struct PointLightData // 80 byte
{
	XMFLOAT4	positionAndRadius;
	XMFLOAT4	ambient;
	XMFLOAT4	diffuse;
	XMFLOAT4	specular;
	XMFLOAT3	attenuation;
	float		padding;
};

///---------------------------------------------------
#endif