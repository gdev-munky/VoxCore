#ifndef _HEADER_VCCOLOR_
#define _HEADER_VCCOLOR_

//Цвет
struct vccolor_t
{
private:
	//приводит все параметры к виду [0..1] (отсечение)
	inline void fixIt();
public:
	//цвет RGB [0..1]
	float* c;
	//прозрачность поверхности или мощность света [0..1] или [0..inf]
	float alpha;

	vccolor_t();
	vccolor_t(float* color);
	vccolor_t(float* color, float a);
	vccolor_t(float r, float g, float b, float a);
	vccolor_t(float r, float g, float b);

	//Складывает два цвета, описывающие свет
	void sumLights(vccolor_t& other);

	//Вычисляет цвет поверхности, учетывая параметры освещения other
	void luminate(vccolor_t& other);	
	static vccolor_t BLACK;
	static vccolor_t WHITE;
	static vccolor_t RED;
	static vccolor_t GREEN;
	static vccolor_t BLUE;
};


#endif