#ifndef _HEADER_VCCOLOR_
#define _HEADER_VCCOLOR_

//����
struct vccolor_t
{
private:
	//�������� ��� ��������� � ���� [0..1] (���������)
	inline void fixIt();
public:
	//���� RGB [0..1]
	float* c;
	//������������ ����������� ��� �������� ����� [0..1] ��� [0..inf]
	float alpha;

	vccolor_t();
	vccolor_t(float* color);
	vccolor_t(float* color, float a);
	vccolor_t(float r, float g, float b, float a);
	vccolor_t(float r, float g, float b);

	//���������� ��� �����, ����������� ����
	void sumLights(vccolor_t& other);

	//��������� ���� �����������, �������� ��������� ��������� other
	void luminate(vccolor_t& other);	
	static vccolor_t BLACK;
	static vccolor_t WHITE;
	static vccolor_t RED;
	static vccolor_t GREEN;
	static vccolor_t BLUE;
};


#endif