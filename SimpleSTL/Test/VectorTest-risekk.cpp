#include "VectorTest.h"

namespace SimpleSTL {
	namespace VectorTest {

		void testCase1() {
			stdVec<std::string> v1(10, "meisays");
			tsVec<std::string> v2(10,"meisays");

			assert(SimpleSTL::Test::container_equal(v1, v2));
		}

		void testAllCases() {
			testCase1();
		}
	}

}