#include "Sprite.h"

Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::inicializar(GLuint texID, glm::vec3 pos, glm::vec3 escala, float angulo)
{
	this->texID = texID;
	this->pos = pos;
	this->escala = escala;
	this->angulo = angulo;

	isJumping = false;
	onGround = true;

	jumpVel.x = 0.0;
	jumpVel.y = 0.5;
	vel = 1.0;



	GLfloat vertices[] = {
		-0.5 , 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,  //v0
		-0.5 ,-0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,  //v1
		 0.5 , 0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0,  //v2
	    -0.5 ,-0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,  //v1
		 0.5 ,-0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,  //v3
		 0.5 , 0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0   //v2
		
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void Sprite::desenhar()
{
	atualizar();

	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Sprite::moverParaDireita()
{
	pos.x += vel;
	if (escala.x < 0.0)
		escala.x = -escala.x;
	
}

void Sprite::moverParaEsquerda()
{
	pos.x -= vel;
	if (escala.x > 0.0)
		escala.x = -escala.x;
}

void Sprite::cair()
{
	if (pos.y >= 100.0)
	{
		pos.y -= 2.5;
	}
	else
	{
		pos.x = 10 + rand() % 781;
		pos.y = 1000.0;
	}
}

void Sprite::atualizar()
{
	if (pos.y >= 500 && isJumping)
	{
		isJumping = false;
	}
	if (isJumping)
	{
		pos.x = pos.x + jumpVel.x * vel;
		pos.y = pos.y + jumpVel.y * vel;
	}
	else {
		if (pos.y <= 150.0 && !onGround)
		{
			pos.y = 150.0;
			onGround = true;
		}
		else
		{
			if (!onGround)
				pos.y -= jumpVel.y * vel; 
		}
		if (pos.y >= 300 && isJumping)
		{
			isJumping = false;
		}
	}

	
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, escala);
	shader->setMat4("model", glm::value_ptr(model));

}
