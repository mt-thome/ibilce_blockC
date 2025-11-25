# Versão mínima requerida do CMake
cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(
    OpenGLProject C
)

# Pacotes utilizados (bibliotecas)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Adicionando ao executável
add_executable(
    main.exe main.c obj_loader.c
)

# Adicionando as bibliotecas a compilação
target_link_libraries(
    main.exe PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    m
)