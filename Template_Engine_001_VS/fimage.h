#ifndef _Fimage_
#define _Fimage_

struct Fimage
{
	int xlen,ylen;
	int len;
	vector<float*> channel;
	Fimage() {xlen=0;ylen=0;len=0;}
	Fimage(int _xlen,int _ylen,int nc)
	{init (_xlen,_ylen,nc);}
	~Fimage(){clear();}
	void init(int _xlen,int _ylen,int nc);
	void clear();
	void toch(int snc,int dnc)
	{copy(channel[snc],channel[snc]+len,channel[dnc]);}
	void tochrgb(int r1,int g1,int b1,
		         int r2,int g2,int b2)
	{toch(r1,r2); toch(g1,g2); toch(b1,b2);}
	void tochrgb(int r1,int g1,int b1,int a1,
		         int r2,int g2,int b2,int a2)
	{tochrgb(r1,g1,b1,r2,g2,b2); toch(a1,a2);}
	void fill(int nc,float v)
	{fill_n(channel[nc],len,v);}
	void fillrgb(int r,int g,int b,float v)
	{fill(r,v); fill(g,v); fill(b,v);}
	void fillrgba(int r,int g, int b,int a,float v)
	{fillrgb(r,g,b,v); fill(a,v);}
	void tored(int nc);
	void togreen(int nc);
	void toblue(int nc);
	void toalpha(int nc);
	void torgb(int r=0,int g=1,int b=2)
	{tored(r); togreen(g); toblue(b);}
	void torgba(int r=0,int g=1,int b=2,int a=3)
	{torgb(r,g,b); toalpha(a);}
	void tomono(int r=0,int g=1,int b=2);
	template<class T>
	void loadrgb(T*src,int t=0,int g=1,int b=2);
	void mul(float v,int nc)
	{
		Array2d tmp(xlen);
		for_each(multiplies<float>(),v,xlen*ylen,1,channel[nc],tmp);
	}
	void mulrgb(float v,int r=0,int g=1,int b=2)
	{mul(v,r); mul(v,g); mul(v,b);}
	void add(float v,int nc)
	{
		Array2d tmp(xlen);
		for_each(plus<float>(),v,xlen*ylen,1,channel[nc],tmp);
	}
	void addrgb(float v,int r=0,int g=1,int b=2)
	{add(v,r); add(v,g); add(v,b);}
	void filter(int nc,int dstnc, float*m,int mlen,
		        float _div=0,float _add=0);
	void filterrgb(int r1,int g1,int b1,
		           int r2,int g2,int b2,
				   float*m,int mlen,float _div=0,
				   float _add=0)
	{
		filter(r1,r2,m,mlen,_div,_add);
		filter(g1,g2,m,mlen,_div,_add);
		filter(b1,b2,m,mlen,_div,_add);
	}
	float* operator[] (int nc) {return channel[nc];}
};

template<class T>
void Fimage::loadrgb(T*src,int r,int g,int b)
{
	int len=xlen*ylen;
	Array2d asrc(len*3,0,0,3,1);
	Array2d afi(len,0,0,1,1);
	copy(len,1,src,channel[r],asrc,afi);
	asrc.ox=1;
	copy(len,1,src,channel[g],asrc,afi);
	asrc.ox=2;
	copy(len,1,src,channel[b],asrc,afi);
}

void Fimage::filter(int nc,int dstnc,float*m,
					int mlen,float _div,float _add)
{
	int ind=mlen/2;
	float newcolor;
	if(_div==0) _div=accumulate(m,m+mlen*mlen,0);
	if(_div==0) _div=1;
	Array2d ach(xlen);
	Array2d atmp(mlen);
	for(int j=0;j<ylen-mlen;j++)
		for(int i=0;i<xlen-mlen;i++)
		{
			ach.setorg(i,j);
			newcolor=accum(multiplies<float>(),plus<float>(),
				           0.0f,mlen,mlen,channel[nc],
						   m,ach,atmp)/_div+_add;
			channel[dstnc][ach.getind(ind,ind)]=newcolor;
		}
}

void Fimage::tomono(int r,int g,int b)
{
	for(int i=0;i<len;i++)
	{
		channel[r][i]=(channel[r][i]+channel[g][i]+channel[b][i])/3;
		channel[g][i]=channel[r][i];
		channel[b][i]=channel[r][i];
	}
}

void Fimage::tored(int nc)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColorMask(1,0,0,0);
	glDrawPixels(xlen,ylen,GL_RED,GL_FLOAT,channel[nc]);
	glPopAttrib();
}

void Fimage::togreen(int nc)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColorMask(0,1,0,0);
	glDrawPixels(xlen,ylen,GL_GREEN,GL_FLOAT,channel[nc]);
	glPopAttrib();
}

void Fimage::toblue(int nc)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColorMask(0,0,1,0);
	glDrawPixels(xlen,ylen,GL_BLUE,GL_FLOAT,channel[nc]);
	glPopAttrib();
}

void Fimage::toalpha(int nc)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColorMask(0,0,0,1);
	glDrawPixels(xlen,ylen,GL_ALPHA,GL_FLOAT,channel[nc]);
	glPopAttrib();
}

void Fimage::clear()
{
	for(uint i=0;i<channel.size();i++)
		delete[]channel[i];
	channel.clear();
}

void Fimage::init(int _xlen,int _ylen,int nc)
{
	clear();
	xlen=_xlen; ylen=_ylen; len=xlen*ylen;
	channel.resize(nc);
	for(uint i=0;i<channel.size();i++)
		channel[i]=new float[len];
}

#endif