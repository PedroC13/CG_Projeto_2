#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>                // glutGet (tempo da tremulação das velas)
#include "ShaderMembroA.h"            // reaproveita o compilarShader() do Membro A
#include "../scene/LogicMembroC.h"    // struct Vela (posição e estado das velas)
#include <iostream>
#include <vector>

// ============================================================================
// SHADER DO MEMBRO B: BLINN-PHONG + EMISSÃO
// ----------------------------------------------------------------------------
// Usa o estado do pipeline fixo (GLSL 120), então ilumina TODOS os objetos
// (glBegin, glutSolid*, gluCylinder...) sem precisar mudar o código deles:
//   - luzes:     glLightfv(...)        -> gl_LightSource[i]
//   - materiais: glMaterialfv(...)     -> gl_FrontMaterial (especular, brilho, emissão)
//   - cor:       glColor(...)          -> gl_Color
//
// Luzes usadas (o OpenGL tem 8 slots: GL_LIGHT0 a GL_LIGHT7):
//   GL_LIGHT0      luar (direcional, fraco, azulado)
//   GL_LIGHT1      lamparina na mão (point light, luz espalhada) -> tecla F
//   GL_LIGHT2..7   as 6 velas dos castiçais (point lights) -> clique / E
// Lustres: os 8 slots já estão ocupados, então cada lustre vira UMA point light
// extra (no centro do aro de velas), enviada ao shader por uniform.
// ============================================================================

const int NUM_VELAS_B = 6;
const float ALTURA_CHAMA_B = 0.6f;     // chama fica ~0.6 acima da base do castiçal
const int NUM_LUSTRES_B = 4;

// ---------------------------------------------------------------------------
// VERTEX SHADER: roda uma vez por vértice
// Passa posição e normal no ESPAÇO DO OLHO (câmera na origem) para o fragment.
// ---------------------------------------------------------------------------
const char* codigoVertexB = R"(
#version 120
varying vec3 vPos;   // posição no espaço do olho
varying vec3 vN;     // normal no espaço do olho
varying vec2 vUV;

void main() {
    vPos = vec3(gl_ModelViewMatrix * gl_Vertex);
    vN   = gl_NormalMatrix * gl_Normal;     // inversa-transposta: corrige o glScalef
    vUV  = gl_MultiTexCoord0.xy;
    gl_FrontColor = gl_Color;
    gl_BackColor  = gl_Color;
    gl_Position   = ftransform();
}
)";

// ---------------------------------------------------------------------------
// FRAGMENT SHADER: roda uma vez por pixel
// cor = emissão + ambiente + soma(Blinn-Phong de cada luz)
// ---------------------------------------------------------------------------
const char* codigoFragmentB = R"(
#version 120
varying vec3 vPos;
varying vec3 vN;
varying vec2 vUV;

uniform sampler2D textura;          // unidade 0
uniform float velaAcesa[6];         // 1.0 = acesa, 0.0 = apagada
uniform float lanternaLigada;       // 1.0 / 0.0 (lamparina na mão)
uniform float tempo;                // segundos (tremulação das velas)
uniform vec3  lustrePos[4];         // centro de cada lustre, no ESPAÇO DO OLHO
uniform float lustresAcesos;        // 1.0 / 0.0

