#include "camera.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "CAMERA"

typedef glm::vec3 VEC3;
typedef glm::mat4 MAT4;

constexpr const char* VERTEX_SHADER_PATH = "./shaders/shader.vs";
constexpr const char* FRAGMENT_SHADER_PATH = "./shaders/shader.fs";


void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height);
void processInput(GLFWwindow* window, VEC3 &cameraPos, const VEC3 cameraFront, const VEC3 cameraUp, const float deltaTime);

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

	// this view likes to walk straight
	VEC3 cameraPos = VEC3(0.0f, 0.0f, 3.0f);
	VEC3 cameraFront = VEC3(0.0f, 0.0f, -1.0f);
	VEC3 cameraUp = VEC3(0.0f, 1.0f, 0.0f);

	float deltaTime(0.0f), lastFrame(0.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get deltaTime
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, cameraPos, cameraFront, cameraUp, deltaTime);

		// use texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i)
		{
			MAT4 model(1.0f), view(1.0f), projection(1.0f);
			model = glm::translate(model, cubePositions[i]);
			if (i % 3 == 0)
			{
				model = glm::rotate(model, glm::radians(20.0f * i), VEC3(1.0f, 0.3f, 0.5f));
			}
			else
			{
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * i), VEC3(1.0f, 0.3f, 0.5f));
			}

			//// set camera view matrix
			//// 1. set the translate
			// VEC3 cameraPos = VEC3(0.0f, 0.0f, 3.0f);

			// 2. set 3 axis - z pos
			// VEC3 cameraTarget = VEC3(0.0f, 0.0f, 0.0f);
			// VEC3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
			// 2. set 3 axis - x pos
			// VEC3 up = VEC3(0.0f, 1.0f, 0.0f);
			// VEC3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
			// 2. set 3 axis - y pos
			// VEC3 cameraUp = glm::cross(cameraDirection, cameraRight);

			// can use glm::mat4 to manually create a lookat
			// however a more convenient way is:
			// MAT4 view(1.0f);
			// view = glm::lookAt(cameraPos, cameraTarget, up);

			// This view likes to dance circle
			// float camX = sin((float)glfwGetTime()) * 10.0f;
			// float camZ = cos((float)glfwGetTime()) * 10.0f;
			// view = glm::lookAt(VEC3(camX, 0.0f, camZ), VEC3(0.0f, 0.0f, 0.0f), VEC3(0.0f, 1.0f, 0.0f));

			// use Euler angles and mouse to control the cameraFront


			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);


			shader.setMat4("model", model);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
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

void processInput(GLFWwindow* window, VEC3 &cameraPos, const VEC3 cameraFront, const VEC3 cameraUp, const float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;  // convert the speed from m/s to m/frame
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
}