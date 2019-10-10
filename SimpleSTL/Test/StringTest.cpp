#include "StringTest.h"

#include <fstream>

namespace SimpleSTL {
	namespace StringTest {
		void testCase1() {
			const char* ptr = "hello world";

			stdStr s1(ptr);
			tsStr s2(ptr);
			assert(SimpleSTL::Test::container_equal(s1, s2));

			stdStr s3(ptr, 5);
			stdStr s4(ptr, 5);
			assert(SimpleSTL::Test::container_equal(s3, s4));

			stdStr s5(10, 'z');
			tsStr s6(10, 'z');
			assert(SimpleSTL::Test::container_equal(s5, s6));

			char arr[] = "zouxiaohang love cpp";
			stdStr s7(std::begin(arr), std::end(arr));
			tsStr s8(std::begin(arr), std::end(arr));
			assert(SimpleSTL::Test::container_equal(s7, s8));

			std::cout << std::string("all cases pass") << std::endl;
		}

		void testCase2() {
			stdStr temp1("hello, world");
			tsStr temp2("hello, world");


			stdStr s1(temp1);
			tsStr s2(temp2);
			assert(SimpleSTL::Test::container_equal(s1, s2));

			//右值引用
			stdStr s3(std::move(s1));
			tsStr s4(std::move(s2));
			assert(SimpleSTL::Test::container_equal(s3, s4));

			stdStr s5(temp1, 1);
			tsStr s6(temp2, 1);
			assert(SimpleSTL::Test::container_equal(s5, s6));



			stdStr s7(temp1, 0, 5);
			tsStr s8(temp2, 0, 5);
			assert(SimpleSTL::Test::container_equal(s7, s8));

			std::cout << std::string("the cases pass") << std::endl;

		}

		void testCase3() {
			// '=' 复制运算符号重载
			stdStr t1("hello, world");
			tsStr t2("hello, world");

			//默认构造器吗， 
			stdStr s1; s1 = 'a';
			tsStr s2; s2 = 'a';
			assert(SimpleSTL::Test::container_equal(s1, s2));

			stdStr s3; s3 = "hello";
			tsStr s4; s4 = "hello";
			assert(SimpleSTL::Test::container_equal(s3, s4));

			stdStr s5; s5 = t1;
			tsStr s6; s6 = t2;
			assert(SimpleSTL::Test::container_equal(s5, s6));

			std::cout << std::string("the cases pass") << std::endl;
		}

		void testCase4() {
			// SimpleSTL::string 和 std::string 混合测试
			tsStr str("Test String");
			stdStr s(str.begin(), str.end());
			auto i = 0;
			for (tsStr::iterator it = str.begin(); it != str.end(); it++, i++) {
				assert(*it == s[i]);
			}

			i = str.size() - 1;
			for (tsStr::reverse_iterator it = str.rbegin(); it != str.rend(); ++it, --i) {
				assert(*it == s[i]);
			}

			//i = str.size() - 1;
			//std::cout << "string:-->" << std::endl;
			//for (tsStr::iterator it = str.end(); it >= str.begin(); it--, i--) {
			//	std::cout << *it;
			//	assert(*it == s[i]);
			//}
			//std::cout << std::endl;
			std::cout << std::string("the cases pass") << std::endl;
		}

		void testCase5() {
			tsStr s;
			assert(s.size() == 0);
			assert(s.length() == 0);

			s = "hello, world";
			assert(s.size() == 12);
			assert(s.length() == 12);

			std::cout << std::string("the cases pass") << std::endl;

		}

		void testCase6() {
			stdStr s1("hello, world");
			tsStr s2("hello, world");


			s1.resize(5);
			s2.resize(5);
			assert(SimpleSTL::Test::container_equal(s1, s2));

			s1.resize(20, 'a');
			s2.resize(20, 'a');
			assert(SimpleSTL::Test::container_equal(s1, s2));

			s1.resize(6, 'z');
			s2.resize(6, 'z');
			assert(SimpleSTL::Test::container_equal(s1, s2));


			s1.resize(100);
			s2.resize(100);
			assert(SimpleSTL::Test::container_equal(s1, s2));

			std::cout << std::string("the cases pass") << std::endl;
		}

		void testCase7() {
			tsStr s;
			s.reserve(10);
			assert(s.capacity() == 10);
			std::cout << std::string("the cases pass") << std::endl;

		}

		void testCase8() {
			tsStr s;
			assert(s.empty());

			s = "hello world";
			assert(!s.empty());

			s.clear();
			assert(s.empty());

			std::cout << std::string("the cases pass") << std::endl;
		}

		void testCase9() {
			//测试 [] 运算符， == 运算符号
			tsStr s;
			s.resize(10);
			for (auto i = 0; i != s.size(); i++) {
				s[i] = 'a' + i;
			}

			assert(s == "abcdefghij");
			
			s.back() = 'Z';
			s.front() = 'A';
			assert(s == "AbcdefghiZ");

			std::cout << std::string("the cases pass") << std::endl;
		}

		void testCase10() {
			stdStr s1;
			tsStr s2;
			for (int i = 0; i != 10; i++) {
				s1.push_back('a' + i);
				s2.push_back('a' + i);
			}

			assert(SimpleSTL::Test::container_equal(s1, s2));
		}

		void testAllCases() {
			//testCase1();
			//testCase3();
			//testCase4();
			//testCase5();
			//testCase6();
			//testCase7();
			//testCase8();
			//testCase9();
			testCase10();
		}
	}
}