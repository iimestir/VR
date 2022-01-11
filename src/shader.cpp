#include "headers/shader.h"

std::string getFileContents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vFile, const char* fFile, const char* gFile) {
	std::string vertexCode = getFileContents(vFile);
	std::string fragmentCode = getFileContents(fFile);
	std::string geometryCode;
	if (gFile != NULL)
		geometryCode = getFileContents(gFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	const char* geometrySource;
	if (gFile != NULL) geometrySource = geometryCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLuint geometryShader;

	if (gFile != NULL) {
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometrySource, NULL);
		glCompileShader(geometryShader);
	}

	compileErrors(vertexShader, "VERTEX", vFile);
	compileErrors(fragmentShader, "FRAGMENT", fFile);

	if (gFile != NULL)
		compileErrors(geometryShader, "GEOMETRY", fFile);
	

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (gFile != NULL) glAttachShader(ID, geometryShader);

	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (gFile != NULL) glDeleteShader(geometryShader);
}

void Shader::activateShader() {
	glUseProgram(ID);
}

void Shader::deleteShader() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type, const char* path) {
	GLint hasCompiled;
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR (" << path << ")" << endl;
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << infoLog << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR (" << path << ")" << endl;
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}
