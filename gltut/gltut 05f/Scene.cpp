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
#include <map>

#include "debug.h"
#include "glhelpers.h"
#include "GLFW/glfw3.h"

#include <math.h>

const int numberOfVertices = 8;

#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f

const float Z_OFFSET = 0.5f;

const float vertexData[] = {
	//Front face positions
	-400.0f,		 400.0f,			0.0f,
	400.0f,		 400.0f,			0.0f,
	400.0f,		-400.0f,			0.0f,
	-400.0f,		-400.0f,			0.0f,
    
	//Rear face positions
	-200.0f,		 600.0f,			-Z_OFFSET,
	600.0f,		 600.0f,			0.0f - Z_OFFSET,
	600.0f,		-200.0f,			0.0f - Z_OFFSET,
	-200.0f,		-200.0f,			-Z_OFFSET,
    
	//Front face colors.
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
    
	//Rear face colors.
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
};

const GLshort indexData[] =
{
	0, 1, 3,
	1, 2, 3,
    
	4, 5, 7,
	5, 6, 7,
};

GLuint offsetUniform;
GLuint perspectiveMatrixUniform;

float perspectiveMatrix[16];
const float frustumScale = 1.0f;

GLuint indexBufferObject;

Scene::Scene()
{
}

void Scene::init()
{
    _shaderProgram = createShaderProgramWithFilenames("Standard5.vert", "Standard5.frag");
    glUseProgram(_shaderProgram);
    printOpenGLError();

    // Uniforms
	offsetUniform = glGetUniformLocation(_shaderProgram, "offset");
    
	perspectiveMatrixUniform = glGetUniformLocation(_shaderProgram, "perspectiveMatrix");
    
	float zNear = 1.0f; float zFar = 100000.0f;
    
	memset(perspectiveMatrix, 0, sizeof(float) * 16);
    
	perspectiveMatrix[0] = frustumScale;
	perspectiveMatrix[5] = frustumScale;
	perspectiveMatrix[10] = (zFar + zNear) / (zNear - zFar);
	perspectiveMatrix[14] = (2 * zFar * zNear) / (zNear - zFar);
	perspectiveMatrix[11] = -1.0f;
    
	glUniformMatrix4fv(perspectiveMatrixUniform, 1, GL_FALSE, perspectiveMatrix);
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
    perspectiveMatrix[0] = frustumScale / (width / (float)height);
    perspectiveMatrix[5] = frustumScale;
    
    glUniformMatrix4fv(perspectiveMatrixUniform, 1, GL_FALSE, perspectiveMatrix);

    this->draw();
}

float fStart = 2534.0f;
float fDelta = 0.0f;

float CalcZOFfset()
{
	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;
    
	float fElapsedTime = glfwGetTime();
    
	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);
    
    float fRet = cosf(fCurrTimeThroughLoop * fScale) * 500.0f - fStart;
	//float fRet = fDelta - fStart;
    
	return fRet;
}

volatile bool bReadBuffer = false;

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void Scene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glUseProgram(_shaderProgram);
	glBindVertexArray(_vertexArrayObject);
    
	float fZOffset = CalcZOFfset();
	glUniform3f(offsetUniform, 0.0f, 0.0f, fZOffset);
	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
    
	glBindVertexArray(0);
	glUseProgram(0);
        
	//Read the backbuffer.
	if(bReadBuffer)
	{
		bReadBuffer = false;
        
		GLuint *pBuffer = new GLuint[500*500];
		glReadPixels(0, 0, 500, 500, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, pBuffer);
        printOpenGLError();
        
		std::string strOutput;
		strOutput.reserve(500*500*2 + (500));
        
		std::map<GLuint, char> charMap;
        
		GLuint *pBufferLoc = pBuffer;
		for(int y = 0; y < 500; y++)
		{
			for(int x = 0; x < 500; x++)
			{
				GLuint iValue = *pBufferLoc >> 8;
				iValue = iValue & 0x00FFFFFF;
                
				if(charMap.find(iValue) == charMap.end())
				{
					if(charMap.size())
						charMap[iValue] = static_cast<char>(65 + charMap.size() - 1);
					else
						charMap[iValue] = '.';
				}
                
				strOutput.push_back(charMap[iValue]);
				strOutput.push_back(' ');
                
				++pBufferLoc;
			}
            
			strOutput.push_back('\n');
		}
        
		delete[] pBuffer;
        
		{
			static int iFile = 0;
			std::ostringstream temp;
			temp << "test" << iFile << ".txt";
			std::string strFilename = temp.str();
            
			std::ofstream shaderFile(strFilename.c_str());
			shaderFile << "Offset: " << fZOffset << std::endl;
			for(std::map<GLuint, char>::const_iterator startIt = charMap.begin();
				startIt != charMap.end();
				++startIt)
			{
				shaderFile << startIt->first << "->\'" << startIt->second << "\'" << std::endl;
			}
            
			shaderFile << strOutput;
            
			printf("finished\n");
            
			iFile++;
		}
        
        
        //		printf(strOutput.c_str());
	}
}

void Scene::keyStateChanged(int key, int action)
{
}
