// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
else if( uMsg == WM_NCACTIVATE ) 
{
	if( !::IsIconic(m_hWnd) ) 
    {                
    	return (wParam == 0) ? TRUE : FALSE;
    }
}
else if( uMsg == WM_NCCALCSIZE ) 
{            
	return 0;
}
else if( uMsg == WM_NCPAINT ) 
{            
	return 0;
}
