#pragma once
#ifndef LOGIC_MEMBRO_C_H
#define LOGIC_MEMBRO_C_H

#include <cmath>
#include <vector>

// ----------------------------------------------------------------------------
// ESTRUTURA PARA CONTROLAR PORTAS
// ----------------------------------------------------------------------------
struct Porta {
    int id;
    float posX, posY, posZ;  // Posição no mundo 3D
    float rotInicialY;       // Rotação da parede (0, 90, 180, etc.)
    float anguloAtual;       // Ângulo atual de abertura (0.0f a 90.0f)
    bool aberta;             // Estado: true = aberta, false = fechada/trancada
    float raioInteracao;     // Distância máxima para o jogador interagir

    Porta(int id, float x, float y, float z, float rotBase)
        : id(id), posX(x), posY(y), posZ(z), rotInicialY(rotBase),
        anguloAtual(0.0f), aberta(false), raioInteracao(2.0f) {
    }

    // Atualiza a animação de rotação suave da porta
    void atualizarAnimacao(float deltaTime) {
        float anguloAlvo = aberta ? 90.0f : 0.0f;
        float velocidade = 180.0f; // Graus por segundo

        if (anguloAtual < anguloAlvo) {
            anguloAtual += velocidade * deltaTime;
            if (anguloAtual > anguloAlvo) anguloAtual = anguloAlvo;
        }
        else if (anguloAtual > anguloAlvo) {
            anguloAtual -= velocidade * deltaTime;
            if (anguloAtual < anguloAlvo) anguloAtual = anguloAlvo;
        }
    }
};

// ----------------------------------------------------------------------------
// ESTRUTURA PARA CONTROLAR VELAS / CASTIÇAIS
// ----------------------------------------------------------------------------
struct Vela {
    int id;
    float posX, posY, posZ;  // Posição da chama no mundo 3D
    bool acesa;              // Estado: true = acesa, false = apagada
    float raioInteracao;     // Distância máxima para acender/apagar
    float corLuz[3];         // Cor RGB da luz emitida

    Vela(int id, float x, float y, float z)
        : id(id), posX(x), posY(y), posZ(z), acesa(true), raioInteracao(1.8f) {
        corLuz[0] = 1.0f; // R (Laranja)
        corLuz[1] = 0.5f; // G
        corLuz[2] = 0.1f; // B
    }
};

// ----------------------------------------------------------------------------
// GERENCIADOR GLOBAL DO ESTADO DA CENA
// ----------------------------------------------------------------------------
class GerenciadorCena {
public:
    std::vector<Porta> portas;
    std::vector<Vela> velas;

    void inicializarCena() {
        portas.clear();
        velas.clear();

        // 1. Instancia as 3 Portas da Igreja
        portas.push_back(Porta(1, 0.0f, 0.0f, 5.0f, 0.0f)); // Entrada Principal
        portas.push_back(Porta(2, -4.5f, 0.0f, -5.0f, 90.0f)); // Confessionário
        portas.push_back(Porta(3, 4.5f, 0.0f, -10.0f, -90.0f)); // Sacristia

        // 2. Instancia as 3 Velas/Castiçais
        velas.push_back(Vela(1, -2.0f, 0.8f, -3.0f)); // Corredor Esquerdo
        velas.push_back(Vela(2, 2.0f, 0.8f, -8.0f)); // Corredor Direito
        velas.push_back(Vela(3, 0.0f, 1.0f, -12.5f)); // Altar Principal
    }

    // Atualiza animações a cada frame
    void atualizar(float deltaTime) {
        for (auto& porta : portas) {
            porta.atualizarAnimacao(deltaTime);
        }
    }
};

#endif