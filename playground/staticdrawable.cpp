#include "staticdrawable.h"

#include "util/shaderloader.h"

#include <iostream>

StaticDrawable::StaticDrawable(const std::string_view &vertexShader, const std::string_view &fragmentShader, bool hasTexture)
{
    glGenVertexArrays(1, &m_vertexArrayId);
    ShaderLoader shaderLoader{};
    m_programId = shaderLoader.load(vertexShader, fragmentShader);
    m_matrixId = glGetUniformLocation(m_programId, "MVP");
    if(hasTexture) {
        m_textureUniformId.emplace(glGetUniformLocation(m_programId, "myTextureSampler"));
    }

}

StaticDrawable::~StaticDrawable()
{
    glDeleteBuffers(1, &m_vertexbuffer);
    glDeleteBuffers(1, &m_colorbuffer);
    glDeleteProgram(m_programId);
    glDeleteVertexArrays(1, &m_vertexArrayId);
}

void StaticDrawable::draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    glBindVertexArray(m_vertexArrayId);
    glUseProgram(m_programId);
    glm::mat4 mvp = projectionMatrix * viewMatrix * (m_translate * m_rotation * m_scale);
    glUniformMatrix4fv(m_matrixId, 1, GL_FALSE, &mvp[0][0]);
    // first attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glVertexAttribPointer(
        0,              // attribute 0, No particualr reason for 0 bust must match the layout in the shader.
        3,              // size
        GL_FLOAT,       // type
        GL_FALSE,       // normalized?
        0,              // stride
        (void*)0        // array buffer offset
    );
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    if(m_textureUVData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
        glVertexAttribPointer(
                    1,          // attribute. No particular reason for 1, but must match the layout in the shader.
                    3,          // size
                    GL_FLOAT,   // type
                    GL_FALSE,   // normalized?
                    0,          // stride
                    (void*) 0   // array buffer offset
        );
    } else {
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(m_textureUniformId.value(), 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
        glVertexAttribPointer(
                    1,          // attribute. No particular reason for 1, but must match the layout in the shader.
                    2,          // size
                    GL_FLOAT,   // type
                    GL_FALSE,   // normalized?
                    0,          // stride
                    (void*) 0   // array buffer offset
        );
    }

    // Draw the triangle!
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size()); // 12 triangles with 3 vertices each
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void StaticDrawable::rotate(glm::mat4 rotation)
{
    m_rotation = rotation;
}

void StaticDrawable::scale(glm::mat4 scale)
{
    m_scale = scale;
}

void StaticDrawable::translate(glm::mat4 translate)
{
    m_translate = translate;
}

void StaticDrawable::setVertices(std::vector<GLfloat> vertices)
{
    m_vertices = vertices;
}

void StaticDrawable::setTextureUVData(std::vector<GLfloat> texture)
{
    m_textureUVData = texture;
}

void StaticDrawable::setColors(std::vector<GLfloat> colors)
{
    m_colors = colors;
}

void StaticDrawable::setTextureId(GLuint textureId)
{
    m_textureId = textureId;
}

void StaticDrawable::init()
{
    glGenBuffers(1, &m_vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    // Give our vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);

//    if(!m_colors.empty()) {

//        glGenBuffers(1, &m_colorbuffer);
//        // The following commands will talk about our 'vertexbuffer' buffer
//        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
//        // Give our vertices to OpenGL
//        glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(GLfloat), m_colors.data(), GL_STATIC_DRAW);
//    }

    if(!m_textureUVData.empty()) {
        glGenBuffers(1, &m_uvBuffer);
        // The following commands will talk about our 'vertexbuffer' buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
        // Give our vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, m_textureUVData.size() * sizeof(GLfloat), m_textureUVData.data(), GL_STATIC_DRAW);
    } else {
        glGenBuffers(1, &m_colorbuffer);
        // The following commands will talk about our 'vertexbuffer' buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
        // Give our vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(GLfloat), m_colors.data(), GL_STATIC_DRAW);
    }
}

