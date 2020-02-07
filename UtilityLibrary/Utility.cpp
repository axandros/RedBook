#include <iostream>
#include <fstream>
#include <string>

#include "GL/glew.h"

using namespace std;

#include "Utility.h"

namespace Utility {

	string ShaderUtil::LoadShader(const string& filename) {
		ifstream stream(filename);

		string ret = "";
		string line;
		while (getline(stream, line)) {
			ret += line + '\n';
		}

		//cout << "[debug] " << ret << endl;

		return ret;
	}

	unsigned int ShaderUtil::CompileShader(unsigned int shaderType, const string& source) {
		unsigned int id = glCreateShader(shaderType);

		string stringSRC = source;
		const char* src = stringSRC.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
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

	int ShaderUtil::CreateShader(const string& vertexShader, const string fragmentShader) {
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);
		return program;
	}
}