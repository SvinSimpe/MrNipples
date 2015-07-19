#include "GeometryBox.h"

GeometryBox::GeometryBox()
{
	mVertexFaces = nullptr;
}

GeometryBox::GeometryBox( XMFLOAT3 origin, float width, float height, float depth )
{
	mVertexFaces = new Vertex48[BOX_VERTEX_COUNT];

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
	Vertex48 v0;
	v0.position.x = origin.x - halfWidth;
	v0.position.y = origin.y + halfHeight;
	v0.position.z = origin.z + halfDepth;

	// Vertex 1
	Vertex48 v1;
	v1.position.x = origin.x + halfWidth;
	v1.position.y = origin.y + halfHeight;
	v1.position.z = origin.z + halfDepth;

	// Vertex 2
	Vertex48 v2;
	v2.position.x = origin.x - halfWidth;
	v2.position.y = origin.y + halfHeight;
	v2.position.z = origin.z - halfDepth;

	// Vertex 3
	Vertex48 v3;
	v3.position.x = origin.x + halfWidth;
	v3.position.y = origin.y + halfHeight;
	v3.position.z = origin.z - halfDepth;

	// Vertex 4
	Vertex48 v4;
	v4.position.x = origin.x - halfWidth;
	v4.position.y = origin.y - halfHeight;
	v4.position.z = origin.z + halfDepth;

	// Vertex 5
	Vertex48 v5;
	v5.position.x = origin.x + halfWidth;
	v5.position.y = origin.y - halfHeight;
	v5.position.z = origin.z + halfDepth;

	// Vertex 6
	Vertex48 v6;
	v6.position.x = origin.x - halfWidth;
	v6.position.y = origin.y - halfHeight;
	v6.position.z = origin.z - halfDepth;

	// Vertex 7
	Vertex48 v7;
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
	mVertexFaces[10] = v4;
	mVertexFaces[11] = v5;


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


	// Normals  | UV-coordinates  |  Tangents
	//========================================

	// Front face
	mVertexFaces[0].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		mVertexFaces[0].texCoord = XMFLOAT2( 0.0f, 0.0f );		mVertexFaces[0].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[1].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		mVertexFaces[1].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[1].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[2].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		mVertexFaces[2].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[2].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );

	mVertexFaces[3].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		mVertexFaces[3].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[3].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[4].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		mVertexFaces[4].texCoord = XMFLOAT2( 1.0f, 1.0f );		mVertexFaces[4].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[5].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		mVertexFaces[5].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[5].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );


	// Back face
	mVertexFaces[6].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f );		mVertexFaces[6].texCoord = XMFLOAT2( 0.0f, 0.0f );		mVertexFaces[6].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	mVertexFaces[7].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 		mVertexFaces[7].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[7].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	mVertexFaces[8].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 		mVertexFaces[8].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[8].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
				    
	mVertexFaces[9].normal  = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 	mVertexFaces[9].texCoord  = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[9].tangent  = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	mVertexFaces[10].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 	mVertexFaces[10].texCoord = XMFLOAT2( 1.0f, 1.0f );		mVertexFaces[10].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	mVertexFaces[11].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 	mVertexFaces[11].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[11].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );


	// Left face
	mVertexFaces[12].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f );	mVertexFaces[12].texCoord = XMFLOAT2( 0.0f, 0.0f );		mVertexFaces[12].tangent = XMFLOAT3( 0.0f, 0.0f, -1.0f ); 
	mVertexFaces[13].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 	mVertexFaces[13].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[13].tangent = XMFLOAT3( 0.0f, 0.0f, -1.0f );
	mVertexFaces[14].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 	mVertexFaces[14].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[14].tangent = XMFLOAT3( 0.0f, 0.0f, -1.0f );
																																							 
	mVertexFaces[15].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 	mVertexFaces[15].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[15].tangent = XMFLOAT3( 0.0f, 0.0f, -1.0f );
	mVertexFaces[16].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 	mVertexFaces[16].texCoord = XMFLOAT2( 1.0f, 1.0f );		mVertexFaces[16].tangent = XMFLOAT3( 0.0f, 0.0f, -1.0f );
	mVertexFaces[17].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 	mVertexFaces[17].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[17].tangent = XMFLOAT3( 0.0f, 0.0f, -1.0f );


	// Right face
	mVertexFaces[18].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );		mVertexFaces[18].texCoord = XMFLOAT2( 0.0f, 0.0f );		mVertexFaces[18].tangent = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
	mVertexFaces[19].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );		mVertexFaces[19].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[19].tangent = XMFLOAT3( 0.0f, 0.0f, 1.0f );
	mVertexFaces[20].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );		mVertexFaces[20].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[20].tangent = XMFLOAT3( 0.0f, 0.0f, 1.0f );

	mVertexFaces[21].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );		mVertexFaces[21].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[21].tangent = XMFLOAT3( 0.0f, 0.0f, 1.0f );
	mVertexFaces[22].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );		mVertexFaces[22].texCoord = XMFLOAT2( 1.0f, 1.0f );		mVertexFaces[22].tangent = XMFLOAT3( 0.0f, 0.0f, 1.0f );
	mVertexFaces[23].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );		mVertexFaces[23].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[23].tangent = XMFLOAT3( 0.0f, 0.0f, 1.0f );


	// Top face
	mVertexFaces[24].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[24].texCoord = XMFLOAT2( 0.0f, 0.0f );		mVertexFaces[24].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f ); 
	mVertexFaces[25].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[25].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[25].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[26].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[26].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[26].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
																																		
	mVertexFaces[27].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[27].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[27].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[28].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[28].texCoord = XMFLOAT2( 1.0f, 1.0f );		mVertexFaces[28].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[29].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[29].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[29].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );


	// Bottom face
	mVertexFaces[30].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );	mVertexFaces[30].texCoord = XMFLOAT2( 0.0f, 0.0f );		mVertexFaces[30].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
	mVertexFaces[31].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );	mVertexFaces[31].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[31].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	mVertexFaces[32].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );	mVertexFaces[32].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[32].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
											  
	mVertexFaces[33].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );	mVertexFaces[33].texCoord = XMFLOAT2( 1.0f, 0.0f );		mVertexFaces[33].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	mVertexFaces[34].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );	mVertexFaces[34].texCoord = XMFLOAT2( 1.0f, 1.0f );		mVertexFaces[34].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	mVertexFaces[35].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );	mVertexFaces[35].texCoord = XMFLOAT2( 0.0f, 1.0f );		mVertexFaces[35].tangent = XMFLOAT3( -1.0f, 0.0f, 0.0f );
}

GeometryBox::~GeometryBox()
{
}

Vertex48* GeometryBox::VertexFaces()
{
	return mVertexFaces;
}

void GeometryBox::Release()
{
	if( mVertexFaces != nullptr )
		delete [] mVertexFaces;
}
