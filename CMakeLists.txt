# Versão mínima requerida do CMake
cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(
    IBILCE_BlockC C
    VERSION 2.0.0
    DESCRIPTION "Visualização 3D do Bloco C - IBILCE/UNESP"
)

# Padrão C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Pacotes utilizados (bibliotecas)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Diretórios de include
include_directories(${CMAKE_SOURCE_DIR}/include)

# Arquivos fonte
set(SOURCES
    src/main.c
    src/obj_loader.c
)

# Executável principal
add_executable(
    ibilce_blockC ${SOURCES}
)

# Adicionando as bibliotecas à compilação
target_link_libraries(
    ibilce_blockC PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    m
)

# Executável de teste (opcional)
add_executable(
    test_texture
    examples/test_texture.c
    src/obj_loader.c
)

target_link_libraries(
    test_texture PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    m
)

# Configurações de compilação
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(ibilce_blockC PRIVATE -Wall -Wextra -g)
    target_compile_options(test_texture PRIVATE -Wall -Wextra -g)
else()
    target_compile_options(ibilce_blockC PRIVATE -O2)
    target_compile_options(test_texture PRIVATE -O2)
endif()