// Blinn-Phong de UMA luz
//   posLuz.w == 0 -> direcional | posLuz.w == 1 -> point/spot
//   spotCos == -1 -> não é spot
vec3 blinnPhong(vec4 posLuz, vec3 corLuz, float kc, float kl, float kq,
                vec3 spotDir, float spotCos, float spotExp, float intensidade,
                vec3 N, vec3 V, vec3 albedo) {
    if (intensidade <= 0.0) return vec3(0.0);

    vec3 L;
    float aten = 1.0;

    if (posLuz.w == 0.0) {
        L = normalize(posLuz.xyz);                          // direcional
    } else {
        vec3 d = posLuz.xyz - vPos;
        float dist = length(d);
        L = d / dist;
        aten = 1.0 / (kc + kl * dist + kq * dist * dist);  // atenuação

        if (spotCos > -1.0) {                               // spotlight
            float cosAng = dot(-L, normalize(spotDir));
            float borda  = smoothstep(spotCos, spotCos + 0.04, cosAng); // borda suave
            aten *= borda * pow(max(cosAng, 0.0), spotExp);
        }
    }

    vec3 H = normalize(L + V);                              // half-vector (o "Blinn")
    float difusa  = max(dot(N, L), 0.0);
    float brilho  = max(gl_FrontMaterial.shininess, 1.0);
    float especular = (difusa > 0.0) ? pow(max(dot(N, H), 0.0), brilho) : 0.0;

    vec3 dif = corLuz * difusa * albedo;
    vec3 spc = corLuz * especular * gl_FrontMaterial.specular.rgb;
    return intensidade * aten * (dif + spc);
}

void main() {
    vec3 N = normalize(vN);
    vec3 V = normalize(-vPos);                              // câmera está na origem

    vec4 texel  = texture2D(textura, vUV);
    vec3 albedo = texel.rgb * gl_Color.rgb;

    // Emissão (chamas, vidro da lanterna) + ambiente global
    vec3 cor = gl_FrontMaterial.emission.rgb + gl_LightModel.ambient.rgb * albedo;

    // GL_LIGHT0: luar
    cor += blinnPhong(gl_LightSource[0].position, gl_LightSource[0].diffuse.rgb,
                      1.0, 0.0, 0.0, vec3(0.0), -1.0, 0.0, 1.0, N, V, albedo);

    // GL_LIGHT1: lamparina (point light: espalha luz para todos os lados)
    float tl = 0.92 + 0.08 * sin(tempo * 8.0) + 0.04 * sin(tempo * 17.0 + 0.6); // chama tremendo
    cor += blinnPhong(gl_LightSource[1].position, gl_LightSource[1].diffuse.rgb,
                      gl_LightSource[1].constantAttenuation,
                      gl_LightSource[1].linearAttenuation,
                      gl_LightSource[1].quadraticAttenuation,
                      vec3(0.0), -1.0, 0.0,                 // -1.0 = não é spot
                      lanternaLigada * tl, N, V, albedo);

    // GL_LIGHT2..7: velas (com tremulação)
    float t0 = 0.85 + 0.15 * sin(tempo * 11.0);
    float t1 = 0.85 + 0.15 * sin(tempo * 13.0 + 1.7);
    float t2 = 0.85 + 0.15 * sin(tempo * 9.0  + 3.1);
    float t3 = 0.85 + 0.15 * sin(tempo * 12.0 + 4.6);
    float t4 = 0.85 + 0.15 * sin(tempo * 10.0 + 2.3);
    float t5 = 0.85 + 0.15 * sin(tempo * 14.0 + 5.2);

    cor += blinnPhong(gl_LightSource[2].position, gl_LightSource[2].diffuse.rgb,
                      gl_LightSource[2].constantAttenuation, gl_LightSource[2].linearAttenuation,
                      gl_LightSource[2].quadraticAttenuation, vec3(0.0), -1.0, 0.0,
                      velaAcesa[0] * t0, N, V, albedo);
    cor += blinnPhong(gl_LightSource[3].position, gl_LightSource[3].diffuse.rgb,
                      gl_LightSource[3].constantAttenuation, gl_LightSource[3].linearAttenuation,
                      gl_LightSource[3].quadraticAttenuation, vec3(0.0), -1.0, 0.0,
                      velaAcesa[1] * t1, N, V, albedo);
    cor += blinnPhong(gl_LightSource[4].position, gl_LightSource[4].diffuse.rgb,
                      gl_LightSource[4].constantAttenuation, gl_LightSource[4].linearAttenuation,
                      gl_LightSource[4].quadraticAttenuation, vec3(0.0), -1.0, 0.0,
                      velaAcesa[2] * t2, N, V, albedo);
    cor += blinnPhong(gl_LightSource[5].position, gl_LightSource[5].diffuse.rgb,
                      gl_LightSource[5].constantAttenuation, gl_LightSource[5].linearAttenuation,
                      gl_LightSource[5].quadraticAttenuation, vec3(0.0), -1.0, 0.0,
                      velaAcesa[3] * t3, N, V, albedo);
    cor += blinnPhong(gl_LightSource[6].position, gl_LightSource[6].diffuse.rgb,
                      gl_LightSource[6].constantAttenuation, gl_LightSource[6].linearAttenuation,
                      gl_LightSource[6].quadraticAttenuation, vec3(0.0), -1.0, 0.0,
                      velaAcesa[4] * t4, N, V, albedo);
    cor += blinnPhong(gl_LightSource[7].position, gl_LightSource[7].diffuse.rgb,
                      gl_LightSource[7].constantAttenuation, gl_LightSource[7].linearAttenuation,
                      gl_LightSource[7].quadraticAttenuation, vec3(0.0), -1.0, 0.0,
                      velaAcesa[5] * t5, N, V, albedo);

    // LUSTRES: as 8 velas de cada lustre somadas numa luz só, no centro do aro.
    // Atenuação fraca porque estão alto (~7 m): iluminam o salão de leve.
    vec3 corLustre = vec3(1.0, 0.62, 0.28);
    for (int i = 0; i < 4; i++) {
        float tr = 0.9 + 0.1 * sin(tempo * (7.0 + float(i)) + float(i) * 1.3);   // tremulação
        cor += blinnPhong(vec4(lustrePos[i], 1.0), corLustre, 1.0, 0.05, 0.02,
                          vec3(0.0), -1.0, 0.0, lustresAcesos * 0.6 * tr, N, V, albedo);
    }

    gl_FragColor = vec4(cor, texel.a * gl_Color.a);
}
)";

