#pragma once
#include <GL/freeglut.h>
#include <render/ModelosMembroC.h>

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

inline void peca(float x, float y, float z, float largura, float altura, float profundidade) {
	glPushMatrix();

	glTranslated(x, y, z);
	glScalef(largura, altura, profundidade);
	
	desenharCuboTexturizado(1.0f);
	glPopMatrix();	
}

inline void desenharBanco(float posX, float posY, float posZ, float rotY, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);

	glColor3f(0.75f, 0.55f, 0.4f);

	glColor3f(0.75f, 0.55f, 0.4f);                  // tinge a madeira para um tom mais escuro

	glPushMatrix();
	glTranslatef(posX, posY, posZ);                 // posição do banco inteiro na cena
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);

	peca(0.0f, 0.45f, 0.0f, 2.0f, 0.05f, 0.40f);   // assento
	peca(-0.98f, 0.35f, 0.02f, 0.05f, 0.70f, 0.45f);  // lateral esquerda
	peca(0.98f, 0.35f, 0.02f, 0.05f, 0.70f, 0.45f);  // lateral direita
	peca(0.0f, 0.12f, 0.0f, 1.90f, 0.05f, 0.05f);  // travessa de baixo

	// Encosto: gira 10° em torno da sua borda de baixo
	glPushMatrix();
	glTranslatef(0.0f, 0.475f, 0.18f);              // vai até a dobra (borda de trás do assento)
	glRotatef(10.0f, 1.0f, 0.0f, 0.0f);             // inclina para trás (+Z)
	peca(0.0f, 0.275f, 0.0f, 1.95f, 0.55f, 0.04f); // sobe metade da altura
	glPopMatrix();

	glPopMatrix();

}

// Duas colunas de bancos, com um corredor no meio
inline void desenharFileirasBancos(GLuint textura) {
	for (int i = 0; i < 5; ++i) {
		float z = -1.5f - i * 1.3f;                 // uma fileira a cada 1,3 m
		desenharBanco(-1.5f, 0.0f, z, 0.0f, textura);   // lado esquerdo
		desenharBanco(1.5f, 0.0f, z, 0.0f, textura);   // lado direito
	}
}
