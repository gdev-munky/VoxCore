#ifndef _IMAGE_
#define _IMAGE_

#pragma pack(push,1)

struct bmpheader
{
	short bmpid;
	ulong fsize;
	ulong tmp1;
	ulong imgdata;
	ulong headersize;
	ulong width,height;
	ushort planes;
	ushort bpp;
	ulong compression;
	ulong imgdatasize;
	ulong HR,VR;
	ulong color;
	ulong imcolor;

	void init()
	{
		bmpid='MB'; headersize=40;
		tmp1=compression=imcolor=HR=VR=0;
		planes=1;
	}
};

struct tgaheader
{
	uchar idlen;
	uchar typepal;
	uchar typeimg;
	ushort colorstart;
	ushort colornum;
	uchar colorbpp;
	ushort x,y;
	ushort width, height;
	uchar bpp;
	uchar ImageDescriptior;

	void init()
	{
		idlen=typepal=typeimg=colorstart=colornum=colorbpp=x=y=width=height=bpp=0;
		ImageDescriptior=0;
	}
};

#pragma pack(pop)

struct IMAGE
{
	int width,
		height,
		bpp,
		sizepal,
		bpppal;
	uchar* inddata;
	uchar* pal;

	void init(int w=320,int h=200,int b=24,
		      int sizepal=0,int bpal=32);

	IMAGE()
	{
		width=0; height=0; bpp=0;
		sizepal=0; inddata=0; pal=0;
	}
	~IMAGE() {clear();}

	void fload_data(uchar*,int,int,int,int,FILE*);
	void fsave_data(uchar*,int,int,int,int,FILE*);
	int loadbmp(char*);
	int loadtga(char*);
	int savebmp(char*);
	int savetga(char*);

	void glsetpal(int,int,int,int,int s=0,int n=256);
	void tocolor();
	void tostencil();
	void fromcolor(int,int);
	int load2dtexture(char*str,uint*,int,int l=0);
	int imagesize();
	void clear();
};

int IMAGE::imagesize()
{
	int k=width*height;
	switch(bpp)
	{
	case 1: k/=8; break;
	case 2: k/=4; break;
	case 4: k/=2; break;
	default: k*=bpp/8;
	};
	return (k<0)?-k:k;
}

void IMAGE::clear()
{
	if(inddata) {delete[]inddata;inddata=0;}
	if(pal) {delete[]pal;pal=0;}
}

void IMAGE::init(int w,int h,int b,
				 int sizepal,int bpal)
{
	width=w;
	height=h;
	bpp=b;
	clear();
	inddata=new uchar[imagesize()];
	if(sizepal)
		pal=new uchar[bpal/8];
}

void IMAGE::fload_data(uchar*dst,int ic,int ia,
					   int max,int order, FILE*f)
{
	for(int i=0;i<max;i++)
	{
		fread(dst,1,ic,f);
		if(order==1||order==2) reverse(dst,dst+ic);
		if(order==2) rotate(dst,dst+1,dst+ic);
		dst+=ia;
	}
}

void IMAGE::fsave_data(uchar*src,int ic,int ia,
					   int max,int order,FILE*f)
{
	uchar *temp=new uchar [ic];
	for(int i=0;i<max;i++)
	{
		copy(src,src+ic,temp);
		if(order==1||order==2) reverse(temp,temp+ic);
		if(order==2) rotate(temp,temp+1,temp+ic);
		fwrite(temp,1,ic,f);
		src+=ia;
	}
	delete[]temp;
}

int IMAGE::loadbmp(char*fname)
{
	bmpheader header;
	int k;
	FILE*f=fopen(fname,"rb+");
	if(!f)return 0;
	clear();
	fread(&header,1,54,f);
	width=header.width;
	height=header.height;
	bpp=header.bpp;
	sizepal=header.color;
	if(sizepal)
	{
		bpppal=4;
		pal=new uchar[sizepal*4];
		fseek(f,header.imgdata-(sizepal*4),SEEK_SET);
		fload_data(pal,4,4,sizepal,2,f);
	}
	k=imagesize();
	inddata=new  uchar[k];
	fseek(f,header.imgdata,SEEK_SET);
	switch(bpp)
	{
	case 32: fload_data(inddata,4,4,k/4,2,f); break;
	case 24: fload_data(inddata,3,3,k/3,1,f); break;
	default: fread(inddata,1,k,f);
	}
	fclose(f);
	return 1;
}

