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

inline void desenharBanco(float posX, float posY, float posZ, float rotY, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);

	glColor3f(0.75f, 0.55f, 0.4f);

	glColor3f(0.75f, 0.55f, 0.4f);                  // tinge a madeira para um tom mais escuro

	glPushMatrix();
	glTranslatef(posX, posY, posZ);                 // posição do banco inteiro na cena
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);

	peca(0.0f, 0.45f, 0.0f, 2.0f, 0.05f, 0.40f);   // assento
	peca(-0.98f, 0.35f, 0.02f, 0.05f, 0.70f, 0.45f);  // lateral esquerda
	peca(0.98f, 0.35f, 0.02f, 0.05f, 0.70f, 0.45f);  // lateral direita
	peca(0.0f, 0.12f, 0.0f, 1.90f, 0.05f, 0.05f);  // travessa de baixo

	// Encosto: gira 10° em torno da sua borda de baixo
	glPushMatrix();
	glTranslatef(0.0f, 0.475f, 0.18f);              // vai até a dobra (borda de trás do assento)
	glRotatef(10.0f, 1.0f, 0.0f, 0.0f);             // inclina para trás (+Z)
	peca(0.0f, 0.275f, 0.0f, 1.95f, 0.55f, 0.04f); // sobe metade da altura
	glPopMatrix();

	glPopMatrix();

}

inline void desenharGruposBancos(GLuint textura) {
    int profundidadeLinhas = 8; // Quantidade de linhas em direção ao altar

    for (int i = 0; i < profundidadeLinhas; ++i) {
        // Começa em Z = 4.0f (perto da porta) e avança para o fundo a cada 3.0 metros
        float z = 4.0f - (i * 3.0f);

        // 3 Bancos juntos à esquerda (Centros: -2.0, -4.0, -6.0)
        desenharBanco(-2.0f, 0.0f, z, 0.0f, textura);
        desenharBanco(-4.0f, 0.0f, z, 0.0f, textura);
        desenharBanco(-6.0f, 0.0f, z, 0.0f, textura);

        // 3 Bancos juntos à direita (Centros: 2.0, 4.0, 6.0)
        desenharBanco(2.0f, 0.0f, z, 0.0f, textura);
        desenharBanco(4.0f, 0.0f, z, 0.0f, textura);
        desenharBanco(6.0f, 0.0f, z, 0.0f, textura);
    }
}


// Uma folha da porta, desenhada a partir da dobradiça (origem local).
// lado = +1: a folha sai para +X (folha esquerda); lado = -1: sai para -X (folha direita)
inline void folhaPorta(float lado, GLuint texMadeira, GLuint texMetal) {
    float l = lado;   // só para encurtar

    // Madeira: placa principal + 2 almofadas em relevo (dos dois lados)
    glBindTexture(GL_TEXTURE_2D, texMadeira);
    glColor3f(0.55f, 0.35f, 0.25f);
    peca(l * 0.50f, 1.40f, 0.0f, 1.00f, 2.80f, 0.08f);   // placa (centro a meia largura da dobradiça)
    glColor3f(0.45f, 0.28f, 0.2f);
    peca(l * 0.50f, 0.80f, 0.0f, 0.70f, 0.90f, 0.12f);   // almofada de baixo
    peca(l * 0.50f, 2.00f, 0.0f, 0.70f, 1.00f, 0.12f);   // almofada de cima

    // Metal: 3 dobradiças na borda da dobradiça + maçaneta perto do centro
    glBindTexture(GL_TEXTURE_2D, texMetal);
    glColor3f(0.6f, 0.6f, 0.6f);
    peca(l * 0.03f, 0.40f, 0.0f, 0.06f, 0.20f, 0.10f);   // dobradiça de baixo
    peca(l * 0.03f, 1.40f, 0.0f, 0.06f, 0.20f, 0.10f);   // dobradiça do meio
    peca(l * 0.03f, 2.40f, 0.0f, 0.06f, 0.20f, 0.10f);   // dobradiça de cima
    peca(l * 0.90f, 1.10f, -0.08f, 0.05f, 0.25f, 0.05f);   // maçaneta (lado de dentro, -Z)
    peca(l * 0.90f, 1.10f, 0.08f, 0.05f, 0.25f, 0.05f);   // maçaneta (lado de fora, +Z)
}

