#include <algorithm>
using namespace std;
#include "..\vertex.h"
#include "vccolor.h"
#include "..\array3d.h"
#include "voxel.h"

Voxel* Voxel::neighbour(int x, int y, int z)
{
	x += 1;		y += 1;		z += 1;
	return nghb(x, y, z);
}

Voxel::Voxel(Voxel* p, int depth, int myX, int myY, int myZ, int levl)
{
	prnt = p;
	prntX = myX;
	prntY = myY;
	prntZ = myZ;
	level = levl;
	nghb = Array3d<Voxel*>(3, 3, 3);
	if (levl == depth)
	{
		isLeaf = true;
	}
	else
	{

	}
}