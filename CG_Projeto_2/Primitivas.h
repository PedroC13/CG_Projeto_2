#pragma once
#ifndef PRIMITIVAS_H
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define PRIMITIVAS_H

#include <GL/glew.h> 
#include <vector>
#include <cmath>

struct Primitivas {
    GLuint vaoCubo = 0;
    GLuint vaoPlano = 0;
    GLuint vaoCilindro = 0;
    int numVerticesCilindro = 0;

    void inicializar() {
        setupCubo();
        setupPlano();
        setupCilindro(32); // 32 segmentos para um cilindro suave
    }

private:
    void setupCubo() {
        // Dados: Posição (3), Normal (3), UV (2) -> 36 vértices
        float vertices[] = {
            // Trás
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            // Frente
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            // Esquerda
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            // Direita
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             // Baixo
             -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
              0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
              0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
              0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
             -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             // Cima
             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
              0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
        };

        GLuint vbo;
        glGenVertexArrays(1, &vaoCubo);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vaoCubo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normal (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // TexCoords (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void setupPlano() {
        float vertices[] = {
            // Posição            // Normal           // UV
            -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
             0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
             0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
            -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f
        };

        GLuint vbo;
        glGenVertexArrays(1, &vaoPlano);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vaoPlano);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void setupCilindro(int segmentos) {
        std::vector<float> data;
        float passo = 2.0f * M_PI / segmentos;

        for (int i = 0; i < segmentos; ++i) {
            float a1 = i * passo;
            float a2 = (i + 1) * passo;

            float x1 = 0.5f * cos(a1), z1 = 0.5f * sin(a1);
            float x2 = 0.5f * cos(a2), z2 = 0.5f * sin(a2);

            // Triângulo 1 (Corpo)
            data.insert(data.end(), { x1,  0.5f, z1,  x1, 0.0f, z1,  (float)i / segmentos, 1.0f });
            data.insert(data.end(), { x1, -0.5f, z1,  x1, 0.0f, z1,  (float)i / segmentos, 0.0f });
            data.insert(data.end(), { x2, -0.5f, z2,  x2, 0.0f, z2,  (float)(i + 1) / segmentos, 0.0f });

            // Triângulo 2 (Corpo)
            data.insert(data.end(), { x1,  0.5f, z1,  x1, 0.0f, z1,  (float)i / segmentos, 1.0f });
            data.insert(data.end(), { x2, -0.5f, z2,  x2, 0.0f, z2,  (float)(i + 1) / segmentos, 0.0f });
            data.insert(data.end(), { x2,  0.5f, z2,  x2, 0.0f, z2,  (float)(i + 1) / segmentos, 1.0f });
        }

        numVerticesCilindro = data.size() / 8;

        GLuint vbo;
        glGenVertexArrays(1, &vaoCilindro);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vaoCilindro);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
};

#endif