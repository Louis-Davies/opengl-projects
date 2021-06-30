//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//  Assignment 1
//  Louis Davies
//  lda62
//  94941666
//
//  Use left/right arrow keys to change angle of motion/view
//  Use up/down arrow keys to move forward/back
//  Use a/d keys to "strafe" left/right
//  Use w/s keys to move up/down
//  Use h key to start helicopter animation or force it to land then stop if
//  its already going
//  Use b key to "bounce" ball effectively adding upwards velocity to it
//  Use o/l keys to tighten/loosen ball vice that it bounces between
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include "loadTGA.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
using namespace std;

//--Globals ---------------------------------------------------------------
// camera globals
// angle of rotation for the camera direction
float angle=0.0;
// vector camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float cam_x=0.0f,cam_z=400.0f;
float cam_hgt = 40.0f;

// heli globals
float blade_angle = 0.0f;
float heli_hgt = 2.0f;
bool heli_up = true;
bool heli_on = false;
bool heli_move = false;

// ball globals
float ball_velocity = 0;
float ball_velocity_new = 0;
float ball_height = 5;
float grav = -98;
bool ball_move = false;
float vice_length = 5;

// other globals
GLuint txId[4];
float white[3] = {1, 1, 1};

void loadTexture()
{
    glGenTextures(4, txId); 	// Create 4 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("Wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("concrete_floor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("dayskybox.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("roof.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

void drawFloor()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
	glBegin(GL_QUADS);
	    glNormal3f(0, 1, 0);
	    glTexCoord2f(0, 0);     glVertex3f(-400, 0, -400);
	    glTexCoord2f(0, 20);    glVertex3f(-400, 0, 400);
	    glTexCoord2f(20, 20);   glVertex3f(400, 0, 400);
	    glTexCoord2f(20, 0);    glVertex3f(400, 0, -400);
	glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawBuild()
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    glBegin(GL_QUADS);
        glColor3f(0.509, 0.352, 0.090);
        // BACK
        glTexCoord2f(0, 0);     glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0, 6);     glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(15, 6);    glVertex3f( 0.5f, 0.5f, -0.5f);
        glTexCoord2f(15, 0);    glVertex3f( 0.5f, -0.5f, -0.5f);
        // LEFT
        glTexCoord2f(15, 0);    glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(15, 6);    glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(0, 6);     glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(0, 0);     glVertex3f(-0.5f, -0.5f, -0.5f);
        // RIGHT
        glTexCoord2f(15, 0);    glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(15, 6);    glVertex3f( 0.5f, 0.5f, -0.5f);
        glTexCoord2f(0, 6);     glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(0, 0);     glVertex3f( 0.5f, -0.5f, 0.5f);
        // FRONT
        glTexCoord2f(0, 0);     glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(0, 6);     glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(6, 6);     glVertex3f( -0.1f, 0.5f, 0.5f);
        glTexCoord2f(6, 0);     glVertex3f( -0.1f, -0.5f, 0.5f);

        glTexCoord2f(0, 0);     glVertex3f(0.1f, -0.5f, 0.5f);
        glTexCoord2f(0, 6);     glVertex3f(0.1f, 0.5f, 0.5f);
        glTexCoord2f(6, 6);     glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(6, 0);     glVertex3f( 0.5f, -0.5f, 0.5f);

        glTexCoord2f(0, 0);     glVertex3f(-0.1f, -0.0f, 0.5f);
        glTexCoord2f(0, 3);     glVertex3f(-0.1f, 0.5f, 0.5f);
        glTexCoord2f(3, 3);     glVertex3f( 0.1f, 0.5f, 0.5f);
        glTexCoord2f(3, 0);     glVertex3f( 0.1f, -0.0f, 0.5f);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
        // TOP
        glTexCoord2f(0, 3);     glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(3, 3);     glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(3, 0);     glVertex3f( 0.5f, 0.5f, -0.5f);
        glTexCoord2f(0, 0);     glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawMuseum()
{
    glPushMatrix();
        glTranslatef(0, 75, 0);
        glScalef(375, 150, 375);
        drawBuild();
    glPopMatrix();
}

void drawHeliFeet()
{
    glColor3f(0.321, 0.321, 0.321);
    glPushMatrix();
        glTranslatef(10, 1, 0);
        glScalef(5, 2, 50);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-10, 1, 0);
        glScalef(5, 2, 50);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(10, 3, 5);
        glRotatef(30, 0, 0, 1);
        glScalef(2, 6, 2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(10, 3, -5);
        glRotatef(30, 0, 0, 1);
        glScalef(2, 6, 2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-10, 3, 5);
        glRotatef(-30, 0, 0, 1);
        glScalef(2, 6, 2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-10, 3, -5);
        glRotatef(-30, 0, 0, 1);
        glScalef(2, 6, 2);
        glutSolidCube(1);
    glPopMatrix();
}

