#ifndef _MISC_H_
#define _MISC_H_

#include <assert.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "observer.h"



//####################################################
// Miscellaneous helper PREPROCESSOR
//####################################################
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//## float -> int 빠른 변환
#define FFTI(f) (*((int *)(&f)))

#define M_PI				3.14159265358979323846f

//## 선 표시를 위한 VERTEX
struct LINEVERTEX
{
	enum FVF { FVF=(D3DFVF_XYZ|D3DFVF_DIFFUSE) };
	D3DXVECTOR3	p;
	D3DCOLOR	color;
};

//## bounding box 구조체
struct S_BBOX
{
	D3DXVECTOR3 vMax;
	D3DXVECTOR3 vMin;
};

#define _USE_INDEX16_

//## 최대값
#define SQR(x) ((x) * (x))

#define MAX(a,b) ( a < b ) ?  b : a ;
#define MIN(a,b) ( a < b ) ?  a : b ;

//## FVF
enum E_CUSTOM_FVF
{
	FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3
};


//## terrain D3D vertex format
struct S_CUSTOMVERTEX
{
	D3DXVECTOR3			vP;
	D3DXVECTOR3			vN;
	float				fTu, fTv;

	S_CUSTOMVERTEX &operator=(const S_CUSTOMVERTEX &Vertex)
	{
		vP = Vertex.vP;
		vN = Vertex.vN;

		fTu = Vertex.fTu;
		fTv = Vertex.fTv;

		return *this;
	}
};


//## terrain D3D index format
struct S_CUSTOMINDEX
{
#ifdef _USE_INDEX16_
	WORD	_0, _1, _2;

#else
	//DWORD	_0, _1, _2;
#endif
};



//## stl container 내용 삭제 함수
template<typename TContainer>
void stl_wipe(TContainer& container)
{
	for(TContainer::iterator i=container.begin();i!=container.end();++i)
		SAFE_DELETE( (*i) );

	container.clear();
}

#endif
