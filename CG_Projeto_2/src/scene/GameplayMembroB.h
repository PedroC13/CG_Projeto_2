#pragma once
#ifndef GAMEPLAY_MEMBRO_B_H
#define GAMEPLAY_MEMBRO_B_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>                       // snprintf
#include "LogicMembroC.h"              // velas e portas (Membro C)
#include "LogicMembroB.h"              // portas do confessionário
#include "../render/ShaderMembroB.h"   // materialB / emissaoB

extern GLuint texBranca;

// ============================================================================
// GAMEPLAY: "O Turno Noturno"
// ----------------------------------------------------------------------------
// 1. ApagarVelas  "Apague as velas da igreja. Não feche as portas."
// 2. PortaBateu   a última vela apaga -> a porta da entrada bate e tranca
// 3. Esconda      depois de 3 s aparece "Esconda-se."
// 4. Escondido    jogador numa cabine do confessionário. 3 s depois o bicho passa
//                 pela frente do confessionário, e a luz de fora escurece:
//                 - porta da cabine FECHADA: um vulto gigante passa devagar
//                   (visto pelas frestas) e, 5 s depois, entra no compartimento
//                   do padre -> FINAL RUIM
//                 - porta ABERTA: o corpo nunca aparece; a lamparina pisca e só
//                   uma sombra passa rápido pelo chão. Abrir a porta enquanto o
//                   vulto passa devagar faz ele sumir.
// 5. Final        aos 15 s escondido: porta aberta -> FINAL BOM (igreja se abre)
//                                     porta fechada -> FINAL RUIM
// ============================================================================
enum class Fase { ApagarVelas, PortaBateu, Esconda, Escondido, FinalRuim, FinalBom };
enum class Passagem { Esperando, Lenta, Entrando, Rapida, Acabou };

struct Gameplay {
    Fase fase = Fase::ApagarVelas;
    float tempoFase = 0.0f;     // segundos desde o início da fase atual
    int cabineFinal = 0;        // 1 = cabine da esquerda, 2 = da direita

    // O bicho, enquanto passa pela frente do confessionário
    Passagem passagem = Passagem::Esperando;
    float bichoX = 0.0f, bichoZ = 0.0f;

    const float ESPERA_ESCONDA   = 3.0f;
    const float TEMPO_ESCONDIDO  = 15.0f;
    const float INICIO_PASSAGEM  = 3.0f;    // segundos escondido até o bicho aparecer
    const float X_FRENTE  = -7.5f;          // linha em frente ao confessionário
    const float X_PADRE   = -8.85f;         // dentro do compartimento do padre
    const float Z_INICIO  = -16.0f;
    const float Z_PADRE   = -21.0f;         // 5 m do início: 5 s andando devagar
    const float Z_FIM     = -27.0f;
    const float VEL_LENTA = 1.0f, VEL_RAPIDA = 5.0f, VEL_ENTRAR = 4.0f;   // m/s

    void mudar(Fase nova) {
        fase = nova;
        tempoFase = 0.0f;
        if (nova == Fase::Escondido) passagem = Passagem::Esperando;
    }

    // Começo do jogo: velas acesas, portas do confessionário abertas
    // ("não feche as portas") e jogador no corredor, de frente para a entrada
    void iniciar(GerenciadorCena& g, LogicaMembroB& b, float& camX, float& camZ, float& camYaw) {
        for (auto& v : g.velas) v.acesa = true;
        for (Porta* p : { &b.portaConfEsq, &b.portaConfDir }) { p->aberta = true; p->anguloAtual = 90.0f; }
        camX = 0.0f;  camZ = 15.0f;  camYaw = -90.0f;
        mudar(Fase::ApagarVelas);
    }

    int velasAcesas(const GerenciadorCena& g) const {
        int n = 0;
        for (const auto& v : g.velas) if (v.acesa) n++;
        return n;
    }

    // Em qual cabine de penitente o jogador está (0 = nenhuma).
    // Confessionário em (-8.8, 0, -21.0), girado 90 graus; frente em x = -8.35.
    static int cabine(float x, float z) {
        if (x > -8.45f) return 0;
        if (z > -20.45f && z < -19.9f) return 1;   // cabine da esquerda
        if (z > -22.1f && z < -21.55f) return 2;   // cabine da direita
        return 0;
    }

