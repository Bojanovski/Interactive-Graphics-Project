#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace engine
{
    class Texture
    {
    public:

        void Bind() const;
        static void UnBind();

        void LoadFromFile(const char *filename);
        void Destroy();

        void GenerateMipmaps() const;
        void TexParami(GLenum paramName, GLuint param) const;
        void TexParamf(GLenum paramName, GLfloat param) const;
        void TexParamIuiv(GLenum paramName, const GLuint *param) const;
        void TexParamIiv(GLenum paramName, const GLint *param) const;
        void TexParamfv(GLenum paramName, const GLfloat *param) const;

		glm::vec2 GetSize();

    private:
        GLuint ID;
		glm::vec2 size;
    };
}

#endif
