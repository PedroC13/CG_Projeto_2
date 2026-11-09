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
#include "render/Varinha.h"
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

// --- MENU INICIAL E PAUSA (tecla P) ---
// Pausado: o jogo congela e o mouse fica livre (aparece o cursor e ele não é
// mais puxado para o centro), para poder redimensionar ou mover a janela.
// O jogo começa pausado, mostrando o menu com o objetivo e os comandos.
bool pausado = true;
bool menuInicial = true;   // true só antes da primeira partida (ENTER começa)

void alternarPausa() {
    pausado = !pausado;
    for (int i = 0; i < 256; i++) teclas[i] = false;   // nada de andar sozinho ao voltar

    if (pausado) {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    } else {
        glutSetCursor(GLUT_CURSOR_NONE);
        primeiroMouse = true;   // o 1o movimento só recentraliza: a câmera não dá um pulo
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    }
    glutPostRedisplay();
}

// Mouse só gira a câmera fora da pausa
void mouseMovimento(int x, int y) {
    if (!pausado) cameraMouse(x, y);
}

// --- MIRA E VARINHA ---
// O que está no centro da tela agora (no máximo um dos dois). Atualizado todo frame.
Porta* alvoPorta = nullptr;
Vela*  alvoVela  = nullptr;

const float DURACAO_GOLPE = 0.35f;   // segundos da animação da varinha
float animVarinha = 0.0f;            // > 0 enquanto a varinha está abaixando/subindo

// Procura o objeto no centro da mira entre o que é do Membro C (porta da entrada
// e velas) e do Membro B (portas do confessionário). Fica o mais centralizado.
void atualizarAlvo() {
    float fX, fY, fZ;
    cameraFrente(fX, fY, fZ);

    Porta* portaC; Vela* vela; Porta* portaB;
    float notaC = gerenciador.procurarAlvo(cameraX, cameraY, cameraZ, fX, fY, fZ, portaC, vela);
    float notaB = logicaB.procurarAlvo(cameraX, cameraY, cameraZ, fX, fY, fZ, portaB);

    if (notaB > notaC) { alvoPorta = portaB; alvoVela = nullptr; }
    else               { alvoPorta = portaC; alvoVela = vela; }
}

// Recomeça a partida do zero (tecla R na tela de final)
void reiniciarJogo() {
    gerenciador.inicializarCena();        // velas, porta da entrada e colisões do zero
    gerenciador.lanternaLigada = true;    // o final ruim apaga a lamparina
    logicaB.inicializar();

    gameplay.passagem = Passagem::Esperando;
    gameplay.cabineFinal = 0;
    gameplay.iniciar(gerenciador, logicaB, cameraX, cameraZ, yaw);   // acende as velas e põe o jogador no corredor
    pitch = 0.0f;

    alvoPorta = nullptr;                  // as portas/velas foram recriadas
    alvoVela = nullptr;
    animVarinha = 0.0f;
    for (int i = 0; i < 256; i++) teclas[i] = false;
}

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
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);   // começa no menu: mouse livre
}

void timer(int value) {
    int tempo_atual = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (tempo_atual - tempo_anterior) / 1000.0f;
    tempo_anterior = tempo_atual;

    if (pausado) {   // congela tudo, mas continua redesenhando (ex.: ao redimensionar)
        glutPostRedisplay();
        glutTimerFunc(16, timer, 0);
        return;
    }

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
    gerenciador.colisaoPortaEntrada(posAntigaX, posAntigaZ, posNovaX, posNovaZ);   // porta da entrada

    cameraX = posNovaX;
    cameraZ = posNovaZ;

    gameplay.atualizar(deltaTime, gerenciador, logicaB, cameraX, cameraZ);

    atualizarAlvo();   // o que está na mira depois de andar/olhar

    if (animVarinha > 0.0f) {
        animVarinha -= deltaTime;
        if (animVarinha < 0.0f) animVarinha = 0.0f;
    }

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
bool luzGlobal = false;   // DEBUG: tecla G clareia a cena inteira

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);   // ESC sai sempre (menu, pausa ou jogo)

    if (menuInicial) {        // menu inicial: só ENTER (ou espaço) começa
        if (key == 13 || key == ' ') {
            menuInicial = false;
            alternarPausa();   // despausa: esconde o cursor e libera o jogo
        }
        return;
    }

    if (key == 'p' || key == 'P') {
        alternarPausa();
        return;
    }
    if (pausado) return;       // pausado: ignora as outras teclas

    // Tela de final: R recomeça a partida
    if ((key == 'r' || key == 'R') &&
        (gameplay.fase == Fase::FinalRuim || gameplay.fase == Fase::FinalBom)) {
        reiniciarJogo();
        return;
    }

    cameraTeclaDown(key);

    if (key == 'g' || key == 'G') {
        luzGlobal = !luzGlobal;
        glutPostRedisplay();
    }

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
        // Interage com o que está no CENTRO DA MIRA (ver atualizarAlvo)
        if (alvoPorta) {
            alvoPorta->aberta = !alvoPorta->aberta;
        } else if (alvoVela) {
            alvoVela->acesa = false;          // a varinha só apaga
            animVarinha = DURACAO_GOLPE;      // dispara a animação da varinha
        }
        glutPostRedisplay();
    }
}

