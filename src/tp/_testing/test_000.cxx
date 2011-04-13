

#include <tp/array.h>
#include <tp/scopeptr.h>
#include <tp/chunk.h>
#include <tp/vec.h>
#include <tp/refptr.h>
#include <tp/string.h>
#include <tp/fixed.h>
#include <tp/log.h>
#include <tp/node.h>
#include <tp/matrix.h>
#include <tp/map.h>
#include <tp/file.h>

class tpTestObject : public tpNode {
public:
	tpTestObject() { tpLogMessage("Constructor"); }
protected:
	~tpTestObject() { tpLogMessage("Destructor"); }
};


int main(int argc, char* argv[])
{
	
	// Chunk
	tpChunk chunk;
	chunk.reserve<float>(2);
	
	chunk.at<float>(0) = 23.5f;
	tpLogMessage("23.5f = %3.1ff",chunk.at<float>(0));
	
	chunk.at<float>(1) = 50.0f;
	tpLogMessage("50.0f = %3.1ff = %3.1ff",chunk.at<float>(1),chunk.ptr<float>()[1]);
	
	
	// Array
	tpArray<int> iv;
	iv.add(10);
	iv.add(20);
	
	tpLogMessage("Array > size, capacity, element_size, max_capacity (%d,%d,%d,%d)",iv.getSize(),iv.getCapacity(),iv.element_size,iv.max_capacity);


	// Map
	tpPair<int,tpString> num_pair;
	
	tpMap<int,tpString> numbers;
	
	numbers.add(1,"one");
	numbers.add(2,"two");
	numbers.add(3,"three");
	numbers.add(4,"four");
	
	for(tpMap<int,tpString>::iterator iter = numbers.begin(); iter != numbers.end(); ++iter)
	{
		tpLogMessage("%d = %s",(*iter).getKey(), (*iter).getValue().c_str());
	}
	
	
	tpString hello("hello");
	tpLogMessage("hello = %s (%d)",hello.c_str(),hello.getLength());
	
	hello.empty();
	tpLogMessage("hello = '%s' (%d) (empty)",hello.c_str(),hello.getLength());
	
	hello.append("hello");
	tpLogMessage("hello = '%s' (%d) (append hello)",hello.c_str(),hello.getLength());
	
	hello.append(" world");
	tpLogMessage("hello = '%s' (%d) (append ' world')",hello.c_str(),hello.getLength());
	
	hello.prepend("this ");
	tpLogMessage("hello = '%s' (%d) (prepend 'this ')",hello.c_str(),hello.getLength());
	
	tpString nothing;
	tpLogMessage("nothing = '%s' (%d)",nothing.c_str(),nothing.getLength());
	
	
//	printf("sizeof(char)(%d) != sizeof(wchar_t)(%d)",sizeof(char),sizeof(wchar_t));
	
//	tpString hello_wide(L"hello wide");
//	printf("hello wide = '%ls' (%d)",hello_wide.c_str(),hello_wide.getLength());
	
	tpString number("45.2344f");
	printf("number: %d %3.3f",number.to<int>(),number.to<float>());
	
	tpVec4<float> vec4;
	
	tpFixed32 fix32; 
	fix32.set(56.0);
	
	tpLogMessage("Aligns: %d %d %d",fix32._bits,fix32._bits_half,fix32._bits_half_val);
	
	tpLogMessage("Fixed > Double %d = %3.3lf",fix32.getX(),fix32.getDouble());
	tpLogMessage("Fixed > Float %d = %3.3f",fix32.getX(),fix32.getFloat());
	

	fix32.set(56);
	tpLogMessage("Fixed > int %d = %d",fix32.getX(),fix32.getInt());
	
	fix32.set(56.23);
	tpLogMessage("Fixed > int (rounded from 56.23) %d = %d",fix32.getX(),fix32.getIntRounded());
	fix32.set(56.78);
	tpLogMessage("Fixed > int (rounded from 56.78) %d = %d",fix32.getX(),fix32.getIntRounded());
	
	fix32.mul(tpFixed32(3.1)).add(tpFixed32(10));
	tpLogMessage("Fixed > float 56.78 * 3.1 + 10 (186.018)  %d = %f",fix32.getX(),fix32.getFloat());
	
	fix32.div(tpFixed32(2)).sub(tpFixed32(10));
	tpLogMessage("Fixed > float / 2 (186.018 / 2 - 10)  %d = %f",fix32.getX(),fix32.getFloat());
	
	fix32.set(25); fix32 = sqrt(fix32);
	tpLogMessage("Fixed > sqrt 25  %d = %f",fix32.getX(),fix32.getDouble());
	
	
	fix32.set(0);
	tpLogMessage("Fixed > (acos,cos,asin,sin) 0 (%f,%f,%f,%f)",
		acos(fix32).getFloat(),cos(fix32).getFloat(),asin(fix32).getFloat(),sin(fix32).getFloat());
	
	
	tpVec4<tpFloat> vec4f;
	tpVec4<tpFixed32> vec4x;
	tpVec3<tpFixed32> vec3x;
	tpVec2<tpFixed32> vec2x(tpFixed32(3),tpFixed32(5));
	
	tpLogMessage("Vec2<Fixed> > float squared length (34.0,5.83)  %f %f",vec2x.getSquareLength().getFloat(),vec2x.getLength().getFloat());
	
	tpLogMessage("Sizeof vec4<float>, vec4<fixed> %d=%d",sizeof(tpVec4<tpFloat>),sizeof(tpVec4<tpFixed32>));
	
	vec2x.normalize();
	
	tpArray<int> i;
	i.add(10);
	
	tpRefPtr<tpReferenced> r(new tpReferenced());
	
	tpMat44<float> m44f;
	tpMat44<tpFixed32> m44x;
	
	m44f.setIdentity();
	m44x.setIdentity();

	m44f.invert();
	m44x.invert();

	
	m44f.setTranslation(3.3f,3.3f,3.3f);

	// file IO

	tpFile io_W;
	if (io_W.open("hello.txt","w"))
	{
		tpString hello_world_io("Hello World IO");
		io_W.write(hello_world_io.c_str(),hello_world_io.getLength());
		tpLogMessage("File > wrote %d bytes",io_W.getCount());
	}

	tpFile io_R;
	if (io_R.open("hello.txt","rb"))
	{
		io_R.seek(0,tpIO::kSeekEnd);
		tpSizeT io_R_size = io_R.tell();
		io_R.rewind();
		tpLogMessage("size > %d",io_R_size);

		tpArray<tpChar> buffer;
		buffer.resize(13);
		tpSizeT rb = io_R.read(buffer.getData(),buffer.getSize()).getCount();

		tpLogMessage("data > (%d) %s",rb,buffer.getData());
	}
	
	
	getchar();
	
	
	
	return 0;
}