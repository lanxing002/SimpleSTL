#pragma once

namespace SimpleSTL {
	/******************�����������ơ���������*****************/
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value) {
		//vector<T, Alloc> ���ܽ���ģ����������
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

	/****************************�޸�������ز���**************/

	template<class T, class Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator positon, InputIterator first, InputIterator last, std::false_type) {
		//���Ƶ�������ָ������

	}

	//template<class T, class Alloc>
	//template<typename Integer>
	//void vector<T, Alloc>::insert_aux(iterator position, Integer n, value_type& value, std::true_type) {

	//}




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
		allocateAndCopy(first, last);
	}

	template<class T, class Alloc>
	template<typename Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type) {
		allocateAndFillN(n, value);
	}


}