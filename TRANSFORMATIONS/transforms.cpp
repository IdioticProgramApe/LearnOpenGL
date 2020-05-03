#include <iostream>
#include "transforms.h"


int main()
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

	// initialize a indentity matrix
	glm::mat4 trans(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;

}