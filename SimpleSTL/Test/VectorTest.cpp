#include "VectorTest.h"

namespace SimpleSTL {
	namespace VectorTest {

		void testCase1() {
			stdVec<std::string> v1(10, "meisays");
			tsVec<std::string> v2(10, "meisays");
			//SimpleSTL::vector<std::string> t(10);
			assert(SimpleSTL::Test::container_equal(v1, v2));

			//测试
			tsVec<int> vv(static_cast<size_t>(10), 100); //该构造函数的实例化为-迭代器


			stdVec<std::string> v3(10);
			tsVec<std::string> v4(10);

			assert(SimpleSTL::Test::container_equal(v3, v4));

			std::array<std::string, 3> arr = { "abc", "def", "ghi" };
			stdVec<std::string> v5(std::begin(arr), std::end(arr));
			tsVec<std::string> v6(std::begin(arr), std::end(arr));
			assert(SimpleSTL::Test::container_equal(v5, v6));
		}

		void testCase2() {
			stdVec<int> temp1(10, 0);
			tsVec<int> temp2(10, 0);
			assert(SimpleSTL::Test::container_equal(temp1, temp2));

			auto v1(temp1); //拷贝构造器
			auto v2(temp2);
			assert(SimpleSTL::Test::container_equal(v1, v2));

			auto v3(std::move(temp1)); //
			auto v4(std::move(temp2));
			assert(SimpleSTL::Test::container_equal(v3, v4));

			auto v5 = v1;
			auto v6 = v2;
			assert(SimpleSTL::Test::container_equal(v5, v6));

			auto v7 = std::move(v5);
			auto v8 = std::move(v6);
			assert(SimpleSTL::Test::container_equal(v7, v8));

		}

		void testCase3(){
			stdVec<int> stdV;
			tsVec<int> v1, v2;
			for (int i = 0; i <= 10000; i++) {
				v1.push_back(i);
				v2.push_back(i);
				stdV.push_back(i);
			}

			assert((v1 == v2));
			assert(SimpleSTL::Test::container_equal(v1, stdV));
			assert(!(v1 != v2));

			tsVec<std::string> v3, v4;
			for (int i = 0; i <= 1000; i++) {
				v3.push_back(std::string('c', i));
				v4.push_back(std::string('c', i));
			}

			assert((v3 == v4));
		}

		void testCase4() {
			tsVec<int> myVector;
			for (int i = 1; i <= 100; i++) myVector.push_back(i);

			auto i = 1;
			for (auto it = myVector.begin(); it != myVector.end(); it++, i++)
				assert(*it == i);

			i = 1;
			for (auto cit = myVector.cbegin(); cit != myVector.cend(); cit++, i++)
				assert(*cit == i);

		}

		void testCase5() {
			tsVec<int> myvector(5);
			int i = 0;
			tsVec<int>::reverse_iterator rit = myvector.rbegin();
			for (; rit != myvector.rend(); rit++)
				*rit = ++i;

			i = 5;
			for (tsVec<int>::iterator it = myvector.begin(); it != myvector.end(); it++, i--)
				assert(*it == i);

			i = 1; 
			for (tsVec<int>::reverse_iterator rit = myvector.rbegin(); rit != myvector.rend(); rit++, i++)
				assert(*rit == i);
		}

		void testCase6() {
			tsVec<int> v(11, 0);
			assert(v.size() == 11);

			v.resize(5);
			assert(v.size() == 5);

			v.resize(20);
			assert(v.size() == 20);
		}

		void testCase7() {
			tsVec<int> v;
			v.reserve(20);
			assert(v.capacity() == 20);
		}

		void testCase8() {
			stdVec<int> v1(10);
			tsVec<int> v2(10);
			for (unsigned i = 0; i < 10; i++) {
				v1[i] = i;
				v2[i] = i;
			}
			assert(SimpleSTL::Test::container_equal(v1, v2));

			v1.front() = 99;
			v2.front() = 99;

			v1.front() = 100;
			v2.front() = 100;

			assert(SimpleSTL::Test::container_equal(v1, v2));
		}

