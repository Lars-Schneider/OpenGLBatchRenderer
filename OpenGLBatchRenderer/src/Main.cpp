#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include <string>
#include "Shader.h"
#include <vector>
#include <array>

void ProcessEvents(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
const unsigned int MAX_QUAD_COUNT = 1024;
const unsigned int MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
const unsigned int MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;

struct Vec3
{
	float array[3];
};
struct Vec4
{
	float array[4];
};
struct Vertex
{
	Vec3 Position;
	Vec4 Color;
};

/*
	2__1
	|\ |
	3__0
*/
int indices[MAX_INDEX_COUNT];

void CreateQuad(std::vector<Vertex>& target, float x, float y, float w, float h, float r, float g, float b, float a)
{
	//Bottom right
	Vertex v0;
	v0.Position = { x + w, y - h, 1.f };
	v0.Color = { r,g,b,a };
	target.push_back(v0);

	//Top right
	Vertex v1;
	v1.Position = { x + w, y, 1.f };
	v1.Color = { r, g, b, a };
	target.push_back(v1);

	//Top left
	Vertex v2;
	v2.Position = { x, y, 1.f };
	v2.Color = { r, g, b, a };
	target.push_back(v2);

	//Bottom left
	Vertex v3;
	v3.Position = { x, y - h, 1.f };
	v3.Color = { r, g, b, a };
	target.push_back(v3);
}

int main(int argc, char** argv)
{
	Window* window = new Window(800, 640, "Batch Renderer");
	window->Init();

	Shader batch_shader("Shaders/Batch.vert", "Shaders/Batch.frag");

	
	unsigned int offset = 0;
	for (unsigned int i = 0; i < MAX_INDEX_COUNT; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	//Generate buffers
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Allocate buffers
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);

	//Unbind stuff
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window->GetGLFWWindow()))
	{
		
		ProcessEvents(window->GetGLFWWindow());

		//Set dynamic vertex buffer.
		std::vector<Vertex> vertices{};

		int indexCount = 0;
		for (float y = 0; y <= 1; y += 0.1f)
		{
			for (float x = 0; x <= 1; x += 0.1f)
			{
				CreateQuad(vertices, x, y, 0.1f, 0.1f, 1.f, 0.75f, 0.23f, 1.f);
				indexCount += 6;
			}
		}
		std::cout << indexCount / 6 << "\n";
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());


		glClearColor(0.2125f, 0.4356f, 0.85f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use shader
		batch_shader.use();
		//Binf v
		glBindVertexArray(VAO);
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window->GetGLFWWindow());

		glfwPollEvents();
	}
	delete window;
	return 1;
}

