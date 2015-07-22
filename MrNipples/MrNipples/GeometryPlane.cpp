#include "GeometryPlane.h"

GeometryPlane::GeometryPlane()
{
	mVertexFaces = nullptr;
}

GeometryPlane::GeometryPlane( XMFLOAT3 origin, float width, float depth )
{
	mVertexFaces = new Vertex48[PLANE_VERTEX_COUNT];

	/*
	        Z
	       ^
		  /		0--------------------1
	     /	   /				    /
	    /     /                    /
	   /	 /                    /
	  /   	2--------------------3
	 /   
	-----------------------------> X
		 Faces = 012,132

	*/

	float halfWidth		= width  * 0.5f;
	float halfDepth		= depth  * 0.5f; 

	// Vertex 0
	Vertex48 v0;
	v0.position.x = origin.x - halfWidth;
	v0.position.y = 0.0f;
	v0.position.z = origin.z + halfDepth;

	// Vertex 1
	Vertex48 v1;
	v1.position.x = origin.x + halfWidth;
	v1.position.y = 0.0f;
	v1.position.z = origin.z + halfDepth;

	// Vertex 2
	Vertex48 v2;
	v2.position.x = origin.x - halfWidth;
	v2.position.y = 0.0f;
	v2.position.z = origin.z - halfDepth;

	// Vertex 3
	Vertex48 v3;
	v3.position.x = origin.x + halfWidth;
	v3.position.y = 0.0f;
	v3.position.z = origin.z - halfDepth;	


	// Face vertices
	mVertexFaces[0] = v0;
	mVertexFaces[1] = v1;
	mVertexFaces[2] = v2;
				 
	mVertexFaces[3] = v1;
	mVertexFaces[4] = v3;
	mVertexFaces[5] = v2;


	// Normals  | UV-coordinates  |  Tangents
	//========================================	
	mVertexFaces[0].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[0].texCoord = XMFLOAT2( 0.0f,  0.0f );		mVertexFaces[0].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f ); 
	mVertexFaces[1].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[1].texCoord = XMFLOAT2( width, 0.0f );		mVertexFaces[1].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[2].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[2].texCoord = XMFLOAT2( 0.0f,  depth );	mVertexFaces[2].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
				 															 														 
	mVertexFaces[3].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[3].texCoord = XMFLOAT2( width, 0.0f );		mVertexFaces[3].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[4].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[4].texCoord = XMFLOAT2( width, depth );	mVertexFaces[4].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mVertexFaces[5].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );		mVertexFaces[5].texCoord = XMFLOAT2( 0.0f,  depth );	mVertexFaces[5].tangent = XMFLOAT3( 1.0f, 0.0f, 0.0f );
}

GeometryPlane::~GeometryPlane()
{
}

Vertex48* GeometryPlane::VertexFaces()
{
	return mVertexFaces;
}

void GeometryPlane::Release()
{
	SAFE_DELETE_ARRAY( mVertexFaces );
}