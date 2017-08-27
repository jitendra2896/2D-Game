#pragma once
#include<GLFW\glfw3.h>
#include"Display.h"
#include<iostream>

Display* window1;

int action_key;
bool keyDown;
bool keyUp;

float xPos;
float yPos;

void cursorPosition(GLFWwindow* window, double x, double y) {
	xPos = (x / window1->getWidth()) * 50;
	yPos = (1-(y / window1->getHeight())) * 50;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	action_key = key;

	if (action == GLFW_REPEAT || action == GLFW_PRESS) {
		keyDown = true;
		keyUp = false;
	}
	if (action == GLFW_RELEASE) {
		keyDown = false;
		keyUp = true;
	}
}

void registerInput(Display* window) {
	window1 = window;
	glfwSetKeyCallback(window->getGLFWWindow(), keyboardCallback);
	glfwSetCursorPosCallback(window->getGLFWWindow(),cursorPosition);
}