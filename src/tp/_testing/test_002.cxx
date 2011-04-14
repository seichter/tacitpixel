
#include <tp/factory.h>
#include <tp/node.h>
#include <tp/log.h>
#include <tp/image.h>
#include <tp/imageop.h>

int main(int argc, char* argv[])
{
	
	tpRefPtr<tpImage> img = tpImage::read(argv[1]);
	
	if (img.isValid())
	{
		tpLogMessage("Image read %dx%d",img->getWidth(),img->getHeight());
		tpLogMessage("Image write %d", img->write("lena_converted.jpg"));
		
		tpRefPtr<tpImage> grey = new tpImage();
		grey->allocate(img->getWidth(),img->getHeight(),TP_GREY8);
		
		tpImageOperator::o(tpImageOperator::kGrey)(img.get(),grey.get());
		tpLogMessage("Image write (grey) %d", grey->write("lena_grey.jpg"));
		
		
	}
	
	return 0;
}