		void testCase9() {
			stdVec<int> v1(5);
			tsVec<int> v2(5);

			auto p1 = v1.data();
			auto p2 = v2.data();

			*p1 = 10; ++p1; *p1 = 20; p1[2] = 100;
			*p2 = 10; ++p2; *p2 = 20; p2[2] = 100;

			assert(SimpleSTL::Test::container_equal(v1, v2));
		}

		void testCase10() {
			tsVec<int> foo(3, 100); //three ints with a value of 100
			tsVec<int> bar(2, 200); //Two ints with a value of 200

			assert(SimpleSTL::Test::container_equal(foo, stdVec<int> {100, 100, 100}));
			assert(SimpleSTL::Test::container_equal(bar, stdVec<int> {200, 200}));

			foo.swap(bar);
		}

		void testCase11() {
			stdVec<std::string> v1;
			tsVec<std::string> v2;

			v1.push_back("hello "); v1.push_back("world");
			v2.push_back("hello "); v2.push_back("world");
			assert(SimpleSTL::Test::container_equal(v1, v2));

			v1.pop_back();
			v2.pop_back();
			assert(SimpleSTL::Test::container_equal(v1, v2));
		}

		void testCase12() {
			stdVec<int> v1;
			tsVec<int> v2;

			v1.insert(v1.begin(), 0);
			v2.insert(v2.begin(), 0);
			assert(SimpleSTL::Test::container_equal(v1, v2));

			v1.insert(v1.end(), 9);
			v2.insert(v2.end(), 9);
			assert(SimpleSTL::Test::container_equal(v1, v2));
			
			v1.insert(v1.begin() + v1.size() / 2, 10, 0);
			v2.insert(v2.begin() + v2.size() / 2, 10, 0);
			assert(SimpleSTL::Test::container_equal(v1, v2));

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			v1.insert(v1.end(), std::begin(arr), std::end(arr));
			v2.insert(v2.end(), std::begin(arr), std::end(arr));
			assert(SimpleSTL::Test::container_equal(v1, v2));
		}

		void testCase13() {
			stdVec<int> v1;
			tsVec<int> v2;

			for (int i = 0; i <= 10; i++) {
				v1.push_back(i);
				v2.push_back(i);
			}
			assert(SimpleSTL::Test::container_equal(v1, v2));

			v1.erase(v1.begin() + 5);
			v2.erase(v2.begin() + 5);

			assert(SimpleSTL::Test::container_equal(v1, v2));

			v1.erase(v1.begin(), v1.begin() + 3);
			v2.erase(v2.begin(), v2.begin() + 3);
			assert(SimpleSTL::Test::container_equal(v1, v2));
		}

		void testCase14() {
			tsVec<int> foo(3, 100);
			tsVec<int> bar(2, 200);
			assert(!(foo == bar));
			assert((foo != bar));
		}

		class TestItem {
		public:
			TestItem() {
				++count;
			}
			TestItem(const TestItem& other) {
				++count;
			}
			virtual ~TestItem() {
				--count;
			}

			static int getCount() {
				return count;
			}
		private:
			static int count;
		};

		int TestItem::count = 0;
		void testCase15() {
			assert(TestItem::getCount() == 0);
			{
				typedef SimpleSTL::vector<TestItem> Tvector;
				Tvector t(10); //count 10
				t.push_back(TestItem()); // count  11
				t.push_back(TestItem()); //count 12
				t.push_back(TestItem()); //count 13
				t.insert(t.begin(), t.begin(), t.begin() + 1); //count 14

				TestItem shoT;
			}
			assert(TestItem::getCount() == 0); //确保没有内存泄漏
		}

		void testAllCases() {
			//testCase1();
			//testCase2();
			//testCase3();
			//testCase4();
			//testCase5();
			//testCase6();
			//testCase7();
			//testCase8();
			//testCase9();
			//testCase10();
			//testCase11();
			testCase12();
			testCase13();
			testCase14();
			testCase15();
		}
	}

}