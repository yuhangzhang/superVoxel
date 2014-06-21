
#include "../nifticlib-2.0.0/niftilib/nifti1_io.h"

#include "../vxl-1.14.0/core/vgui/vgui.h"
#include "../vxl-1.14.0/core/vgui/vgui_viewer2D_tableau.h"
#include "../vxl-1.14.0/core/vgui/vgui_image_tableau.h"

#include "../vxl-1.14.0/core/vil/vil_image_view.h"

int main1(int argc, char * argv[])
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


	vil_image_view<vxl_byte> im(nim->dim[1],nim->dim[2]);

	for(int i=0;i<im.ni();i++)
	{
		for(int j=0;j<im.nj();j++)
		{
			im(i,j) = (mydata[i*im.nj()+j]-minvox)/maxgap*255;
			//printf("%d\n",im(i,j));
			//getchar();
		}
	}

	vgui_register_all_implementations();
	vgui::init(argc,argv);

	vgui_image_tableau_new image(im);
    vgui_viewer2D_tableau_new viewer(image);
    vgui::run(viewer, image->width()+100, image->height()+100);



	nifti_image_free( nim );


	return 0;
}

