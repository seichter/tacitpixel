#ifndef TP_STRINGFORMATER_H
#define TP_STRINGFORMATER_H 1

#include <tp/string.h>
#include <tp/mat.h>

template <typename T, tpUInt R, tpUInt C>
tpString& operator << (tpString& out, const tpMat<R,C,T>& m)
{
	// we are using MatLab style
	tpString result("[ ");
	for (int r = 0; r < m.rows;++r)
	{
		for (int c = 0; c < m.cols;++c)
		{
			result += tpString::format("%3.3f",m(r,c));
			if (c != m.cols - 1) result += ", ";
		}
		if (r != m.rows - 1) result += "; ";
	}
	result += " ]";
	return out.append(result);
}

template <typename T, tpUInt C>
tpString& operator << (tpString& out, const tpVec<T,C>& v)
{
	// we are using MatLab style
	tpString result("[ ");
	for (int c = 0; c < v.dimensions;++c)
	{
		result += tpString::format("%3.3f",v[c]);
		if (c != v.dimensions - 1) result += "; ";
	}
	result += " ]";
	return out.append(result);
}


#endif
