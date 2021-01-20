#include "tuto.h"


int main()
{
	int returncode = 0;

	// 1. test tuto uniform
	// returncode |= tuto::uniform();

	// 2. test tuto color vertex attribute
	returncode |= tuto::colorAttrib();

	return returncode;
}