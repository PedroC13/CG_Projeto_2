# O Turno Noturno da Igreja Assombrada

Cena 3D interativa de uma igreja assombrada à noite, feita em **C++ e OpenGL** (sem engine de jogo) para a disciplina de **Computação Gráfica (UFAL) — AB2**.

O jogador é o vigia noturno: anda pela igreja com uma lamparina na mão, abre portas e acende ou apaga as velas dos castiçais.

---

## Requisitos

| O que | Detalhe |
|---|---|
| Sistema | Windows 10 ou 11 |
| IDE | **Visual Studio 2026** com a carga de trabalho **"Desenvolvimento para desktop com C++"** (o projeto usa o toolset `v145`) |
| Git | para clonar o repositório |
| Placa de vídeo | qualquer uma com suporte a OpenGL 2.1 ou superior (Intel, NVIDIA ou AMD) |

As bibliotecas **freeglut** e **GLEW** vêm pelo pacote NuGet `nupengl`, baixado automaticamente pelo Visual Studio. O **stb_image** já está no repositório, em `CG_Projeto_2/include/`. Não é preciso instalar nada à mão.

---

## Como compilar e rodar

### 1. Clonar o repositório

```bash
git clone https://github.com/PedroC13/CG_Projeto_2.git
cd CG_Projeto_2
```

### 2. Abrir no Visual Studio

Abra o arquivo **`CG_Projeto_2.slnx`** (duplo clique, ou *Arquivo → Abrir → Projeto/Solução*).

### 3. Restaurar os pacotes NuGet

Na primeira vez, o Visual Studio baixa o freeglut e o GLEW sozinho ao compilar. Se aparecer o erro *"This project references NuGet package(s) that are missing"*:

- clique com o botão direito na **Solução** (no Gerenciador de Soluções) → **Restaurar Pacotes NuGet**;
- depois, compile de novo.

### 4. Escolher a configuração

Na barra de ferramentas, selecione:

- **Debug** (para testar) ou **Release** (mais rápido);
- plataforma **x86** ou **x64** (as duas funcionam).

### 5. Compilar e rodar

Pressione **F5** (ou **Ctrl + F5** para rodar sem o depurador).

Rodando pelo Visual Studio, a pasta de trabalho já é `CG_Projeto_2/CG_Projeto_2/`, onde fica a pasta `assets`, então as texturas carregam normalmente.

---

## Rodar o executável fora do Visual Studio

O programa carrega as texturas pelo caminho relativo `assets/textures/...`. Para abrir o `.exe` direto (por exemplo, para enviar a alguém):

1. Compile em **Release**.
2. Pegue a pasta de saída (`Release/` ou `x64/Release/`, na raiz do repositório). Ela já contém `CG_Projeto_2.exe`, `freeglut.dll` e `glew32.dll`.
3. Copie para dentro dela a pasta **`CG_Projeto_2/assets`**, ficando assim:

```
Release/
├── CG_Projeto_2.exe
├── freeglut.dll
├── glew32.dll
└── assets/
    └── textures/
```

4. Dê duplo clique no `CG_Projeto_2.exe`.

---

## Compilar sem o Visual Studio (alternativo)

> ⚠️ **Não recomendado.** O caminho oficial é o Visual Studio (seção acima). As opções abaixo funcionam, mas exigem instalar as bibliotecas à mão e são para quem não pode ou não quer usar a IDE.

### Opção A: só a linha de comando da Microsoft (sem a IDE)

Usa o mesmo compilador do Visual Studio, mas sem abrir a IDE.

1. Instale o **Build Tools para Visual Studio 2026** (instalador gratuito da Microsoft), marcando **"Desenvolvimento para desktop com C++"**.
2. Baixe o **`nuget.exe`** (em nuget.org/downloads) e coloque na raiz do repositório.
3. Abra o **"Developer PowerShell for VS 2026"** (menu Iniciar), entre na pasta do repositório e rode:

```powershell
# baixa o freeglut e o GLEW para a pasta packages/
.\nuget.exe restore CG_Projeto_2\packages.config -PackagesDirectory packages

# compila (Release, 32 bits)
msbuild CG_Projeto_2\CG_Projeto_2.vcxproj -p:Configuration=Release -p:Platform=Win32 -p:SolutionDir="$PWD\\"
```

