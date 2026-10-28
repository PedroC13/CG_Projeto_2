#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include "../ModelosMembroC.h" // Seu arquivo com os 3 modelos
#include "../LogicMembroC.h"
#include "../CameraMembroA.h"


GerenciadorCena gerenciador;

int tempo_anterior = 0;

void setup() {
    // ... configurações do OpenGL ...
	tempo_anterior = glutGet(GLUT_ELAPSED_TIME);
    gerenciador.inicializarCena();
	glutSetCursor(GLUT_CURSOR_NONE); // Oculta o cursor do mouse)
}



void timer(int value) {

    int tempo_atual = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (tempo_atual - tempo_anterior) / 1000.0f;
    tempo_anterior = tempo_atual;

	// std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;

    // Atualiza rotações das portas
    gerenciador.atualizar(deltaTime);
	atualizarCamera(deltaTime); // Atualiza a posição da câmera com base nas teclas pressionadas
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);

    // CONFIGURAÇÃO DA PERSPECTIVA 3D (Câmera)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0); // Campo de visão de 60 graus

    glMatrixMode(GL_MODELVIEW);
}

bool modoWireframe = false; // Alterna entre Wireframe e Preenchido

// Callback de Teclado(Pressione T para alternar Wireframe)
void keyboard(unsigned char key, int x, int y) {
    
	cameraTeclaDown(key); // Marca a tecla como pressionada para movimentação da câmera

    if (key == 't' || key == 'T') {
        modoWireframe = !modoWireframe; // Inverte o estado
        glutPostRedisplay();
    }

	if (key == 27) { // ESC para sair
		exit(0);
	}

}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Posicionamento da Câmera
	cameraAplicar();

    // ALTERNA ENTRE WIREFRAME E MODO SÓLIDO
    if (modoWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Mostra só as linhas/wireframe
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Mostra as superfícies preenchidas
    }

    // --- RENDERIZAÇÃO DOS 3 OBJETOS ---
    desenharVitral(-2.5f, 0.0f, 0.0f, 0.0f);

    static float tempo = 0.0f;
    tempo += 0.016f;
    float anguloPendulo = std::sin(tempo * 3.0f) * 15.0f;
    desenharRelogioPendulo(0.0f, 0.0f, 0.0f, 0.0f, anguloPendulo);

    desenharLanterna(2.0f, 0.0f, 0.0f, 0.0f);

    glutSwapBuffers();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Igreja Assombrada - Visualizacao (Pressione W para Wireframe)");

    setup();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutKeyboardUpFunc(cameraTeclaUp); // Registra a liberação da tecla)
	glutIgnoreKeyRepeat(1); // Ignora repetição de tecla
	glutPassiveMotionFunc(cameraMouse); // Registra o movimento do mouse
	glutMotionFunc(cameraMouse); // Registra o movimento do mouse com botão pressionado)
    glutKeyboardFunc(keyboard); // Registra a tecla do teclado
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}