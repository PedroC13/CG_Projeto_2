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

// Uma folha da porta, desenhada a partir da dobradiça (origem local).
// lado = +1: a folha sai para +X (folha esquerda); lado = -1: sai para -X (folha direita)
inline void folhaPorta(float lado, GLuint texMadeira, GLuint texMetal) {
    float l = lado;   // só para encurtar

    // Madeira: placa principal + 2 almofadas em relevo (dos dois lados)
    glBindTexture(GL_TEXTURE_2D, texMadeira);
    glColor3f(0.55f, 0.35f, 0.25f);
    peca(l * 0.50f, 1.40f, 0.0f, 1.00f, 2.80f, 0.08f);   // placa (centro a meia largura da dobradiça)
    glColor3f(0.45f, 0.28f, 0.2f);
    peca(l * 0.50f, 0.80f, 0.0f, 0.70f, 0.90f, 0.12f);   // almofada de baixo
    peca(l * 0.50f, 2.00f, 0.0f, 0.70f, 1.00f, 0.12f);   // almofada de cima

    // Metal: 3 dobradiças na borda da dobradiça + maçaneta perto do centro
    glBindTexture(GL_TEXTURE_2D, texMetal);
    glColor3f(0.6f, 0.6f, 0.6f);
    peca(l * 0.03f, 0.40f, 0.0f, 0.06f, 0.20f, 0.10f);   // dobradiça de baixo
    peca(l * 0.03f, 1.40f, 0.0f, 0.06f, 0.20f, 0.10f);   // dobradiça do meio
    peca(l * 0.03f, 2.40f, 0.0f, 0.06f, 0.20f, 0.10f);   // dobradiça de cima
    peca(l * 0.90f, 1.10f, -0.08f, 0.05f, 0.25f, 0.05f);   // maçaneta (lado de dentro, -Z)
    peca(l * 0.90f, 1.10f, 0.08f, 0.05f, 0.25f, 0.05f);   // maçaneta (lado de fora, +Z)
}

// Portas duplas da entrada, com batente. angulo: 0 = fechada, 90 = aberta para -Z
inline void desenharPortasDuplas(float posX, float posY, float posZ, float rotY,
    float angulo, GLuint texMadeira, GLuint texMetal) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // Batente (fixo): duas colunas + verga em cima
    glBindTexture(GL_TEXTURE_2D, texMadeira);
    glColor3f(0.35f, 0.22f, 0.15f);
    peca(-1.10f, 1.45f, 0.0f, 0.20f, 2.90f, 0.25f);   // coluna esquerda
    peca(1.10f, 1.45f, 0.0f, 0.20f, 2.90f, 0.25f);   // coluna direita
    peca(0.00f, 2.95f, 0.0f, 2.40f, 0.20f, 0.25f);   // verga

    // Folha esquerda: dobradiça em x = -1, gira +angulo
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);          // 1. vai até a dobradiça
    glRotatef(angulo, 0.0f, 1.0f, 0.0f);      // 2. gira em torno dela
    folhaPorta(+1.0f, texMadeira, texMetal);  // 3. folha sai para +X
    glPopMatrix();

    // Folha direita: dobradiça em x = +1, gira -angulo (espelhada)
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(-angulo, 0.0f, 1.0f, 0.0f);
    folhaPorta(-1.0f, texMadeira, texMetal);  // folha sai para -X
    glPopMatrix();

    glPopMatrix();
}