// OBJETOS NA MÃO DO VIGIA (HUD 3D): varinha na esquerda, lamparina na direita.
// Desenhados no espaço da CÂMERA (glLoadIdentity), por isso acompanham o olhar.
void desenharObjetosNaMao() {
    glPushMatrix();
    glLoadIdentity(); // Entra no espaço da câmera

    // Limpa só a profundidade: os objetos da mão não são cortados por paredes próximas,
    // mas suas próprias peças continuam se escondendo corretamente
    glClear(GL_DEPTH_BUFFER_BIT);

    // Varinha: canto inferior esquerdo. Ao apagar uma vela, a ponta desce e volta (seno de 0 a pi).
    float golpe = 0.0f;
    if (animVarinha > 0.0f) golpe = std::sin(3.14159265f * animVarinha / DURACAO_GOLPE);
    materialB(0.50f, 48.0f);   // latão
    glPushMatrix();
    glTranslatef(-0.25f, -0.28f, -0.50f);
    desenharVarinha(golpe);
    glPopMatrix();

    // Lamparina: canto inferior direito (só quando ligada, tecla F)
    if (gerenciador.lanternaLigada) {
        materialB(0.60f, 64.0f);   // metal da lanterna (o vidro é emissivo)
        glPushMatrix();
        glTranslatef(0.28f, -0.32f, -0.55f);
        glRotatef(12.0f, 0.0f, 1.0f, 0.0f);   // Inclina suavemente para o centro
        glScalef(0.35f, 0.35f, 0.35f);         // Reduz escala para proporção de mão
        desenharLanterna(0.0f, 0.0f, 0.0f, 0.0f);
        glPopMatrix();
    }

    glPopMatrix();
}

// PAINEL DE COMANDOS (menu inicial e pausa). Já deve estar em projeção 2D (glOrtho).
// Escurece a tela e mostra o objetivo + a lista de teclas.
void desenharPainelComandos(int w, int h) {
    // Fundo: tela inteira escurecida + painel mais escuro no meio
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f((float)w, 0); glVertex2f((float)w, (float)h); glVertex2f(0, (float)h);
    glEnd();

    float pw = 520.0f, ph = 400.0f;                 // tamanho do painel
    float px = (w - pw) / 2.0f, py = (h - ph) / 2.0f;
    glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
    glBegin(GL_QUADS);
    glVertex2f(px, py); glVertex2f(px + pw, py); glVertex2f(px + pw, py + ph); glVertex2f(px, py + ph);
    glEnd();
    glDisable(GL_BLEND);

    float cx = w / 2.0f;
    float y = py + ph - 45.0f;                       // escreve de cima para baixo

    // Título e objetivo (centralizados)
    const char* titulo = menuInicial ? "O TURNO NOTURNO" : "PAUSADO";
    glColor3f(1.0f, 0.85f, 0.30f);
    Gameplay::texto(cx - Gameplay::larguraTexto(titulo) / 2.0f, y, titulo);
    y -= 38.0f;

    const char* objetivo[] = {
        "Você é o vigia da igreja.",
        "Apague todas as velas com a varinha.",
        "Depois, siga o que aparecer na tela.",
    };
    glColor3f(0.90f, 0.90f, 0.90f);
    for (const char* linha : objetivo) {
        Gameplay::texto(cx - Gameplay::larguraTexto(linha) / 2.0f, y, linha);
        y -= 24.0f;
    }
    y -= 14.0f;

    // Comandos: tecla (amarela) à esquerda, descrição à direita
    const char* comandos[][2] = {
        { "W A S D", "andar" },
        { "Mouse",   "olhar (a mira mostra o alvo)" },
        { "E",       "apagar vela / abrir e fechar porta" },
        { "F",       "lamparina" },
        { "P",       "pausar e liberar o mouse" },
        { "T  /  G", "wireframe  /  luz global (debug)" },
        { "ESC",     "sair" },
    };
    float colTecla = px + 50.0f, colDesc = px + 170.0f;
    for (auto& c : comandos) {
        glColor3f(1.0f, 0.85f, 0.30f);
        Gameplay::texto(colTecla, y, c[0]);
        glColor3f(0.85f, 0.85f, 0.85f);
        Gameplay::texto(colDesc, y, c[1]);
        y -= 24.0f;
    }

    // Rodapé: como sair desta tela
    const char* rodape = menuInicial ? "Pressione ENTER para começar" : "Pressione P para continuar";
    glColor3f(1.0f, 1.0f, 1.0f);
    Gameplay::texto(cx - Gameplay::larguraTexto(rodape) / 2.0f, py + 25.0f, rodape);
}

