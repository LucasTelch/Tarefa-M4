#include <iostream>
#include <string>
#include <assert.h>

#include<stb_image.h>


using namespace std;

#include "Shader.h"

#include "Sprite.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint loadTexture(string texturePath);

const GLuint WIDTH = 1280, HEIGHT = 720;


Sprite spr;


int main()
{
	

	glfwInit();

	srand((int)glfwGetTime());



	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Teste M4", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	Shader shader("../shaders/tex.vs", "../shaders/tex.fs");

	GLuint texID = loadTexture("../../Textures/pixelWall.png");
	GLuint texID2 = loadTexture("../../Textures/car.png");
	GLuint texID3 = loadTexture("../../Textures/fundocidade.png");
	GLuint texID4 = loadTexture("../../Textures/policia.png");


	spr.setShader(&shader);
	spr.inicializar(texID2, glm::vec3(400.0, 150.0, 0.0), glm::vec3(128, 128, 1.0));
	
	Sprite background;
	background.setShader(&shader);
	background.inicializar(texID3, glm::vec3(400.0, 300.0, 0.0), glm::vec3(1920.0/2.0, 1080.0/2.0, 1.0));

	Sprite bomba;
	bomba.setShader(&shader);
	bomba.inicializar(texID4, glm::vec3(400.0, 900.0, 0.0), glm::vec3(128.0, 128.0, 1.0));


	glActiveTexture(GL_TEXTURE0);

	shader.Use();

	glm::mat4 projection = glm::ortho(0.0,800.0,0.0,600.0,-1.0,1.0);
	shader.setMat4("projection", glm::value_ptr(projection));
	
	shader.setInt("texBuffer", 0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height); 
		
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		background.desenhar();
		
		spr.desenhar();

		bomba.cair();
		bomba.desenhar();

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!spr.getJumping())
		{
			spr.setJumping(true);
			spr.setOnGround(false);
		}
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		spr.setJumping(false);
		spr.setJumpVel(glm::vec2(0.0, 0.5));
	}

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		spr.moverParaDireita();
		if (spr.getJumping())
		{
			spr.setJumpVel(glm::vec2(0.5, 0.5));
		}
	}
	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		spr.moverParaEsquerda();
		if (spr.getJumping())
		{
			spr.setJumpVel(glm::vec2(-0.5, 0.5));
		}
	}
	
}


GLuint loadTexture(string texturePath)
{
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

