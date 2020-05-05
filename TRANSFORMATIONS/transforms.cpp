#include "transforms.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "TRANSFORMATIONS"
#define EXO false

typedef glm::vec3 VEC3;
typedef glm::mat4 MAT4;

constexpr const char* VERTEX_SHADER_PATH = "./shaders/shader.vs";
constexpr const char* FRAGMENT_SHADER_PATH = "./shaders/shader.fs";


void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height);
void processInput(GLFWwindow* window);
MAT4 generateTransform();

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
	Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);


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
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);

		// use texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		shader.setMat4("transform", generateTransform());

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
#if EXO
		MAT4 tmpTrans(1.0f);
		tmpTrans = glm::translate(tmpTrans, VEC3(-0.5f, 0.5f, 0.0f));
		tmpTrans = glm::scale(tmpTrans, VEC3(sin((float)glfwGetTime()), sin((float)glfwGetTime()), 1.0f));
		shader.setMat4("transform", tmpTrans);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);

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

MAT4 generateTransform()
{
	float curValue = static_cast<float>(glfwGetTime());
	// initialize a indentity matrix
	MAT4 trans(1.0f);
	// begin to prepare the transformation matrices
	// 1. translation
	trans = glm::translate(trans, VEC3(0.5f * cos(curValue), -0.5f * sin(curValue), 0.0f));
	// 2. rotation
	trans = glm::rotate(trans, 3.0f * (float)glfwGetTime(), VEC3(0.0f, 0.0f, 1.0f));
	// 3. scaling
	trans = glm::scale(trans, VEC3(sin(curValue), cos(curValue), 1.0f));
	return trans;
}

// for The First Exercise in the TRANSFORMATIONS tuto
// switching the order around by first rotating and second translating
// we will have no impact on the ratation but the translation is not what we expected
// the reason is the translate vector is under the influence of the rotation as well.