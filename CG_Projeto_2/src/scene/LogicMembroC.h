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

// ============================================================================
// MIRA: o jogador interage com o objeto que está no CENTRO DA TELA.
// Devolve o cosseno do ângulo entre "para onde o jogador olha" (f, tamanho 1)
// e "do olho até o objeto" (produto escalar). 1.0 = bem no centro da mira.
// Devolve -1 se o objeto está longe demais ou fora do cone (cos < cosMinimo).
// ============================================================================
inline float notaMira(float objX, float objY, float objZ,
                      float jogX, float jogY, float jogZ,
                      float fX, float fY, float fZ,
                      float alcance, float cosMinimo) {
    float dx = objX - jogX, dy = objY - jogY, dz = objZ - jogZ;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (dist > alcance || dist < 0.001f) return -1.0f;

    float cosAngulo = (dx * fX + dy * fY + dz * fZ) / dist;
    return (cosAngulo >= cosMinimo) ? cosAngulo : -1.0f;
}

// Prende v dentro do intervalo [minimo, maximo]. Usado para mirar no ponto da
// PORTA mais perto do jogador (e não só no centro dela, que falha quando se está colado nela).
inline float limitar(float v, float minimo, float maximo) {
    return (v < minimo) ? minimo : (v > maximo) ? maximo : v;
}

struct Porta {
    int id;
    float posX, posY, posZ;
    float rotInicialY;
    float anguloAtual;
    bool aberta;
    bool trancada = false;   // o gameplay tranca a porta da entrada: aí o E não abre
    float raioInteracao;

    Porta(int id, float x, float y, float z, float rotBase)
        : id(id), posX(x), posY(y), posZ(z), rotInicialY(rotBase),
        anguloAtual(0.0f), aberta(false), raioInteracao(2.5f) {
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
        : id(id), posX(x), posY(y), posZ(z), acesa(false), raioInteracao(2.2f) {
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

        // 7. Colunas do batente da porta da entrada (x de 1.0 a 1.2 de cada lado, em z = 9.4)
        obstaculos.push_back({ -1.2f, -1.0f, 9.275f, 9.525f });
        obstaculos.push_back({  1.0f,  1.2f, 9.275f, 9.525f });

        // 8. Castiçais no chão (velas 5 e 6): caixinha de 30 cm em volta do pé
        obstaculos.push_back({ -6.15f, -5.85f, -20.15f, -19.85f });
        obstaculos.push_back({ -7.65f, -7.35f, -27.65f, -27.35f });

        // (As folhas da porta da entrada mudam de lugar ao abrir: a colisão
        //  delas fica em GerenciadorCena::colisaoPortaEntrada)
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
        // Portas laterais desativadas: nunca foram desenhadas, então o E abria uma porta invisível
        // portas.push_back(Porta(2, -9.4f, 0.0f, -10.0f, 90.0f)); // Parede Esquerda
        // portas.push_back(Porta(3, 9.4f, 0.0f, -15.0f, -90.0f)); // Parede Direita

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

    // Procura o que está no centro da mira: a porta da entrada (se não estiver
    // trancada) ou uma vela ACESA. Devolve a nota do melhor alvo (-1 = nenhum).
    float procurarAlvo(float jogX, float jogY, float jogZ, float fX, float fY, float fZ,
                       Porta*& portaAlvo, Vela*& velaAlvo) {
        float melhor = -1.0f;
        portaAlvo = nullptr;
        velaAlvo = nullptr;

        for (auto& porta : portas) {
            if (porta.trancada) continue;
            // ponto da porta mais perto do jogador (a porta vai de x-0.9 a x+0.9 e tem 2,8 m de altura)
            float pontoX = limitar(jogX, porta.posX - 0.9f, porta.posX + 0.9f);
            float pontoY = limitar(jogY, porta.posY + 0.3f, porta.posY + 2.5f);
            float nota = notaMira(pontoX, pontoY, porta.posZ,
                                  jogX, jogY, jogZ, fX, fY, fZ, porta.raioInteracao, 0.80f);
            if (nota > melhor) { melhor = nota; portaAlvo = &porta; velaAlvo = nullptr; }
        }

        for (auto& vela : velas) {
            if (!vela.acesa) continue;   // a varinha só apaga: vela apagada não é alvo
            // mira na chama (~0,55 m acima da base do castiçal)
            float nota = notaMira(vela.posX, vela.posY + 0.55f, vela.posZ,
                                  jogX, jogY, jogZ, fX, fY, fZ, vela.raioInteracao, 0.94f);
            if (nota > melhor) { melhor = nota; velaAlvo = &vela; portaAlvo = nullptr; }
        }

        return melhor;
    }

    // Colisão da porta da entrada (z = 9.4). Mesmo esquema do SistemaColisao:
    // testa cada eixo separado, assim o jogador desliza em vez de travar.
    //  - fechada (< 45 graus): uma placa tapa o vão da porta
    //  - aberta: as duas folhas ficam em pé PARA DENTRO da igreja
    //    (x = -1 e x = +1, de z = 8.4 até 9.4) e viram dois obstáculos finos
    void colisaoPortaEntrada(float antX, float antZ, float& novoX, float& novoZ, float raio = 0.2f) const {
        const Porta& p = portas[0];
        AABB pecas[2];
        int n;

        if (p.anguloAtual < 45.0f) {
            pecas[0] = { -1.0f, 1.0f, 9.3f, 9.5f };
            n = 1;
        } else {
            pecas[0] = { -1.07f, -0.93f, 8.4f, 9.4f };
            pecas[1] = {  0.93f,  1.07f, 8.4f, 9.4f };
            n = 2;
        }

        for (int i = 0; i < n; i++) {
            if (pecas[i].colide(antX, antZ, raio)) continue;   // fechou em cima do jogador: deixa sair
            if (pecas[i].colide(novoX, antZ, raio)) novoX = antX;
            if (pecas[i].colide(antX, novoZ, raio)) novoZ = antZ;
        }
    }
};

#endif