#pragma once
#include <GL/freeglut.h>
#include <render/ModelosMembroC.h>

inline void desenharChao(GLint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não alterar a textura

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f); // Normal apontando para cima
        // Coordenadas UV dobradas para a textura não esticar
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-9.4f, 0.0f, -29.4f);
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 9.4f, 0.0f, -29.4f);
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 9.4f, 0.0f,  9.4f);
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-9.4f, 0.0f,  9.4f);
    glEnd();
}

inline void desenharParedes(GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBegin(GL_QUADS);

    // Parede Esquerda (X = -9.4f)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-9.4f, 0.0f, -29.4f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(-9.4f, 0.0f,  9.4f);
    glTexCoord2f(10.0f, 3.0f); glVertex3f(-9.4f, 10.0f, 9.4f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-9.4f, 10.0f, -29.4f);

    // Parede Direita (X = 9.4f)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 9.4f, 0.0f,  9.4f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f( 9.4f, 0.0f, -29.4f);
    glTexCoord2f(10.0f, 3.0f); glVertex3f( 9.4f, 10.0f, -29.4f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f( 9.4f, 10.0f,  9.4f);

    // Parede Frontal / Altar (Z = -29.4f)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 9.4f, 0.0f, -29.4f);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(-9.4f, 0.0f, -29.4f);
    glTexCoord2f(5.0f, 3.0f); glVertex3f(-9.4f, 10.0f, -29.4f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f( 9.4f, 10.0f, -29.4f);

    // Parede Traseira / Entrada (Z = 9.4f)
   // Parede Traseira / Entrada (Z = 9.4f) COM BURACO PARA A PORTA
    glNormal3f(0.0f, 0.0f, -1.0f);

    // Pedaço Esquerdo (-9.4 até -1.2)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-9.4f, 0.0f, 9.4f);
    glTexCoord2f(2.2f, 0.0f); glVertex3f(-1.2f, 0.0f, 9.4f);
    glTexCoord2f(2.2f, 3.0f); glVertex3f(-1.2f, 10.0f, 9.4f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-9.4f, 10.0f, 9.4f);

    // Pedaço Direito (1.2 até 9.4)
    glTexCoord2f(2.8f, 0.0f); glVertex3f(1.2f, 0.0f, 9.4f);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(9.4f, 0.0f, 9.4f);
    glTexCoord2f(5.0f, 3.0f); glVertex3f(9.4f, 10.0f, 9.4f);
    glTexCoord2f(2.8f, 3.0f); glVertex3f(1.2f, 10.0f, 9.4f);

    // Pedaço Superior (acima da porta, Y = 3.1 até 10.0)
    glTexCoord2f(2.2f, 0.93f); glVertex3f(-1.2f, 3.1f, 9.4f);
    glTexCoord2f(2.8f, 0.93f); glVertex3f(1.2f, 3.1f, 9.4f);
    glTexCoord2f(2.8f, 3.00f); glVertex3f(1.2f, 10.0f, 9.4f);
    glTexCoord2f(2.2f, 3.00f); glVertex3f(-1.2f, 10.0f, 9.4f);

    glEnd();
}

inline void desenharCorredor(GLuint texParede, GLuint texChao, GLuint texTeto) {
    // Chão do corredor (Avançando 10 metros para trás: Z de 9.4f até 19.4f)
    glBindTexture(GL_TEXTURE_2D, texChao);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.2f, 0.0f, 9.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2f, 0.0f, 9.4f);
    glTexCoord2f(1.0f, 3.0f); glVertex3f(1.2f, 0.0f, 19.4f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-1.2f, 0.0f, 19.4f);
    glEnd();

    // Teto do corredor (Y = 3.1f)
    glBindTexture(GL_TEXTURE_2D, texTeto);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.2f, 3.1f, 19.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2f, 3.1f, 19.4f);
    glTexCoord2f(1.0f, 3.0f); glVertex3f(1.2f, 3.1f, 9.4f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-1.2f, 3.1f, 9.4f);
    glEnd();

    // Paredes Laterais do corredor
    glBindTexture(GL_TEXTURE_2D, texParede);
    glBegin(GL_QUADS);

    // Parede Esquerda (X = -1.2f)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.2f, 0.0f, 19.4f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(-1.2f, 0.0f, 9.4f);
    glTexCoord2f(3.0f, 1.0f); glVertex3f(-1.2f, 3.1f, 9.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.2f, 3.1f, 19.4f);

    // Parede Direita (X = 1.2f)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.2f, 0.0f, 9.4f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(1.2f, 0.0f, 19.4f);
    glTexCoord2f(3.0f, 1.0f); glVertex3f(1.2f, 3.1f, 19.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.2f, 3.1f, 9.4f);

    glEnd();
}

