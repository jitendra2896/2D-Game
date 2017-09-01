#include"Model.h"
#include<GL\glew.h>
#include<GL\freeglut.h>
#include<iostream>

float clamp(float num, float min, float max) {
	if (num <= min)
		num = min;
	else if (num >= max)
		num = max;
	return num;
}

void multiplyVectorAndMatrix(Vector2f& v, glm::mat4 matrix) {
	glm::vec4 vector(v.x, v.y, 0, 1);
	glm::vec4 temp(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 result = matrix*temp;
	v.x = result.x;
	v.y = result.y;
}

Model2D::Model2D(const Vector2f& pos, const Vector2f& fv,float scale, const Vector3f& color) {
	this->position = pos;
	this->frontVector = fv;
	this->scale = scale;
	this->color = color;
	vertices[0] = -1;
	vertices[1] = 1;
	vertices[2] = -1;
	vertices[3] = -1;
	vertices[4] = 1;
	vertices[5] = 1;
	vertices[6] = 1;
	vertices[7] = -1;

	loadDataToBuffers();
}

Vector2f Model2D::getPosition() {
	return position;
}

Vector2f Model2D::getFrontVector() {
	return frontVector;
}

void Model2D::loadDataToBuffers() {
	GLuint vboId;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->vboId = vboId;
}

void Model2D::bindVertexAttributes(int vPosition) {
	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);
	this->vaoId = vaoId;
}

glm::mat4 Model2D::createProjectionMatrix() {
	glm::mat4 projectionMatrix = glm::ortho(0.0f, 50.0f, 0.0f, 50.0f, -1.0f, 1.0f);
	return projectionMatrix;
}

glm::mat4 Model2D::createTransformationMatrix() {
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, 0));
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scale, scale, scale));
	multiplyVectorAndMatrix(frontVector, rotationMatrix);
	glm::mat4 transformationMatrix = translationMatrix*rotationMatrix*scaleMatrix;
	return transformationMatrix;
}

void Model2D::setUniformMatrixLocation(int pml, int tml,int cl) {
	uniformProjectionMatrixLocation = pml;
	uniformTransformationMatrixLocation = tml;
	uniformColorLocation = cl;
}

bool Model2D::isVisible() {
	if (position.x > 50 || position.y > 50 || position.x < 0 || position.y<0) {
		return false;
	}
	return true;
}

void Model2D::clear() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
}

StaticModel2D::StaticModel2D(const Vector2f& pos,float scale, const Vector3f& color) :Model2D(pos, Vector2f(0, 0),scale,color) {

}

void StaticModel2D::render() {
	glBindVertexArray(vaoId);
	projectionMatrix = createProjectionMatrix();
	transformationMatrix = createTransformationMatrix();
	glUniformMatrix4fv(uniformProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(uniformTransformationMatrixLocation, 1, GL_FALSE, &transformationMatrix[0][0]);
	glUniform4f(uniformColorLocation, color.x, color.y, color.z, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

DynamicModel2D::DynamicModel2D(const Vector2f& pos,const Vector2f& fv,float scale,float speed,float rotationSpeed, const Vector3f& color):Model2D(pos,fv,scale,color) {
	this->speed = speed;
	this->rotationSpeed = rotationSpeed;
	angle = 0.0f;
}

void DynamicModel2D::move(float dx, float dy,float deltaTime) {
	position.x += dx*deltaTime*speed;
	position.y += dy*deltaTime*speed;
	position.x = clamp(position.x, 1, 49);
	position.y = clamp(position.y, 1, 49);
}

void DynamicModel2D::rotate(int angle,float deltaTime) {
	this->angle += angle*rotationSpeed*0.0174533*deltaTime;
}

void DynamicModel2D::render() {
	glBindVertexArray(vaoId);
	projectionMatrix = createProjectionMatrix();
	transformationMatrix = createTransformationMatrix();
	glUniformMatrix4fv(uniformProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(uniformTransformationMatrixLocation, 1, GL_FALSE, &transformationMatrix[0][0]);
	glUniform4f(uniformColorLocation, color.x, color.y, color.z, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Player::Player(const Vector2f& pos, const Vector2f& fv, float scale, float speed, float rotationSpeed, const Vector3f& color) :DynamicModel2D(pos, fv, scale, speed, rotationSpeed, color) {

}

void Player::move(float dx, float dy, float deltaTime) {
	position.x += dx*deltaTime*speed;
	position.y += dy*deltaTime*speed;
	position.x = clamp(position.x, 1, 49);
	position.y = clamp(position.y, 1, 49);
}

Bullet::Bullet(const Vector2f& position, const Vector2f& direction,float scale, float speed, const Vector3f& color) :DynamicModel2D(position, Vector2f(0, 0),scale, speed, 0,color) {
	this->direction = direction;
}

void Bullet::move(float dx,float dy,float deltaTime) {
	position.x += direction.x*deltaTime*speed;
	position.y += direction.y*deltaTime*speed;
}

void Bullet::shoot(float deltaTime) {
	move(0,0,deltaTime);
	render();
}

Enemy::Enemy(const Vector2f& position, const Vector2f& fv, float scale, float speed, const Vector3f& color) :DynamicModel2D(position, fv, scale, speed, 0,color) {
}

SimpleEnemy::SimpleEnemy(const Vector2f& position, float scale, float speed, const Vector3f& color,Axis axis) : Enemy(position, Vector2f(0,0), scale, speed, color) {
	this->axis = axis; 
	if (axis == Axis::xAxis) {
		xDirection = 1;
		yDirection = 0;
	}
	else {
		xDirection = 0;
		yDirection = 1;
	}
}

void SimpleEnemy::move(float dx,float dy,float deltaTime) {
	if (position.x >= 49 || position.x <= 1) {
		xDirection = -xDirection;
	}

	if (position.y >= 49 || position.y <= 1) {
		yDirection = -yDirection;
	}
	this->position.x += xDirection*speed*deltaTime;
	this->position.y += yDirection*speed*deltaTime;

}

void SimpleEnemy::moveEnemy(float deltaTime) {
	move(0, 0, deltaTime);
}
