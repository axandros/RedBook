
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "GL/glew.h"
#include "GL/GL.h"
#include "GL/freeglut.h"

#include "Utility.h"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#define BUFFER_OFFSET(x)  ((const void*) (x))

enum VAO_IDS { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init(void) {

	GLuint program;

	glClearColor(1, 0, 0, 1);

	string vs = Utility::ShaderUtil::LoadShader("uniform.vert");
	string fs = Utility::ShaderUtil::LoadShader("uniform.frag");

	program = Utility::ShaderUtil::CreateShader(vs, fs);
	glUseProgram(program);

	// Initialize uniform values in uniform block "Uniforms"
	GLuint uboIndex;
	GLint uboSize = 0;
	GLuint ubo;
	GLvoid *buffer;

	// Find the uniform buffer index for "Uniforms" and  determine the block's sizes;
	uboIndex = glGetUniformBlockIndex(program, "Uniforms");

	glGetActiveUniformBlockiv(program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
	
	buffer = malloc(uboSize);
	
	if (buffer == NULL) {
		fprintf(stderr, "Unable to allocate buffer\n");
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Entered Init Else" << endl;
		
		enum { Translation, Scale, Rotation, Enabled, NumUniforms};

		// Values to be stored in the buffer object.
		GLfloat scale = 0.5;
		GLfloat translation[] = {0.1, 0.1, 0.0};
		GLfloat rotation[] = { 90, 0.0, 0.0, 1.0 };
		GLboolean enabled = GL_TRUE;

		// Since we know the names of the uniforms in our block, make an array of those values
		const char* names[NumUniforms] = {
			"translation",
			"scale",
			"rotation",
			"enabled"
		};

		// Query the necessary attributes to determine where in the buffer we should write the values
		GLuint indices[NumUniforms];
		GLint size[NumUniforms];
		GLint offset[NumUniforms];
		GLint type[NumUniforms];

		glGetUniformIndices(program, NumUniforms, names, indices);
		glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_OFFSET, offset);
		glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_SIZE, size);
		glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_TYPE, type);

		for (int i = 0; i < NumBuffers; i++) {
			cout << "type [" << i << "] = " << type[i] << endl;
		}

		// Copy the uniform values into the buffer
		// Book has buffer + offset[... but this is illegal in MSVC and considered bad practice in most compilers that do allow it.  Ya know, the whole void* thing being dangerous and all.
		memcpy((char*)buffer + offset[Scale]			, &scale		, size[Scale] *			Utility::GLUtil::TypeSize(type[Scale]));	
		memcpy((char*)buffer + offset[Translation]	, &translation	, size[Translation] *	Utility::GLUtil::TypeSize(type[Translation]));
		memcpy((char*)buffer + offset[Rotation]		, &rotation		, size[Rotation] *		Utility::GLUtil::TypeSize(type[Rotation]));
		memcpy((char*)buffer + offset[Enabled]		, &enabled		, size[Enabled] *		Utility::GLUtil::TypeSize(type[Enabled]));

		// Create the uniform buffer object, initialize its storage, and associated it with the shader program
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, uboSize, buffer, GL_STATIC_DRAW); // GL_STATIC_RAW <- book says raw, but this doesn't exist.
		
		glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo);
		
	}


	/// Old INIT
	/*
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


	

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
	*/
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