// ============================================================================
// FUNÇÕES DO LADO C++
// ============================================================================

// Compila e linka o programa (mesmo esquema do criarProgramaShader do Membro A)
inline GLuint criarProgramaShaderB() {
    GLuint vs = compilarShader(GL_VERTEX_SHADER, codigoVertexB);
    GLuint fs = compilarShader(GL_FRAGMENT_SHADER, codigoFragmentB);

    GLuint programa = glCreateProgram();
    glAttachShader(programa, vs);
    glAttachShader(programa, fs);
    glLinkProgram(programa);

    GLint linkado;
    glGetProgramiv(programa, GL_LINK_STATUS, &linkado);
    if (!linkado) {
        char log[1024];
        glGetProgramInfoLog(programa, 1024, nullptr, log);
        std::cout << "erro no link do shader B: " << log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return programa;
}

// Cores e alcance das luzes. Chamar UMA vez no setup().
inline void configurarLuzesB() {
    // Ambiente global bem baixo: igreja escura (clima de terror)
    GLfloat ambiente[] = { 0.07f, 0.07f, 0.09f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiente);

    // GL_LIGHT0: luar entrando pelos vitrais (fraco e azulado)
    GLfloat luar[] = { 0.10f, 0.12f, 0.20f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luar);

    // GL_LIGHT1: lamparina na mão (amarelo quente, luz espalhada, alcance ~6 m)
    GLfloat lamparina[] = { 1.0f, 0.78f, 0.45f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lamparina);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0f);           // 180 = sem cone (point light)
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.12f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.06f);

    // GL_LIGHT2..7: velas (laranja, alcance curto de ~3-4 m)
    GLfloat vela[] = { 1.0f, 0.55f, 0.20f, 1.0f };
    for (int i = 0; i < NUM_VELAS_B; i++) {
        GLenum luz = GL_LIGHT2 + i;
        glLightfv(luz, GL_DIFFUSE, vela);
        glLightf(luz, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(luz, GL_LINEAR_ATTENUATION, 0.25f);
        glLightf(luz, GL_QUADRATIC_ATTENUATION, 0.30f);
    }
}

