#include "StdAfx.h"
#include "dib.h"

CDib::CDib(void)
{
	m_lpHandle = NULL;
	m_lpbmih	= NULL;
	m_lpImgData	= NULL;
}

CDib::~CDib(void)
{
	DeleteHandle();
}

bool CDib::LoadHandle( LPCTSTR lpFilename )
{
	FILE				*fp;
	BITMAPFILEHEADER	bmfh;
	HGLOBAL				hDib;

	if( !(fp = fopen( lpFilename, "rb" )) )
	{
		MessageBox( NULL, "존재하지 않는 파일입니다.", "Error", MB_OK );
		fclose(fp);
		return false;
	}

	//## BITMAPFILEHEADER 구조체 데이타 읽음
	fread( &bmfh, sizeof(BITMAPFILEHEADER), 1, fp );

	//## 비트맵인지 체크
	if(bmfh.bfType != BITMAP_MAGIC_NUMBER)
	{
		MessageBox(NULL,"bmp format이 아닙니다. bmp format의 파일을 선택해 주십시요.","Error",MB_OK);
		fclose(fp);
		return false;
	}

	//## BITMAPFILEHEADER를 제외한 나머지 크기
	int nSize = bmfh.bfSize - sizeof(BITMAPFILEHEADER);

	//## 전역 힙 할당
	hDib = (HGLOBAL) GlobalAlloc( GMEM_FIXED, nSize );
	m_lpHandle = (LPBYTE)GlobalLock( hDib );

	if( !m_lpHandle )
	{
		fclose(fp);
		return false;
	}

	//## 전역힙에 데이타 로드
	fread( m_lpHandle, nSize, 1, fp );
	fclose(fp);

	//## LPBITMAPINFOHEADER
	m_lpbmih = ((LPBITMAPINFOHEADER)(m_lpHandle));

	//## 이미지 비트 정보 시작 위치
	m_lpImgData = (LPBYTE)(m_lpHandle)+((bmfh.bfOffBits)-sizeof(BITMAPFILEHEADER));

	//## 가로 바이트 사이즈
	//## biBitCount는 한 픽셀이 몇 개의 비트로 이루어지는가를 나타내므로
	//## biBitCount/8 은 한 픽셀당 바이트 수
	m_nByteWidth = (int)((m_lpbmih->biWidth)*((m_lpbmih->biBitCount)/8));

	//## 실제 가로 바이트 사이즈 - 4의 배수
	m_nPadWidth = m_nByteWidth%4 ? m_nByteWidth+4-(m_nByteWidth%4) : m_nByteWidth;

	return true;
}

void CDib::DeleteHandle()
{
	//## 메모리 해제
	if( m_lpHandle )
		GlobalFree( m_lpHandle );
}