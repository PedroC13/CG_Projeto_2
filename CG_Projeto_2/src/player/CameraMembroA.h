#pragma once
#include <GL/freeglut.h>
#include <cmath>
#include <cctype>

bool teclas[256] = { false }; 
float velocidadeCamera = 3.0f; // unidades por segundo
float sensibilidadeMouse = 0.1f; // Sensibilidade do mouse para rotação da câmera
bool primeiroMouse = true;

float cameraX = 0.0f, cameraY = 1.6f, cameraZ = 5.0f; // Posição inicial da câmera
float yaw = -90.0f; // Ângulo de rotação da câmera em torno do eixo Y
float pitch = 0.0; // Ângulo de rotação da câmera em torno do eixo X

inline float grausParaRad(float degrees) {
	return degrees * 3.14159265f / 180.0f; 
}

inline void cameraAplicar() {
	float y = grausParaRad(yaw);
	float p = grausParaRad(pitch);

	float frenteX = std::cos(y) * std::cos(p);
	float frenteY = std::sin(p);
	float frenteZ = std::sin(y) * std::cos(p);

	gluLookAt(cameraX, cameraY, cameraZ, //olho da câmera
		cameraX + frenteX, cameraY + frenteY, cameraZ + frenteZ, // ponto para onde a câmera está olhando
		0.0f, 1.0f, 0.0f); // vetor "up" da câmera
}

// chamar a camera e marcar a tecla como pressionada
inline void cameraTeclaDown(unsigned char key) {

	teclas[std::tolower(key)] = true;
}

//quando a tecla é liberada, marcar como não pressionada
inline void cameraTeclaUp(unsigned char key, int x, int y) {
	teclas[std::tolower(key)] = false;
}

inline void atualizarCamera(float deltaTime) {
	
	float y = grausParaRad(yaw);
	float frenteX = std::cos(y), frenteZ = std::sin(y);
	float direitaX = -frenteZ, direitaZ = frenteX; // vetor perpendicular à frente

	float passo = velocidadeCamera * deltaTime;

	if (teclas['w']) { cameraX += frenteX * passo;  cameraZ += frenteZ * passo; }
	if (teclas['s']) { cameraX -= frenteX * passo;  cameraZ -= frenteZ * passo; }
	if (teclas['d']) { cameraX += direitaX * passo; cameraZ += direitaZ * passo; }
	if (teclas['a']) { cameraX -= direitaX * passo; cameraZ -= direitaZ * passo; }
}

inline void cameraMouse(int x, int y) {
	int centroX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int centroY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	if (x == centroX && y == centroY) { //evento gerado pelo próprio warp, não queremos processar
		return;
	}

	if (primeiroMouse) {
		primeiroMouse = false;
		glutWarpPointer(centroX, centroY);
		return;
	}

	float dx = (float)(x - centroX);
	float dy = (float)(centroY - y); // Invertido porque a coordenada Y aumenta para baixo

	yaw += dx * sensibilidadeMouse;
	pitch += dy * sensibilidadeMouse;
	
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glutWarpPointer(centroX, centroY); // devolve o cursor para o centro da tela para capturar movimento contínuo
}
	
