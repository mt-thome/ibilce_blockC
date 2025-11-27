#include "obj_loader.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <libgen.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Define glGenerateMipmap se não estiver disponível
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP 0x8191
#endif

// Tipo da função glGenerateMipmap
typedef void (*PFNGLGENERATEMIPMAPPROC)(GLenum target);

// Função wrapper para glGenerateMipmap
static void generate_mipmap(GLenum target) {
    static PFNGLGENERATEMIPMAPPROC glGenerateMipmapFunc = NULL;
    if (glGenerateMipmapFunc == NULL) {
        // Tenta carregar a função dinamicamente
        glGenerateMipmapFunc = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress((const GLubyte*)"glGenerateMipmap");
        if (glGenerateMipmapFunc == NULL) {
            // Fallback para glGenerateMipmapEXT se disponível
            glGenerateMipmapFunc = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress((const GLubyte*)"glGenerateMipmapEXT");
        }
    }
    if (glGenerateMipmapFunc != NULL) {
        glGenerateMipmapFunc(target);
    }
}

// Parse otimizado de float
static inline float fast_atof(const char **str) {
    const char *p = *str;
    while (*p == ' ' || *p == '\t') p++;
    
    float sign = 1.0f;
    if (*p == '-') {
        sign = -1.0f;
        p++;
    } else if (*p == '+') {
        p++;
    }
    
    float value = 0.0f;
    while (*p >= '0' && *p <= '9') {
        value = value * 10.0f + (*p - '0');
        p++;
    }
    
    if (*p == '.') {
        p++;
        float decimal = 0.1f;
        while (*p >= '0' && *p <= '9') {
            value += (*p - '0') * decimal;
            decimal *= 0.1f;
            p++;
        }
    }
    
    *str = p;
    return sign * value;
}

// Parse otimizado de inteiro
static inline int fast_atoi(const char **str) {
    const char *p = *str;
    while (*p == ' ' || *p == '\t') p++;
    
    int sign = 1;
    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        p++;
    }
    
    int value = 0;
    while (*p >= '0' && *p <= '9') {
        value = value * 10 + (*p - '0');
        p++;
    }
    
    *str = p;
    return sign * value;
}

