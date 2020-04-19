#include "tuto.h"

int main()
{
	int returncode = 0;
	// 1. test file read
	returncode |= utils::test();
	// 2. test tuto
	returncode |= tuto::test();
	return returncode;
}