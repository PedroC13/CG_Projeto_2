#pragma once
#ifndef MODELOS_MEMBRO_B_H
#define MODELOS_MEMBRO_B_H

#include <GL/freeglut.h>
#include <cmath>
#include <initializer_list>

// Texturas (carregadas no setup() do Main.cpp)
extern GLuint texPedraAltar;             // pedra.jpg
extern GLuint texMetalCastical;          // metal.jpg
extern GLuint texMadeiraConfessionario;  // madeira.jpg

// ============================================================================
// AUXILIARES DE MATERIAL
// ============================================================================

// Define cor (tinge a textura) + material Blinn-Phong (ambiente, difusa,
// especular, brilho e emissão). A emissão é zerada por padrão, então um
// objeto emissivo (chama) não "vaza" brilho para o próximo.
inline void aplicarMaterial(float r, float g, float b, float especular, float brilho,
                            float er = 0.0f, float eg = 0.0f, float eb = 0.0f) {
    GLfloat ambiente[] = { r * 0.25f, g * 0.25f, b * 0.25f, 1.0f };
    GLfloat difusa[]   = { r, g, b, 1.0f };
    GLfloat spec[]     = { especular, especular, especular, 1.0f };
    GLfloat emissao[]  = { er, eg, eb, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissao);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brilho);
    glColor4f(r, g, b, 1.0f);
}

// ============================================================================
// BLOCO COM UV PROPORCIONAL AO TAMANHO
// ----------------------------------------------------------------------------
// Diferente de glScalef + cubo unitário, aqui os vértices já saem com as
// dimensões reais (larg x alt x prof). Vantagens:
//   - a textura repete na mesma densidade em todos os blocos (não estica);
//   - as normais continuam unitárias (glScalef deformaria a iluminação).
// ============================================================================
inline void desenharBloco(float larg, float alt, float prof, float densidadeUV = 1.5f) {
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

// Atalho: posiciona (e opcionalmente gira em Z) um bloco
inline void blocoEm(float px, float py, float pz, float larg, float alt, float prof, float rotZ = 0.0f) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    if (rotZ != 0.0f) glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
    desenharBloco(larg, alt, prof);
    glPopMatrix();
}

// ============================================================================
// AUXILIARES DE QUÁDRICAS (GLU)
// ----------------------------------------------------------------------------
// Os cilindros da GLU crescem em +Z; aqui eles são girados para crescer em +Y.
// ============================================================================

