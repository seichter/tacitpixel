#ifndef TPUTILS_H
#define TPUTILS_H


template <typename T> inline const T& tpMax(const T& value1,const T& value2)
{
	return (value1 > value2) ? value1 : value2;
};

template <typename T> inline const T& tpMin(const T& value1,const T& value2)
{
	return (value1 < value2) ? value1 : value2;
};

template <typename T> inline const T& tpClamp(const T& value,const T& min,const T& max)
{
	return (value > min) ? (value < max) ? value : max : min;
};

template <typename T> inline const T& tpBounce(const T& val,const T& min,const T& max,T& var)
{
	if (val < min) var = abs(var);
	if (val > max) var = -abs(var);
	return (val += var);
};


template <typename T> void tpSwap(T& value1,T& value2)
{
	T _temp = value1;
	value1 = value2;
	value2 = _temp;
};


template <typename T> inline bool tpBetween(const T& value,const T& border1,const T& border2)
{
	return (border2 > border1) ? ((value < border2) && (value > border1)) :
		((value > border2) && (value < border1));
};


#endif