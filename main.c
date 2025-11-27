#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>

#define translado 30
#define escala 3
int rotating=0, timeOfDay=0, skyColor=255;

void making_enviroment();
void making_sun();
void making_sky();

void DesenharCena();

void making_class_block(float x, float y, float z);
void making_door(float x, float y, float z);

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
    glColor3ub(228, 228, 228);
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
}

void making_class_block(float x, float y, float z){
    
    //Parede de tras
    back_block(x-73, y, z);

    //Parede da esquerda
    glPushMatrix();
    glTranslatef(x-100, y, z+50);
    glScalef(0.05, 3, 5);
    glColor3ub(209, 80, 6);
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
    glTranslatef(x-93, y+20, z+99);
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
    glTranslatef(x-43, y+20, z+99);
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
    glTranslatef(x+7, y+20, z+99);
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
    glTranslatef(x+57, y+20, z+99);
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
    glTranslatef(x+131, y+20, z+99);
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

void desenhaMeioCilindro(float altura, float raio) {
    float x, z, angulo;
    
    // Parte curva
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 20; i++) {
        angulo = -i * 3.14159f / 20.0f;
        x = raio * cos(angulo);
        z = raio * sin(angulo);
        glNormal3f(x/raio, 0.0f, z/raio);
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, altura, z);
    }
    glEnd();

    // Tampa superior
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, altura, 0.0f);
    for (int i = 0; i <= 20; i++) {
        angulo = -i * 3.14159f / 20.0f;
        x = raio * cos(angulo);
        z = raio * sin(angulo);
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
    glVertex3f(-x_meia, y_piso, z_inicio);
    glVertex3f( x_meia, y_piso, z_inicio);
    glVertex3f( x_meia, y_piso, z_fim);
    glVertex3f(-x_meia, y_piso, z_fim);

    glColor3f(0.35, 0.35, 0.35);
    glVertex3f(-x_meia, y_piso, z_inicio); glVertex3f(-x_meia, y_piso, z_fim);
    glVertex3f(-x_meia, base_solida, z_fim); glVertex3f(-x_meia, base_solida, z_inicio);

    glVertex3f( x_meia, y_piso, z_inicio); glVertex3f( x_meia, y_piso, z_fim);
    glVertex3f( x_meia, base_solida, z_fim); glVertex3f( x_meia, base_solida, z_inicio);
    glEnd();

    // 2. Murinhos
    float larg_base = 1.5; 
    float gap_borda = 1.0;
    
    // Topo = Base + gap esq + gap dir
    float larg_topo = larg_base + gap_borda + gap_borda; // 1.5 + 1 + 1 = 3.5
    float alt_muro = 8.0;
    
    // Cálculo da Posição X:
    // Borda do corredor + Gap + Metade da largura da base (pois o pivot é no centro)
    float pos_x_esq = -x_meia + gap_borda + (larg_base / 2.0);
    float pos_x_dir =  x_meia - gap_borda - (larg_base / 2.0);

    // Muro Esquerdo
    glPushMatrix();
    glTranslatef(pos_x_esq, y_piso, z_inicio);
    desenhaMurinho(comprimento, larg_base, larg_topo, alt_muro);
    glPopMatrix();

    // Muro Direito
    glPushMatrix();
    glTranslatef(pos_x_dir, y_piso, z_inicio);
    desenhaMurinho(comprimento, larg_base, larg_topo, alt_muro);
    glPopMatrix();

    // 3. Semi-Cilindros ("Orelhões")
    // Ajustei para ficarem centralizados com a linha do muro
    float raio_orelhao = 6.0;
    float alt_orelhao = 18.0;
    
    glColor3f(0.65, 0.3, 0.25); 

    // Canto 1 e 3 (Esquerda)
    glPushMatrix();
    glTranslatef(pos_x_esq, y_piso, z_inicio - 2.0);
    glRotatef(90, 0, 1, 0);
    desenhaMeioCilindro(alt_orelhao, raio_orelhao);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(pos_x_esq, y_piso, z_fim + 2.0);
    glRotatef(90, 0, 1, 0);
    desenhaMeioCilindro(alt_orelhao, raio_orelhao);
    glPopMatrix();

    // Canto 2 e 4 (Direita)
    glPushMatrix();
    glTranslatef(pos_x_dir, y_piso, z_inicio - 2.0);
    glRotatef(-90, 0, 1, 0);
    desenhaMeioCilindro(alt_orelhao, raio_orelhao);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(pos_x_dir, y_piso, z_fim + 2.0);
    glRotatef(-90, 0, 1, 0);
    desenhaMeioCilindro(alt_orelhao, raio_orelhao);
    glPopMatrix();
}

