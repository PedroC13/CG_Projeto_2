#include "render/ShaderMembroA.h"
#include <GL/freeglut.h>
#include <cmath>
#include "render/ModelosMembroC.h"
#include "scene/LogicMembroC.h"
#include "scene/LogicMembroB.h"
#include "render/ShaderMembroB.h"
#include "../Textura.h"
#include "render/ModelosMembroA.h"
#include "render/ModelosMembroB.h"
#include "scene/GameplayMembroB.h"
#include <iostream>
#include <cstdlib>
#include "player/CameraMembroA.h"

GerenciadorCena gerenciador;
LogicaMembroB logicaB;
Gameplay gameplay;      // Membro B: fases do jogo (velas -> esconder -> final)

GLuint texMostradorRelogio;
GLuint texMadeiraRelogio;
GLuint texLanterna;
GLuint texVitral;
GLuint texMolduraVitral;
GLuint shaderPrincipal = 0;
GLuint shaderB = 0; // Membro B - Blinn-Phong da cena inteira
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
    shaderB = criarProgramaShaderB(); // Membro B
    configurarLuzesB();               // Membro B: cores e alcance das luzes

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
    logicaB.inicializar();
    gameplay.iniciar(gerenciador, logicaB, cameraX, cameraZ, yaw); // velas acesas, jogador no corredor
    glutSetCursor(GLUT_CURSOR_NONE);
}

