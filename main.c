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
int rotating=0, timeOfDay=0, skyColor=255, chairList=0;
float p[3] = { -122.5, -10, -20 };

void making_enviroment();
void making_sun();
void making_sky();

void stroke_output(GLfloat x, GLfloat y, char *format,...);

void DesenharCena();

void making_class_block(float x, float y, float z, int i);
void making_door(float x, float y, float z);

//Declara��o de Vari�veis Globis
int projecao=0; //Vari�vel L�gica para Definir o Tipo de Proje��o (Perspectiva ou Ortogonal)
//Original 0 30 200
//       |
//      V
float posx=0, posy=30, posz=200; //Vari�veis que definem a posi��o da c�mera
float oy=30,ox=0,oz=0;         //Vari�veis que definem para onde a c�mera olha
int lx=0, ly=1,  lz=0;     //Vari�veis que definem o eixo da c�mera


GLfloat angulox=0.0;
GLfloat anguloy=0.0;

int x_temp=-1; //Armazena posi��o X do ponteiro do mouse
int y_temp=-1; //Armazena posi��o Y do ponteiro do mouse
int rotacao = 0; //Controla eixo de rota��o do mouse
int i;//controle do for

void making_sun(){
    glTranslatef(-40, 300, 0);
  glColor3ub(255, 165, 0);
  gluSphere(gluNewQuadric(), 40, 100, 100);
}

void making_sky(){
  glPushMatrix();
  glColor3ub(0, skyColor, skyColor);
  glutSolidCube(2000);
  glPopMatrix();
}

// --- FUNÇÃO PORTA DUPLA ---
void making_double_door(float x, float y, float z){
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // 1. Batente / Fundo Escuro
    glPushMatrix();
    glColor3ub(80, 80, 80); 
    glScalef(0.65, 1.35, 0.05); 
    glutSolidCube(20);
    glPopMatrix();

    // 2. Folha da Esquerda
    glPushMatrix();
    glTranslatef(-3.2, 0, 0.2); 
    glColor3ub(185, 182, 181); 
    glScalef(0.3, 1.25, 0.05);
    glutSolidCube(20);
    glPopMatrix();

    // 3. Folha da Direita
    glPushMatrix();
    glTranslatef(3.2, 0, 0.2); 
    glColor3ub(185, 182, 181); 
    glScalef(0.3, 1.25, 0.05);
    glutSolidCube(20);
    glPopMatrix();

    // 4. Detalhes / Maçanetas
    glPushMatrix();
    glTranslatef(-0.5, 0, 0.8); 
    glColor3ub(255, 215, 0); 
    glutSolidSphere(0.4, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5, 0, 0.8); 
    glColor3ub(255, 215, 0); 
    glutSolidSphere(0.4, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

// Desenha Cadeira
void draw_auditorium_chair(float x, float y, float z, float rotY) {
    if (chairList == 0) {
        chairList = glGenLists(1);
        glNewList(chairList, GL_COMPILE);

        // 1. Pernas 
        glColor3ub(50, 50, 50);
        glPushMatrix(); glTranslatef(-1.2, 1.0, -1.2); glScalef(0.4, 2.0, 0.4); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef(1.2, 1.0, -1.2); glScalef(0.4, 2.0, 0.4); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef(-1.2, 1.0, 1.2); glScalef(0.4, 2.0, 0.4); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef(1.2, 1.0, 1.2); glScalef(0.4, 2.0, 0.4); glutSolidCube(1.0); glPopMatrix();

        // 2. Assento
        glColor3ub(160, 40, 40);
        glPushMatrix(); glTranslatef(0, 2.2, 0); glScalef(3.0, 0.4, 3.2); glutSolidCube(1.0); glPopMatrix();

        // 3. Encosto
        glPushMatrix(); glTranslatef(0, 4.0, -1.4); glScalef(3.0, 3.0, 0.4); glutSolidCube(1.0); glPopMatrix();
        
        glEndList();
    }

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotY, 0, 1, 0);
    glScalef(1.2, 1.2, 1.2); 
    glCallList(chairList);
    glPopMatrix();
}

void making_door(float x, float y, float z){

    //Porta
    glPushMatrix();
    glColor3ub(185, 182, 181);
    glTranslatef(x, y, z);
    glScalef(0.3, 1, 0.05);
    glutSolidCube(20);
    glPopMatrix();

    //Parte de cima da porta
    glPushMatrix();
    glColor3ub(185, 182, 181);
    glTranslatef(x, y+13.1, z);
    glScalef(0.3, 0.3, 0.05);
    glutSolidCube(20);
    glPopMatrix();

    //Trava
    glPushMatrix();
    glColor3ub(57, 57, 57);
    glTranslatef(x+1.8, y, z+0.4);
    glScaled(1, 1, 0.3);
    glutSolidCube(2);
    glPopMatrix();

    //Macaneta
    glPushMatrix();
    glTranslatef(x+2, y-0.1, z+0.8);
    glScaled(0.5, 0.5, 0.3);
    glColor3ub(245, 174, 31);
    gluSphere(gluNewQuadric(), 0.6, 100,100);
    glPopMatrix();
}

void making_pillar(float x, float y, float z){
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.25, 3.2, 0.2);
    glColor3ub(180, 180, 180);
    glutSolidCube(20);
    glPopMatrix();
}

