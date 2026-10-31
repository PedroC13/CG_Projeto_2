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
        // 1. Limites expansivos da nova arquitetura
        limitesIgreja = { -9.4f, 9.4f, -29.4f, 9.4f };

        obstaculos.clear();

        // 2. Altar Principal (Centro em Z = -28.0f, Largura 2.5f, Profundidade 1.1f)
        obstaculos.push_back({ -1.25f, 1.25f, -28.55f, -27.45f });

        // 3. Relógios de Pêndulo (Criamos uma caixa de 0.8x0.8 ao redor de cada devido à rotação de 45º)
        obstaculos.push_back({ -9.2f, -8.4f,   8.4f,   9.2f }); // Atrás Esquerda (-8.8, 8.8)
        obstaculos.push_back({ 8.4f,  9.2f,   8.4f,   9.2f }); // Atrás Direita  (8.8, 8.8)
        obstaculos.push_back({ -9.2f, -8.4f, -29.2f, -28.4f }); // Frente Esquerda (-8.8, -28.8)
        obstaculos.push_back({ 8.4f,  9.2f, -29.2f, -28.4f }); // Frente Direita  (8.8, -28.8)

        // 4. Blocos de Bancos
        // Como os bancos estão colados, geramos apenas 2 grandes retângulos por fila
        int profundidadeLinhas = 8;
        for (int i = 0; i < profundidadeLinhas; ++i) {
            float z = 4.0f - (i * 3.0f); // Mesma matemática da renderização

            // Bloco Esquerdo (Cobre os 3 bancos: de X = -7.0 até X = -1.0)
            obstaculos.push_back({ -7.0f, -1.0f, z - 0.3f, z + 0.3f });

            // Bloco Direito (Cobre os 3 bancos: de X = 1.0 até X = 7.0)
            obstaculos.push_back({ 1.0f,  7.0f, z - 0.3f, z + 0.3f });
        }
    }

    // Processa o movimento usando raio padrão mais fino (0.2f)
    void processarMovimento(float posAntigaX, float posAntigaZ, float& posNovaX, float& posNovaZ, float raioJogador = 0.1f) {
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
    bool lanternaLigada = true;

    void inicializarCena() {
        colisao.inicializar();

        portas.clear();
        velas.clear();

        // Ajustamos o Z da entrada e o X das portas laterais para os novos limites (9.4f)
        portas.push_back(Porta(1, 0.0f, 0.0f, 9.4f, 0.0f)); // Entrada
        portas.push_back(Porta(2, -9.4f, 0.0f, -10.0f, 90.0f)); // Parede Esquerda
        portas.push_back(Porta(3, 9.4f, 0.0f, -15.0f, -90.0f)); // Parede Direita

        velas.push_back(Vela(1, -2.0f, 0.8f, -3.0f));
        velas.push_back(Vela(2, 2.0f, 0.8f, -8.0f));
        velas.push_back(Vela(3, 0.0f, 1.0f, -27.0f)); // Vela reposicionada para perto do novo altar
    }

    void atualizar(float deltaTime) {
        for (auto& porta : portas) {
            porta.atualizarAnimacao(deltaTime);
        }
    }
};

#endif