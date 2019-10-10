#include "String.h"

#include <iostream>

namespace SimpleSTL {
	const size_t string::npos;

	string::string(size_t n, char c) {
		allocateAndFillN(n, c);
	}

	string::string(const char* s) {
		//
		allocateAndCopy(s, s + strlen(s));
	}

	string::string(const char* s, size_t n) {
		//只复制n 位数字
		allocateAndCopy(s, s + n);
	}

	//拷贝构造器
	string::string(const string& str) {
		allocateAndCopy(str.begin(), str.end());
	}


	string::string(string&& str) {
		//右值引用 
		moveData(str);
	}

	string::string(const string& str, size_t pos, size_t len) {
		//复制内容的长度，不应该是字符串的长度
		len = changeVarWhenEuqalNPOS(len, str.size(), pos);
		allocateAndCopy(str._start + pos, str._start + pos + len);
	}


	//复制函数
	string& string::operator=(char c) {
		//首先销毁该对象 内存空间
		destroyAndDeallocate();
		allocateAndFillN(1, c);
		return *this;
	}

	string& string::operator=(const char* s) {
		destroyAndDeallocate();
		allocateAndCopy(s, s + strlen(s));
		return *this;
	}

	string& string::operator=(const string& str){
		destroyAndDeallocate();
		allocateAndCopy(str.begin(), str.end());
		return *this;
	}

	string::~string() {
		destroyAndDeallocate();
	}

	void string::resize(size_t n) {
		resize(n, value_type());
	}
	 
	string::iterator string::insert_aux_filln(iterator p, size_t n, value_type c) {
		//for (; n > 0; n--)
		//	*p++ = c;
		//return p;

		auto newCapacity = getNewCapacity(n);
		//newStart 是新对象的起始地址
		iterator newStart = dataAllocator::allocate(newCapacity);
		iterator newFinish = SimpleSTL::uninitialized_copy(_start, p, newStart);
		//插入char
		newFinish = SimpleSTL::uninitialized_fill_n(newFinish, n, c);
		iterator res = newFinish;

		newFinish = SimpleSTL::uninitialized_copy(p, _finish, newFinish);
		destroyAndDeallocate();
		_start = newStart;
		_finish = newFinish;
		_endOfStorage = _start + newCapacity;

		return res;
	}

	void string::resize(size_t n, char c) {
		if (n < size()) {
			dataAllocator::destroy(_start + n, _finish);
			_finish = _start + n;
		}
		else if (n > size() && n <= capacity()) {
			//添加尾元素
			auto lengthOfInsert = n - size();
			_finish = SimpleSTL::string::insert_aux_filln(_finish, lengthOfInsert, c);
		}
		else if (n >= capacity()) {
			//重新分发内存
			auto lengthOfInsert = n - size();
			iterator newStart = dataAllocator::allocate(getNewCapacity(lengthOfInsert));
			iterator newFinish = SimpleSTL::uninitialized_copy(begin(), end(), newStart);
			newFinish = SimpleSTL::uninitialized_fill_n(newFinish, lengthOfInsert, c);

			destroyAndDeallocate();
			_start = newStart;
			_finish = newFinish;
			_endOfStorage = newStart + n;

		}
	}


	void string::reserve(size_t n) {
		//保证容器的容量大于 n
		if (capacity() >= n)
			return;
		iterator newStart = dataAllocator::allocate(n);
		iterator newFinish = SimpleSTL::uninitialized_copy(begin(), end(), newStart);

		destroyAndDeallocate();

		_start = newStart;
		_finish = newFinish;
		_endOfStorage = _start + n;
	}

	void string::destroyAndDeallocate() {
		//销毁对象
		dataAllocator::destroy(_start, _finish);
		//释放内存空间
		dataAllocator::deallocate(_start, _endOfStorage - _start);
	}

	string::size_type string::getNewCapacity(size_type len) const {
		//用拓展长度返回新容器的size
		size_type oldCapacity = _endOfStorage - _finish;
		auto res = SimpleSTL::max(oldCapacity, len);
		//为什么考虑 capacity 的容量
		auto newCapacity = oldCapacity + res;
		return newCapacity;
	}

	void string::allocateAndFillN(size_t n, char c) {
		_start = dataAllocator::allocate(n); 
		_finish = SimpleSTL::uninitialized_fill_n(_start, n ,c);
		_endOfStorage = _finish;
	}

	void string::moveData(string& str) {
		_start = str._start;
		_finish = str._finish;
		_endOfStorage = str._endOfStorage;
		//销毁源对象；
		str._start = str._finish = str._endOfStorage = nullptr;
	}

	size_t string::changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue) const {
		return (var == npos ? minuend - minue : var);
	}

	//友元函数， 重载运算符
	bool operator==(const string& lhs, const string& rhs) {
		if (lhs.size() != rhs.size())
			return false;

		auto lhit = lhs.begin();
		auto rhit = rhs.begin();
		for (; lhit != lhs.end() && rhit != rhs.end(); lhit++, rhit++)
			if (*rhit != *lhit)
				return false;
		return true;
	}

	string& string::insert(size_t pos, const string& str) {

	}

	string& string::insert(size_t pos, const string& str, size_t subpos, size_t sublen = npos) {
		//插入str subpos 的 sublen长度的字符串
	}

	string& string::insert(size_t pos, const char* s) {

	}

	string& string::insert(size_t pos, const char* s, size_t n) {

	}

	string& string::insert(size_t pos, size_t n, char c) {
		iterator it  = _start + pos;
		insert(it, n, c);
		return *this;
	}

	string::iterator string::insert(iterator p, char c) {
		if (capacity() > size())
			return insert(p, 1, c);
	}

	string::iterator string::insert(iterator p, size_t n, char c) {
		auto lengthOfInsert = capacity() - size();
		if (n <= lengthOfInsert) {
			//先进行右移，然后插入
			for (auto it = end() - 1; it >= p; it--)
				*(it + n) = *it;
			SimpleSTL::uninitialized_fill_n(p, n, c);
			_finish = _finish + n;
			return (p + n);
		}
		else {
			//该调用确定会开辟新的内存空间
			return insert_aux_filln(p, n ,c);
		}
	}

	bool operator==(const string& lhs, const char* rhs) {
		auto len = strlen(rhs);
		if (len == lhs.size()) {
			const char* p = rhs;
			for (auto cit = lhs.begin(); cit != lhs.end() && p != rhs + len; cit++, p++) {
				if (*p != *cit)
					return false;
			}
			return true;
		}
		return false;
	}

	bool operator==(const char* s, const string& rhs) {
		return rhs == s;
	}
}
