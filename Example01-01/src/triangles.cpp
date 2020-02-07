

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "GL/glew.h"
#include "GL/GL.h"
#include "GL/freeglut.h"

//#include "LoadShaders.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#define BUFFER_OFFSET(x)  ((const void*) (x))

enum VAO_IDS {Triangles, NumVAOs};
enum Buffer_IDs { ArrayBuffer, NumBuffers};
enum Attrib_IDs{vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

string LoadShader(const string& filename) {
	ifstream stream(filename);

	string ret = "";
	string line;
	while (getline(stream, line)) {
		ret += line + '\n';
	}

	//cout << "[debug] " << ret << endl;

	return ret;
}

static unsigned int CompileShader(unsigned int shaderType, const string& source) {
	unsigned int id = glCreateShader(shaderType);

	string stringSRC = source;
	const char* src = stringSRC.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){ 
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length*sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		string shaderTypeString;
		switch (shaderType) {
		case GL_VERTEX_SHADER: shaderTypeString = "Vertex";
			break;
		case GL_FRAGMENT_SHADER: shaderTypeString = "Fragment";
			break;
		}

		cout << "Failed to compile " << shaderTypeString << " shader! " << endl;
		cout << message << endl;
		glDeleteShader(id);
		id = 0;
	}

	return id;
}

static int CreateShader(const string& vertexShader, const string fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	return program;
}

void init(void) {
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[NumVertices][2] = {
		{ -0.90f, -0.90f }, // Triangle 1
		{  0.85f, -0.90f },
		{ -0.90f,  0.85f },
		{  0.90f, -0.85f }, // Triangle 2
		{  0.90f,  0.90f },
		{ -0.85f,  0.90f }
	};

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	string vs = LoadShader("triangles.vert");
		/*"#version 430 core\n"
		"layout(location = 0) in vec4 vPosition;\n"
		"void main(){\n"
		"gl_Position = vPosition;	}";*/
	string fs = LoadShader("triangles.frag");
		/*"#version 430 core\n"
		"out vec4 fColor;\n"
		"void main() {\n"
		"color = vec4(0.0, 0.0, 1.0, 1.0);}";*/

	GLuint program = CreateShader(vs, fs);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glFlush();
}

int main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);

	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}

	init();
	
	glutDisplayFunc(display);
	glutMainLoop();
}