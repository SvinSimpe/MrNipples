#include "GeometryBox.h"

unsigned int GeometryBox::sID = 0;

GeometryBox::GeometryBox()
{
	mIsVisible	= true;
	mVertexFaces = nullptr;
}

GeometryBox::GeometryBox( XMFLOAT3 origin, float width, float height, float depth )
{
	mID = sID++;
	mIsVisible	= true;
	mVertexFaces = new Vertex32[36];
	XMStoreFloat4x4( &mPerObjectData.world,  XMMatrixIdentity() );
	mPerObjectData.color = XMFLOAT3( 1.0f, 1.0f, 1.0f );

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
		   Back   = 105,054
		   Left   = 024,264
		   Right  = 317,157
		   Top    = 012,132
		   Bottom = 547,467
	*/

	float halfWidth		= width  * 0.5f;
	float halfHeight	= height * 0.5f; 
	float halfDepth		= depth  * 0.5f; 

	// Vertex 0
	Vertex32 v0;
	v0.position.x = origin.x - halfWidth;
	v0.position.y = origin.y + halfHeight;
	v0.position.z = origin.z + halfDepth;

	// Vertex 1
	Vertex32 v1;
	v1.position.x = origin.x + halfWidth;
	v1.position.y = origin.y + halfHeight;
	v1.position.z = origin.z + halfDepth;

	// Vertex 2
	Vertex32 v2;
	v2.position.x = origin.x - halfWidth;
	v2.position.y = origin.y + halfHeight;
	v2.position.z = origin.z - halfDepth;

	// Vertex 3
	Vertex32 v3;
	v3.position.x = origin.x + halfWidth;
	v3.position.y = origin.y + halfHeight;
	v3.position.z = origin.z - halfDepth;

	// Vertex 4
	Vertex32 v4;
	v4.position.x = origin.x - halfWidth;
	v4.position.y = origin.y - halfHeight;
	v4.position.z = origin.z + halfDepth;

	// Vertex 5
	Vertex32 v5;
	v5.position.x = origin.x + halfWidth;
	v5.position.y = origin.y - halfHeight;
	v5.position.z = origin.z + halfDepth;

	// Vertex 6
	Vertex32 v6;
	v6.position.x = origin.x - halfWidth;
	v6.position.y = origin.y - halfHeight;
	v6.position.z = origin.z - halfDepth;

	// Vertex 7
	Vertex32 v7;
	v7.position.x = origin.x + halfWidth;
	v7.position.y = origin.y - halfHeight;
	v7.position.z = origin.z - halfDepth;

	// Front face
	mVertexFaces[0] = v2;
	mVertexFaces[1] = v3;
	mVertexFaces[2] = v6;

	mVertexFaces[3] = v3;
	mVertexFaces[4] = v7;
	mVertexFaces[5] = v6;


	// Back face
	mVertexFaces[6] = v1;
	mVertexFaces[7] = v0;
	mVertexFaces[8] = v5;

	mVertexFaces[9]  = v0;
	mVertexFaces[10] = v5;
	mVertexFaces[11] = v4;


	// Left face
	mVertexFaces[12] = v0;
	mVertexFaces[13] = v2;
	mVertexFaces[14] = v4;

	mVertexFaces[15] = v2;
	mVertexFaces[16] = v6;
	mVertexFaces[17] = v4;


	// Right face
	mVertexFaces[18] = v3;
	mVertexFaces[19] = v1;
	mVertexFaces[20] = v7;

	mVertexFaces[21] = v1;
	mVertexFaces[22] = v5;
	mVertexFaces[23] = v7;


	// Top face
	mVertexFaces[24] = v0;
	mVertexFaces[25] = v1;
	mVertexFaces[26] = v2;

	mVertexFaces[27] = v1;
	mVertexFaces[28] = v3;
	mVertexFaces[29] = v2;


	// Bottom face
	mVertexFaces[30] = v5;
	mVertexFaces[31] = v4;
	mVertexFaces[32] = v7;

	mVertexFaces[33] = v4;
	mVertexFaces[34] = v6;
	mVertexFaces[35] = v7;


	// Normals
	//---------------------

	// Front face
	mVertexFaces[0].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f ); 
	mVertexFaces[1].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f ); 
	mVertexFaces[2].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f ); 

	mVertexFaces[3].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f ); 
	mVertexFaces[4].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f ); 
	mVertexFaces[5].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f ); 


	// Back face
	mVertexFaces[6].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
	mVertexFaces[7].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
	mVertexFaces[8].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
				    
	mVertexFaces[9].normal  = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
	mVertexFaces[10].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
	mVertexFaces[11].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 


	// Left face
	mVertexFaces[12].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
	mVertexFaces[13].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
	mVertexFaces[14].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 

	mVertexFaces[15].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
	mVertexFaces[16].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
	mVertexFaces[17].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 


	// Right face
	mVertexFaces[18].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[19].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[20].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );

	mVertexFaces[21].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[22].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[23].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );


	// Top face
	mVertexFaces[24].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	mVertexFaces[25].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	mVertexFaces[26].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
										
	mVertexFaces[27].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	mVertexFaces[28].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	mVertexFaces[29].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );


	// Bottom face
	mVertexFaces[30].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
	mVertexFaces[31].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
	mVertexFaces[32].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
											  
	mVertexFaces[33].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
	mVertexFaces[34].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
	mVertexFaces[35].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
}

GeometryBox::~GeometryBox()
{
	delete [] mVertexFaces;
}

void GeometryBox::IsVisible( bool isVisible )
{
	if( mIsVisible != isVisible )
		mIsVisible = isVisible; 
}

bool GeometryBox::IsVisible() const
{
	return mIsVisible;
}

Vertex32* GeometryBox::VertexFaces()
{
	return mVertexFaces;
}

PerObjectData GeometryBox::PerObjectData()
{
	return mPerObjectData;
}

void GeometryBox::PerObjectData( XMFLOAT4X4 data )
{
	mPerObjectData.world = data;
}