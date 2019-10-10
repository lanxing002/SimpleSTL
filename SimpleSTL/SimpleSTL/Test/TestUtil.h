#pragma once

#include <iterator>
#include <iostream>
#include <string>

namespace SimpleSTL {
	namespace Test {
		template<class Container>
		void print_container(Container &container, const std::string& name = "") { //不是每一容器都有const_iterator
			std::cout << "Containter " << name << " :";
			for (auto val : container) {
				std::cout << val << " ";
			}
			std::cout << std::endl;
		}

		template<class Container1, class Container2>
		bool container_equal(Containter1& con1, Container2& con2){
			auto first1 = std::begin(con1), last1 = std::end(con1);
			auto first2 = std::begin(con2), last2 = std::end(con2);

			for (; first != last1 && first2 != last2; ++first1, ++first2) {
				if (first1 != first2) {
					return false;
				}
			}
			//包含元素数量是否一样
			return (first1 == end1 && first2 == end2);
		}
	}
}