    void atualizar(float dt, GerenciadorCena& g, LogicaMembroB& b, float camX, float camZ) {
        tempoFase += dt;

        switch (fase) {
        case Fase::ApagarVelas:
            if (velasAcesas(g) == 0) mudar(Fase::PortaBateu);
            break;
        case Fase::PortaBateu:
            if (tempoFase >= ESPERA_ESCONDA) mudar(Fase::Esconda);
            break;
        case Fase::Esconda:
            if (cabine(camX, camZ) != 0) mudar(Fase::Escondido);
            break;
        case Fase::Escondido: {
            int c = cabine(camX, camZ);
            if (c == 0) { mudar(Fase::Esconda); break; }      // saiu: tudo recomeça
            const Porta& porta = (c == 1) ? b.portaConfEsq : b.portaConfDir;
            bool fechada = porta.anguloAtual < 45.0f;
            cabineFinal = c;
            atualizarPassagem(dt, fechada, g);
            if (fase == Fase::Escondido && tempoFase >= TEMPO_ESCONDIDO && passagem == Passagem::Acabou) {
                if (fechada) { mudar(Fase::FinalRuim); g.lanternaLigada = false; }
                else           mudar(Fase::FinalBom);
            }
            break;
        }
        default:
            break;
        }

        // Porta da entrada: trancada depois que as velas apagam, aberta no final bom
        if (fase == Fase::FinalBom)          g.portas[0].aberta = true;
        else if (fase != Fase::ApagarVelas)  g.portas[0].aberta = false;
        g.portas[0].trancada = (fase != Fase::ApagarVelas && fase != Fase::FinalBom);   // E não mexe nela
    }

    void atualizarPassagem(float dt, bool fechada, GerenciadorCena& g) {
        switch (passagem) {
        case Passagem::Esperando:
            if (tempoFase >= INICIO_PASSAGEM) {
                bichoX = X_FRENTE;  bichoZ = Z_INICIO;
                passagem = fechada ? Passagem::Lenta : Passagem::Rapida;
            }
            break;
        case Passagem::Lenta:                              // porta fechada: anda devagar
            if (!fechada) { passagem = Passagem::Acabou; break; }   // abriu a porta: ele some
            bichoZ -= VEL_LENTA * dt;
            if (bichoZ <= Z_PADRE) { bichoZ = Z_PADRE; passagem = Passagem::Entrando; }
            break;
        case Passagem::Entrando:                           // entra rápido no compartimento do padre
            if (!fechada) { passagem = Passagem::Acabou; break; }
            bichoX -= VEL_ENTRAR * dt;
            if (bichoX <= X_PADRE) {
                passagem = Passagem::Acabou;
                mudar(Fase::FinalRuim);
                g.lanternaLigada = false;                  // tudo escuro, só os olhos
            }
            break;
        case Passagem::Rapida:                             // porta aberta: só a sombra, rápido
            bichoZ -= VEL_RAPIDA * dt;
            if (bichoZ <= Z_FIM) passagem = Passagem::Acabou;
            break;
        default:
            break;
        }
    }

    bool passando() const {
        return fase == Fase::Escondido &&
               (passagem == Passagem::Lenta || passagem == Passagem::Entrando || passagem == Passagem::Rapida);
    }

