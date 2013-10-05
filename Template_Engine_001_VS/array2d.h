#ifndef _array2d_
#define _array2d_
struct Array2d
{
	int xlen;
	int sx, sy;
	int ox, oy;
	int ind;
    Array2d (int xl, int _ox=0, int _oy=0,
		     int _sx=1, int _sy=1)
	  {init(xl, _ox, _oy, _sx, _sy);}
	void init(int xl,int _ox=0, int _oy=0,
		      int _sx=1, int _sy=1);
	int getind(int x, int y)
	    {return (y*sy+oy)*xlen+x*sx+ox;}
    void setind(int x, int y)
	{ind = (y*sy+oy)*xlen+x*sx+ox;}
	void setorg(int x, int y){ox=x; oy=y;}
	void setscale(int _sx, int _sy){sx=_sx; sy=_sy;}
	int operator ++(int i) {return (ind+=sx)-sx;}
	operator int() {return ind;}
    };

    void Array2d::init(int xl, int _ox, int _oy,
		               int _sx, int _sy)
	{
		xlen = xl;
		ox=_ox; oy=_oy;
		sx=_sx; sy=_sy;
		ind=0;
	}

	template<class Src, class Dst>
	void copy(int xl, int yl, Src src, Dst dst,
		      Array2d& sind, Array2d& dind)
	{
		for(int j=0; j<yl; j++)
		{
			sind.setind(0,j);
			dind.setind(0,j);
			for(int i=0; i<xl;i++)
				dst[dind++]=src[sind++];
		}
	}

	template<class Dst, class T>
	void fill(T v, int xl, int yl, Dst dst, Array2d&dind)
	{
		for(int j=0; j<yl;j++)
		{
			dind.setind(0,j);
			for(int i=0; i<xl; i++)
				dst[dind++]=v;
		}
	}

	template<class Dst, class T, class T1>
	void for_each(T f, T1 arg, int xl,
		          int yl, Dst dst, Array2d&dind)
	{
		for(int j=0; j<yl; j++)
		{
			dind.setind(0,j);
			for(int i=0; i<xl; i++)
				dst[dind]=f(dst[dind++], arg);
		}
	}

	template<class Src, class T, class Tf, class Tfa>
	T accum(Tf f, Tfa fa, T init, int xl, int yl,
		    Src src1, Src src2, Array2d& ind1,
			Array2d& ind2)
	{
		for(int j=0; j<yl; j++)
		{
			ind1.setind(0,j);
			ind2.setind(0,j);
			for(int i=0; i<xl; i++)
				init=fa(init, f(src1[ind1++],src2[ind2++]));
		}
    return init;
	}

#endif