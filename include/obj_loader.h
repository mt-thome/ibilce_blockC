#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

// Estruturas para armazenar dados do modelo OBJ
typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float x, y, z;
} Normal;

typedef struct {
    float u, v;
} TexCoord;

typedef struct {
    int v[3];      // Índices dos vértices
    int vt[3];     // Índices das coordenadas de textura
    int vn[3];     // Índices das normais
    int material_index; // Índice do material usado nesta face
} Face;

// Estrutura para armazenar materiais
typedef struct {
    char name[256];
    float Ka[3];    // Cor ambiente
    float Kd[3];    // Cor difusa
    float Ks[3];    // Cor especular
    float Ns;       // Expoente especular
    float d;        // Transparência
    char map_Kd[512]; // Caminho da textura difusa
    GLuint texture_id; // ID da textura OpenGL (0 se não houver textura)
} Material;

typedef struct {
    char name[256];
    char base_path[512]; // Caminho base para texturas relativas
    Vertex *vertices;
    Normal *normals;
    TexCoord *texcoords;
    Face *faces;
    Material *materials;
    int vertex_count;
    int normal_count;
    int texcoord_count;
    int face_count;
    int material_count;
} OBJModel;

// Funções para carregar e desenhar modelos OBJ
OBJModel* load_obj(const char *filename);
void draw_obj_model(OBJModel *model);
void free_obj_model(OBJModel *model);

// Funções auxiliares para materiais
Material* load_mtl(const char *filename, const char *base_path, int *material_count);
GLuint load_texture(const char *filename);

#endif // OBJ_LOADER_H
