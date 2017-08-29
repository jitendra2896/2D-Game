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

const int BULLET_FIRE_RATE = 4;
const float PER_BULLET_TIME = 1.0f / BULLET_FIRE_RATE;
float bullet_shoot_time = glfwGetTime() + PER_BULLET_TIME;

float lastShootTime = 0;

Display* window = Display::createDisplay(600, 600, "Hello World");
DynamicModel2D* player = new DynamicModel2D(Vector2f(25, 25),Vector2f(0,1),1,50, 75);
std::vector<Bullet> bullets;
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

bool canShoot() {
	if ((glfwGetTime()-lastShootTime)<=PER_BULLET_TIME) {
		return false;
	}

	return true;
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

	if (canShoot() && keyDown && action_key == GLFW_KEY_SPACE) {
		Bullet ob(player->getPosition(), player->getFrontVector(), 0.4f, 50);
		ob.bindVertexAttributes(shader.getAttributeLocation("position"));
		bullets.push_back(ob);
		lastShootTime = glfwGetTime();
	}

	player->rotate(rot,TIME_PER_FRAME);
	player->move(playerX, playerY, TIME_PER_FRAME);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	player->setUniformMatrixLocation(shader.getUniformLocation("projectionMatrix"), shader.getUniformLocation("transformationMatrix"));
	shader.useProgram();
	player->render();
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].setUniformMatrixLocation(shader.getUniformLocation("projectionMatrix"), shader.getUniformLocation("transformationMatrix"));
		bullets[i].shoot(TIME_PER_FRAME);
	}
	shader.stopProgram();
}

int main(int argc, char** argv) {
	init();
	while (!glfwWindowShouldClose(window->getGLFWWindow())) {
		render();
		update();
		wait();
		window->swapBuffers();
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}