// Portas duplas da entrada, com batente. angulo: 0 = fechada, 90 = aberta para -Z
inline void desenharPortasDuplas(float posX, float posY, float posZ, float rotY,
    float angulo, GLuint texMadeira, GLuint texMetal) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // Batente (fixo): duas colunas + verga em cima
    glBindTexture(GL_TEXTURE_2D, texMadeira);
    glColor3f(0.35f, 0.22f, 0.15f);
    peca(-1.10f, 1.45f, 0.0f, 0.20f, 2.90f, 0.25f);   // coluna esquerda
    peca(1.10f, 1.45f, 0.0f, 0.20f, 2.90f, 0.25f);   // coluna direita
    peca(0.00f, 2.95f, 0.0f, 2.40f, 0.20f, 0.25f);   // verga

    // Folha esquerda: dobradiça em x = -1, gira +angulo
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);          // 1. vai até a dobradiça
    glRotatef(angulo, 0.0f, 1.0f, 0.0f);      // 2. gira em torno dela
    folhaPorta(+1.0f, texMadeira, texMetal);  // 3. folha sai para +X
    glPopMatrix();

    // Folha direita: dobradiça em x = +1, gira -angulo (espelhada)
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(-angulo, 0.0f, 1.0f, 0.0f);
    folhaPorta(-1.0f, texMadeira, texMetal);  // folha sai para -X
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
// tempo em segundos, para o balanço.
inline void desenharLustre(float posX, float posY, float posZ, float tempo,
    GLuint texMetal, GLuint texBranca) {
    const int   N = 8;          // braços, segmentos do aro e velas
    const float R = 0.6f;       // raio do aro (m)
    const float PI = 3.14159265f;

    glPushMatrix();
    glTranslatef(posX, posY, posZ);                   // ponto fixo no teto

    // BALANÇO: um único giro no topo move TODA a árvore abaixo
    float balanco = 3.0f * std::sin(1.2f * tempo);    // θ = A·sen(ωt), em graus
    glRotatef(balanco, 0.0f, 0.0f, 1.0f);

    // --- Corrente: 6 elos de 30 cm, alternando a orientação ---
    glBindTexture(GL_TEXTURE_2D, texMetal);
    glColor3f(0.45f, 0.4f, 0.3f);                     // metal escuro, envelhecido
    for (int k = 0; k < 6; ++k) {
        glPushMatrix();
        glTranslatef(0.0f, -0.15f - k * 0.3f, 0.0f);  // centro do elo k
        glRotatef(k * 90.0f, 0.0f, 1.0f, 0.0f);       // elos alternam 90°, como numa corrente real
        peca(0.0f, 0.0f, 0.0f, 0.08f, 0.28f, 0.02f);
        glPopMatrix();
    }

    // Tudo abaixo fica preso ao fim da corrente (1,8 m abaixo do teto)
    glTranslatef(0.0f, -1.8f, 0.0f);
    peca(0.0f, -0.08f, 0.0f, 0.18f, 0.16f, 0.18f);   // cubo central

    for (int i = 0; i < N; ++i) {
        glPushMatrix();
        glRotatef(i * 360.0f / N, 0.0f, 1.0f, 0.0f);  // gira até a direção do braço i

        // Braço: do centro até o aro, ao longo de +X
        glBindTexture(GL_TEXTURE_2D, texMetal);
        glColor3f(0.45f, 0.4f, 0.3f);
        peca(R / 2.0f, -0.1f, 0.0f, R, 0.03f, 0.03f);

        // Segmento do aro: na distância R, deitado na direção tangente (Z)
        float comprimento = 2.0f * R * std::sin(PI / N);   // corda entre dois braços
        peca(R, -0.1f, 0.0f, 0.04f, 0.04f, comprimento);

        // Vela na ponta do braço, apoiada no aro
        glBindTexture(GL_TEXTURE_2D, texBranca);
        glColor3f(0.95f, 0.9f, 0.75f);                     // cera creme
        peca(R, 0.02f, 0.0f, 0.05f, 0.20f, 0.05f);
        glColor3f(1.0f, 0.75f, 0.2f);                      // chama (o B pode dar brilho)
        peca(R, 0.15f, 0.0f, 0.025f, 0.05f, 0.025f);

        glPopMatrix();
    }

    glPopMatrix();
}
