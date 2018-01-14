#include "okno.h"




Shaders::Shaders()
{
	this->szerokoscOkna = 1024;
	this->wysokoscOkna = 768;
	this->posX = 100;
	this->posY = 100;
}

Shaders::Shaders(int szerokoscOkna, int wysokoscOkna, int posX, int posY)
{
	this->szerokoscOkna = szerokoscOkna;
	this->wysokoscOkna = wysokoscOkna;
	this->posX = posX;
	this->posY = posY;
}

void Shaders::inicjalizacjaGlew()

{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "NIe udalo sie zainicjalizowac GlEW.Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}



void Shaders::stworzenieOkna(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(posX, posY);
	glutCreateWindow("Shaders");
}

void Shaders::Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}

void Shaders::stworzenieVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}
void Shaders::stworzenieVBO()
{
	GLfloat Wierzcholki[9] = { -1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wierzcholki), Wierzcholki, GL_STATIC_DRAW);
}

void Shaders::stworzenieProgramu(){

	programZShaderami = glCreateProgram();

	if (programZShaderami == 0)
	{
		std::cerr << "Blad podczas tworzenia programu shaderow." << std::endl;
		system("pause");

		exit(1);
	}
	char * vertexShader =
		"#version 330\n"
		"layout (location = 0) in vec3 polozenie;"
		"void main()"
		"{"
		"gl_Position = vec4(0.5 * polozenie.x, 0.5 * polozenie.y, polozenie.z, 1.0);"
		"}";
	char * fragmentShader =
		"#version 330\n"
		"out vec4 kolor;"
		"void main()"
		"{"
		"kolor = vec4(0.8, 0.8, 0.0, 1.0);"
		"}";

	dodanieDoProgramu(programZShaderami, vertexShader, GL_VERTEX_SHADER);
	dodanieDoProgramu(programZShaderami, fragmentShader, GL_FRAGMENT_SHADER);

	GLint linkowanieOK = 0;
	glLinkProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_LINK_STATUS, &linkowanieOK);
	if (linkowanieOK == GL_FALSE){
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar>log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);

		std::cerr << "Blad podczas linkowania shadera." << std::endl;

		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); i++)
			std::cerr << *i;

		std::cerr << std::endl;
		glDeleteShader(programZShaderami);
		system("pause");
		exit(1);
	}

	GLint walidacjaOK = 0;
	glValidateProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_VALIDATE_STATUS, &walidacjaOK);
	if (walidacjaOK == GL_FALSE){
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar>log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);

		std::cerr << "Blad podczas walidacji shadera." << std::endl;

		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); i++)
			std::cerr << *i;

		std::cerr << std::endl;
		glDeleteShader(programZShaderami);
		system("pause");
		exit(1);
	}
	glUseProgram(programZShaderami);

}

void Shaders::dodanieDoProgramu(GLuint programZShaderami, const GLchar * tekstShadera, GLenum typShadera)
{
	GLuint shader = glCreateShader(typShadera);

	if (shader == 0)
	{
		std::cerr << "Blad podczas tworzenia programu shaderow." << std::endl;
		system("pause");

		exit(1);
	}
	const GLchar* tekstShaderaTab[1];
	tekstShaderaTab[0] = tekstShadera;
	GLint dlugoscShadera[1];
	dlugoscShadera[0] = strlen(tekstShadera);
	glShaderSource(shader, 1, tekstShaderaTab, dlugoscShadera);
	glCompileShader(shader);
	GLint kompilacjaOK = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &kompilacjaOK);
	if (kompilacjaOK == GL_FALSE){
		GLint dlugoscLoga = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar>log(dlugoscLoga);
		glGetShaderInfoLog(shader, dlugoscLoga, NULL, &log[0]);

		std::cerr << "Blad podczas kompilacji shadera." << std::endl;

		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); i++)
			std::cerr << *i;

		std::cerr << std::endl;
		glDeleteShader(shader);
		system("pause");
		exit(1);
	}

	glAttachShader(programZShaderami, shader);

}





int main(int argc, char ** argv)
{
	Shaders Shaders(786, 1024, 100, 100);



	// po zamknieciu okna kontrola wraca do programu
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	Shaders.stworzenieOkna(argc, argv);
	Shaders.inicjalizacjaGlew();



	Shaders.stworzenieVAO();
	Shaders.stworzenieVBO();
	Shaders.stworzenieProgramu();
	glutDisplayFunc(Shaders.Display);

	glClearColor(0.2f, 0.1f, 0.0f, 0.0f);

	glutMainLoop();
	system("pause");
	return 0;
}