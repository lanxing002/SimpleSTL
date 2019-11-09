#pragma once

#include "Allocator.h"
#include "ReverseIterator.h"
#include "UninitializedFunctions.h"
#include "Utility.h"

#include <cstring>
#include <type_traits>
#include <iostream>

namespace SimpleSTL {

	class string
	{
	public:
		typedef char      value_type;
		typedef char*     iterator;
		typedef const char* const_iterator;
		typedef reverse_iterator_t<char*> reverse_iterator;
		typedef reverse_iterator_t<const char*> const_reverse_iterator;
		typedef char& reference;
		typedef const char& const_reference;
		typedef size_t  size_type;
		typedef ptrdiff_t  difference_type;
		//npos is a static member constant value with the greatest possible value for an element of the size_t.
		static const size_t npos = -1; //转换为无符号以后对应整数最大数值

	private: // 私有域
		char* _start;
		char* _finish;
		char* _endOfStorage;

		typedef SimpleSTL::allocator<char> dataAllocator;

	public: //公共方法
		string() :_start(0), _finish(0), _endOfStorage(0) {}
		string(const string&  str); //拷贝构造器
		string(string&& str); //移动构造器
		string(const string& str, size_t pos, size_t len = npos);
		string(const char* s);
		string(const char*s , size_t n);
		string(size_t n, char c); //重复构造
		template<class InputIterator>
		string(InputIterator first, InputIterator last);

		string& operator=(const string& str);
		string& operator=(string&& str);
		string& operator=(const char* s);
		string& operator=(char c);

		~string(); //析构函数

		iterator begin() { return _start; }
		iterator end() { return _finish; }
		const_iterator begin() const { return _start; }
		const_iterator end() const { return _finish; }
		reverse_iterator rbegin() { return reverse_iterator(_finish); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(_finish);}
		reverse_iterator rend() { return reverse_iterator(_start); }
		const_reverse_iterator rend() const { return const_reverse_iterator(_start); }
		size_t size() const { return _finish - _start; }
		size_t length() const { return size(); }
		size_t capacity() const { return _endOfStorage - _start; }

		void clear() {
			dataAllocator::destroy(_start, _finish);
			_start = _finish;
		}
		bool empty() const { return (_start == _finish); }
		void resize(size_t n);
		void resize(size_t n, char c);
		void reserve(size_t n = 0);

		void shrink_to_fit() {
			//去掉多余还没有使用的内存空间
			//回收多余空间，deallocate 回收作用， destory销毁对象作用
			dataAllocator::deallocate(_finish, _endOfStorage - _finish);
			_endOfStorage = _finish;
		}

		char& operator[] (size_t pos) { return *(_start + pos); }
		const char& operator[] (size_t pos) const { return *(_start + +pos); }
		char& back() { return *(_finish - 1); }
		const char& back() const { return *(_finish - 1); }
		char& front() { return *(_start); }
		const char& front() const { return *(_start); }

		void push_back(char c) { insert(end(), c); };
		string& insert(size_t pos, const string& str);
		string& insert(size_t pos, const string& str, size_t subpos, size_t sublen = npos);
		string& insert(size_t pos, const char* s);
		string& insert(size_t pos, const char* s, size_t n);
		string& insert(size_t pos, size_t n, char c);
		iterator insert(iterator p, size_t n, char c);
		iterator insert(iterator p, char c);
		template <class InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);

		string& append(const string& str);
		string& append(const string& str, size_t subpos, size_t sublen = npos);
		string& append(const char* s);
		string& append(const char* s, size_t n);
		string& append(size_t n, char c);
		template<class InputIterator>
		string& append(InputIterator first, InputIterator last);

		string& operator+= (const string& str);
		string& operator+= (const char* s);
		string& operator+= (char c);

		void pop_back() { erase(_finish - 1, _finish); } // TODO 自己加了递减操作符
		string& erase(size_t pos, size_t len = npos);
		iterator erase(iterator p);
		iterator erase(iterator first, iterator last);

