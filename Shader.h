#pragma once
#include<string>

class ShaderProgram {
private:
	int programId;
	int vertexShaderId;
	int fragmentShaderId;
	std::string parseShader(std::string shaderFile);
	int loadShader(std::string shaderSource, int type);
	int linkProgram();
	void deleteShaders();
	std::string vertexShaderFile;
	std::string fragmentShaderFile;

public:
	ShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile);
	void useProgram();
	void stopProgram();
	int getAttributeLocation(std::string attribName);
	int getUniformLocation(std::string uniformName);
};

class StaticShader : public ShaderProgram {
public:
	StaticShader();
};

class StaticShader2 :public ShaderProgram {
public:
	StaticShader2();
};