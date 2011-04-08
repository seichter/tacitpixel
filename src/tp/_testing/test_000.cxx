

#include <tp/array.h>
#include <tp/scopeptr.h>
#include <tp/chunk.h>
#include <tp/vec.h>
#include <tp/refptr.h>
#include <tp/string.h>
#include <tp/fixed.h>
#include <tp/log.h>

#include <stdio.h>

int main(int argc, char* argv[])
{
	
	tpChunk chunk;
	chunk.reserve<float>(2);
	
	chunk.at<float>(0) = 23.5f;
	printf("23.5f = %3.1ff\n",chunk.at<float>(0));
	
	chunk.at<float>(1) = 50.0f;
	printf("50.0f = %3.1ff = %3.1ff\n",chunk.at<float>(1),chunk.ptr<float>()[1]);
	
	tpString hello("hello");
	printf("hello = %s (%d)\n",hello.c_str(),hello.getLength());
	
	hello.empty();
	printf("hello = '%s' (%d) (empty)\n",hello.c_str(),hello.getLength());
	
	hello.append("hello");
	printf("hello = '%s' (%d) (append hello)\n",hello.c_str(),hello.getLength());
	
	hello.append(" world");
	printf("hello = '%s' (%d) (append ' world')\n",hello.c_str(),hello.getLength());
	
	hello.prepend("this ");
	printf("hello = '%s' (%d) (prepend 'this ')\n",hello.c_str(),hello.getLength());
	
	tpString nothing;
	printf("nothing = '%s' (%d)\n",nothing.c_str(),nothing.getLength());
	
	
	printf("sizeof(char)(%d) != sizeof(wchar_t)(%d)\n",sizeof(char),sizeof(wchar_t));
	
	tpString hello_wide(L"hello wide");
	printf("hello wide = '%ls' (%d)\n",hello_wide.c_str(),hello_wide.getLength());
	
	tpString number("45.2344f");
	printf("number: %d %3.3f\n",number.to<int>(),number.to<float>());
	
	tpVec4<float> vec4;
	
	tpFixed32 fix32; 
	fix32.set(56.0);
	
	tpLogMessage("Aligns: %d %d %d\n",fix32._bits,fix32._bits_half,fix32._bits_half_val);
	
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
	
	
	tpVec4<tpFixed32> vecx4;
	tpVec3<tpFixed32> vecx3;
	tpVec2<tpFixed32> vecx2(tpFixed32(3),tpFixed32(5));
	
	tpLogMessage("Vec2<Fixed> > float squared length (34.0,5.83)  %f %f",vecx2.getSquareLength().getFloat(),vecx2.getLength().getFloat());
	
	vecx2.normalize();
	
	tpArray<int> i;
	i.add(10);
	
	tpRefPtr<tpReferenced> r(new tpReferenced());
	
	return 0;
}