// Cilindro (ou tronco de cone) vertical com tampas, base em y = baseY
inline void cilindroY(GLUquadric* q, float baseY, float raioBase, float raioTopo,
                      float altura, int fatias = 24) {
    glPushMatrix();
    glTranslatef(0.0f, baseY, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(q, raioBase, raioTopo, altura, fatias, 1);

    // tampa de baixo (virada para -Z local = para baixo)
    glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(q, 0.0, raioBase, fatias, 1);
    glPopMatrix();

    // tampa de cima
    glTranslatef(0.0f, 0.0f, altura);
    gluDisk(q, 0.0, raioTopo, fatias, 1);
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
    aplicarMaterial(0.70f, 0.68f, 0.64f, 0.05f, 6.0f);
    blocoEm(0.0f, 0.075f, 0.0f, 2.4f, 0.15f, 1.1f);     // y: 0.00 a 0.15

    // --- CORPO ---
    aplicarMaterial(0.80f, 0.78f, 0.74f, 0.08f, 8.0f);
    blocoEm(0.0f, 0.525f, 0.0f, 2.2f, 0.75f, 0.9f);     // y: 0.15 a 0.90

    // --- TAMPO (pedra escura e polida) ---
    aplicarMaterial(0.55f, 0.53f, 0.50f, 0.30f, 32.0f);
    blocoEm(0.0f, 0.95f, 0.0f, 2.5f, 0.10f, 1.1f);      // y: 0.90 a 1.00

    // --- CRUZ EM RELEVO (sai 4 cm da frente do corpo) ---
    aplicarMaterial(0.40f, 0.38f, 0.36f, 0.10f, 10.0f);
    blocoEm(0.0f, 0.52f, 0.47f, 0.08f, 0.50f, 0.04f);   // haste
    blocoEm(0.0f, 0.62f, 0.47f, 0.32f, 0.08f, 0.04f);   // braços

    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // não vazar a cor para os próximos objetos
    glPopMatrix();
}

// ============================================================================
// 2. CASTIÇAL DE VELA
// ----------------------------------------------------------------------------
// Origem = centro do pé, apoiado na superfície (chão ou tampo do altar).
// Pé circular -> haste cilíndrica (com nó) -> prato -> vela de cera -> chama.
//
// ALTURA_CHAMA_CASTICAL: use para posicionar a point light da vela:
//   luz em (posX, posY + ALTURA_CHAMA_CASTICAL, posZ)
// ============================================================================
const float ALTURA_CHAMA_CASTICAL = 0.58f;

inline void desenharCastical(float posX, float posY, float posZ, bool acesa) {
    GLUquadric* q = gluNewQuadric();
    gluQuadricNormals(q, GLU_SMOOTH);
    gluQuadricTexture(q, GL_TRUE);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);

    // --- METAL (pé, haste, nó, prato): bronze envelhecido e refletivo ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMetalCastical);
    aplicarMaterial(0.72f, 0.56f, 0.32f, 0.80f, 96.0f);

    cilindroY(q, 0.00f, 0.12f,  0.11f,  0.03f);   // pé (disco largo)
    cilindroY(q, 0.03f, 0.10f,  0.025f, 0.06f);   // transição cônica do pé
    cilindroY(q, 0.09f, 0.025f, 0.025f, 0.28f);   // haste

    glPushMatrix();                                // nó decorativo no meio da haste
    glTranslatef(0.0f, 0.21f, 0.0f);
    gluSphere(q, 0.04f, 16, 12);
    glPopMatrix();

    cilindroY(q, 0.37f, 0.025f, 0.07f,  0.015f);  // base do prato (abre em cone)
    cilindroY(q, 0.385f, 0.07f, 0.07f,  0.01f);   // prato

    glDisable(GL_TEXTURE_2D);

    // --- VELA DE CERA (sem textura, fosca) ---
    aplicarMaterial(0.90f, 0.86f, 0.74f, 0.10f, 12.0f);
    cilindroY(q, 0.395f, 0.035f, 0.035f, 0.15f);  // vela: y 0.395 a 0.545

    // pavio
    aplicarMaterial(0.08f, 0.07f, 0.06f, 0.0f, 1.0f);
    cilindroY(q, 0.545f, 0.004f, 0.004f, 0.02f, 6);

    // --- CHAMA (emissiva, só quando acesa) ---
    if (acesa) {
        // leve tremulação usando o tempo
        float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        float tremor = 1.0f + 0.08f * sinf(t * 13.0f) + 0.05f * sinf(t * 7.3f + 1.7f);

        aplicarMaterial(1.0f, 0.62f, 0.18f, 0.0f, 1.0f,
                        1.0f, 0.55f, 0.15f);       // emissão laranja
        glPushMatrix();
        glTranslatef(0.0f, ALTURA_CHAMA_CASTICAL, 0.0f);
        glScalef(0.022f, 0.045f * tremor, 0.022f); // gota alongada
        gluSphere(q, 1.0, 12, 10);
        glPopMatrix();

        // miolo mais claro
        aplicarMaterial(1.0f, 0.95f, 0.70f, 0.0f, 1.0f,
                        1.0f, 0.90f, 0.60f);
        glPushMatrix();
        glTranslatef(0.0f, ALTURA_CHAMA_CASTICAL - 0.01f, 0.0f);
        glScalef(0.011f, 0.022f * tremor, 0.011f);
        gluSphere(q, 1.0, 10, 8);
        glPopMatrix();

        aplicarMaterial(1.0f, 1.0f, 1.0f, 0.0f, 1.0f);  // zera a emissão
    }

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
    gluDeleteQuadric(q);
}

// ============================================================================
// 3. CONFESSIONÁRIO
// ----------------------------------------------------------------------------
// Frente voltada para +Z. Origem = centro da base no chão.
// Dimensões: 2.6 (largura) x 2.45 (altura) x 1.15 (profundidade).
//
// 3 compartimentos:
//   [ penitente ] [ padre ] [ penitente ]
//      0.67 m      1.0 m      0.67 m
// O padre fica atrás de uma meia-porta + cortina. Cada divisória tem uma
// janelinha com GRADE de madeira, na altura do penitente ajoelhado.
// ============================================================================

// Grade de madeira na janela de uma divisória (plano YZ, em x = px)
inline void gradeConfessionario(float px, float yMin, float yMax, float zMin, float zMax) {
    const float barra = 0.02f;
    const float esp   = 0.03f;              // espessura da divisória na grade
    const int   nVert = 5, nHoriz = 4;

    float alt  = yMax - yMin, larg = zMax - zMin;
    float yC   = (yMin + yMax) / 2.0f, zC = (zMin + zMax) / 2.0f;

    // moldura da janela
    blocoEm(px, yMax + 0.02f, zC, 0.06f, 0.04f, larg + 0.08f);
    blocoEm(px, yMin - 0.02f, zC, 0.06f, 0.04f, larg + 0.08f);

    // barras verticais
    for (int i = 1; i <= nVert; i++) {
        float z = zMin + larg * i / (nVert + 1);
        blocoEm(px, yC, z, esp, alt, barra);
    }
    // barras horizontais
    for (int j = 1; j <= nHoriz; j++) {
        float y = yMin + alt * j / (nHoriz + 1);
        blocoEm(px, y, zC, esp, barra, larg);
    }
}