inline void desenharTeto(GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-9.4f, 10.0f,  9.4f);
    glTexCoord2f(5.0f, 0.0f); glVertex3f( 9.4f, 10.0f,  9.4f);
    glTexCoord2f(5.0f, 10.0f); glVertex3f( 9.4f, 10.0f, -29.4f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-9.4f, 10.0f, -29.4f);
    glEnd();
}


inline void peca(float x, float y, float z, float largura, float altura, float profundidade) {
	glPushMatrix();

	glTranslated(x, y, z);
	glScalef(largura, altura, profundidade);
	
	desenharCuboTexturizado(1.0f);
	glPopMatrix();	
}

// Banco de igreja de comprimento qualquer. Frente voltada para -Z (o altar).
inline void desenharBanco(float posX, float posY, float posZ, float rotY,
    float comprimento, GLuint textura) {
    float C = comprimento;
    float meio = C / 2.0f;

    glBindTexture(GL_TEXTURE_2D, textura);
    glColor3f(0.75f, 0.55f, 0.4f);                   // madeira escurecida

    glPushMatrix();
    glTranslatef(posX, posY, posZ);                  // banco inteiro na cena
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    peca(0.0f, 0.45f, 0.0f, C, 0.05f, 0.40f);       // assento contínuo
    peca(0.0f, 0.12f, 0.0f, C - 0.10f, 0.05f, 0.05f); // travessa de baixo

    // Laterais só nas pontas
    peca(-meio + 0.025f, 0.35f, 0.02f, 0.05f, 0.70f, 0.45f);
    peca(meio - 0.025f, 0.35f, 0.02f, 0.05f, 0.70f, 0.45f);

    // Apoios intermediários: um a cada ~2 m
    int n = (int)(C / 2.0f) - 1;                     // 6 m → 2 apoios
    for (int k = 1; k <= n; ++k) {
        float x = -meio + k * C / (n + 1);           // divide o comprimento em partes iguais
        peca(x, 0.22f, 0.02f, 0.04f, 0.44f, 0.35f);
    }

    // Encosto: gira 10° em torno da borda de baixo
    glPushMatrix();
    glTranslatef(0.0f, 0.475f, 0.18f);               // pivô na dobra
    glRotatef(10.0f, 1.0f, 0.0f, 0.0f);              // inclina para trás
    peca(0.0f, 0.275f, 0.0f, C - 0.05f, 0.55f, 0.04f);

    // Porta-livros: preso no ENCOSTO, herda a inclinação dele (encadeado)
    peca(0.0f, 0.30f, 0.07f, C - 0.10f, 0.03f, 0.10f); // prateleira atrás do encosto
    peca(0.0f, 0.34f, 0.115f, C - 0.10f, 0.08f, 0.01f); // borda que segura os livros
    glPopMatrix();

    glPopMatrix();
}

inline void desenharGruposBancos(GLuint textura) {
    int profundidadeLinhas = 8; // Quantidade de linhas em direção ao altar

    for (int i = 0; i < profundidadeLinhas; ++i) {
        // Começa em Z = 4.0f (perto da porta) e avança para o fundo a cada 3.0 metros
        float z = 4.0f - (i * 3.0f);

        desenharBanco(-4.0f, 0.0f, z, 0.0f, 6.0f, textura);   // grupo esquerdo: x de -7 a -1
        desenharBanco(4.0f, 0.0f, z, 0.0f, 6.0f, textura);   // grupo direito:  x de  1 a  7
    }
}


