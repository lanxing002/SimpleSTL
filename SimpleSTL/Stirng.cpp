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
		//ֻ����n λ����
		allocateAndCopy(s, s + n);
	}

	//����������
	string::string(const string& str) {
		allocateAndCopy(str.begin(), str.end());
	}


	string::string(string&& str) {
		//��ֵ���� 
		moveData(str);
	}

	string::string(const string& str, size_t pos, size_t len) {
		//�������ݵĳ��ȣ���Ӧ�����ַ����ĳ���
		len = changeVarWhenEuqalNPOS(len, str.size(), pos);
		allocateAndCopy(str._start + pos, str._start + pos + len);
	}


	//���ƺ���
	string& string::operator=(char c) {
		//�������ٸö��� �ڴ�ռ�
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
		//newStart ���¶������ʼ��ַ
		iterator newStart = dataAllocator::allocate(newCapacity);
		iterator newFinish = SimpleSTL::uninitialized_copy(_start, p, newStart);
		//����char
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
			//���βԪ��
			auto lengthOfInsert = n - size();
			_finish = SimpleSTL::string::insert_aux_filln(_finish, lengthOfInsert, c);
		}
		else if (n >= capacity()) {
			//���·ַ��ڴ�
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
		//��֤�������������� n
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
		//���ٶ���
		dataAllocator::destroy(_start, _finish);
		//�ͷ��ڴ�ռ�
		dataAllocator::deallocate(_start, _endOfStorage - _start);
	}

	string::size_type string::getNewCapacity(size_type len) const {
		//����չ���ȷ�����������size
		size_type oldCapacity = _endOfStorage - _finish;
		auto res = SimpleSTL::max(oldCapacity, len);
		//Ϊʲô���� capacity ������
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
		//����Դ����
		str._start = str._finish = str._endOfStorage = nullptr;
	}

	size_t string::changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue) const {
		return (var == npos ? minuend - minue : var);
	}

	//��Ԫ������ ���������
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
		//����str subpos �� sublen���ȵ��ַ���
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
			//�Ƚ������ƣ�Ȼ�����
			for (auto it = end() - 1; it >= p; it--)
				*(it + n) = *it;
			SimpleSTL::uninitialized_fill_n(p, n, c);
			_finish = _finish + n;
			return (p + n);
		}
		else {
			//�õ���ȷ���Ὺ���µ��ڴ�ռ�
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
