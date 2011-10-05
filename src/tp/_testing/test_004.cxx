
#include <tp/refptr.h>
#include <tp/image.h>
#include <tp/library.h>

int main(int argc,char* argv[])
{

	tpLibrary::load("tacit_jpg");
	tpLibrary::load("tacit_png");

	tpRefPtr<tpImage> img = tpImage::read(argv[1]);
	img->write(argv[2]);

	return 0;
}
