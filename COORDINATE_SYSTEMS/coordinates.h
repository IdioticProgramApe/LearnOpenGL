#ifndef COORDINATES_H
#define COORDINATES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <../External/stb/stb_image.h>

constexpr const char* TEX_CONTAINER = "../src/textures/container.jpg";
constexpr const char* TEX_WALL = "../src/textures/wall.jpg";
constexpr const char* TEX_BORDERLANDS = "../src/textures/borderlands3.bmp";

#endif // !COORDINATES_H
