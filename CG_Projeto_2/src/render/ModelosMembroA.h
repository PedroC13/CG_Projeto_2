#pragma once
#include<GL/freeglut.h>

inline void desenharChao(GLint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não alterar a textura)

	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f); // Normal apontando para cima
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
		glTexCoord2f(5.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
		glTexCoord2f(5.0f, 5.0f); glVertex3f(10.0f, 0.0f, 10.0f);
		glTexCoord2f(0.0f, 5.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
	glEnd();

}
