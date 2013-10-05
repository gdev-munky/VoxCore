#ifndef _GLobj_
#define _GLobj_

struct GLobj
{
	Vertex pos;
	Vertex dir;
	Vertex look;
	float da;
	float ds;

	GLobj(float _ds=0.8,float _da=0.5)
	{
		init(_ds,_da); pos.set(0,0,0);
		dir.set(0,0,0); look.set(0,0,0);
	}

	void init(float _ds=1,float _da=0.5)
	{da=_da; ds=_ds;}

	void position(float x=0,float y=0,float z=0)
	{pos.set(-x,-y,-z);}

	void direction(float x=0,float y=0,float z=0)
	{dir.set(x,y,z);}

	void looking(float x=0,float y=0,float z=0)
	{dir.set(x,y,z);}

	void movefb(float steps=1)
	{pos.move(ds*steps,dir.x,dir.y);}

	void monelr(float steps=1)
	{pos.move(ds*steps,0,dir.y-90);}

	void moneud(float steps=1)
	{pos.move(ds*steps,dir.x+90,dir.y);}

	void dirlr(float steps=1) {dir.y+=steps*da;}

	void dirud(float steps=1) {dir.x+=steps*da;}

	void looklr(float steps) {look.y+=steps*da;}

	void lookud(float steps) {look.x+=steps*da;}

	void transform(int i=1)
	{
		glRotatef(dir.x,1,0,0);
		if(i)glRotatef(look.x,1,0,0);
		glRotatef(dir.y,0,1,0);
		if(i)glRotatef(look.y,1,0,0);
		glTranslatef(pos.x,pos.y,pos.z-0.0005);
	}

	void btransform(int i=1)
	{
		glTranslatef(-pos.x,-pos.y,-pos.z-0.0005);
		glRotatef(-dir.y,0,1,0);
		if(i)glRotatef(-look.y,0,1,0);
		glRotatef(-dir.x,1,0,0);
		if(i)glRotatef(-look.x,1,0,0);
	}

	void shadow(float* plane,float lw);
	void lightpos(int i=0);
	virtual void input(BASE_input* myin,float winw=640,
		               float winh=480);
	virtual void draw(int l=0);
};

void GLobj::lightpos(int i)
{
	float ldir[]={0,0,-1};
	float lpos[]={0,0,0,1};
	glPushMatrix();
	btransform();
	glLightfv(GL_LIGHT0+i,GL_POSITION,lpos);
	glLightfv(GL_LIGHT0+i,GL_SPOT_DIRECTION,ldir);
	glPushMatrix();
}

void GLobj::shadow(float* plane,float lw)
{
	float m[4][4];
	float tmp;
	pos.mul(-1);
	tmp=plane[0]*pos.x+plane[1]*pos.y+plane[2]*pos[2]+plane[3]*lw;
	m[0][0]=tmp-pos.x*plane[0];
	m[1][0]=0.0-pos.x*plane[1];
	m[2][0]=0.0-pos.x*plane[2];
    m[3][0]=0.0-pos.x*plane[3];

	m[0][1]=0.0-pos.y*plane[0];
	m[1][1]=tmp-pos.y*plane[1];
	m[2][1]=0.0-pos.y*plane[2];
	m[3][2]=0.0-pos.y*plane[3];

	m[0][2]=0.0-pos.z*plane[0];
	m[1][2]=0.0-pos.z*plane[1];
	m[2][2]=tmp-pos.z*plane[2];
	m[3][2]=0.0-pos.z*plane[3];

	m[0][3]=0.0-lw*plane[0];
	m[1][3]=0.0-lw*plane[1];
	m[2][3]=0.0-lw*plane[2];
	m[3][3]=tmp-lw*plane[3];

	glMultMatrixf((const GLfloat*)m);
	pos.mul(-1);
}

void GLobj::draw(int l)
{
	Models m;
	if(l)
	{
		glPushMatrix();
		btransform();
		glCallList(l);
		glPopMatrix();
		return;
	}
	glPushMatrix();
	btransform();
	glColor3f(0,1,0);
	m.planeXY(-2.5,2.5,2.5,-2.5);
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,0,-10);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	btransform(0);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glColor3f(1,0,0);
	    glVertex3f(0,0,0);
		glVertex3f(0,0,-10);
		
		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);

		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
    glEnd();
	glPopMatrix();
}

void GLobj::input(BASE_input* myin,
				  float winw,float winh)
{
	POINT pt;
	static char instate=0;
	if(myin->vkey==VK_ESCAPE)
	{
		myin->vkey=-1; instate=!instate;
		if(instate)
		{
			SetCursorPos(winw/2,winh/2);
			ShowCursor(0);
		} else ShowCursor(1);
	}

	if(instate)
	{
		if(myin->vkeys['E'])movefb();
		if(myin->vkeys['D'])movefb(-1);
		if(myin->vkeys['S'])movefb();
		if(myin->vkeys['F'])movefb(-1);
		if(myin->vkeys['C'])movefb();
		if(myin->vkeys['V'])movefb(-1);
		if(myin->vkeys[VK_LEFT])dirlr(-1);
		if(myin->vkeys[VK_RIGHT])dirlr();
		if(myin->vkeys[VK_UP])dirud(-1);
		if(myin->vkeys[VK_DOWN])dirud();
		GetCursorPos(&pt);
		SetCursorPos(winw/2,winh/2);
		dirlr(-(winw/2-pt.x));
		lookud(-(winh/2-pt.y));
	}
}

#endif 
