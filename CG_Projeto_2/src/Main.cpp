#include "render/ShaderMembroA.h"
#include <GL/freeglut.h>
#include <cmath>
#include "render/ModelosMembroC.h"
#include "scene/LogicMembroC.h"
#include "../Textura.h"
#include "render/ModelosMembroA.h"
#include "render/ModelosMembroB.h"
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
GLuint texPedraAltar = 0; // Membro B - altar
GLuint texMetalCastical = 0; // Membro B - castiçal
GLuint texMadeiraConfessionario = 0; // Membro B - confessionário
GLuint texMetalPorta = 0;
GLuint texBranca = 0;
GLuint texParede = 0;
GLuint texTeto = 0;

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
	texPedraAltar = carregarTextura("assets/textures/pedra.jpg"); // Membro B
    texMetalPorta = carregarTextura("assets/textures/metal.jpg");
    texParede = carregarTextura("assets/textures/pedra.jpg"); // ou outra textura de parede
    texTeto = carregarTextura("assets/textures/madeira.jpg"); // ou outra textura de teto
    texBranca = criarTexturaBranca();
	texMetalCastical = carregarTextura("assets/textures/metal.jpg"); // Membro B
	texMadeiraConfessionario = carregarTextura("assets/textures/madeira.jpg"); // Membro B
    // ... configurações do OpenGL ...
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

    // Posicionamento da Câmera
	cameraAplicar();
	glUseProgram(shaderPrincipal); // Ativa o shader principal
    glUniform1i(glGetUniformLocation(shaderPrincipal, "textura"), 0);  // sampler lê a unidade 0
	desenharChao(texChao); // Desenha o chão com a textura carregada
    desenharParedes(texParede);
    desenharTeto(texTeto);
    desenharFileirasBancos(texMadeiraBanco); // desenha a fileira de bancos
    Porta& entrada = gerenciador.portas[0];   // porta da entrada principal (lógica do C)
    desenharPortasDuplas(entrada.posX, entrada.posY, entrada.posZ, entrada.rotInicialY,
        entrada.anguloAtual, texMadeiraBanco, texMetalPorta);
    float tempoSeg = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;            // segundos desde o início
    desenharLustre(0.0f, 6.0f, -4.0f, tempoSeg, texMetalPorta, texBranca);  // preso a 6 m de altura
    glUseProgram(0);   // volta ao pipeline fixo para os objetos do C

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

    // --- MEMBRO B ---
    desenharAltar(0.0f, 0.0f, -4.0f, 0.0f); // posição de teste; frente virada para a câmera
    desenharCastical(-0.9f, 1.0f, -4.0f, true);  // castiçais em cima do tampo do altar (y = 1.0)
    desenharCastical( 0.9f, 1.0f, -4.0f, true);
    desenharConfessionario(-5.0f, 0.0f, -2.0f, 90.0f); // posição de teste; frente virada para o centro

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