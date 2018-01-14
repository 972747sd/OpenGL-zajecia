#include <GL\glew.h>
#include <GL\freeglut.h>
#include <vector>
#include <iostream>

#pragma once

GLuint VBO;
GLuint VAO;


class Shaders
{
public:
	Shaders();

	Shaders(int szerokoscOkna, int wysokoscOkna, int posX, int posY);

	//~Shaders(void);




	void stworzenieOkna(int argc, char** argv);
	void inicjalizacjaGlew();



	void stworzenieVAO();
	void stworzenieVBO();
	void stworzenieProgramu();
	void dodanieDoProgramu(GLuint programZShaderami, const GLchar * tekstShadera, GLenum typShadera);

	static void Display();

private:
	int szerokoscOkna;
	int wysokoscOkna;
	int posX;
	int posY;
	GLuint programZShaderami;
};