// MIRA no centro da tela (2D, por cima de tudo). Fica amarela e maior quando
// há algo para interagir, com a dica da tecla embaixo. Chamar com o shader DESLIGADO.
void desenharMira() {
    int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
    float cx = w / 2.0f, cy = h / 2.0f;
    bool temAlvo = (alvoPorta != nullptr || alvoVela != nullptr);
    bool fimDeJogo = (gameplay.fase == Fase::FinalRuim || gameplay.fase == Fase::FinalBom);
    if (fimDeJogo && !pausado) return;   // tela de final: sem mira

    // Projeção 2D em pixels (mesmo esquema da caixa de texto do HUD)
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    if (pausado) {
        desenharPainelComandos(w, h);   // menu inicial ou pausa (no lugar da mira)
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);  glPopMatrix();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }

    if (temAlvo) glColor3f(1.0f, 0.85f, 0.30f);
    else         glColor3f(0.85f, 0.85f, 0.85f);

    float t = temAlvo ? 10.0f : 6.0f;   // tamanho dos tracinhos
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(cx - t, cy);  glVertex2f(cx - 3, cy);   // esquerda
    glVertex2f(cx + 3, cy);  glVertex2f(cx + t, cy);   // direita
    glVertex2f(cx, cy - t);  glVertex2f(cx, cy - 3);   // baixo
    glVertex2f(cx, cy + 3);  glVertex2f(cx, cy + t);   // cima
    glEnd();
    glLineWidth(1.0f);

    if (temAlvo) {
        const char* dica = alvoVela ? "[E] Apagar vela"
                         : (alvoPorta->aberta ? "[E] Fechar porta" : "[E] Abrir porta");
        Gameplay::texto(cx - Gameplay::larguraTexto(dica) / 2.0f, cy - 36.0f, dica);
    }

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPolygonMode(GL_FRONT_AND_BACK, modoWireframe ? GL_LINE : GL_FILL);

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

    // Lustres: acesos só enquanto há velas para apagar (a porta bate e eles apagam)
    bool lustresAcesos = (gameplay.fase == Fase::ApagarVelas);
    float luzLustres[NUM_LUSTRES_B][3];
    for (int i = 0; i < NUM_LUSTRES_B; ++i) {
        luzLustres[i][0] = 0.0f;
        luzLustres[i][1] = 6.9f;                 // centro do aro: 9.8 - 2 x 1.7 + chamas
        luzLustres[i][2] = 4.55f - i * 9.7f;     // mesmos Z do desenho dos lustres
    }
    enviarLustresB(shaderB, luzLustres, lustresAcesos);

    // DEBUG: luz ambiente cheia (precisa vir DEPOIS de aplicarLuzes, que redefine o ambiente todo frame)
    if (luzGlobal) {
        GLfloat ambienteDebug[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambienteDebug);
    }

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
        desenharLustre(0.0f, 9.8f, z, tempoSeg + i * 0.8f, 2.0f, texBranca, lustresAcesos);
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

    // Varinha e lanterna na mão do vigia: SEMPRE POR ÚLTIMO (ficam por cima de tudo)
    // Não coloque nenhum objeto depois desta linha!
    desenharObjetosNaMao();

    glUseProgram(0);
    if (!pausado) gameplay.desenharHUD(gerenciador);   // caixa de texto (some no menu/pausa)
    desenharMira();                      // mira no centro da tela
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Igreja Assombrada - Visualizacao (T Wireframe, F Lanterna, P Pausa)");

    setup();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(cameraTeclaUp);
    glutIgnoreKeyRepeat(1);
    glutPassiveMotionFunc(mouseMovimento);
    glutMotionFunc(mouseMovimento);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}