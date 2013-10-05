#ifndef _BASE_input_
#define _BASE_input_

struct BASE_input
{
	int vkeys[256];
	int mousepos[2];
	int mkeys[3];
	int mkey;
	int vkey;
	int ascii;
	void (*user_input) (WPARAM,LPARAM,UINT);
	void save_keystate (WPARAM,LPARAM,UINT);
	virtual void keyinput(WPARAM,LPARAM,UINT);
	BASE_input()
	{user_input=0; ascii=-1; vkey=-1;mkey=-1;}
};

void BASE_input::save_keystate(WPARAM wp,
							   LPARAM lp,UINT state)
{
	switch (state)
	{
	case WM_KEYDOWN:
		vkey=wp;
		vkeys[wp]=1;
		break;
	case WM_KEYUP: vkeys[wp]=0; break;
	case WM_CHAR: ascii=wp; break;
	case WM_LBUTTONDOWN: mkeys[0]=1; mkey=0; break;
	case WM_LBUTTONUP:mkeys[0]=0; break;
	case WM_MBUTTONDOWN: mkeys[1]=1; mkey=1; break;
	case WM_MBUTTONUP: mkeys[1]=0; break;
	case WM_RBUTTONDOWN: mkeys[2]=1; mkey=2; break;
	case WM_RBUTTONUP: mkeys[2]=0; break;
	case WM_MOUSEMOVE:
		mousepos[0]=LOWORD(lp);
		mousepos[1]=HIWORD(lp);
		break;
	}
}

void BASE_input::keyinput(WPARAM wp, LPARAM lp,
						  UINT state)
{
	save_keystate(wp,lp,state);
	if(user_input)user_input(wp,lp,state);
}

#endif