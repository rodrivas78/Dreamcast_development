
// Dreamcast development experiments 
// by Rodrigo G. Rivas - rodrivas78
// Terrain generation with perlin noise + Control Inputs + Skybox
//
// KallistiOS 2.0.0


#include <kos.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include "perlin.h"
#include "vector.h"
#include "input.h"

////// Definições do terreno 
#define BITMAP_ID 0x4D42		      // the universal bitmap ID
#define MAP_X	128			          // size of map along x-axis
#define MAP_Z	128					// size of map along z-axis
#define MAP_SCALE	2.0f		         // the scale of the terrain map //Diminuindo fica mais íngrime/escarpado

//#define PI						3.14159265

////// Terrain Data
float terrain[MAP_X][MAP_Z][4];		// heightfield terrain data (0-255); 256x256

static vector3f      up = { 0,  1,  0 },
                     camFrom = { -15, 5.0, 5.0 },
                     camTo = { -5, 5, 5.0 };


int z;
int x;


GLuint texture[2];  /* Storage For One Texture */

/* Load a PVR texture - located in pvr-texture.c */
extern GLuint glTextureLoadPVR(char *fname, unsigned char isMipMapped, unsigned char glMipMap);

void glSetCameraPosition(vector3f campos, vector3f camdst) {
    /* Set up GL Render Stack based on Camera Perspective */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glhLookAtf2(campos, camdst, up);
	
	 glBindTexture(GL_TEXTURE_2D, texture[0]);

  for (z = 0; z < MAP_Z-1; z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (x = 0; x < MAP_X-1; x++)
		{
			
			glColor3f(-terrain[x][z][1]/255.0f, -terrain[x][z][1]/255.0f, -terrain[x][z][1]/255.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(terrain[x][z][0], -terrain[x][z][1], terrain[x][z][2]);

			// draw vertex 1
			glColor3f(-terrain[x+1][z][1]/255.0f, -terrain[x+1][z][1]/255.0f, -terrain[x+1][z][1]/255.0f);
			glTexCoord2f(1.0f, 0.0f);		   
			glVertex3f(terrain[x+1][z][0], -terrain[x+1][z][1], terrain[x+1][z][2]);

			// draw vertex 2
			glColor3f(-terrain[x][z+1][1]/255.0f, -terrain[x][z+1][1]/255.0f, -terrain[x][z+1][1]/255.0f);
			glTexCoord2f(0.0f, 1.0f);			
			glVertex3f(terrain[x][z+1][0], -terrain[x][z+1][1], terrain[x][z+1][2]);

			// draw vertex 3
			glColor3f(-terrain[x+1][z+1][1]/255.0f, -terrain[x+1][z+1][1]/255.0f, -terrain[x+1][z+1][1]/255.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(terrain[x+1][z+1][0], -terrain[x+1][z+1][1], terrain[x+1][z+1][2]);
		}
		glEnd();
	}
		
}

void draw_gl(void) {
	
	
  	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	glTranslatef((MAP_X*2)/2, 0.0f, (-MAP_Z*2)/2);

    glBegin(GL_QUADS);
    /* Front Face */
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    /* Back Face */
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    /* Top Face */
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    /* Bottom Face */ 
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    /* Right face */ 
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    /* Left Face  */
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();	
}


void Draw_Skybox(float x2, float y2, float z2, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x2 = x2 - width  / 2;
	y2 = y2 - (height*1.2) / 2;
	z2 = z2 - length / 2;


	// Draw Front side
	 glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x2,		  y2,		z2+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x2,		  y2+height, z2+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x2+width, y2+height, z2+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x2+width, y2,		z2+length);
	//glEnd();

	// Draw Back side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x2+width, y2,		z2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x2+width, y2+height, z2); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x2,		  y2+height,	z2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x2,		  y2,		z2);
	//glEnd();

	// Draw Left side
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x2,		  y2+height,	z2);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x2,		  y2+height,	z2+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x2,		  y2,		z2+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x2,		  y2,		z2);		
	//glEnd();

	// Draw Right side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x2+width, y2,		z2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x2+width, y2,		z2+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x2+width, y2+height,	z2+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x2+width, y2+height,	z2);
	//glEnd();

	// Draw Up side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x2+width, y2+height, z2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x2+width, y2+height, z2+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x2,		  y2+height,	z2+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x2,		  y2+height,	z2);
	//glEnd();

	// Draw Down side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x2,		  y2,		z2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x2,		  y2,		z2+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x2+width, y2,		z2+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x2+width, y2,		z2);
	glEnd();

}

void InitializeTerrain()
{

	for (z = 0; z < MAP_Z; z++)
	{
		for (x = 0; x < MAP_X; x++)
		{
			terrain[x][z][0] = (x)*MAP_SCALE;					
			//terrain [x][z][1] =  perlin2d(x, z, 0.08, 15)*4; //Muito boa 	
			terrain [x][z][1] =  perlin2d(x, z, 0.05, 15)*4; //Muito boa 			
			terrain[x][z][2] = -(z)*MAP_SCALE;
		}
	}
	
}

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

int main(int argc, char **argv) {
    maple_device_t *cont;
    cont_state_t *state;

    printf("nehe06 beginning\n");

    /* Get basic stuff initialized */
    glKosInit();
	
	InitializeTerrain();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	//gluPerspective(54.0f,800.0f/600.0f,1.0f,1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);	
	glEnable(GL_KOS_NEARZ_CLIPPING);
    glDepthFunc(GL_LEQUAL);

    /* Set up the texture */
    texture[0] = glTextureLoadPVR("/rd/greengrass128.pvr", 1, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_FILTER, GL_LINEAR);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	texture[1] = glTextureLoadPVR("/rd/starbox.pvr", 0, 0);		   
   

    while(1) {
        InputCallback(camFrom, camTo);

        glSetCameraPosition(camFrom, camTo);
        /* Draw the GL "scene" */
        draw_gl();
		
		Draw_Skybox(0,0,0,512,512,512);	// Draw the Skybox
		
		/* Finish the frame */
        glutSwapBuffers();
		
		
    }
	
	

    return 0;
}
