这个方法获取不到httponly的cookie信息

CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2>	pDoc2;
_bstr_t			bStrText;
CComBSTR		value;
HRESULT			hr;

pDoc2 = m_HtmlCtl.GetHtmlDocument();
if(pDoc2 == NULL)
	return;

hr = pDoc2->get_cookie(&value);
if(FAILED(hr))
	return;

bStrText = "cookie: ";
bStrText += (BSTR)value;
Msg("%s", (CHAR *)bStrText);



让Webbrowser、CDHtmlDialog中的控件显示为系统主题样式
class CWebBrowserThemeDlg : public CDHtmlDialog
 { 
    STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo){pInfo->dwFlags |= DOCHOSTUIFLAG_THEME; return S_OK;}; 
};
 
