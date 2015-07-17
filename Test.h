#ifndef SRC_TEST_H_
#define SRC_TEST_H_
#include <stdio.h>
class Test
{
public:
	Test();
	virtual ~Test();
	void func1(void* userdata, int len)
	{
		printf("func 1\n");
	}

	void func2(void* userdata, int len)
	{
		printf("func 2\n");
	}

	void func3(void* userdata, int len)
	{
		printf("func 3\n");
	}
};

#endif /* SRC_TEST_H_ */
