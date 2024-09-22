#ifndef _DIB_H_
#define _DIB_H_

//## bitmap인지 체크하기 위한 값
const short BITMAP_MAGIC_NUMBER = 19778;

//####################################################
//####################################################
// CDib
//####################################################
//####################################################
class AFX_EXT_CLASS CDib
{
	LPBYTE				m_lpHandle;
	LPBITMAPINFOHEADER	m_lpbmih;
	LPBYTE				m_lpImgData;

	int					m_nByteWidth;
	int					m_nPadWidth;

public:
	CDib(void);
	~CDib(void);

	int	GetWidth(){ return m_lpbmih->biWidth; }
	int GetHeight(){ return m_lpbmih->biHeight; }

	LPBYTE GetPixelColorPointer(int x, int z)
	{
		return (m_lpImgData+(x*(m_lpbmih->biBitCount/8))+((m_lpbmih->biHeight-z-1)*m_nPadWidth));
	}

	bool LoadHandle( LPCTSTR lpFilename );
	void DeleteHandle();
};

#endif