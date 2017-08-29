#pragma once
#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include<iostream>

class Display {
private:
	static GLFWwindow* window;
	static int w, h;
	Display() {

	}
	static Display* display;

public:
	static Display* createDisplay(int width, int height, char* title) {
		if (!display) {
			if (!glfwInit()) {
				std::cout << "Cannot initialize glfw" << std::endl;
				return NULL;
			}
			w = width;
			h = height;
			glfwWindowHint(GLFW_SAMPLES, 4);
			window = glfwCreateWindow(width, height, title, NULL, NULL);
			glfwMakeContextCurrent(window);
			glewInit();
			display = new Display();
		}
		return display;
	}

	static void swapBuffers() {
		glfwSwapBuffers(window);
	}

	static GLFWwindow* getGLFWWindow() {
		return window;
	}
	static int getWidth() {
		return w;
	}
	static int getHeight() {
		return h;
	}
};
GLFWwindow* Display::window = 0;
Display* Display::display = 0;
int Display::w = 0;
int Display::h = 0;