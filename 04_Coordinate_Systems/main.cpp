#include "Textures.h"


#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "COORDINATE SYSTEMS"

#define DIM_MODE 3

typedef glm::vec3 VEC3;
typedef glm::mat4 MAT4;

constexpr const char* VERTEX_SHADER_PATH = "./shaders/Vertex.glsl";
constexpr const char* FRAGMENT_SHADER_PATH = "./shaders/Fragment.glsl";


void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::WINDOW::INITIALIZATION" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::LOADING" << std::endl;
		return -1;
	}

	// get an instance object of shader
	glEnable(GL_DEPTH_TEST);
	Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

#if DIM_MODE == 2
	// define the vertices and indices
	float vertices[] = {
		// positions        // texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,      // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,      // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,      // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f       // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle 
		1, 2, 3  // second triangle
	};

	unsigned int VBO, EBO, VAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// get geomatrical information x y z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// get texture coord information s t (r)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#elif DIM_MODE == 3
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// using different positions to create multiple identical cubes
	VEC3 cubePositions[] = {
		VEC3(0.0f,  0.0f,   0.0f),
		VEC3(2.0f,  5.0f, -15.0f),
		VEC3(-1.5f, -2.2f,  -2.5f),
		VEC3(-3.8f, -2.0f, -12.3f),
		VEC3(2.4f, -0.4f,  -3.5f),
		VEC3(-1.7f,  3.0f,  -7.5f),
		VEC3(1.3f, -2.0f,  -2.5f),
		VEC3(1.5f,  2.0f,  -2.5f),
		VEC3(1.5f,  0.2f,  -1.5f),
		VEC3(-1.3f,  1.0f,  -1.5f),
	};
	
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#endif

	// create variables and set up for image loading
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data;
	int width, height, nrChannels;

	unsigned int texture0, texture1;
	glGenTextures(1, &texture0);
	glGenTextures(1, &texture1);
	
	// first texture 
	glBindTexture(GL_TEXTURE_2D, texture0);

	// setting up wrap and filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// load first texture from source
	data = stbi_load(TEX_CONTAINER, &width, &height, &nrChannels, 0);
	if (data)
	{
		// pass the image data information into this GL_TEXTURE_2D
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::STBI_LOAD FAILED" << std::endl;
	}
	stbi_image_free(data);

	// second texture
	glBindTexture(GL_TEXTURE_2D, texture1);

	// setting up warp and filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	data = stbi_load(TEX_BORDERLANDS, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::STBI_LOAD FAILED" << std::endl;
	}
	stbi_image_free(data);

	shader.use();
	shader.setInt("ourTexture0", 0);
	shader.setInt("ourTexture1", 1);
	shader.setFloat("ratio", 0.2f);

	// how to dynamically get viewport width and height ?
	// create a orthographic projection matrix
	// MAT4 projOrtho = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	// create a perspective projection matrix
	// MAT4 projPersp = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.f);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		// use texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

#if DIM_MODE == 2
		// setting coords transform matrices
		// don't forget to initialize the MAT4 matrices with a Identity Matrix
		MAT4 model(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), VEC3(1.0f, 0.0f, 0.0f));

		MAT4 view(1.0f);
		view = glm::translate(view, VEC3(0.0f, 0.0f, -3.0f));

		MAT4 projection(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
		
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#elif DIM_MODE == 3
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i)
		{
			MAT4 model(1.0f), view(1.0f), projection(1.0f);
			{
				model = glm::translate(model, cubePositions[i]);
				if (i % 3 == 0)
				{
					model = glm::rotate(model, glm::radians(20.0f * i), VEC3(1.0f, 0.3f, 0.5f));
				}
				else
				{
					model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * i), VEC3(1.0f, 0.3f, 0.5f));
				}
				model = glm::scale(model, glm::vec3(0.5f));
			}
			
			{
				view = glm::translate(view, VEC3(0.0f, 0.0f, -3.0f));
				// view = glm::translate(view, VEC3(sin((float)glfwGetTime()) * 3, 0.0f, cos((float)glfwGetTime()) * 5));
			}
			
			{
				projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
			}

			shader.setMat4("model", model);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
#endif
		glBindTexture(GL_TEXTURE_2D, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
#if DIM_MODE == 2
	glDeleteBuffers(1, &EBO);
#endif
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);

	glDisable(GL_DEPTH_TEST);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}