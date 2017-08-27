#pragma once
#include"Math.h"
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
class Model2D {
private:
	float vertices[8];
	void loadDataToBuffers();

protected:
	Vector2f position;
	Vector2f frontVector;
	float angle;
	int vboId;
	int vaoId;
	int uniformProjectionMatrixLocation;
	int uniformTransformationMatrixLocation;
	glm::mat4 projectionMatrix;
	glm::mat4 transformationMatrix;
	glm::mat4 createProjectionMatrix();
	glm::mat4 createTransformationMatrix();

public:
	Model2D(const Vector2f& pos, const Vector2f& fv);
	void bindVertexAttributes(int vPosition);
	Vector2f getPosition();
	Vector2f getFrontVector();
	void setUniformMatrixLocation(int pml, int tml);
	virtual void render() = 0;
};

class StaticModel2D : public Model2D {
public:
	StaticModel2D(const Vector2f& pos);
	void render();

};

class DynamicModel2D :public Model2D {
private:
	float speed;
	float rotationSpeed;
public:
	DynamicModel2D(const Vector2f& pos, const Vector2f& fv, float speed,float rotationSpeed);
	void render();
	void move(float dx,float dy,float deltaTime);
	void rotate(int angle);
};

class Bullet : public DynamicModel2D {
public:
	void shoot();
};