int IMAGE::savebmp(char*fname)
{
	bmpheader header;
	header.init();
	int k=imagesize();
	int retvalue=1;
	header.width=width;
	header.height=height;
	header.bpp=bpp;
	header.color=sizepal;
	header.imgdatasize=k;
	header.imgdata=sizeof(header)+sizepal*4;
	header.fsize=header.imgdata+header.imgdatasize;
	FILE*f;
	f=fopen(fname,"wb+");
	if(!f)return 0;
	fwrite(&header,1,54,f);
	if(sizepal)
	{
		fseek(f,header.imgdata-(sizepal*4),SEEK_SET);
		fsave_data(pal,4,4,sizepal,2,f);
	}
	fseek(f,header.imgdata,SEEK_SET);
	switch(bpp)
	{
	case 32:
		fsave_data(inddata,4,4,k/4,2,f);
		break;
	case 24:
		fsave_data(inddata,3,3,k/3,1,f);
		break;
	case 1:case 4:case 8:
		fwrite(inddata,1,k,f);
		break;
	default:retvalue=0;
	}
	fclose(f);
	if(!retvalue) {clear();return 0;}
	return 1;
}

int IMAGE::loadtga(char*fname)
{
	tgaheader header;
	int k;
	int retvalue=1;
	FILE*f=fopen(fname,"rb+");
	if(!f)return 0;
	clear();
	fread(&header,1,18,f);
	if(header.idlen)
		fseek(f,header.idlen,SEEK_CUR);
	width=header.width;
	height=header.height;
	bpp=header.bpp;
	if(header.typepal)
	{
		sizepal=header.colornum;
		bpppal=header.colorbpp;
		pal=new uchar[sizepal*4];
		fload_data(pal,bpppal/8,4,sizepal,
			       (bpppal==24)?1:2,f);
	}
	else sizepal=0;
	k=imagesize();
	inddata=new uchar[k];
	switch(bpp)
	{
	case 32:
		fload_data(inddata,4,4,k/4,2,f);
		break;
	case 24:
		fload_data(inddata,3,3,k/3,1,f);
		break;
	case 8:
		fread(inddata,1,k,f); break;
	default: retvalue=0;
	}
	fclose(f);
	if(!retvalue) {clear();return 0;}
	return 1;
}

int IMAGE::savetga(char*fname)
{
	tgaheader header;
	header.init();
	int k=imagesize();
	int retvalue=1;
	FILE*f=fopen(fname,"wb");
	if(!f) return 0;
	header.width=width;
	header.height=height;
	header.bpp=bpp;
	header.colornum=sizepal;
	if(sizepal)
	{
		header.typepal=1;
		header.typeimg=1;
		header.colorbpp=bpppal;
	} else header.typeimg=2;
	fwrite(&header,1,18,f);
	if(sizepal) fsave_data(pal,bpppal/8,4,sizepal,
		                   (bpppal==24)?1:2,f);
	switch(bpp)
	{
	case 32:
		fsave_data(inddata,4,4,k/4,2,f);
		break;
	case 24:
		fsave_data(inddata,3,3,k/3,1,f);
		break;
	case 1:case 4:case 8:
		fwrite(inddata,1,k,f);
		break;
	default: retvalue=0;
	}
	fclose(f);
	if(!retvalue) {clear(); return 0;}
	return 1;
}