// --- FUNÇÃO PRINCIPAL ---
void desenhaEscada() {
    float largura = 30.0;
    float altura_degrau = 4.0;
    float prof_degrau = 6.0;
    float comp_corredor = 70.0;
    float base_solida = -70.0; 
    float x_meia_largura = largura / 2.0;
    
    float y_atual = 0.0;
    float z_atual = 0.0;

    // --- PARTE 1: ESCADA SUPERIOR ---
    glBegin(GL_QUADS);
    
    // Fechamento traseiro
    glColor3f(0.35, 0.35, 0.35);
    glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
    glVertex3f( x_meia_largura, base_solida, z_atual); glVertex3f(-x_meia_largura, base_solida, z_atual);

    for (int i = 0; i < 5; i++) {
        // Espelho
        glColor3f(0.4, 0.4, 0.4); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual - altura_degrau, z_atual); glVertex3f(-x_meia_largura, y_atual - altura_degrau, z_atual);
        y_atual -= altura_degrau; 
        
        // Piso
        glColor3f(0.6, 0.6, 0.6); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        
        // Laterais
        glColor3f(0.35, 0.35, 0.35); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f(-x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f(-x_meia_largura, base_solida, z_atual);
        
        glVertex3f( x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f( x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f( x_meia_largura, base_solida, z_atual);
        
        z_atual -= prof_degrau; 
    }
    glEnd(); 

    // --- PARTE 2: CHAMADA DO CORREDOR ---
    desenharCorredor(largura, comp_corredor, y_atual, z_atual, base_solida);
    
    // Atualiza o Z para o fim do corredor para continuar a escada
    z_atual -= comp_corredor;

    // --- PARTE 3: ESCADA INFERIOR ---
    glBegin(GL_QUADS);
    for (int i = 0; i < 5; i++) {
        // Espelho
        glColor3f(0.4, 0.4, 0.4); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual - altura_degrau, z_atual); glVertex3f(-x_meia_largura, y_atual - altura_degrau, z_atual);
        y_atual -= altura_degrau; 
        
        // Piso
        glColor3f(0.6, 0.6, 0.6); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
        glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        
        // Laterais
        glColor3f(0.35, 0.35, 0.35); 
        glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f(-x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f(-x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f(-x_meia_largura, base_solida, z_atual);
        
        glVertex3f( x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual - prof_degrau);
        glVertex3f( x_meia_largura, base_solida, z_atual - prof_degrau); glVertex3f( x_meia_largura, base_solida, z_atual);
        
        z_atual -= prof_degrau; 
    }
    
    // Fechamento Final
    glColor3f(0.35, 0.35, 0.35);
    glVertex3f(-x_meia_largura, y_atual, z_atual); glVertex3f( x_meia_largura, y_atual, z_atual);
    glVertex3f( x_meia_largura, base_solida, z_atual); glVertex3f(-x_meia_largura, base_solida, z_atual);
    glEnd();
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

void desenhaChaoPersonalizado() {
    // --- CONFIGURAÇÕES IGUAIS À ESCADA ---
    float largura_escada = 30.0; 
    float x_gap_esq = -largura_escada / 2.0; // -15.0
    float x_gap_dir =  largura_escada / 2.0; // +15.0

    // Limites do mundo (Grama)
    float x_mundo_min = -300.0;
    float x_mundo_max =  300.0;

    // Coordenadas Z (Profundidade) - Sincronizadas com a escada
    float z_antes = 200.0;     // O horizonte atrás da câmera
    float z_inicio = 0.0;      // Onde começa a descer
    float z_plat1 = -30.0;     // Fim da primeira rampa
    float z_plat2 = -100.0;    // Fim do corredor plano
    float z_fim = -130.0;      // Fim da segunda rampa
    float z_horizonte = -600.0;// O horizonte lá na frente

    // Alturas Y
    float y_topo = 0.0;
    float y_meio = -20.0;
    float y_fundo = -40.0;

    glColor3ub(100, 200, 0); // Verde Grama
    glNormal3f(0, 1, 0);

    glBegin(GL_QUADS);

    // 1. BLOCO TRASEIRO (O chão plano antes da escada, onde ficam os prédios)
    // Vai de X min a X max, e de Z=200 até Z=0
    glVertex3f(x_mundo_min, y_topo, z_antes);
    glVertex3f(x_mundo_max, y_topo, z_antes);
    glVertex3f(x_mundo_max, y_topo, z_inicio);
    glVertex3f(x_mundo_min, y_topo, z_inicio);

    // 2. BLOCO FRONTAL (O chão plano lá embaixo no final da escada)
    // Vai de X min a X max, e de Z=-130 até Z=-600
    glVertex3f(x_mundo_min, y_fundo, z_fim);
    glVertex3f(x_mundo_max, y_fundo, z_fim);
    glVertex3f(x_mundo_max, y_fundo, z_horizonte);
    glVertex3f(x_mundo_min, y_fundo, z_horizonte);

    // 3. FAIXA LATERAL ESQUERDA (Acompanha o perfil da escada)
    // Vai do mundo_min até a beirada da escada (-15)
    
    // 3.1 Rampa 1
    glVertex3f(x_mundo_min, y_topo, z_inicio);
    glVertex3f(x_gap_esq,   y_topo, z_inicio);
    glVertex3f(x_gap_esq,   y_meio, z_plat1);
    glVertex3f(x_mundo_min, y_meio, z_plat1);

    // 3.2 Corredor Plano
    glVertex3f(x_mundo_min, y_meio, z_plat1);
    glVertex3f(x_gap_esq,   y_meio, z_plat1);
    glVertex3f(x_gap_esq,   y_meio, z_plat2);
    glVertex3f(x_mundo_min, y_meio, z_plat2);

    // 3.3 Rampa 2
    glVertex3f(x_mundo_min, y_meio, z_plat2);
    glVertex3f(x_gap_esq,   y_meio, z_plat2);
    glVertex3f(x_gap_esq,   y_fundo, z_fim);
    glVertex3f(x_mundo_min, y_fundo, z_fim);


    // 4. FAIXA LATERAL DIREITA (Mesma coisa, do outro lado)
    // Vai da beirada da escada (+15) até mundo_max
    
    // 4.1 Rampa 1
    glVertex3f(x_gap_dir,   y_topo, z_inicio);
    glVertex3f(x_mundo_max, y_topo, z_inicio);
    glVertex3f(x_mundo_max, y_meio, z_plat1);
    glVertex3f(x_gap_dir,   y_meio, z_plat1);

    // 4.2 Corredor Plano
    glVertex3f(x_gap_dir,   y_meio, z_plat1);
    glVertex3f(x_mundo_max, y_meio, z_plat1);
    glVertex3f(x_mundo_max, y_meio, z_plat2);
    glVertex3f(x_gap_dir,   y_meio, z_plat2);

    // 4.3 Rampa 2
    glVertex3f(x_gap_dir,   y_meio, z_plat2);
    glVertex3f(x_mundo_max, y_meio, z_plat2);
    glVertex3f(x_mundo_max, y_fundo, z_fim);
    glVertex3f(x_gap_dir,   y_fundo, z_fim);

    glEnd();
}

void DesenharCena ()
{
    // --- 1. BLOCOS DE PRÉDIO E ELEMENTOS DO MUNDO ---
    // Esses objetos usam a escala global e posição padrão
    glPushMatrix();
    glTranslatef(0, translado, 0);
    glScalef(escala, escala, escala);

    // Texto
    glColor3ub(0,0,0);
    glPushMatrix();
    glTranslatef(-35, 41, 40.2);
    stroke_output(0, 0, "CASA ABRIDANTE EM openGL!!");
    glPopMatrix();
    
    // Prédio Principal
    making_class_block(0, 0, -70);
    
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

    // Desenha o chão verde (com o buraco no meio)
    desenhaChaoPersonalizado();
    
    // Desenha a escada cinza (encaixa no buraco)
    glTranslatef(0, 5, 0); 
    desenhaEscada();
    
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