void timer(int value) {
    int tempo_atual = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (tempo_atual - tempo_anterior) / 1000.0f;
    tempo_anterior = tempo_atual;

    gerenciador.atualizar(deltaTime);
    logicaB.atualizar(deltaTime);

    float posAntigaX = cameraX;
    float posAntigaZ = cameraZ;

    atualizarCamera(deltaTime);

    float posNovaX = cameraX;
    float posNovaZ = cameraZ;

    // Raio de 0.2f para dar mais espaço livre de navegação
    gerenciador.colisao.processarMovimento(posAntigaX, posAntigaZ, posNovaX, posNovaZ, 0.2f);
    logicaB.colisaoPortas(posAntigaX, posAntigaZ, posNovaX, posNovaZ);   // portas do confessionário

    cameraX = posNovaX;
    cameraZ = posNovaZ;

    gameplay.atualizar(deltaTime, gerenciador, logicaB, cameraX, cameraZ);

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

    if (key == 'e' || key == 'E') {
        // Primeiro as portas do confessionário (B); se nenhuma estiver perto, o resto (C)
        if (!logicaB.interagir(cameraX, cameraY, cameraZ)) {
            gerenciador.interagir(cameraX, cameraY, cameraZ);
        }
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

    // Limpa só a profundidade: a lanterna não é cortada por paredes próximas,
    // mas suas próprias peças continuam se escondendo corretamente
    glClear(GL_DEPTH_BUFFER_BIT);
    desenharLanterna(0.0f, 0.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // --- LUZES (Membro B) ---
    // Lanterna ANTES da câmera: fica presa ao olho e segue o olhar
    posicionarLanternaB();

    // Posicionamento da Câmera
	cameraAplicar();

    // Luar e velas DEPOIS da câmera: ficam fixos no mundo
    posicionarLuzesMundoB(gerenciador.velas);

    // Shader Blinn-Phong ligado na CENA INTEIRA (até a lanterna na mão)
    usarShaderB(shaderB, gerenciador.velas, gerenciador.lanternaLigada);
    gameplay.aplicarLuzes();   // luz de fora escurece e lamparina pisca quando o bicho passa

    // Cada materialB(especular, brilho) vale para os objetos desenhados depois dele
    materialB(0.10f, 8.0f);    // pedra e madeira do prédio: foscas
    desenharChao(texChao);
    desenharParedes(texParede);
    desenharTeto(texTeto);
    desenharCorredor(texParede, texChao, texTeto);

    materialB(0.25f, 24.0f);   // madeira envernizada
    desenharGruposBancos(texMadeiraBanco);

    materialB(0.30f, 32.0f);   // madeira + ferragens
    Porta& entrada = gerenciador.portas[0];
    // Ajustei forçadamente a renderização da porta para o novo final da igreja Z = 9.4f
    desenharPortasDuplas(entrada.posX, entrada.posY, 9.4f, entrada.rotInicialY,
        entrada.anguloAtual, texMadeiraBanco, texMetalPorta, texBranca);
    
    // desenho de 4 lustes
    materialB(0.60f, 48.0f);   // metal dos lustres
    float tempoSeg = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    for (int i = 0; i < 4; ++i) {
        float z = 4.55f - i * 9.7f;                    // 4,55 / -5,15 / -14,85 / -24,55
        desenharLustre(0.0f, 9.8f, z, tempoSeg + i * 0.8f, 2.0f, texBranca);
    }
    
    if (modoWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // 8 Vitrais (4 na parede Esquerda, 4 na Direita)
    materialB(0.90f, 96.0f);   // vidro: reflexo forte e concentrado
    float posicoesVitraisZ[] = { -4.0f, -11.0f, -18.0f, -25.0f };
    for (int i = 0; i < 4; i++) {
        // Colocados em 9.38f para evitar "z-fighting" com a textura da parede (9.4f)
        desenharVitral(-9.38f, 1.5f, posicoesVitraisZ[i], 90.0f);
        desenharVitral( 9.38f, 1.5f, posicoesVitraisZ[i], -90.0f);
    }

    static float tempo = 0.0f;
    tempo += 0.016f;
    float anguloPendulo = std::sin(tempo * 3.0f) * 15.0f;
    
    materialB(0.30f, 32.0f);   // madeira + metal
    // 4 Relógios nos cantos, rotacionados para olhar para o centro da nave
    desenharRelogioPendulo(-8.8f, 0.0f,  8.8f,  90.0f, anguloPendulo);  // Atrás Esquerda (perto da porta)
    desenharRelogioPendulo( 8.8f, 0.0f,  8.8f, -90.0f, anguloPendulo);  // Atrás Direita (perto da porta)
    desenharRelogioPendulo(-8.8f, 0.0f, -28.8f, 90.0f, anguloPendulo); // Frente Esquerda (perto do altar)
    desenharRelogioPendulo( 8.8f, 0.0f, -28.8f,-90.0f, anguloPendulo); // Frente Direita (perto do altar)

    // --- MEMBRO B ---
    materialB(0.15f, 16.0f);   // pedra levemente polida
    desenharAltar(0.0f, 0.0f, -28.0f, 0.0f);

    materialB(0.05f, 8.0f);    // madeira fosca
    // 1 Confessionário na parede esquerda, perto do altar, virado para a nave
    // Portas abrem com E (mesma lógica da porta da entrada)
    desenharConfessionario(-8.8f, 0.0f, -21.0f, 90.0f,
        logicaB.portaConfEsq.anguloAtual, logicaB.portaConfDir.anguloAtual);

    // --- 6 Velas (Castiçais) ---
    materialB(0.70f, 64.0f);   // bronze (a cera e a chama ajustam o próprio material)
    // 2 no tampo do Altar
    desenharCastical(-1.0f, 1.0f, -28.0f, gerenciador.velas[0].acesa);
    desenharCastical(1.0f, 1.0f, -28.0f, gerenciador.velas[1].acesa);

    // 2 em cima de Bancos diferentes
    desenharCastical(-2.0f, 0.47f, 4.0f, gerenciador.velas[2].acesa);
    desenharCastical(2.0f, 0.47f, 1.0f, gerenciador.velas[3].acesa);

    // 1 no chão perto do confessionário
    desenharCastical(-6.0f, 0.0f, -20.0f, gerenciador.velas[4].acesa);

    // 1 no chão perto do relógio frontal esquerdo
    desenharCastical(-7.5f, 0.0f, -27.5f, gerenciador.velas[5].acesa);

    // O bicho do final ruim (só aparece nessa fase)
    gameplay.desenharMundo();

    // Lanterna na mão do vigia: SEMPRE POR ÚLTIMO (fica por cima de tudo)
    // Não coloque nenhum objeto depois desta linha!
    materialB(0.60f, 64.0f);   // metal da lanterna (o vidro é emissivo)
    desenharLanternaNaMao();

    glUseProgram(0);
    gameplay.desenharHUD(gerenciador);   // caixa de texto, por cima de tudo
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