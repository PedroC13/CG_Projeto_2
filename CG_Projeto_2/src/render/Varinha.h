#pragma once
#ifndef VARINHA_H
#define VARINHA_H

#include <GL/freeglut.h>

extern GLuint texBranca;

// ============================================================================
// VARINHA APAGA-VELAS (o "apagador de velas" de igreja)
// ----------------------------------------------------------------------------
// Modelo hierárquico simples, igual ao castiçal: cabo + haste + sininho.
// A origem local é a MÃO do vigia; a haste cresce no +Z local.
// golpe: 0 = parada, 1 = ponta toda abaixada (animação ao apagar uma vela).
// ============================================================================
inline void desenharVarinha(float golpe) {
    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, texBranca);   // peças só com cor (shader usa textura branca)

    glPushMatrix();
    // Aponta a haste para a frente, um pouco para cima e para o centro da tela.
    // O golpe gira mais em X: a ponta desce, como quem cobre a chama.
    glRotatef(-12.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(-160.0f - 30.0f * golpe, 1.0f, 0.0f, 0.0f);

    // 1. Cabo de madeira escura (12 cm)
    glColor4f(0.35f, 0.22f, 0.14f, 1.0f);
    gluCylinder(quad, 0.012, 0.012, 0.12, 12, 1);

    // 2. Haste fina de latão (35 cm), começando onde o cabo termina
    glColor4f(0.80f, 0.65f, 0.40f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.12f);
    gluCylinder(quad, 0.005, 0.004, 0.35, 8, 1);
    glPopMatrix();

    // 3. Sininho na ponta: cone com a boca virada para BAIXO.
    //    Depois das rotações acima, o +Y local aponta para baixo na tela.
    glPushMatrix();
    glTranslatef(0.0f, 0.03f, 0.47f);          // pendurado logo abaixo da ponta da haste
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);        // eixo do cone (+Z) passa a apontar para -Y (para cima)
    glutSolidCone(0.025, 0.04, 12, 1);
    glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

#endif
