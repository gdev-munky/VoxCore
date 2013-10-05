#include "myinc.h" //заголовояные файлы

#define VOXELCORE_VERSION 0
#define VOXELCORE_TITLE L"Voxel Core (ALPHA v0.0.0)"

//размер окна
int width = 800, 
	height = 600; 

GLmain gl; //объект OpenGL
BASE_input in; //объект ввода

/*--Обявление функций программы--*/
ATOM winclass(); //создание и регистрация нового оконного класса
HWND makewin(); //новое окно
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, //оконная процедура
						 WPARAM wParam, LPARAM lParam);
void draw();
void example_init();
void example_deinit();
/*--Конец объявления--*/

/*--------------------------------------------------------------*/

//создание и регистрация нового оконного класса
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

HWND makewin() //новое окно
{
	return CreateWindowEx(0, LPCWSTR("OpenGL"), VOXELCORE_TITLE, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		0, 0, width, height, NULL, NULL, GetModuleHandle(NULL),
		NULL);
}

//оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,
						 WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY: //уничтожение окна
		//освобождение ресурсов примера
		example_deinit();
		PostQuitMessage(0);
		break;
	case WM_SIZE: //изменение размера окна
		gl.resize();
		break;
	//ввод с клавиатуры
	case WM_KEYDOWN:case WM_CHAR:case WM_KEYUP:
		in.keyinput(wParam,lParam,uMsg);
		break;
	//ввод с мыши
	case WM_LBUTTONDOWN:case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:case WM_RBUTTONUP:
		in.keyinput(wParam,lParam,uMsg);
		break;
	case WM_MOUSEMOVE: //движение мыши
		in.keyinput(wParam,lParam,uMsg);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return 0;
}

void example_init() //инстализация примера
{
	gl.set(1); //инстализация OpenGL
}

void example_deinit() //освобождение ресурсов
{
	//...
}

void draw() //вывод сцены
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

int WINAPI WinMain //точка входа в Windows программу
	(HINSTANCE hInstance,HINSTANCE hPrevInstence,
	 LPSTR lpCmdLine, int nCmdShow)
{
	winclass(); //регистрация оконного класса
	HWND hWnd = makewin(); //создание окна
	gl.init(hWnd); //инстализация OpenGL
	example_init(); //инстализация переменных
	ShowWindow(hWnd, SW_SHOW); //отображаем окно

	MSG msg; //индентификатор обробатываемого сообщения
	while(msg.message != WM_QUIT)
	{
		//получаем сообщение из очереди
		PeekMessage(&msg,NULL,0,0,PM_REMOVE);
		//передаем сообщение окну адрусату
		DispatchMessage(&msg);
		TranslateMessage(&msg);
		draw(); //вывод сцены в буфер кадра OpenGL
		//вывод в окно
		SwapBuffers(gl.hdc);
	}
	return 0;
}