void drawHeliFront()
{
    glColor3f(0.941, 0, 0);
    float vx = -10.;
    float wx = 10;
    // Generate parabola points for quad strips and triangle fans
    float vy[22] = {-10., -9., -8., -7., -6., -5., -4., -3., -2., -1., 0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., -10.};
    float vz[22] = {0};
    for (int index = 0; index < 22; index++) {
        vz[index] = (vy[index] * vy[index])/10; // parabola equation
    }
    glBegin(GL_QUAD_STRIP);
        for (int i = 0; i < 22; i++) {
            glVertex3f(vx, vy[i], vz[i]);
            glVertex3f(wx, vy[i], vz[i]);
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(vx, 0, 10);
        for (int j = 0; j < 21; j++) {
            glVertex3f(vx, vy[j], vz[j]);
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(wx, 0, 10);
        for (int j = 0; j < 21; j++) {
            glVertex3f(wx, vy[j], vz[j]);
        }
    glEnd();
}

void drawHeliBody()
{
    glColor3f(0.941, 0, 0);
    glPushMatrix();
        glTranslatef(0, 15, 0);
        glScalef(20, 20, 30);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 15, 25);
        glRotatef(180, 1, 0, 0);
        drawHeliFront();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 25, 0);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(1, 5, 5, 5);
    glPopMatrix();
}

void drawPyramid()
{
    glBegin( GL_TRIANGLES );
    glVertex3f( 0.0f, 1.f, 0.0f );
    glVertex3f( -1.0f, -1.0f, 1.0f );
    glVertex3f( 1.0f, -1.0f, 1.0f);

    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f( -1.0f, -1.0f, 1.0f);
    glVertex3f( 0.0f, -1.0f, -1.0f);

    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);

    glVertex3f( -1.0f, -1.0f, 1.0f);
    glVertex3f( 0.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);

    glEnd();
}

void drawHeliTail()
{
    glColor3f(0.941, 0, 0);
    glPushMatrix();
        glTranslatef(0, 15, -10);
        glutSolidSphere(10, 10, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 15, -35);
        glRotatef(-90, 1, 0, 0);
        glScalef(5, 40, 5);
        drawPyramid();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 16, -73);
        glScalef(0.2, 4, 4);
        glutSolidCube(1);
    glPopMatrix();
}

void drawHeliBlades()
{
    glColor3f(0.321, 0.321, 0.321);
    glPushMatrix();
        glTranslatef(0, 30, 0);
        glRotatef(blade_angle, 0, 1, 0);
        glScalef(100, 0.5, 2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 30, 0);
        glRotatef(blade_angle, 0, 1, 0);
        glScalef(2, 0.5, 100);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.1, 17, -74);
        glRotatef(blade_angle, 1, 0, 0);
        glScalef(0.2, 15, 1);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.1, 17, -74);
        glRotatef(blade_angle, 1, 0, 0);
        glScalef(0.2, 1, 15);
        glutSolidCube(1);
    glPopMatrix();
}

void drawHeliPad() {
    bool flag = false;

    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        for(float x = -0.5; x <= 0.5; x += 0.05)
        {
            for(float z = -0.55; z <= 0.5; z += 0.05)
            {
                if(flag) glColor3f(0.231, 0.231, 0.231);
                else glColor3f(0.458, 0.458, 0.458);
                glVertex3f(x, 0, z);
                glVertex3f(x, 0, z+0.05);
                glVertex3f(x+0.05, 0, z+0.05);
                glVertex3f(x+0.05, 0, z);
                flag = !flag;
            }
        }
        glVertex3f(-0.5, 0, -0.55);
        glVertex3f(-0.5, -2, -0.55);
        glVertex3f(0.5, -2, -0.55);
        glVertex3f(0.5, 0, -0.55);

        glVertex3f(-0.5, 0, 0.5);
        glVertex3f(-0.5, -2, 0.5);
        glVertex3f(0.5, -2, 0.5);
        glVertex3f(0.5, 0, 0.5);

        glVertex3f(-0.5, 0, -0.55);
        glVertex3f(-0.5, -2, -0.55);
        glVertex3f(-0.5, -2, 0.5);
        glVertex3f(-0.5, 0, 0.5);

        glVertex3f(0.5, 0, -0.55);
        glVertex3f(0.5, -2, -0.55);
        glVertex3f(0.5, -2, 0.5);
        glVertex3f(0.5, 0, 0.5);
    glEnd();
}

