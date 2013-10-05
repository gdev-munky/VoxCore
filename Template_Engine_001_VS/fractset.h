#ifndef _fractals_
#define _fractals_

struct Fractalset
{
	int w,h;
	unsigned char*data;
	complexf min,max;
	complexf c;
	int r;
	int maxit;
	Fractalset(){init();}
	~Fractalset(){if(data)delete[]data;}
    void init(int ww=320,int hh=200,
		      complexf mmin=complexf(-2,2),
			  complexf mmax=complexf(2,2),int rr=4)
	{
		w=ww;h=hh;r=rr;
		if(data)delete[]data;
		data=new unsigned char[w*h];
		min=mmin;
		max=mmax;
		maxit=255;
		c=complexf(-0.5,0);
	}
    void setM(int(*F)(complexf,complexf,int,int));
	void setJ(int(*F)(complexf,complexf,int,int));
	void move(int,int,float);
};

void Fractalset::setM(int(*F)(complexf,complexf,int,int))
{
	float xinc,yinc;
	xinc=(max-min).real()/w;
	yinc=(max-min).imag()/h;
	complexf z(min);
	int k=0,j;
	for(j=0;j<h;j++)
	{
		z=complexf(min.real(),z.imag());
		for(int i=0;i<w;i++)
		{
		    data[k++]=F(c,z,maxit,r);
		    z+=xinc;
		}
	    z=complexf(z.real(),z.imag()+yinc);
	}
}

void Fractalset::setJ(int(*F)(complexf,complexf,int,int))
{
	float xinc,yinc;
	xinc=(max-min).real()/w;
	yinc=(max-min).imag()/h;
	complexf z(min);
	int k=0,j;
	for(j=0;j<h;j++)
	{
		z=complexf(min.real(),z.imag());
		for(int i=0;i<w;i++)
		{
			data[k++]=F(z,c,maxit,r);
			z+=xinc;
		}
		z=complexf(z.real(),z.imag()+yinc);
	}
}

void Fractalset::move(int x,int y,float zoom)
{
	complexf delta;
	float xinc=(max.real()-min.real())/w;
	float yinc=(max.imag()-min.imag())/h;
	xinc*=x;
	yinc*=y;
	min=complexf((min.real()+xinc)/zoom,
		         (min.imag()+yinc)/zoom);
	max=complexf((max.real()+xinc)/zoom,
		         (max.imag()+yinc)/zoom);
}

int complex_f(complexf z0,complexf zc,
			 int color,int rr)
{
	while(norm(z0)<=rr&&color--)
	{z0*=z0; z0+=zc;}
	return color;
}

int complex_f1(complexf z0,complexf zc,
			   int color,int rr)
{
	while(norm(z0)<=rr&&color--)
		z0=pow(z0,3)+zc;
	return color;
}

int complex_f2(complexf z0,complexf zc,
			   int color,int rr)
{
	while(norm(z0)<=rr&&color--)
		z0=pow(z0,4)+zc;
	return color;
}

int complex_f3(complexf z0,complexf zc,
			   int color,int rr)
{
	while(norm(z0)<=rr&&color--)
		z0=pow(z0,5)+zc;
	return color;
}

struct IFS
{
	Vertex v;
	int iter;
	int n;
	float* m;
	float* rnd;
	IFS(){srand(time(0)); m=0; n=0; rnd=0;}
	~IFS(){clear();}
	void clear();
	int getrnd();
	void identity();
	void eval(Vertex*_v,int nv);
	void draw(uint l);
	void draw();
	void leaf();
	void tree();
	void fern();
	void dragon();
	void koh();
	void ghost();
};

int IFS::getrnd()
{
	float r=(float)(rand() % 10000)/100.0;
	for(int i=0;i<n-1;i++)
		if(r>=rnd[i]&&r<rnd[i+1])return i;
	return n-1;
}

void IFS::clear()
{
	if(n)
	{
		delete[]m; m=0;
		delete[]rnd; rnd=0;
		n=0;
	}
}

void IFS::identity()
{
	fill_n(m,16,0);
	m[0]=1;m[5]=1;m[10]=1;m[15]=1;
	for(int i=1;i<n;i++)
		copy(m,m+16,m+16*i);
}

void IFS::eval(Vertex*_v,int nv)
{
	int r;
	for(int i=0;i<nv;i++)
	{
		r=getrnd();
		v.mul_matrc(m+r*16);
		_v[i]=v;
	}
}

void IFS::draw(uint l)
{
	Mtransform M;
	int r;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for(int i=0;i<iter;i++)
	{
		glPushMatrix();
		glTranslatef(v.x,v.y,v.z);
		r=getrnd();
		M.mul(0,m+r*16,0);
		glMultMatrixf(M);
		glCallList(l);
		glPopMatrix();
	}
	glPopMatrix();
}

void IFS::draw()
{
	int r;
	glBegin(GL_POINTS);
	for(int i=0;i<iter;i++)
	{
		r=getrnd();
		v.mul_matrc(m+r*16);
		glVertex3fv(v);
	}
	glEnd();
}