// Uma folha da porta, desenhada a partir da dobradiça (origem local).
// lado = +1: sai para +X (folha esquerda); lado = -1: sai para -X (direita).
// +Z = lado de FORA da igreja; -Z = lado de DENTRO.
inline void folhaPorta(float lado, GLuint texMadeira, GLuint texMetal, GLuint texBranca) {
    float l = lado;
    const float PI = 3.14159265f;

    // --- 5 pranchas verticais, tons alternados ---
    glBindTexture(GL_TEXTURE_2D, texMadeira);
    for (int p = 0; p < 5; ++p) {
        if (p % 2 == 0) glColor3f(0.50f, 0.32f, 0.22f);
        else            glColor3f(0.43f, 0.27f, 0.18f);
        float x = l * (0.10f + p * 0.20f);                 // centro da prancha p
        peca(x, 1.40f, 0.0f, 0.195f, 2.80f, 0.06f);       // fresta de 5 mm entre pranchas
    }

    // --- Estrutura em "Z" no lado de dentro: 2 travessas + diagonal ---
    glColor3f(0.38f, 0.24f, 0.16f);
    peca(l * 0.50f, 0.45f, -0.05f, 0.90f, 0.15f, 0.04f);  // travessa de baixo
    peca(l * 0.50f, 2.35f, -0.05f, 0.90f, 0.15f, 0.04f);  // travessa de cima

    float dx = l * 0.76f, dy = 1.76f;                      // de (0,12; 0,52) a (0,88; 2,28)
    float comp = std::sqrt(dx * dx + dy * dy);             // ≈ 1,92 m
    float ang = std::atan2(dy, dx) * 180.0f / PI;         // ≈ 66,6° (ou 113,4° espelhado)
    glPushMatrix();
    glTranslatef(l * 0.50f, 1.40f, -0.05f);                // centro da diagonal
    glRotatef(ang, 0.0f, 0.0f, 1.0f);                      // deita a peça na direção certa
    peca(0.0f, 0.0f, 0.0f, comp, 0.12f, 0.04f);           // comprimento ao longo de X
    glPopMatrix();

    // --- Ferragens longas no lado de fora ---
    glBindTexture(GL_TEXTURE_2D, texMetal);
    glColor3f(0.25f, 0.25f, 0.25f);                        // ferro escuro
    peca(l * 0.40f, 0.45f, 0.04f, 0.75f, 0.07f, 0.015f);
    peca(l * 0.40f, 2.35f, 0.04f, 0.75f, 0.07f, 0.015f);

    // --- Peças do GLUT (sem UV): textura branca + cor ---
    glBindTexture(GL_TEXTURE_2D, texBranca);
    glColor3f(0.2f, 0.2f, 0.2f);

    // Dobradiças: cilindros em pé na borda da dobradiça
    for (float y : { 0.45f, 2.35f }) {
        glPushMatrix();
        glTranslatef(0.0f, y - 0.10f, 0.04f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);               // +Z → +Y
        glutSolidCylinder(0.025, 0.20, 10, 1);
        glPopMatrix();
    }

    // Aldrava (lado de fora): anel + pino de fixação
    glPushMatrix();
    glTranslatef(l * 0.85f, 1.25f, 0.06f);
    glutSolidTorus(0.012, 0.07, 8, 20);                    // anel em pé, virado para fora
    glPopMatrix();
    glPushMatrix();
    glTranslatef(l * 0.85f, 1.32f, 0.04f);
    glutSolidSphere(0.025, 10, 8);
    glPopMatrix();

    // Puxador (lado de dentro): barra vertical + 2 suportes, todos cilindros
    glPushMatrix();
    glTranslatef(l * 0.85f, 1.00f, -0.09f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);                   // barra em pé
    glutSolidCylinder(0.015, 0.30, 10, 1);
    glPopMatrix();
    for (float y : { 1.03f, 1.27f }) {
        glPushMatrix();
        glTranslatef(l * 0.85f, y, -0.03f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);               // +Z → -Z (para dentro)
        glutSolidCylinder(0.01, 0.06, 8, 1);
        glPopMatrix();
    }
}

// Portas duplas da entrada, com batente. angulo vem do C: 0 = fechada, 90 = aberta (-Z)
inline void desenharPortasDuplas(float posX, float posY, float posZ, float rotY, float angulo,
    GLuint texMadeira, GLuint texMetal, GLuint texBranca) {
    // Abertura suave (smoothstep): só muda COMO o ângulo do C aparece
    float t = angulo / 90.0f;
    angulo = 90.0f * t * t * (3.0f - 2.0f * t);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // Batente fixo: duas colunas + verga
    glBindTexture(GL_TEXTURE_2D, texMadeira);
    glColor3f(0.30f, 0.19f, 0.13f);
    peca(-1.10f, 1.45f, 0.0f, 0.20f, 2.90f, 0.25f);
    peca(1.10f, 1.45f, 0.0f, 0.20f, 2.90f, 0.25f);
    peca(0.00f, 2.95f, 0.0f, 2.40f, 0.20f, 0.25f);

    // Folha esquerda: dobradiça em x = -1, gira +angulo
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);
    glRotatef(angulo, 0.0f, 1.0f, 0.0f);
    folhaPorta(+1.0f, texMadeira, texMetal, texBranca);
    glPopMatrix();

    // Folha direita: dobradiça em x = +1, gira -angulo (espelhada)
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(-angulo, 0.0f, 1.0f, 0.0f);
    folhaPorta(-1.0f, texMadeira, texMetal, texBranca);
    glPopMatrix();

    glPopMatrix();
}

