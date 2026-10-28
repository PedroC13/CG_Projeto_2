#ifndef MODELOS_MEMBRO_C_H
#define MODELOS_MEMBRO_C_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <GL/freeglut.h>
#include <cmath>

// ============================================================================
// 1. MOLDURA DO VITRAL LATERAL
// ============================================================================
inline void desenharVitral(float posX, float posY, float posZ, float rotY) {
    glPushMatrix();
    // Posicionamento e rotação global do objeto
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // --- MOLDURA EXTERNA (Madeira/Pedra) ---
    // Viga Esquerda
    glPushMatrix();
    glTranslatef(-0.85f, 1.5f, 0.0f);
    glScalef(0.1f, 3.0f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Viga Direita
    glPushMatrix();
    glTranslatef(0.85f, 1.5f, 0.0f);
    glScalef(0.1f, 3.0f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Viga Superior
    glPushMatrix();
    glTranslatef(0.0f, 2.95f, 0.0f);
    glScalef(1.8f, 0.1f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Viga Inferior
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(1.8f, 0.1f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- DIVISÓRIAS INTERNAS ---
    // Barra Horizontal Central
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(1.6f, 0.04f, 0.05f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Barra Vertical Central
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.04f, 2.8f, 0.05f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- PLANO DO VIDRO (Vitral) ---
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.8f, -1.4f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.8f, -1.4f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.8f, 1.4f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.8f, 1.4f, 0.0f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

// ============================================================================
// 2. RELÓGIO DE PÊNDULO
// ============================================================================
inline void desenharRelogioPendulo(float posX, float posY, float posZ, float rotY, float anguloPendulo) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // 1. Pedestal do Chão
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glScalef(0.6f, 0.3f, 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // 2. Corpo Principal
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(0.48f, 1.4f, 0.32f);
    glutSolidCube(1.0);
    glPopMatrix();

    // 3. Caixa Superior do Mostrador
    glPushMatrix();
    glTranslatef(0.0f, 1.95f, 0.0f);
    glScalef(0.55f, 0.55f, 0.38f);
    glutSolidCube(1.0);
    glPopMatrix();

    // 4. Mostrador Circular
    glPushMatrix();
    glTranslatef(0.0f, 1.95f, 0.19f);
    glutSolidCylinder(0.2, 0.02, 20, 1);
    glPopMatrix();

    // 5. Pêndulo Animado
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.12f);               // Articulação superior
    glRotatef(anguloPendulo, 0.0f, 0.0f, 1.0f);      // Oscilação no eixo Z

    // Haste
    glPushMatrix();
    glTranslatef(0.0f, -0.35f, 0.0f);
    glScalef(0.02f, 0.7f, 0.02f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Disco da Ponta
    glPushMatrix();
    glTranslatef(0.0f, -0.7f, 0.0f);
    glutSolidCylinder(0.1, 0.02, 16, 1);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

// ============================================================================
// 3. LANTERNA DO VIGILANTE
// ============================================================================
inline void desenharLanterna(float posX, float posY, float posZ, float rotY) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // Base de Metal
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Alinha o cilindro verticalmente
    glutSolidCylinder(0.1, 0.05, 16, 1);
    glPopMatrix();

    // Vidro / Lâmpada
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.07, 0.3, 16, 1);
    glPopMatrix();

    // 4 Hastes de Proteção
    float offsets[2] = { -0.07f, 0.07f };
    for (float x : offsets) {
        for (float z : offsets) {
            glPushMatrix();
            glTranslatef(x, 0.2f, z);
            glScalef(0.015f, 0.3f, 0.015f);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }

    // Tampo Superior
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.11, 0.05, 16, 1);
    glPopMatrix();

    // Alça Superior
    glPushMatrix();
    glTranslatef(0.0f, 0.43f, 0.0f);
    glScalef(0.12f, 0.08f, 0.02f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

#endif