void drawHelicopter()
{
    glPushMatrix();
        glTranslatef(80, heli_hgt, -70);
        drawHeliFeet();
        drawHeliBody();
        drawHeliTail();
        drawHeliBlades();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(80, 2, -70);
        glScalef(80, 1, 80);
        drawHeliPad();
    glPopMatrix();
}

void drawBouncingBall()
{
    glColor3f(1, 0, 0);
    glPushMatrix();
        glTranslatef(0, 10 + ball_height, 0);
        glutSolidSphere(10, 20, 20);
    glPopMatrix();
    glColor3f(0.321, 0.321, 0.321);
    glPushMatrix();
        glTranslatef(0, vice_length / 2, 0);
        glScalef(30, vice_length, 30);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 150 - vice_length / 2, 0);
        glScalef(30, vice_length, 30);
        glutSolidCube(1);
    glPopMatrix();
}

void drawSkyBox()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);
        // Back
        glTexCoord2f(0.25, 0.6666); glVertex3f(-0.5, -0.5, -0.5);
        glTexCoord2f(0.25, 0.3334); glVertex3f(-0.5, 0.5, -0.5);
        glTexCoord2f(0.5, 0.3334);  glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(0.5, 0.6666);  glVertex3f(0.5, -0.5, -0.5);
        // Front
        glTexCoord2f(1, 0.6666);    glVertex3f(-0.5, -0.5, 0.5);
        glTexCoord2f(1, 0.3334);    glVertex3f(-0.5, 0.5, 0.5);
        glTexCoord2f(0.75, 0.3334); glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(0.75, 0.6666); glVertex3f(0.5, -0.5, 0.5);
        // Left
        glTexCoord2f(0, 0.6666);    glVertex3f(-0.5, -0.5, 0.5);
        glTexCoord2f(0, 0.3334);    glVertex3f(-0.5, 0.5, 0.5);
        glTexCoord2f(0.25, 0.3334); glVertex3f(-0.5, 0.5, -0.5);
        glTexCoord2f(0.25, 0.6666); glVertex3f(-0.5, -0.5, -0.5);
        // Right
        glTexCoord2f(0.75, 0.6666); glVertex3f(0.5, -0.5, 0.5);
        glTexCoord2f(0.75, 0.3334); glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(0.5, 0.3334);  glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(0.5, 0.6666);  glVertex3f(0.5, -0.5, -0.5);
        // Top
        glTexCoord2f(0.25, 0.3333); glVertex3f(-0.5, 0.5, -0.5);
        glTexCoord2f(0.5, 0.3333);  glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(0.5, 0);       glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(0.25, 0);      glVertex3f(-0.5, 0.5, 0.5);
        // Bottom
        glTexCoord2f(0.25, 1);      glVertex3f(-0.5, -0.5, -0.5);
        glTexCoord2f(0.25, 0.6667); glVertex3f(0.5, -0.5, -0.5);
        glTexCoord2f(0.5, 0.6667);  glVertex3f(0.5, -0.5, 0.5);
        glTexCoord2f(0.5, 1);       glVertex3f(-0.5, -0.5, 0.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawShapeWithShadow()
{
    float shadowMat[16] = {100, 0, 0, 0, -0, 0, -0, -1, 0, 0, 100, 0, 0, 0, 0, 100};
    glDisable(GL_LIGHTING);
    glPushMatrix(); //Draw Shadow
        glMultMatrixf(shadowMat);
        glTranslatef(-50, 0, -50);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        glPushMatrix();
            glRotatef(45, 0, 1, 0);
            glColor4f(0.2, 0.2, 0.2, 1.0);
            glPushMatrix();
                glTranslatef(0, 50, 0);
                glRotatef(22.5, 0, 0, 1);
                glScalef(20, 20, 20);
                glutSolidCube(1);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0, 50, 0);
                glRotatef(22.5, 0, 0, 1);
                glRotatef(45, 0, 0, 1);
                glScalef(20, 20, 20);
                glutSolidCube(1);
            glPopMatrix();
            glColor4f(0.2, 0.2, 0.2, 1.0);
            glPushMatrix();
                glTranslatef(0, 20, 0);
                glScalef(25, 40, 25);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPushMatrix(); //Draw Object
        glTranslatef(-50, 0, -50);
        glPushMatrix();
            glRotatef(45, 0, 1, 0);
            glColor3f(1, 1, 0);
            glPushMatrix();
                glTranslatef(0, 50, 0);
                glRotatef(22.5, 0, 0, 1);
                glScalef(20, 20, 20);
                glutSolidCube(1);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0, 50, 0);
                glRotatef(22.5, 0, 0, 1);
                glRotatef(45, 0, 0, 1);
                glScalef(20, 20, 20);
                glutSolidCube(1);
            glPopMatrix();
            glColor3f(0, 0, 1);
            glPushMatrix();
                glTranslatef(0, 20, 0);
                glScalef(25, 40, 25);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void myTimer(int value)
{
    if (heli_move) {
        if (heli_up) {
            blade_angle += 30.0;
        } else {
            blade_angle += 15.0;
        }
        if (blade_angle >= 360) {
            blade_angle = 0.0;
        }
        if (heli_up) {
            heli_hgt += 0.5;
        } else {
            heli_hgt -= 0.5;
        }
        if (heli_hgt >= 100) {
            heli_up = false;
        }
        if (heli_hgt <= 2) {
            heli_up = true;
            if (!heli_on) {
                heli_move = false;
            }
        }
    }

    if (ball_move) {
        // Ball Physics
        ball_velocity_new = ball_velocity + grav * 0.05;
        ball_height = ball_height + ((ball_velocity + ball_velocity_new) / 2) * 0.05;
        ball_velocity = ball_velocity_new;
        if (ball_height <= vice_length) {
            ball_height = vice_length;
            if (abs(ball_velocity) < 20) {
                ball_velocity = 0;
                ball_move = false;
            }
            ball_velocity = ((3 * -ball_velocity) / 4);
        }
        if (ball_height >= (130 - vice_length)) {
            ball_height = 130 - vice_length;
            ball_velocity = ((3 * -ball_velocity) / 4);
        }
    }
    if (vice_length > ball_height) {
        ball_height = vice_length;
    }
    if (!ball_move && vice_length < ball_height) {
        ball_move = true;
    }

    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);
}

