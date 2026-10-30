#pragma once
#include <GL/freeglut.h>
#include <cmath>
#include <cctype>

bool teclas[256] = { false };
float velocidadeCamera = 3.0f;
float sensibilidadeMouse = 0.1f;
bool primeiroMouse = true;

// Posição inicial dentro dos limites da igreja
float cameraX = 0.0f, cameraY = 1.6f, cameraZ = 4.0f;
float yaw = -90.0f;
float pitch = 0.0f;

inline float grausParaRad(float degrees) {
    return degrees * 3.14159265f / 180.0f; // Multiplicação corrigida
}

inline void cameraAplicar() {
    float y = grausParaRad(yaw);
    float p = grausParaRad(pitch);

    float frenteX = std::cos(y) * std::cos(p);
    float frenteY = std::sin(p);
    float frenteZ = std::sin(y) * std::cos(p);

    gluLookAt(cameraX, cameraY, cameraZ,
        cameraX + frenteX, cameraY + frenteY, cameraZ + frenteZ,
        0.0f, 1.0f, 0.0f);
}

inline void cameraTeclaDown(unsigned char key) {
    teclas[std::tolower(key)] = true;
}

inline void cameraTeclaUp(unsigned char key, int x, int y) {
    teclas[std::tolower(key)] = false;
}

inline void atualizarCamera(float deltaTime) {
    float y = grausParaRad(yaw);
    float frenteX = std::cos(y), frenteZ = std::sin(y);
    float direitaX = -frenteZ, direitaZ = frenteX;

    float passo = velocidadeCamera * deltaTime;

    if (teclas['w']) { cameraX += frenteX * passo;  cameraZ += frenteZ * passo; }
    if (teclas['s']) { cameraX -= frenteX * passo;  cameraZ -= frenteZ * passo; }
    if (teclas['d']) { cameraX += direitaX * passo; cameraZ += direitaZ * passo; }
    if (teclas['a']) { cameraX -= direitaX * passo; cameraZ -= direitaZ * passo; }
}

inline void cameraMouse(int x, int y) {
    int centroX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centroY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    if (x == centroX && y == centroY) {
        return;
    }

    if (primeiroMouse) {
        primeiroMouse = false;
        glutWarpPointer(centroX, centroY);
        return;
    }

    float dx = (float)(x - centroX); // Subtração corrigida
    float dy = (float)(centroY - y);

    yaw += dx * sensibilidadeMouse;
    pitch += dy * sensibilidadeMouse;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glutWarpPointer(centroX, centroY);
}