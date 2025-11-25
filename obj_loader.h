#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} Face;

typedef struct {
    char name[256];
    Vertex *vertices;
    Normal *normals;
    TexCoord *texcoords;
    Face *faces;
    int vertex_count;
    int normal_count;
    int texcoord_count;
    int face_count;
} OBJModel;

// Funções para carregar e desenhar modelos OBJ
OBJModel* load_obj(const char *filename);
void draw_obj_model(OBJModel *model);
void free_obj_model(OBJModel *model);

#endif // OBJ_LOADER_H
