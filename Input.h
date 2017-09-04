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

bool KEY_W,KEY_S,KEY_A,KEY_D,KEY_RIGHT,KEY_LEFT,KEY_SPACE;
int state;

void receiveInput(GLFWwindow* window) {
	state = glfwGetKey(window, GLFW_KEY_W);
	if (state == GLFW_PRESS) {
		KEY_W = true;
	}
	else if (state == GLFW_RELEASE) {
		KEY_W = false;
	}
	state = glfwGetKey(window, GLFW_KEY_S);
	if (state == GLFW_PRESS) {
		KEY_S = true;
	}
	else if (state == GLFW_RELEASE) {
		KEY_S = false;
	}

	state = glfwGetKey(window, GLFW_KEY_A);
	if (state == GLFW_PRESS) {
		KEY_A = true;
	}
	else if (state == GLFW_RELEASE) {
		KEY_A = false;
	}

	state = glfwGetKey(window, GLFW_KEY_D);
	if (state == GLFW_PRESS) {
		KEY_D = true;
	}
	else if (state == GLFW_RELEASE) {
		KEY_D = false;
	}

	state = glfwGetKey(window, GLFW_KEY_SPACE);
	if (state == GLFW_PRESS) {
		KEY_SPACE = true;
	}
	else if (state == GLFW_RELEASE) {
		KEY_SPACE = false;
	}

	state = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (state == GLFW_PRESS) {
		KEY_RIGHT = true;
	}
	else if (state == GLFW_RELEASE) {
		KEY_RIGHT = false;
	}

	state = glfwGetKey(window, GLFW_KEY_LEFT);
	if (state == GLFW_PRESS) {
		KEY_LEFT = true;
	}
	else if (state == GLFW_RELEASE) {
		KEY_LEFT = false;
	}
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
}