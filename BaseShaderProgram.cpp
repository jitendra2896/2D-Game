#include"Shader.h"
#include<GL\glew.h>
#include<GL\freeglut.h>
#include<fstream>
#include<iostream>
using namespace std;


ShaderProgram::ShaderProgram(string vertexShaderFile, string fragmentShaderFile) {
	this->vertexShaderFile = vertexShaderFile;
	this->fragmentShaderFile = fragmentShaderFile;
	vertexShaderId = loadShader(parseShader(vertexShaderFile), GL_VERTEX_SHADER);
	fragmentShaderId = loadShader(parseShader(fragmentShaderFile), GL_FRAGMENT_SHADER);
	programId = linkProgram();
}

int ShaderProgram::loadShader(string source, int shaderType) {
	int shaderId = glCreateShader(shaderType);
	if (shaderId == 0) {
		cout << "Error creating Shader\n";
		return 0;
	}
	const GLchar* str = source.c_str();
	glShaderSource(shaderId, 1, &str, NULL);
	glCompileShader(shaderId);

	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* compilationMessage = new char[length];
		glGetShaderInfoLog(shaderId, length, &length, compilationMessage);
		cout << "Cannot Compile Shader: "<<((shaderType == GL_VERTEX_SHADER)?vertexShaderFile:fragmentShaderFile)<<"  " << compilationMessage;
		delete[] compilationMessage;
		glDeleteShader(shaderId);
		return 0;
	}
	return shaderId;
}

int ShaderProgram::linkProgram() {
	int programId = glCreateProgram();
	if (programId == 0) {
		deleteShaders();
		cout << "Cannot Create Program Object";
		return 0;
	}

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);

	GLint linkStatus;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
	//glValidateProgram(programId);
	if (!linkStatus) {
		glDetachShader(programId, vertexShaderId);
		glDetachShader(programId, fragmentShaderId);
		deleteShaders();
		glDeleteProgram(programId);
		cout << "Cannot Link Program";
		//int length;
		//glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
		//cout << "Length 2: " << length;
		//char* linkMessage = new char[length];
		//glGetProgramInfoLog(programId, length, &length, linkMessage);
		//cout << "Cannot Link Program: "<<linkMessage;
		//delete[] linkMessage;
		return 0;
	}
	return programId;
}

string ShaderProgram::parseShader(string shaderFile) {
	string shaderCode;
	ifstream ifs;
	ifs.open(shaderFile, ios::in);
	string line;
	while (!ifs.eof()) {
		getline(ifs, line);
		shaderCode += line;
		shaderCode += "\n";
	}
	ifs.close();
	return shaderCode;
}

void ShaderProgram::deleteShaders() {
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

int ShaderProgram::getAttributeLocation(string attribName) {
	int location = glGetAttribLocation(programId, attribName.c_str());
	return location;
}

int ShaderProgram::getUniformLocation(string uniformName) {
	int location;
	location = glGetUniformLocation(programId, uniformName.c_str());
	return location;
}

void ShaderProgram::useProgram() {
	glUseProgram(programId);
}

void ShaderProgram::stopProgram() {
	glUseProgram(0);
}
