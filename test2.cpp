
#include "../nifticlib-2.0.0/niftilib/nifti1_io.h"

#include "../vxl-1.14.0/core/vgui/vgui.h"
#include "../vxl-1.14.0/core/vgui/vgui_viewer2D_tableau.h"
#include "../vxl-1.14.0/core/vgui/vgui_image_tableau.h"

#include "../vxl-1.14.0/core/vil/vil_image_view.h"

#include "../supervoxel/superVoxel.h"

int main(int argc, char * argv[])
{
	nifti_image * nim =  nifti_image_read("C:\\cygwin64\\home\\Yuhang\\visceral\\visceral-dataset\\trainingset\\volumes\\temp\\10000014_1_CT_wb.nii",1);

	printf("ndim=%d\n",nim->ndim);

	for(int i=0;i<=nim->ndim;i++)
	{
		printf("%d\n",nim->dim[i]);
	}

	printf("nvox=%d\n",nim->nvox);
	printf("nbyper=%d\n",nim->nbyper);
	printf("datatype=%d\n",nim->datatype);
	printf("cal_min=%f\n",nim->cal_min);
	printf("cal_max=%f\n",nim->cal_max);
	//getchar();

	float * mydata = (float*)nim->data;

	float minvox,maxvox;

	minvox = mydata[0];
	maxvox = mydata[0];

	for(int i=0; i<nim->nvox ;i++)
	{
		if(minvox>=mydata[i]) minvox=mydata[i];
		if(maxvox<=mydata[i]) maxvox=mydata[i];
	}

	float maxgap = maxvox-minvox;


	vil_image_view<float> im(nim->dim[1],nim->dim[2],1);

	for(int i=0;i<im.ni();i++)
	{
		for(int j=0;j<im.nj();j++)
		{
			for(int k=0;k<im.nplanes();k++)
			{
				im(i,j,k) = (mydata[k*im.ni()*im.nj()+i*im.nj()+j]-minvox)/maxgap;
				//if(i%20==1) im(i,j,k)=im(i-1,j,k);
				//else im(i,j,k)=1;
			}
		}
	}

	printf("here\n");

	//getchar();

	superVoxel<float> sv(im,20);

	printf("here2\n");

	vil_image_view<vxl_byte> im2(nim->dim[1],nim->dim[2],3);

	for(int i=0;i<im2.ni()-1;i++)
	{
		for(int j=0;j<im2.nj()-1;j++)
		{
			for(int k=0;k<3;k++)
			{
				im2(i,j,k) = im(i,j,0);				
			}
			if(sv.getLabel(i,j,0)!=sv.getLabel(i+1,j,0)
				||sv.getLabel(i,j,0)!=sv.getLabel(i,j+1,0))
			{
				im2(i,j,0)=200;
				im2(i,j,1)=200;
				im2(i,j,2)=0;
			}
		}
	}


	vgui::init(argc,argv);

	vgui_image_tableau_new image(im2);
    vgui_viewer2D_tableau_new viewer(image);
    vgui::run(viewer, image->width()+100, image->height()+100);



	nifti_image_free( nim );


	return 0;
}

