
#include <iostream>
#include <cstring>
#include "CustomHeap.hpp"

struct my_struct
{
	int a;
	double b;
};

int main()
{
	Heap myHeap(2400);
	void *my_ptr;
	size_t my_data = 45463;
	my_struct my_data2;
	my_data2.a = 6;
	my_data2.b = 7.7;
	void *my_ptr2;

	myHeap.Allocate(sizeof(my_data), &my_ptr);
	std::cout << "Allocated " << my_ptr << std::endl;
	memcpy(my_ptr, &my_data, sizeof(my_data));


	myHeap.Allocate(sizeof(my_data2), &my_ptr2);
	std::cout << "Allocated " << my_ptr2 << std::endl;
    memcpy(my_ptr2, &my_data2, sizeof(my_data2));

	std::cout << *((size_t *) my_ptr) << std::endl;
	std::cout << ((my_struct *) my_ptr2)->a << " " << ((my_struct *) my_ptr2)->b << std::endl;


	myHeap.Free(my_ptr);
	std::cout << ((my_struct *) my_ptr2)->a << " " << ((my_struct *) my_ptr2)->b << std::endl;
	myHeap.Free(my_ptr2);
}


