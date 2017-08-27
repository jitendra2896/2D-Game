#include"Display.h"
#include"Model.h"
#include"Shader.h"
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<iostream>
#include"Input.h"
#include<GLFW\glfw3.h>
#include<vector>

const int MAX_BULLETS = 4;
int nextBulletPosition = 0;
const float TIME_PER_FRAME = 1.0f / 60.0f;
float expected_frame_end = glfwGetTime() + TIME_PER_FRAME;

Display* window = Display::createDisplay(600, 600, "Hello World");
DynamicModel2D* player = new DynamicModel2D(Vector2f(25, 25),Vector2f(1,0),50, 2);
std::vector<DynamicModel2D*> bullets;
StaticShader shader;

void init() {
	registerInput(window);
	glClearColor(0, 0, 0, 0);
	player->bindVertexAttributes(shader.getAttributeLocation("position"));
}

void wait() {
	while (glfwGetTime() < expected_frame_end) {

	}
	expected_frame_end += TIME_PER_FRAME;
}

void update() {
	float playerX = 0;
	float playerY = 0;

	int rot = 0;

	if (keyDown && action_key == GLFW_KEY_W) {
		playerX = 0;
		playerY = 1;
	}
	else if (keyDown && action_key == GLFW_KEY_S) {
		playerX = 0;
		playerY = -1;
	}
	else if (keyDown && action_key == GLFW_KEY_A) {
		playerX = -1;
		playerY = 0;
	}
	else if (keyDown && action_key == GLFW_KEY_D) {
		playerX = 1;
		playerY = 0;
	}

	if (keyDown && action_key == GLFW_KEY_RIGHT) {
		rot = -1;
	}
	else if (keyDown && action_key == GLFW_KEY_LEFT) {
		rot = 1;
	}

	if (keyDown && action_key == GLFW_KEY_SPACE) {
		std::cout << "SPACE" << std::endl;
		DynamicModel2D* bullet = new DynamicModel2D(player->getPosition(), Vector2f(0, 0), 70, 0);
		bullet->bindVertexAttributes(shader.getAttributeLocation("position"));
		bullets.push_back(bullet);
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move(0, 1, TIME_PER_FRAME);
	}

	player->rotate(rot);
	player->move(playerX, playerY, TIME_PER_FRAME);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	player->setUniformMatrixLocation(shader.getUniformLocation("projectionMatrix"), shader.getUniformLocation("transformationMatrix"));
	shader.useProgram();
	player->render();
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->setUniformMatrixLocation(shader.getUniformLocation("projectionMatrix"), shader.getUniformLocation("transformationMatrix"));
		bullets[i]->render();
	}
	wait();
	shader.stopProgram();
}

int main(int argc, char** argv) {
	init();
	while (!glfwWindowShouldClose(window->getGLFWWindow())) {
		render();
		update();
		window->swapBuffers();
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}