// Lamparina: chamar ANTES do cameraAplicar(), com a matriz identidade.
// A posição fica no espaço do olho, então a luz ACOMPANHA O JOGADOR sozinha.
// Fica onde a lamparina é desenhada (canto inferior direito, à frente).
inline void posicionarLanternaB() {
    GLfloat pos[] = { 0.28f, -0.25f, -0.55f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, pos);
}

// Luar e velas: chamar DEPOIS do cameraAplicar(). Ficam fixos no mundo.
inline void posicionarLuzesMundoB(const std::vector<Vela>& velas) {
    GLfloat luar[] = { -0.4f, 1.0f, 0.3f, 0.0f };  // w = 0: direcional
    glLightfv(GL_LIGHT0, GL_POSITION, luar);

    for (int i = 0; i < NUM_VELAS_B && i < (int)velas.size(); i++) {
        GLfloat pos[] = { velas[i].posX, velas[i].posY + ALTURA_CHAMA_B, velas[i].posZ, 1.0f };
        glLightfv(GL_LIGHT2 + i, GL_POSITION, pos);
    }
}

// Liga o shader e envia os estados (velas acesas, lanterna, tempo).
inline void usarShaderB(GLuint programa, const std::vector<Vela>& velas, bool lanternaLigada) {
    glUseProgram(programa);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(programa, "textura"), 0);

    float acesa[NUM_VELAS_B] = { 0 };
    for (int i = 0; i < NUM_VELAS_B && i < (int)velas.size(); i++) {
        acesa[i] = velas[i].acesa ? 1.0f : 0.0f;
    }
    glUniform1fv(glGetUniformLocation(programa, "velaAcesa"), NUM_VELAS_B, acesa);
    glUniform1f(glGetUniformLocation(programa, "lanternaLigada"), lanternaLigada ? 1.0f : 0.0f);
    glUniform1f(glGetUniformLocation(programa, "tempo"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
}

// Lustres: manda a posição das 4 luzes para o shader.
// O glLightfv converte a posição para o espaço do olho sozinho; num uniform isso
// é feito à mão: posOlho = MODELVIEW * posMundo. Chamar DEPOIS do cameraAplicar()
// e do usarShaderB() (a modelview atual é a da câmera).
inline void enviarLustresB(GLuint programa, const float posMundo[][3], bool acesos) {
    GLfloat mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);          // matriz guardada por colunas

    GLfloat posOlho[NUM_LUSTRES_B * 3];
    for (int i = 0; i < NUM_LUSTRES_B; i++) {
        float x = posMundo[i][0], y = posMundo[i][1], z = posMundo[i][2];
        posOlho[3 * i + 0] = mv[0] * x + mv[4] * y + mv[8]  * z + mv[12];
        posOlho[3 * i + 1] = mv[1] * x + mv[5] * y + mv[9]  * z + mv[13];
        posOlho[3 * i + 2] = mv[2] * x + mv[6] * y + mv[10] * z + mv[14];
    }
    glUniform3fv(glGetUniformLocation(programa, "lustrePos"), NUM_LUSTRES_B, posOlho);
    glUniform1f(glGetUniformLocation(programa, "lustresAcesos"), acesos ? 1.0f : 0.0f);
}

// Material de um objeto: quanto ele brilha (especular) e quão concentrado é
// o reflexo (brilho). A cor continua vindo do glColor + textura.
//   pedra/madeira fosca: especular baixo, brilho baixo
//   metal/vidro:         especular alto,  brilho alto
inline void materialB(float especular, float brilho) {
    GLfloat spec[]    = { especular, especular, especular, 1.0f };
    GLfloat semEmis[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brilho);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, semEmis);
}

// Emissão: o objeto brilha sozinho, mesmo sem luz batendo (chamas, vidro aceso)
inline void emissaoB(float r, float g, float b) {
    GLfloat emis[] = { r, g, b, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emis);
}
