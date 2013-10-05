#ifndef _array3d_
#define _array3d_

template< typename T >
struct Array3d
{
private:
	T*** arr;
	int width, length, height;
public:
	void setup(int w, int l, int h) 
	{
		arr = new T**[ w ];
		int x, y;
		for (x = 0; x < w; x++)
		{
			arr[x] = new T*[ l ];
			for (y = 0; y < l; y++)
				arr[x][y] = new T[ h ];
		}
	}
	Array3d() {}
	Array3d(int w, int l, int h) 
	{
		setup(w,l,h);
	}
	~Array3d()
	{
		int x, y;
		for (x = 0; x < width; x++)
		{
			for (y = 0; y < length; y++)
				delete[] arr[x][y];
			delete[] arr[x];
		}
	}

	//UNSAFE CALL!
	T operator()(int x, int y, int z)
	{
		return arr[x][y][z];
	}

	inline int Width()	{	return width;	}
	inline int Length()	{	return length;	}
	inline int Heght()	{	return height;	}
};
#endif