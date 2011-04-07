

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
	tpFixed32 fix32_2;
	fix32.set(56.0);
	fix32_2.set(100.0);
	
	tpLogMessage("Aligns: %d %d %d\n",fix32._bits,fix32._bits_half,fix32._bits_half_val);
	
	tpLogMessage("Fixed %d = %3.3lf",fix32.m,fix32.getDouble());

	fix32.set(56);
	tpLogMessage("FixedToInt %d = %d",fix32.m,fix32.getInt());


	fix32.mult(fix32_2);
	tpLogMessage("FixedToInt %d = %d",fix32.m,fix32.getInt());
	
	
	tpArray<int> i;
	i.add(10);
	
	tpRefPtr<tpReferenced> r(new tpReferenced());
	
	return 0;
}