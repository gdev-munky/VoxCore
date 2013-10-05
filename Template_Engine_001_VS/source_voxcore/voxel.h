#ifndef _HEADER_VOXEL_
#define _HEADER_VOXEL_

class Voxel
{
private:	
	//���������� ��� ��������� ��� ������ �������
	vccolor_t lum_acc[6];
	
	//������������ ����� ������������ (call lightCalc();)
	vccolor_t calcC[6];
public:
	//������� ��������� ���� �������
	vccolor_t dif_color;
	Vertex mid, cmin, cmax;
	float size, hsize;
	
	//������� � ������ (��� ������ - ��� ������ �������)
	int level;

	//���������� ����� ������� � ��������
	int prntX, prntY, prntZ;

	//�� ������ �����! �������� �� ������ ������� ������ (������� ����������?)
	bool isLeaf;

	//�������� (���������)
	Voxel* prnt;
	//���� (����������)
	Array3d<Voxel*> chld;
	//�������� ������� (use neighbour(-1..1, -1..1, -1..1))
	Array3d<Voxel*> nghb;
	Voxel* neighbour(int x, int y, int z);

	Voxel() {};
	Voxel(Voxel* p, int depth, int myX, int myY, int myZ, int levl);
	~Voxel();

	//���� ������� ������� � ������ ��������� (first call lightCalc(); once a frame)
	inline float* getColor(int side) { return calcC[side].c; }

	//���������� ����������� ��������� �����
	void lightAdd(vccolor_t light, Vertex dir, float dist);
	//���������� ����������� ����������� ��������� ����� (���������� ��� �������)
	inline void lightAdd(vccolor_t light, Vertex dir) { lightAdd(light, dir, 1.0f); }

	//�������� ��������� ���� �� ���������������� ���� ���������� �����
	void lightCalc();

};

#endif