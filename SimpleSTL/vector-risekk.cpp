
#include "vector.h"

namespace SimpleSTL {
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type value) {
		//vector<T, Alloc> 才能进入模板类作用域
		allocateAndFillN(n, value);

	}


	/****************************修改容器相关操作**************/

	template<class T, class Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll() {
		if (capacity() != 0) {
			dataAllocator::destroy(_start, _finish); //可能包含其他资源，确保引用的资源释放
			dataAllocator::deallocate(_start, capacity()); //归还内存空间
		}
	}

	template<class T, class Alloc>
	void vector<T,  Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
		_start = dataAllocator::allocate(n); //n 一定是指针，不然如果修改vector 的内容，将引起大的错误
		SimpleSTL::uninitialized_fill_n(_start, n, value); //并不是new出一堆对象，而是预先分配空间（存储指针）
		_finish = _endOfStorage = _start + n;
	}

	template<class T, class Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last) {
		_start = dataAllocator::allocate(last - first);
		_finish = SimpleSTL::uninitialized_copy(first, last, _start);
		_endOfStorage = _finish;
	}

	template<class T, class Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) {
		//存储指针内容
		allocateAndCopy(first, last);
	}

	template<class T, class Alloc>
	template<typename Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type) {
		allocateAndFillN(n, value);
	}

	template<class T, class Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator positon, InputIterator first, InputIterator last, std::false_type) {

	}

	template<class T, class Alloc>
	template<typename Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, value_type& value, std::true_type) {

	}

}