#ifndef _ASSETMANAGER_H_
#define _ASSETMANAGER_H_

#include "3DLibs.h"


enum TextureType
{
	COBBLE,
	NUM_TEXTURES
};

struct TextureData
{
	ID3D11ShaderResourceView*	colorMap		= nullptr;
	ID3D11ShaderResourceView*	specularMap		= nullptr;
	ID3D11ShaderResourceView*	normalMap		= nullptr;
	ID3D11ShaderResourceView*	displacementMap	= nullptr;

	void Release()
	{
		SAFE_RELEASE( colorMap );
		SAFE_RELEASE( specularMap );
		SAFE_RELEASE( normalMap );
		SAFE_RELEASE( displacementMap );
	}
};

class AssetManager
{
	private:
		ID3D11Device*			mDevice;
		ID3D11DeviceContext*	mDeviceContext;
		TextureData**			mTextureData;
	
	private:
		HRESULT LoadTexture( std::string assetPath, TextureType type );

	public:
						AssetManager();
						~AssetManager();
		HRESULT			Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
		HRESULT			SetTexture( TextureType type );
		TextureData*	GetTexture( TextureType type ) const;
		void Release();
};
#endif