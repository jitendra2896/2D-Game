#include"CollisionDetection.h"
#include"Math.h"
#include<iostream>
bool checkCollision(Model2D* enemy, Model2D* bullet) {
	
	glm::mat4 enemyTransformationMatrix = enemy->getTransformationMatrixWithoutRotation();
	glm::mat4 bulletTransformationMatrix = bullet->getTransformationMatrixWithoutRotation();

	glm::vec4 eTopLeft = enemyTransformationMatrix*glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 eBottomRight = enemyTransformationMatrix*glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	float enemyWidth = eBottomRight.x - eTopLeft.x;
	float enemyHeight = eTopLeft.y - eBottomRight.y;

	glm::vec4 bTopLeft = bulletTransformationMatrix*glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 bBottomRight = bulletTransformationMatrix*glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	float bulletWidth = bBottomRight.x - bTopLeft.x;
	float bulletHeight = bTopLeft.y - bBottomRight.y;

	if (eTopLeft.x<bTopLeft.x + bulletWidth&&
		eTopLeft.x + enemyWidth>bTopLeft.x&&
		eTopLeft.y > bTopLeft.y - bulletHeight&&
		eTopLeft.y - enemyHeight < bTopLeft.y) {
		return true;
	}
	return false;
}