void back_block(float x, float y, float z){
    //Parede de baixo
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(2.65, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();
    
    //Parede de cima
    glPushMatrix();
    glTranslatef(x, y+29, z);
    glScalef(2.65, 0.1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Janela
    //Borda de cima
    glPushMatrix();
    glTranslatef(x, y+27.5, z);
    glScalef(2.65, 0.05, 0.05);
    glColor3ub(70, 17, 149);
    glutSolidCube(20);
    glPopMatrix();

    //Borda de baixo
    glPushMatrix();
    glTranslatef(x, y+10.5, z);
    glScalef(2.65, 0.05, 0.05);
    glColor3ub(70, 17, 149);
    glutSolidCube(20);
    glPopMatrix();

    //Borda esquerda
    glPushMatrix();
    glTranslatef(x-26, y+18, z);
    glScalef(0.05, 0.9, 0.04);
    glColor3ub(70, 17, 149);
    glutSolidCube(20);
    glPopMatrix();

    //Borda direita
    glPushMatrix();
    glTranslatef(x+25, y+18, z);
    glScalef(0.05, 0.9, 0.04);
    glColor3ub(70, 17, 149);
    glutSolidCube(20);
    glPopMatrix();

    //Haste do meio
    glPushMatrix();
    glTranslatef(x, y+18, z);
    glScalef(0.05, 0.9, 0.04);
    glColor3ub(70, 17, 149);
    glutSolidCube(20);
    glPopMatrix();

    //Barra horizontal de cima
    glPushMatrix();
    glTranslatef(x, y+21.9, z);
    glScalef(2.65, 0.05, 0.05);
    glColor3ub(70, 17, 149);
    glutSolidCube(20);
    glPopMatrix();

    //Barra horizontal de baixo
    glPushMatrix();
    glTranslatef(x, y+16.3, z);
    glScalef(2.65, 0.05, 0.05);
    glColor3ub(70, 17, 149);
    glutSolidCube(20);
    glPopMatrix();
}

void making_conditional_air(float x, float y, float z){
    //caixote
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.4, 0.3, 0.3);
    glColor3ub(200, 200, 200);
    glutSolidCube(20);
    glPopMatrix();

    //fundo da helice
    glPushMatrix();
    glTranslatef(x+1.2, y, z-3);
    glScalef(1, 1, 0.01);
    glColor3ub(100, 100, 100);
    gluSphere(gluNewQuadric(), 2.6, 100,100);
    glPopMatrix();

    //helice
    glPushMatrix();
    glTranslatef(x+1.2, y, z-3);
    glScalef(0.1, 1, 0.1);
    glRotatef(200, 0, 0, 2);
    glColor3ub(150, 150, 150);
    glutSolidCube(4);
    glPopMatrix();
}

void making_class_block(float x, float y, float z, int block_number){
    
    //Parede de tras
    back_block(x-73, y, z);
    back_block(x-22.5, y, z);
    back_block(x+28, y, z);
    back_block(x+78.5, y, z);
    back_block(x+129, y, z);
    back_block(x+170, y, z);

    //Muretinha em cima do ar da parede de tras
    glPushMatrix();
    glTranslatef(x+50, y+10.5, z-2);
    glScalef(14.7, 0.05, 0.2);
    glColor3ub(180, 180, 180);
    glutSolidCube(20);
    glPopMatrix();

    //Pillares de tras
    making_pillar(x-98, y, z-2);
    making_pillar(x-48, y, z-2);
    making_pillar(x+2, y, z-2);
    making_pillar(x+52, y, z-2);
    making_pillar(x+78, y, z-2);
    making_pillar(x+102, y, z-2);
    making_pillar(x+126, y, z-2);
    making_pillar(x+152, y, z-2);
    making_pillar(x+175, y, z-2);
    making_pillar(x+198, y, z-2);

    //Correcoes
    glPushMatrix();
    glTranslatef(x+152, y, z+1);
    glScalef(0.25, 3.2, 0.01);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x+175, y, z+1);
    glScalef(0.25, 3.2, 0.01);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();
    
    //Parede da esquerda
    glPushMatrix();
    glTranslatef(x-100, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

   //Telhado banheiro
    glPushMatrix();
    glColor3ub(228, 228, 228);
    glTranslatef(x-155, y+38, z+50);
    glScalef(0.9, 0.15, 1.6);
    glutSolidCube(100);
    glPopMatrix();

    //Chao Banheiro
    glPushMatrix();
    glColor3ub(140, 140, 140);
    glTranslatef(x-170, y-12.3, z+50);
    glScalef(0.5, 0.05, 1);
    glutSolidCube(100);
    glPopMatrix();

    //Pia banheiro fem
    glPushMatrix();
    glColor3ub(60, 60, 60);
    glTranslatef(x-189, y-3.8, z+50);
    glScalef(0.5, 0.2, 3);
    glutSolidCube(20);
    glPopMatrix();

    

    //Parede da frente banheiro
    glPushMatrix();
    glTranslatef(x-145, y-3.5, z+40);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    making_door(0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-145, y-3.5, z+60);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    making_door(0, 0, 0);
    glPopMatrix();

    
    glPushMatrix();
    glTranslatef(x-145, y, z+50);
    glScalef(0.04, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

     //Parede do fundo banheiro
    glPushMatrix();
    glTranslatef(x-194, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Pillar banheiro
    making_pillar(x-145, y, z+102);
    making_pillar(x-169, y, z+102);
    making_pillar(x-193, y, z+102);
    making_pillar(x-145, y, z-2);
    making_pillar(x-169, y, z-2);
    making_pillar(x-193, y, z-2);

    //privada
    

    //Parede da esquerda banheiro - parte 1
    

    glPushMatrix();
    glTranslatef(x-169, y, z+100);
    glScalef(2.5, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-93, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Parede da direita banheiro - parte 1
    

    glPushMatrix();
    glTranslatef(x-169, y, z);
    glScalef(2.5, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-93, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Divisoria banheiro - parte 1
    

    glPushMatrix();
    glTranslatef(x-169, y, z+50);
    glScalef(2.5, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-93, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Box
    glPushMatrix();
    glTranslatef(x-153, y+4, z+1);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-153, y+4, z+11);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-162, y+4, z+5);
    glRotatef(50.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-153, y+4, z+21);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-160, y+4, z+16);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

     glPushMatrix();
    glTranslatef(x-153, y+4, z+31);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-160, y+4, z+27);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();
    
//
    glPushMatrix();
    glTranslatef(x-153, y+4, z+70);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-153, y+4, z+80);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-162, y+4, z+74);
    glRotatef(50.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-153, y+4, z+90);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-160, y+4, z+84);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

     glPushMatrix();
    glTranslatef(x-153, y+4, z+100);
    glScalef(0.7, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-160, y+4, z+95);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 1, 0.05);
    glColor3ub(195, 195, 195);
    glutSolidCube(20);
    glPopMatrix();

    //Parede da direita
    glPushMatrix();
    glTranslatef(x+200, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Paredes do meio - 1
    glPushMatrix();
    glTranslatef(x-47, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Paredes do meio - 2
    glPushMatrix();
    glTranslatef(x+3, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Paredes do meio - 3
    glPushMatrix();
    glTranslatef(x+53, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Paredes do meio - 4
    glPushMatrix();
    glTranslatef(x+128, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Parede da frente - parte 1
    making_door(x-93, y-3.5, z+99);

    glPushMatrix();
    glTranslatef(x-70, y, z+100);
    glScalef(2, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-93, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Pillar 1 - 3
    making_pillar(x-98, y, z+102);
    making_pillar(x-72, y, z+102);
    making_pillar(x-48, y, z+102);

    //Parede da frente - parte 2
    making_door(x-43, y-3.5, z+99);

    glPushMatrix();
    glTranslatef(x-20, y, z+100);
    glScalef(2, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x-43, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Pillar 4 - 5
    making_pillar(x-22, y, z+102);
    making_pillar(x+2, y, z+102);

    //Parede da frente - parte 3
    making_door(x+7, y-3.5, z+99);

    glPushMatrix();
    glTranslatef(x+30, y, z+100);
    glScalef(2, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x+7, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Pillar 6 - 7
    making_pillar(x+28, y, z+102);
    making_pillar(x+52, y, z+102);

    //Parede da frente - parte 4
    making_door(x+57, y-3.5, z+99);
    
    glPushMatrix();
    glTranslatef(x+80, y, z+100);
    glScalef(2, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x+57, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x+104, y, z+100);
    glScalef(2, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Pillar 8 - 10
    making_pillar(x+78, y, z+102);
    making_pillar(x+102, y, z+102);
    making_pillar(x+126, y, z+102);

    //Parede da frente - parte 5
    making_door(x+131, y-3.5, z+99);

    glPushMatrix();
    glTranslatef(x+154, y, z+100);
    glScalef(2, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x+131, y+22.5, z+99);
    glScalef(0.3, 1, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x+178, y, z+100);
    glScalef(2, 3, 0.05);
    glColor3ub(209, 80, 6);
    glutSolidCube(20);
    glPopMatrix();

    //Pillar 11 - 13
    making_pillar(x+152, y, z+102);
    making_pillar(x+175, y, z+102);
    making_pillar(x+198, y, z+102);

    //Telhado
    glPushMatrix();
    glColor3ub(228, 228, 228);
    glTranslatef(x+50, y+38, z+50);
    glScalef(3.2, 0.15, 1.6);
    glutSolidCube(100);
    glPopMatrix();

    //Ares condicionados
    making_conditional_air(x-55, y+2, z-3);
    making_conditional_air(x-41, y+2, z-3);
    making_conditional_air(x+9, y+2, z-3);
    making_conditional_air(x+119, y+2, z-3);
    making_conditional_air(x+133, y+2, z-3);

    //Piso
    glPushMatrix();
    glColor3ub(50, 50, 50);
    glTranslatef(x+50, y-9.9, z+50);
    glScalef(3, 0, 1);
    glutSolidCube(100);
    glPopMatrix();

    OBJModel *cadeira = load_obj("../assets/chair_h/chair_h.obj");
    for(int i=0; i<5;i++){
        for(int j=0; j<5;j++){
            glPushMatrix();
            glColor3ub(255, 204, 102);
            glRotatef(-90, 0, 1, 0);
            glTranslatef(x-(j*10), y-10, z+62.5-(i*10));
            glScalef(5, 5, 5);
            draw_obj_model(cadeira);
            glPopMatrix();
        }
    }

    for(int i=0; i<5;i++){
        for(int j=0; j<5;j++){
            glPushMatrix();
            glColor3ub(255, 204, 102);
            glRotatef(-90, 0, 1, 0);
            glTranslatef(x-(j*10), y-10, z+112.5-(i*10));
            glScalef(5, 5, 5);
            draw_obj_model(cadeira);
            glPopMatrix();  
        }
    }

    for(int i=0; i<5;i++){
        for(int j=0; j<5;j++){
            glPushMatrix();
            glColor3ub(255, 204, 102);
            glRotatef(-90, 0, 1, 0);
            glTranslatef(x-(j*10), y-10, z+162.5-(i*10));
            glScalef(5, 5, 5);
            draw_obj_model(cadeira);
            glPopMatrix();  
        }
    }

    OBJModel *mesa_cadeira = load_obj("../assets/deskschool/school chair.obj");
    for(int i=0; i<5;i++){
        for(int j=0; j<3;j++){
            glPushMatrix();
            glColor3ub(255, 204, 102);
            glTranslatef(x+110-(i*10), y-7, z+60-(j*20));
            glScalef(2, 2, 2);
            draw_obj_model(mesa_cadeira);
            glPopMatrix();
        }
    }

    for(int i=0; i<5;i++){
        for(int j=0; j<3;j++){
            glPushMatrix();
            glColor3ub(255, 204, 102);
            glTranslatef(x+185-(i*10), y-7, z+60-(j*20));
            glScalef(2, 2, 2);
            draw_obj_model(mesa_cadeira);
            glPopMatrix();
        }
    }

    OBJModel *lousa = load_obj("../assets/chalkboard/ChalkBoard.obj");
    glPushMatrix();
    glColor3ub(24, 104, 7);
    glTranslatef(x+30, y+4, z+99);
    glScalef(5, 5, 5);
    draw_obj_model(lousa);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(24, 104, 7);
    glTranslatef(x-20, y+4, z+99);
    glScalef(5, 5, 5);
    draw_obj_model(lousa);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(24, 104, 7);
    glTranslatef(x-70, y+4, z+99);
    glScalef(5, 5, 5);
    draw_obj_model(lousa);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(24, 104, 7);
    glTranslatef(x+90, y+4, z+99);
    glScalef(7, 5, 5);
    draw_obj_model(lousa);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(24, 104, 7);
    glTranslatef(x+170, y+4, z+99);
    glScalef(7, 5, 5);
    draw_obj_model(lousa);
    glPopMatrix();

    //Plaquinha de bloco
    glPushMatrix();
    glTranslatef(x-84.37, y+4, z+101);
    glScalef(0.23, 0.12, 0.02);
    glColor3ub(0, 0, 0);
    glutSolidCube(20);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(204, 153, 0);
    glTranslatef(x-86, y+4, z+102);
    glScalef(0.15, 0.12, 1);
    stroke_output(0, 0, "Bloco %d", block_number);
    glPopMatrix();
}

void making_auditorium(float x, float y, float z){
    glPushMatrix();
    
    glTranslatef(x, y, z);
    glScalef(0.78, 0.88, 0.78); 

    // --- Estrutura Básica ---
    // Parede do Fundo
    glPushMatrix(); glTranslatef(0, 0, -50); glScalef(8, 3, 0.05); glColor3ub(209, 80, 6); glutSolidCube(20); glPopMatrix();
    
    // Teto
    glPushMatrix(); 
    glTranslatef(0, 29, 0); 
    glScalef(8, 0.1, 5); 
    glColor3ub(228, 228, 228);
    glutSolidCube(20); 
    glPopMatrix();

    // Piso do Auditório
    glPushMatrix();
    glTranslatef(0, -5.5, 0);
    glScalef(8, 0.1, 5);      
    glColor3ub(128, 128, 128); // Cinza
    glutSolidCube(20);
    glPopMatrix();

    // Paredes Laterais
    glPushMatrix(); glTranslatef(-80, 0, 0); glScalef(0.05, 3, 5); glColor3ub(209, 80, 6); glutSolidCube(20); glPopMatrix();
    glPushMatrix(); glTranslatef(80, 0, 0); glScalef(0.05, 3, 5); glColor3ub(209, 80, 6); glutSolidCube(20); glPopMatrix();
    
    // Fachada Frontal
    glPushMatrix(); glTranslatef(-37.5, 0, 50); glScalef(4.25, 3, 0.05); glColor3ub(209, 80, 6); glutSolidCube(20); glPopMatrix();
    glPushMatrix(); glTranslatef(52.5, 0, 50); glScalef(2.75, 3, 0.05); glColor3ub(209, 80, 6); glutSolidCube(20); glPopMatrix();
    glPushMatrix(); glTranslatef(15, 20, 50); glScalef(1.0, 1, 0.05); glColor3ub(209, 80, 6); glutSolidCube(20); glPopMatrix();

    // Pilares
    making_pillar(-80, 0, 52); making_pillar(80, 0, 52);  
    making_pillar(-80, 0, -48); making_pillar(80, 0, -48);
    making_pillar(2, 0, 52); 
    making_pillar(28, 0, 52);

    // Portas Duplas
    making_double_door(9, -3.5, 49); 
    making_double_door(21, -3.5, 49);  

    // --- LOUSA (VERDE ESCURO) ---
    glPushMatrix();
    glTranslatef(79, 8, 0); 
    glPushMatrix(); glScalef(0.1, 0.8, 2.5); glColor3ub(47, 69, 56); glutSolidCube(20); glPopMatrix(); // Moldura
    glPushMatrix(); glTranslatef(-0.2, 0, 0); glScalef(0.05, 0.75, 2.45); glColor3ub(30, 90, 30); glutSolidCube(20); glPopMatrix(); // Tela
    glPopMatrix();

    // --- MESA E CADEIRA DO PROFESSOR ---
    // Mesa em frente à lousa
    glPushMatrix();
    glTranslatef(55, -2.0, 0); 
    
    // Tampo da Mesa (Reduzido 3.0 comprimento, 1.2 largura)
    glPushMatrix();
    glScalef(1.2, 0.1, 3.0); 
    glColor3ub(139, 69, 19); // Madeira Marrom
    glutSolidCube(10);
    glPopMatrix();

    // Pé Esquerdo
    glPushMatrix();
    glTranslatef(0, -1.5, -11);
    glScalef(1.0, 3.0, 0.5);
    glColor3ub(100, 50, 0);
    glutSolidCube(1);
    glPopMatrix();

    // Pé Direito
    glPushMatrix();
    glTranslatef(0, -1.5, 11);
    glScalef(1.0, 3.0, 0.5);
    glColor3ub(100, 50, 0);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix(); // Fim Mesa

    // Cadeira do Professor (Atrás da mesa)
    draw_auditorium_chair(62, -5.2f, 0, -90.0f);


    // --- DESKTOP NO CANTO ---
    glPushMatrix();
    glTranslatef(75, -5.0, -40); // Base no chão

    // Mesa do PC
    glPushMatrix();
    glTranslatef(0, 2.5, 0);
    glScalef(2.0, 0.1, 3.0);
    glColor3ub(50, 50, 50); // Cinza escuro
    glutSolidCube(5);
    glPopMatrix();
    
    // Pé da Mesa PC
    glPushMatrix();
    glTranslatef(4, 0, 0); // Perto da parede
    glScalef(0.2, 2.5, 2.8);
    glutSolidCube(5);
    glPopMatrix();

    // Gabinete (Desktop Tower) - Preto
    glPushMatrix();
    glTranslatef(-2, 3.5, -4); // Em cima da mesa, canto
    glScalef(0.8, 1.5, 1.5);
    glColor3ub(10, 10, 10); // Preto Quase total
    glutSolidCube(3);
    glPopMatrix();

    // Monitor - Preto
    glPushMatrix();
    glTranslatef(-1, 3.5, 2); 
    glRotatef(-45, 0, 1, 0);  // Virado para o usuário
    
    // Tela
    glPushMatrix();
    glScalef(0.2, 1.2, 1.8);
    glColor3ub(20, 20, 20); 
    glutSolidCube(3);
    glPopMatrix();
    
    // Base Monitor
    glPushMatrix();
    glTranslatef(0, -1.8, 0);
    glScalef(0.5, 0.1, 0.8);
    glColor3ub(20, 20, 20);
    glutSolidCube(3);
    glPopMatrix();
    glPopMatrix(); // Fim Monitor

    glPopMatrix(); // Fim Setup PC


    // --- CADEIRAS DA AUDIÊNCIA ---
    float floorY = -5.2f; 
    
    float startZ = -40.0f; 
    float endZ = 40.0f;   
    float stepZ = 10.0f;   

    float startX = -70.0f; 
    float endX = 25.0f;    
    float stepX = 9.0f;    

    for (float xPos = startX; xPos <= endX; xPos += stepX) {
        for (float zPos = startZ; zPos <= endZ; zPos += stepZ) {
            draw_auditorium_chair(xPos, floorY, zPos, 90.0f);
        }
    }

    glPopMatrix();
}

void stroke_output(GLfloat x, GLfloat y, char *format,...)//fun��o para escrever em 3d
{
    va_list args;
    char buffer[200], *p;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    for (p = buffer; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}

// Inicializa par�metros de rendering
void Inicializa(void)
{
    // Define a cor de fundo da janela de visualiza��o como preta
    glClearColor(0, 255, 255, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutWarpPointer(400,300);
    glutSetCursor(GLUT_CURSOR_NONE);
}

void desenhaCilindro(float altura, float raio) {
    float x, z, angulo;
    
    // Tubo (Lateral)
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 20; i++) {
        angulo = i * 2.0f * 3.14159f / 20.0f; // 360 graus (2*PI)
        x = cos(angulo) * raio;
        z = sin(angulo) * raio;
        glNormal3f(x/raio, 0.0f, z/raio);
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, altura, z);
    }
    glEnd();

    // Tampa Superior
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glVertex3f(0.0f, altura, 0.0f);
    for (int i = 0; i <= 20; i++) {
        angulo = -i * 2.0f * 3.14159f / 20.0f; // Inverso para a normal ficar pra cima
        x = cos(angulo) * raio;
        z = sin(angulo) * raio;
        glVertex3f(x, altura, z);
    }
    glEnd();
}

void desenhaMurinho(float comp, float larg_base, float larg_topo, float altura) {
    float margem = (larg_topo - larg_base) / 2.0;
    float profundidade_extra = 10.0;
    
    // TRUQUE PARA CORRIGIR O BUG VISUAL:
    // Aumentamos a largura da parte de baixo em 0.1 para ela ficar 
    // na frente da parede cinza da fundação e evitar o "Z-Fighting"
    float sobreposicao = 0.1; 

    // 1. Extensão Inferior (Com o truque da sobreposicao)
    glBegin(GL_QUADS);
    glColor3f(0.65, 0.3, 0.25); 

    // Lateral Externa (Direita) - Empurramos um pouco pra fora (+ sobreposicao)
    glVertex3f( (larg_base/2) + sobreposicao, 0, 0);
    glVertex3f( (larg_base/2) + sobreposicao, 0, -comp);
    glVertex3f( (larg_base/2) + sobreposicao, -profundidade_extra, -comp);
    glVertex3f( (larg_base/2) + sobreposicao, -profundidade_extra, 0);

    // Lateral Interna (Esquerda) - Empurramos um pouco pra fora (- sobreposicao)
    glVertex3f( -(larg_base/2) - sobreposicao, 0, 0);
    glVertex3f( -(larg_base/2) - sobreposicao, 0, -comp);
    glVertex3f( -(larg_base/2) - sobreposicao, -profundidade_extra, -comp);
    glVertex3f( -(larg_base/2) - sobreposicao, -profundidade_extra, 0);

    // Frente (fechamento inferior)
    glVertex3f( -(larg_base/2) - sobreposicao, 0, 0);
    glVertex3f( (larg_base/2) + sobreposicao, 0, 0);
    glVertex3f( (larg_base/2) + sobreposicao, -profundidade_extra, 0);
    glVertex3f( -(larg_base/2) - sobreposicao, -profundidade_extra, 0);

    // Trás (fechamento inferior)
    glVertex3f( -(larg_base/2) - sobreposicao, 0, -comp);
    glVertex3f( (larg_base/2) + sobreposicao, 0, -comp);
    glVertex3f( (larg_base/2) + sobreposicao, -profundidade_extra, -comp);
    glVertex3f( -(larg_base/2) - sobreposicao, -profundidade_extra, -comp);
    glEnd();

    // 2. Base do Muro (Parte visível normal)
    glBegin(GL_QUADS);
    // (Cor já definida)
    
    // Frente
    glVertex3f(-larg_base/2, 0, 0);
    glVertex3f( larg_base/2, 0, 0);
    glVertex3f( larg_base/2, altura, 0);
    glVertex3f(-larg_base/2, altura, 0);
    
    // Trás
    glVertex3f(-larg_base/2, 0, -comp);
    glVertex3f( larg_base/2, 0, -comp);
    glVertex3f( larg_base/2, altura, -comp);
    glVertex3f(-larg_base/2, altura, -comp);
    
    // Topo da base (fechamento interno)
    glVertex3f(-larg_base/2, altura, 0);
    glVertex3f( larg_base/2, altura, 0);
    glVertex3f( larg_base/2, altura, -comp);
    glVertex3f(-larg_base/2, altura, -comp);
    
    // Laterais (Importante desenhar para fechar o objeto)
    // Direita
    glVertex3f( larg_base/2, 0, 0);
    glVertex3f( larg_base/2, 0, -comp);
    glVertex3f( larg_base/2, altura, -comp);
    glVertex3f( larg_base/2, altura, 0);
    // Esquerda
    glVertex3f(-larg_base/2, 0, 0);
    glVertex3f(-larg_base/2, 0, -comp);
    glVertex3f(-larg_base/2, altura, -comp);
    glVertex3f(-larg_base/2, altura, 0);
    glEnd();

    // 3. Assento (Topo - Cinza Concreto)
    float y_topo_fim = altura + 1.5;
    
    glBegin(GL_QUADS);
    glColor3f(0.7, 0.7, 0.7); 
    
    // Topo onde senta
    glVertex3f(-larg_topo/2, y_topo_fim, 0);
    glVertex3f( larg_topo/2, y_topo_fim, 0);
    glVertex3f( larg_topo/2, y_topo_fim, -comp);
    glVertex3f(-larg_topo/2, y_topo_fim, -comp);
    
    // Bordas laterais do assento
    // Esquerda
    glVertex3f(-larg_topo/2, altura, 0);
    glVertex3f(-larg_topo/2, altura, -comp);
    glVertex3f(-larg_topo/2, y_topo_fim, -comp);
    glVertex3f(-larg_topo/2, y_topo_fim, 0);
    // Direita
    glVertex3f( larg_topo/2, altura, 0);
    glVertex3f( larg_topo/2, altura, -comp);
    glVertex3f( larg_topo/2, y_topo_fim, -comp);
    glVertex3f( larg_topo/2, y_topo_fim, 0);
    // Frente e Trás do assento
    glVertex3f(-larg_topo/2, altura, 0);
    glVertex3f( larg_topo/2, altura, 0);
    glVertex3f( larg_topo/2, y_topo_fim, 0);
    glVertex3f(-larg_topo/2, y_topo_fim, 0);

    glVertex3f(-larg_topo/2, altura, -comp);
    glVertex3f( larg_topo/2, altura, -comp);
    glVertex3f( larg_topo/2, y_topo_fim, -comp);
    glVertex3f(-larg_topo/2, y_topo_fim, -comp);

    glEnd();
}

void desenharCorredor(float largura, float comprimento, float y_piso, float z_inicio, float base_solida) {
    float x_meia = largura / 2.0;
    float z_fim = z_inicio - comprimento;
    
    // 1. Chão e Fundação
    glBegin(GL_QUADS);
    glColor3f(0.6, 0.6, 0.6);
    glVertex3f(-x_meia, y_piso, z_inicio); glVertex3f( x_meia, y_piso, z_inicio);
    glVertex3f( x_meia, y_piso, z_fim); glVertex3f(-x_meia, y_piso, z_fim);

    glColor3f(0.35, 0.35, 0.35);
    glVertex3f(-x_meia, y_piso, z_inicio); glVertex3f(-x_meia, y_piso, z_fim);
    glVertex3f(-x_meia, base_solida, z_fim); glVertex3f(-x_meia, base_solida, z_inicio);
    glVertex3f( x_meia, y_piso, z_inicio); glVertex3f( x_meia, y_piso, z_fim);
    glVertex3f( x_meia, base_solida, z_fim); glVertex3f( x_meia, base_solida, z_inicio);
    glEnd();

    // 2. Murinhos
    float larg_base = 1.5; 
    float gap_borda = 1.0;
    float larg_topo = larg_base + gap_borda + gap_borda; 
    float alt_muro = 5; 
    
    float pos_x_esq = -x_meia + gap_borda + (larg_base / 2.0);
    float pos_x_dir =  x_meia - gap_borda - (larg_base / 2.0);

    glPushMatrix(); glTranslatef(pos_x_esq, y_piso, z_inicio);
    desenhaMurinho(comprimento, larg_base, larg_topo, alt_muro); glPopMatrix();

    glPushMatrix(); glTranslatef(pos_x_dir, y_piso, z_inicio);
    desenhaMurinho(comprimento, larg_base, larg_topo, alt_muro); glPopMatrix();

    // 3. PILARES
    float raio_cilindro = 4.0;   
    float alt_acima_solo = 36.0; // Altura do pilar
    
    float prof_abaixo = 15.0;    
    float altura_total = alt_acima_solo + prof_abaixo;
    
    glColor3f(0.5f, 0.5f, 0.5f); 

    glPushMatrix(); glTranslatef(pos_x_esq, y_piso - prof_abaixo, z_inicio); 
    desenhaCilindro(altura_total, raio_cilindro); glPopMatrix();
    
    glPushMatrix(); glTranslatef(pos_x_esq, y_piso - prof_abaixo, z_fim); 
    desenhaCilindro(altura_total, raio_cilindro); glPopMatrix();

    glPushMatrix(); glTranslatef(pos_x_dir, y_piso - prof_abaixo, z_inicio); 
    desenhaCilindro(altura_total, raio_cilindro); glPopMatrix();

    glPushMatrix(); glTranslatef(pos_x_dir, y_piso - prof_abaixo, z_fim); 
    desenhaCilindro(altura_total, raio_cilindro); glPopMatrix();

    // --- 4. TELHADO BRANCO 3D (NOVO) ---
    // Medidas do telhado (Com beiral/sobra de 4.0 para cada lado)
    float beiral = 6.0;
    float largura_telhado = largura + beiral;     
    float comp_telhado = comprimento + beiral;
    float espessura_telhado = 2.0;

    // Posição Y: Altura do chão + Altura do pilar + Metade da espessura (pois o cubo cresce do centro)
    float y_telhado = y_piso + alt_acima_solo + (espessura_telhado / 2.0);
    
    // Posição Z: Centro do corredor
    float z_centro = z_inicio - (comprimento / 2.0);

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0); // Branco Puro
    
    // Move para o centro, topo dos pilares
    glTranslatef(0.0, y_telhado, z_centro);
    
    // Escala o cubo para virar uma laje retangular
    glScalef(largura_telhado, espessura_telhado, comp_telhado);
    
    glutSolidCube(1.0); // Desenha o cubo base
    glPopMatrix();
}

void desenhaEscada(float px, float py, float pz) {
    glPushMatrix(); 
    glTranslatef(px, py, pz); 

    // --- MUDANÇA: LARGURA AGORA É 45.0 (30 + 15) ---
    float largura = 45.0; 
    
    float altura_degrau = 4.0 / 3.0; 
    float prof_degrau = 6.0 / 3.0;   
    float comp_corredor = 35.0; 
    float base_solida = -70.0 / 3.0; 
    
    float x_meia_largura = largura / 2.0;
    float y_atual = 0.0;
    float z_atual = 0.0;

    // --- PARTE 1: LANCE SUPERIOR ---
    glBegin(GL_QUADS);
    glColor3f(0.35, 0.35, 0.35);
    glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
    glVertex3f( x_meia_largura, base_solida, z_atual); glVertex3f(-x_meia_largura, base_solida, z_atual);

    for (int i = 0; i < 4; i++) {
        glColor3f(0.4, 0.4, 0.4); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual - altura_degrau, z_atual); glVertex3f(-x_meia_largura, y_atual - altura_degrau, z_atual);
        y_atual -= altura_degrau; 
        
        glColor3f(0.6, 0.6, 0.6); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        
        glColor3f(0.35, 0.35, 0.35); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f(-x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f(-x_meia_largura, base_solida, z_atual);
        glVertex3f( x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f( x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f( x_meia_largura, base_solida, z_atual);
        z_atual -= prof_degrau; 
    }
    glEnd(); 

    // --- PARTE 2: CORREDOR ---
    // Aqui passamos a largura 45.0 para a sua função desenharCorredor
    desenharCorredor(largura, comp_corredor, y_atual, z_atual, base_solida);
    z_atual -= comp_corredor;

    // --- PARTE 3: LANCE INFERIOR ---
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor3f(0.4, 0.4, 0.4); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual - altura_degrau, z_atual); glVertex3f(-x_meia_largura, y_atual - altura_degrau, z_atual);
        y_atual -= altura_degrau; 
        
        glColor3f(0.6, 0.6, 0.6); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        
        glColor3f(0.35, 0.35, 0.35); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f(-x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f(-x_meia_largura, base_solida, z_atual);
        glVertex3f( x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f( x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f( x_meia_largura, base_solida, z_atual);
        z_atual -= prof_degrau; 
    }
    
    glColor3f(0.35, 0.35, 0.35);
    glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
    glVertex3f( x_meia_largura, base_solida, z_atual); glVertex3f(-x_meia_largura, base_solida, z_atual);
    glEnd();
    
    glPopMatrix(); 
}

// --- FUNÇÃO DE COLISÃO / ALTURA DO CHÃO ---
float calcularAlturaDoChao(float x, float z) {
    float global_scale = 3.0;        
    float global_trans_y = 30.0;     
    
    float offset_scene_x = -20.0;
    float offset_scene_y = -10.0;
    float offset_scene_z = -120.0;

    float offset_escada_y = 5.0; 

    float escadaWorldX = (offset_scene_x + p[0]) * global_scale;
    float escadaWorldY = global_trans_y + (offset_scene_y + p[1] + offset_escada_y) * global_scale;
    float escadaWorldZ = (offset_scene_z + p[2]) * global_scale;

    float altura_olhos = 30.0;
    
    // --- MUDANÇA: LARGURA DA FÍSICA 45.0 ---
    float largura_real = 45.0 * global_scale; 
    
    float meia_largura = largura_real / 2.0;
    
    float comp_degraus = 8.0 * global_scale; 
    float comp_corredor = 35.0 * global_scale; 
    float queda_altura = 5.3333f * global_scale; 

    float z_inicio = escadaWorldZ;
    float z_fim_rampa1 = z_inicio - comp_degraus;
    float z_fim_corredor = z_fim_rampa1 - comp_corredor;
    float z_fim_total = z_fim_corredor - comp_degraus;

    // --- LÓGICA DE ALTURA ---
    if (x < (escadaWorldX - meia_largura) || x > (escadaWorldX + meia_largura)) {
         return escadaWorldY + altura_olhos;
    }

    if (z > z_inicio) {
        return escadaWorldY + altura_olhos;
    }
    else if (z > z_fim_rampa1) {
        float dist = z_inicio - z;
        float pct = dist / comp_degraus;
        return (escadaWorldY - (pct * queda_altura)) + altura_olhos;
    }
    else if (z > z_fim_corredor) {
        return (escadaWorldY - queda_altura) + altura_olhos;
    }
    else if (z > z_fim_total) {
        float dist = z_fim_corredor - z;
        float pct = dist / comp_degraus;
        return ((escadaWorldY - queda_altura) - (pct * queda_altura)) + altura_olhos;
    }
    else {
        return ((escadaWorldY - (queda_altura * 2))) + altura_olhos;
    }
}

void DISPLAY(void)
{
    timeOfDay--;
    timeOfDay = timeOfDay%360;
    skyColor--;
    if(timeOfDay%360<=-0.5)
      skyColor=0;
    else if(skyColor<0)
      skyColor=255;
    
    glMatrixMode(GL_PROJECTION);/*glMatrixMode()- define qual matriz ser� alterada. SEMPRE defina o tipo de apresenta��o
                              (Ortogonal ou Perspectiva) na matriz PROJECTION.*/
    glLoadIdentity();//"Limpa" ou "transforma" a matriz em identidade, reduzindo poss�veis erros.
    if (projecao==1)
    {
        glOrtho(-200, 200, -200, 200, -200, 200);  //Define a proje��o como ortogonal
        if (rotacao=1)
            glRotatef(angulox,1.0,0.0,0.0);
        if (rotacao=2)
            glRotatef(anguloy,0.0,1.0,0.0);
    }
    if (projecao == 0)
    {
        gluPerspective(90,1,1,2000); //Define a proje��o como perspectiva
        if (rotacao=1)
            glRotatef(angulox,1.0,0.0,0.0);
        if (rotacao=2)
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

void desenhaChaoPersonalizado(float px, float py, float pz) {
    glPushMatrix(); 
    glTranslatef(px, py, pz); 

    // --- MUDANÇA: LARGURA DO BURACO AGORA É 45.0 ---
    float largura_escada = 45.0; 
    
    float x_gap_esq = -largura_escada / 2.0; 
    float x_gap_dir =  largura_escada / 2.0; 

    float x_mundo_min = -300.0;
    float x_mundo_max =  300.0;

    float z_antes = 200.0;     
    float z_inicio = 0.0;      
    float z_plat1 = -8.0;          
    float z_plat2 = -43.0;   
    float z_fim = -51.0;     
    float z_horizonte = -600.0;

    float y_topo = 0.0;
    float y_meio = -5.333;         
    float y_fundo = -10.666;       

    glColor3ub(100, 200, 0); 
    glNormal3f(0, 1, 0);

    glBegin(GL_QUADS);

    // 1. BLOCO TRASEIRO
    glVertex3f(x_mundo_min, y_topo, z_antes); glVertex3f(x_mundo_max, y_topo, z_antes);
    glVertex3f(x_mundo_max, y_topo, z_inicio); glVertex3f(x_mundo_min, y_topo, z_inicio);

    // 2. BLOCO FRONTAL
    glVertex3f(x_mundo_min, y_fundo, z_fim); glVertex3f(x_mundo_max, y_fundo, z_fim);
    glVertex3f(x_mundo_max, y_fundo, z_horizonte); glVertex3f(x_mundo_min, y_fundo, z_horizonte);

    // 3. FAIXA LATERAL ESQUERDA
    glVertex3f(x_mundo_min, y_topo, z_inicio); glVertex3f(x_gap_esq, y_topo, z_inicio);
    glVertex3f(x_gap_esq, y_meio, z_plat1); glVertex3f(x_mundo_min, y_meio, z_plat1);

    glVertex3f(x_mundo_min, y_meio, z_plat1); glVertex3f(x_gap_esq, y_meio, z_plat1);
    glVertex3f(x_gap_esq, y_meio, z_plat2); glVertex3f(x_mundo_min, y_meio, z_plat2);

    glVertex3f(x_mundo_min, y_meio, z_plat2); glVertex3f(x_gap_esq, y_meio, z_plat2);
    glVertex3f(x_gap_esq, y_fundo, z_fim); glVertex3f(x_mundo_min, y_fundo, z_fim);

    // 4. FAIXA LATERAL DIREITA
    glVertex3f(x_gap_dir, y_topo, z_inicio); glVertex3f(x_mundo_max, y_topo, z_inicio);
    glVertex3f(x_mundo_max, y_meio, z_plat1); glVertex3f(x_gap_dir, y_meio, z_plat1);

    glVertex3f(x_gap_dir, y_meio, z_plat1); glVertex3f(x_mundo_max, y_meio, z_plat1);
    glVertex3f(x_mundo_max, y_meio, z_plat2); glVertex3f(x_gap_dir, y_meio, z_plat2);

    glVertex3f(x_gap_dir, y_meio, z_plat2); glVertex3f(x_mundo_max, y_meio, z_plat2);
    glVertex3f(x_mundo_max, y_fundo, z_fim); glVertex3f(x_gap_dir, y_fundo, z_fim);

    glEnd();
    glPopMatrix(); 
}

void DesenharCena ()
{
    // --- 1. BLOCOS DE PRÉDIO E ELEMENTOS DO MUNDO ---
    // Esses objetos usam a escala global e posição padrão
    glPushMatrix();
    glTranslatef(0, translado, 0);
    glScalef(escala, escala, escala);

    OBJModel *privada = load_obj("../assets/toilet/toilet.obj");
    glPushMatrix();
    glColor3ub(180, 180, 180);
    glTranslatef(-450, 15, -190);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15, 15, 15);
    draw_obj_model(privada);
    glPopMatrix();

    
    glPushMatrix();
    glColor3ub(180, 180, 180);
    glTranslatef(-450, 15, -160);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15, 15, 15);
    draw_obj_model(privada);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(180, 180, 180);
    glTranslatef(-450, 15, -130);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15, 15, 15);
    draw_obj_model(privada);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(180, 180, 180);
    glTranslatef(-450, 15, 15);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15, 15, 15);
    draw_obj_model(privada);
    glPopMatrix();

    
    glPushMatrix();
    glColor3ub(180, 180, 180);
    glTranslatef(-450, 15, 45);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15, 15, 15);
    draw_obj_model(privada);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(180, 180, 180);
    glTranslatef(-450, 15,75);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15, 15, 15);
    draw_obj_model(privada);
    glPopMatrix();
    
    
    // Prédio Principal
    // making_class_block(0, 0, -70);
    
    // Porta Giratória
    glPushMatrix();
    glTranslatef(-3, 0, 10);
    glRotatef(rotating, 0, 1, 0);
    glTranslatef(3, 0, -10);
    //making_door(); // Descomente se sua função existir
    glPopMatrix();


    // --- 2. O CHÃO E A ESCADA ---
    // Atenção: Desenhamos eles aqui dentro para herdar a escala do mundo,
    // mas aplicamos um ajuste de posição para encaixar na frente do prédio.
    
    glPushMatrix();
    // AJUSTE FINO DE POSIÇÃO:
    // X = -20 (Centraliza na frente da porta esquerda)
    // Y = -25 (Abaixa o chão para ficar abaixo da câmera)
    // Z = -50 (Puxa para perto ou longe do prédio)
    glTranslatef(-20, -10, -50); 
    making_class_block(0, 0, -70, 1);
    making_auditorium(-260, 0, -8);
    glPopMatrix();

    glPushMatrix();
    // Desenha o chão verde (com o buraco no meio)
    glTranslatef(-20, -10, -120);

    desenhaChaoPersonalizado(p[0], p[1], p[2]);
    desenhaEscada(p[0], p[1], p[2]);
    
    glPopMatrix(); 
    // ----------------------------

    glPopMatrix(); // Fim do Scale Global

    // Sol e Céu (Desenhados fora da escala para ficarem no infinito)
    glPushMatrix();
    glRotatef(timeOfDay, 0, 0, 1);
    making_sun();
    glPopMatrix();

    making_sky();
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
            posy=30;
            posz=200;
            oy=30;
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
    // Guarda Y antigo para ajustar a câmera
    float y_antigo = posy;

    switch(tecla)
    {
    case 's':
        posx=posx+(5*cos((anguloy+90)/57.32));
        posz=posz+(5*sin((anguloy+90)/57.32));
        ox=ox+(5*cos((anguloy+90)/57.32));
        oz=oz+(5*sin((anguloy+90)/57.32));
        break;

    case 'w':
        posx=posx-(5*cos((anguloy+90)/57.32));
        posz=posz-(5*sin((anguloy+90)/57.32));
        ox=ox-(5*cos((anguloy+90)/57.32));
        oz=oz-(5*sin((anguloy+90)/57.32));
        break;

    case 'd':
        posx=posx+(5*sin((anguloy+90)/57.32));
        posz=posz-(5*cos((anguloy+90)/57.32));
        ox=ox+(5*sin((anguloy+90)/57.32));
        oz=oz-(5*cos((anguloy+90)/57.32));
        break;

    case 'a':
        posx=posx-(5*sin((anguloy+90)/57.32));
        posz=posz+(5*cos((anguloy+90)/57.32));
        ox=ox-(5*sin((anguloy+90)/57.32));
        oz=oz+(5*cos((anguloy+90)/57.32));
        break;
    case 'e':
      rotating++;
      if(rotating>=0) rotating=0;
      break;
    case 'r':
      rotating--;
      if(rotating<=-90) rotating=-90;
      break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }

    // --- ATUALIZA A ALTURA AUTOMATICAMENTE ---
    posy = calcularAlturaDoChao(posx, posz);
    
    // Ajusta o alvo da câmera para não "olhar para cima" ao descer
    float diff = posy - y_antigo;
    oy += diff;

    glutPostRedisplay();
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
    glutCreateWindow("Aula 07");
    Inicializa();
    glutDisplayFunc(DISPLAY);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(MOUSE_Button);
    glutPassiveMotionFunc(MOUSE);
    glutSpecialFunc(TeclasEspeciais);
    glutMainLoop();
    return 0;
}
