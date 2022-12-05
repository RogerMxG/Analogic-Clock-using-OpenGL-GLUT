/*
Graficos por Computadora

Nombre: Analogic Clock
Nombre del archivo: Analogic Clock.c
Fecha:  04-11-2022
Programador: Rogelio Manríquez Cobián

Objetivo: Mostrar las transformaciones geometricas de 
          traslacion, rotacion y escalamiento sobre 
          las figuras geometricas basicas utilizano
          coordenadas

Compilar:
gcc -c aclock.c
Enlazar:
gcc aclock.o -o run  -l glut32 -l opengl32
*/

#include<gl\glut.h>
#include<math.h>
#include<time.h>
#include<stdio.h>
#include <windows.h>

typedef enum{NEGRO, AZUL, VERDE, CYAN, ROJO, MAGENTA, AMARILLO, BLANCO, GRIS,
             GRIS_OSCURO, GRIS_CLARO}COLOR;

float PALETA_COLORES[11][3]={ 0, 0, 0, //NEGRO
                             0, 0, 1, //AZUL
                             0, 1, 0, //VERDE
                             0, 1, 1, //CYAN
                             1, 0, 0, //ROJO
                             1, 0, 1, //MAGENTA
                             1, 1, 0, //AMARILLO
                             1, 1, 1, //BLANCO
                             0.5,0.5,0.5, //GRIS
                             0.25,0.25,0.25, //GRIS_OSC
                             0.8,0.8,0.8 }; //GRIS_CLARO
typedef struct{ float x;
                float y;
              }PUNTO;

typedef struct{ PUNTO ini;
                PUNTO fin;
                COLOR color;
              }LINEA; 

typedef struct{ PUNTO c;
                float r;
              }CIRCULO;

void DibujarTexto(char *Texto, float x, float y);
void AsignaColor(COLOR c);
void display(void);
void Linea(LINEA L);
void Circulo(CIRCULO ci);
void RotacionLinea(LINEA *L,float xr, float yr, float Theta);
void animar(void);

const float pi = 3.14159;
unsigned int minFlag = 0, hourFlag = 0;


LINEA segundero, minutero, horas;

int main(int argc, char **argv){  
    segundero.ini.x = 0;
    segundero.ini.y = 0;
    segundero.fin.x = 0;
    segundero.fin.y = 6.5;
    segundero.color = BLANCO;

    minutero.ini.x = 0;
    minutero.ini.y = 0;
    minutero.fin.x = 0;
    minutero.fin.y = 5;
    minutero.color = GRIS;

    horas.ini.x = 0;
    horas.ini.y = 0;
    horas.fin.x = 0;
    horas.fin.y = 4;
    horas.color = BLANCO;

    glutInit(&argc,argv);
    glutInitWindowSize(600,600);
    glutCreateWindow("Animacion");
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,10.0,-10.0,10.0,1.0,-1.0); 
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(display);

    /*Animación asíncrona
        Para realizar animación GLUT proporciona la función:
        
        glutIdleFunc();

        La cual asigna una función que debe cumplir ocn el siguiente prototipo

        void funcionALlamar(void);
    
        La función que se envía como parámetro en la función glIdleFunc() 
        es llamada cada N tiempo. Idealmente cada que se termina de dibujar

        Entonces básicamente para realizar una animación lo que debemos hacer
        es dibujar, cambiar los pparámetros de dibujo y volver a dibujar,
        repetir lo mismo cada N tiempo.


    */

   glutIdleFunc(animar);

    glutMainLoop();
    return 0;
}//FIN DE MAIN
  
void display(void){
    int k;
    float ang=30, theta, x, y;
    char Hora[5];
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    AsignaColor(BLANCO);
    CIRCULO C1;
    C1.c.x = 0;
    C1.c.y = 0;
    C1.r = 10;
    Circulo(C1);

    LINEA L1;
    L1.color = BLANCO;
    L1.ini.x = 8;
    L1.ini.y = 0;
    L1.fin.x = 10;
    L1.fin.y = 0;

    //DIBUJA LOS SEGMENTOS DE LAS HORAS DE LA CARÁTULA
    for (k=0; k<12; k++){
        RotacionLinea(&L1, 0, 0, ang*(pi/180.0));
        Linea(L1);
    }
    //DIBUJA LOS NÚMEROS CORRESPONDIENTES A LAS HORAS
    k=1;
    // for(theta=0; theta<6.28; theta+=0.523598){
    for(theta=1.04719; theta>-5; theta-=0.523598){
        x = 0 + 7*cos(theta);
        y = 0 + 7*sin(theta);
        sprintf(Hora, "%i", k);
        DibujarTexto(Hora, x, y);
        k++;
    }

    //DIBUJA EL SEGUNDERO, MINUTERO Y HORA
    Linea(segundero);
    Linea(minutero);
    Linea(horas);
    // glRectf(-10.0,10.0,-8.0,8.0);

    glFlush();  
}

void animar(void){
    RotacionLinea(&segundero, 0, 0, -0.104719);
    if(minFlag==60){
        RotacionLinea(&minutero, 0, 0, -0.104719);    
        minFlag=0;
    }
    if(hourFlag==3600){
        RotacionLinea(&horas, 0, 0, -(2*pi)/12);    
        hourFlag=0;
    }   
    sleep(60);
    minFlag++;
    hourFlag++;
    glutPostRedisplay();
}

void RotacionLinea(LINEA *L,float xr, float yr, float Theta)
{ float xp,yp;
  xp=xr + (L->ini.x-xr)*cos(Theta) - (L->ini.y-yr)*sin(Theta);
  yp=yr + (L->ini.x-xr)*sin(Theta) + (L->ini.y-yr)*cos(Theta);
  L->ini.x=xp;
  L->ini.y=yp;
  xp=xr + (L->fin.x-xr)*cos(Theta) - (L->fin.y-yr)*sin(Theta);
  yp=yr + (L->fin.x-xr)*sin(Theta) + (L->fin.y-yr)*cos(Theta);
  L->fin.x=xp;
  L->fin.y=yp;
}

void DibujarTexto(char *Texto, float x, float y)
{ int k=0;
  glRasterPos2f(x,y);
  while(Texto[k]!='\0')
  { glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,Texto[k]);
    k++;
    }
}

void AsignaColor(COLOR c)
{ glColor3f(PALETA_COLORES[c][0],PALETA_COLORES[c][1],PALETA_COLORES[c][2]);
 }

void Circulo(CIRCULO ci)
{ float x,y,theta;

  glBegin(GL_LINE_LOOP);
  for(theta=0; theta<6.28; theta+=0.1)
     { x=ci.c.x+ci.r*cos(theta);
       y=ci.c.y+ci.r*sin(theta);
       glVertex2f(x,y);
      }
  glEnd(); 
}

void Linea(LINEA L)
{ AsignaColor(L.color);
  glBegin(GL_LINES);
  glVertex2f(L.ini.x,L.ini.y);
  glVertex2f(L.fin.x,L.fin.y);
  glEnd();
}