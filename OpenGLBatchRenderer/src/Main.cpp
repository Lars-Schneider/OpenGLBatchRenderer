#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include <string>
#include "Shader.h"
#include <vector>

void ProcessEvents(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

struct Vertex
{
	float Position[3];
	float Color[3];
};

std::vector<float> vertices
{
	//Positions			 //Colors
	 0.5,  -0.5f,  1.f,    0.13f,  0.3f,   0.3f,
	 0.5f,  0.5f,  1.f,    0.7f,   0.1f,   0.9f,
    -0.5f,  0.5f,  1.f,    0.3f,   0.3f,   0.3f,
	-0.5f, -0.5f,  1.f,    0.1f,   0.9f,   0.5f
};

//Indices
/*
	2__1
	|\ |
	3__0
*/
std::vector<unsigned int> indices
{
	0,1,2,
	2,3,0
};

int main(int argc, char** argv)
{
	Window* window = new Window(800, 640, "Batch Renderer");
	window->Init();

	Shader batch_shader("Shaders/Batch.vert", "Shaders/Batch.frag");

	//Generate buffers
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glGenBuffers(1, &EBO);

	//Allocate buffers
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), nullptr, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Unbind stuff
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window->GetGLFWWindow()))
	{
		
		ProcessEvents(window->GetGLFWWindow());

		glClearColor(0.2125f, 0.4356f, 0.85f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use shader
		batch_shader.use();
		//Binf v
		glBindVertexArray(VAO);
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window->GetGLFWWindow());

		glfwPollEvents();
	}
	delete window;
	return 1;
}

