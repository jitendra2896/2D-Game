#include"Display.h"
#include"Model.h"
#include"Shader.h"
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<iostream>
#include"Input.h"
#include<GLFW\glfw3.h>
#include<vector>
#include"CollisionDetection.h"

const int MAX_BULLETS = 4;
const float TIME_PER_FRAME = 1.0f / 60.0f;
float expected_frame_end = glfwGetTime() + TIME_PER_FRAME;

const int BULLET_FIRE_RATE = 5;
const float PER_BULLET_TIME = 1.0f / BULLET_FIRE_RATE;
float lastShootTime = 0;

Display* window = Display::createDisplay(600, 600, "Hello World");
DynamicModel2D* player = new Player(Vector2f(25, 25),Vector2f(0,1),20,50, 200,Texture("wall.png",512,512));
std::vector<Bullet> bullets;
std::vector<Enemy*> enemies;

StaticShader shader;
StaticShader2 shader2;

void init() {
	registerInput(window);
	glClearColor(0, 0, 0, 0);
	
	enemies.push_back(new FollowEnemy(Vector2f(25, 45), 1, 25, Vector3f(0, 0, 1), player));
	enemies.push_back(new SimpleEnemy(Vector2f(25, 25), 2, 25, Vector3f(0, 1, 1), SimpleEnemy::Axis::yAxis));
	enemies.push_back(new SimpleEnemy(Vector2f(10, 30), 1, 30, Vector3f(1, 1, 1), SimpleEnemy::Axis::xAxis));
	enemies.push_back(new SimpleEnemy(Vector2f(13, 25), 0.95f, 12, Vector3f(1, 0, 1), SimpleEnemy::Axis::yAxis));
	enemies.push_back(new SimpleEnemy(Vector2f(13, 28), 0.5f, 1, Vector3f(1, 1, 1), SimpleEnemy::Axis::xAxis));
	enemies.push_back(new FollowEnemy(Vector2f(0, 0), 1.5f, 30, Vector3f(1, 0.5f, 0.2f), player));

	player->bindVertexAttributes(shader.getAttributeLocation("position"),shader.getAttributeLocation("texCoords"));
	player->setUniformMatrixLocation(shader.getUniformLocation("projectionMatrix"), shader.getUniformLocation("transformationMatrix"));

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->bindVertexAttributes(shader2.getAttributeLocation("position"));
		enemies[i]->bindVertexAttributes(shader2.getAttributeLocation("position"));
		enemies[i]->setUniformMatrixLocation(shader2.getUniformLocation("projectionMatrix"), shader2.getUniformLocation("transformationMatrix"), shader2.getUniformLocation("color"));
		enemies[i]->setUniformMatrixLocation(shader2.getUniformLocation("projectionMatrix"), shader2.getUniformLocation("transformationMatrix"), shader2.getUniformLocation("color"));
	}
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

	receiveInput(window->getGLFWWindow());

	float playerX = 0;
	float playerY = 0;

	int rot = 0;

	if (KEY_W) {
		playerX = 0;
		playerY = 1;
	}
	else if (KEY_S) {
		playerX = 0;
		playerY = -1;
	}
	else if (KEY_A) {
		playerX = -1;
		playerY = 0;
	}
	else if (KEY_D) {
		playerX = 1;
		playerY = 0;
	}

	if (KEY_RIGHT) {
		rot = -1;
	}
	else if (KEY_LEFT) {
		rot = 1;
	}

	if (canShoot() && KEY_SPACE) {
		Bullet ob(player->getPosition(), player->getFrontVector(), 0.4f, 75,Vector3f(1,1,0));
		ob.bindVertexAttributes(shader2.getAttributeLocation("position"));
		bullets.push_back(ob);
		lastShootTime = glfwGetTime();
	}

	//Delete bullets if they are not visible
	for (int i = 0; i < bullets.size(); i++) {
		if (!bullets[i].isVisible()) {
			bullets[i].clear();
			bullets.erase(bullets.begin() + i);
		}
	}
	
	for (int i = 0; i < bullets.size(); i++) {
		for (int j = 0; j < enemies.size(); j++) {
			if (checkCollision(enemies[j], &bullets[i])) {
				bullets[i].clear();
				bullets.erase(bullets.begin() + i);
				enemies[j]->clear();
				enemies.erase(enemies.begin() + j);
				break;
			}
		}
	}

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->moveEnemy(TIME_PER_FRAME);
	}
	player->rotate(rot,TIME_PER_FRAME);
	player->move(playerX, playerY, TIME_PER_FRAME);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	shader.useProgram();
	player->render();
	shader2.useProgram();
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->render();
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].setUniformMatrixLocation(shader2.getUniformLocation("projectionMatrix"), shader2.getUniformLocation("transformationMatrix"), shader2.getUniformLocation("color"));
		bullets[i].shoot(TIME_PER_FRAME);
	}
	shader2.stopProgram();
}

int main(int argc, char** argv) {
	glEnable(GL_MULTISAMPLE);
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