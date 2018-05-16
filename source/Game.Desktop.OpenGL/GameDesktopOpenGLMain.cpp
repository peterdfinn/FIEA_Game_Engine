#include "pch.h"
#include "GLFW\glfw3.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, INT nCmdShow)
{
	// Preventing warnings
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// Creating color of window
	float puceR = 204.0f / 255.0f;
	float puceG = 136.0f / 255.0f;
	float puceB = 153.0f / 255.0f;
	float puceA = 1.0f;

	// Initializing OpenGL
	if (glfwInit() == GLFW_FALSE)
	{
		return -1;
	}

	// Creating window
	GLFWwindow* openGLWindow = glfwCreateWindow(500, 500, "Puce is such a love\
		ly color, isn't it?",
		nullptr, nullptr);
	if (openGLWindow == nullptr)
	{
		return -1;
	}

	glfwMakeContextCurrent(openGLWindow);

	glViewport(0, 0, 500, 500);

	while (!glfwWindowShouldClose(openGLWindow))
	{
		glClearColor(puceR, puceG, puceB, puceA);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(openGLWindow);
		glfwPollEvents();
	}

	glfwDestroyWindow(openGLWindow);
	glfwTerminate();

    return 0;
}

