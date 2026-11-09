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
        : id(id), posX(x), posY(y), posZ(z), acesa(false), raioInteracao(1.8f) {
    }
};

class SistemaColisao {
public:
    AABB limitesIgreja;
    std::vector<AABB> obstaculos;

    void inicializar() {
        // 1. Limites expansivos da nova arquitetura
        limitesIgreja = { -9.4f, 9.4f, -29.4f, 19.4f };
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

        // 5. Confessionário "Oco" (Em X=-8.8 e Z=-21.0, girado 90º)
        // Permite a entrada do jogador nas duas cabines laterais dos penitentes

        // Fundo do confessionário (colado na parede da igreja)
        obstaculos.push_back({ -9.4f, -9.1f, -22.3f, -19.7f });

        // Parede Externa Direita 
        obstaculos.push_back({ -9.1f, -8.3f, -22.3f, -22.1f });

        // Divisória Interna Direita (Entre penitente e padre)
        obstaculos.push_back({ -9.1f, -8.3f, -21.55f, -21.45f });

        // Divisória Interna Esquerda (Entre padre e penitente)
        obstaculos.push_back({ -9.1f, -8.3f, -20.55f, -20.45f });

        // Parede Externa Esquerda
        obstaculos.push_back({ -9.1f, -8.3f, -19.9f, -19.7f });

        // Porta e Cortina do Padre (Bloqueia a entrada exclusiva na cabine central)
        obstaculos.push_back({ -8.5f, -8.3f, -21.45f, -20.55f });
        
        // 6. Paredes de Bloqueio Externas ao Corredor (Z entre 9.4 e 19.4)
        // Impede que o jogador saia do corredor e "caia no vazio" aos lados da igreja
        obstaculos.push_back({ -9.4f, -1.2f, 9.4f, 19.4f }); // Bloco maciço esquerdo do túnel
        obstaculos.push_back({ 1.2f,  9.4f, 9.4f, 19.4f }); // Bloco maciço direito do túnel
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

        // Cadastro Lógico das 6 velas correspondentes (para interação, se houver lógica no jogo)
        velas.push_back(Vela(1, -1.0f, 1.0f, -28.0f)); // Altar Esq
        velas.push_back(Vela(2, 1.0f, 1.0f, -28.0f)); // Altar Dir
        velas.push_back(Vela(3, -2.0f, 0.47f, 4.0f));  // Banco Esq
        velas.push_back(Vela(4, 2.0f, 0.47f, 1.0f));  // Banco Dir
        velas.push_back(Vela(5, -6.0f, 0.0f, -20.0f)); // Chão perto do Confessionário
        velas.push_back(Vela(6, -7.5f, 0.0f, -27.5f)); // Chão perto do Relógio
    }

    void atualizar(float deltaTime) {
        for (auto& porta : portas) {
            porta.atualizarAnimacao(deltaTime);
        }
    }

    void interagir(float jogX, float jogY, float jogZ) {
        // Checa proximidade com as portas
        for (auto& porta : portas) {
            float dist = std::sqrt(std::pow(porta.posX - jogX, 2) +
                std::pow(porta.posY - jogY, 2) +
                std::pow(porta.posZ - jogZ, 2));
            if (dist <= porta.raioInteracao) {
                porta.aberta = !porta.aberta;
                return; // Interage com apenas um objeto por clique
            }
        }

        // Checa proximidade com as velas
        for (auto& vela : velas) {
            float dist = std::sqrt(std::pow(vela.posX - jogX, 2) +
                std::pow(vela.posY - jogY, 2) +
                std::pow(vela.posZ - jogZ, 2));
            if (dist <= vela.raioInteracao) {
                vela.acesa = !vela.acesa;
                return; // Interage com apenas um objeto por clique
            }
        }
    }
};

#endif