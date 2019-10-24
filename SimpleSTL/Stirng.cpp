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

	string& string::operator=(const string& str) {
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
		auto newCapacity = capacity() + res;
		return newCapacity;
	}

	void string::allocateAndFillN(size_t n, char c) {
		_start = dataAllocator::allocate(n);
		_finish = SimpleSTL::uninitialized_fill_n(_start, n, c);
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
		insert(_start + pos, str.begin(), str.end());
		return *this;
	}

	string& string::insert(size_t pos, const string& str, size_t subpos, size_t sublen) {
		//����str subpos �� sublen���ȵ��ַ���
		sublen = changeVarWhenEuqalNPOS(sublen, str.size(), npos);
		insert(_start + pos, str.begin() + subpos, str.begin() + subpos + sublen);
		return *this;
	}

	string& string::insert(size_t pos, const char* s) {
		insert(_start + pos, s, s + strlen(s));
		return *this;
	}

	string& string::insert(size_t pos, const char* s, size_t n) {
		//��� n ���� string.size ��ô���� �׳��쳣��
		insert(_start + pos, s, s + n);
		return *this;
	}

	string& string::insert(size_t pos, size_t n, char c) {
		iterator it = _start + pos;
		insert(it, n, c);
		return *this;
	}

	string::iterator string::insert(iterator p, char c) {
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
			return insert_aux_filln(p, n, c);
		}
	}

	string& string::append(const string& str) {
		append(str.begin(), str.end());
		return *this;
	}

	string& string::append(const string& str, size_t subpos, size_t sublen) {
		sublen = changeVarWhenEuqalNPOS(sublen, str.size(), subpos);
		append(str.begin() + subpos, str.begin() + subpos + sublen);
		return *this;
	}

	string& string::append(const char* s) {
		append(s, s + strlen(s));
		return *this;
	}

	string& string::append(const char* s, size_t n) {
		append(s, s + n);
		return *this;
	}

	string& string::append(size_t n, char c) {
		insert(_finish, n, c);
		return *this;
	}

	string& string::operator+= (const string& str) {
		this->append(str);
		return *this;
	}

	string& string::operator+= (const char* s) {
		this->append(s);
		return *this;
	}

	string& string::operator+= (char c) {
		this->append(1, c);
		return *this;
	}

	string& string::erase(size_t pos, size_t len) {
		len = changeVarWhenEuqalNPOS(len, size(), pos);
		erase(_start + pos, _start + pos + len);
		return *this;
	}

	string::iterator string::erase(iterator p) {
		return erase(p, p + 1);
	}

	string::iterator string::erase(iterator first, iterator last) {
		auto move_length = _finish - last;
		for (int i = 0; i != move_length; i++)
			*(first + i) = *(last + i);
		dataAllocator::destroy(first + move_length, _finish);
		_finish = first + move_length;
		return first;
	}

	string& string::replace(size_t pos, size_t len, const string& str) {
		return replace(_start + pos, _start + pos + len, str.begin(), str.end());
	}

	string& string::replace(iterator i1, iterator i2, const string& str) {
		return replace(i1, i2, str.begin(), str.end());
	}

	string& string::replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen) { //�����������ĳ���
		sublen = changeVarWhenEuqalNPOS(sublen, str.size(), sublen);
		return replace(_start + pos, _start + pos + len,
			str.begin() + subpos, str.begin() + subpos + sublen);
	}

	string& string::replace(size_t pos, size_t len, const char* s) {
		return replace(_start + pos, _start + pos + len, s, s + strlen(s));
	}

	string& string::replace(iterator i1, iterator i2, const char* s) {
		return replace(i1, i2, s, s + strlen(s));
	}

	string& string::replace(size_t pos, size_t len, const char* s, size_t n) {
		return replace(_start + pos, _start + pos + len, s, s + n);
	}



	string& string::replace(iterator i1, iterator i2, const char* s, size_t n) {
		return replace(i1, i2, s, s + n);
	}

	string& string::replace(iterator i1, iterator i2, size_t n, char c) {
		auto ptr = erase(i1, i2);
		insert(ptr, n, c);
		return *this;
	}

	string& string::replace(size_t pos, size_t len, size_t n, char c) {
		return replace(_start + pos, _start + pos + len, n, c);
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

	bool operator!= (const string& lhs, const string& rhs){	return !(lhs == rhs);}
	bool operator!= (const char* lhs, const string& rhs) { return !(rhs == lhs); }
	bool operator!= (const string& lhs, const char* rhs) { return !(rhs == lhs); }


	bool operator<  (const string& lhs, const string& rhs) { return lhs.compare(rhs) < 0; }
	bool operator<  (const char* lhs, const string& rhs) { return rhs.compare(rhs) > 0;	}
	bool operator<  (const string& lhs, const char* rhs) { return lhs.compare(rhs) < 0;	}

	bool operator<= (const string& lhs, const string& rhs) { return !(lhs > rhs); }
	bool operator<= (const char* lhs, const string& rhs) { return !(lhs > rhs); }
	bool operator<= (const string& lhs, const char* rhs) { return !(lhs > rhs); }
	bool operator>  (const string& lhs, const string& rhs) { return lhs.compare(rhs) > 0; }
	bool operator>  (const char* lhs, const string& rhs) { return rhs.compare(lhs) < 0; }
	bool operator>  (const string& lhs, const char* rhs) { return lhs.compare(rhs) < 0; }
	bool operator>= (const string& lhs, const string& rhs) { return !(lhs < rhs); }
	bool operator>= (const char* lhs, const string& rhs) { return !(lhs < rhs); }
	bool operator>= (const string& lhs, const char* rhs) { return !(lhs < rhs); }

	size_t string::rfind_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond)const {
		//if (cond < lengthOfS) // �����Ӵ��ȱ������ݳ�
		//	return npos;
		//  forѭ����Ϳ��Խ������ж��ˣ�û�ж���Ĵ���
		//size_t i, j;
		//for (i = pos ; i >= cond; i--) {
		//	for (j = lengthOfS - 1; j != 0; j--) {
		//		if (i + j  < lengthOfS) {
		//			//Ѱ�ҵ�stringͷ��
		//			return npos;
		//		}
		//		if (*(_start + i + j - lengthOfS) != cit[j]) //��β��ͷƥ��
		//			break;
		//	}
		//	if (j == 0) return i;
		//}
		//return npos;

		//����ʹ�ø�������

		//if (pos < cond + lengthOfS)
		//	return npos;

		int i, j;
		for (i = pos; i != cond - 1; i--) {
			for (j = 0; j != lengthOfS; j++) {
				if (*(_start + i + j) != cit[j])  //���������Χ���� ���ַ������� ����ô�죿
					break;

			}
			if (j == lengthOfS)
				return i;
		}
		return npos;
	}


	size_t string::find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond) const {
		size_t i, j;
		for (i = pos; i != cond; i++) {
			for (j = 0; j != lengthOfS; j++) {
				if (*(begin() + i + j) != cit[j])
					break;
			}
			if (j == lengthOfS) return i;
		}
		return npos;
	}

	int string::compare_aux(size_t pos, size_t len, const_iterator cit, size_t subpos, size_t sublen)const {
		size_t i, j;
		for(i = 0, j = 0; i != len && j != sublen; i++, j++){
				if (*(_start + i + pos) > *(cit + j + subpos)) return 1;
				else if (*(_start + i + pos) < *(cit + j + subpos)) return -1;
		}

		if (i == len && j == sublen) return 0;

		if (len > sublen) return 1;
		else return -1;
	}


	//size_t string::find_aux(const_iterator cit, size_t lengthOfS, size_t pos,  size_t cond) const {
	//	size_t i, j;
	//	for (i = pos; i != cond; i++) {
	//		for (j = pos; j != lengthOfS; j++) {
	//			if (*(begin() + i + j) != cit[j])
	//				break;

	//		}
	//		if (j == lengthOfS) return i;
	//	}
	//	return npos;
	//}

	size_t string::find(const string& str, size_t pos) const {
		return find_aux(str.begin(), pos, str.size(), size());
	}

	size_t string::find(const char* s, size_t pos) const {
		return find_aux(s, pos, strlen(s), size());
	}

	size_t string::find(const char* s, size_t pos, size_t n) const {
		return find_aux(s, pos, n, size());
	}

	size_t string::find(char c, size_t pos) const {
		size_t i = pos;
		for (; i != size(); i++)
			if (*(_start + i) == c)
				return i;
		return npos;
	}

	size_t string::rfind(const string& str, size_t pos ) const {
		auto lengthOfS = str.size();
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		return rfind_aux(str.begin(), pos, str.length(), 0);
	}

	size_t string::rfind(const char* s, size_t pos) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		return rfind_aux(s, pos, strlen(s), 0);
	}

	size_t string::rfind(const char* s, size_t pos, size_t n) const {
		//auto lengthOfS = strlen(s);
		//pos ����ΪĬ��ֵ
		return rfind_aux(s, pos, n, 0);
	}

	size_t string::rfind(char c, size_t pos) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		size_t target;

		for (target = pos; target != 0; target--)
			if (*(_start + target) == c)
				return target;

		//��һ��Ԫ�ػ�û�м��
		if (*(_start + 0) == c)
			return 0;
		return npos;
	}

	size_t string::find_first_of(const string& str, size_t pos) const {
		return find_first_of(str.begin(), pos, str.size());
	}

	size_t string::find_first_of(const char* s, size_t pos) const {
		return find_first_of(s, pos, strlen(s));
	}

	size_t string::find_first_of(const char* s, size_t pos, size_t n) const {
		if (pos > size() - 1)
			return npos; //Ѱ�ҷ�Χ���
		
		size_t i, j;
		for (i = pos; i != size(); i++)
			for (j = 0; j < n; j++)
				if (*(_start + i) == *(s + j))
					return i;
		return npos;
	}

	size_t string::find_first_of(char c, size_t pos) const {
		if (pos > size() - 1)
			return npos;

		size_t i;
		for (i = pos; i != size(); i++)
			if (*(_start + i) == c)
				return i;
		return npos;
	}

	size_t string::find_last_of(const string& str, size_t pos) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		return find_last_of(str.begin(), pos, str.size());
	}

	size_t string::find_last_of(const char* s, size_t pos) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		return find_last_of(s, pos, strlen(s));
	}

	size_t string::find_last_of(const char* s, size_t pos, size_t n) const {
		size_t i, j;
		for (i = pos; i != 0; i--)
			for (j = 0; j < n; j++)
				if (*(_start + i) == *(s + j))
					return i;
		//����size_t ��Զ��Ϊ����������ж�����Ϊ����������
		for (j = 0; j < n; j++)
			if (*(_start  + 0) == *(s + j))
				return 0;
		return npos;
	}

	size_t string::find_last_of(char c, size_t pos ) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		size_t i, j;
		for (i = pos; i != 0; i--)
			if (*(_start + i) == c)
					return i;
		//����size_t ��Զ��Ϊ����������ж�����Ϊ����������
		if (*(_start + 0) == c)
				return 0;
		return npos;
	}

	size_t string::find_first_not_of(const string& str, size_t pos) const {
		return find_first_not_of(str.begin(), pos, str.size());
	}

	size_t string::find_first_not_of(const char* s, size_t pos) const {
		return find_first_not_of(s, pos, strlen(s));
	}

	size_t string::find_first_not_of(const char* s, size_t pos, size_t n) const {
		size_t i, j;
		for (i = pos; i != size(); i++) {
			for (j = 0; j != n; j++) {
				if (*(_start + i) == *(s + j))
					break;

			}
			if (j == n)
				return i; // match
		}

		return npos;
	}

	//size_t string::find_first_not_of(char c, size_t pos = 0) const {}

	size_t string::find_last_not_of(const string& str, size_t pos) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		return find_last_not_of(str.begin(), pos, str.size());
	}

	size_t string::find_last_not_of(const char* s, size_t pos) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		return find_last_not_of(s, pos, strlen(s));
	}

	size_t string::find_last_not_of(const char* s, size_t pos, size_t n) const {
		size_t i, j;
		for (i = pos; i != 0; i--) {
			for (j = 0; j < n; j++)
				if (*(_start + i) == *(s + j)) break;
			if (j == n) return i;
		}
		//when i == 0
		for (j = 0; j < n; j++)
			if (*(_start + 0) == *(s + j)) return npos;
		
		return 0;
	}

	size_t string::find_last_not_of(char c, size_t pos) const {
		size_t i;
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		for (i = pos; i != 0; i++)
			if (*(_start + i) == c)
				break;


		if (i != 0) {
			return npos;
		}
		else {
			if (*(_start + 0) == c) return 0;
			else return npos;
		}
	}

	int string::compare(const string& str) const {
		return compare_aux(0, size(), str.begin(), 0, str.size());
	}

	int string::compare(size_t pos, size_t len, const string& str) const {
		return compare_aux(pos, len, str.begin(), 0, str.size());
	}

	int string::compare(size_t pos, size_t len, const string& str,
		size_t subpos, size_t sublen) const {
		return compare_aux(pos, len, str.begin(), subpos, sublen);
	}

	int string::compare(const char* s) const {
		return compare_aux(0, size(), s, 0, strlen(s));
	}

	int string::compare(size_t pos, size_t len, const char* s) const {
		return compare_aux(pos, len, s, 0, strlen(s));
	}

	int string::compare(size_t pos, size_t len, const char* s, size_t n) const {
		return compare_aux(pos, len, s, 0, n);
	}


	std::ostream& operator <<(std::ostream& os, const string& str) {
		for (auto it = str.begin(); it != str.end(); it++)
			os << *it;
		return os;
	}
	std::istream& operator >> (std::istream& is, string& str) {
		char ch;
		string::size_type oldSize = str.size(), index = 0;
		bool hasPrevBlank = false;

		//����ǰ���հ�
		while (is.get(ch)) {
			if (isblank(ch) || ch == '\n')
				hasPrevBlank = true;
			else break;
		}

		is.putback(ch);
		str.clear();

		while (is.get(ch)) {
			if (ch != EOF && !isblank(ch) && ch != '\n')
				str.push_back(ch);
			else break;
		}

		return is;
	}

	string operator+ (const string& lhs, const string& rhs) {
		string temp(lhs);
		return temp += rhs;
	}

	string operator+ (const string& lhs, const char* rhs) {
		string temp(lhs);
		return temp += rhs;
	}
	string operator+ (const char* lhs, const string& rhs) {
		string temp(lhs);
		return temp += rhs;
	}
	string operator+ (const string& lhs, char rhs) {
		string temp(lhs);
		return temp += rhs;
	}
	string operator+ (char lhs, const string& rhs) {
		string temp(1, lhs);
		return temp += rhs;
	}
}