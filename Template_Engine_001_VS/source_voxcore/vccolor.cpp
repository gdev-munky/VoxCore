#include <algorithm>
#include "vccolor.h"
using namespace std;

vccolor_t::vccolor_t()
{
	c = new float[3];
	c[0] = c[1] = c[2] = 1.0f;//white. Need black?
	alpha = 1.0f;
	fixIt();
}

vccolor_t::vccolor_t(float* color)
{
	c = color;
	alpha = 1.0f;
	fixIt();
}

vccolor_t::vccolor_t(float* color, float a)
{
	c = color;
	alpha = a;
	fixIt();
}

vccolor_t::vccolor_t(float r, float g, float b, float a)
{
	float arr[3] = {r, g, b};
	c = arr;
	alpha = a;
	fixIt();
}

void vccolor_t::fixIt()
{
	alpha = min(1.0f, max(0.0f, alpha));
	c[0] = min(1.0f, max(0.0f, c[0]));
	c[1] = min(1.0f, max(0.0f, c[1]));
	c[2] = min(1.0f, max(0.0f, c[2]));
}

void vccolor_t::sumLights(vccolor_t& other)
{	
	alpha += other.alpha;
	float alphaSum = alpha > 1.0f ? alpha : 1.0f;
	c[0] = min(1.0f, max(0.0f, (c[0]*alpha + other.c[0]*other.alpha)/alphaSum));
	c[1] = min(1.0f, max(0.0f, (c[1]*alpha + other.c[1]*other.alpha)/alphaSum));
	c[2] = min(1.0f, max(0.0f, (c[2]*alpha + other.c[2]*other.alpha)/alphaSum));
}

void vccolor_t::luminate(vccolor_t& other)
{	
	c[0] *= other.c[0]*other.alpha;
	c[1] *= other.c[1]*other.alpha;
	c[2] *= other.c[2]*other.alpha;
	fixIt();
}