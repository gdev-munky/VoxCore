#ifndef _MODEL_
#define _MODEL_

struct Models
{
	void array(GLenum m,int is, int n, float*v, int nv,
		       float*texc=0,float*norm=0,float*c=0);
	void trianglefan(float*v,int nv,float*texc=0,
		             float*norm=0,float*c=0)
	{array(GL_TRIANGLE_FAN,0,nv,v,nv,texc,norm,c);}
	void triangles(float*v,int nv,float*texc=0,
		           float*norm=0,float*c=0)
	{array(GL_TRIANGLES,0,nv,v,nv,texc,norm,c);}
	void points(float*v,int nv,float*texc=0,
		        float*norm=0,float*c=0)
	{array(GL_POINTS,0,nv,v,nv,texc,norm,c);}
	void polygon(float*v,int nv,float*texc=0,
		         float*norm=0,float*c=0)
	{array(GL_POLYGON,0,nv,v,nv,texc,norm,c);}
	void glusphere(GLdouble r=1,int sl=30,int st=30);
	void glucylinder(GLdouble r=1,GLdouble h=1,
		             int sl=30,int st=30);
	void gluconus(GLdouble rb=1,GLdouble h=1,
		          GLdouble ru=0,int sl=30,int st=30);
	void gludisk(GLdouble ro=1,GLdouble ri=0,
		         int sl=30,int st=30);
	void glupartdisk(GLdouble ro=1,GLdouble sa=0,
		             GLdouble swa=360,GLdouble ri=0,
					 int sl=30,int st=30);
	void planeXY(float x1, float y1,float x2,float y2,
		         float z=0,float s=1,float t=1,float*tex=0)
	{
		float p[]={x1,y1,z,x1,y2,z,x2,y2,z,x2,y1,z};
		quads(p,4,0,0,s,t,0);
	}
	void planeXZ(float x1,float z1,float x2,float z2,
		         float y=0,float s=1,float t=1,float*tex=0)
	{
		float p[]={x1,y,z1,x1,y,z2,x2,y,z2,x2,y,z1};
		quads(p,4,0,0,s,t,0);
	}
	void planeYZ(float y1,float z1,float y2,float z2,
		         float x=0,float s=1,float t=1,float*tex=0)
	{
		float p[]={x,y1,z1,x,y1,z2,x,y2,z2,x,y2,z1};
		quads(p,4,0,0,s,t,0);
	}
	void quads(float*v,int nv,float s0=0, float t0=0,
		       float s1=1,float t1=1,float*texc=0,
			   float*norm=0,float*c=0);
	void cube(float l=1,float s0=0,float t0=0,
		      float s1=1,float t1=1);
	void scene1(uint list=0)
	{
		glNewList(list,GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT|GL_POLYGON_BIT);
		glMatrixMode(GL_MODELVIEW);
		glPolygonMode(GL_FRONT,GL_FILL);
		glColor3f(0.4,0.4,1);
		planeXZ(-100,-300,100,-1,-5);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor3f(1,0.5,0.5);
		cube(35);
		glPushMatrix();
		glTranslatef(0,0,-290);
		glColor3f(1,0.5,0.5);
		glusphere(30);
		glPopMatrix();
		glPolygonMode(GL_FRONT,GL_LINE);
		glColor3f(1,0.8,0.8);
		cube(35);
		glPushMatrix();
		glTranslatef(0,0,-290);
		glColor3f(1,0.8,0.8);
		glusphere(30);
		glPopMatrix();
		glPopAttrib();
		glEndList();
	}
	};
    
    void Models::array(GLenum m,int is,int n,float*v,
		               int nv,float*texc,float*norm,float*c)
	{
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(float)*3,v);
		if(texc)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2,GL_FLOAT,sizeof(float)*2,texc);
		} else
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		if(norm)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT,sizeof(float)*3,norm);
    	}else
			glDisableClientState(GL_NORMAL_ARRAY);
		if(c)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3,GL_FLOAT,sizeof(float)*3,c);
		}else
			glDisableClientState(GL_COLOR_ARRAY);
		glDrawArrays(m,is,n);
		glPopClientAttrib();
	}

	void Models::quads(float*v,int nv,float s0,float t0,
		               float s1,float t1,float*texc,
					   float*norm,float*c)
	{
		float* tex=texc;
		if(tex==0)
		{
			tex=new float[nv*2];
			tex[0]=s0; tex[1]=t0; tex[2]=s1; tex[3]=t0;
			tex[4]=s1; tex[5]=t1; tex[6]=s0; tex[7]=t1;
			copy(tex,tex+nv*2-8,tex+8);
		}
		array(GL_QUADS,0,nv,v,nv,tex,norm,c);
		if(texc==0) delete[]tex;
	}

	void Models::cube(float l,float s0,
		              float t0,float s1, float t1)
	{
		l/=2;
		float v[24*3]=
		{
			-1,-1,1,1,-1,1,1,1,1,-1,1,1,
			1,-1,-1,-1,-1,-1,-1,1,-1,1,1,-1,
			1,1,1,1,1,-1,-1,1,-1,-1,1,1,
			1,-1,1,-1,-1,1,-1,-1,-1,1,-1,-1,
			1,-1,1,1,-1,-1,1,1,-1,1,1,1,
			-1,-1,1,-1,1,1,-1,1,-1,-1,-1,-1
		};
		quads(v,24,s0,t0,s1,t1,0,0,0);
	}

	void Models::glupartdisk(GLdouble ro,GLdouble sa,
		                     GLdouble swa,GLdouble ri,int sl,int st)
	{
		GLUquadricObj*obj=gluNewQuadric();
		gluQuadricNormals(obj,GLU_SMOOTH);
		gluQuadricTexture(obj,1);
		if(ro<0)
		{
			gluQuadricOrientation(obj,GLU_INSIDE);
			ro=-ro; ri=-ri;
		}
		gluPartialDisk(obj,ri,ro,sl,st,sa,swa);
		gluDeleteQuadric(obj);
	}

	void Models::gludisk(GLdouble ro,GLdouble ri,int sl,int st)
	{
		GLUquadricObj*obj=gluNewQuadric();
		gluQuadricNormals(obj,GLU_SMOOTH);
		gluQuadricTexture(obj,1);
		if(ro<0)
		{
			gluQuadricOrientation(obj,GLU_INSIDE);
			ro=-ro; ri=-ri;
		}
		gluDisk(obj,ri,ro,sl,st);
		gluDeleteQuadric(obj);
	}

	void Models::gluconus(GLdouble rb,GLdouble h,
		                  GLdouble ru,int sl,int st)
	{
		GLUquadricObj*obj=gluNewQuadric();
		gluQuadricNormals(obj,GLU_SMOOTH);
		gluQuadricTexture(obj,1);
		if(rb<0)
		{
			gluQuadricOrientation(obj,GLU_INSIDE);
			rb=-rb; ru=-ru;
		}
		gluCylinder(obj,rb,ru,h,sl,st);
		gluDeleteQuadric(obj);
	}

	void Models::glucylinder(GLdouble r, GLdouble h,
		                     int sl, int st)
	{
		GLUquadricObj*obj=gluNewQuadric();
		gluQuadricNormals(obj,GLU_SMOOTH);
		gluQuadricTexture(obj,1);
		if(r<0)
		{
			gluQuadricOrientation(obj,GLU_INSIDE);
			r=-r;
		}
		gluCylinder(obj,r,r,h,sl,st);
		gluDeleteQuadric(obj);
	}

	void Models::glusphere(GLdouble r,int sl,int st)
	{
		GLUquadricObj*obj=gluNewQuadric();
		gluQuadricNormals(obj,GLU_SMOOTH);
		gluQuadricTexture(obj,1);
		if(r<0)
		{
			gluQuadricOrientation(obj,GLU_INSIDE);
			r=-r;
		}
		gluSphere(obj,r,sl,st);
		gluDeleteQuadric(obj);
	}
#endif