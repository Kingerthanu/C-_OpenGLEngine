#include "shader.h"

std::string get_file_contents(const char* filename)
{
	// Get binary of file into stream
	std::ifstream input(filename, std::ios::binary);


	// If found
	if (input) {
		std::string content;

		// From first to last go through 
		input.seekg(0, std::ios::end);
		
		

		// Resize returned content to size of input
		content.resize(input.tellg());

		// Go to first element and read through while placing in content
		input.seekg(0, std::ios::beg);
		input.read(&content[0], content.size());
		input.close();
		//std::cout << content << '\n';
		return (content);

	}
	throw(errno);


}

void Shader::compileErrors(unsigned int shader, const char* type)
{

	GLint hasCompiled;
	char infoLog[1024];

	if (type != "PROGRAM") {

		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE) {

			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "Shader had a error compiling for " << type << "\n\n";

		}

	}
	else {

		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {

			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "Shader had a linking error for " << type << "\n\n";

		}
	}

}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	
	// Grab shader progs

	std::string vCode = get_file_contents(vertexFile);
	std::string fCode = get_file_contents(fragmentFile);

	const char* vertexSource = vCode.c_str();
	const char* fragmentSource = fCode.c_str();

	// Allocate for a shader
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

	// Tell GL to use this source code for its shading
	glShaderSource(vertShader, 1, &vertexSource, nullptr);
	glCompileShader(vertShader);
	compileErrors(vertShader, "VERTEX");	// Check if successfulling working

	
	// Tell GL to use this source code for its shading
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(fragShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragShader);
	compileErrors(fragShader, "FRAGMENT");	// Check if successfulling working


	// Will work as shading engine ( Assembly line for rendering )
	this->ID = glCreateProgram();

	// Attach shader to given pieces
	glAttachShader(this->ID, vertShader);
	glAttachShader(this->ID, fragShader);

	// Link parts into overall whole of shader
	glLinkProgram(this->ID);
	compileErrors(ID, "PROGRAM"); // Check if successfulling working

	// Remove tmp handlers
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);


}

void Shader::Activate()
{
	// Start shader message to openGL
	glUseProgram(this->ID);

}

void Shader::Delete()
{
	// Terminate shader message to openGL
	glDeleteProgram(this->ID);

}