

#include <tp/array.h>
#include <tp/autoptr.h>
#include <tp/chunk.h>
#include <tp/vec.h>
#include <tp/refptr.h>

int main(int argc, char* argv[])
{
	
	tpChunk<int> chunk;
	tpVec4<float> vec4;
	
	tpArray<int> i;
	i.add(10);
	
	tpRefPtr<tpReferenced> r(new tpReferenced());
	
	return 0;
}