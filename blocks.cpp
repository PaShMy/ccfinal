#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;

enum BoxDirect{DIR_UP,DIR_DOWN,DIR_LEFT,DIR_RIGHT};
enum BoxMap   {MAP_NONE,MAP_WALL,MAP_DEST};
int REACH_BOX_NUM=0;

int M ;
int N ;
int Box_Num=1 ;
int Curent_Status=1;

int Map[32][32];

typedef struct Point{
	int x,y;
}point;


point Box_Position[100];
point People_Position;


GLfloat xspin = 0;
GLfloat yspin = 0;
GLfloat zspin = 0;
GLfloat zoom = -15;

GLfloat xpos;
GLfloat ypos;

GLuint	texture[3];			// Storage For 1 Texture
GLuint	box,dest;				// Storage For The Box Display List

bool Init_Reached_Box()
{
	REACH_BOX_NUM=0;
	int tempx,tempy;
	for(int i=0;i<Box_Num;i++)
	{
		tempx=Box_Position[i].x;
		tempy=Box_Position[i].y;
		if(Map[tempx][tempy]==MAP_DEST)
			REACH_BOX_NUM++;
	}
	return true;
}

void readworld()
{
	int i,j,k;
	ifstream infile("./Data/map.txt");
	for( k=0;k<Curent_Status;k++)
	{
		infile >> M;
//		cout<<"M = "<<M<<endl;
		infile >> N;
//		cout<<"N = "<<N<<endl;
		infile >> Box_Num;
		for( i=0;i<M;i++)
		{
			for( j =0;j<N;j++)
			{
				infile>>Map[i][j];
//				cout<<"Map["<<i<<"]["<<j<<"] = "<<Map[i][j]<<endl;
			}
		}
		for(i=0;i<Box_Num;i++)
		{
			infile>>Box_Position[i].x;
			infile>>Box_Position[i].y;
//			cout<<"Box Position : "<<i<<" = "<<Box_Position[i].x<<"  "<<Box_Position<<endl;
		}
		infile >>People_Position.x;
		infile >>People_Position.y;
//		cout<<"People Position = "<<People_Position.x<<"   "<<People_Position<<endl;
	}
	infile.close();
	Init_Reached_Box();
}



int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=true;									// Status Indicator
	unsigned char pImage[256*256*3];	
	FILE *f1,*f2,*f3; 
	f1=fopen("./Data/wall.bmp","rb"); 
	fseek(f1,0,SEEK_SET); 
	fread(pImage,256*256*3,1,f1); 
	fclose(f1); 
	glGenTextures(3,texture); 
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	f2=fopen("./Data/box.bmp","rb"); 
	fseek(f2,54,SEEK_SET); 
	fread(pImage,256*256*3,1,f2); 
	fclose(f2); 
	glBindTexture(GL_TEXTURE_2D,texture[1]);   
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	f3=fopen("./Data/people.bmp","rb"); 
	fseek(f3,54,SEEK_SET); 
	fread(pImage,256*256*3,1,f3); 
	fclose(f3); 
	glBindTexture(GL_TEXTURE_2D,texture[2]);   
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	return Status;
}

