#ifndef TP_SORT_H
#define TP_SORT_H 1

/**
  * Quicksort algorithm for arrays
  */

#include <tp/array.h>

template <typename T>
tpSizeT
tpSortPartition(tpArray<T>& array, tpSizeT left, tpSizeT right)
{

	tpSizeT rIdx(right + 1); tpSizeT lIdx(left - 1);

	for (;;) {
		do { rIdx--; } while (array[rIdx] > array[left]);
		do { lIdx++; } while (array[lIdx] < array[left]);

		if (lIdx < rIdx)
			{ tpSwap(array[lIdx],array[rIdx]); }
		else
			break;
	}
	return rIdx;
}

template <typename T>
void
tpSortProxy(tpArray<T>& items,tpSizeT left,tpSizeT right)
{
	if (left < right)
	{
		tpSizeT partition = tpSortPartition(items,left,right);

		tpSortProxy(items,left,partition);
		tpSortProxy(items,partition + 1,right);
	}
}

template <typename T>
static inline void
tpSort(tpArray<T>& items)
{
	tpSortProxy(items,0,items.getSize()-1);
}



#endif
