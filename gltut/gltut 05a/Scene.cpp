//
//  Scene.cpp
//  gltut-glfw
//
// Copyright (C) 2010-2012 by Jason L. McKesson
// Xcode and glfw adaptation: Ricardo Sánchez-Sáez.
//
// This file is licensed under the MIT License.
//

#include "Scene.h"

#include <string>
#include <vector>

#include "debug.h"
#include "glhelpers.h"
#include "GLFW/glfw3.h"

#include <math.h>

const int numberOfVertices = 36;

#define RIGHT_EXTENT 0.8f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT 0.20f
#define MIDDLE_EXTENT 0.0f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -1.25f
#define REAR_EXTENT -1.75f

#define GREEN_COLOR 0.75f, 0.75f, 1.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.5f, 0.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertexData[] = {
	//Object 1 positions
	LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
    
	LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
	LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
    
	LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
    
	RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
    
	LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
	LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
    
	//Object 2 positions
	TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
	MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
	TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,
    
	BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
	MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
	BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,
    
	TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
	BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
    
	TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
	BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,
    
	BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
	TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
	TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,
	BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,
    
	//Object 1 colors
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
    
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,
    
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
    
	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,
    
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
    
	//Object 2 colors
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
    
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
    
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,
    
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
    
	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,
};

const GLshort indexData[] =
{
	0, 2, 1,
	3, 2, 0,
    
	4, 5, 6,
	6, 7, 4,
    
	8, 9, 10,
	11, 13, 12,
    
	14, 16, 15,
	17, 16, 14,
};

GLuint offsetUniform;
GLuint perspectiveMatrixUniform;

float perspectiveMatrix[16];
const float frustumScale = 1.0f;

GLuint indexBufferObject;

GLuint vertexArrayObject1;
GLuint vertexArrayObject2;


Scene::Scene()
{
}

void Scene::init()
{
    _shaderProgram = createShaderProgramWithFilenames("Standard5.vert", "Standard5.frag");
    glUseProgram(_shaderProgram);
    printOpenGLError();

    // Vertex buffer objects
    glGenBuffers(1, &_vertexBufferObject);
    
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glGenBuffers(1, &indexBufferObject);
    
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    printOpenGLError();

    
    // Vertex array objects
    glGenVertexArrays(1, &vertexArrayObject1);
    glBindVertexArray(vertexArrayObject1);
    
    size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    
    glBindVertexArray(0);
    
    
    glGenVertexArrays(1, &vertexArrayObject2);
    glBindVertexArray(vertexArrayObject2);
    
    size_t posDataOffset = sizeof(float) * 3 * (numberOfVertices/2);
    colorDataOffset += sizeof(float) * 4 * (numberOfVertices/2);
    
    //Use the same buffer object previously bound to GL_ARRAY_BUFFER.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)posDataOffset);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    
    glBindVertexArray(0);
    printOpenGLError();

    // Enable cull facing
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    printOpenGLError();

    // Uniforms
	offsetUniform = glGetUniformLocation(_shaderProgram, "offset");
    
	perspectiveMatrixUniform = glGetUniformLocation(_shaderProgram, "perspectiveMatrix");
    
	float zNear = 0.5f; float zFar = 3.0f;
    
	memset(perspectiveMatrix, 0, sizeof(float) * 16);
    
	perspectiveMatrix[0] = frustumScale;
	perspectiveMatrix[5] = frustumScale;
	perspectiveMatrix[10] = (zFar + zNear) / (zNear - zFar);
	perspectiveMatrix[14] = (2 * zFar * zNear) / (zNear - zFar);
	perspectiveMatrix[11] = -1.0f;
    
	glUniformMatrix4fv(perspectiveMatrixUniform, 1, GL_FALSE, perspectiveMatrix);
    printOpenGLError();

}

Scene::~Scene()
{
    glDeleteProgram(_shaderProgram);
    glDeleteBuffers(1, &_vertexBufferObject);
    printOpenGLError();
}

void Scene::reshape(int width, int height)
{
    perspectiveMatrix[0] = frustumScale / (width / (float)height);
    perspectiveMatrix[5] = frustumScale;
    
    glUniformMatrix4fv(perspectiveMatrixUniform, 1, GL_FALSE, perspectiveMatrix);

    this->draw();
}

void Scene::draw()
{
    glClearColor(0.2f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
 
	glBindVertexArray(vertexArrayObject1);
    printOpenGLError();
	glUniform3f(offsetUniform, 0.0f, 0.0f, 0.0f);
    printOpenGLError();
 	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
    printOpenGLError();

	glBindVertexArray(vertexArrayObject2);
    printOpenGLError();
	glUniform3f(offsetUniform, 0.0f, 0.0f, -1.0f);
    printOpenGLError();
	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
    printOpenGLError();

}