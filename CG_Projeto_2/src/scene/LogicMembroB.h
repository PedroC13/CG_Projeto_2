#pragma once
#ifndef LOGIC_MEMBRO_B_H
#define LOGIC_MEMBRO_B_H

#include <cmath>
#include "LogicMembroC.h"   // reaproveita a struct Porta do Membro C

// ============================================================================
// LÓGICA DO MEMBRO B: portas do confessionário
// ----------------------------------------------------------------------------
// Usa a mesma struct Porta da entrada (anguloAtual animado de 0 a 90 graus).
// Confessionário em (-8.8, 0, -21.0), girado 90 graus.
// Posição de cada porta = centro da folha, na altura da mão (y = 1.0).
// ============================================================================
struct LogicaMembroB {
    Porta portaConfEsq{ 4, -8.33f, 1.0f, -20.11f, 90.0f };
    Porta portaConfDir{ 5, -8.33f, 1.0f, -21.89f, 90.0f };

    void inicializar() {
        // Alcance do E. Como agora vale a MIRA (o que está no centro da tela),
        // as duas portas (a ~1.8 m uma da outra) não se confundem mais.
        portaConfEsq.raioInteracao = 1.6f;
        portaConfDir.raioInteracao = 1.6f;
    }

    void atualizar(float deltaTime) {
        portaConfEsq.atualizarAnimacao(deltaTime);
        portaConfDir.atualizarAnimacao(deltaTime);
    }

    // Procura qual porta do confessionário está no centro da mira.
    // Devolve a nota do melhor alvo (-1 = nenhuma). Ver notaMira (LogicMembroC.h).
    float procurarAlvo(float jogX, float jogY, float jogZ, float fX, float fY, float fZ, Porta*& alvo) {
        float melhor = -1.0f;
        alvo = nullptr;
        Porta* portas[2] = { &portaConfEsq, &portaConfDir };
        for (Porta* porta : portas) {
            // ponto da porta mais perto do jogador (folha de ~0,6 m de largura, ao longo de Z)
            float pontoY = limitar(jogY, porta->posY - 0.8f, porta->posY + 0.8f);
            float pontoZ = limitar(jogZ, porta->posZ - 0.3f, porta->posZ + 0.3f);
            float nota = notaMira(porta->posX, pontoY, pontoZ,
                                  jogX, jogY, jogZ, fX, fY, fZ, porta->raioInteracao, 0.80f);
            if (nota > melhor) { melhor = nota; alvo = porta; }
        }
        return melhor;
    }

    // Colisão das portas: uma porta quase fechada (menos de 45 graus) tapa a
    // entrada da cabine. Mesmo esquema do Membro C: testa cada eixo separado,
    // assim o jogador desliza encostado na porta em vez de travar.
    void colisaoPortas(float antX, float antZ, float& novoX, float& novoZ, float raio = 0.2f) const {
        if (!livre(antX, antZ, raio)) return;   // porta fechou em cima do jogador: deixa sair
        if (!livre(novoX, antZ, raio)) novoX = antX;
        if (!livre(antX, novoZ, raio)) novoZ = antZ;
    }

    bool livre(float x, float z, float raio) const {
        // vão de cada cabine na frente do confessionário (x = -8.35)
        if (portaConfEsq.anguloAtual < 45.0f && bate(x, z, raio, -20.50f, -19.78f)) return false;
        if (portaConfDir.anguloAtual < 45.0f && bate(x, z, raio, -22.22f, -21.50f)) return false;
        return true;
    }

    static bool bate(float x, float z, float raio, float minZ, float maxZ) {
        return x + raio > -8.45f && x - raio < -8.25f && z + raio > minZ && z - raio < maxZ;
    }
};

#endif
