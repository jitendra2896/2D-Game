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
	float scale;
	Vector2f frontVector;
	float angle;
	unsigned int vboId;
	unsigned int vaoId;
	int uniformProjectionMatrixLocation;
	int uniformTransformationMatrixLocation;
	int uniformColorLocation;
	Vector3f color;
	glm::mat4 projectionMatrix;
	glm::mat4 transformationMatrix;
	glm::mat4 createProjectionMatrix();

public:
	Model2D(const Vector2f& pos, const Vector2f& fv,float scale,const Vector3f& color);
	void bindVertexAttributes(int vPosition);
	Vector2f getPosition();
	Vector2f getFrontVector();
	float getScale();
	void clear();
	bool isVisible();
	void setUniformMatrixLocation(int pml, int tml,int cl);
	glm::mat4 createTransformationMatrix();
	virtual void render() = 0;
};

class StaticModel2D : public Model2D {
public:
	StaticModel2D(const Vector2f& pos,float scale,const Vector3f& color);
	void render();

};

class DynamicModel2D : public Model2D {
protected:
	float speed;
	float rotationSpeed;
public:
	DynamicModel2D(const Vector2f& pos, const Vector2f& fv,float scale, float speed,float rotationSpeed,const Vector3f& color);
	void render();
	virtual void move(float dx,float dy,float deltaTime) = 0;
	void rotate(int angle,float deltaTime);
};

class Player : public DynamicModel2D {
public:
	Player(const Vector2f& pos, const Vector2f& fv, float scale, float speed, float rotationSpeed, const Vector3f& color);
	void move(float dx, float dy, float deltaTime);
};

class Bullet : public DynamicModel2D {

	Vector2f direction;
	void move(float dx,float dy,float deltaTime);
public:
	Bullet(const Vector2f& pos, const Vector2f& direction,float scale, float speed,const Vector3f& color);
	void shoot(float deltaTime);
};

class Enemy : public DynamicModel2D {
public:
	Enemy(const Vector2f& pos, const Vector2f& fv, float scale, float speed,const Vector3f& color);
	virtual void moveEnemy(float deltaTime) = 0;
};

class SimpleEnemy :public Enemy {

public:
	enum Axis
	{
		xAxis, yAxis
	};
	SimpleEnemy(const Vector2f& pos, float scale, float speed, const Vector3f& color, Axis axis);
	void moveEnemy(float deltaTime);
private:
	Axis axis;
	int xDirection, yDirection;
	void move(float dx, float dy, float deltaTime);
};