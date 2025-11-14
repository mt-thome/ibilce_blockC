#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>
#include "obj_loader.h"

#define translado 30
#define escala 3
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
int rotating=0, timeOfDay=360, skyColor=255;

// Modelos OBJ carregados
OBJModel *model_air_cond = NULL;
OBJModel *model_aud_chair = NULL;
OBJModel *model_blackboard_big = NULL;
OBJModel *model_blackboard_small = NULL;
OBJModel *model_chair = NULL;
OBJModel *model_desk = NULL;
OBJModel *model_extintor = NULL;
OBJModel *model_fan = NULL;
OBJModel *model_pc = NULL;
OBJModel *model_toilet = NULL;

void DesenharCena();
void making_sky();
void making_sun();
void making_moon();
void load_all_models();
void free_all_models();

void making_environment();
void audi_c();
void block1();
void block2();
void block3();

// Funções auxiliares para desenhar elementos da sala de aula
void draw_chair(float x, float y, float z, float rotation);
void draw_blackboard(float x, float y, float z, float rotation);
void draw_classroom_interior(float width, float height, float depth);
void draw_door(float x, float y, float z, float rotation, int isOpen);
void draw_auditorium_seat(float x, float y, float z, float rotation);

//Declara��o de Vari�veis Globis
int projecao=0; //Vari�vel L�gica para Definir o Tipo de Proje��o (Perspectiva ou Ortogonal)
float posx=0, posy=30, posz=200; //Vari�veis que definem a posi��o da c�mera
float oy=30,ox=0,oz=0;         //Vari�veis que definem para onde a c�mera olha
int lx=0, ly=1,  lz=0;     //Vari�veis que definem o eixo da c�mera


GLfloat angulox=0.0;
GLfloat anguloy=0.0;

int x_temp=-1; //Armazena posi��o X do ponteiro do mouse
int y_temp=-1; //Armazena posi��o Y do ponteiro do mouse
int rotacao = 0; //Controla eixo de rota��o do mouse
int i;//controle do for

void making_floor(){
  glBegin(GL_QUADS);
  glPushMatrix();
  glColor3ub(130, 160, 0);
  glVertex3f(-200, -10, 200);
  glVertex3f(200, -10, 200);
  glVertex3f(200, -10, -200);
  glVertex3f(-200, -10, -200);
  glPopMatrix();
  glEnd();
}

void making_sun(){
  glTranslatef(0, 0, 0);
  glColor3ub(255, 220, 0);
  glutSolidSphere(40, 100, 100);
}

void making_moon(){
  glTranslatef(0, 0, 0);
  glColor3ub(220, 220, 240);
  glutSolidSphere(35, 100, 100);
}

void making_sky(){
  // Calcula as cores do céu baseado no ângulo do sol
  float angle_normalized = fmod(timeOfDay + 360, 360) / 360.0; // 0 a 1
  
  int r, g, b;
  
  // Transição suave entre dia e noite
  if (angle_normalized < 0.25) { // Amanhecer (0 - 90 graus)
      float t = angle_normalized / 0.25;
      r = (int)(10 + t * 125); // 10 -> 135
      g = (int)(10 + t * 196); // 10 -> 206
      b = (int)(30 + t * 205); // 30 -> 235
  } else if (angle_normalized < 0.5) { // Dia (90 - 180 graus)
      r = 135;
      g = 206;
      b = 235;
  } else if (angle_normalized < 0.75) { // Entardecer (180 - 270 graus)
      float t = (angle_normalized - 0.5) / 0.25;
      r = (int)(135 - t * 95); // 135 -> 40
      g = (int)(206 - t * 156); // 206 -> 50
      b = (int)(235 - t * 185); // 235 -> 50
  } else { // Noite (270 - 360 graus)
      r = 10;
      g = 10;
      b = 30;
  }
  
  glPushMatrix();
  glColor3ub(r, g, b);
  glutSolidCube(2000);
  glPopMatrix();
}

