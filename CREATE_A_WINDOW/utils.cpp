#include "utils.h"

namespace utils 
{
	// try to use file reading to replace the direct assignment
	std::string readFileIntoString(const char* filename)
	{
		std::ifstream in(filename);
		std::string str((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		return str;
	}

	int test()
	{
		std::string str;
		str = utils::readFileIntoString("vertexShader.txt");
		std::cout << str << std::endl;
		return 0;
	}
}