O executável sai em `Release\CG_Projeto_2.exe`, já com as DLLs do lado. Para rodar, siga a seção **"Rodar o executável fora do Visual Studio"** (copiar a pasta `assets`).

### Opção B: MSYS2 + g++ (MinGW)

Usa o compilador g++, sem nada da Microsoft.

1. Instale o **MSYS2** (msys2.org) e abra o terminal **"MSYS2 UCRT64"**.
2. Instale o compilador e as bibliotecas:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut mingw-w64-ucrt-x86_64-glew
```

3. Entre na pasta do código (troque o caminho pelo do seu clone) e compile:

```bash
cd /c/caminho/para/CG_Projeto_2/CG_Projeto_2

g++ -std=c++20 -O2 -Isrc -Iinclude src/Main.cpp -o igreja.exe \
    -lfreeglut -lglew32 -lglu32 -lopengl32
```

4. Rode **pelo mesmo terminal**, a partir dessa pasta (onde está `assets/`):

```bash
./igreja.exe
```

Para abrir o `igreja.exe` fora do terminal do MSYS2, copie para o lado dele estas DLLs de `C:\msys64\ucrt64\bin\`: `libfreeglut.dll`, `glew32.dll`, `libstdc++-6.dll`, `libgcc_s_seh-1.dll` e `libwinpthread-1.dll`.

### Opção C: Linux

```bash
# Debian/Ubuntu
sudo apt install g++ freeglut3-dev libglew-dev

cd CG_Projeto_2/CG_Projeto_2
g++ -std=c++20 -O2 -Isrc -Iinclude src/Main.cpp -o igreja -lglut -lGLEW -lGLU -lGL
./igreja
```

---

## Controles

| Tecla | Ação |
|---|---|
| **W A S D** | andar |
| **Mouse** | olhar em volta |
| **E** | abrir/fechar a porta mais próxima (entrada e confessionário) e acender/apagar a vela mais próxima |
| **F** | acender/apagar a lamparina na mão |
| **T** | alternar o modo wireframe |
| **Esc** | sair |

---

## Problemas comuns

| Problema | Solução |
|---|---|
| Erro de pacote NuGet ausente | botão direito na Solução → **Restaurar Pacotes NuGet** e compilar de novo |
| `freeglut.dll` ou `glew32.dll` não encontrado ao abrir o `.exe` | as DLLs precisam estar na mesma pasta do executável (veja a seção acima) |
| Cena sem textura ou console com *"Erro ao carregar textura"* | o programa não está rodando da pasta que contém `assets/`; rode pelo Visual Studio ou copie a pasta `assets` para junto do `.exe` |
| Erro *"toolset v145 não encontrado"* | instale o Visual Studio 2026, ou, numa versão anterior, vá em *Projeto → Propriedades → Geral → Conjunto de Ferramentas da Plataforma* e escolha o toolset instalado |
| Console com *"erro no shader"* | a placa de vídeo ou o driver não suporta GLSL 1.20; atualize o driver de vídeo |

---

## Estrutura do projeto

```
CG_Projeto_2/
├── CG_Projeto_2.slnx              solução do Visual Studio
└── CG_Projeto_2/
    ├── assets/textures/           texturas (pedra, madeira, metal, vitral...)
    ├── include/                   stb_image.h
    ├── Textura.h                  carregamento de texturas
    └── src/
        ├── Main.cpp               janela, loop principal e montagem da cena
        ├── player/                câmera em primeira pessoa (WASD + mouse)
        ├── render/                modelos dos objetos e shaders
        └── scene/                 lógica das interações (portas, velas) e colisão
```

---

## Tecnologias

- **C++20** e **OpenGL**
- **freeglut** (janela, teclado e mouse) e **GLEW** (funções modernas do OpenGL)
- **GLSL 1.20**: iluminação **Blinn-Phong** por pixel com luar, lamparina e as luzes das velas
- **stb_image** para carregar as texturas

---

## Equipe

| Membro | Papel |
|---|---|
| Membro A | Tech Lead: estrutura, janela, loop principal e câmera |
| Membro B | Graphics Lead: shaders, iluminação, materiais e texturas |
| Membro C | Logic Lead: interações, estado dos objetos e colisão |
