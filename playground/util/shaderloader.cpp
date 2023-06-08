#include "shaderloader.h"

#include <stdio.h>
#include <fstream>
#include <sstream>

ShaderLoader::ShaderLoader()
{

}

GLuint ShaderLoader::load(const std::string_view vertex_file_path, std::string_view fragment_file_path)
{
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    std::optional<std::string> vertex_shader_code = readShaderFromFile(vertex_file_path);
    if(vertex_shader_code.has_value()) {
        return 0;
    }
    std::optional<std::string> fragment_shader_code = readShaderFromFile(fragment_file_path);
    if(fragment_shader_code.has_value()) {
        return 0;
    }

    // Compile and check VertexShader
    printf("Compiling shader : %s\n", vertex_file_path.data());
    compileAndCheckShader(vertex_shader_code.value(), vertex_shader_id);
    // Compile and check FragmentShader
    printf("Compiling shader : %s\n", fragment_file_path.data());
    compileAndCheckShader(fragment_shader_code.value(), fragment_shader_id);

    return linkAndCheckProgram({vertex_shader_id, fragment_shader_id});
}

std::optional<std::string> ShaderLoader::readShaderFromFile(const std::string_view file_path)
{
    std::string shader_code{};
    std::ifstream shader_stream{file_path.data(), std::ios::in};
    if(shader_stream.is_open()) {
        std::stringstream sstr{};
        sstr << shader_stream.rdbuf();
        shader_code = sstr.str();
        shader_stream.close();
        return std::optional<std::string>{shader_code};
    } else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ!\n", file_path.data());
        getchar();
        return std::nullopt;
    }
}

bool ShaderLoader::compileAndCheckShader(const std::string_view shader_code, GLuint &shader_Id)
{
    GLint result = GL_FALSE;
    int info_log_length;

    // compile shader
    char const* shader_sourc_pointer = shader_code.data();
    glShaderSource(shader_Id, 1, &shader_sourc_pointer, nullptr);
    glCompileShader(shader_Id);

    // check shader
    glGetShaderiv(shader_Id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_Id, GL_INFO_LOG_LENGTH, &info_log_length);
    if(info_log_length > 0) {
        std::vector<char> shader_error_message(info_log_length+1);
        glGetShaderInfoLog(shader_Id, info_log_length, nullptr, &shader_error_message[0]);
        printf("%s\n", &shader_error_message[0]);
        return false;
    }
    return true;
}

GLuint ShaderLoader::linkAndCheckProgram(std::vector<GLuint> shaders_to_link)
{
    // link the program
    printf("Linking program\n");
    GLuint program_id = glCreateProgram();
    for(GLuint shader : shaders_to_link) {
        glAttachShader(program_id, shader);
    }
    glLinkProgram(program_id);

    // check the program
    GLint result = GL_FALSE;
    int info_log_length;

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> program_error_message(info_log_length + 1);
        glGetProgramInfoLog(program_id, info_log_length, nullptr, &program_error_message[0]);
        printf("%s\n", &program_error_message[0]);
    }

    for(GLuint shader : shaders_to_link) {
        glDetachShader(program_id, shader);
        glDeleteShader(shader);
    }

    return program_id;
}