		string& replace(size_t pos, size_t len, const string& str);
		string& replace(iterator i1, iterator i2, const string& str);
		string& replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen = npos); //尽量复制最大的长度
		string& replace(size_t pos, size_t len, const char* s);
		string& replace(iterator i1, iterator i2, const char* s);
		string& replace(iterator i1, iterator i2, const char* s, size_t n);
		string& replace(size_t pos, size_t len, const char* s, size_t n);
		string& replace(iterator i1, iterator i2, size_t n, char c);
		string& replace(size_t pos, size_t len, size_t n, char c);
		template<class InputIterator>
		string& replace(iterator i1, iterator i2, InputIterator first, InputIterator last);

		void swap(string& str) {
			SimpleSTL::swap(str._start, _start);
			SimpleSTL::swap(str._finish, _finish);
			SimpleSTL::swap(str._endOfStorage, _endOfStorage);
		}
		
		size_t copy(char* s, size_t len, size_t pos) {
			auto ptr = SimpleSTL::uninitialized_copy(begin() + pos, begin() + pos + len, s);
			return size_t(ptr - s);
		}

		size_t find(const string& str, size_t pos = 0) const;
		size_t find(const char* s, size_t pos = 0) const;
		size_t find(const char* s, size_t pos, size_t n) const;
		size_t find(char c, size_t pos = 0) const;
		size_t rfind(const string& str, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos, size_t n) const;
		size_t rfind(char c, size_t pos = npos) const;
		size_t find_first_of(const string& str, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_of(char c, size_t pos = 0) const;
		size_t find_last_of(const string& str, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_of(char c, size_t pos = npos) const;
		size_t find_first_not_of(const string& str, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_not_of(char c, size_t pos = 0) const;
		size_t find_last_not_of(const string& str, size_t pos = npos) const;
		size_t find_last_not_of(const char* s, size_t pos = npos) const;
		size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_not_of(char c, size_t pos = npos) const;

		string substr(size_t pos = 0, size_t len = npos) const {
			return string(begin() + pos, begin() + pos + len);
		}

		int compare(const string& str) const;
		int compare(size_t pos, size_t len, const string& str) const;
		int compare(size_t pos, size_t len, const string& str,
			size_t subpos, size_t sublen) const;
		int compare(const char* s) const;
		int compare(size_t pos, size_t len, const char* s) const;
		int compare(size_t pos, size_t len, const char* s,  size_t n) const;

	private:
		void moveData(string& str);
		//插入时空间不足的情况
		template<class InputIterator>
		iterator insert_aux_copy(iterator p,  InputIterator first, InputIterator last);
		iterator insert_aux_filln(iterator p, size_t n, value_type c);
		size_type getNewCapacity(size_type len) const;
		void allocateAndFillN(size_t n, char c);
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);
		void string_aux(size_t n, char c, std::true_type);
		template<class InputIterator>
		void string_aux(InputIterator first, InputIterator last, std::false_type);
		void destroyAndDeallocate();
		size_t rfind_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond)const;
		size_t find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond)const;
		int compare_aux(size_t pos, size_t len, const_iterator cit, size_t subpos, size_t sublen)const;
		bool isContained(char ch, const_iterator first, const_iterator last)const;
		size_t changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue)const;
	
	public:
		friend std::ostream& operator <<(std::ostream& os, const string& str);
		friend std::istream& operator >> (std::istream& is, string& str);
		friend string operator+ (const string& lhs, const string& rhs);
		friend string operator+ (const string& lhs, const char* rhs);
		friend string operator+ (const char* lhs, const string& rhs);
		friend string operator+ (const string& lhs, char rhs);
		friend string operator+ (char lhs, const string& rhs);
		friend bool operator== (const string& lhs, const string& rhs);
		friend bool operator== (const char* lhs, const string& rhs);
		friend bool operator== (const string& lhs, const char* rhs);
		friend bool operator!= (const string& lhs, const string& rhs);
		friend bool operator!= (const char* lhs, const string& rhs);
		friend bool operator!= (const string& lhs, const char* rhs);
		friend bool operator<  (const string& lhs, const string& rhs);
		friend bool operator<  (const char* lhs, const string& rhs);
		friend bool operator<  (const string& lhs, const char* rhs);
		friend bool operator<= (const string& lhs, const string& rhs);
		friend bool operator<= (const char* lhs, const string& rhs);
		friend bool operator<= (const string& lhs, const char* rhs);
		friend bool operator>  (const string& lhs, const string& rhs);
		friend bool operator>  (const char* lhs, const string& rhs);
		friend bool operator>  (const string& lhs, const char* rhs);
		friend bool operator>= (const string& lhs, const string& rhs);
		friend bool operator>= (const char* lhs, const string& rhs);
		friend bool operator>= (const string& lhs, const char* rhs);
		friend void swap(string& x, string& y);
		friend std::istream& getline(std::istream& is, string& str, char delim);
		friend std::istream& getline(std::istream& is, string& str);
	};//end of string

	template<class InputIterator>
	string::string(InputIterator first, InputIterator last) {
		//处理指针和数字间区别的函数
		string_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class InputIterator>
	string::iterator string::insert_aux_copy(iterator p, InputIterator first, InputIterator last) {
		//插入操作的 copy 辅助器方法，主要用来开辟新空间并复制元素
		size_t lengthOfInsert = last - first;
		auto newCapacity = getNewCapacity(lengthOfInsert);
		iterator newStart = dataAllocator::allocate(newCapacity);
		iterator newFinish = SimpleSTL::uninitialized_copy(_start, p, newStart);
		newFinish = SimpleSTL::uninitialized_copy(first, last, newFinish);
		auto res = newFinish;
		newFinish = SimpleSTL::uninitialized_copy(p, _finish, newFinish);

		destroyAndDeallocate();
		_start = newStart;
		_finish = newFinish;
		_endOfStorage = _start + newCapacity;
		return res;
	}

	template<class InputIterator>
	string::iterator string::insert(iterator p, InputIterator first, InputIterator last) {
		/**
			if(first - lat > endofStorge - _finish)
				allocate new store
			else
				
		*/
		auto capac = capacity();
		auto si = size();
		auto lengthOfLeft = capacity() - size();
		size_t lengthOfInsert = distance(first, last);
		if (lengthOfInsert > lengthOfLeft) {
			//空间不够
			return insert_aux_copy(p, first, last);
		}
		else {
			for (iterator it = _finish - 1; it >= p; it--) {
				//从右边开始逐个右移
				*(it + lengthOfInsert) = *it;
			}
			SimpleSTL::uninitialized_copy(first, last, p);
			_finish += lengthOfInsert;
			return (p + lengthOfInsert);
		}
	}
	template<class InputIterator>
	string& string::append(InputIterator first, InputIterator last) {
		insert(end(), first, last);
		return *this;
	}
	template<class InputIterator>
	string& string::replace(iterator i1, iterator i2, 
		InputIterator first, InputIterator last) {
		//如果两个长度不一样呢，应该在哪里保证长度
		//first delete data and insert data
		auto ptr = erase(i1, i2);
		insert(ptr, first, last);
		return *this;
	}
	template<class InputIterator>
	void string::allocateAndCopy(InputIterator first, InputIterator last) {
		_start = dataAllocator::allocate(last - first);
		_finish = SimpleSTL::uninitialized_copy(first, last, _start);
		_endOfStorage = _finish;
	}
	template<class InputIterator>
	void string::string_aux(InputIterator first, InputIterator last, std::false_type) {
		allocateAndCopy(first, last);
	}
}