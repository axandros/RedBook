
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "GL/glew.h"
//#include "GL/freeglut.h"

namespace Utility {
	class ShaderUtil {
	public:
		///<summary>
		/// Reads the contents of the file into a string
		///</summary>
		///<param name="filename"> The name of the file to open.  Can be a relative file path.</param>
		/// <returns> Returns the file contents as a string. </returns>
		static std::string LoadShader(const std::string& filename);

		///<summary>
		/// Creates a shader of type using the source code source.
		///</summary>
		///<param name="shaderType">A GL_ shader type.</param>
		///<param name="source">The sourcecode of the shader to compile.</param>
		/// <returns> Returns the shader program id. </returns>
		static unsigned int CompileShader(unsigned int shaderType, const std::string& source);

		///<summary>
		/// Builds a simple GL program using the provided strings as source code for the required shaders.
		///</summary>
		///<param name="vertexShader">The source code for the vertex shader. </param>
		///<param name="fragmentShader">The source code for the sfragment shader. </param>
		/// <returns> Returns the compiled program id. </returns>
		static int CreateShader(const std::string& vertexShader, const std::string fragmentShader);
	};

	class GLUtil {
	public:
		///<summary>
		/// Get the size of the provided GL EnumType.
		///</summary>
		///<param name="type">The GL Enum Type </param>
		/// <returns> Returns the size of the type for the current OS.</returns>
		static size_t TypeSize(GLenum type);
	};

}

#endif // !UTILITY_H