#pragma once

#include "Allocator.h"
#include "ReverseIterator.h"
#include "UninitializedFunctions.h"
#include "Utility.h"

#include <cstring>
#include <type_traits>

namespace SimpleSTL {

	class stirng
	{
		typedef char      value_type;
		typedef char*     iterator;
		typedef const char* const_iterator;
		typedef reverse_iterator_t<char*> reverse_iterator;
	};

}