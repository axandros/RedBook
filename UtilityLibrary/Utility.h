
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "GL/glew.h"
//#include "GL/freeglut.h"

namespace Utility {
	class ShaderUtil {
	public:
		static std::string LoadShader(const std::string& filename);
		static unsigned int CompileShader(unsigned int shaderType, const std::string& source);
		static int CreateShader(const std::string& vertexShader, const std::string fragmentShader);
	};

	class GLUtil {
	public:
		static size_t TypeSize(GLenum type);
	};

}

#endif // !UTILITY_H