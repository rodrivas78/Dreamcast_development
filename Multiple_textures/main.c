
// Dreamcast development experiments 
// by Rodrigo G. Rivas - Rodrivas78
// "Ancient Stadium"  /  Loading multiple textures + Control Inputs
// KallistiOS 2.0.0


#include <kos.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#include "vector.h"
#include "input.h"


static vector3f      up = { 0,  1,  0 },
                     camFrom = { -15, 5.0, 5.0 },
                     camTo = { -5, 5, 5.0 };


int g = 45;

int var;
//VX=X2:VY=Y2:B=0:PX=10:PY=170:AX=0:AY=1
float vx = 0.1f;
float vy = 0.7f;
float vz = 0.9f;

float az = 0;
float ay = - 0.11;
float ax = 0;

float t = 0.02;

float newPosX;
float newPosZ;
int newIntPosX;
int newIntPosY;
int newIntPosZ;

double obsX, obsY, obsZ = 5;
//double obsX = 10.0f;
//gluLookAt(-15.0,2.0,5.0, obsX, obsY, obsZ, 0.0,1.0,0.0);
double camX = -15.0;
double camY = 2.0;
double camZ = 5.0;
float s1;
float s2;

int cx = 0;
int cy = 0;
int cz = 0;

bool random;
//bool shoot = false;
bool contact;

float sinCos;


struct vector3 
{
	float x;
	float y;
	float z;
	
} Vector3;

 void applyAngle()
   {
	   Vector3.z = Vector3.z * cos(sinCos)  - Vector3.z * sin(sinCos) / 25;
	   Vector3.x = Vector3.x * cos(sinCos)  - Vector3.z * sin(sinCos) / 25;
			
   }

   void integrate()
   {
	Vector3.x = Vector3.x + vx * t;
    Vector3.y = Vector3.y + vy * t;
	Vector3.z = Vector3.z + vz * t;
	
	vx = vx + ax * t;
	vy = vy + ay * t;
    vz = vz + az * t;	
   }

GLuint texture[4];  /* Storage For One Texture */

/* Load a PVR texture - located in pvr-texture.c */
extern GLuint glTextureLoadPVR(char *fname, unsigned char isMipMapped, unsigned char glMipMap);

void glSetCameraPosition(vector3f campos, vector3f camdst) {
    /* Set up GL Render Stack based on Camera Perspective */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glhLookAtf2(campos, camdst, up);
	
		
}

void drawQuads() {

 //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
 glTranslatef(20, 1, 37);
glBindTexture(GL_TEXTURE_2D, texture[0]); 
 glColor3f(1,1,1);
 
 glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
	    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
glEnd();
 	

}

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	
	//Teste
	
	
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	// Draw Front side
	//glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);
	glTranslatef(0,0,400);
	glBegin(GL_QUADS);	
	     glColor3f(1, 1, 1);  //red
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();
	
	// Draw Back side
	//glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYBACK]);
	glTranslatef(0,0,-400);
	glBegin(GL_QUADS);		
	glColor3f(0.80, 0.80, 0.80); //red
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();
  
	//Balcony 
	
	/*
	glBindTexture(GL_TEXTURE_2D, texture[3]);	
	glTranslatef(0,100, 200);
	glBegin(GL_QUADS);		
	glColor3f(1, 1, 1); //red
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();
	*/
	glTranslatef(0,100, 200);
	
	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, texture[0]);	
	glTranslatef(0,-100, -200);
	glBegin(GL_QUADS);		
	glColor3f(1, 1, 1); //blue
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	  glTranslatef(0,0,200);
	glBegin(GL_QUADS);		
	glColor3f(1, 1, 1); //blue
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();
	  glTranslatef(0,0,200);
	glBegin(GL_QUADS);		
	glColor3f(1, 1, 1); //blue
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	 glTranslatef(0,0,-400);
	glBegin(GL_QUADS);		
	   glColor3f(1, 1, 1);  //cyan
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();
	//Right Side 2
	glTranslatef(0,0,200);
	glBegin(GL_QUADS);		
	   glColor3f(1, 1, 1);  //cyan
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();
	glTranslatef(0,0,200);
	glBegin(GL_QUADS);		
	   glColor3f(1, 1, 1);  //cyan
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glTranslatef(0,0,-400);
	glBegin(GL_QUADS);		
	 glColor3f(0.75, 0.75, 0.75);  //yellow
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, texture[1]); 
	glBegin(GL_QUADS);		
	 glColor3f(1, 1, 1); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+0.5f,		z);
		glTexCoord2f(40.f, 40.0f); glVertex3f(x,		  y+0.5f,		z+length);
		glTexCoord2f(40.0f, 0.0f); glVertex3f(x+width, y+0.5f,		z+length); 
		glTexCoord2f(0.0f, 40.0f); glVertex3f(x+width, y+0.5f,		z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]); 
    glTranslatef(0,0,200);
	glBegin(GL_QUADS);		
	 glColor3f(1, 1, 1); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(40.f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(40.0f, 40.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 40.0f); glVertex3f(x+width, y,		z);
	glEnd();

	glTranslatef(0,0,200);
	glBegin(GL_QUADS);	
	 glColor3f(1, 1, 1); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(40.f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(40.0f, 40.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 40.0f); glVertex3f(x+width, y,		z);
	glEnd();

}




extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

int main(int argc, char **argv) {
    maple_device_t *cont;
    cont_state_t *state;

    printf("nehe06 beginning\n");

    /* Get basic stuff initialized */
    glKosInit();
	
	//InitializeTerrain();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	//gluPerspective(54.0f,800.0f/600.0f,1.0f,1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
   // glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
   // glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);	
	glEnable(GL_KOS_NEARZ_CLIPPING);
    glDepthFunc(GL_LEQUAL);

    /* Set up the texture */
	//Wallss
    texture[0] = glTextureLoadPVR("/rd/walls.pvr", 0, 0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //cobbles
	texture[1] = glTextureLoadPVR("/rd/cobbles.pvr", 0, 0);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//grass
	texture[2] = glTextureLoadPVR("/rd/grass.pvr", 0, 0);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//arcos	
	texture[3] = glTextureLoadPVR("/rd/arcos512.pvr", 0, 0);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	   
   

    while(1) {
        InputCallback(camFrom, camTo);

        //GLuint start = GetTime();
        //InputCb();
        glSetCameraPosition(camFrom, camTo);
        /* Draw the GL "scene" */
        //draw_gl();
		drawQuads();
		Draw_Skybox(0,0,0,512,512,512);	// Draw the Skybox
		
		
		/* Finish the frame */
        glutSwapBuffers();
		
		
    }
	
	

    return 0;
}