GLvoid BuildLists()
{
	box=glGenLists(2);									// Generate 2 Different Lists
	glNewList(box,GL_COMPILE);							// Start With The Box List
		glBegin(GL_QUADS);
			// Bottom Face
			glNormal3f( 0.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
			// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
			// Back Face
			glNormal3f( 0.0f, 0.0f,-1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
			// Right face
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
			// Left Face
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
			// Top Face
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glEnd();
	glEndList();
	dest = box+1;
	glNewList(dest,GL_COMPILE);							// Start With The Box List
	glEndList();

}


void init(void)
{
	if (!LoadGLTextures())
	{
		printf("LoadGLTextures()");
		return ;
	}
	readworld();
	BuildLists();
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glColor4f(1.0f,1.0f,1.0f,0.5f);			// ȫ���ȣ� 50% Alpha ���?
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// ����Դ����alphaͨ��ֵ�İ�͸����Ϻ���?
}

void display(void)
{
	int i,j;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glTranslatef(xpos,ypos,zoom);
	glRotatef(xspin,1.0,0.0,0.0);
	glRotatef(yspin,0.0,1.0,0.0);
	glRotatef(zspin,0.0,0.0,1.0);
	for( i=0;i<M;i++)
	{
		for( j = 0 ; j<N;j++)
		{
			switch(Map[i][j])
			{
			case MAP_NONE:
				break;
			case MAP_WALL:
				glBindTexture(GL_TEXTURE_2D, texture[0]);
				glPushMatrix();
				glTranslatef((i-M/2)*2,(j-N/2)*2,0);
				glCallList(box);
				glPopMatrix();
				break;
			case MAP_DEST:
				glPushMatrix();
				glTranslatef((i-M/2)*2,(j-N/2)*2,0);
			//	glColor3f(0.5,0.8,0.0);
				glutSolidSphere(1,50,50);
				glPopMatrix();
			}

		}
	}

	for(i=0;i<Box_Num;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glPushMatrix();
		glTranslatef((Box_Position[i].x-M/2)*2,(Box_Position[i].y-N/2)*2,0);
		if(Map[Box_Position[i].x][Box_Position[i].y] == MAP_DEST)
		{
			glEnable(GL_BLEND);		// �򿪻��?
			glDisable(GL_DEPTH_TEST);	// �ر���Ȳ���?
		}
	//	glColor3f(0.5,0.8,0.0);
		glCallList(box);
		glPopMatrix();
		glDisable(GL_BLEND);		// �رջ��?
		glEnable(GL_DEPTH_TEST);	// ����Ȳ���?
	}



	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glPushMatrix();
	if(Map[People_Position.x][People_Position.y] == MAP_DEST)
	{
		glEnable(GL_BLEND);		// �򿪻��?
		glDisable(GL_DEPTH_TEST);	// �ر���Ȳ���?
	}
	glTranslatef((People_Position.x-M/2)*2,(People_Position.y-N/2)*2,0);
//	glColor3f(0.0,0.8,0.8);
	glCallList(box);
	glPopMatrix();
	glDisable(GL_BLEND);		// �رջ��?
	glEnable(GL_DEPTH_TEST);	// ����Ȳ���?
	glPopMatrix();
	glutSwapBuffers();
	if(REACH_BOX_NUM == Box_Num)
	{
		
			Curent_Status++;
			readworld();
			glutPostRedisplay();
		
	}
}

void reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool Is_Box(int i,int j)
{
	for(int k=0;k<Box_Num;k++)
	{
		if(Box_Position[k].x==i && Box_Position[k].y ==j)
			return true;
	}
	return false;
}

bool Is_Move_Ok(int Direct)
{
	int curx=People_Position.x;
	int cury=People_Position.y;
	switch(Direct)
	{
	case DIR_DOWN:
		if(cury==0 || (Map[curx][cury-1]==MAP_WALL) || (Is_Box(curx,cury-1)&&(cury-1==0 || (Map[curx][cury-2]==MAP_WALL)  ||Is_Box(curx,cury-2) ) ) )
			return false;
		break;
	case DIR_UP:
		if(cury==N-1 ||(Map[curx][cury+1]==MAP_WALL) ||(Is_Box(curx,cury+1)&&(cury+1==N-1 ||(Map[curx][cury+2]==MAP_WALL) ||Is_Box(curx,cury+2) ) ) )
			return false;
		break;
	case DIR_LEFT:
		if(curx==0 || (Map[curx-1][cury]==MAP_WALL) ||(Is_Box(curx-1,cury)&&(curx-1==0 || (Map[curx-2][cury]==MAP_WALL)   ||Is_Box(curx-2,cury) ) ) )
			return false;
		break;
	case DIR_RIGHT:
		if(curx==M-1 ||(Map[curx+1][cury]==MAP_WALL) ||(Is_Box(curx+1,cury)&&(curx+1==M-1 || (Map[curx+2][cury]==MAP_WALL)  ||Is_Box(curx+2,cury) ) ) )
			return false;
		break;
	}
	return true;
}

point* GetBoxPosition(int i,int j)
{
	for(int k=0;k<Box_Num;k++)
	{
		if(Box_Position[k].x==i && Box_Position[k].y ==j)
			return &Box_Position[k];
	}
	return NULL;
}


bool Move_Down(void )
{
	int curx=People_Position.x;
	int cury=People_Position.y;
	People_Position.y-=1;
	if(Map[curx][cury-1]==MAP_DEST && Is_Box(curx,cury-1))
		REACH_BOX_NUM-=1;
	if(Is_Box(curx,cury-1))
	{
		 point *temp=GetBoxPosition(curx,cury-1);
		 temp->y-=1;
		 if(Map[curx][cury-2]==MAP_DEST)
		       REACH_BOX_NUM+=1;
	 }
	 return true;
}
bool  Move_Up(void)
{
	int curx=People_Position.x;
	int cury=People_Position.y;
	People_Position.y+=1;
	if(Is_Box(curx,cury+1) && Map[curx][cury+1]==MAP_DEST )
		REACH_BOX_NUM-=1;
	if(Is_Box(curx,cury+1) )
	{
		 point *temp=GetBoxPosition(curx,cury+1);
		 temp->y+=1;
		 if(Map[curx][cury+2]==MAP_DEST)
			REACH_BOX_NUM+=1;
	 }
	return true;
}
bool Move_Left(void)
{
	int curx=People_Position.x;
	int cury=People_Position.y;
	People_Position.x-=1;
	if(Is_Box(curx-1,cury) && Map[curx-1][cury]==MAP_DEST)
		REACH_BOX_NUM-=1;
	if(Is_Box(curx-1,cury))
	{
		 point *temp=GetBoxPosition(curx-1,cury);
		 temp->x-=1;
		 if(Map[curx-2][cury]==MAP_DEST)
			REACH_BOX_NUM+=1;
	 }
	return true;
}
bool Move_Right(void)
{
	int curx=People_Position.x;
	int cury=People_Position.y;
	People_Position.x+=1;
	if(Is_Box(curx+1,cury) && Map[curx+1][cury]==MAP_DEST)
		REACH_BOX_NUM-=1;
	if(Is_Box(curx+1,cury))
	{
		 point *temp=GetBoxPosition(curx+1,cury);
		 temp->x+=1;
		 if(Map[curx+2][cury]==MAP_DEST)
			REACH_BOX_NUM+=1;
	 }

	return true;
}


void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{

	case '<':
	case ',':
	case '6':
		yspin += 1.0;
		//if(yspin>45.0)
		//	yspin=45.0;
		break;
	case '>':
	case '.':
	case '4':
		yspin -= 1.0;
	//	if(yspin<-45.0)
		//	yspin=-45.0;
		break;
	case '=':
	case '+':
		zoom +=2;
		break;
	case '-':
	case '_':
		zoom -=2;
		break;
	case '(':
	case '2':
		xspin +=2;
		break;
	case ')':
	case '8':
		xspin-=2;
		break;
	case '9':
		zspin+=2;
		break;
	case '1':
		zspin-=2;
		break;
	case '5':
		xspin = 0.0;
		yspin=0;
		zspin = 0;
		zoom = -15;
		xpos=0;
		ypos=0;
		break;

	case '*':
		glutFullScreen();
		break;
	case '&':
	case 'Q':
		glutReshapeWindow(700,500);
		glutPositionWindow(100,100);
		break;
	case 'w':
	case 'W':
		if(Is_Move_Ok(DIR_UP))
		{
				Move_Up();
		}
		break;
	case 'a':
	case 'A':
		if(Is_Move_Ok(DIR_LEFT))
		{
				Move_Left();
		}
		break;
	case 'd':
	case 'D':
		if(Is_Move_Ok(DIR_RIGHT))
		{
				Move_Right();
		}
		break;
	case 's':
	case 'S':
		if(Is_Move_Ok(DIR_DOWN))
		{
				Move_Down();
		}
		break;
	case 'R':
	case 'r':
		readworld();
		break;
	case 'N':
	case 'n':
		Curent_Status+=1;
		readworld();
		break;
	case 'P':
	case 'p':
		Curent_Status-=1;
		readworld();
		break;

	}
	glutPostRedisplay();
}

void specialkeyboard(int key,int x,int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		ypos+=2;
		break;
	case GLUT_KEY_DOWN:
		ypos-=2;
		break;
	case GLUT_KEY_LEFT:
		xpos-=2;
		break;
	case GLUT_KEY_RIGHT:
		xpos+=2;
		break;

	}
	glutPostRedisplay();
}


void menu(int value)
{
	switch(value)
	{
	case 1:
		break;
	case 2:
		break;
	}
}


int main(int argc, char **argv)

{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(700, 500);

    glutInitWindowPosition(100,100);


    glutCreateWindow(argv[0]);

    init();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	glutSpecialFunc(specialkeyboard);

	glutCreateMenu(menu);
	glutAddMenuEntry("����",1);
	glutAddMenuEntry("����˵��",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return 0;

}