void display()  
{
	float lpos[4] = {0., 100., 0., 1.0};  //main light's position

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (cam_hgt < 40) {
	    cam_hgt = 40.0;
	}
    if (cam_hgt > 400) {
        cam_hgt = 400.0;
    }
	if (cam_x > 300) {
	    cam_x = 300.0;
	}
    if (cam_x < -300) {
        cam_x = -300.0;
    }
    if (cam_z > 300) {
        cam_z = 300.0;
    }
    if (cam_z < -300) {
        cam_z = -300.0;
    }

    gluLookAt(	cam_x, cam_hgt, cam_z,
                  cam_x+lx, cam_hgt,  cam_z+lz,
                  0.0f, 1.0f,  0.0f);

	glLightfv(GL_LIGHT1, GL_POSITION, lpos);

    // spotlight on bottom of heli facing downwards
    float spotdir[] = {0, -1, 0};
    float spotpos[] = {80, heli_hgt + 2, -70, 1.0};
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 40.0);
    glLightfv(GL_LIGHT0, GL_POSITION, spotpos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotdir);


    glRotatef(angle, 0.0, 1.0, 0.0);    //rotate the whole scene

	drawFloor();

    drawMuseum();

    drawHelicopter();

    glPushMatrix();
        glTranslatef(-100, 0, 100);
        drawBouncingBall();
    glPopMatrix();

    drawShapeWithShadow();

    glPushMatrix();
        glTranslatef(0, 50, 0);
        glScalef(1000, 1000, 1000);
        drawSkyBox();
    glPopMatrix();

	glFlush();
}

void initialize()
{
    loadTexture();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0., 0., 0.);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.0, 40.0, 1000.);
}

void special(int key, int x, int y)
{
    float speed = 5.0f;

    switch (key) {
        case GLUT_KEY_LEFT :
            angle -= 0.05f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT :
            angle += 0.05f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_UP :
            cam_x += lx * speed;
            cam_z += lz * speed;
            break;
        case GLUT_KEY_DOWN :
            cam_x -= lx * speed;
            cam_z -= lz * speed;
            break;
    }
    
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    float speed = 5.0f;

    switch (key){
        case 'w':
            cam_hgt += 2;
            break;

        case 's':
            cam_hgt -= 2;
            break;
        case 'a':
            cam_x += lz * speed;
            cam_z -= lx * speed;
            break;
        case 'd':
            cam_x -= lz * speed;
            cam_z += lx * speed;
            break;
        case 'h':
            if (heli_move && !heli_on) {
                break;
            }
            if (heli_on) {
                heli_on = false;
                heli_up = false;
            } else {
                heli_on = true;
                heli_move = true;
            }
            break;
        case 'b':
            ball_velocity = 100;
            ball_move = true;
            break;
        case 'o':
            if (vice_length < 50) {
                vice_length += 1;
                break;
            }
            break;
        case 'l':
            if (vice_length > 1) {
                vice_length -= 1;
                break;
            }
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Museum");
   initialize();

   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutTimerFunc(5, myTimer, 0);
   glutMainLoop();
   return 0;
}
