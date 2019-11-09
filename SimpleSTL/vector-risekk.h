#pragma once

#include <algorithm>
#include <type_traits>

#include "Allocator.h"
#include "Algorithm.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "UninitializedFunctions.h"

namespace SimpleSTL {
	template<class T, class Alloc = allocator<T>>// Ĭ�ϵķ�����Ϊ allocator<T>
	class vector {
	private:
		T* _start;
		T* _finish;
		T* _endOfStorage;
		typedef Alloc dataAllocator;
	public:
		typedef T                                   value_type;
		typedef T*                                  iterator;
		typedef const T*                            const_iterator;
		typedef reverse_iterator_t<T*>              reverse_iterator;
		typedef reverse_iterator_t<const T*>        const_reverse_iterator;
		typedef iterator                            pointer;
		typedef T&                                  reference;
		typedef const T&                            const_reference;
		typedef size_t                              size_type;
		typedef ptrdiff_t                           difference_type;

	public:
		//���죬���ƣ�������غ���
		vector()
			��_start(0), _finish(0), _endOfStorage(0) {}
		explicit vector(const size_type n);
		vector(const size_type n, const value_type value);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);
		vector(const vector& v);
		vector& operator = (const vector& v);
		vector& operator = (vector&& v);
		~vector();

		//�Ƚ���ز���
		bool operator == (const vector& v) const;
		bool operatr != (const vector & v) const;

		//��������ز�����
		iterator begin() { return (_start); }
		const_iterator begin() { return (_start); }
		const_iterator cbegin() { return (_start); }

		iterator end() { return(_finish); }
		const_iterator end() { return (_finish); }
		const_iterator cend() { return (_finish); }

		reverse_iterator rbegin() { return reverse_iterator(_finish); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(_finish); }
		reverse_iterator rend() { return reverse_iterator(_start); }
		const_reverse_iterator crend() const { return const_reverse_iterator(_start); }

		//�������йصĲ���
		difference_type size() const { return _finish - _start; }
		difference_type capacity() const{ return _endOfStorage - _start; }
		bool empty() { return _start == _finish; }
		void resize(size_type n, value_type val = value_type());
		void reverse(size_type n);
		void shrink_to_fit();

		//

		Alloc get_allocator() { return dataAllocator; }

	private:
		void destroyAndDeallocateAll();
		void allocateAndFillN(const size_type n, const value_type& val);
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);

		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void vector_aux(Integer n, const value_type& value, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void insert_aux(iterator position, Integer n, value_type& value, std::true_type);
		template<class InputIterator>
		void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
		void reallocateAndCopy(iterator position, const size_type n, const value_type& val);
		size_type getNewCapacity(size_type len) const;
		

	}; //end of class vector
}