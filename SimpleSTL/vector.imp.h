#pragma once

namespace SimpleSTL {
	/******************构造器、复制、析构函数*****************/
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value) {
		//vector<T, Alloc> 才能进入模板类作用域
		allocateAndFillN(n, value);
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		//allocateAndCopy(first, last);
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector& v) {
		allocateAndCopy(v._start, v._finish);
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(vector&& v) {
		_start = v._start;
		_finish = v._finish;
		_endOfStorage = v._endOfStorage;
		v._start = v._finish = v._endOfStorage = 0;
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& v) { //如果不是引用就会陷入无限循环调用
		if (this != &v) {
			// 处理自赋值情况
			allocateAndCopy(v._start, v._finish);
		}
		return *this;
	}

	//移动赋值操作
	template<typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (vector&& v) {
		if (this != &v) {
			destroyAndDeallocateAll();  //释放自己内存空间
			_start = v._start;
			_finish = v._finish;
			_endOfStorage = v._endOfStorage;
			v._start = v._finish = v._endOfStorage = 0;
		}
		return *this;
	}

	template<class T, class Alloc>
	vector<T, Alloc>::~vector() {
		destroyAndDeallocateAll();
	}

	/****************************修改容器相关操作**************/

	template<class T, class Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator positon, InputIterator first, InputIterator last, std::false_type) {
		//复制迭代器所指的内容

	}

	//template<class T, class Alloc>
	//template<typename Integer>
	//void vector<T, Alloc>::insert_aux(iterator position, Integer n, value_type& value, std::true_type) {

	//}




	template<class T, class Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll() {
		if (capacity() != 0) {
			dataAllocator::destroy(_start, _finish); //可能包含其他资源，确保引用的资源释放
			dataAllocator::deallocate(_start, capacity()); //归还内存空间
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
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


}