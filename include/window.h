#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"


GLFWwindow* initializeWindow(int* windowFlag);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
