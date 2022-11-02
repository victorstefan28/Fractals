#include <iostream>
#include "GLFW/glfw3.h"
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <dos.h>
#include <cmath>
#define NMAXMANDEL 350
#define NMAXSIERPINSKI 500

struct punct{
    float x, y;
    punct(float a, float b) {x = a; y = b;}
    float lungime2d(punct a)
    {
        return (x - a.x) * (x - a.x) + (y - a.y) * (y - a.y);
    }
    punct imp3d(punct a, int div)
    {
        return punct((x+a.x)/div, (y+a.y)/div);
    }
    float modul()
    {
        return x*x + y*y;
    }
    punct operator*(const float inm) {
        this->x *= inm;
        this->y *= inm;
        return *this;
    }
    punct operator-(const punct p2)
    {
        punct p1 = *this;
        p1.x-=p2.x;
        p1.y-=p2.y;
        return p1;
    }
    punct operator+(const punct p2)
    {
        punct p1 = *this;
        p1.x+=p2.x;
        p1.y+=p2.y;
        return p1;
    }
};
punct complex_inmultire(punct z1, punct z2)
{
        return punct(z1.x * z2.x - z1.y * z2.y, z1.y * z2.x + z1.x * z2.y);
}
punct complex_dimpartire(punct z1, punct z2)
{
        float numitor = z2.x * z2.x + z2.y * z2.y;
        return punct((z1.x * z2.x + z1.y * z2.y) / numitor, (z1.y * z2.x - z1.x + z2.y) / numitor);
}

void triunghirec(punct p1, punct p2, punct p3, int n)
{

    punct u1((p1.x + p3.x)/2, (p1.y + p3.y)/2),
          u2((p1.x + p2.x)/2,(p1.y + p2.y)/2),
          u3((p2.x + p3.x)/2, (p2.y + p3.y)/2);
    if(n > 10)
    {
        triunghirec(u1, u2, p1, n/2);
        triunghirec(u1, u3, p3, n/2);
        triunghirec(u2, u3, p2, n/2);
        glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 1.f, 0.f);
        glVertex2f(u1.x, u1.y);
        glColor3f(0.0f, 1.f, 0.f);
        glVertex2f(u2.x, u2.y);
        glColor3f(1.f, 1.f, 0.f);
        glVertex2f(u3.x, u3.y);
        glEnd();
    }
}

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    const punct minim(-1.0, -1.0);
    const punct maxim(1.0, 1.0);
    int opt = -1;

    std::cout<<"1. Mandelbrot's set\n";
    std::cout<<"2. Sierpinski's triangle\n";
    std::cin>>opt;
    window = glfwCreateWindow(720, 480, "Fractal", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    int ncur = 0, nmax = NMAXMANDEL;
    float mod = 1.0f;
    if(opt == 2)
        nmax = NMAXSIERPINSKI, ncur = 5, mod = 0.3f;
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
            glClear(GL_COLOR_BUFFER_BIT);
            if(opt == 1)
            {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0, 720, 0, 480, -1, 1);
                for(int x = 0; x<=720; x++)
                    for(int y = 0;y <= 480; y++)
                    {
                        const
                        const punct c( (-2.0 + ( x * ( 1.0 - (-2.0))) / 720), (1.125 + (y * (-1.125 - 1.125) / 480) ));
                        punct z(0.0, 0.0);
                        int n = 0;
                        glBegin(GL_POINTS);
                        while(z.modul() < 4.0 && n < ncur)
                        {
                            z = complex_inmultire(z, z) + c; /// f(z) + c f:C->C f(z) = z^2 (2D)
                            n++;
                        }
                        float culoare = pow((n*1.0f)/ncur, 0.35f);
                        if(n == ncur)
                            glColor3f(1.0f, 0.0f, 0.05f);
                        else glColor3f(0.0f, culoare/2, culoare * 0.8f);
                        glVertex2i(x,y);
                        glEnd();
                    }
               if(ncur <= nmax)
               {
                   ncur += mod;
                   mod += 0.2f;
                   Sleep(15);
               }
               else ncur = 1, mod = 1.0f, Sleep(250);
            }
            else if(opt == 2)
            {

                triunghirec(punct(-0.8f, -0.8f), punct(0.f, 0.8f), punct(0.8f, -0.8f), ncur);
                if(ncur <= nmax)
                {
                    ncur += mod;
                    mod += 0.05;
                    Sleep(5);
                }
                else ncur = 1, mod = 1.0, Sleep(2500);
            }
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    glfwTerminate();
    return 0;
}