void making_environment(){
    float angle_rad = timeOfDay * M_PI / 180.0;
    float distance = 500;
    
    // Posição do sol/lua em órbita
    float x = distance * cos(angle_rad);
    float y = distance * sin(angle_rad);
    
    // Sol está visível de 0 a 180 graus (parte superior do círculo)
    // Lua está visível de 180 a 360 graus (parte inferior do círculo)
    
    if (timeOfDay >= 0 && timeOfDay < 180) {
        // Desenha o sol
        glPushMatrix();
        glTranslatef(x, y, 0);
        making_sun();
        glPopMatrix();
    } else {
        // Desenha a lua (no lado oposto)
        glPushMatrix();
        glTranslatef(x, y, 0);
        making_moon();
        glPopMatrix();
        
        // Desenha algumas estrelas
        glColor3ub(255, 255, 255);
        for(int i = 0; i < 50; i++) {
            float star_x = -800 + (i * 35) % 1600;
            float star_y = 200 + (i * 47) % 400;
            float star_z = -500 + (i * 23) % 1000;
            glPushMatrix();
            glTranslatef(star_x, star_y, star_z);
            glutSolidSphere(2, 10, 10);
            glPopMatrix();
        }
    }
    
    making_sky();
}

// Desenha uma cadeira simples
void draw_chair(float x, float y, float z, float rotation){
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    
    // Assento
    glColor3ub(139, 69, 19);
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glScalef(1.5, 0.2, 1.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Encosto
    glPushMatrix();
    glTranslatef(0, 1, -0.7);
    glScalef(1.5, 2, 0.15);
    glutSolidCube(1);
    glPopMatrix();
    
    // Pernas (4)
    glColor3ub(101, 67, 33);
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        float offsetX = (i % 2) ? 0.6 : -0.6;
        float offsetZ = (i / 2) ? 0.6 : -0.6;
        glTranslatef(offsetX, -0.7, offsetZ);
        glScalef(0.15, 1.4, 0.15);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    glPopMatrix();
}

// Desenha uma lousa/quadro negro
void draw_blackboard(float x, float y, float z, float rotation){
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    
    // Quadro negro
    glColor3ub(30, 40, 30);
    glPushMatrix();
    glScalef(10, 5, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    
    // Moldura inferior (apoio para giz)
    glColor3ub(139, 90, 43);
    glPushMatrix();
    glTranslatef(0, -2.7, 0);
    glScalef(10.5, 0.3, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
}

// Desenha uma porta
void draw_door(float x, float y, float z, float rotation, int isOpen){
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    
    // Moldura da porta
    glColor3ub(101, 67, 33);
    
    // Lateral esquerda
    glPushMatrix();
    glTranslatef(-3.5, 0, 0);
    glScalef(0.4, 7, 0.4);
    glutSolidCube(1);
    glPopMatrix();
    
    // Lateral direita
    glPushMatrix();
    glTranslatef(3.5, 0, 0);
    glScalef(0.4, 7, 0.4);
    glutSolidCube(1);
    glPopMatrix();
    
    // Superior
    glPushMatrix();
    glTranslatef(0, 3.5, 0);
    glScalef(7, 0.4, 0.4);
    glutSolidCube(1);
    glPopMatrix();
    
    // Porta (aberta)
    if(isOpen){
        glPushMatrix();
        glTranslatef(-3.2, 0, -3.2);
        glRotatef(70, 0, 1, 0);
        
        glColor3ub(139, 90, 43);
        glPushMatrix();
        glScalef(6.5, 6.5, 0.2);
        glutSolidCube(1);
        glPopMatrix();
        
        // Maçaneta
        glColor3ub(200, 200, 50);
        glPushMatrix();
        glTranslatef(2.5, 0, 0.3);
        glutSolidSphere(0.2, 10, 10);
        glPopMatrix();
        
        glPopMatrix();
    }
    
    glPopMatrix();
}

// Desenha o interior de uma sala de aula
void draw_classroom_interior(float width, float height, float depth){
    // Lousa na frente
    draw_blackboard(0, 1.5, -depth/2 + 1, 0);
    
    // Mesa do professor
    glColor3ub(139, 90, 43);
    glPushMatrix();
    glTranslatef(0, -1.5, -depth/2 + 5);
    glScalef(5, 0.3, 3);
    glutSolidCube(1);
    glPopMatrix();
    
    // Cadeira do professor
    draw_chair(0, -height/2 + 1.5, -depth/2 + 5, 180);
    
    // Cadeiras dos alunos (fileiras)
    int rows = 3;
    int cols = 3;
    float startX = -width/2 + 8;
    float startZ = -depth/2 + 15;
    float spacingX = (width - 16) / (cols - 1);
    float spacingZ = (depth - 20) / (rows - 1);
    
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            float chairX = startX + col * spacingX;
            float chairZ = startZ + row * spacingZ;
            
            // Mesa do aluno
            glColor3ub(160, 120, 80);
            glPushMatrix();
            glTranslatef(chairX, -1.5, chairZ);
            glScalef(3, 0.2, 2);
            glutSolidCube(1);
            glPopMatrix();
            
            // Cadeira
            draw_chair(chairX, -height/2 + 1.5, chairZ + 1.5, 180);
        }
    }
}

// Desenha uma poltrona de auditório
void draw_auditorium_seat(float x, float y, float z, float rotation){
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    
    // Assento
    glColor3ub(128, 0, 0); // Vermelho escuro
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glScalef(1.2, 0.3, 1.2);
    glutSolidCube(1);
    glPopMatrix();
    
    // Encosto
    glPushMatrix();
    glTranslatef(0, 1, -0.5);
    glScalef(1.2, 2, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    
    // Braços
    glColor3ub(100, 0, 0);
    // Braço esquerdo
    glPushMatrix();
    glTranslatef(-0.6, 0.3, 0);
    glScalef(0.15, 0.6, 1.0);
    glutSolidCube(1);
    glPopMatrix();
    
    // Braço direito
    glPushMatrix();
    glTranslatef(0.6, 0.3, 0);
    glScalef(0.15, 0.6, 1.0);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
}

void audi_c(){
    float width = 80;
    float height = 40;
    float depth = 80;
    
    glPushMatrix();
    glTranslatef(-13 * 20, 20, -10 * 20);
    
    // Piso interno
    glColor3ub(150, 150, 150);
    glPushMatrix();
    glTranslatef(0, -height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Teto
    glColor3ub(200, 200, 200);
    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede de trás (com palco)
    glColor3ub(180, 180, 160);
    glPushMatrix();
    glTranslatef(0, 0, -depth/2);
    glScalef(width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede frontal
    glColor3ub(180, 180, 160);
    glPushMatrix();
    glTranslatef(0, 0, depth/2);
    glScalef(width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede lateral esquerda
    glPushMatrix();
    glTranslatef(-width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede lateral direita
    glPushMatrix();
    glTranslatef(width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // PALCO
    glColor3ub(101, 67, 33);
    glPushMatrix();
    glTranslatef(0, -height/2 + 2, -depth/2 + 15);
    glScalef(width - 10, 4, 25);
    glutSolidCube(1);
    glPopMatrix();
    
    // Cortina do palco
    glColor3ub(139, 0, 0);
    glPushMatrix();
    glTranslatef(0, 5, -depth/2 + 2);
    glScalef(width - 10, 20, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // POLTRONAS ESCALONADAS
    // 6 níveis de altura, cada um um pouco mais alto que o anterior
    int levels = 6;
    int rows_per_level = 2;
    int seats_per_row = 8;
    
    float start_z = -depth/2 + 35;
    float seat_spacing_x = 5;
    float seat_spacing_z = 3.5;
    float level_height_increment = 2.5;
    
    for(int level = 0; level < levels; level++){
        float level_y = -height/2 + 2 + (level * level_height_increment);
        float level_z = start_z + (level * rows_per_level * seat_spacing_z);
        
        for(int row = 0; row < rows_per_level; row++){
            float row_z = level_z + (row * seat_spacing_z);
            
            for(int seat = 0; seat < seats_per_row; seat++){
                float seat_x = -((seats_per_row - 1) * seat_spacing_x) / 2.0 + (seat * seat_spacing_x);
                
                draw_auditorium_seat(seat_x, level_y, row_z, 180);
            }
        }
        
        // Plataforma do nível
        glColor3ub(120, 120, 120);
        glPushMatrix();
        glTranslatef(0, level_y - 1.5, level_z + seat_spacing_z);
        glScalef(seats_per_row * seat_spacing_x + 5, 0.3, rows_per_level * seat_spacing_z + 2);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    glPopMatrix();
}

// class block from 1 to 5
void block1(){
    float width = 40;
    float height = 15;
    float depth = 40;
    
    glPushMatrix();
    glTranslatef(35, 7.5, -35);
    
    // Piso interno
    glColor3ub(180, 180, 180);
    glPushMatrix();
    glTranslatef(0, -height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Teto
    glColor3ub(240, 240, 230);
    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede de trás
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(0, 0, -depth/2);
    glScalef(width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede da frente (com abertura para a porta)
    glPushMatrix();
    glTranslatef(0, 0, depth/2);
    float door_width = 7.0;
    float door_height = 7.0;

    // Parte esquerda da parede
    glPushMatrix();
    glTranslatef(-(width/2 + door_width)/2, 0, 0);
    glScalef(width/2 - door_width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parte direita da parede
    glPushMatrix();
    glTranslatef((width/2 + door_width)/2, 0, 0);
    glScalef(width/2 - door_width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parte superior da parede (sobre a porta)
    glPushMatrix();
    glTranslatef(0, (height/2 + door_height)/2, 0);
    glScalef(door_width*2, height - door_height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
    
    // Porta aberta
    draw_door(0, -height/2 + 3.5, depth/2, 0, 1);
    
    // Parede lateral esquerda (com janelas)
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(-width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Janelas na parede esquerda
    glColor3ub(150, 200, 255);
    for(int i = 0; i < 2; i++){
        glPushMatrix();
        glTranslatef(-width/2, 2, -depth/2 + 15 + i * 20);
        glScalef(0.3, 8, 8);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    // Parede lateral direita
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Interior da sala
    draw_classroom_interior(width, height, depth);
    
    glPopMatrix();
}

// class block from 6 to 10
void block2(){
    float width = 40;
    float height = 15;
    float depth = 40;
    
    glPushMatrix();
    glTranslatef(-35, 7.5, -85);
    
    // Piso interno
    glColor3ub(180, 180, 180);
    glPushMatrix();
    glTranslatef(0, -height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Teto
    glColor3ub(240, 240, 230);
    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede de trás
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(0, 0, -depth/2);
    glScalef(width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede da frente (com abertura para a porta)
    glPushMatrix();
    glTranslatef(0, 0, depth/2);
    float door_width = 7.0;
    float door_height = 7.0;

    // Parte esquerda da parede
    glPushMatrix();
    glTranslatef(-(width/2 + door_width)/2, 0, 0);
    glScalef(width/2 - door_width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parte direita da parede
    glPushMatrix();
    glTranslatef((width/2 + door_width)/2, 0, 0);
    glScalef(width/2 - door_width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parte superior da parede (sobre a porta)
    glPushMatrix();
    glTranslatef(0, (height/2 + door_height)/2, 0);
    glScalef(door_width*2, height - door_height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
    
    // Porta aberta
    draw_door(0, -height/2 + 3.5, depth/2, 0, 1);
    
    // Parede lateral esquerda
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(-width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede lateral direita (com janelas)
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Janelas na parede direita
    glColor3ub(150, 200, 255);
    for(int i = 0; i < 2; i++){
        glPushMatrix();
        glTranslatef(width/2, 2, -depth/2 + 15 + i * 20);
        glScalef(0.3, 8, 8);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    // Interior da sala
    draw_classroom_interior(width, height, depth);
    
    glPopMatrix();
}

//class block from 11 to 15
void block3(){
    float width = 40;
    float height = 15;
    float depth = 40;
    
    glPushMatrix();
    glTranslatef(35, 7.5, -85);
    
    // Piso interno
    glColor3ub(180, 180, 180);
    glPushMatrix();
    glTranslatef(0, -height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Teto
    glColor3ub(240, 240, 230);
    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width, 0.2, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede de trás
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(0, 0, -depth/2);
    glScalef(width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parede da frente (com abertura para a porta)
    glPushMatrix();
    glTranslatef(0, 0, depth/2);
    float door_width = 7.0;
    float door_height = 7.0;

    // Parte esquerda da parede
    glPushMatrix();
    glTranslatef(-(width/2 + door_width)/2, 0, 0);
    glScalef(width/2 - door_width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parte direita da parede
    glPushMatrix();
    glTranslatef((width/2 + door_width)/2, 0, 0);
    glScalef(width/2 - door_width, height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    // Parte superior da parede (sobre a porta)
    glPushMatrix();
    glTranslatef(0, (height/2 + door_height)/2, 0);
    glScalef(door_width*2, height - door_height, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
    
    // Porta aberta
    draw_door(0, -height/2 + 3.5, depth/2, 0, 1);
    
    // Parede lateral esquerda (com janelas)
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(-width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Janelas na parede esquerda
    glColor3ub(150, 200, 255);
    for(int i = 0; i < 2; i++){
        glPushMatrix();
        glTranslatef(-width/2, 2, -depth/2 + 15 + i * 20);
        glScalef(0.3, 8, 8);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    // Parede lateral direita
    glColor3ub(230, 220, 200);
    glPushMatrix();
    glTranslatef(width/2, 0, 0);
    glScalef(0.5, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Interior da sala
    draw_classroom_interior(width, height, depth);
    
    glPopMatrix();
}
// Carrega todos os modelos OBJ
void load_all_models() {
    printf("\n=== Carregando modelos OBJ ===\n");
    
    // Tenta carregar cada modelo disponível
    // Nota: Ajuste os caminhos conforme a estrutura dos seus arquivos OBJ
    
    // Ar condicionado
    model_air_cond = load_obj("assets/air-cond/pc.obj");
    
    // Cadeira de auditório
    model_aud_chair = load_obj("assets/aud-chair/aud-chair.obj");
    
    // Cadeira
    model_chair = load_obj("assets/chair_h/chair_h.obj");
    
    // Mesa/carteira escolar
    model_desk = load_obj("assets/deskschool/school chair.obj");
    
    // Ventilador de teto
    model_fan = load_obj("assets/fan/fan.obj");
    
    // Computador/PC
    model_pc = load_obj("assets/pc/pc.obj");
    
    printf("=== Modelos carregados ===\n\n");
}

// Libera todos os modelos da memória
void free_all_models() {
    free_obj_model(model_air_cond);
    free_obj_model(model_aud_chair);
    free_obj_model(model_blackboard_big);
    free_obj_model(model_blackboard_small);
    free_obj_model(model_chair);
    free_obj_model(model_desk);
    free_obj_model(model_extintor);
    free_obj_model(model_fan);
    free_obj_model(model_pc);
    free_obj_model(model_toilet);
}

// Inicializa parmetros de rendering
void Inicializa(void)
{
    // Define a cor de fundo da janela de visualizao como preta
    glClearColor(0, 255, 255, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Habilita iluminação para um cenário mais realista
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Configura luz ambiente
    GLfloat light_ambient[] = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0.0, 200.0, 0.0, 1.0};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    // Carrega os modelos OBJ
    load_all_models();
    
    glutWarpPointer(400,300);
    glutSetCursor(GLUT_CURSOR_NONE);
}

void DISPLAY(void)
{
    // Atualiza o ciclo dia/noite (infinito)
    timeOfDay -= 0.5; // Velocidade do ciclo (menor = mais lento)
    timeOfDay = fmod(timeOfDay + 360, 360); // Mantém entre 0 e 360
    
    glMatrixMode(GL_PROJECTION);/*glMatrixMode()- define qual matriz ser� alterada. SEMPRE defina o tipo de apresenta��o
                              (Ortogonal ou Perspectiva) na matriz PROJECTION.*/
    glLoadIdentity();//"Limpa" ou "transforma" a matriz em identidade, reduzindo poss�veis erros.
    if (projecao==1)
    {
        glOrtho(-200, 200, -200, 200, -200, 200);  //Define a proje��o como ortogonal
        if (rotacao==1)
            glRotatef(angulox,1.0,0.0,0.0);
        if (rotacao==2)
            glRotatef(anguloy,0.0,1.0,0.0);
    }
    if (projecao == 0)
    {
        gluPerspective(90,1,1,2000); //Define a proje��o como perspectiva
        if (rotacao==1)
            glRotatef(angulox,1.0,0.0,0.0);
        if (rotacao==2)
            glRotatef(anguloy,0.0,1.0,0.0);
    }

    glMatrixMode(GL_MODELVIEW);/*glMatrixMode()- define qual matriz ser� alterada. SEMPRE defina a c�mera
                              (Ortogonal ou Perspectiva) na matriz MODELVIEW (onde o desenho ocorrer�).*/
    glLoadIdentity(); ////"Limpa" ou "transforma" a matriz em identidade, reduzindo poss�veis erros.

    gluLookAt(posx,posy,posz,ox,oy,oz,lx,ly,lz); //Define a pos da c�mera, para onde olha e qual eixo est� na vertical.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* "limpa" um buffer particular ou combina��es de buffers,
                                                         onde buffer � uma �rea de armazenamento para informa��es da imagem.
                                                        Nesse caso, est� "limpando os buffers para suportarem anima��es */
    DesenharCena();
    glutSwapBuffers();
    glutPostRedisplay();
}

void DesenharCena()
{
    // Ambiente (céu, sol, lua)
    making_environment();
    
    //Making Audi C
    audi_c();

    //making class blocks
    block1();
    block2();
    block3();
    
    // Desenha alguns modelos OBJ como exemplo
    // Você pode posicionar e escalar conforme necessário
    
    // Exemplo: Desenha um ventilador no teto da primeira sala
    if (model_fan) {
        glPushMatrix();
        glTranslatef(35, 14, -35);  // Posição no teto do block1
        glScalef(0.5, 0.5, 0.5);
        glColor3ub(150, 150, 150);
        draw_obj_model(model_fan);
        glPopMatrix();
    }
    
    // Exemplo: Desenha um extintor na parede
    if (model_extintor) {
        glPushMatrix();
        glTranslatef(50, 0, -35);
        glScalef(2, 2, 2);
        glColor3ub(255, 0, 0);
        draw_obj_model(model_extintor);
        glPopMatrix();
    }
    
    // Exemplo: Desenha um PC na mesa do professor
    if (model_pc) {
        glPushMatrix();
        glTranslatef(35, 0, -48);  // Na mesa do professor
        glScalef(0.3, 0.3, 0.3);
        glColor3ub(80, 80, 80);
        draw_obj_model(model_pc);
        glPopMatrix();
    }
}

void MOUSE_Button(int botao, int estado, int x, int y)
{
    //bot�o - recebe o c�digo do bot�o pressionado
    //estado - recebe se est� pressionado ou n�o
    //x, y - recebem respectivamente as posi��es do cursor do mouse
    switch (botao)
    {
    case GLUT_LEFT_BUTTON:
        if (estado == GLUT_DOWN)
        {
            projecao=1;
            posx=0;
            posy=0;
            posz=20;
            ox=0,oy=0,oz=0;
            lx=0, ly=1,lz=0;
            glutPostRedisplay();
        }
        break;

    case GLUT_RIGHT_BUTTON:
        if(estado == GLUT_DOWN)
        {
            projecao=0;
            posx=0;
            posy=10;
            posz=50;
            oy=10;
            ox=0;
            oz=0;
            lx=0, ly=1, lz=0;
            glutPostRedisplay();
        }
        break;
    }
}

void MOUSE(int x, int y)
{
    if (x > x_temp)
    {
        anguloy=anguloy+(x/100);
        if(anguloy > 360.0)
            anguloy = anguloy-360;
        rotacao = 2;
        glutPostRedisplay();
    }
    if (x < x_temp)
    {
        anguloy=anguloy-(x/100);
        if(anguloy > 360.0)
            anguloy = anguloy-360;
        rotacao = 2;
        glutPostRedisplay();
    }
    x_temp=x;
    if (y>y_temp)
    {
        angulox=angulox+(y/100);
        if(angulox > 360.0)
            angulox = angulox-360;
        rotacao = 1;
       glutPostRedisplay();
    }
    if (y<y_temp)
    {
        angulox=angulox-(y/100);
        if(angulox > 360.0)
            angulox = angulox-360;
        rotacao = 1;
        glutPostRedisplay();
    }
    y_temp = y;
    if(x>=500 || x<=300 || y>=400 || y<=200) //delimitar uma area de acesso ao mouse de 600x450
    {
        glutWarpPointer(400,300);
        glutPostRedisplay();
    }
    
}

void keyboard(unsigned char tecla, int x, int y)
{
    switch(tecla)
    {
    case 's':
        posx=posx+(5*cos((anguloy+90)/57.32));
        posz=posz+(5*sin((anguloy+90)/57.32));
        ox=ox+(5*cos((anguloy+90)/57.32));
        oz=oz+(5*sin((anguloy+90)/57.32));
        glutPostRedisplay();
        break;

    case 'w':
        posx=posx-(5*cos((anguloy+90)/57.32));
        posz=posz-(5*sin((anguloy+90)/57.32));
        ox=ox-(5*cos((anguloy+90)/57.32));
        oz=oz-(5*sin((anguloy+90)/57.32));
        glutPostRedisplay();
        break;

    case 'd':
        posx=posx+(5*sin((anguloy+90)/57.32));
        posz=posz-(5*cos((anguloy+90)/57.32));
        ox=ox+(5*sin((anguloy+90)/57.32));
        oz=oz-(5*cos((anguloy+90)/57.32));
        glutPostRedisplay();
        break;

    case 'a':
        posx=posx-(5*sin((anguloy+90)/57.32));
        posz=posz+(5*cos((anguloy+90)/57.32));
        ox=ox-(5*sin((anguloy+90)/57.32));
        oz=oz+(5*cos((anguloy+90)/57.32));
        glutPostRedisplay();
        break;
    case 'e':
      rotating++;
      if(rotating>=0)
        rotating=0;
      break;
    case 'r':
      rotating--;
      if(rotating<=-90)
        rotating=-90;
      break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void TeclasEspeciais (int key, int x, int y)
{
    //Key - recebe o c�digo ASCII da tecla
    //x, y - recebem respectivamente as posi��es do cursor do mouse (permite tratar os dois dispositivos)
    if (key==GLUT_KEY_PAGE_UP)
    {
        posy+=5;
        oy+=5;
    }
    else if (key==GLUT_KEY_PAGE_DOWN)
    {
        posy-=5;
        oy-=5;
    }
    glutPostRedisplay();
}

int main(int argc, char**argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL); /*Define as caracter�sticas do espa�o vetorial.
                                                                        //  Nesse caso, permite anima��es (sem cintila��es), cores compostas por Verm. Verde e Azul,
                                                                //  Buffer que permite trablhar com profundidade e elimina faces escondidas.*/
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("IBILCE Block C - OpenGL");
    Inicializa();
    glutDisplayFunc(DISPLAY);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(MOUSE_Button);
    glutPassiveMotionFunc(MOUSE);
    glutSpecialFunc(TeclasEspeciais);
    glutMainLoop();
    
    // Libera modelos antes de sair
    free_all_models();
    
    return 0;
}
