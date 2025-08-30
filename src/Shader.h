//
// Created by kaguya on 6/17/24.
//

#ifndef SHADERPLAYGROUND_SHADER_H
#define SHADERPLAYGROUND_SHADER_H

#include <GL/glew.h>
#include <format>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

enum ShaderType
{
	VERTEX = 0x01,
	FRAGMENT = 0x02,
	GEOMETRY = 0x04
	// Add the others later...
};

class ShaderTypeConverter
{
  public:
	static GLenum ToGlenum(ShaderType type);
	static ShaderType FromExtension(const char *ext);
	// static std::string ToString(ShaderType type);
};

class Shader
{
  public:
	void DestroyShader();
	void ReloadShader();
	void Use() const;

	Shader &CreateProgram();
	Shader &AttachShader(const char *path, ShaderType shaderType);
	void LinkProgram();

	/**
	 * Sets an integer value in the shader.
	 * @param name Uniform name
	 * @param value Value to send
	 */
	void Set(const char *name, int value) const;

	/**
	 * Sets a boolean value in the shader.
	 * @param name Uniform name
	 * @param value Value to send
	 */
	void Set(const char *name, bool value) const;

	/**
	 * Sets a float value in the shader.
	 * @param name Uniform name
	 * @param value Value to send
	 */
	void Set(const char *name, float value) const;
	void Set(GLint id, int value) const;
	void Set(GLint id, bool value) const;
	void Set(GLint id, float value) const;

	/**
	 * Gets a uniform id from the shader.
	 * @param name Uniform name
	 * @return Uniform id
	 */
	[[nodiscard]] GLint GetUniformLocation(const char *name) const;

	/**
	 * Sets a int <size> vector in the shader.
	 * @tparam size Vector Size
	 * @param name Uniform name
	 * @param vector Vector to send
	 */
	template <unsigned int size>
	void Set(const char *name, const glm::vec<size, int> &vector);

	/**
	 * Sets a float vector in the shader
	 * @tparam size Vector size.
	 * @param name Uniform name
	 * @param vector Vector to send
	 */
	template <unsigned int size>
	void Set(const char *name, const glm::vec<size, float> &vector);

	template <unsigned int size_x, unsigned int size_y>
	void Set(const char *name, const glm::mat<size_x, size_y, float> &matrix);

	template <unsigned int size>
	void Set(GLint id, const glm::vec<size, int> &vector);

	template <unsigned int size>
	void Set(GLint id, const glm::vec<size, float> &vector);

	template <unsigned int size_x, unsigned int size_y>
	void Set(GLint id, const glm::mat<size_x, size_y, float> &matrix);

	[[nodiscard]] bool IsLinked() const;

  private:
	GLuint programId = 0;
	bool isLinked = false;
	unsigned int shaderTypes = 0;
	std::unordered_map<unsigned int, std::string> shadersAttached;
	static GLuint CompileShader(GLuint program, const char *shaderCode, GLenum type);
	static std::string ReadFile(const char *file);
};

template <unsigned int size>
void Shader::Set(const char *name, const glm::vec<size, int> &vector)
{
	switch (size)
	{
	case 2:
		glUniform2iv(glGetUniformLocation(programId, name), 1, glm::value_ptr(vector));
		break;
	case 3:
		glUniform3iv(glGetUniformLocation(programId, name), 1, glm::value_ptr(vector));
		break;
	case 4:
		glUniform4iv(glGetUniformLocation(programId, name), 1, glm::value_ptr(vector));
		break;
	default:;
	}
}

template <unsigned int size>
void Shader::Set(const char *name, const glm::vec<size, float> &vector)
{
	switch (size)
	{
	case 2:
		glUniform2fv(glGetUniformLocation(programId, name), 1, glm::value_ptr(vector));
		break;
	case 3:
		glUniform3fv(glGetUniformLocation(programId, name), 1, glm::value_ptr(vector));
		break;
	case 4:
		glUniform4fv(glGetUniformLocation(programId, name), 1, glm::value_ptr(vector));
		break;
	default:;
	}
}

template <unsigned int size_x, unsigned int size_y>
void Shader::Set(const char *name, const glm::mat<size_x, size_y, float> &matrix)
{
	switch (size_x)
	{
	case 2:
		switch (size_y)
		{
		case 2:
			glUniformMatrix2fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 3:
			glUniformMatrix2x3fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 4:
			glUniformMatrix2x4fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		default:;
		}
		break;
	case 3:
		switch (size_y)
		{
		case 2:
			glUniformMatrix3x2fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 3:
			glUniformMatrix3fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 4:
			glUniformMatrix3x4fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		default:;
		}
		break;
	case 4:
		switch (size_y)
		{
		case 2:
			glUniformMatrix4x2fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 3:
			glUniformMatrix4x3fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 4:
			glUniformMatrix4fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		default:;
		}
	default:;
	}
}

template <unsigned int size>
void Shader::Set(const GLint id, const glm::vec<size, int> &vector)
{
	switch (size)
	{
	case 2:
		glUniform2iv(id, 1, glm::value_ptr(vector));
		break;
	case 3:
		glUniform3iv(id, 1, glm::value_ptr(vector));
		break;
	case 4:
		glUniform4iv(id, 1, glm::value_ptr(vector));
		break;
	default:;
	}
}

template <unsigned int size>
void Shader::Set(const GLint id, const glm::vec<size, float> &vector)
{
	switch (size)
	{
	case 2:
		glUniform2fv(id, 1, glm::value_ptr(vector));
		break;
	case 3:
		glUniform3fv(id, 1, glm::value_ptr(vector));
		break;
	case 4:
		glUniform4fv(id, 1, glm::value_ptr(vector));
		break;
	default:;
	}
}

template <unsigned int size_x, unsigned int size_y>
void Shader::Set(const GLint id, const glm::mat<size_x, size_y, float> &matrix)
{
	switch (size_x)
	{
	case 2:
		switch (size_y)
		{
		case 2:
			glUniformMatrix2fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 3:
			glUniformMatrix2x3fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 4:
			glUniformMatrix2x4fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		default:;
		}
		break;
	case 3:
		switch (size_y)
		{
		case 2:
			glUniformMatrix3x2fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 3:
			glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 4:
			glUniformMatrix3x4fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		default:;
		}
		break;
	case 4:
		switch (size_y)
		{
		case 2:
			glUniformMatrix4x2fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 3:
			glUniformMatrix4x3fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		case 4:
			glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
			break;
		default:;
		}
	default:;
	}
}
#endif // SHADERPLAYGROUND_SHADER_H
