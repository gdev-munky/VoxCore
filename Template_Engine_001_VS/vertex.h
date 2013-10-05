#ifndef _Vertex_
#define _Vertex_
#define toRadian 0.01745329251f // pi/180.0 //WHY NOT?
#define toDegree 57.2957795131f // 180.0/pi //WHY NOT?

struct Vertex
{
	float x,y,z;	
	void set(float xx=0, float yy=0, float zz=0)
	{x=xx;y=yy;z=zz;}
	void set(const Vertex&a) {set(a.x,a.y,a.z);}
	void set(float*v) {set(v[0],v[1],v[2]);}
	Vertex() {set(0,0,0);}
	Vertex(float xx, float yy, float zz) {set(xx,yy,zz);}
	Vertex(float*v) {set(v);}
	Vertex(Vertex& a) {set(a);}
	~Vertex(){;}
	void round() {x=(int)x;y=(int)y;z=(int)z;}
	//TODO: MUNKY: inline?
	void add(Vertex&a) {x+=a.x;y+=a.y;z+=a.z;}
	//TODO: MUNKY: inline?
	void sub(Vertex&a) {x-=a.x;y-=a.y;z-=a.z;}
	//TODO: MUNKY: inline?
	void mul(float a) {x*=a;y*=a;z*=a;}
	//TODO: MUNKY: inline?
	void div(float a) {x/=a;y/=a;z/=a;}
	//TODO:MUNKY: RENAME negate(); also: operator-(Vertex& a)
	void uminus() {x=-x;y=-y;z=-z;}	
	void scale(float xx,float yy, float zz)
		{ x*=xx; y*=yy; z*=zz; }
	void mid(Vertex&a,Vertex&b)
	        {x=(b.x+a.x)/2; y=(b.y+a.y)/2; z=(b.z+a.z)/2;}
	void mirror(Vertex a)
	           {a.sub(*this); a.mul(2); add(a);}
	void rotz(float angle)
	{
		angle*=toRadian;
		float xx=x*cos(angle)-y*sin(angle);
		float yy=x*sin(angle)+y*cos(angle);
		x=xx;y=yy;
	}
    void move(float l,float xa, float ya)
	{
		float k;
		Vertex tmp;
		xa*=toRadian;
		ya*=toRadian;
		k=cos(xa);
		tmp.set(-sin(ya)*k,sin(xa),cos(ya)*k);
		tmp.mul(l);
		add(tmp);
	}
	float len() {return sqrt(x*x+y*y+z*z);}
	//MUNKY: Пояснение lenSq() - нам не всегда нужно извлекать квадрат (в частности для сравнения длинн)
	float lenSq() {return sqrt(x*x+y*y+z*z);}
	float cosX() {return x/len();}
	float cosY() {return y/len();}
	float cosZ() {return z/len();}
	//TODO:MUNKY:  Alternate: operator-- (returns new normalized vector)
	void normalize()
	{float l=sqrt(x*x+y*y+z*z);x/=l;y/=l;z/=l;}
	int iscolinear(Vertex&a)
	{return (x/a.x==y/a.y)&&(y/a.y==z/a.z);}
	float mul_scalar(Vertex&a)
	{return x*a.x+y*a.y+z*a.z;}
	float mul_vector(Vertex&a)
	{float xx=y*a.x-z*a.y,yy=z*a.x-x*a.z, zz=x*a.y-y*a.x; x=xx;y=yy;z=zz;}
	float cos_vertex(Vertex&a)
	{return mul_scalar(a)/(len()*a.len());}
	int is_onside(Vertex&a,float*plane)
	{
		float is1=plane[0]*x+plane[1]*y+plane[2]*z+plane[3];
		float is2=plane[0]*a.x+plane[1]*a.y+plane[2]*a.x+plane[3];
		return (is1*is2>0)?1:0; 
	}

	void mul_matrl(float*m)
	{
		float xx=x*m[0]+y*m[1]+z*m[2]+m[3];
		float yy=x*m[4]+y*m[5]+z*m[6]+m[7];
		float zz=x*m[8]+y*m[9]+z*m[10]+m[11];
        x=xx;y=yy;z=zz;
	}

	void mul_matrc(float*m)
	{
		float xx=x*m[0]+y*m[4]+z*m[8]+m[12];
		float yy=x*m[1]+y*m[5]+z*m[9]+m[13];
		float zz=x*m[2]+y*m[6]+z*m[10]+m[14];
        x=xx;y=yy;z=zz;
	}

	operator float*() {return (float*)this;}
	void operator=(const Vertex& a){set(a);}
	Vertex operator*(float a)
	{Vertex temp(*this);temp.mul(a);return temp;}
	Vertex operator/(float a)
	{Vertex temp(*this);temp.div(a);return temp;}
	void operator+=(Vertex&a) {add(a);}
	void operator-=(Vertex&a) {sub(a);}
	void operator*=(float a) {mul(a);}
	void operator/=(float a) {div(a);}
	int operator==(Vertex&a)
	{
		return equal((float*)this,(float*)this+3,(float*)a);
	}
};

static Vertex operator+ (const Vertex&a,const Vertex&b)
{return Vertex(b.x+a.x,b.y+a.y,b.z+a.z);}
static Vertex operator- (const Vertex&b,const Vertex&a)
{return Vertex(b.x-a.x,b.y-a.y,b.z-a.z);}
static Vertex operator* (const Vertex&a,const Vertex&b)
{return Vertex(b.x*a.x,b.y*a.y,b.z*a.z);}
static Vertex operator/ (const Vertex&b,const Vertex&a)
{return Vertex(b.x/a.x,b.y/a.y,b.z/a.z);}


#endif