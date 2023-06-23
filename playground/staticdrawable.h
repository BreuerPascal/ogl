#ifndef STATICDRAWABLE_H
#define STATICDRAWABLE_H

#include <GL/glew.h>

#include <vector>
#include <string_view>
#include <optional>

#include <glm/glm.hpp>

class StaticDrawable
{
public:
    StaticDrawable(const std::string_view &vertexShader, const std::string_view &fragmentShader, bool hasTexture = false);
    ~StaticDrawable();
    void draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
    void rotate(glm::mat4 roatation);
    void scale(glm::mat4 scale);
    void translate(glm::mat4 translate);
    void setVertices(std::vector<GLfloat> vertices);
    void setTextureUVData(std::vector<GLfloat> texture);
    void setColors(std::vector<GLfloat> colors);
    void setTextureId(GLuint textureId);
    void init();

private:
    std::vector<GLfloat> m_vertices;
    std::vector<GLfloat> m_colors;
    std::vector<GLfloat> m_textureUVData;
    glm::mat4 m_modelMatrix;
    glm::mat4 m_translate;
    glm::mat4 m_scale;
    glm::mat4 m_rotation;
    GLuint m_vertexArrayId;
    GLuint m_vertexbuffer;
    GLuint m_colorbuffer;
    GLuint m_uvBuffer;
    GLuint m_programId;
    GLuint m_matrixId;
    std::optional<GLuint> m_textureUniformId;
    GLuint m_textureId;


};

#endif // STATICDRAWABLE_H
