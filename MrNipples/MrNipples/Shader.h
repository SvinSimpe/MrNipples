#ifndef _SHADER_H_
#define _SHADER_H_

#include "3DLibs.h"

enum ShaderType
{
	STANDARD,
	GEOMETRY_PASS,
	DEPTH,
	DEPTH_OMNI,
	DEFERRED_PASS,

	NUM_SHADERS
};

enum InputLayoutType
{
	VERTEX48,
	VERTEX32,

	NUM_INPUT_LAYOUT_TYPES
};

struct ShaderDesc
{
	char*	shaderFile	= nullptr;
	bool	hasVertex	= true;
	bool	hasHull		= false;
	bool	hasDomain	= false;
	bool	hasGeometry	= false;
	bool	hasPixel	= true;

	InputLayoutType	inputLayoutType = InputLayoutType::VERTEX48;
};

class Shader
{
	private:
		ID3D11VertexShader*		mVertexShader;
		ID3D11HullShader*		mHullShader;
		ID3D11DomainShader*		mDomainShader;
		ID3D11GeometryShader*	mGeometryShader;
		ID3D11PixelShader*		mPixelShader;
		ID3D11InputLayout*		mInputLayout;

	private:
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
	
	public:
		Shader();
		Shader( ID3D11Device* device, ShaderDesc desc );
		~Shader();
		
		ID3D11VertexShader*		GetVertexShader();
		ID3D11HullShader*		GetHullShader();
		ID3D11DomainShader*		GetDomainShader();
		ID3D11GeometryShader*	GetGeometryShader();
		ID3D11PixelShader*		GetPixelShader();
		ID3D11InputLayout*		GetInputLayout();

		void Relase();
};
#endif