    // ------------------------------------------------------------- luzes ---
    // Chamar todo frame no display(), depois do usarShaderB(). Os valores base
    // são os mesmos do configurarLuzesB() (ShaderMembroB.h).
    void aplicarLuzes() const {
        float fora = passando() ? 0.35f : 1.0f;            // luz de fora escurece

        float lamp = 1.0f;                                 // lamparina pisca (porta aberta)
        if (passagem == Passagem::Rapida && fase == Fase::Escondido) {
            float t = tempoFase;
            lamp = (std::sin(t * 41.0f) + std::sin(t * 17.0f) > 0.4f) ? 0.12f : 1.0f;
        }

        GLfloat ambiente[]  = { 0.07f * fora, 0.07f * fora, 0.09f * fora, 1.0f };
        GLfloat luar[]      = { 0.10f * fora, 0.12f * fora, 0.20f * fora, 1.0f };
        GLfloat lamparina[] = { 1.0f * lamp, 0.78f * lamp, 0.45f * lamp, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luar);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lamparina);
    }

    // ------------------------------------------------------------- bicho ---
    // Chamar com o shader ligado, antes da lamparina na mão.
    void desenharMundo() const {
        glBindTexture(GL_TEXTURE_2D, texBranca);

        // Porta fechada: vulto gigante (mais alto que o confessionário, então de
        // dentro não dá para ver onde ele termina), visto pelas frestas
        if (fase == Fase::Escondido && (passagem == Passagem::Lenta || passagem == Passagem::Entrando)) {
            materialB(0.0f, 1.0f);
            glColor4f(0.01f, 0.01f, 0.01f, 1.0f);
            glPushMatrix();
            glTranslatef(bichoX, 2.5f, bichoZ);
            glScalef(0.6f, 5.0f, 1.0f);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        // Porta aberta: o corpo nunca aparece, só uma sombra passando no chão
        if (fase == Fase::Escondido && passagem == Passagem::Rapida) {
            materialB(0.0f, 1.0f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);
            // mancha oval: escura no centro, sumindo nas bordas
            glBegin(GL_TRIANGLE_FAN);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glColor4f(0.0f, 0.0f, 0.0f, 0.85f);
            glVertex3f(bichoX, 0.02f, bichoZ);
            glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
            for (int i = 0; i <= 24; i++) {
                float a = i * 6.2831853f / 24;
                glVertex3f(bichoX + 0.7f * std::cos(a), 0.02f, bichoZ + 1.1f * std::sin(a));
            }
            glEnd();
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }

        // Final ruim: o vulto no compartimento do padre, com os olhos vermelhos
        // na altura da grade (o único lugar por onde o jogador vê o outro lado)
        if (fase == Fase::FinalRuim) {
            materialB(0.05f, 4.0f);
            glColor4f(0.02f, 0.02f, 0.02f, 1.0f);
            glPushMatrix();
            glTranslatef(X_PADRE, 0.95f, Z_PADRE);
            glScalef(0.6f, 1.9f, 0.6f);
            glutSolidCube(1.0);
            glPopMatrix();

            float zOlhos = (cabineFinal == 1) ? -20.68f : -21.32f;
            emissaoB(1.0f, 0.05f, 0.05f);
            glColor4f(1.0f, 0.1f, 0.1f, 1.0f);
            for (float dx : { -0.09f, 0.09f }) {
                glPushMatrix();
                glTranslatef(X_PADRE + dx, 1.38f, zOlhos);
                glutSolidSphere(0.025, 10, 8);
                glPopMatrix();
            }
            emissaoB(0.0f, 0.0f, 0.0f);
        }

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // ---------------------------------------------------------------- HUD ---
    // Texto com acentos: o código está em UTF-8 e as fontes do GLUT em Latin-1
    static void texto(float x, float y, const char* s) {
        glRasterPos2f(x, y);
        for (const unsigned char* p = (const unsigned char*)s; *p; p++) {
            unsigned char c = *p;
            if ((c == 0xC2 || c == 0xC3) && p[1]) { c = (unsigned char)(((c & 0x03) << 6) | (p[1] & 0x3F)); p++; }
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }
    static int larguraTexto(const char* s) {
        int w = 0;
        for (const unsigned char* p = (const unsigned char*)s; *p; p++) {
            unsigned char c = *p;
            if ((c == 0xC2 || c == 0xC3) && p[1]) { c = (unsigned char)(((c & 0x03) << 6) | (p[1] & 0x3F)); p++; }
            w += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, c);
        }
        return w;
    }

    // Caixa de texto na parte de baixo da tela. Chamar com o shader DESLIGADO.
    void desenharHUD(const GerenciadorCena& g) const {
        const char* linha1 = nullptr;
        char linha2[64] = "";
        float r = 1.0f, gr = 1.0f, bl = 1.0f;

        switch (fase) {
        case Fase::ApagarVelas:
            linha1 = "Apague as velas da igreja. Não feche as portas.";
            snprintf(linha2, sizeof(linha2), "Velas acesas: %d de %d", velasAcesas(g), (int)g.velas.size());
            break;
        case Fase::Esconda:
            linha1 = "Esconda-se.";
            break;
        case Fase::FinalRuim:
            linha1 = "Você não estava sozinho.";
            snprintf(linha2, sizeof(linha2), "FINAL RUIM  -  R para reiniciar  -  ESC para sair");
            r = 1.0f; gr = 0.25f; bl = 0.25f;
            break;
        case Fase::FinalBom:
            linha1 = "O silêncio voltou. A porta da igreja está aberta.";
            snprintf(linha2, sizeof(linha2), "FINAL BOM  -  R para reiniciar  -  ESC para sair");
            break;
        default:
            return;   // porta batendo e escondido: tela sem texto (tensão)
        }

        int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);

        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
        glOrtho(0, w, 0, h, -1, 1);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);

        // caixa semitransparente: embaixo; no final ruim, no topo (não cobre os olhos)
        float alturaCaixa = linha2[0] ? 76.0f : 48.0f;
        float y0 = (fase == Fase::FinalRuim) ? h - 30.0f - alturaCaixa : 30.0f;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.65f);
        glBegin(GL_QUADS);
        glVertex2f(40, y0); glVertex2f(w - 40.0f, y0);
        glVertex2f(w - 40.0f, y0 + alturaCaixa); glVertex2f(40, y0 + alturaCaixa);
        glEnd();
        glDisable(GL_BLEND);

        glColor3f(r, gr, bl);
        float y1 = y0 + alturaCaixa - 30;
        texto((w - larguraTexto(linha1)) / 2.0f, y1, linha1);
        if (linha2[0]) {
            glColor3f(0.75f, 0.75f, 0.75f);
            texto((w - larguraTexto(linha2)) / 2.0f, y1 - 28, linha2);
        }

        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);  glPopMatrix();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
};

#endif
