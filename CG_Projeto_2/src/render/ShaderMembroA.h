#pragma once
#include <GL/Glew.h>
#include <iostream>

//vertex shader roda uma vez por vértice

const char* codigoVertex = R"(
#version 120
varying vec3 vNormal;   // varying = valor passado do vertex para o fragment
varying vec4 vCor;
varying vec2 vUV;

void main() {
    vNormal = normalize(gl_NormalMatrix * gl_Normal);       // normal no espaço da câmera
    vCor    = gl_Color;                                      // vem do glColor
    vUV     = gl_MultiTexCoord0.xy;                          // vem do glTexCoord2f
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  // mesma matriz do pipeline fixo
}
)";

// fragment shader roda uma vez por pixel

const char* codigoFragment = R"(
#version 120
varying vec3 vNormal;
varying vec4 vCor;
varying vec2 vUV;

uniform sampler2D textura;   // a textura presa na unidade 0

void main() {
    vec3 luz = normalize(vec3(0.3, 1.0, 0.5));              // direção da luz
    float difusa = max(dot(normalize(vNormal), luz), 0.0);  // Lambert: cos do ângulo

    vec4 texel = texture2D(textura, vUV);                   // cor da imagem naquele ponto
    vec3 cor = texel.rgb * vCor.rgb * (0.25 + 0.75 * difusa); // 25% ambiente + difusa
    gl_FragColor = vec4(cor, texel.a * vCor.a);
}
)";

// Compila um shader e imprime o erro se tiver algum.

inline GLuint compilarShader(GLenum tipo, const char* codigo) {
	GLuint shader = glCreateShader(tipo);
	glShaderSource(shader, 1, &codigo, nullptr);
	glCompileShader(shader);
	
	GLint compilado;


	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilado);
	
	if (!compilado) {
		GLint log[512];
		glGetShaderInfoLog(shader, 512, nullptr, (GLchar*)log);
		std::cout << "erro no shader: " << log << std::endl;
		
		return 0;
	}
	return shader;
}

// junta os shaders e cria o programa de GPU

inline GLuint criarProgramaShader() {
	GLuint vs = compilarShader(GL_VERTEX_SHADER, codigoVertex);
	GLuint fs = compilarShader(GL_FRAGMENT_SHADER, codigoFragment);
	
	GLuint programa = glCreateProgram();
	glAttachShader(programa, vs);
	glAttachShader(programa, fs);
	glLinkProgram(programa);

	GLint linkado;
	glGetProgramiv(programa, GL_LINK_STATUS, &linkado);
	
	if (!linkado) {
		GLint log[512];
		glGetProgramInfoLog(programa, 512, nullptr, (GLchar*)log);
		std::cout << "erro no link do programa: " << log << std::endl;
		
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return programa;
}