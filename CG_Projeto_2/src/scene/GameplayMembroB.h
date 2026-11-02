#pragma once
#ifndef GAMEPLAY_MEMBRO_B_H
#define GAMEPLAY_MEMBRO_B_H

#include <GL/glew.h>
#include <GL/freeglut.h>
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
// 4. Escondido    jogador dentro de uma cabine do confessionário: conta 15 s
// 5. Final        porta da cabine FECHADA -> final ruim (o bicho aparece)
//                 porta da cabine ABERTA  -> final bom  (a igreja se abre)
// ============================================================================
enum class Fase { ApagarVelas, PortaBateu, Esconda, Escondido, FinalRuim, FinalBom };

struct Gameplay {
    Fase fase = Fase::ApagarVelas;
    float tempoFase = 0.0f;     // segundos desde o início da fase atual
    int cabineFinal = 0;        // 1 = cabine da esquerda, 2 = da direita

    const float ESPERA_ESCONDA   = 3.0f;
    const float TEMPO_ESCONDIDO  = 15.0f;

    void mudar(Fase nova) { fase = nova; tempoFase = 0.0f; }

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
            if (c == 0) { mudar(Fase::Esconda); break; }      // saiu: o tempo recomeça
            if (tempoFase >= TEMPO_ESCONDIDO) {
                Porta& porta = (c == 1) ? b.portaConfEsq : b.portaConfDir;
                cabineFinal = c;
                if (porta.aberta) mudar(Fase::FinalBom);
                else { mudar(Fase::FinalRuim); g.lanternaLigada = false; }  // tudo escuro
            }
            break;
        }
        default:
            break;
        }

        // Porta da entrada: trancada depois que as velas apagam, aberta no final bom
        if (fase == Fase::FinalBom)          g.portas[0].aberta = true;
        else if (fase != Fase::ApagarVelas)  g.portas[0].aberta = false;
    }

    // O bicho: um vulto preto no compartimento do padre, com olhos vermelhos
    // na altura da grade (o único lugar por onde o jogador vê o outro lado)
    // virados para a cabine onde o jogador se escondeu. Chamar com o shader ligado.
    void desenharMundo() const {
        if (fase != Fase::FinalRuim) return;

        glBindTexture(GL_TEXTURE_2D, texBranca);
        materialB(0.05f, 4.0f);
        glColor4f(0.02f, 0.02f, 0.02f, 1.0f);
        glPushMatrix();
        glTranslatef(-8.85f, 0.95f, -21.0f);
        glScalef(0.6f, 1.9f, 0.6f);
        glutSolidCube(1.0);
        glPopMatrix();

        float zOlhos = (cabineFinal == 1) ? -20.68f : -21.32f;
        emissaoB(1.0f, 0.05f, 0.05f);
        glColor4f(1.0f, 0.1f, 0.1f, 1.0f);
        for (float dx : { -0.09f, 0.09f }) {
            glPushMatrix();
            glTranslatef(-8.85f + dx, 1.38f, zOlhos);   // na altura da grade
            glutSolidSphere(0.025, 10, 8);
            glPopMatrix();
        }
        emissaoB(0.0f, 0.0f, 0.0f);
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
            snprintf(linha2, sizeof(linha2), "FINAL RUIM  -  ESC para sair");
            r = 1.0f; gr = 0.25f; bl = 0.25f;
            break;
        case Fase::FinalBom:
            linha1 = "O silêncio voltou. A porta da igreja está aberta.";
            snprintf(linha2, sizeof(linha2), "FINAL BOM  -  ESC para sair");
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
