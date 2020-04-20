#include "tuto.h"
#include "utils.h"
#include "exercises.h"

int main()
{
	int returncode = 0;
	// 1. test file read
	returncode |= utils::test();

	// 2. test tuto
	// returncode |= tuto::test();

	// 3. test exercise1
	// returncode |= exo::exercise1();

	// 4. test exercise2 (TODO: need debug)
	returncode |= exo::exercise2();

	return returncode;
}