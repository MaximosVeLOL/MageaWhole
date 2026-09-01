#ifndef __CORE_LIST_H__
#define __CORE_LIST_H__

#include <exception>

//C++ std::vector, but it doesn't take up 72 bytes

template<typename T, typename C>
class List {
private:
	T* mData = nullptr;
	C mCount = 0;
	
	C arraySize(T* pArray) {
		return (sizeof(pArray) / sizeof(T));
	}

	void _resize(C pNewSize, T* pExtra = nullptr) {
		if (mCount == 0) {
			mData = new T[pNewSize]{ T() };
			if (pExtra) {
				for (C i = 0; i < pNewSize;i++) {
					mData[i] = pExtra[i];
				}
			}
			mCount = pNewSize;
			return;
		}
		T* output = new T[pNewSize];
		C& small = (mCount > pNewSize ? pNewSize : mCount);
		C i = 0;
		for (; i < small;i++) {
			output[i] = mData[i];
		}
		if (pExtra) {
			for (C j = 0;j < pNewSize - i;j++) {
				output[i + j] = pExtra[j];
			}
		}
		delete[] mData;
		mData = pExtra;
		mCount = pNewSize;
	}
	void _xBounds(C pBounds) {
		if (pBounds < 0 || pBounds > mCount) {
			throw new std::exception("pBounds is not in range of array!");
		}
	}
public:
	

	void Insert(T pItem, C pIndex, bool pReplace = false) {
		_resize(mCount + 1);
		for (C i = pIndex; i < mCount - 1;i++) {
			mData[i] = mData[i] + 1;
		}
		mData[pIndex] = pItem;
	}

	void Push(T pItem) {
		_resize(mCount + 1);
		mData[mCount] = pItem;
	}
	void Push(T[] pArray) {
		_resize(mCount + arraySize(pArray), pArray);
	}

	void Erase(C pIndex) {
		for (pIndex < mCount - 1;pIndex++) {
			mData[pIndex] = mData[pIndex] + 1;
		}
	}
	
};


#endif