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
		MessageBox( NULL, "�������� �ʴ� �����Դϴ�.", "Error", MB_OK );
		fclose(fp);
		return false;
	}

	//## BITMAPFILEHEADER ����ü ����Ÿ ����
	fread( &bmfh, sizeof(BITMAPFILEHEADER), 1, fp );

	//## ��Ʈ������ üũ
	if(bmfh.bfType != BITMAP_MAGIC_NUMBER)
	{
		MessageBox(NULL,"bmp format�� �ƴմϴ�. bmp format�� ������ ������ �ֽʽÿ�.","Error",MB_OK);
		fclose(fp);
		return false;
	}

	//## BITMAPFILEHEADER�� ������ ������ ũ��
	int nSize = bmfh.bfSize - sizeof(BITMAPFILEHEADER);

	//## ���� �� �Ҵ�
	hDib = (HGLOBAL) GlobalAlloc( GMEM_FIXED, nSize );
	m_lpHandle = (LPBYTE)GlobalLock( hDib );

	if( !m_lpHandle )
	{
		fclose(fp);
		return false;
	}

	//## �������� ����Ÿ �ε�
	fread( m_lpHandle, nSize, 1, fp );
	fclose(fp);

	//## LPBITMAPINFOHEADER
	m_lpbmih = ((LPBITMAPINFOHEADER)(m_lpHandle));

	//## �̹��� ��Ʈ ���� ���� ��ġ
	m_lpImgData = (LPBYTE)(m_lpHandle)+((bmfh.bfOffBits)-sizeof(BITMAPFILEHEADER));

	//## ���� ����Ʈ ������
	//## biBitCount�� �� �ȼ��� �� ���� ��Ʈ�� �̷�����°��� ��Ÿ���Ƿ�
	//## biBitCount/8 �� �� �ȼ��� ����Ʈ ��
	m_nByteWidth = (int)((m_lpbmih->biWidth)*((m_lpbmih->biBitCount)/8));

	//## ���� ���� ����Ʈ ������ - 4�� ���
	m_nPadWidth = m_nByteWidth%4 ? m_nByteWidth+4-(m_nByteWidth%4) : m_nByteWidth;

	return true;
}

void CDib::DeleteHandle()
{
	//## �޸� ����
	if( m_lpHandle )
		GlobalFree( m_lpHandle );
}