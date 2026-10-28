#include <GL/freeglut.h>
#include <cmath>
#include "render/ModelosMembroC.h"
#include "scene/LogicMembroC.h"
#include "../Textura.h"

GerenciadorCena gerenciador;
// Defina as variáveis globais ou membros da classe para armazenar os IDs das texturas
GLuint texMostradorRelogio;
GLuint texMadeiraRelogio;
GLuint texLanterna;
GLuint texVitral;
GLuint texMolduraVitral;

void setup() {

    // ... suas configurações atuais de Iluminação e Depth Test ...
    glEnable(GL_DEPTH_TEST);

    // Carrega as imagens
    texMadeiraRelogio = carregarTextura("assets/textures/madeira.jpg");
    texMostradorRelogio = carregarTextura("assets/textures/faceRelogio.png");
    texLanterna = carregarTextura("assets/textures/metal.jpg");
    texVitral = carregarTextura("assets/textures/vitral.jpg");
	texMolduraVitral = carregarTextura("assets/textures/pedra.jpg");
    // ... configurações do OpenGL ...
    gerenciador.inicializarCena();
}

void timer(int value) {
    float deltaTime = 0.016f; // ~60 FPS

    // Atualiza rotações das portas
    gerenciador.atualizar(deltaTime);

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

// Callback de Teclado(Pressione W para alternar Wireframe)
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        modoWireframe = !modoWireframe; // Inverte o estado
        glutPostRedisplay();
    }
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Posicionamento da Câmera
    gluLookAt(0.0, 2.0, 5.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0);

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
    glutKeyboardFunc(keyboard); // Registra a tecla do teclado
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}