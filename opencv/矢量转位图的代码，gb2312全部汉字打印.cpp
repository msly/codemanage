

#include <default.h>
#include "ft2build.h"

#include FT_FREETYPE_H
#include FT_IMAGE_H
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

#define _FLOOR(x)  ((x) & -64)
#define _CEIL(x)   (((x)+63) & -64)
#define _TRUNC(x)  ((x) >> 6)

FT_Library ftLibrary = NULL;  



struct glyphInfo{
	float	x1,x2,y1,y2;	//预计算轮廓偏移值
	float	u1,u2,v1,v2;	//贴图坐标
	float	xSkip,ySkip;	//字间距 ,横/纵
};





//英文可打印字符
#define	IDX_SIZE_EN		('~'-' '+1)
//符号只支持高位a1-a2的
#define	IDX_SIZE_SYM	((0xa2-0xa1+1)*(0xfe-0xa1+1))
#define	IDX_SIZE_GB		((0xf7-0xb0+1)*(0xfe-0xa1+1))

#define	between(a,b,c)	((a>=b)&&(a<=c))
DWORD	inline	glyphIndex(pcsz_t& chr){
	pcb_t	&p=(pcb_t&)chr;
	WORD	h=*p;
	++p;
	if(h<' '){
		//英文不可打印字符，当做空格处理
		return	0;
	}elif(h<='~'){
		return	h-' ';
	}else{//中文或者符号
		WORD	l=*p;

		if(between(l,0xa1,0xfe)){
			if(between(h,0xa1,0xa2)){
				//符号
				++p;
				return	IDX_SIZE_EN+(h-0xa1)*(0xfe-0xa1+1)+l-0xa1;
			}elif(between(h,0xb0,0xf7)){
				++p;
				return	IDX_SIZE_EN+IDX_SIZE_SYM+(h-0xb0)*(0xfe-0xa1+1)+l-0xa1;
			}
		}

		//不是gb2312及支持的符号，作为不可打印字处理

		return	0;

	}
		
	
}



WORD	GB2312ToUnicode(WORD code){
	wchar_t	out;
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)&code,2,&out,1);
	return	out;
}


BITMAPFILEHEADER	bmpFileHeader;
BITMAPINFOHEADER	bmpHeader;
RGBQUAD				grayColors[256];


byte	*data=NULL;

void	saveData(pcsz_t f,int type,int width,int height){
	bmpHeader.biHeight=height;
	bmpHeader.biWidth=width;
	bmpHeader.biSizeImage=width*height;

	char	fn[1024];


	_snprintf(fn,1024,"fonts\\%s%d.bmp",f,type);

	FILE*	fp=fopen(fn,"wb");
	fwrite(&bmpFileHeader,1,sizeof(bmpFileHeader),fp);
	fwrite(&bmpHeader,1,sizeof(bmpHeader),fp);
	fwrite(grayColors,1,sizeof(grayColors),fp);
	for(int i=height-1;i>=0;--i)
		fwrite(data+i*width,1,width,fp);
	fclose(fp);


}

byte	bmp17[17][17];
byte	bmp16[16][16];
byte	bmp20[20][20];
byte	bmp24[24][24];
byte	bmp32[32][32];
byte	bmp48[48][48];


