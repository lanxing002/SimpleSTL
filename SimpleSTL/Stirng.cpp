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
		insert(_start + pos, str.begin(), str.end());
		return *this;
	}

	string& string::insert(size_t pos, const string& str, size_t subpos, size_t sublen) {
		//插入str subpos 的 sublen长度的字符串
		sublen = changeVarWhenEuqalNPOS(sublen, str.size(), npos);
		insert(_start + pos, str.begin() + subpos, str.begin() + subpos + sublen);
		return *this;
	}

	string& string::insert(size_t pos, const char* s) {
		insert(_start + pos, s, s + strlen(s));
		return *this;
	}

	string& string::insert(size_t pos, const char* s, size_t n) {
		//如果 n 大于 string.size 怎么处理， 抛出异常吗？
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
			//先进行右移，然后插入
			for (auto it = end() - 1; it >= p; it--)
				*(it + n) = *it;
			SimpleSTL::uninitialized_fill_n(p, n, c);
			_finish = _finish + n;
			return (p + n);
		}
		else {
			//该调用确定会开辟新的内存空间
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

	string& string::replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen) { //尽量复制最大的长度
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
		//if (cond < lengthOfS) // 查找子串比本身内容长
		//	return npos;
		//  for循环里就可以将条件判断了，没有多余的代码
		//size_t i, j;
		//for (i = pos ; i >= cond; i--) {
		//	for (j = lengthOfS - 1; j != 0; j--) {
		//		if (i + j  < lengthOfS) {
		//			//寻找到string头了
		//			return npos;
		//		}
		//		if (*(_start + i + j - lengthOfS) != cit[j]) //从尾到头匹配
		//			break;
		//	}
		//	if (j == 0) return i;
		//}
		//return npos;

		//不能使用负数特性

		//if (pos < cond + lengthOfS)
		//	return npos;

		int i, j;
		for (i = pos; i != cond - 1; i--) {
			for (j = 0; j != lengthOfS; j++) {
				if (*(_start + i + j) != cit[j])  //如果搜索范围超出 本字符串长度 ，怎么办？
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
		//pos 可能为默认值
		return rfind_aux(s, pos, n, 0);
	}

	size_t string::rfind(char c, size_t pos) const {
		pos = changeVarWhenEuqalNPOS(pos, size(), 1);
		size_t target;

		for (target = pos; target != 0; target--)
			if (*(_start + target) == c)
				return target;

		//第一个元素还没有检查
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
			return npos; //寻找范围溢出
		
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
		//由于size_t 永远不为负数，因此判断条件为零算作特例
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
		//由于size_t 永远不为负数，因此判断条件为零算作特例
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

		//跳过前导空白
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