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
        // Raio menor porque as duas portas ficam a ~1.8 m uma da outra
        portaConfEsq.raioInteracao = 1.2f;
        portaConfDir.raioInteracao = 1.2f;
    }

    void atualizar(float deltaTime) {
        portaConfEsq.atualizarAnimacao(deltaTime);
        portaConfDir.atualizarAnimacao(deltaTime);
    }

    // Retorna true se abriu/fechou alguma porta (aí o Membro C não precisa checar)
    bool interagir(float jogX, float jogY, float jogZ) {
        Porta* portas[2] = { &portaConfEsq, &portaConfDir };
        for (Porta* porta : portas) {
            float dx = porta->posX - jogX, dy = porta->posY - jogY, dz = porta->posZ - jogZ;
            float dist = std::sqrt(dx * dx + dy * dy + dz * dz);   // tudo em float: sem aviso C4244
            if (dist <= porta->raioInteracao) {
                porta->aberta = !porta->aberta;
                return true;
            }
        }
        return false;
    }
};

#endif