void IFS::fern()
{
	clear();
	v.set(0,0,-20);
	iter=10000;
	n=4;
	m=new float[n*16];
	rnd=new float[n];
	identity();
	rnd[0]=0;rnd[1]=1;rnd[2]=86;rnd[3]=93;
	m[0]=0;        m[4]=0;        m[12]=0;
	m[1]=0;        m[5]=0.16;     m[13]=0;

	m[16+0]=0.85;  m[16+4]=0.04;  m[16+12]=0;
	m[16+1]=-0.04; m[16+5]=0.85;  m[16+13]=1.6;

	m[32+0]=0.2;   m[32+4]=-0.26; m[32+12]=0;
	m[32+1]=0.23;  m[32+5]=0.22;  m[32+13]=1.6;

	m[48+0]=-0.15; m[48+4]=0.28;  m[48+12]=0;
	m[48+1]=0.26;  m[48+5]=0.22;  m[48+13]=0.44;
}

void IFS::tree()
{
	clear();
	v.set(0,0,-2);
	iter=10000;
	n=4;
	m=new float[n*16];
	rnd=new float[n];
	identity();
	rnd[0]=0;rnd[1]=5;rnd[2]=45;rnd[3]=85;
	m[0]=0;        m[4]=0;        m[12]=0;
	m[1]=0;        m[5]=0.5;      m[13]=0;

	m[16+0]=0.42;  m[16+4]=-0.42; m[16+12]=0;
	m[16+1]=0.42;  m[16+5]=0.42;  m[16+13]=0.2;

	m[32+0]=0.42;  m[32+4]=0.42;  m[32+12]=0;
	m[32+1]=-0.42; m[32+5]=0.42;  m[32+13]=0.2;

	m[48+0]=0.1;   m[48+4]=0;     m[48+12]=0;
	m[48+1]=0;     m[48+5]=0.1;   m[48+13]=0.2;
}

void IFS::dragon()
{
	clear();
	v.set(0,0,-250);
	iter=10000;
	n=2;
	m=new float[n];
	identity();
	rnd[0]=0;rnd[1]=50;
	m[0]=-0.5;     m[4]=-0.5;     m[12]=49;
	m[1]=0.5;      m[5]=-0.5;     m[13]=12;

	m[16+0]=0.5;   m[16+4]=-0.5;  m[16+12]=-34;
	m[16+1]=0.5;   m[16+5]=0.5;   m[16+13]=-11;
}

void IFS::koh()
{
	clear();
	v.set(0,0,-90);
	iter=10000;
	n=4;
	m=new float[n*16];
	rnd=new float[n];
	identity();
	rnd[0]=0;rnd[1]=25;rnd[2]=50;rnd[3]=75;
	m[0]=0.31;      m[4]=0;         m[12]=4.12;
	m[1]=0;         m[5]=0.29;      m[13]=1.6;

	m[16+0]=0.19;   m[16+4]=-0.21;  m[16+12]=-0.69;
	m[16+1]=0.65;   m[16+5]=0.09;   m[16+13]=5.98;

	m[32+0]=0.19;   m[32+4]=0.21;   m[32+12]=0.67;
	m[32+1]=-0.65;  m[32+5]=0.09;   m[32+13]=5.96;

	m[48+0]=0.31;   m[48+4]=0;      m[48+12]=-4.14;
	m[48+1]=0;      m[48+5]=0.29;   m[48+13]=1.6;
}

void IFS::leaf()
{
	clear();
	v.set(0,0,-20);
	iter=10000;
	n=3;
	m=new float[n*16];
	rnd=new float[n];
	identity();
	rnd[0]=0;rnd[1]=6;rnd[2]=43;rnd[3]=79;
	m[0]=0.14;      m[4]=0.01;     m[12]=-0.08;
	m[1]=0;         m[5]=0.51;     m[13]=-1.31;

	m[16+0]=0.43;   m[16+4]=0.52;  m[16+12]=1.49;
	m[16+1]=-0.45;  m[16+5]=0.5;   m[16+13]=-0.75;

	m[32+0]=0.45;   m[32+4]=-0.49; m[32+12]=-1.62;
	m[32+1]=0.47;   m[32+5]=0.47;  m[32+13]=-0.74;

	m[48+0]=0.49;   m[48+4]=0;     m[48+12]=0.02;
	m[48+1]=0;      m[48+5]=0.51;  m[48+13]=1.62;
}

void IFS::ghost()
{
	clear();
	v.set(0,0,-20);
    iter=10000;
	n=3;
	m=new float[n*16];
	rnd=new float[n];
	identity();
	rnd[0]=0;rnd[1]=33;rnd[2]=66;
	m[0]=-0.5;      m[4]=-0.3;     m[12]=-6.07;
	m[1]=0.3;       m[5]=-0.5;     m[13]=-3.58;

	m[16+0]=-0.5;   m[16+4]=-0.3;  m[16+12]=5.92;
	m[16+1]=0.3;    m[16+5]=-0.5;  m[16+13]=3.5;

	m[32+0]=0;      m[32+4]=-0.59; m[32+12]=-0.09;
	m[32+1]=0.59;   m[32+5]=0;     m[32+13]=-0.04;
}

#endif