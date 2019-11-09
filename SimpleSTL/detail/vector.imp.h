#pragma once

namespace SimpleSTL {
	/******************构造器、复制、析构函数*****************/
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n) {
		allocateAndFillN(n, value_type());
	}

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value) {
		//vector<T, Alloc> 才能进入模板类作用域
		allocateAndFillN(n, value);
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		//allocateAndCopy(first, last);
		//当存放的内容就是整数型的时候，
		//当以两个整数构造vector的时候，实例化的是该函数还是上一个函数

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
	/******************************逻辑比较相关操作符**************************/
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator == (const vector& v)const {
		auto first1 = v.cbegin();
		auto first2 = cbegin();

		for (; first1 != v.cend() && first2 != cend(); first1++, first2++) {
			if (*first1 != *first2)
				return false;
		}

		if (!(first1 == v.cend() && first2 == cend()))
			return false;

		return true;
	}

	template<class T, class Alloc>
	bool vector<T, Alloc>::operator != (const vector& v)const {
		return !(*this == v);
	}


	/****************************修改容器相关操作**************/
	template<class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n, value_type val) {
		if (n < size()) {
			dataAllocator::destroy(_start + n, _finish);
			_finish = _start + n;
		}
		else if (n > size() && n <= capacity()) {
			auto lengthOfInsert = n - size();
			_finish = SimpleSTL::uninitialized_fill_n(_finish, lengthOfInsert, val);
		}
		else if(n > capacity()){
			auto lengthOfInsert = n - size();
			auto newCapacity = getNewCapacity(n);

			T* newStart = dataAllocator::allocate(newCapacity);
			T* newFinish = SimpleSTL::uninitialized_copy(begin(), end(), newStart);
			newFinish = SimpleSTL::uninitialized_fill_n(newFinish, lengthOfInsert, val);
			
			destroyAndDeallocateAll();
			_start = newStart;
			_finish = newFinish;
			_endOfStorage = _start + newCapacity;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		if (n <= capacity())
			return;


		T* newStart = dataAllocator::allocate(n);
		T* newFinish = SimpleSTL::uninitialized_copy(begin(), end(), newStart);

		_start = newStart;
		_finish = newFinish;
		_endOfStorage = _start + n;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector& v) {
		if (this != &v) {
			SimpleSTL::swap(_start, v._start);
			SimpleSTL::swap(_finish, v._finish);
			SimpleSTL::swap(_endOfStorage, v._endOfStorage);
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const value_type& value) {
		insert(end(), value);
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--_finish;
		dataAllocator::destroy(_finish);
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& val) {
		//哪里保证position 的位置合法呢
		const auto index = position - begin();
		insert(position, 1, val);
		return begin() + index;
	}


	template<typename T, typename Alloc>
	void vector<T, Alloc>::insert(iterator position, const size_type& n, const value_type& val) {
		insert_aux(position, n, val, typename std::is_integral<size_type>::type()); //直接调用true_type .
	}

	template<typename T, typename Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
		return erase(position, position + 1);
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(const iterator first, const iterator last) {
		//如果擦除的元素保存资源，需要释放资源
		//保证范围的代码呢？？
		dataAllocator::destroy(first, last);

		//移动元素
		difference_type tailLength = end() - last;
		difference_type reaseLength = last - first;
		for (size_type i = 0; i != tailLength; i++) {
			construct(first + i, *(last + i));
		}
		_finish = first + tailLength;

		return first;
	}

	template<class T, class Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		//复制迭代器所指的内容
		difference_type locationLeft = _endOfStorage - _finish;
		difference_type locationNeed = distance(first, last);


		if (locationLeft >= locationNeed) {
			if (_finish - position > locationNeed) {
				SimpleSTL::uninitialized_copy(_finish - locationNeed, _finish, _finish);
				std::copy_backward(position, _finish - locationNeed, _finish);
				std::copy(first, last, position);
			}
			else {
				iterator temp = SimpleSTL::uninitialized_copy(first + (_finish - position), last, _finish);
				SimpleSTL::uninitialized_copy(position, _finish, temp);
				std::copy(first, first + (_finish - position), position);
			}
			_finish += locationNeed;
		}
		else {
			reallocateAndCopy(position, first, last);
		}
	}



	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, const value_type& value, std::true_type) {
		//复制迭代器所指的内容,迭代器指向的是一个整数
		assert(n != 0);
		difference_type locationLeft = _endOfStorage - _finish;
		difference_type locationNeed = n;
		if (locationLeft >= locationNeed) {
			auto tempPtr = end() - 1; // 为什么使用函数呢。因为防止直接获取指针而对其进行改变，获取拷贝，改变拷贝不会影响_start;
			for (; tempPtr >= position; tempPtr--) {
				//*(tempPtr + locationNeed) = *tempPtr;  //vector 本身存放的就是指针，这样的构造内存不够
				construct(tempPtr + locationNeed, *tempPtr);
			}
			SimpleSTL::uninitialized_fill_n(position, n, value);
			_finish += locationNeed;
		}
		else {
			reallocateAndFillN(position, n, value);
		}
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::reallocateAndCopy(iterator position, InputIterator first, InputIterator last) {
		difference_type newCapacity = getNewCapacity(last - first);

		T* newStart = dataAllocator::allocate(newCapacity);
		T* newEndofStorage = newStart + newCapacity;
		T* newFinish = SimpleSTL::uninitialized_copy(begin(), position, newStart);
		newFinish = SimpleSTL::uninitialized_copy(first, last, newFinish);
		newFinish = SimpleSTL::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		_start = newStart;
		_finish = newFinish;
		_endOfStorage = newEndofStorage;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::reallocateAndFillN(iterator position, const size_type& n, const value_type& val) {
		difference_type newCapacity = getNewCapacity(n);
		
		T* newStart = dataAllocator::allocate(newCapacity);
		T* newEndOfStorage = newStart + newCapacity;
		T* newFinish = SimpleSTL::uninitialized_copy(begin(), position, newStart);
		newFinish = SimpleSTL::uninitialized_fill_n(newFinish, n, val);
		newFinish = SimpleSTL::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		_start = newStart;
		_finish = newFinish;
		_endOfStorage = newEndOfStorage;
	}

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
		//存放的内容
		allocateAndCopy(first, last);
	}

	template<class T, class Alloc>
	template<typename Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type) {
		//vector 本身存放的就是整数类型
		//std::cout << "vector<T, Alloc>::vector(InputIterator first, InputIterator last)" +
		//	typename std::is_integral<InputIterator>::type() << std::endl;

		allocateAndFillN(n, value);
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const {
		//获取新的内存大小
		size_type oldCapacity = _endOfStorage - _start;

		auto res = SimpleSTL::max(oldCapacity, len);
		size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);

		/*
		如果 oldCapacity == 0 新内存就是零
		*/

		return newCapacity;
	}


}