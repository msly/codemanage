///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////
// PictureCtrl.h
// 
// Author: Tobias Eiseler
//
// E-Mail: tobias.eiseler@sisternicky.com
// 
// Function: A MFC Picture Control to display
//           an image on a Dialog, etc.
///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#pragma once
#include "afxwin.h"

class CPictureCtrl :
	public CStatic
{
public:

	//Constructor
	CPictureCtrl(void);

	//Destructor
	~CPictureCtrl(void);

public:

	//Loads an image from a file
	BOOL LoadFromFile(CString &szFilePath);

	//Loads an image from an IStream interface
	BOOL LoadFromStream(IStream* piStream);

	//Loads an image from a byte stream;
	BOOL LoadFromStream(BYTE* pData, size_t nSize);

	//Loads an image from a Resource
// 	BOOL LoadFromResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType);

	//Overload - Single load function
	BOOL Load(CString &szFilePath);
	BOOL Load(IStream* piStream);
	BOOL Load(BYTE* pData, size_t nSize);
// 	BOOL Load(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType);

	//Frees the image data
	void FreeData();

protected:
	virtual void PreSubclassWindow();

	//Draws the Control
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnEraseBkgnd(CDC* pDC);



private:

	//Internal image stream buffer
	IStream* m_pStream;

	//Control flag if a pic is loaded
	BOOL m_bIsPicLoaded;

	//GDI Plus Token
	ULONG_PTR m_gdiplusToken;
};
