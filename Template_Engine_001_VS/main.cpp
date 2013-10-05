#include "myinc.h" //������������ �����

#define VOXELCORE_VERSION 0
#define VOXELCORE_TITLE L"Voxel Core (ALPHA v0.0.0)"

//������ ����
int width = 800, 
	height = 600; 

GLmain gl; //������ OpenGL
BASE_input in; //������ �����

/*--��������� ������� ���������--*/
ATOM winclass(); //�������� � ����������� ������ �������� ������
HWND makewin(); //����� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, //������� ���������
						 WPARAM wParam, LPARAM lParam);
void draw();
void example_init();
void example_deinit();
/*--����� ����������--*/

/*--------------------------------------------------------------*/

//�������� � ����������� ������ �������� ������
ATOM winclass()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc=(WNDPROC)WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=GetModuleHandle(NULL);
	wc.hIcon=LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=NULL;
	wc.lpszMenuName=NULL;
	wc.lpszClassName=LPCWSTR("OpenGL");
	return RegisterClass(&wc);
}

HWND makewin() //����� ����
{
	return CreateWindowEx(0, LPCWSTR("OpenGL"), VOXELCORE_TITLE, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		0, 0, width, height, NULL, NULL, GetModuleHandle(NULL),
		NULL);
}

//������� ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,
						 WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY: //����������� ����
		//������������ �������� �������
		example_deinit();
		PostQuitMessage(0);
		break;
	case WM_SIZE: //��������� ������� ����
		gl.resize();
		break;
	//���� � ����������
	case WM_KEYDOWN:case WM_CHAR:case WM_KEYUP:
		in.keyinput(wParam,lParam,uMsg);
		break;
	//���� � ����
	case WM_LBUTTONDOWN:case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:case WM_RBUTTONUP:
		in.keyinput(wParam,lParam,uMsg);
		break;
	case WM_MOUSEMOVE: //�������� ����
		in.keyinput(wParam,lParam,uMsg);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return 0;
}

void example_init() //������������ �������
{
	gl.set(1); //������������ OpenGL
}

void example_deinit() //������������ ��������
{
	//...
}

void draw() //����� �����
{
	glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex3f(-5, -5 ,0);
		glColor3f(0, 1, 0);
		glVertex3f(5, -5 ,0);
		glColor3f(0, 0, 1);
		glVertex3f(5, 5 ,0);
		glColor3f(1, 0, 1);
		glVertex3f(-5, 5 ,0);
	glEnd();
}

int WINAPI WinMain //����� ����� � Windows ���������
	(HINSTANCE hInstance,HINSTANCE hPrevInstence,
	 LPSTR lpCmdLine, int nCmdShow)
{
	winclass(); //����������� �������� ������
	HWND hWnd = makewin(); //�������� ����
	gl.init(hWnd); //������������ OpenGL
	example_init(); //������������ ����������
	ShowWindow(hWnd, SW_SHOW); //���������� ����

	MSG msg; //�������������� ��������������� ���������
	while(msg.message != WM_QUIT)
	{
		//�������� ��������� �� �������
		PeekMessage(&msg,NULL,0,0,PM_REMOVE);
		//�������� ��������� ���� ��������
		DispatchMessage(&msg);
		TranslateMessage(&msg);
		draw(); //����� ����� � ����� ����� OpenGL
		//����� � ����
		SwapBuffers(gl.hdc);
	}
	return 0;
}