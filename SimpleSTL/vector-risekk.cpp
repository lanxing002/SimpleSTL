
#include "vector.h"

namespace SimpleSTL {
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type value) {
		//vector<T, Alloc> ���ܽ���ģ����������
		allocateAndFillN(n, value);

	}


	/****************************�޸�������ز���**************/

	template<class T, class Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll() {
		if (capacity() != 0) {
			dataAllocator::destroy(_start, _finish); //���ܰ���������Դ��ȷ�����õ���Դ�ͷ�
			dataAllocator::deallocate(_start, capacity()); //�黹�ڴ�ռ�
		}
	}

	template<class T, class Alloc>
	void vector<T,  Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
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

	template<class T, class Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator positon, InputIterator first, InputIterator last, std::false_type) {

	}

	template<class T, class Alloc>
	template<typename Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, value_type& value, std::true_type) {

	}

}