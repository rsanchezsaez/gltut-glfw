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

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

const float vertexPositions[] = {
	0.25f, 0.25f, 0.0f, 1.0f,
	0.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
};

Scene::Scene()
{
}

void Scene::init()
{
    _shaderProgram = createShaderProgramWithFilenames("standard.vert", "standard.frag");
    glUseProgram(_shaderProgram);
    printOpenGLError();
    
    // Initialize Vertex Buffer
    glGenBuffers(1, &_positionBufferObject);
    
	glBindBuffer(GL_ARRAY_BUFFER, _positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    printOpenGLError();
    
    // Vertex array object
    glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
    printOpenGLError();
}

Scene::~Scene()
{
    glDeleteProgram(_shaderProgram);
    glDeleteBuffers(1, &_positionBufferObject);
    printOpenGLError();
}

void Scene::computePositionOffsets(float &fXOffset, float &fYOffset)
{
	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;
    
	float fElapsedTime = glfwGetTime();
    
	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);
    
	fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.5f;
	fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.5f;
}

void Scene::adjustVertexData(float fXOffset, float fYOffset)
{
	std::vector<float> fNewData(ARRAY_COUNT(vertexPositions));
	memcpy(&fNewData[0], vertexPositions, sizeof(vertexPositions));
    
	for(int iVertex = 0; iVertex < ARRAY_COUNT(vertexPositions); iVertex += 4)
	{
		fNewData[iVertex] += fXOffset;
		fNewData[iVertex + 1] += fYOffset;
	}
    
	glBindBuffer(GL_ARRAY_BUFFER, _positionBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), &fNewData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Scene::draw()
{
    float fXOffset = 0.0f, fYOffset = 0.0f;
    this->computePositionOffsets(fXOffset, fYOffset);
    this->adjustVertexData(fXOffset, fYOffset);

    glClearColor(0.2f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	glBindBuffer(GL_ARRAY_BUFFER, _positionBufferObject);    
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
    printOpenGLError();
    
	glDisableVertexAttribArray(0);
    printOpenGLError();
}