inline void desenharConfessionario(float posX, float posY, float posZ, float rotY) {
    // Medidas principais
    const float esp     = 0.06f;            // espessura das paredes
    const float yPiso   = 0.10f;            // topo do plinto
    const float altPar  = 2.00f;            // altura das paredes
    const float yParede = yPiso + altPar / 2.0f;
    const float xLat    = 1.17f;            // paredes laterais
    const float xDiv    = 0.50f;            // divisórias (padre no meio)
    const float prof    = 1.00f;

    // Janela da grade (na divisória)
    const float jYMin = 1.15f, jYMax = 1.55f;
    const float jZMin = -0.10f, jZMax = 0.25f;

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMadeiraConfessionario);

    // --- MADEIRA ESCURA FOSCA (estrutura) ---
    aplicarMaterial(0.55f, 0.42f, 0.32f, 0.05f, 8.0f);

    blocoEm(0.0f, yPiso / 2.0f, 0.0f, 2.5f, yPiso, 1.1f);                 // plinto
    blocoEm(0.0f, yParede, -prof / 2.0f + esp / 2.0f, 2.4f, altPar, esp);   // fundo
    for (float s : { -1.0f, 1.0f }) {
        blocoEm(s * xLat, yParede, 0.0f, esp, altPar, prof);              // laterais
    }

    // Divisórias com abertura para a grade
    for (float s : { -1.0f, 1.0f }) {
        float x = s * xDiv;
        // parte de baixo e parte de cima (inteiras)
        blocoEm(x, (yPiso + jYMin) / 2.0f, 0.0f, esp, jYMin - yPiso, prof);
        blocoEm(x, (jYMax + yPiso + altPar) / 2.0f, 0.0f, esp, yPiso + altPar - jYMax, prof);
        // faixa do meio: pedaço de trás e pedaço da frente da janela
        float zTras = -prof / 2.0f, zFrente = prof / 2.0f;
        blocoEm(x, (jYMin + jYMax) / 2.0f, (zTras + jZMin) / 2.0f, esp, jYMax - jYMin, jZMin - zTras);
        blocoEm(x, (jYMin + jYMax) / 2.0f, (jZMax + zFrente) / 2.0f, esp, jYMax - jYMin, zFrente - jZMax);

        gradeConfessionario(x, jYMin, jYMax, jZMin, jZMax);
    }

    // Teto + cornija + frontão central
    blocoEm(0.0f, yPiso + altPar + 0.03f, 0.0f, 2.4f, 0.06f, prof);
    blocoEm(0.0f, yPiso + altPar + 0.11f, 0.02f, 2.6f, 0.10f, 1.15f);
    blocoEm(0.0f, yPiso + altPar + 0.285f, 0.50f, 1.1f, 0.25f, 0.08f);

    // Vergas sobre as entradas dos penitentes
    for (float s : { -1.0f, 1.0f }) {
        blocoEm(s * (xDiv + xLat) / 2.0f, yPiso + altPar - 0.075f, prof / 2.0f - 0.03f,
                xLat - xDiv, 0.15f, esp);
    }

    // --- MADEIRA UM TOM MAIS CLARA (móveis internos) ---
    aplicarMaterial(0.65f, 0.50f, 0.38f, 0.08f, 10.0f);

    // Meia-porta do padre
    blocoEm(0.0f, yPiso + 0.5f, prof / 2.0f - 0.02f, 2.0f * xDiv - esp, 1.0f, 0.04f);
    // Banco do padre
    blocoEm(0.0f, 0.50f, -0.25f, 0.85f, 0.06f, 0.40f);

    // Genuflexórios (de frente para a grade) + apoio de braço sob a janela
    for (float s : { -1.0f, 1.0f }) {
        blocoEm(s * 0.68f, yPiso + 0.06f, 0.07f, 0.22f, 0.12f, 0.50f);
        blocoEm(s * (xDiv + 0.09f), jYMin - 0.06f, 0.07f, 0.12f, 0.04f, 0.45f);
    }

    glDisable(GL_TEXTURE_2D);

    // --- CORTINA DO PADRE (tecido vinho, sem textura) ---
    aplicarMaterial(0.35f, 0.05f, 0.07f, 0.02f, 4.0f);
    blocoEm(0.0f, yPiso + 1.0f + 0.475f, prof / 2.0f - 0.03f, 2.0f * xDiv - esp, 0.95f, 0.02f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

#endif