// Textura de 1 pixel branco: com ela, o shader mostra só a cor do glColor
inline GLuint criarTexturaBranca() {
    unsigned char branco[3] = { 255, 255, 255 };
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, branco);
    return id;
}

// Lustre pendente: (posX, posY, posZ) é o ponto de fixação no TETO.
// tempo em segundos (balanço); escala: 1 = tamanho base, 2 = dobro.
inline void desenharLustre(float posX, float posY, float posZ, float tempo, float escala,
    GLuint texBranca, bool acesas = true) {
    const int   N = 8;            // braços, segmentos do aro e velas
    const float R = 0.6f;         // raio do aro no tamanho base (m)
    const float PI = 3.14159265f;
    const float yAro = -1.7f;     // altura do aro abaixo do teto (tamanho base)

    glBindTexture(GL_TEXTURE_2D, texBranca);    // formas do GLUT não têm UV: só cor

    glPushMatrix();
    glTranslatef(posX, posY, posZ);             // ponto fixo no teto

    float balanco = 3.0f * std::sin(1.2f * tempo);   // θ = A·sen(ωt)
    glRotatef(balanco, 0.0f, 0.0f, 1.0f);            // 1 giro move a árvore toda
    glScalef(escala, escala, escala);                // 1 escala dobra a árvore toda

    // --- Corrente: 7 elos em anel, alternando 90°, encaixados ---
    glColor3f(0.35f, 0.3f, 0.22f);                   // ferro envelhecido
    for (int k = 0; k < 7; ++k) {
        glPushMatrix();
        glTranslatef(0.0f, -0.11f - k * 0.22f, 0.0f);    // um elo a cada 22 cm
        glRotatef(k * 90.0f, 0.0f, 1.0f, 0.0f);          // alterna a orientação
        glScalef(1.0f, 1.6f, 1.0f);                      // elo oval (mais alto que largo)
        glutSolidTorus(0.015, 0.07, 8, 16);              // tubo de 1,5 cm, raio de 7 cm
        glPopMatrix();
    }

    // Tudo abaixo fica preso ao fim da corrente
    glTranslatef(0.0f, yAro, 0.0f);

    glColor3f(0.55f, 0.45f, 0.25f);                  // latão escurecido
    glutSolidSphere(0.12, 16, 12);                   // esfera central

    // Pingente: cone apontando para baixo
    glPushMatrix();
    glTranslatef(0.0f, -0.08f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);              // +Z → -Y
    glutSolidCone(0.08, 0.3, 12, 1);
    glPopMatrix();

    float comprimento = 2.0f * R * std::sin(PI / N); // corda entre dois braços

    for (int i = 0; i < N; ++i) {
        glPushMatrix();
        glRotatef(i * 360.0f / N, 0.0f, 1.0f, 0.0f); // direção do braço i

        // Braço: cilindro do centro até o aro
        glColor3f(0.55f, 0.45f, 0.25f);
        glPushMatrix();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);          // +Z → +X
        glutSolidCylinder(0.018, R, 8, 1);
        glPopMatrix();

        // Segmento do aro: cilindro deitado na tangente (Z), centrado no ponto R
        glPushMatrix();
        glTranslatef(R, 0.0f, -comprimento / 2.0f);
        glutSolidCylinder(0.025, comprimento, 8, 1);
        glPopMatrix();

        // Base da vela: esfera pequena sobre o aro
        glPushMatrix();
        glTranslatef(R, 0.03f, 0.0f);
        glutSolidSphere(0.035, 10, 8);
        glPopMatrix();

        // Vela: cilindro de cera em pé
        glColor3f(0.95f, 0.9f, 0.75f);
        glPushMatrix();
        glTranslatef(R, 0.05f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);         // +Z → +Y
        glutSolidCylinder(0.025, 0.18, 10, 1);
        glPopMatrix();

        // Chama: esfera alongada e EMISSIVA (brilha sozinha). Apagada: não desenha.
        if (acesas) {
            GLfloat emissaoChama[] = { 1.0f, 0.6f, 0.15f, 1.0f };
            GLfloat semEmissao[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissaoChama);
            glColor3f(1.0f, 0.75f, 0.2f);
            glPushMatrix();
            glTranslatef(R, 0.27f, 0.0f);
            glScalef(1.0f, 1.8f, 1.0f);
            glutSolidSphere(0.022, 10, 8);
            glPopMatrix();
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, semEmissao);   // não vaza para o resto
        }

        glPopMatrix();
    }

    glPopMatrix();
}
