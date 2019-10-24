#pragma once

#include <new>

#include "TypeTraits.h"



namespace SimpleSTL {
	template<class T1, class T2>
	inline void consttruct(T1* ptr1, const T2& value) {
		new(ptr1) T1(value);
		//ptr1 = new T1(value);
	}

	template<class T>
	inline void destroy(T* ptr) {
		ptr->~T();
	}

	template<class ForwardIterator>
	inline void _destory(ForwardIterator first, ForwardIterator last, _true_type) {}

	template<class ForwardIterator>
	inline void _destory(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first != last; ++first) {
			destroy(&*first);
		}
	}
}

//TODO ѧϰ֪ʶ�㣺
/**
  1.delete �� ������������ new �����Ķ���Ч��һ����
  2.new(varibaries) �﷨��ô����
  3. _true_type and _false_type ���Դ���ʲô
  4. SimpleTest::Test::equal_container ֮�д��� ��ʱ���� �������
  5. find_aux ���㷨ʱ�临�Ӷ�̫���ˣ��ܲ����ø��Ӹ�Ч���㷨���
  6.rfind_aux����̫���ˣ���ʲô���Ĵ���û�У���
  7. �� rfind ֮����ᵽ�˷���ָ������ã�����ָ����Ҫһ������ָ���߼���һ�µĹ��ܡ�

*/