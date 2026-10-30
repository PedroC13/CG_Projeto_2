#ifndef LOGIC_MEMBRO_C_H
#define LOGIC_MEMBRO_C_H

#include <vector>
#include <cmath>

struct AABB {
    float minX, maxX;
    float minZ, maxZ;

    bool colide(float x, float z, float raioJogador = 0.2f) const {
        return (x + raioJogador > minX && x - raioJogador < maxX &&
            z + raioJogador > minZ && z - raioJogador < maxZ);
    }
};

struct Porta {
    int id;
    float posX, posY, posZ;
    float rotInicialY;
    float anguloAtual;
    bool aberta;
    float raioInteracao;

    Porta(int id, float x, float y, float z, float rotBase)
        : id(id), posX(x), posY(y), posZ(z), rotInicialY(rotBase),
        anguloAtual(0.0f), aberta(false), raioInteracao(2.0f) {
    }

    void atualizarAnimacao(float deltaTime) {
        float anguloAlvo = aberta ? 90.0f : 0.0f;
        float velocidade = 180.0f;

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

struct Vela {
    int id;
    float posX, posY, posZ;
    bool acesa;
    float raioInteracao;

    Vela(int id, float x, float y, float z)
        : id(id), posX(x), posY(y), posZ(z), acesa(true), raioInteracao(1.8f) {
    }
};

class SistemaColisao {
public:
    AABB limitesIgreja;
    std::vector<AABB> obstaculos;

    void inicializar() {
        // Limites expansivos da nave da igreja
        limitesIgreja = { -4.7f, 4.7f, -14.7f, 4.7f };

        obstaculos.clear();

        // 1. Relógio de Pêndulo no Canto (X=3.8, Z=-2.0)
        obstaculos.push_back({ 3.4f, 4.2f, -2.3f, -1.7f });

        // 2. Bancos Esquerda (Membro A) - caixa ajustada ao tamanho real
        obstaculos.push_back({ -3.6f, -1.2f, -8.5f, -2.5f });

        // 3. Bancos Direita (Membro A) - caixa ajustada ao tamanho real
        obstaculos.push_back({ 1.2f, 3.6f, -8.5f, -2.5f });
    }

    // Processa o movimento usando raio padrão mais fino (0.2f)
    void processarMovimento(float posAntigaX, float posAntigaZ, float& posNovaX, float& posNovaZ, float raioJogador = 0.2f) {
        if (!posicaoValida(posNovaX, posAntigaZ, raioJogador)) {
            posNovaX = posAntigaX;
        }

        if (!posicaoValida(posAntigaX, posNovaZ, raioJogador)) {
            posNovaZ = posAntigaZ;
        }
    }

private:
    bool posicaoValida(float x, float z, float raioJogador) {
        if (x - raioJogador < limitesIgreja.minX || x + raioJogador > limitesIgreja.maxX ||
            z - raioJogador < limitesIgreja.minZ || z + raioJogador > limitesIgreja.maxZ) {
            return false;
        }

        for (const auto& obs : obstaculos) {
            if (obs.colide(x, z, raioJogador)) {
                return false;
            }
        }

        return true;
    }
};

class GerenciadorCena {
public:
    std::vector<Porta> portas;
    std::vector<Vela> velas;
    SistemaColisao colisao;
    bool lanternaLigada = true; // Estado da Lanterna (Tecla F)

    void inicializarCena() {
        colisao.inicializar();

        portas.clear();
        velas.clear();

        portas.push_back(Porta(1, 0.0f, 0.0f, 4.6f, 0.0f)); // Entrada
        portas.push_back(Porta(2, -4.5f, 0.0f, -5.0f, 90.0f)); // Confessionário
        portas.push_back(Porta(3, 4.5f, 0.0f, -10.0f, -90.0f)); // Sacristia

        velas.push_back(Vela(1, -2.0f, 0.8f, -3.0f));
        velas.push_back(Vela(2, 2.0f, 0.8f, -8.0f));
        velas.push_back(Vela(3, 0.0f, 1.0f, -12.5f));
    }

    void atualizar(float deltaTime) {
        for (auto& porta : portas) {
            porta.atualizarAnimacao(deltaTime);
        }
    }
};

#endif