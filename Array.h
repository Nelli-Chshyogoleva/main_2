#ifndef __ARRAY_15_2021__
#define __ARRAY_15_2021__

template <typename ElementType>
class Array {
	ElementType* data;
	unsigned int Len, maxLen;
	bool isSorted;
	void QuickSort(ElementType*, unsigned int);
public:
	Array(unsigned int N = 0) : Len(0), maxLen(N), data(nullptr), isSorted(true){
		if (maxLen > 0) {
			data = new ElementType[maxLen];
			if (!data) throw "Out of memory";
		}
	}
	~Array() { clear(); }
	void clear() {
		if (data) delete[] data;
		data = nullptr; Len = 0; maxLen = 0;
	}
	void empty() { Len = 0; }
	unsigned int size() const { return Len; }
	unsigned int capacity() const { return maxLen; }
	void push_back(const ElementType&);
	ElementType& operator[] (unsigned int i) const {
		if (i>=Len) throw "Out of array's bounds";
		return data[i];
	}
	class iterator {
		ElementType* current;
	public:
		iterator(ElementType* p = nullptr) : current(p) {}
		ElementType& operator*() { return *current; }
		ElementType& operator*() const { return *current; }
		bool operator==(const iterator& p) const { return current == p.current; }
		bool operator!=(const iterator& p) const { return current != p.current; }
		iterator& operator++() { ++current; return *this; }
		iterator operator++(int) { ElementType* tmp = current; ++current; return iterator(tmp); }
		friend class Array;
	};
	iterator begin() const { return iterator(data); }
	iterator end() const { return iterator(data+Len); }

	class reverse_iterator {
		ElementType* current;
	public:
		reverse_iterator(ElementType* p = nullptr) : current(p) {}
		ElementType& operator*() { return *current; }
		ElementType& operator*() const { return *current; }
		bool operator==(const reverse_iterator& p) const { return current == p.current; }
		bool operator!=(const reverse_iterator& p) const { return current != p.current; }
		reverse_iterator& operator++() { --current; return *this; }
		reverse_iterator operator++(int) { ElementType* tmp = current; --current; return reverse_iterator(tmp); }
	};
	reverse_iterator rbegin() const { return reverse_iterator(data+Len-1); }
	reverse_iterator rend() const { return reverse_iterator(data-1); }

	iterator find(const ElementType&)const;
	void sort() { QuickSort(data, Len); isSorted = true; }
	void erase(const iterator&);
};

template <typename ElementType>
void Array<ElementType>::push_back(const ElementType& X) {
	if (Len + 1 > maxLen) {
		
		unsigned int newLen = maxLen + 1;
		if(maxLen<20)
			newLen = maxLen + 1;
		else
			newLen = maxLen + 100;
			
			
		ElementType* tmp = new ElementType[newLen];
		if (!tmp) throw "Out of memory";
		for (unsigned int i = 0; i < Len; ++i)
			tmp[i] = std::move(data[i]);
		delete[] data;
		data = tmp;
		maxLen = newLen;
	}
	
	data[Len++] = X;
	isSorted = false;
}

template <typename ElementType>
typename Array<ElementType>::iterator Array<ElementType>::find(const ElementType& X)const {
	if (isSorted) {
		if (Len == 0) return end();
		unsigned int left = 0, right = Len - 1, p;
		while (left < right) {
			p = left + (right - left) / 2;
			if (data[p] < X) left = p + 1;
			else if (data[p] == X)
				return iterator(data + p);
			else if (p == 0) break;
			else right = p - 1;
		}
		if (data[left] == X) return iterator(data + left);
	} else {
		for (unsigned int i = 0; i < Len; ++i)
			if (data[i] == X) return iterator(data + i);
	}
	return end();
}

template <typename ElementType>
void Array<ElementType>::QuickSort(ElementType* M, unsigned int n) {
	if (n < 2) return;
	unsigned int i = 0, j = n - 1;
	ElementType p = M[(j-1)/2];
	while (i < j) {
		while (M[i] < p) ++i;
		while (p < M[j]) --j;
		if (i < j) {
			std::swap(M[i], M[j]);
			++i; --j;
		}
		else if (i == j) {
			++i; if (j) --j; else break;
		}
	}
	if (i > (n - 1) / 2) {
		if (i < n - 1) QuickSort(M + i, n - i);
		if (j > 0) QuickSort(M, j + 1);
	} else {
		if (j > 0) QuickSort(M, j + 1);
		if (i < n - 1) QuickSort(M + i, n - i);
	}
}

template <typename ElementType>
void Array<ElementType>::erase(typename const Array<ElementType>::iterator& pos) {
	if (pos == end()) return;
	ElementType* cur = pos.current; // Это возможно из-за того, что написали friend class Array;
	while (cur != (data + Len - 1)) {
		*cur = std::move(*(cur + 1));
		++cur;
	}
	--Len;
}
#endif
