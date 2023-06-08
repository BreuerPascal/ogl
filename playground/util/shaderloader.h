#ifndef SHADERLOADER_H
#define SHADERLOADER_H


#include <GL/glew.h>

#include <string_view>
#include <string>
#include <optional>
#include <vector>

class ShaderLoader
{
public:
    ShaderLoader();

    GLuint load(const std::string_view vertex_file_path, std::string_view fragment_file_path);

private:
    std::optional<std::string> readShaderFromFile(const std::string_view file_path);

    bool compileAndCheckShader(const std::string_view shader_code, GLuint &shader_id);
    GLuint linkAndCheckProgram(std::vector<GLuint> shaders_to_link);
};

#endif // SHADERLOADER_H
