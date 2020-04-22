#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

namespace ShaderSource
{
	const char* const vertex = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos, 1.0f);\n"
		"}\0";

	const char* const fragment = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = ourColor;\n"
		"}\0";

	const char* const vertexWithColor = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (locaiton = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos, 1.0f);\n"
		"ourColor = aColor;\n"
		"}\0";

	const char* const fragmentWithColor = "#version 330 core\n"
		"in vec3 ourColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(ourColor, 1.0f);\n"
		"}\0";
}

#endif // !SHADERSOURCE_H

