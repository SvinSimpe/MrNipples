#include "AssetManager.h"
#include <DDSTextureLoader.h>

HRESULT AssetManager::LoadTexture( std::string assetPath, TextureType type )
{
	mTextureData[type] = new TextureData();
	
	ID3D11Texture2D*	tempTexture		= nullptr;	
	std::wstring		directory( L"Textures/" );
	std::wstring		color( L"_COLOR.DDS" );
	std::wstring		specular( L"_SPEC.DDS" );
	std::wstring		normal( L"_NRM.DDS" );
	std::wstring		displacement( L"_DISP.DDS" );
	// Convert std:string to std::wstring
	std::wstring convertedPath( assetPath.begin(), assetPath.end() );

	directory += convertedPath;

	// COLOR-------------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , (directory + color).c_str(), (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mTextureData[type]->colorMap ) ) )
		return E_FAIL;
	// SPECULAR----------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , (directory + specular).c_str(), (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mTextureData[type]->specularMap ) ) )
		return E_FAIL;
	// NORMAL------------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , (directory + normal).c_str(), (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mTextureData[type]->normalMap ) ) )
		return E_FAIL;
	// DISPLACEMENT------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , (directory + displacement).c_str(), (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mTextureData[type]->displacementMap ) ) )
		return E_FAIL;
	//-------------------------------------------------------------------------------------------------------------------

	return S_OK;
}

AssetManager::AssetManager()
{
	mDevice			= nullptr;
	mDeviceContext	= nullptr;
	mTextureData	= nullptr;
}

AssetManager::~AssetManager()
{}

HRESULT AssetManager::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;
	mTextureData	= new TextureData*[TextureType::NUM_TEXTURES];
	/*
		Add textures that you want to load here!
		Be sure to add new types of texture to the
		TextureType enum in AssetManager.h.
	*/

	if( FAILED( LoadTexture( "Cobble/cobbleTexture", TextureType::COBBLE ) ) )
		return E_FAIL;


	return S_OK;
}

HRESULT AssetManager::SetTexture( TextureType type )
{

	mDeviceContext->DSSetShaderResources( 0, 1, &mTextureData[type]->displacementMap );
	ID3D11ShaderResourceView* pixelViews[3] = { mTextureData[type]->colorMap, mTextureData[type]->specularMap, mTextureData[type]->normalMap };
	mDeviceContext->PSSetShaderResources( 1, 3, pixelViews );

	return S_OK;
}

TextureData* AssetManager::GetTexture( TextureType type ) const
{
	return mTextureData[type];
}

void AssetManager::Release()
{
	for (size_t i = 0; i < TextureType::NUM_TEXTURES; i++)
	{
		mTextureData[i]->Release();
		delete mTextureData[i];
	}

	SAFE_DELETE_ARRAY( mTextureData );
}