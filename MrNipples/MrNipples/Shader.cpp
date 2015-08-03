#include "Shader.h"

HRESULT Shader::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags =	D3DCOMPILE_ENABLE_STRICTNESS | 
							D3DCOMPILE_IEEE_STRICTNESS;

	std::string shader_code;
	std::ifstream in( shaderFile, std::ios::in | std::ios::binary );

	if ( in )
	{
		in.seekg( 0, std::ios::end );
		shader_code.resize( (unsigned int)in.tellg() );
		in.seekg( 0, std::ios::beg );
		in.read( &shader_code[0], shader_code.size() );
		in.close();
	}

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompile( shader_code.data(),
							 shader_code.size(),
							 NULL,
							 pDefines,
							 nullptr,
							 pEntrypoint,
							 pTarget,
							 dwShaderFlags,
							 NULL,
							 pCompiledShader,
							 &pErrorBlob );

	if( pErrorBlob )
	{
		OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
		pErrorBlob->Release();
	}


	return hr;
}

Shader::Shader()
{
	mVertexShader	= nullptr;
	mHullShader		= nullptr;
	mDomainShader	= nullptr;
	mGeometryShader	= nullptr;
	mPixelShader	= nullptr;
	mInputLayout	= nullptr;
}

Shader::Shader( ID3D11Device* device, ShaderDesc desc )
{
	HRESULT hr = S_OK;

	//------------------------
	// Compile Vertex Shader |
	//------------------------
	ID3DBlob* vs = nullptr;
	
	if ( SUCCEEDED( hr = CompileShader( desc.shaderFile, "VS", "vs_5_0", nullptr, &vs ) ) )
	{
		if( SUCCEEDED( hr = device->CreateVertexShader( vs->GetBufferPointer(),
														  vs->GetBufferSize(),
														  nullptr,
														  &mVertexShader ) ) )
		{

			if( desc.inputLayoutType == InputLayoutType::VERTEX48 )
			{
				D3D11_INPUT_ELEMENT_DESC inputDescInstanced[] = {				 
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, 24, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 32, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "WORLD",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 } };



				hr = device->CreateInputLayout( inputDescInstanced,
										     	  ARRAYSIZE( inputDescInstanced ),
												  vs->GetBufferPointer(),
												  vs->GetBufferSize(),
												  &mInputLayout );
			}
			else if( desc.inputLayoutType == InputLayoutType::VERTEX32 )
			{
				D3D11_INPUT_ELEMENT_DESC inputDesc[] = {				 
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, 24, D3D11_INPUT_PER_VERTEX_DATA,   0 } };

				hr = device->CreateInputLayout( inputDesc,
										     	  ARRAYSIZE( inputDesc ),
												  vs->GetBufferPointer(),
												  vs->GetBufferSize(),
												  &mInputLayout );
			}
		}


		vs->Release();
	}


	if( desc.hasHull )
	{
		//----------------------
		// Compile Hull Shader |
		//----------------------
		ID3DBlob* ths = nullptr;

		if( SUCCEEDED( hr = CompileShader( desc.shaderFile, "HS", "hs_5_0", nullptr, &ths ) ) )
		{
			hr = device->CreateHullShader(  ths->GetBufferPointer(),
											  ths->GetBufferSize(),
											  nullptr,
											  &mHullShader );
			ths->Release();
		}
	}

	if( desc.hasDomain )
	{
		//------------------------
		// Compile Domain Shader |
		//------------------------
		ID3DBlob* tds = nullptr;

		if( SUCCEEDED( hr = CompileShader( desc.shaderFile, "DS", "ds_5_0", nullptr, &tds ) ) )
		{
			hr = device->CreateDomainShader(  tds->GetBufferPointer(),
												tds->GetBufferSize(),
												nullptr,
												&mDomainShader );
			tds->Release();
		}
	}

	if( desc.hasGeometry )
	{
		//--------------------------
		// Compile Geometry Shader |
		//--------------------------
		ID3DBlob* tgs = nullptr;

		if( SUCCEEDED( hr = CompileShader( desc.shaderFile, "GS", "gs_5_0", nullptr, &tgs ) ) )
		{
			hr = device->CreateGeometryShader(  tgs->GetBufferPointer(),
												tgs->GetBufferSize(),
												nullptr,
												&mGeometryShader );
			tgs->Release();
		}
	}

	if( desc.hasPixel )
	{
		//-----------------------
		// Compile Pixel Shader |
		//-----------------------
		ID3DBlob* tps = nullptr;

		if( SUCCEEDED( hr = CompileShader( desc.shaderFile, "PS", "ps_5_0", nullptr, &tps ) ) )
		{
			hr = device->CreatePixelShader( tps->GetBufferPointer(),
											  tps->GetBufferSize(),
											  nullptr,
											  &mPixelShader );
			tps->Release();
		}
	}

	if( hr == S_OK )
		{
			OutputDebugStringA( "---------- " );
				OutputDebugStringA( "Succesfully compiled " );
			OutputDebugStringA( desc.shaderFile );
			OutputDebugStringA( " ---------- \n" );
		}
		else
		{
			OutputDebugStringA( "---------- " );
				OutputDebugStringA( "Failed to compile " );
			OutputDebugStringA( desc.shaderFile );
			OutputDebugStringA( " ---------- \n" );

			exit(0);
		
		}
}

Shader::~Shader()
{}

ID3D11VertexShader* Shader::GetVertexShader()
{
	return mVertexShader;
}

ID3D11HullShader* Shader::GetHullShader()
{
	return mHullShader;
}

ID3D11DomainShader* Shader::GetDomainShader()
{
	return mDomainShader;
}

ID3D11GeometryShader* Shader::GetGeometryShader()
{
	return mGeometryShader;
}

ID3D11PixelShader* Shader::GetPixelShader()
{
	return mPixelShader;
}

ID3D11InputLayout* Shader::GetInputLayout()
{
	return mInputLayout;
}

void Shader::Relase()
{
	SAFE_RELEASE( mVertexShader );
	SAFE_RELEASE( mHullShader	 );
	SAFE_RELEASE( mDomainShader );
	SAFE_RELEASE( mGeometryShader );
	SAFE_RELEASE( mPixelShader );
	SAFE_RELEASE( mInputLayout );
}