#ifndef _GLmain_
#define _GLmain_

struct GLmain
{
	HGLRC hrc;
	HDC hdc;
	HWND hwnd;
	int select_size;
	int feedback_size;
	uint* select;
	float* feedback;
	float l,r,t,b,n,f;
	void resize();
	void init(HWND hWnd);
	void deinit();
	void set(int);

	void font(unsigned int disp_start=1, char*fontname=0,
		      LOGFONT*lg=0, int type=0)
	{
		GLYPHMETRICSFLOAT char_set[256];
		LOGFONT font_info;
		if(lg==0)
		{
			font_info.lfHeight=25;
			font_info.lfWidth=0;
			font_info.lfEscapement=0;
			font_info.lfOrientation=0;
			font_info.lfWeight=FW_BOLD;
			font_info.lfItalic=0;
			font_info.lfUnderline=0;
			font_info.lfStrikeOut=0;
			font_info.lfCharSet=ANSI_CHARSET;
			font_info.lfOutPrecision=OUT_DEFAULT_PRECIS;
			font_info.lfClipPrecision=CLIP_DEFAULT_PRECIS;;
			font_info.lfQuality=DEFAULT_QUALITY;
			font_info.lfPitchAndFamily=34;
			if(fontname)
				lstrcpy(font_info.lfFaceName,LPCWSTR(fontname));
			else
				lstrcpy(font_info.lfFaceName,LPCWSTR("ARIAL"));
			lg=&font_info;
		}

		HFONT myfont=CreateFontIndirect(lg);

		HGDIOBJ temp=SelectObject(hdc, myfont);
		switch(type)
		{
		case 1:
			wglUseFontOutlines(hdc,0,255,disp_start,0,0.1,WGL_FONT_LINES,char_set);
		break;
		
		case 2:
			wglUseFontBitmaps(hdc,0,255,disp_start);
        break;

		default:
			wglUseFontOutlines(hdc,0,255,disp_start,0,0.1,WGL_FONT_POLYGONS, char_set);
		}

		SelectObject(hdc,temp);
		DeleteObject(myfont);
	}
};

void GLmain::deinit()
{
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hrc);
	hrc=NULL;
	ReleaseDC(hwnd,hdc);
	hdc=NULL;
	if(select_size)delete[]select;
	if(feedback_size)delete[]feedback;
}

void GLmain::init(HWND hWnd)
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),1,
		PFD_DRAW_TO_WINDOW|
		PFD_SUPPORT_OPENGL|
		PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,
		16,0,0,0,0,0,0,0,0,0,0,0,0,0,16,
		16,0, PFD_MAIN_PLANE,0,0,0,0
	};
	hwnd=hWnd;
	hdc=GetDC(hwnd);
	int PixelFormat = ChoosePixelFormat(hdc,&pfd);
	SetPixelFormat(hdc,PixelFormat,&pfd);
	hrc=wglCreateContext(hdc);
	wglMakeCurrent(hdc,hrc);
}

void GLmain::set(int mode)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	RECT rect;
	switch(mode)
	{
	case 1:
		glOrtho(-100,100,-100,100,-100,100);
		l=-100; r=100;b=-100;t=100;n=-100;f=100;
		break;
	case 2:
		glFrustum(-100,100,-100,100,1,200);
		l=-100;r=100;b=-100;t=100;n=1;f=200;
		break;
	case 3:
		glFrustum(l,r,b,t,n,f);
		break;
	default:
		GetClientRect(hwnd,&rect);
		gluPerspective(45.0f, rect.right/((!rect.bottom)?1:rect.bottom),1,500.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	select_size=100;
	select=new uint[select_size];
	glSelectBuffer(select_size,select);
	feedback_size=200;
	feedback=new float[feedback_size];
	glFeedbackBuffer(feedback_size,GL_3D,feedback);
}

void GLmain::resize()
{
	RECT rect;
	GetClientRect(hwnd,&rect);
	glViewport(0,0,rect.right,rect.bottom);
}

#endif