# ibilce_blockC

Projeto em grupo para recriar o Bloco C da UNESP - IBILCE utilizando OpenGL e modelos 3D do Blender.

## Descrição

Este projeto implementa uma visualização 3D interativa do Bloco C do IBILCE utilizando OpenGL/GLUT e C. O projeto inclui:

- **Renderização 3D** de ambientes internos e externos
- **Loader de arquivos OBJ** customizado para importar modelos 3D
- **Modelos 3D** criados no Blender (cadeiras, lousas, ar-condicionado, ventiladores, etc.)
- **Sistema de iluminação** com ciclo dia/noite
- **Controles interativos** para navegação na cena

## Estrutura do Projeto

```
ibilce_blockC/
├── main.c              # Código principal com renderização e controles
├── obj_loader.c        # Implementação do loader de arquivos OBJ
├── obj_loader.h        # Header do loader OBJ
├── CMakeLists.txt      # Configuração do CMake
├── assets/             # Modelos 3D (não versionados - ver seção Assets)
│   ├── air-cond/
│   ├── aud-chair/
│   ├── blackboardbig/
│   ├── blackboardsmall/
│   ├── chair_h/
│   ├── deskschool/
│   ├── extintor-lowpoly/
│   ├── fan/
│   ├── pc/
│   ├── switch/
│   └── toilet/
└── README.md
```

## Dependências

- **OpenGL** - Biblioteca gráfica
- **GLUT** (FreeGLUT) - Gerenciamento de janelas e entrada
- **CMake** (>= 3.10) - Sistema de build
- **GCC** ou outro compilador C compatível

### Instalação no Linux (Ubuntu/Debian):
```bash
sudo apt-get install build-essential cmake
sudo apt-get install freeglut3-dev libglu1-mesa-dev
```

## Compilação e Execução

### Compilar o projeto:
```bash
cmake -B build
cmake --build build
```

### Executar:
```bash
./build/main.exe
```

## Assets

**IMPORTANTE:** Os assets não estão incluídos no repositório Git devido ao tamanho dos arquivos.

**Download dos assets:**
[Link do Google Drive](https://drive.google.com/drive/folders/1PoGKk-z51i-leNRsoNhrTegFYwa-ZcSy?usp=drive_link)

Após o download, extraia a pasta `assets` na raiz do projeto.

## Controles

*(A ser documentado - inclua aqui os controles de câmera e interação)*

## Licença

Este projeto está sob a licença especificada no arquivo `LICENSE`.