// Cache simples para busca de materiais (evita busca O(n) repetida)
static int find_material_cached(Material *materials, int material_count, const char *name, int *last_index) {
    // Verifica cache primeiro
    if (*last_index >= 0 && *last_index < material_count && 
        strcmp(materials[*last_index].name, name) == 0) {
        return *last_index;
    }
    
    // Busca linear (para poucos materiais é suficiente)
    for (int i = 0; i < material_count; i++) {
        if (strcmp(materials[i].name, name) == 0) {
            *last_index = i;
            return i;
        }
    }
    return -1;
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
    
    // Extrai o caminho base para texturas
    char *filename_copy = strdup(filename);
    char *dir = dirname(filename_copy);
    snprintf(model->base_path, sizeof(model->base_path), "%s", dir);
    free(filename_copy);
    
    // Inicializa materiais
    model->materials = NULL;
    model->material_count = 0;
    
    // Aloca buffers iniciais
    int v_capacity = 1024, vn_capacity = 1024, vt_capacity = 1024, f_capacity = 2048;
    model->vertices = (Vertex*)malloc(v_capacity * sizeof(Vertex));
    model->normals = (Normal*)malloc(vn_capacity * sizeof(Normal));
    model->texcoords = (TexCoord*)malloc(vt_capacity * sizeof(TexCoord));
    model->faces = (Face*)malloc(f_capacity * sizeof(Face));
    
    // Valida alocações
    if (!model->vertices || !model->normals || !model->texcoords || !model->faces) {
        fprintf(stderr, "Erro ao alocar memória para o modelo\n");
        free(model->vertices);
        free(model->normals);
        free(model->texcoords);
        free(model->faces);
        free(model);
        fclose(file);
        return NULL;
    }
    
    model->vertex_count = 0;
    model->normal_count = 0;
    model->texcoord_count = 0;
    model->face_count = 0;
    
    int current_material_index = -1;
    int material_cache = -1; 
    
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Biblioteca de materiais
        if (strncmp(line, "mtllib ", 7) == 0) {
            char mtl_filename[256];
            sscanf(line, "mtllib %s", mtl_filename);
            
            // Constrói caminho completo do arquivo MTL
            char mtl_path[512];
            snprintf(mtl_path, sizeof(mtl_path), "%s/%s", model->base_path, mtl_filename);
            
            // Carrega materiais
            model->materials = load_mtl(mtl_path, model->base_path, &model->material_count);
            continue;
        }
        // Uso de material
        if (strncmp(line, "usemtl ", 7) == 0) {
            char material_name[256];
            sscanf(line, "usemtl %s", material_name);
            
            // Procura o material usando cache
            current_material_index = find_material_cached(model->materials, model->material_count, material_name, &material_cache);
            continue;
        }
        // Vértices
        if (strncmp(line, "v ", 2) == 0) {
            // Expande buffer se necessário
            if (model->vertex_count >= v_capacity) {
                v_capacity *= 2;
                model->vertices = (Vertex*)realloc(model->vertices, v_capacity * sizeof(Vertex));
            }
            const char *p = line + 2;
            model->vertices[model->vertex_count].x = fast_atof(&p);
            model->vertices[model->vertex_count].y = fast_atof(&p);
            model->vertices[model->vertex_count].z = fast_atof(&p);
            model->vertex_count++;
        }
        // Normais 
        else if (strncmp(line, "vn ", 3) == 0) {
            if (model->normal_count >= vn_capacity) {
                vn_capacity *= 2;
                model->normals = (Normal*)realloc(model->normals, vn_capacity * sizeof(Normal));
            }
            const char *p = line + 3;
            model->normals[model->normal_count].x = fast_atof(&p);
            model->normals[model->normal_count].y = fast_atof(&p);
            model->normals[model->normal_count].z = fast_atof(&p);
            model->normal_count++;
        }
        // Coordenadas de textura (parse otimizado)
        else if (strncmp(line, "vt ", 3) == 0) {
            if (model->texcoord_count >= vt_capacity) {
                vt_capacity *= 2;
                model->texcoords = (TexCoord*)realloc(model->texcoords, vt_capacity * sizeof(TexCoord));
            }
            const char *p = line + 3;
            model->texcoords[model->texcoord_count].u = fast_atof(&p);
            model->texcoords[model->texcoord_count].v = fast_atof(&p);
            model->texcoord_count++;
        }
        // Faces 
        else if (strncmp(line, "f ", 2) == 0) {
            // Expande buffer se necessário
            if (model->face_count >= f_capacity - 1) {
                f_capacity *= 2;
                model->faces = (Face*)realloc(model->faces, f_capacity * sizeof(Face));
            }
            
            Face f;
            int v4 = 0, vt4 = 0, vn4 = 0; // Para quads
            int matches = 0;
            
            // Tenta diferentes formatos de face - Formato: v/vt/vn (triângulo ou quad)
            matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &f.v[0], &f.vt[0], &f.vn[0], &f.v[1], &f.vt[1], &f.vn[1], &f.v[2], &f.vt[2], &f.vn[2], &v4, &vt4, &vn4);
            
            if (matches == 9 || matches == 12) {
                // Formato v/vt/vn detectado
                f.material_index = current_material_index;
                model->faces[model->face_count++] = f;
                
                // Se é quad, divide em dois triângulos
                if (matches == 12) {
                    Face f2;
                    f2.v[0] = f.v[0]; f2.vt[0] = f.vt[0]; f2.vn[0] = f.vn[0];
                    f2.v[1] = f.v[2]; f2.vt[1] = f.vt[2]; f2.vn[1] = f.vn[2];
                    f2.v[2] = v4; f2.vt[2] = vt4; f2.vn[2] = vn4;
                    f2.material_index = current_material_index;
                    model->faces[model->face_count++] = f2;
                }
            } else {
                // Formato: v//vn
                matches = sscanf(line, "f %d//%d %d//%d %d//%d %d//%d",  &f.v[0], &f.vn[0], &f.v[1], &f.vn[1], &f.v[2], &f.vn[2], &v4, &vn4);
                
                if (matches == 6 || matches == 8) {
                    f.vt[0] = f.vt[1] = f.vt[2] = 0;
                    f.material_index = current_material_index;
                    model->faces[model->face_count++] = f;
                    
                    if (matches == 8) {
                        Face f2;
                        f2.v[0] = f.v[0]; f2.vn[0] = f.vn[0]; f2.vt[0] = 0;
                        f2.v[1] = f.v[2]; f2.vn[1] = f.vn[2]; f2.vt[1] = 0;
                        f2.v[2] = v4; f2.vn[2] = vn4; f2.vt[2] = 0;
                        f2.material_index = current_material_index;
                        model->faces[model->face_count++] = f2;
                    }
                } else {
                    // Formato: v/vt
                    matches = sscanf(line, "f %d/%d %d/%d %d/%d %d/%d",
                                   &f.v[0], &f.vt[0],
                                   &f.v[1], &f.vt[1],
                                   &f.v[2], &f.vt[2],
                                   &v4, &vt4);
                    
                    if (matches == 6 || matches == 8) {
                        f.vn[0] = f.vn[1] = f.vn[2] = 0;
                        f.material_index = current_material_index;
                        model->faces[model->face_count++] = f;
                        
                        if (matches == 8) {
                            Face f2;
                            f2.v[0] = f.v[0]; f2.vt[0] = f.vt[0]; f2.vn[0] = 0;
                            f2.v[1] = f.v[2]; f2.vt[1] = f.vt[2]; f2.vn[1] = 0;
                            f2.v[2] = v4; f2.vt[2] = vt4; f2.vn[2] = 0;
                            f2.material_index = current_material_index;
                            model->faces[model->face_count++] = f2;
                        }
                    } else {
                        // Formato: v (apenas vértices)
                        matches = sscanf(line, "f %d %d %d %d",
                                       &f.v[0], &f.v[1], &f.v[2], &v4);
                        
                        if (matches == 3 || matches == 4) {
                            f.vt[0] = f.vt[1] = f.vt[2] = 0;
                            f.vn[0] = f.vn[1] = f.vn[2] = 0;
                            f.material_index = current_material_index;
                            model->faces[model->face_count++] = f;
                            
                            if (matches == 4) {
                                Face f2;
                                f2.v[0] = f.v[0]; f2.v[1] = f.v[2]; f2.v[2] = v4;
                                f2.vt[0] = f2.vt[1] = f2.vt[2] = 0;
                                f2.vn[0] = f2.vn[1] = f2.vn[2] = 0;
                                f2.material_index = current_material_index;
                                model->faces[model->face_count++] = f2;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Ajusta buffers para tamanho exato (libera memória não usada)
    model->vertices = (Vertex*)realloc(model->vertices, model->vertex_count * sizeof(Vertex));
    model->normals = (Normal*)realloc(model->normals, model->normal_count * sizeof(Normal));
    model->texcoords = (TexCoord*)realloc(model->texcoords, model->texcoord_count * sizeof(TexCoord));
    model->faces = (Face*)realloc(model->faces, model->face_count * sizeof(Face));
    
    printf("Modelo carregado: %d vértices, %d normais, %d texturas, %d faces\n", model->vertex_count, model->normal_count, model->texcoord_count, model->face_count);
    
    fclose(file);
    return model;
}

// Calcula normal de uma face a partir de 3 vértices
static void calculate_face_normal(Vertex v0, Vertex v1, Vertex v2, float *nx, float *ny, float *nz) {
    float u_x = v1.x - v0.x;
    float u_y = v1.y - v0.y;
    float u_z = v1.z - v0.z;
    
    float v_x = v2.x - v0.x;
    float v_y = v2.y - v0.y;
    float v_z = v2.z - v0.z;
    
    // Produto vetorial (normal da face)
    *nx = u_y * v_z - u_z * v_y;
    *ny = u_z * v_x - u_x * v_z;
    *nz = u_x * v_y - u_y * v_x;
    
    // Normaliza o vetor
    float length = sqrtf((*nx) * (*nx) + (*ny) * (*ny) + (*nz) * (*nz));
    if (length > 0.0001f) {
        *nx /= length;
        *ny /= length;
        *nz /= length;
    }
}

// Desenha o modelo OBJ
void draw_obj_model(OBJModel *model) {
    if (!model) return;
    
    // Habilita texturização com modo de modulação correto
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Habilita normalização automática (importante para escala não uniforme)
    glEnable(GL_NORMALIZE);
    
    // Desabilita face culling para ver ambos os lados (debug)
    glDisable(GL_CULL_FACE);
    
    int last_material_index = -2; // Força aplicação do primeiro material
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < model->face_count; i++) {
        Face f = model->faces[i];
        
        // Se o material mudou, aplica novo material
        if (f.material_index != last_material_index) {
            glEnd(); // Finaliza batch anterior
            
            // Aplica material
            if (f.material_index >= 0 && f.material_index < model->material_count) {
                Material *mat = &model->materials[f.material_index];
                
                // Aplica propriedades do material
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat->Ka);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->Kd);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->Ks);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->Ns);
                
                // Aplica textura se disponível
                if (mat->texture_id != 0) {
                    glBindTexture(GL_TEXTURE_2D, mat->texture_id);
                    glEnable(GL_TEXTURE_2D);
                } else {
                    glDisable(GL_TEXTURE_2D);
                }
            } else {
                // Sem material, desabilita textura
                glDisable(GL_TEXTURE_2D);
            }
            
            last_material_index = f.material_index;
            glBegin(GL_TRIANGLES); // Inicia novo batch
        }
        
        for (int j = 0; j < 3; j++) {
            // Índices em OBJ começam em 1, não em 0
            int v_idx = f.v[j] - 1;
            int vn_idx = f.vn[j] - 1;
            int vt_idx = f.vt[j] - 1;
            
            // Valida índice do vértice
            if (v_idx < 0 || v_idx >= model->vertex_count) {
                fprintf(stderr, "Aviso: índice de vértice inválido %d na face %d\n", f.v[j], i);
                continue;
            }
            
            // Normal (se não houver, calcula da face)
            if (vn_idx >= 0 && vn_idx < model->normal_count) {
                Normal n = model->normals[vn_idx];
                glNormal3f(n.x, n.y, n.z);
            } else if (j == 0) {
                // Calcula normal da face no primeiro vértice
                int v0_idx = f.v[0] - 1;
                int v1_idx = f.v[1] - 1;
                int v2_idx = f.v[2] - 1;
                
                if (v0_idx >= 0 && v0_idx < model->vertex_count &&
                    v1_idx >= 0 && v1_idx < model->vertex_count &&
                    v2_idx >= 0 && v2_idx < model->vertex_count) {
                    
                    float nx, ny, nz;
                    calculate_face_normal(model->vertices[v0_idx], model->vertices[v1_idx], model->vertices[v2_idx], &nx, &ny, &nz);
                    glNormal3f(nx, ny, nz);
                }
            }
            
            // Coordenada de textura
            if (vt_idx >= 0 && vt_idx < model->texcoord_count) {
                TexCoord t = model->texcoords[vt_idx];
                glTexCoord2f(t.u, t.v);
            }
            
            // Vértice
            Vertex v = model->vertices[v_idx];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();
    
    // Restaura estado do OpenGL
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
}

// Carrega uma textura de arquivo
GLuint load_texture(const char *filename) {
    int width, height, channels;
    
    // Corrige orientação 
    stbi_set_flip_vertically_on_load(1);
    
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    
    if (!data) {
        fprintf(stderr, "Erro ao carregar textura: %s\n", filename);
        fprintf(stderr, "Motivo: %s\n", stbi_failure_reason());
        return 0;
    }
    
    printf("Textura carregada: %s (%dx%d, %d canais)\n", filename, width, height, channels);
    
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    // Configurações de filtro e wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Determina formato baseado nos canais
    GLenum format;
    if (channels == 1)
        format = GL_LUMINANCE;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;
    else {
        fprintf(stderr, "Formato de imagem não suportado: %d canais\n", channels);
        stbi_image_free(data);
        return 0;
    }
    
    // Corrige distorção: previne erros quando largura não é múltiplo de 4 pixels
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Carrega a textura
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // Gera mipmaps (método moderno, compatível com drivers recentes)
    generate_mipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return texture_id;
}

// Carrega arquivo MTL
Material* load_mtl(const char *filename, const char *base_path, int *material_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Aviso: não foi possível abrir arquivo MTL: %s\n", filename);
        *material_count = 0;
        return NULL;
    }
    
    printf("Carregando materiais de: %s\n", filename);
    
    // Primeira passagem: contar materiais
    char line[512];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "newmtl ", 7) == 0) {
            count++;
        }
    }
    
    if (count == 0) {
        fclose(file);
        *material_count = 0;
        return NULL;
    }
    
    printf("Encontrados %d materiais\n", count);
    
    // Aloca memória para materiais
    Material *materials = (Material*)malloc(count * sizeof(Material));
    if (!materials) {
        fclose(file);
        *material_count = 0;
        return NULL;
    }
    
    // Segunda passagem: carregar dados dos materiais
    rewind(file);
    int current_material = -1;
    
    while (fgets(line, sizeof(line), file)) {
        // Novo material
        if (strncmp(line, "newmtl ", 7) == 0) {
            current_material++;
            sscanf(line, "newmtl %s", materials[current_material].name);
            
            // Inicializa valores padrão
            materials[current_material].Ka[0] = 0.2f;
            materials[current_material].Ka[1] = 0.2f;
            materials[current_material].Ka[2] = 0.2f;
            
            materials[current_material].Kd[0] = 0.8f;
            materials[current_material].Kd[1] = 0.8f;
            materials[current_material].Kd[2] = 0.8f;
            
            materials[current_material].Ks[0] = 0.5f;
            materials[current_material].Ks[1] = 0.5f;
            materials[current_material].Ks[2] = 0.5f;
            
            materials[current_material].Ns = 32.0f;
            materials[current_material].d = 1.0f;
            materials[current_material].map_Kd[0] = '\0';
            materials[current_material].texture_id = 0;
        }
        // Cor ambiente
        else if (current_material >= 0 && strncmp(line, "Ka ", 3) == 0) {
            sscanf(line, "Ka %f %f %f", &materials[current_material].Ka[0], &materials[current_material].Ka[1], &materials[current_material].Ka[2]);
        }
        // Cor difusa
        else if (current_material >= 0 && strncmp(line, "Kd ", 3) == 0) {
            sscanf(line, "Kd %f %f %f", &materials[current_material].Kd[0], &materials[current_material].Kd[1], &materials[current_material].Kd[2]);
        }
        // Cor especular
        else if (current_material >= 0 && strncmp(line, "Ks ", 3) == 0) {
            sscanf(line, "Ks %f %f %f",  &materials[current_material].Ks[0], &materials[current_material].Ks[1], &materials[current_material].Ks[2]);
        }
        // Expoente especular
        else if (current_material >= 0 && strncmp(line, "Ns ", 3) == 0) {
            sscanf(line, "Ns %f", &materials[current_material].Ns);
        }
        // Transparência
        else if (current_material >= 0 && strncmp(line, "d ", 2) == 0) {
            sscanf(line, "d %f", &materials[current_material].d);
        }
        // Textura difusa
        else if (current_material >= 0 && strncmp(line, "map_Kd ", 7) == 0) {
            char tex_filename[512];
            sscanf(line, "map_Kd %s", tex_filename);
            
            // Constrói caminho completo da textura
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, tex_filename);
            
            strncpy(materials[current_material].map_Kd, full_path, sizeof(materials[current_material].map_Kd) - 1);
            
            // Verifica se a textura já foi carregada em outro material 
            GLuint existing_texture = 0;
            for (int i = 0; i < current_material; i++) {
                if (strcmp(materials[i].map_Kd, full_path) == 0 && materials[i].texture_id != 0) {
                    existing_texture = materials[i].texture_id;
                    break;
                }
            }
            
            if (existing_texture != 0) {
                // Reutiliza textura já carregada
                materials[current_material].texture_id = existing_texture;
            } else {
                // Carrega a textura
                materials[current_material].texture_id = load_texture(full_path);
                
                if (materials[current_material].texture_id == 0) {
                    fprintf(stderr, "Aviso: falha ao carregar textura %s\n", full_path);
                }
            }
        }
    }
    
    fclose(file);
    *material_count = count;
    
    printf("Materiais carregados com sucesso\n");
    return materials;
}

// Libera a memória do modelo
void free_obj_model(OBJModel *model) {
    if (!model) return;
    
    if (model->materials) {
        for (int i = 0; i < model->material_count; i++) {
            if (model->materials[i].texture_id != 0) {
                int is_shared = 0;
                for (int j = 0; j < i; j++) {
                    if (model->materials[j].texture_id == model->materials[i].texture_id) {
                        is_shared = 1;
                        break;
                    }
                }
                if (!is_shared) {
                    glDeleteTextures(1, &model->materials[i].texture_id);
                }
            }
        }
        free(model->materials);
    }
    
    free(model->vertices);
    free(model->normals);
    free(model->texcoords);
    free(model->faces);
    free(model);
}
