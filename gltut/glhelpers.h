//
//  glhelpers.h
//  gltut-glfw
//
//  Created by Ricardo Sánchez-Sáez on 31/08/2013.
//  Copyright (c) 2013 rsanchezsaez. All rights reserved.
//

#ifndef gltut_glfw_glhelpers_h
#define gltut_glfw_glhelpers_h

#include <fstream>
#include <sstream>

//std::string FindFileOrThrow( const std::string &strBasename )
//{
//    std::string strFilename = strBasename;
//    std::ifstream testFile(strFilename.c_str());
//    if(testFile.is_open())
//        return strFilename;
//    
//    strFilename = strBasename;
//    testFile.open(strFilename.c_str());
//    if(testFile.is_open())
//        return strFilename;
//    
//    throw std::runtime_error("Could not find the file " + strBasename);
//}

GLuint createAndCompilerShaderWithSource(GLenum shaderType, const std::string &shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    const char *strFileData = shaderSource.c_str();
    glShaderSource(shader, 1, &strFileData, NULL);
    
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        
        const char *strShaderType = NULL;
        switch(shaderType)
        {
            case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
            case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
            case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }
        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }
    
	return shader;
}

GLuint createAndCompilerShaderWithFilename(GLenum shaderType, const std::string &shaderFilename)
{
//    std::string strFilename = FindFileOrThrow(strShaderFilename);
    std::ifstream shaderFile(shaderFilename.c_str());
    std::stringstream shaderData;
    shaderData << shaderFile.rdbuf();
    shaderFile.close();
    
    try
    {
        return createAndCompilerShaderWithSource(shaderType, shaderData.str());
    }
    catch(std::exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }
}

GLuint createProgramWithShaderList(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();
    
	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    {
		glAttachShader(program, shaderList[iLoop]);
    }
    
	glLinkProgram(program);
    
	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
    
	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);
    
	return program;
}

GLuint createShaderProgramWithFilenames(const std::string &vertexShaderFilename,
                                        const std::string &fragmentShaderFilename)
{
    // Compiler shaders and create program
    std::vector<GLuint> shaderList;
    GLuint vertexShader = createAndCompilerShaderWithFilename(GL_VERTEX_SHADER, vertexShaderFilename);
	shaderList.push_back( vertexShader );
    GLuint fragmentShader = createAndCompilerShaderWithFilename(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	shaderList.push_back( fragmentShader );
    
    // Default behavior already
    // glBindFragDataLocationEXT( fragmentShader, 0, "outColor" );
    
	GLuint shaderProgram = createProgramWithShaderList(shaderList);
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    printOpenGLError();
    
    return shaderProgram;
    
}


GLuint createShaderProgramWithSource(const std::string &vertexShaderSource,
                                     const std::string &fragmentShaderSource)
{
    // Compiler shaders and create program
    std::vector<GLuint> shaderList;
    GLuint vertexShader = createAndCompilerShaderWithSource(GL_VERTEX_SHADER, vertexShaderSource);
	shaderList.push_back( vertexShader );
    GLuint fragmentShader = createAndCompilerShaderWithSource(GL_FRAGMENT_SHADER, fragmentShaderSource);
	shaderList.push_back( fragmentShader );
    
    // Default behavior already
    // glBindFragDataLocationEXT( fragmentShader, 0, "outColor" );
    
	GLuint shaderProgram = createProgramWithShaderList(shaderList);
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    printOpenGLError();
    
    return shaderProgram;
    
}


#endif
