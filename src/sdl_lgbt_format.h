/*
To avoid corrupting my font library during development, I created this source file to make a set of functions entirely dedicated to my new file format.
*/


struct lgbt
{
 Uint32 width,height,bpp;
 Uint32 *pixels;
};

struct lgbt main_lgbt;


/*loads data into an lgbt image by first loading a bitmap file and then copying all the data over*/
struct lgbt lgbt_load_bmp(const char *s)
{
 int x,y;
 SDL_Surface *temp_surface,*temp_surface1;
 Uint32 *ssp; /*ssp is short for Source Surface Pointer*/
 struct lgbt new_lgbt;

 /*load bitmap to temporary surface*/
 temp_surface=SDL_LoadBMP(s);

 /*convert to BGRA32 pixel format for easy handling for later functions*/
 temp_surface1=SDL_ConvertSurfaceFormat(temp_surface, SDL_PIXELFORMAT_BGRA32, 0);
 SDL_FreeSurface(temp_surface);



 /*set the initial data for the lgbt structure copied from the SDL_Surface*/
 new_lgbt.width=temp_surface1->w;
 new_lgbt.height=temp_surface1->h;
 new_lgbt.bpp=32;

 printf("new_lgbt width=%d height=%d bpp=%d\n",new_lgbt.width,new_lgbt.height,new_lgbt.bpp);

 new_lgbt.pixels=(uint32_t*)malloc((new_lgbt.width*new_lgbt.height)*sizeof(*new_lgbt.pixels));
 if(new_lgbt.pixels==NULL){printf("Error: malloc failed,\n");}
 else
 {
  printf("Allocated the pixels for lgbt image.\n");

  ssp=(Uint32*)main_font.surface->pixels;

  /*copy the pixels from the surface to this structure*/
  y=0;
  while(y<new_lgbt.height)
  {
   x=0;
   while(x<new_lgbt.width)
   {
    new_lgbt.pixels[x+y*new_lgbt.width]=ssp[x+y*new_lgbt.width] & 0xFFFFFF;
    /*printf("%06X %d,%d\n",new_lgbt.pixels[x+y*new_lgbt.width],x,y);*/
    x++;
   }
   y++;
  }
  
 }

 SDL_FreeSurface(temp_surface1);

 return new_lgbt;
}










/*
Saves to PBM(Portable Bitmap Format). This format takes less space than Windows BMP files because the header is a lot smaller but the pixel data is stored as 1 bit per pixel. It doesn't get smaller than that without compression.
*/
void lgbt_save_pbm(const char* filename)
{
 uint32_t x,y,pixel,r,g,b,gray,bitcount,bits,bpp=1;

 FILE* fp;
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return;}
 else{/*printf("File \"%s\" opened.\n",filename);*/}
 fprintf(fp,"P4\n"); fprintf(fp,"%d %d\n",main_lgbt.width,main_lgbt.height);

 /*start of pixel writing section*/
 y=0;
 while(y<main_lgbt.height)
 {
  bitcount=0;
  bits=0;
  x=0;
  while(x<main_lgbt.width)
  {
   pixel=main_lgbt.pixels[x+y*main_lgbt.width];
   r=(pixel&0xFF0000)>>16;
   g=(pixel&0x00FF00)>>8;
   b=(pixel&0x0000FF);
   gray=(r+g+b)/3;
   gray>>=8-bpp; gray^=1;
   bits<<=bpp;
   bits|=gray;
   bitcount+=bpp;
   x++;
   while(bitcount>=8)
   {
    fputc(bits,fp);
    bitcount-=8;
   }
  }

  /*If width is not a multiple of 8 pad the bits to a full byte*/
  while(bitcount!=0)
  {
   bits<<=1;
   bitcount++;
   if(bitcount==8)
   {
    fputc(bits,fp);
    bitcount=0;
   }
  }
  y++;
 }

 /*end of pixel writing section*/


 fclose(fp);
 printf("Saved to file: %s\n",filename);
}











/*
A quick dirty replacement for never needing to use fwrite to write my integers! However the catch is that it only works for little endian. For this reason I choose little endian as the format the binary integers will take in my own image formats I am creating. It writes integer i to file pointer fp using count bytes. Most PCs these days can't actually do more than 32 or 64 bits.

This function was necessary so that my code won't completely fail if I do programming on a different CPU in which case fwrite would spit out the big endian byte order. Besides it wasn't too hard to write and requires only 3 arguments instead of 4.
*/
void fputint(unsigned long int i,FILE *fp,int count)
{
 while(count>0)
 {
  fputc(i,fp);
  i>>=8;
  count--;
 }
}








/*
Saves to my own format which is named LGBT.
*/
void lgbt_save(const char* filename)
{
 uint32_t x,y,pixel,r,g,b,gray,bitcount,bits,bpp=1;

 FILE* fp;
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return;}
 else{/*printf("File \"%s\" opened.\n",filename);*/}

 fputs("LGBT",fp);
 fputint(main_lgbt.width,fp,4);
 fputint(main_lgbt.height,fp,4);
 fputint(bpp,fp,4);

 /*start of pixel writing section*/
 y=0;
 while(y<main_lgbt.height)
 {
  bitcount=0;
  bits=0;
  x=0;
  while(x<main_lgbt.width)
  {
   pixel=main_lgbt.pixels[x+y*main_lgbt.width];
   r=(pixel&0xFF0000)>>16;
   g=(pixel&0x00FF00)>>8;
   b=(pixel&0x0000FF);
   gray=(r+g+b)/3;
   gray>>=8-bpp; /*gray^=1;*/
   bits<<=bpp;
   bits|=gray;
   bitcount+=bpp;
   x++;
   while(bitcount>=8)
   {
    fputc(bits,fp);
    bitcount-=8;
   }
  }

  /*If width is not a multiple of 8 pad the bits to a full byte*/
  while(bitcount!=0)
  {
   bits<<=1;
   bitcount++;
   if(bitcount==8)
   {
    fputc(bits,fp);
    bitcount=0;
   }
  }
  y++;
 }

 /*end of pixel writing section*/


 fclose(fp);
 printf("Saved to file: %s\n",filename);
}

