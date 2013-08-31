//
//  glhelpers.h
//  gltut-glfw
//
//  Created by Ricardo Sánchez-Sáez on 31/08/2013.
//  Copyright (c) 2013 rsanchezsaez. All rights reserved.
//

#ifndef gltut_glfw_glhelpers_h
#define gltut_glfw_glhelpers_h

GLuint createAndCompilerShader(GLenum shaderType, const std::string &strShaderFile)
{
    GLuint shader = glCreateShader(shaderType);
    const char *strFileData = strShaderFile.c_str();
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

GLuint createShaderProgram(const std::string &vertexShaderString,
                           const std::string &fragmentShaderString)
{
    // Compiler shaders and create program
    std::vector<GLuint> shaderList;
    GLuint vertexShader = createAndCompilerShader(GL_VERTEX_SHADER, vertexShaderString);
	shaderList.push_back( vertexShader );
    GLuint fragmentShader = createAndCompilerShader(GL_FRAGMENT_SHADER, fragmentShaderString);
	shaderList.push_back( fragmentShader );
    
    // Default behavior already
    // glBindFragDataLocationEXT( fragmentShader, 0, "outColor" );
    
	GLuint shaderProgram = createProgramWithShaderList(shaderList);
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    printOpenGLError();
    
    return shaderProgram;
    
}


#endif
