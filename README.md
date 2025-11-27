# IBILCE Block C - Visualização 3D Interativa

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![OpenGL](https://img.shields.io/badge/OpenGL-3D-green.svg)](https://www.opengl.org/)

Projeto para recriar o Bloco C da UNESP - IBILCE utilizando OpenGL, C e modelos 3D do Blender, com sistema completo de carregamento de texturas.

## Características

- **Renderização 3D realista** com iluminação dinâmica
- **Sistema de texturas** - Carregamento automático via arquivos MTL
- **Loader OBJ customizado** com suporte a materiais múltiplos
- **Ciclo dia/noite** com transições suaves
- **Controles interativos** para navegação livre
- **Ambientes completos** - Salas de aula, auditório e áreas externas
- **Modelos 3D detalhados** criados no Blender

## Estrutura do Projeto

```
ibilce_blockC/
├── src/                    # Código fonte
│   ├── main.c             # Aplicação principal
│   └── obj_loader.c       # Loader de OBJ/MTL com texturas
├── include/               # Headers
│   ├── obj_loader.h      # API do loader
│   └── stb_image.h       # Biblioteca de imagens
├── examples/              # Exemplos e testes
│   └── test_texture.c    # Teste de carregamento de texturas
├── assets/                # Modelos 3D e texturas (Git LFS)
│   ├── chair_h/          # Cadeira com texturas
│   ├── chalkboard/       # Lousa para sala de aula
│   ├── deskschool/       # Mesa escolar
│   └── toilet/           # Sanitário
├── docs/                  # Documentação
│   └── TEXTURE_SYSTEM.md # Documentação do sistema de texturas
├── build/                 # Arquivos de compilação
├── Makefile              # Build system
├── CMakeLists.txt        # Configuração CMake
└── README.md             # Este arquivo
```

## Início Rápido

### Pré-requisitos

- **GCC** ou compilador C compatível
- **OpenGL** e **GLU**
- **FreeGLUT**
- **Make** (opcional)
- **Git LFS** (para baixar assets)

#### Instalação no Linux (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install freeglut3-dev libglu1-mesa-dev
```

#### Instalação no Arch Linux:
```bash
sudo pacman -S base-devel freeglut mesa
```

#### Instalação do Git LFS:
```bash
# Ubuntu/Debian
sudo apt-get install git-lfs

# Arch Linux
sudo pacman -S git-lfs

# Inicializar Git LFS
git lfs install
```

### Assets

Os assets 3D estão versionados usando **Git LFS** (Large File Storage) para gerenciar arquivos grandes de forma eficiente. A pasta `assets` contém modelos otimizados e suas texturas.

**Modelos incluídos:**
- `chair_h/` - Cadeira com texturas de madeira e metal
- `chalkboard/` - Lousa para sala de aula
- `deskschool/` - Mesa escolar
- `toilet/` - Sanitário completo

Ao clonar o repositório, o Git LFS baixará automaticamente os arquivos grandes. Se você já tem o repositório clonado, execute:
```bash
git lfs pull
```

### Compilação

#### Usando Makefile (Recomendado):
```bash
# Compilar projeto principal
make

# Compilar programa de teste
make test

# Limpar arquivos compilados
make clean
```

#### Usando CMake:
```bash
cmake -B build
cmake --build build
```

#### Compilação Manual:
```bash
gcc -Iinclude src/main.c src/obj_loader.c -o ibilce_blockC -lGL -lGLU -lglut -lm
```

### Execução

```bash
# Executar projeto principal
./ibilce_blockC

# Executar teste de texturas
./test_texture
```

## Controles

### Navegação
- **W** - Mover para frente
- **S** - Mover para trás
- **A** - Mover para esquerda
- **D** - Mover para direita
- **Page Up** - Subir
- **Page Down** - Descer

### Câmera
- **Mouse** - Olhar ao redor (movimento livre)
- **Botão Esquerdo** - Projeção ortogonal
- **Botão Direito** - Projeção perspectiva

### Sistema
- **ESC** - Sair do programa
- **E/R** - Controle do ciclo dia/noite

## Sistema de Texturas

O projeto inclui um sistema completo de carregamento de texturas:

### Uso Básico

```c
// Carregar modelo com texturas
OBJModel *cadeira = load_obj("assets/chair_h/chair_h.obj");

// Renderizar (texturas aplicadas automaticamente)
draw_obj_model(cadeira);

// Renderizar múltiplas instâncias
for (int i = 0; i < 10; i++) {
    glPushMatrix();
    glTranslatef(i * 2.0f, 0, 0);
    draw_obj_model(cadeira);
    glPopMatrix();
}

// Liberar recursos
free_obj_model(cadeira);
```

### Formatos Suportados

- **Modelos:** OBJ com materiais MTL
- **Texturas:** PNG, JPEG, BMP, TGA, GIF
- **Materiais:** Propriedades MTL (Ka, Kd, Ks, Ns, d, map_Kd)

Para documentação completa, consulte [docs/TEXTURE_SYSTEM.md](docs/TEXTURE_SYSTEM.md)

## Documentação

- **[Sistema de Texturas](docs/TEXTURE_SYSTEM.md)** - Documentação completa da API e uso de texturas
- **[Exemplos](examples/)** - Códigos de exemplo e testes

## Ambientes Implementados

### Salas de Aula (Blocks 1-3)
- Cadeiras e mesas dos alunos
- Mesa do professor
- Lousa
- Janelas com iluminação natural
- Portas funcionais

### Auditório (Audi C)
- Poltronas escalonadas
- Palco com cortina
- Iluminação especial
- Capacidade para múltiplos espectadores

## Desenvolvimento

### Adicionar Novos Modelos

1. Exporte o modelo do Blender como OBJ com materiais
2. Coloque o arquivo `.obj`, `.mtl` e texturas em `assets/nome_modelo/`
3. No código, carregue com:
```c
OBJModel *modelo = load_obj("assets/nome_modelo/modelo.obj");
```

### Estrutura de Assets Recomendada

```
assets/
└── nome_modelo/
    ├── modelo.obj           # Modelo 3D
    ├── modelo.mtl           # Materiais
    └── texture/             # Texturas
        ├── diffuse.png
        └── ...
```

## Solução de Problemas

### Texturas não aparecem
- Verifique se `glEnable(GL_LIGHTING)` está ativo
- Certifique-se que há luz na cena (`glEnable(GL_LIGHT0)`)
- Confirme que os arquivos de textura existem nos caminhos corretos

### Modelo aparece preto
- Habilite iluminação: `glEnable(GL_LIGHTING)`
- Configure material: `glEnable(GL_COLOR_MATERIAL)`
- Verifique se há luz posicionada na cena

### Erro ao carregar arquivo
- Confirme que o executável está sendo rodado da raiz do projeto
- Verifique os caminhos relativos nos arquivos MTL
- Certifique-se que os assets foram baixados

## Licença

Este projeto está sob a licença especificada no arquivo [LICENSE](LICENSE).

## Autores

Projeto desenvolvido como trabalho acadêmico para UNESP - IBILCE.
---

**Nota:** Este é um projeto educacional desenvolvido para fins acadêmicos.
