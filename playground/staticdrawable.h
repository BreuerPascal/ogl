#ifndef STATICDRAWABLE_H
#define STATICDRAWABLE_H

#include <GL/glew.h>

#include <vector>
#include <string_view>

#include <glm/glm.hpp>

class StaticDrawable
{
public:
    StaticDrawable(const std::string_view &vertexShader, const std::string_view &fragmentShader);
    ~StaticDrawable();
    void draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
    void rotate(glm::mat4 roatation);
    void scale(glm::mat4 scale);
    void translate(glm::mat4 translate);
    void setVertices(std::vector<GLfloat> vertices);
    void setColors(std::vector<GLfloat> colors);
    void init();

private:
    std::vector<GLfloat> m_vertices;
    std::vector<GLfloat> m_colors;
    glm::mat4 m_modelMatrix;
    glm::mat4 m_translate;
    glm::mat4 m_scale;
    glm::mat4 m_rotation;
    GLuint m_vertexArrayId;
    GLuint m_vertexbuffer;
    GLuint m_colorbuffer;
    GLuint m_programId;
    GLuint m_matrixId;


};

#endif // STATICDRAWABLE_H
