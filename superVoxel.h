#include "../volumeElimination/volumeElimination.h"
#include <vil/vil_image_view.h>

#ifndef _SUPERVOXEL_H_
#define _SUPERVOXEL_H_

template<class T>
class superVoxel
{
public:
	superVoxel(vil_image_view<T> &im, int voxelsize);
	bool getLabel(int i, int j, int k);
private:
	int _width;
	int _height;
	int _layers;
	volumeElimination::vector3i _label;
	double superVoxel<T>::cost(double v1, double v2);
};

#include "superVoxel.cpp"

#endif