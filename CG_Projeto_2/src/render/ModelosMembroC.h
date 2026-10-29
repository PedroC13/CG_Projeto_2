#ifndef MODELOS_MEMBRO_C_H
#define MODELOS_MEMBRO_C_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <GL/freeglut.h>
#include <cmath>

extern GLuint texMadeiraRelogio;
extern GLuint texMostradorRelogio;
extern GLuint texLanterna;
extern GLuint texVitral;
extern GLuint texMolduraVitral;

// Função auxiliar para desenhar um cubo com coordenadas de textura UV em todas as faces
inline void desenharCuboTexturizado(float tamanho) {
    float h = tamanho / 2.0f;

    glBegin(GL_QUADS);
    // Face Frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-h, -h, h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(h, -h, h);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(h, h, h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-h, h, h);

    // Face Traseira
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-h, -h, -h);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-h, h, -h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(h, h, -h);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(h, -h, -h);

    // Face Superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-h, h, -h);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-h, h, h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(h, h, h);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(h, h, -h);

    // Face Inferior
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-h, -h, -h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(h, -h, -h);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(h, -h, h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-h, -h, h);

    // Face Direita
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(h, -h, -h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(h, h, -h);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(h, h, h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(h, -h, h);

    // Face Esquerda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-h, -h, -h);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-h, -h, h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-h, h, h);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-h, h, -h);
    glEnd();
}

// ============================================================================
// 1. MOLDURA E VIDRO DO VITRAL LATERAL (Com Textura Completa)
// ============================================================================
inline void desenharVitral(float posX, float posY, float posZ, float rotY) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // --- MOLDURA E DIVISÓRIAS (Textura de Madeira/Pedra/Metal) ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMolduraVitral);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Viga Esquerda
    glPushMatrix();
    glTranslatef(-0.85f, 1.5f, 0.0f);
    glScalef(0.1f, 3.0f, 0.15f);
    desenharCuboTexturizado(1.0f);
    glPopMatrix();

    // Viga Direita
    glPushMatrix();
    glTranslatef(0.85f, 1.5f, 0.0f);
    glScalef(0.1f, 3.0f, 0.15f);
    desenharCuboTexturizado(1.0f);
    glPopMatrix();

    // Viga Superior
    glPushMatrix();
    glTranslatef(0.0f, 2.95f, 0.0f);
    glScalef(1.8f, 0.1f, 0.15f);
    desenharCuboTexturizado(1.0f);
    glPopMatrix();

    // Viga Inferior
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(1.8f, 0.1f, 0.15f);
    desenharCuboTexturizado(1.0f);
    glPopMatrix();

    // Barra Horizontal Central
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(1.6f, 0.04f, 0.05f);
    desenharCuboTexturizado(1.0f);
    glPopMatrix();

    // Barra Vertical Central
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.04f, 2.8f, 0.05f);
    desenharCuboTexturizado(1.0f);
    glPopMatrix();

    // --- PLANO DO VIDRO (Vitral Colorido) ---
    glBindTexture(GL_TEXTURE_2D, texVitral);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.8f, 0.1f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.8f, 0.1f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.8f, 2.9f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.8f, 2.9f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
// ============================================================================
// 2. RELÓGIO DE PÊNDULO (Corpo + Mostrador + Pêndulo Visível)
// ============================================================================
inline void desenharRelogioPendulo(float posX, float posY, float posZ, float rotY, float anguloPendulo) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    // --- 1. ESTRUTURA DE MADEIRA ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMadeiraRelogio);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Pedestal do Chão
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glScalef(0.6f, 0.3f, 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Corpo Principal (Espessura Z = 0.32 / 2 = 0.16)
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(0.48f, 1.4f, 0.32f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Caixa Superior do Mostrador (Espessura Z = 0.38 / 2 = 0.19)
    glPushMatrix();
    glTranslatef(0.0f, 1.95f, 0.0f);
    glScalef(0.55f, 0.55f, 0.38f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- 2. MOSTRADOR CIRCULAR COM AS HORAS ---
    glBindTexture(GL_TEXTURE_2D, texMostradorRelogio);

    glPushMatrix();
    // Posicionado logo à frente da caixa superior (Z > 0.19)
    glTranslatef(0.0f, 1.95f, 0.201f);
    gluDisk(quad, 0.0, 0.22, 32, 1);
    glPopMatrix();

    // Borda metálica do mostrador
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.8f, 0.7f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 1.95f, 0.191f);
    glutSolidCylinder(0.225, 0.01, 32, 1);
    glPopMatrix();

    // --- 3. PÊNDULO ANIMADO (Posicionado à frente do corpo principal) ---
    // Reposicionado para Z = 0.21f para sobressair da madeira (Z = 0.16f)
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.21f);
    glRotatef(anguloPendulo, 0.0f, 0.0f, 1.0f);

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

    glPopMatrix(); // Fim do Pêndulo

    gluDeleteQuadric(quad);
    glPopMatrix(); // Fim do Relógio
}

// ============================================================================
// 3. LANTERNA DO VIGILANTE (Com Textura)
// ============================================================================
inline void desenharLanterna(float posX, float posY, float posZ, float rotY) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // Cria o objeto quadrico para gerar cilindros com coordenadas de textura UV
    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    // Garante que a textura renderize com suas cores reais
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // --- 1. BASE DE METAL ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texLanterna);

    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Alinha o cilindro verticalmente
    gluCylinder(quad, 0.1, 0.1, 0.05, 16, 1);

    // Tampo da base (disco inferior)
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0, 0.1, 16, 1);
    glPopMatrix();

    // --- 2. VIDRO / LÂMPADA (Sem textura de metal) ---
    glDisable(GL_TEXTURE_2D);

    // Opcional: define uma cor levemente translúcida/amarelada para o vidro
    glColor4f(0.9f, 0.9f, 0.7f, 0.8f);

    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.07, 0.3, 16, 1);
    glPopMatrix();

    // --- 3. HASTES DE PROTEÇÃO (Com textura de metal) ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texLanterna);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    float offsets[2] = { -0.07f, 0.07f };
    for (float x : offsets) {
        for (float z : offsets) {
            glPushMatrix();
            glTranslatef(x, 0.2f, z);
            glScalef(0.015f, 0.3f, 0.015f);
            glutSolidCube(1.0); // O cubo herdará a textura do metal
            glPopMatrix();
        }
    }

    // --- 4. TAMPO SUPERIOR ---
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.11, 0.11, 0.05, 16, 1);

    // Disco superior
    glTranslatef(0.0f, 0.0f, 0.05f);
    gluDisk(quad, 0.0, 0.11, 16, 1);
    glPopMatrix();

    // --- 5. ALÇA SUPERIOR ---
    glPushMatrix();
    glTranslatef(0.0f, 0.43f, 0.0f);
    glScalef(0.12f, 0.08f, 0.02f);
    glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // Libera a memória do objeto quadrico
    gluDeleteQuadric(quad);

    glPopMatrix();
}
#endif