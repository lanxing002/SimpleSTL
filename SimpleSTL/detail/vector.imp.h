#pragma once

namespace SimpleSTL {
	/******************�����������ơ���������*****************/
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n) {
		allocateAndFillN(n, value_type());
	}

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value) {
		//vector<T, Alloc> ���ܽ���ģ����������
		allocateAndFillN(n, value);
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		//allocateAndCopy(first, last);
		//����ŵ����ݾ��������͵�ʱ��
		//����������������vector��ʱ��ʵ�������Ǹú���������һ������

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
	vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& v) { //����������þͻ���������ѭ������
		if (this != &v) {
			// �����Ը�ֵ���
			allocateAndCopy(v._start, v._finish);
		}
		return *this;
	}

	//�ƶ���ֵ����
	template<typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (vector&& v) {
		if (this != &v) {
			destroyAndDeallocateAll();  //�ͷ��Լ��ڴ�ռ�
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
	/******************************�߼��Ƚ���ز�����**************************/
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


	/****************************�޸�������ز���**************/
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
		//���ﱣ֤position ��λ�úϷ���
		const auto index = position - begin();
		insert(position, 1, val);
		return begin() + index;
	}


	template<typename T, typename Alloc>
	void vector<T, Alloc>::insert(iterator position, const size_type& n, const value_type& val) {
		insert_aux(position, n, val, typename std::is_integral<size_type>::type()); //ֱ�ӵ���true_type .
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
		//���������Ԫ�ر�����Դ����Ҫ�ͷ���Դ
		//��֤��Χ�Ĵ����أ���
		dataAllocator::destroy(first, last);

		//�ƶ�Ԫ��
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
		//���Ƶ�������ָ������
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
		//���Ƶ�������ָ������,������ָ�����һ������
		assert(n != 0);
		difference_type locationLeft = _endOfStorage - _finish;
		difference_type locationNeed = n;
		if (locationLeft >= locationNeed) {
			auto tempPtr = end() - 1; // Ϊʲôʹ�ú����ء���Ϊ��ֱֹ�ӻ�ȡָ���������иı䣬��ȡ�������ı俽������Ӱ��_start;
			for (; tempPtr >= position; tempPtr--) {
				//*(tempPtr + locationNeed) = *tempPtr;  //vector �����ŵľ���ָ�룬�����Ĺ����ڴ治��
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
			dataAllocator::destroy(_start, _finish); //���ܰ���������Դ��ȷ�����õ���Դ�ͷ�
			dataAllocator::deallocate(_start, capacity()); //�黹�ڴ�ռ�
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
		_start = dataAllocator::allocate(n); //n һ����ָ�룬��Ȼ����޸�vector �����ݣ��������Ĵ���
		SimpleSTL::uninitialized_fill_n(_start, n, value); //������new��һ�Ѷ��󣬶���Ԥ�ȷ���ռ䣨�洢ָ�룩
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
		//�洢ָ������
		//��ŵ�����
		allocateAndCopy(first, last);
	}

	template<class T, class Alloc>
	template<typename Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type) {
		//vector �����ŵľ�����������
		//std::cout << "vector<T, Alloc>::vector(InputIterator first, InputIterator last)" +
		//	typename std::is_integral<InputIterator>::type() << std::endl;

		allocateAndFillN(n, value);
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const {
		//��ȡ�µ��ڴ��С
		size_type oldCapacity = _endOfStorage - _start;

		auto res = SimpleSTL::max(oldCapacity, len);
		size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);

		/*
		��� oldCapacity == 0 ���ڴ������
		*/

		return newCapacity;
	}


}