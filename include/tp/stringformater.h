#ifndef TP_STRINGFORMATER_H
#define TP_STRINGFORMATER_H 1

#include <tp/string.h>
#include <tp/mat.h>

inline
tpString& operator << (tpString& out, const tpMat44r& m)
{
	tpString result("[");
	for (int r = 0; r < m.rows;++r)
	{
		for (int c = 0; c < m.cols;++c)
		{
			result += tpString::format("%3.3f",m(r,c));
			if (c != m.cols - 1) result += ", ";
		}
		if (r != m.rows - 1) result += ";\n";
	}
	result += "]\n";

	return out.append(result);
}

#endif
