#include "obj_loader.h"
#include <GL/gl.h>

// Função auxiliar para contar linhas no arquivo
static void count_obj_elements(FILE *file, int *v_count, int *vt_count, int *vn_count, int *f_count) {
    char line[256];
    *v_count = *vt_count = *vn_count = *f_count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) (*v_count)++;
        else if (strncmp(line, "vt ", 3) == 0) (*vt_count)++;
        else if (strncmp(line, "vn ", 3) == 0) (*vn_count)++;
        else if (strncmp(line, "f ", 2) == 0) (*f_count)++;
    }
    rewind(file);
}

// Carrega um arquivo OBJ
OBJModel* load_obj(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", filename);
        return NULL;
    }
    
    OBJModel *model = (OBJModel*)malloc(sizeof(OBJModel));
    if (!model) {
        fclose(file);
        return NULL;
    }
    
    // Copia o nome do arquivo
    strncpy(model->name, filename, sizeof(model->name) - 1);
    model->name[sizeof(model->name) - 1] = '\0';
    
    // Conta elementos
    int v_count, vt_count, vn_count, f_count;
    count_obj_elements(file, &v_count, &vt_count, &vn_count, &f_count);
    
    printf("Carregando %s: %d vértices, %d normais, %d texturas, %d faces\n",
           filename, v_count, vn_count, vt_count, f_count);
    
    // Aloca memória
    model->vertices = (Vertex*)malloc(v_count * sizeof(Vertex));
    model->normals = (Normal*)malloc(vn_count * sizeof(Normal));
    model->texcoords = (TexCoord*)malloc(vt_count * sizeof(TexCoord));
    model->faces = (Face*)malloc(f_count * sizeof(Face));
    
    model->vertex_count = 0;
    model->normal_count = 0;
    model->texcoord_count = 0;
    model->face_count = 0;
    
    // Lê o arquivo
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Vértices
        if (strncmp(line, "v ", 2) == 0) {
            Vertex v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            model->vertices[model->vertex_count++] = v;
        }
        // Normais
        else if (strncmp(line, "vn ", 3) == 0) {
            Normal n;
            sscanf(line, "vn %f %f %f", &n.x, &n.y, &n.z);
            model->normals[model->normal_count++] = n;
        }
        // Coordenadas de textura
        else if (strncmp(line, "vt ", 3) == 0) {
            TexCoord t;
            sscanf(line, "vt %f %f", &t.u, &t.v);
            model->texcoords[model->texcoord_count++] = t;
        }
        // Faces
        else if (strncmp(line, "f ", 2) == 0) {
            Face f;
            int matches = 0;
            
            // Tenta diferentes formatos de face
            // Formato: v/vt/vn
            matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                           &f.v[0], &f.vt[0], &f.vn[0],
                           &f.v[1], &f.vt[1], &f.vn[1],
                           &f.v[2], &f.vt[2], &f.vn[2]);
            
            if (matches != 9) {
                // Formato: v//vn
                matches = sscanf(line, "f %d//%d %d//%d %d//%d",
                               &f.v[0], &f.vn[0],
                               &f.v[1], &f.vn[1],
                               &f.v[2], &f.vn[2]);
                if (matches == 6) {
                    f.vt[0] = f.vt[1] = f.vt[2] = 0;
                }
            }
            
            if (matches != 9 && matches != 6) {
                // Formato: v/vt
                matches = sscanf(line, "f %d/%d %d/%d %d/%d",
                               &f.v[0], &f.vt[0],
                               &f.v[1], &f.vt[1],
                               &f.v[2], &f.vt[2]);
                if (matches == 6) {
                    f.vn[0] = f.vn[1] = f.vn[2] = 0;
                }
            }
            
            if (matches != 9 && matches != 6) {
                // Formato: v (apenas vértices)
                matches = sscanf(line, "f %d %d %d",
                               &f.v[0], &f.v[1], &f.v[2]);
                if (matches == 3) {
                    f.vt[0] = f.vt[1] = f.vt[2] = 0;
                    f.vn[0] = f.vn[1] = f.vn[2] = 0;
                }
            }
            
            if (matches >= 3) {
                model->faces[model->face_count++] = f;
            }
        }
    }
    
    fclose(file);
    printf("Modelo carregado com sucesso: %s\n", filename);
    return model;
}

// Desenha o modelo OBJ
void draw_obj_model(OBJModel *model) {
    if (!model) return;
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < model->face_count; i++) {
        Face f = model->faces[i];
        
        for (int j = 0; j < 3; j++) {
            // Índices em OBJ começam em 1, não em 0
            int v_idx = f.v[j] - 1;
            int vn_idx = f.vn[j] - 1;
            int vt_idx = f.vt[j] - 1;
            
            // Normal
            if (vn_idx >= 0 && vn_idx < model->normal_count) {
                Normal n = model->normals[vn_idx];
                glNormal3f(n.x, n.y, n.z);
            }
            
            // Coordenada de textura
            if (vt_idx >= 0 && vt_idx < model->texcoord_count) {
                TexCoord t = model->texcoords[vt_idx];
                glTexCoord2f(t.u, t.v);
            }
            
            // Vértice
            if (v_idx >= 0 && v_idx < model->vertex_count) {
                Vertex v = model->vertices[v_idx];
                glVertex3f(v.x, v.y, v.z);
            }
        }
    }
    glEnd();
}

// Libera a memória do modelo
void free_obj_model(OBJModel *model) {
    if (!model) return;
    
    free(model->vertices);
    free(model->normals);
    free(model->texcoords);
    free(model->faces);
    free(model);
}
