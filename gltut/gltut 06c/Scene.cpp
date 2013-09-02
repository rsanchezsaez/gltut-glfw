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

#include <math.h>
#include <string>
#include <vector>

#include "debug.h"
#include "glhelpers.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

const int numberOfVertices = 8;

#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertexData[] =
{
	+1.0f, +1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
    
	-1.0f, -1.0f, -1.0f,
	+1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,
    
	GREEN_COLOR,
	BLUE_COLOR,
	RED_COLOR,
	BROWN_COLOR,
    
	GREEN_COLOR,
	BLUE_COLOR,
	RED_COLOR,
	BROWN_COLOR,
};

const GLshort indexData[] =
{
	0, 1, 2,
	1, 0, 3,
	2, 3, 0,
	3, 2, 1,
    
	5, 4, 6,
	4, 5, 7,
	7, 6, 4,
	6, 7, 5,
};

glm::mat4 cameraToClipMatrix(0.0f);

GLuint modelToCameraMatrixUniform;
GLuint cameraToClipMatrixUniform;

float CalcFrustumScale(float fFovDeg)
{
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	float fFovRad = fFovDeg * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
}

const float frustumScale = CalcFrustumScale(45.0f);

GLuint indexBufferObject;

float CalcLerpFactor(float fElapsedTime, float fLoopDuration)
{
	float fValue = fmodf(fElapsedTime, fLoopDuration) / fLoopDuration;
	if(fValue > 0.5f)
		fValue = 1.0f - fValue;
    
	return fValue * 2.0f;
}

glm::mat3 NullRotation(float fElapsedTime)
{
	return glm::mat3(1.0f);
}

float ComputeAngleRad(float fElapsedTime, float fLoopDuration)
{
	const float fScale = 3.14159f * 2.0f / fLoopDuration;
	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);
	return fCurrTimeThroughLoop * fScale;
}

glm::mat3 RotateX(float fElapsedTime)
{
	float fAngRad = ComputeAngleRad(fElapsedTime, 3.0);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);
    
	glm::mat3 theMat(1.0f);
	theMat[1].y = fCos; theMat[2].y = -fSin;
	theMat[1].z = fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateY(float fElapsedTime)
{
	float fAngRad = ComputeAngleRad(fElapsedTime, 2.0);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);
    
	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[2].x = fSin;
	theMat[0].z = -fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateZ(float fElapsedTime)
{
	float fAngRad = ComputeAngleRad(fElapsedTime, 2.0);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);
    
	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[1].x = -fSin;
	theMat[0].y = fSin; theMat[1].y = fCos;
	return theMat;
}

glm::mat3 RotateAxis(float fElapsedTime)
{
	float fAngRad = ComputeAngleRad(fElapsedTime, 2.0);
	float fCos = cosf(fAngRad);
	float fInvCos = 1.0f - fCos;
	float fSin = sinf(fAngRad);
	// float fInvSin = 1.0f - fSin;
    
	glm::vec3 axis(1.0f, 1.0f, 1.0f);
	axis = glm::normalize(axis);
    
	glm::mat3 theMat(1.0f);
	theMat[0].x = (axis.x * axis.x) + ((1 - axis.x * axis.x) * fCos);
	theMat[1].x = axis.x * axis.y * (fInvCos) - (axis.z * fSin);
	theMat[2].x = axis.x * axis.z * (fInvCos) + (axis.y * fSin);
    
	theMat[0].y = axis.x * axis.y * (fInvCos) + (axis.z * fSin);
	theMat[1].y = (axis.y * axis.y) + ((1 - axis.y * axis.y) * fCos);
	theMat[2].y = axis.y * axis.z * (fInvCos) - (axis.x * fSin);
    
	theMat[0].z = axis.x * axis.z * (fInvCos) - (axis.y * fSin);
	theMat[1].z = axis.y * axis.z * (fInvCos) + (axis.x * fSin);
	theMat[2].z = (axis.z * axis.z) + ((1 - axis.z * axis.z) * fCos);
	return theMat;
}

glm::vec3 DynamicNonUniformScale(float fElapsedTime)
{
	const float fXLoopDuration = 3.0f;
	const float fZLoopDuration = 5.0f;
    
	return glm::vec3(glm::mix(1.0f, 0.5f, CalcLerpFactor(fElapsedTime, fXLoopDuration)),
                     1.0f,
                     glm::mix(1.0f, 10.0f, CalcLerpFactor(fElapsedTime, fZLoopDuration)));
}

struct Instance
{
	typedef glm::mat3(*RotationFunc)(float);
    
	RotationFunc CalcRotation;
	glm::vec3 offset;
    
	glm::mat4 ConstructMatrix(float fElapsedTime)
	{
		const glm::mat3 &rotMatrix = CalcRotation(fElapsedTime);
		glm::mat4 theMat(rotMatrix);
		theMat[3] = glm::vec4(offset, 1.0f);
        
		return theMat;
	}
};

Instance g_instanceList[] =
{
	{NullRotation,				glm::vec3(0.0f, 0.0f, -25.0f)},
	{RotateX,					glm::vec3(-5.0f, -5.0f, -25.0f)},
	{RotateY,					glm::vec3(-5.0f, 5.0f, -25.0f)},
	{RotateZ,					glm::vec3(5.0f, 5.0f, -25.0f)},
	{RotateAxis,				glm::vec3(5.0f, -5.0f, -25.0f)},
};

Scene::Scene()
{
}

void Scene::init()
{
    _shaderProgram = createShaderProgramWithFilenames("PosColorLocalTransform.vert", "ColorPassthrough.frag");
    glUseProgram(_shaderProgram);
    printOpenGLError();
    
    // Uniforms
	modelToCameraMatrixUniform = glGetUniformLocation(_shaderProgram, "modelToCameraMatrix");
	cameraToClipMatrixUniform = glGetUniformLocation(_shaderProgram, "cameraToClipMatrix");
    
    // Matrix
    float fzNear = 1.0f; float fzFar = 45.0f;
    
	cameraToClipMatrix[0].x = frustumScale;
	cameraToClipMatrix[1].y = frustumScale;
	cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
	cameraToClipMatrix[2].w = -1.0f;
	cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);
    
	glUniformMatrix4fv(cameraToClipMatrixUniform, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
    
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
    glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);
    
    size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    
    glBindVertexArray(0);
    printOpenGLError();
    
    // Enable cull facing
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    printOpenGLError();
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}

Scene::~Scene()
{
    glDeleteProgram(_shaderProgram);
    glDeleteBuffers(1, &_vertexBufferObject);
    printOpenGLError();
}

void Scene::reshape(int width, int height)
{
    cameraToClipMatrix[0].x = frustumScale / (width / (float)height);
    cameraToClipMatrix[1].y = frustumScale;
    
    glUniformMatrix4fv(cameraToClipMatrixUniform, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
    
    this->draw();
}

void Scene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glBindVertexArray(_vertexArrayObject);
    
	float fElapsedTime = glfwGetTime();
	for(int iLoop = 0; iLoop < ARRAY_COUNT(g_instanceList); iLoop++)
	{
		Instance &currInst = g_instanceList[iLoop];
		const glm::mat4 &transformMatrix = currInst.ConstructMatrix(fElapsedTime);
        
		glUniformMatrix4fv(modelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(transformMatrix));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
	}
    
	glBindVertexArray(0);
}