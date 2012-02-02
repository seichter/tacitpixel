#include <tp/vec.h>
#include <tp/logutils.h>



int main(int argc,char* argv[])
{

//	tpVec3<float> v1(1,2,3);
//	tpVec3<float> v2(2,4,6);


//	tpVec3<float> v3(v1-v2);

//	tpLog::get() << v1 << v2 << v3;

	tpVec3r v1(1.f,2.f,3.f);
	tpVec3r v2(2,4,6);

	printf("%3.3f %3.3f %3.3f",v1[0],v1[1],v1[2]);

	tpVec3r v3(v1-v2);

	tpLog::get() << v1 << v2 << v3;


	return 0;
}
