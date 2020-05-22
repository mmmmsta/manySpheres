#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>





class VertexArrayObject {
	

public:

	GLuint VAO;
	GLuint VBO;

	VertexArrayObject(void* vertices, unsigned int numVertices) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO); // vao saves buffer and layout of data

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float)*3, vertices, GL_STATIC_DRAW);

		//position
		// 2: 3 coordinates; 3: type; 4: normalization; 5: stride; 6: offset 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

	}

	virtual ~VertexArrayObject() {
		// i dont know if this is correct
		glDeleteVertexArrays(1, &VAO);

	}

	void bind() {
		glBindVertexArray(VAO);
	}

	void unbind() {
		glBindVertexArray(0);
	}


	
	
};

