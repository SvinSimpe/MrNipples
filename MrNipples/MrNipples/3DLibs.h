#ifndef _3DLIBS_H_
#define _3DLIBS_H_
///---------------------------------------------------
#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete [] (x); (x) = NULL; }
#define PI (3.14159265358979323846f)

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <fstream>
#include <vector>

using namespace DirectX;
///---------------------------------------------------

static const float CLEAR_COLOR_BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const float CLEAR_COLOR_RED[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
static const float CLEAR_COLOR_GREEN[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
static const float CLEAR_COLOR_BLUE[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

struct PerInstanceData // 80 byte
{
	XMFLOAT4X4  world;
	XMFLOAT4	color;
};

struct PerFrameData // 152 byte
{
	XMFLOAT4X4			view;
	XMFLOAT4X4			projection;
	XMFLOAT4			eyePositionAndTessAmount;
};

struct DepthLightData // 16 byte
{
	XMFLOAT4X4 worldViewProjection;
};

struct PointLightData // 144 byte
{
	XMFLOAT4	positionAndRadius;
	XMFLOAT4	ambient;
	XMFLOAT4	diffuse;
	XMFLOAT4	specular;
	XMFLOAT3	attenuation;
	XMFLOAT4X4  world;
	float		padding;
};
///---------------------------------------------------
#endif



	/*
		       0-------1
			 / .     / |
		   2-------3   |
		   |   .   |   |
		   |   .   |   |
		   |   4...|...5
		   | .     | /
		   6-------7
		   Front  = 236,376
		   Back   = 105,045
		   Left   = 024,264
		   Right  = 317,157
		   Top    = 012,132
		   Bottom = 547,467
	*/