bool	RenderGlyph(FT_Face &face,FT_Bitmap&bitmap, glyphInfo* font,int point ,int width,int height,int &pos_x,int&pos_y,int& page,int& cnt,int fnt_idx,WORD code){
	if(0xffff==fnt_idx)
	{
		printf ("test failed\n");
		return false;
	}
	
		if(fnt_idx!=cnt){
			printf("idx missmatch %d %d",fnt_idx,cnt);
			return false;
		}


		WORD	unicode=GB2312ToUnicode(code);

		if(FT_Load_Char(face,unicode , /*FT_LOAD_TARGET_LIGHT|*/FT_LOAD_NO_BITMAP)){
			printf("load char failed\n");
			return false;
		}

		if(face->glyph->format!=FT_GLYPH_FORMAT_OUTLINE){
			printf("why not outline format?!!!\n");
			return false;
		}

		//if(FT_Load_Char(face,unicode , FT_LOAD_DEFAULT  )){
		//	printf("load char failed\n");
		//	return;
		//}

		++cnt;

		if(0)
			printf(	
			"	w:%d ha:%d hx:%d hy:%d\n"
			"	h:%d va:%d vx:%d vy %d\n"
			,
			face->glyph->metrics.width,
			face->glyph->metrics.horiAdvance,
			face->glyph->metrics.horiBearingX,
			face->glyph->metrics.horiBearingY,
			face->glyph->metrics.height,
			face->glyph->metrics.vertAdvance,
			face->glyph->metrics.vertBearingX,
			face->glyph->metrics.vertBearingY
			);
		//	return;
rebuild_:
		if(		(pos_x==-1) ||((pos_x+point)>width) )
		{
			pos_x=0;
			if(pos_y!=-1)
				pos_y+=point;
			else
				pos_y=0;

			if((pos_y +point)>height){
				printf("bitmap overflow...\n",pos_y);
				return	false;
//				saveData(fn,point,width,height);
				memset(data,0,width*height);
				pos_x=pos_y=-1;
				goto	rebuild_;
			}

		}

		memset(bitmap.buffer,0,bitmap.pitch*bitmap.rows);

		FT_Glyph glyph;
		if(FT_Get_Glyph( face->glyph, &glyph )){
			printf("get_glyph failed\n");
			return false;
		}




		//把包围合对齐到base_line


		int	left,right,top,bottom,w,h;
		left  = ( face->glyph->metrics.horiBearingX );
		right = ( face->glyph->metrics.horiBearingX + face->glyph->metrics.width );

		top    = ( face->glyph->metrics.horiBearingY );
		bottom = ( face->glyph->metrics.horiBearingY - face->glyph->metrics.height );

		w=face->glyph->metrics.width /64;
		h=face->glyph->metrics.height/64;

		if(w>point){
			//printf("%d %d\n",face->glyph->metrics.width);
			w=point;
		}
		if(h>point){
			//printf("%d %d\n",h,point);
			h=point;

		}

		FT_Outline_Translate( &face->glyph->outline,-left,64*point-top);



		//FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_LIGHT, 0, 1 );
		//FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		//// 保存位图
		//FT_Bitmap& bitmap=bitmap_glyph->bitmap;


		if(FT_Outline_Get_Bitmap( ftLibrary, &face->glyph->outline, &bitmap)){
			printf("outline to bmp failed\n");
			return false;
		}

		for(int k=0;k<bitmap.rows;++k){
			memcpy(data+(pos_y+k)*width+pos_x,bitmap.buffer+k*bitmap.pitch,bitmap.pitch);
		}

		glyphInfo	&fnt=font[fnt_idx];

		fnt.x1=left/64.0;
		fnt.x2=right/64.0;
		fnt.y1=-top/64.0;
		fnt.y2=-bottom/64.0;


		fnt.v1=((float)pos_y)/height;//+0.5/height;
		fnt.v2=(float)(pos_y+h)/height;//-0.5/height;
		fnt.u1=((float)pos_x)/width;//+0.5/width;
		fnt.u2=(float)(pos_x+w)/width;//-0.5/width;

		fnt.xSkip=face->glyph->metrics.horiAdvance/64.0 ;
		fnt.ySkip=face->glyph->metrics.vertAdvance/64.0 ;





		pos_x+=point;
		return	true;

}

