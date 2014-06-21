#include "superVoxel.h"

#ifndef _SUPERVOXEL_CPP_
#define _SUPERVOXEL_CPP_

template<class T>
double superVoxel<T>::cost(double v1, double v2)
{
	double tempf=fabs(v1-v2);

	if(tempf>0.01) tempf=0.02;
	//else if(tempf>0.002) tempf = 0.2;
	else if(tempf>0.005) tempf = 0.5;
	else if(tempf>0.002) tempf = 0.8;
	else tempf=1;
	//printf("%f %f %f\n",v1,v2,tempf);
	
	return tempf;
}

template<class T>
superVoxel<T>::superVoxel(vil_image_view<T>& im, int voxelsize)
{
	_width=im.ni();
	_height=im.nj();
	_layers=im.nplanes();

	printf("%d %d %d\n",_width,_height,_layers);


	volumeElimination ve(_width,_height,_layers);

	for(int i=0;i<_width;i++)
	{		
		for(int j=0;j<_height;j++)
		{
			for(int k=0;k<_layers;k++)
			{
				//printf("%d %d\n",i,_width-1);//getchar();
				if(i!=_width-1)
				{
					double tempd = cost(im(i,j,k),im(i+1,j,k));

					if(i%voxelsize==0) 
					{
						//printf("%d\n",i);
						ve.addEdgeterm(i,j,k,i+1,j,k,tempd);
					}
					else if((voxelsize/2+i)%voxelsize==0)
					{
						//printf("=%d\n",i);
						ve.addDataterm(i,j,k,-tempd);
						ve.addDataterm(i+1,j,k,-tempd);	
						ve.addEdgeterm(i,j,k,i+1,j,k,tempd);
					}
					ve.addDataterm(i,j,k,tempd);
					ve.addDataterm(i+1,j,k,tempd);			
					ve.addEdgeterm(i,j,k,i+1,j,k,tempd*-2);	
				}



				if(j!=_height-1)
				{
					double tempd = cost(im(i,j,k),im(i,j+1,k));
					ve.addDataterm(i,j,k,tempd);
					ve.addDataterm(i,j+1,k,tempd);
					ve.addEdgeterm(i,j,k,i,j+1,k,tempd*-2);
				}
				if(k!=_layers-1)
				{
					double tempd = cost(im(i,j,k),im(i,j,k+1));
					ve.addDataterm(i,j,k,tempd);
					ve.addDataterm(i,j,k+1,tempd);
					ve.addEdgeterm(i,j,k,i,j,k+1,tempd*-2);
				}
			}
		}
	}
	


	ve.minimize();
	printf("energy=%f\n",ve.energy());

	_label=ve.getLabel();

	printf("done\n");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	volumeElimination ve2(_width,_height,_layers);

	for(int i=0;i<_width;i++)
	{		
		for(int j=0;j<_height;j++)
		{
			for(int k=0;k<_layers;k++)
			{
				//printf("%d %d\n",i,_width-1);//getchar();
				if(i!=_width-1)
				{
					double tempd = cost(im(i,j,k),im(i+1,j,k));
					ve2.addDataterm(i,j,k,tempd);
					ve2.addDataterm(i+1,j,k,tempd);			
					ve2.addEdgeterm(i,j,k,i+1,j,k,tempd*-2);	
				}



				if(j!=_height-1)
				{
					double tempd = cost(im(i,j,k),im(i,j+1,k));
					if(j%voxelsize==0) 
					{
						//printf("%d\n",i);
						ve2.addEdgeterm(i,j,k,i,j+1,k,tempd);
					}
					else if((voxelsize/2+j)%voxelsize==0)
					{
						//printf("=%d\n",i);
						ve2.addDataterm(i,j,k,-tempd);
						ve2.addDataterm(i,j+1,k,-tempd);	
						ve2.addEdgeterm(i,j,k,i,j+1,k,tempd);
					}
					ve2.addDataterm(i,j,k,tempd);
					ve2.addDataterm(i,j+1,k,tempd);
					ve2.addEdgeterm(i,j,k,i,j+1,k,tempd*-2);
				}

				if(k!=_layers-1)
				{
					double tempd = cost(im(i,j,k),im(i,j,k+1));
					ve2.addDataterm(i,j,k,tempd);
					ve2.addDataterm(i,j,k+1,tempd);
					ve2.addEdgeterm(i,j,k,i,j,k+1,tempd*-2);
				}
			}
		}
	}
	


	ve2.minimize();
	printf("energy=%f\n",ve2.energy());

	for(int i=0;i<_width;i++)
	{		
		for(int j=0;j<_height;j++)
		{
			for(int k=0;k<_layers;k++)
			{
				_label[i][j][k]+=ve2.getLabel(i,j,k)*2;
			}
		}
	}



	return;
}


template<class T>
bool superVoxel<T>::getLabel(int i, int j, int k)
{
	return _label[i][j][k];
}









#endif