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

//TODO 学习知识点：
/**
  1.delete 和 析构函数对于 new 出来的对象效果一样吗
  2.new(varibaries) 语法怎么来的
  3. _true_type and _false_type 各自代表什么
*/