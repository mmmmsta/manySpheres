

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cerrno>
#include <cstdlib>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "window.h"
#include "utility.h"

void processInput(GLFWwindow *window);
std::string get_file_contents(const char *filename);
void checkCompileErrors(unsigned int shader, std::string type);
float r(float a, float b);



struct Particle{
	glm::vec3 position;
};

int numParticles = 30;

Particle particles[30]; 


int main (){

	// initialilze particle position
	for(int i=0; i<numParticles; i++){

		particles[i].position = glm::vec3(r(-5.0f,5.0f) ,r(-5.0f,5.0f),r(0,5.0f));
	}

	int windowFlag;
	GLFWwindow* window = initializeWindow(&windowFlag);
	if (windowFlag == -1){
		return -1;
	}



	float vertices[] = { -1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f};

	VertexArrayObject vertexAO = VertexArrayObject(vertices, 4);

	vertexAO.bind();

	int indices[] = {0,1,2, 1,2,3};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	GLuint particleBuffer;
	glGenBuffers(1, &particleBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleBuffer);

	std::string vsc = get_file_contents("basic.vert");
	const char* vShaderCode = vsc.c_str();


	std::string fsc = get_file_contents("multi.frag");
	const char* fShaderCode = fsc.c_str();

	
	unsigned int vs, fs;
	// vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vShaderCode, 0);
	glCompileShader(vs);
	checkCompileErrors(vs, "VERTEX");

	// fragment Shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fShaderCode, 0);
	glCompileShader(fs);
	checkCompileErrors(fs, "FRAGMENT");

	// shader Program
	unsigned int ID;
	ID = glCreateProgram();
	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	glLinkProgram(ID);
	checkCompileErrors(ID, "RENDER_PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vs);
	glDeleteShader(fs);


	// compute shader

	cs = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(cs,1,&cShaderCode,0);
	glComplieShader(cs);
	CheckCompileErrors(cs,"COMPUTE");

	// compute shader program
	unsigned int cID;
	cID = glCreateProgram();
	glAttachShader(cID, cs);
	glLinkProgram(cID);
	checkCompileErrors(ID, "COMPUTE_PROGRAM");
	
	glDeleteShader(cs);

	

	glUseProgram(ID);


	glUniform1i(glGetUniformLocation(ID, "numParticles"), numParticles);

	while(!glfwWindowShouldClose(window)){	
		processInput(window);

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		
		glUniform1f(glGetUniformLocation(ID, "width"), width);
		glUniform1f(glGetUniformLocation(ID, "height"), height);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}


	glfwTerminate();
	return 0;



}



void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// from: http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}


    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }



float r(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}