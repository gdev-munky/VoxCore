#ifndef _HEADER_VOXEL_
#define _HEADER_VOXEL_

class Voxel
{
private:	
	//Накопитель для освещения для каждой стороны
	vccolor_t lum_acc[6];
	
	//Рассчитанные цвета поверхностей (call lightCalc();)
	vccolor_t calcC[6];
public:
	//Базовый диффузный цвет вокселя
	vccolor_t dif_color;
	Vertex mid, cmin, cmax;
	float size, hsize;
	
	//Уровень в дереве (чем больше - тем меньше размеры)
	int level;

	//Координаты этого вокселя у родителя
	int prntX, prntY, prntZ;

	//Не менять извне! Является ли данный воксель листом (уровень максимален?)
	bool isLeaf;

	//Родитель (контейнер)
	Voxel* prnt;
	//Дети (содержимое)
	Array3d<Voxel*> chld;
	//Соседние воксели (use neighbour(-1..1, -1..1, -1..1))
	Array3d<Voxel*> nghb;
	Voxel* neighbour(int x, int y, int z);

	Voxel() {};
	Voxel(Voxel* p, int depth, int myX, int myY, int myZ, int levl);
	~Voxel();

	//Цвет стороны вокселя с учетом освещения (first call lightCalc(); once a frame)
	inline float* getColor(int side) { return calcC[side].c; }

	//Рассчитать воздействие источника света
	void lightAdd(vccolor_t light, Vertex dir, float dist);
	//Рассчитать воздействие абсолютного источника света (солнечного или лунного)
	inline void lightAdd(vccolor_t light, Vertex dir) { lightAdd(light, dir, 1.0f); }

	//Умножает диффузный цвет на аккумулированный свет источников света
	void lightCalc();

};

#endif