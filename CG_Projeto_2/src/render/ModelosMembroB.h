#pragma once
#ifndef MODELOS_MEMBRO_B_H
#define MODELOS_MEMBRO_B_H

#include <GL/freeglut.h>

// Textura de pedra do altar (carregada no setup() do Main.cpp)
extern GLuint texPedraAltar;

// ============================================================================
// AUXILIARES DE MATERIAL
// ============================================================================

// Define cor (tinge a textura) + material Blinn-Phong do pipeline fixo.
// O material só faz efeito com GL_LIGHTING ligado; a cor funciona sempre.
inline void materialPedra(float r, float g, float b, float especular, float brilho) {
    GLfloat ambiente[] = { r * 0.25f, g * 0.25f, b * 0.25f, 1.0f };
    GLfloat difusa[]   = { r, g, b, 1.0f };
    GLfloat spec[]     = { especular, especular, especular, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brilho);
    glColor4f(r, g, b, 1.0f);
}

// ============================================================================
// BLOCO DE PEDRA COM UV PROPORCIONAL AO TAMANHO
// ----------------------------------------------------------------------------
// Diferente de glScalef + cubo unitário, aqui os vértices já saem com as
// dimensões reais (larg x alt x prof). Vantagens:
//   - a textura repete na mesma densidade em todos os blocos (não estica);
//   - as normais continuam unitárias (glScalef deformaria a iluminação).
// ============================================================================
inline void desenharBlocoPedra(float larg, float alt, float prof, float densidadeUV = 1.5f) {
    float x = larg / 2.0f, y = alt / 2.0f, z = prof / 2.0f;
    float u = larg * densidadeUV, v = alt * densidadeUV, w = prof * densidadeUV;

    glBegin(GL_QUADS);
    // Frente (+Z)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0, 0); glVertex3f(-x, -y,  z);
    glTexCoord2f(u, 0); glVertex3f( x, -y,  z);
    glTexCoord2f(u, v); glVertex3f( x,  y,  z);
    glTexCoord2f(0, v); glVertex3f(-x,  y,  z);
    // Trás (-Z)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0, 0); glVertex3f( x, -y, -z);
    glTexCoord2f(u, 0); glVertex3f(-x, -y, -z);
    glTexCoord2f(u, v); glVertex3f(-x,  y, -z);
    glTexCoord2f(0, v); glVertex3f( x,  y, -z);
    // Cima (+Y)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0, 0); glVertex3f(-x,  y,  z);
    glTexCoord2f(u, 0); glVertex3f( x,  y,  z);
    glTexCoord2f(u, w); glVertex3f( x,  y, -z);
    glTexCoord2f(0, w); glVertex3f(-x,  y, -z);
    // Baixo (-Y)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0, 0); glVertex3f(-x, -y, -z);
    glTexCoord2f(u, 0); glVertex3f( x, -y, -z);
    glTexCoord2f(u, w); glVertex3f( x, -y,  z);
    glTexCoord2f(0, w); glVertex3f(-x, -y,  z);
    // Direita (+X)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0, 0); glVertex3f( x, -y,  z);
    glTexCoord2f(w, 0); glVertex3f( x, -y, -z);
    glTexCoord2f(w, v); glVertex3f( x,  y, -z);
    glTexCoord2f(0, v); glVertex3f( x,  y,  z);
    // Esquerda (-X)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0, 0); glVertex3f(-x, -y, -z);
    glTexCoord2f(w, 0); glVertex3f(-x, -y,  z);
    glTexCoord2f(w, v); glVertex3f(-x,  y,  z);
    glTexCoord2f(0, v); glVertex3f(-x,  y, -z);
    glEnd();
}

// Atalho: posiciona (e opcionalmente gira em Z) um bloco de pedra
inline void blocoEm(float px, float py, float pz, float larg, float alt, float prof, float rotZ = 0.0f) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    if (rotZ != 0.0f) glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
    desenharBlocoPedra(larg, alt, prof);
    glPopMatrix();
}

// ============================================================================
// 1. ALTAR PRINCIPAL DE PEDRA
// ----------------------------------------------------------------------------
// Frente voltada para +Z (para a nave). Origem = centro da base no chão.
// Base -> corpo -> tampo, com uma cruz em baixo-relevo na frente.
// O tampo fica em y = 1.0, onde ficam os castiçais.
// ============================================================================
inline void desenharAltar(float posX, float posY, float posZ, float rotY) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texPedraAltar);

    // --- BASE ---
    materialPedra(0.70f, 0.68f, 0.64f, 0.05f, 6.0f);
    blocoEm(0.0f, 0.075f, 0.0f, 2.4f, 0.15f, 1.1f);     // y: 0.00 a 0.15

    // --- CORPO ---
    materialPedra(0.80f, 0.78f, 0.74f, 0.08f, 8.0f);
    blocoEm(0.0f, 0.525f, 0.0f, 2.2f, 0.75f, 0.9f);     // y: 0.15 a 0.90

    // --- TAMPO (pedra escura e polida) ---
    materialPedra(0.55f, 0.53f, 0.50f, 0.30f, 32.0f);
    blocoEm(0.0f, 0.95f, 0.0f, 2.5f, 0.10f, 1.1f);      // y: 0.90 a 1.00

    // --- CRUZ EM RELEVO (sai 4 cm da frente do corpo) ---
    materialPedra(0.40f, 0.38f, 0.36f, 0.10f, 10.0f);
    blocoEm(0.0f, 0.52f, 0.47f, 0.08f, 0.50f, 0.04f);   // haste
    blocoEm(0.0f, 0.62f, 0.47f, 0.32f, 0.08f, 0.04f);   // braços

    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // não vazar a cor para os próximos objetos
    glPopMatrix();
}

#endif
