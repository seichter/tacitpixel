#ifndef TPSTRINGUTILS_H
#define TPSTRINGUTILS_H



template <typename T> 
unsigned int tpStrLen(const T* str)
{
	unsigned int ret = 0;
	for(; str && *str; str++) {ret++;}
	return ret;
}


template <typename T> inline int tpStrCmp(const T* str1,const T* str2)
{
	if (str1 == 0 && str2) return  1;
	if (str2 == 0 && str1) return -1;

	while ((*str1 == *str2) && *str1 && *str2) str1++, str2++;

	if (*(str1) > *(str2)) return 1;
	if (*(str1) < *(str2)) return -1;

	return 0;
}

template <typename T> inline int tpStrNCmp(const T* str1, const T* str2, tpSizeT n)
{
	while (0 < n--)
	{
		T c1 = *str1++;
		T c2 = *str2++;
		if (c1 != c2)
		{
			return c1 - c2;
		}
		if (c1 == (T)0)
		{
			return 0;
		}
	}
	return 0;
}

template <typename T> inline T* tpStrDup(const T* src)
{
	tpSizeT _size = tpStrLen(src) * sizeof(T) + 1;
	T* res = (T*)malloc(_size);
	memcpy(res,src,_size);	
	if (_size) res[_size-1] = (T)0;
	return res;
}

template <typename T> inline const T* tpStrRChr(const T* str,T ch)
{
	const T* res = 0;
	do {
		if (ch == *str) res = str;
	} while (*str++);
	return res;
}

template <typename T> inline const T* tpStrChr(const T* str,T ch)
{
	do {
		if (*str == ch)
		{
			return str;
		}
	} while (*str++);
	return 0;
};


template <typename T> inline const T* tpStrStr( const T* s1, const T* s2 )
{
	const T* p = s1;
	tpUInt len = tpStrLen( s2 );
	for (; (p = tpStrChr(p, *s2)) != 0; p++)
	{
		if (tpStrNCmp( p, s2, len ) == 0)
		{
			return p;
		}
	}
	return 0;
}

#endif