#pragma once
#ifndef MODELOS_MEMBRO_B_H
#define MODELOS_MEMBRO_B_H

#include <GL/freeglut.h>
#include "ModelosMembroC.h"   // reaproveita o desenharCuboTexturizado() do Membro C

extern GLuint texPedraAltar;             // pedra.jpg
extern GLuint texMetalCastical;          // metal.jpg
extern GLuint texMadeiraConfessionario;  // madeira.jpg

// Desenha um cubo texturizado posicionado e escalado (uma "peça" do modelo)
inline void desenharPeca(float x, float y, float z, float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    desenharCuboTexturizado();
    glPopMatrix();
}

// ============================================================================
// 1. ALTAR PRINCIPAL DE PEDRA (Base + Corpo + Tampo + Cruz em relevo)
// Frente voltada para +Z. O tampo fica em y = 1.0.
// ============================================================================
inline void desenharAltar(float posX, float posY, float posZ, float rotY) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texPedraAltar);

    // Base
    glColor4f(0.70f, 0.68f, 0.64f, 1.0f);
    desenharPeca(0.0f, 0.075f, 0.0f, 2.4f, 0.15f, 1.1f);

    // Corpo
    glColor4f(0.80f, 0.78f, 0.74f, 1.0f);
    desenharPeca(0.0f, 0.525f, 0.0f, 2.2f, 0.75f, 0.9f);

    // Tampo (pedra mais escura)
    glColor4f(0.55f, 0.53f, 0.50f, 1.0f);
    desenharPeca(0.0f, 0.95f, 0.0f, 2.5f, 0.10f, 1.1f);

    // Cruz em relevo na frente
    glColor4f(0.40f, 0.38f, 0.36f, 1.0f);
    desenharPeca(0.0f, 0.52f, 0.47f, 0.08f, 0.50f, 0.04f);  // haste
    desenharPeca(0.0f, 0.62f, 0.47f, 0.32f, 0.08f, 0.04f);  // braços

    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

// ============================================================================
// 2. CASTIÇAL DE VELA (Pé + Haste + Prato + Vela + Chama)
// A chama fica a 0.58 acima da base.
// ============================================================================
inline void desenharCastical(float posX, float posY, float posZ, bool acesa) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // quádricas da GLU crescem em Z: vira para cima

    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    // --- 1. METAL (pé, haste e prato) ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMetalCastical);
    glColor4f(0.80f, 0.65f, 0.40f, 1.0f);  // tom de bronze

    // Pé
    gluCylinder(quad, 0.12, 0.10, 0.04, 24, 1);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.04f);
    gluDisk(quad, 0.0, 0.10, 24, 1);
    glPopMatrix();

    // Haste
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.04f);
    gluCylinder(quad, 0.025, 0.025, 0.33, 16, 1);
    glPopMatrix();

    // Prato
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.37f);
    gluCylinder(quad, 0.07, 0.07, 0.02, 24, 1);
    glTranslatef(0.0f, 0.0f, 0.02f);
    gluDisk(quad, 0.0, 0.07, 24, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // --- 2. VELA DE CERA ---
    glColor4f(0.92f, 0.88f, 0.75f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.39f);
    gluCylinder(quad, 0.035, 0.035, 0.15, 16, 1);
    glTranslatef(0.0f, 0.0f, 0.15f);
    gluDisk(quad, 0.0, 0.035, 16, 1);
    glPopMatrix();

    // --- 3. CHAMA (só quando acesa) ---
    if (acesa) {
        glColor4f(1.0f, 0.6f, 0.15f, 1.0f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.58f);
        glScalef(0.025f, 0.025f, 0.05f);  // gota alongada
        glutSolidSphere(1.0, 12, 10);
        glPopMatrix();
    }

    gluDeleteQuadric(quad);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

// ============================================================================
// 3. CONFESSIONÁRIO (Cabine com 3 compartimentos + Grade + Portas + Cortina)
// Frente voltada para +Z. [ penitente | padre | penitente ]
// anguloPortas: 0 = fechadas, 90 = abertas (padrão: entreabertas)
// ============================================================================
inline void desenharConfessionario(float posX, float posY, float posZ, float rotY, float anguloPortas = 60.0f) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // --- 1. ESTRUTURA DE MADEIRA ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMadeiraConfessionario);
    glColor4f(0.60f, 0.45f, 0.35f, 1.0f);

    desenharPeca(0.0f, 0.05f, 0.0f, 2.6f, 0.1f, 1.0f);     // base
    desenharPeca(0.0f, 1.1f, -0.42f, 2.5f, 2.0f, 0.06f);   // fundo
    desenharPeca(-1.22f, 1.1f, 0.0f, 0.06f, 2.0f, 0.9f);   // lateral esquerda
    desenharPeca( 1.22f, 1.1f, 0.0f, 0.06f, 2.0f, 0.9f);   // lateral direita
    desenharPeca(0.0f, 2.14f, 0.0f, 2.6f, 0.08f, 1.0f);    // teto

    // --- 2. DIVISÓRIAS COM GRADE (entre o padre e cada penitente) ---
    for (int lado = -1; lado <= 1; lado += 2) {
        float x = lado * 0.5f;
        desenharPeca(x, 0.625f, 0.0f, 0.06f, 1.05f, 0.9f);  // parte de baixo
        desenharPeca(x, 1.825f, 0.0f, 0.06f, 0.55f, 0.9f);  // parte de cima

        // Grade no vão entre as duas partes (y de 1.15 a 1.55)
        for (int i = 0; i < 5; i++) {
            desenharPeca(x, 1.35f, -0.36f + i * 0.18f, 0.03f, 0.4f, 0.03f);  // verticais
        }
        desenharPeca(x, 1.28f, 0.0f, 0.03f, 0.03f, 0.9f);   // horizontais
        desenharPeca(x, 1.42f, 0.0f, 0.03f, 0.03f, 0.9f);
    }

    // --- 3. PORTAS DOS PENITENTES (dobradiça na lateral, abrem para fora) ---
    for (int lado = -1; lado <= 1; lado += 2) {
        glPushMatrix();
        glTranslatef(lado * 1.22f, 0.0f, 0.45f);           // dobradiça na quina da lateral
        glRotatef(lado * anguloPortas, 0.0f, 1.0f, 0.0f);   // gira para fora

        glColor4f(0.60f, 0.45f, 0.35f, 1.0f);
        desenharPeca(-lado * 0.33f, 1.05f, 0.02f, 0.64f, 1.9f, 0.04f);   // folha da porta

        glColor4f(0.30f, 0.22f, 0.16f, 1.0f);
        desenharPeca(-lado * 0.58f, 1.05f, 0.06f, 0.04f, 0.12f, 0.04f);  // puxador
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);

    // --- 4. CORTINA DO PADRE (tecido vinho) ---
    glColor4f(0.35f, 0.05f, 0.07f, 1.0f);
    desenharPeca(0.0f, 1.1f, 0.44f, 0.94f, 2.0f, 0.02f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

#endif
