#include "render/ShaderMembroA.h"
#include <GL/freeglut.h>
#include <cmath>
#include "render/ModelosMembroC.h"
#include "scene/LogicMembroC.h"
#include "../Textura.h"
#include "render/ModelosMembroA.h"
#include <iostream>
#include <cstdlib>
#include "player/CameraMembroA.h"

GerenciadorCena gerenciador;

GLuint texMostradorRelogio;
GLuint texMadeiraRelogio;
GLuint texLanterna;
GLuint texVitral;
GLuint texMolduraVitral;
GLuint shaderPrincipal = 0;
GLuint texChao = 0;
GLuint texMadeiraBanco = 0;

int tempo_anterior = 0;

void setup() {
    glEnable(GL_DEPTH_TEST);

    glewInit();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    shaderPrincipal = criarProgramaShader();

    texMadeiraRelogio = carregarTextura("assets/textures/madeira.jpg");
    texMostradorRelogio = carregarTextura("assets/textures/faceRelogio.png");
    texLanterna = carregarTextura("assets/textures/metal.jpg");
    texVitral = carregarTextura("assets/textures/vitral.jpg");
    texMolduraVitral = carregarTextura("assets/textures/pedra.jpg");
    texChao = carregarTextura("assets/textures/chao.jpg");
    texMadeiraBanco = carregarTextura("assets/textures/madeira.jpg");

    tempo_anterior = glutGet(GLUT_ELAPSED_TIME);
    gerenciador.inicializarCena();
    glutSetCursor(GLUT_CURSOR_NONE);
}

void timer(int value) {
    int tempo_atual = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (tempo_atual - tempo_anterior) / 1000.0f;
    tempo_anterior = tempo_atual;

    gerenciador.atualizar(deltaTime);

    float posAntigaX = cameraX;
    float posAntigaZ = cameraZ;

    atualizarCamera(deltaTime);

    float posNovaX = cameraX;
    float posNovaZ = cameraZ;

    // Raio de 0.2f para dar mais espaço livre de navegação
    gerenciador.colisao.processarMovimento(posAntigaX, posAntigaZ, posNovaX, posNovaZ, 0.2f);

    cameraX = posNovaX;
    cameraZ = posNovaZ;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

bool modoWireframe = false;

void keyboard(unsigned char key, int x, int y) {
    cameraTeclaDown(key);

    if (key == 't' || key == 'T') {
        modoWireframe = !modoWireframe;
        glutPostRedisplay();
    }

    // Alternar Lanterna (Ligar / Desligar ou Mostrar / Esconder)
    if (key == 'f' || key == 'F') {
        gerenciador.lanternaLigada = !gerenciador.lanternaLigada;
        glutPostRedisplay();
    }

    if (key == 27) { // ESC para sair
        exit(0);
    }
}

// RENDERIZA A LANTERNA FIXA NA MÃO DO JOGADOR (HUD 3D)
void desenharLanternaNaMao() {
    if (!gerenciador.lanternaLigada) return;

    glPushMatrix();
    glLoadIdentity(); // Entra no espaço da câmera

    // Posiciona no canto inferior direito da tela
    glTranslatef(0.28f, -0.32f, -0.55f);
    glRotatef(12.0f, 0.0f, 1.0f, 0.0f);   // Inclina suavemente para o centro
    glScalef(0.35f, 0.35f, 0.35f);         // Reduz escala para proporção de mão

    // Desabilita teste de profundidade para a lanterna não ser cortada por paredes próximas
    glDisable(GL_DEPTH_TEST);
    desenharLanterna(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 1. Aplica Câmera do Jogador
    cameraAplicar();

    // 2. Renderiza Chão e Bancos
    glUseProgram(shaderPrincipal);
    glUniform1i(glGetUniformLocation(shaderPrincipal, "textura"), 0);
    desenharChao(texChao);
    desenharFileirasBancos(texMadeiraBanco);
    glUseProgram(0);

    if (modoWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // 3. Renderiza Objetos do Mundo (Vitrais e Relógio)
    desenharVitral(-4.8f, 1.5f, -2.0f, 90.0f);
    desenharVitral(4.8f, 1.5f, -6.0f, -90.0f);

    static float tempo = 0.0f;
    tempo += 0.016f;
    float anguloPendulo = std::sin(tempo * 3.0f) * 15.0f;
    desenharRelogioPendulo(3.8f, 0.0f, -2.0f, -90.0f, anguloPendulo);

    // 4. Renderiza a Lanterna na Mão do Vigia (Fixa na câmera)
    desenharLanternaNaMao();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Igreja Assombrada - Visualizacao (Pressione T para Wireframe, F para Lanterna)");

    setup();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(cameraTeclaUp);
    glutIgnoreKeyRepeat(1);
    glutPassiveMotionFunc(cameraMouse);
    glutMotionFunc(cameraMouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}