void	createFont(const char* fnt,const char* fn,int point,int width,int height)
{
	if(data){
		delete	data;
	}
	data=new	byte[width*height];
	memset(data,0,width*height);

	int	page=0;
	//FT_Library ftLibrary = NULL;
	if (FT_Init_FreeType( &ftLibrary )) {
		printf("R_InitFreeType: Unable to initialize FreeType.\n");
		return;
	}
	FT_Face	face;
	if(FT_New_Face(ftLibrary,fnt,0,&face)){
		printf("New face failed\n");
		return;
	}
	FT_Select_Charmap(face,FT_ENCODING_UNICODE);
	if(FT_Set_Pixel_Sizes(face,point,point)){
		printf("set pixel size failed");
	}
	FT_Bitmap	bitmap;
	bitmap.pitch=point;
	bitmap.width=point;
	bitmap.rows=point;
	bitmap.pixel_mode=FT_PIXEL_MODE_GRAY;
	bitmap.num_grays=256;

	switch(point){
case	17:
	bitmap.buffer=(byte*)bmp17;
	break;
case	16:
	bitmap.buffer=(byte*)bmp16;
	break;
case	20:
	bitmap.buffer=(byte*)bmp20;
	break;
case	24:
	bitmap.buffer=(byte*)bmp24;
	break;
case	32:
	bitmap.buffer=(byte*)bmp32;
	break;
case	48:
	bitmap.buffer=(byte*)bmp48;
	break;

default:
	printf("point size %d not support\n",point);
	return;
	}


	//	printf("face :x1 %d x2 %d y1 %d y2 %d \nw %d h %d\n",face->bbox.xMin,face->bbox.xMax,face->bbox.yMin,face->bbox.yMax,face->max_advance_width,face->max_advance_height);
	int	pos_x=-1;
	int	pos_y=-1;
	glyphInfo	font_info[IDX_SIZE_EN+IDX_SIZE_GB+IDX_SIZE_SYM];

	int	cnt=0;

	WORD	fnt_idx;
	pcsz_t	pChr;

	for(WORD	en=' ';en<='~';++en){
		pChr=(pcsz_t)&en;
		fnt_idx=glyphIndex(pChr);
		//printf("%c %d %d\n",en,fnt_idx,cnt);
		if(!RenderGlyph(face,bitmap,font_info,point,width,height,pos_x,pos_y,page,cnt,fnt_idx,en))
			break;
	}

	for(int j=0xa1;j<=0xa2;++j)
		for(int i=0xa1;i<0xff;++i){
			WORD	sym=(i<<8)|j;
			pChr=(pcsz_t)&sym;
			fnt_idx=glyphIndex(pChr);

			if(!RenderGlyph(face,bitmap,font_info,point,width,height,pos_x,pos_y,page,cnt,fnt_idx,sym))
				break;
		}

	for(int j=0xb0;j<0xf8;++j)
		for(int i=0xa1;i<0xff;++i){
			WORD	gb=(i<<8)|j;
			fnt_idx=cnt;
			
			if(!RenderGlyph(face,bitmap,font_info,point,width,height,pos_x,pos_y,page,cnt,fnt_idx,gb))
				break;
		}

//-------------------------
		byte	max=0;
		for(int i=0;i<width*height;++i){
			if(data[i]>max)
				max=data[i];
		}


		printf("max =%02x\n",max);


		saveData(fn,point,width,height);
		char	gbname[1024];
		_snprintf(gbname,1024,"fonts\\%s%d.fnt",fn,point);
		FILE*	fp=fopen(gbname,"wb");
		fwrite(font_info,1,sizeof(font_info),fp);
		fwrite(data,width,height,fp);

		fclose(fp);

		FT_Done_FreeType(ftLibrary);
		printf("%d save size=%d\n",cnt,IDX_SIZE_SYM+IDX_SIZE_GB+IDX_SIZE_EN);

}


void	main(){


	int	size=1024;
	for(int i=0;i<256;++i){
		grayColors[i].rgbBlue=grayColors[i].rgbGreen=grayColors[i].rgbRed=i;
		grayColors[i].rgbReserved=0;
	};
	bmpFileHeader.bfType='MB';
	bmpFileHeader.bfReserved1=0;
	bmpFileHeader.bfReserved2=0;
	bmpFileHeader.bfSize=sizeof(bmpFileHeader)+sizeof(bmpHeader)+sizeof(grayColors)+sizeof(data);
	bmpFileHeader.bfOffBits=sizeof(bmpFileHeader)+sizeof(bmpHeader)+sizeof(grayColors);

	bmpHeader.biBitCount=8;
	bmpHeader.biClrImportant=256;
	bmpHeader.biClrUsed=256;
	bmpHeader.biCompression=0;
	bmpHeader.biPlanes=1;
	bmpHeader.biSize=sizeof(bmpHeader);

	bmpHeader.biXPelsPerMeter=0;
	bmpHeader.biYPelsPerMeter=0;



	pcsz_t	fnt[]={
		"c:\\windows\\fonts\\stsong.ttf",
		"c:\\windows\\fonts\\stxihei.ttf",
		"c:\\windows\\fonts\\stkaiti.ttf",
		"c:\\windows\\fonts\\simyou.ttf",

	};
	pcsz_t	fn[]={"songti","xihei","kaiti","youyuan"};

	for(int i=0;i<4;++i){
		createFont(fnt[i],fn[i],16,1024,2048);
		createFont(fnt[i],fn[i],32,4096,2048);
		createFont(fnt[i],fn[i],24,2048,2048);
		createFont(fnt[i],fn[i],48,4096,4096);

	}




	//createFnt(,32);
	//createFnt("fnt2.ttf",32);


}

