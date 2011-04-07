

#include <tp/array.h>
#include <tp/scopeptr.h>
#include <tp/chunk.h>
#include <tp/vec.h>
#include <tp/refptr.h>
#include <tp/string.h>

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
	
	tpString nothing;
	printf("nothing = '%s' (%d)\n",nothing.c_str(),nothing.getLength());
	
	
	printf("sizeof(char)(%d) != sizeof(wchar_t)(%d)\n",sizeof(char),sizeof(wchar_t));
	
	tpString hello_wide(L"hello wide");
	printf("hello wide = '%ls' (%d)",hello_wide.c_str(),hello_wide.getLength());
	
	tpVec4<float> vec4;
	
	tpArray<int> i;
	i.add(10);
	
	tpRefPtr<tpReferenced> r(new tpReferenced());
	
	return 0;
}