void IMAGE::fromcolor(int xorg,int yorg)
{
	glReadBuffer(GL_BACK);
	switch(bpp)
	{
	case 1:
		glReadPixels(xorg,yorg,width,height,
			         GL_RGBA,GL_BITMAP,inddata);
		break;
	case 8:
		glReadPixels(xorg,yorg,width,height,
			         GL_COLOR_INDEX,GL_UNSIGNED_BYTE,inddata);
		break;
	case 24:
		glReadPixels(xorg,yorg,width,height,
			         GL_RGB,GL_UNSIGNED_BYTE,inddata);
		break;
	case 32:
		glReadPixels(xorg,yorg,width,height,
			         GL_RGBA,GL_UNSIGNED_BYTE,inddata);
		break;
	default:;
	}
}

int IMAGE::load2dtexture(char*str,uint* tex,
						 int tex_n,int level)
{
	string temp,fname,fext;
	temp=str;
	int npos,fpos,tex_i=0,ret=0;
	glGenTextures(tex_n, tex);
	while(npos>=0)
	{
		npos=temp.find(' ',0);
		fname=temp.substr(0,npos);
		fpos=fname.rfind('.');
		fext=fname.substr(fpos+1);
		if(fext=="bmp")
			if(!loadbmp((char*)fname.c_str()))
				return 0;
		if(fext=="tga")
			if(!loadtga((char*)fname.c_str()))
				return 0;
		temp.erase(0,npos+1);
		if(bpp>=24)
		{
			glBindTexture(GL_TEXTURE_2D,tex[tex_i++]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,(float)GL_REPLACE);
			glTexImage2D(GL_TEXTURE_2D,level,(bpp==24)?3:4,
				         width,height,0,(bpp==24)?GL_RGB:GL_RGBA,
						 GL_UNSIGNED_BYTE,inddata);
			ret++;
		}
	}
	return ret;
}

void IMAGE::tocolor()
{
	switch(bpp)
	{
	case 1:
		glBitmap(width,height,0,0,width,height,inddata);
		break;
	case 8:
		glDrawPixels(width,height,GL_COLOR_INDEX,
			         GL_UNSIGNED_BYTE,inddata);
		break;
	case 24:
		glDrawPixels(width,height,GL_RGB,
			         GL_UNSIGNED_BYTE,inddata);
		break;
	case 32:
		glDrawPixels(width,height,GL_RGBA,
			         GL_UNSIGNED_BYTE,inddata);
		break;
	default:;
	}
}

void IMAGE::glsetpal(int r,int g,int b,int a,
					 int start,int num)
{
	if(num==0)num=sizepal;
	float* temp=new float[num];
	if(r)
	{
		for(int i=start,j=0;i<num+start;i++)
			temp[j++]=(float)pal[i*4]/255.0;
		glPixelMapfv(GL_PIXEL_MAP_I_TO_R,num,temp);
	}
	if(g)
	{
		for(int i=start,j=0;i<num+start;i++)
			temp[j++]=(float)pal[i*4+1]/255.0;
		glPixelMapfv(GL_PIXEL_MAP_I_TO_G,num,temp);
	}
	if(b)
	{
		for(int i=start,j=0;i<num+start;i++)
			temp[j++]=(float)pal[i*4+2]/255.0;
		glPixelMapfv(GL_PIXEL_MAP_I_TO_B,num,temp);
	}
	if(a)
	{
		for(int i=start,j=0;i<num+start;i++)
			temp[j++]=(float)pal[i*4+3]/255.0;
		glPixelMapfv(GL_PIXEL_MAP_I_TO_A,num,temp);
	}
	delete[]temp;
}

void IMAGE::tostencil()
{
	switch(bpp)
	{
	case 1:
		glDrawPixels(width,height,GL_STENCIL_INDEX,GL_BITMAP,inddata);
		break;
	case 8:
		glDrawPixels(width,height,GL_STENCIL_INDEX,GL_UNSIGNED_BYTE,inddata);
		break;
	case 16:
		glDrawPixels(width,height,GL_STENCIL_INDEX,GL_UNSIGNED_SHORT,inddata);
		break;
	case 32:
		glDrawPixels(width,height,GL_STENCIL_INDEX,GL_UNSIGNED_INT,inddata);
		break;
	default:;
	}
}

#endif