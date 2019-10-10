#pragma once
#include <cstdlib>

namespace SimpleSTL {
	class alloc {
	private:
		enum EAlign { ALIGN = 8 };  //С�������ϵ��ñ߽�
		enum EMaxBytes { MAXBYTES = 128 }; //С����������ޣ�ʣ�µ���malloc����
		enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN) }; //free-list �ĸ���
		enum ENObjs { NOBJS = 20 }; //ÿ�����ӵĽ������

	private:
		//free list �Ľ�㹹��
		union obj {
			union obj* next;
			char client[1];
		};

		static obj* free_list[ENFreeLists::NFREELISTS];

	private:
		static char* start_free; // �ڴ����ʼλ��
		static char* end_free;    //�ڴ�ؽ���λ��
		static size_t heap_size;

	private:

		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
		}
		//���������С������ʹ�õ�n��free-list��n��0��ʼ����
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void* refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣬nobjs���ܻή��
		static char* chunk_alloc(size_t size, size_t& nobjs);

	public:
		static void* allocate(size_t bytes);
		static void deallocate(void* ptr, size_t bytes);
		static void* reallocate(void* ptr, size_t bytes, size_t new_sz);
		//
	};
}