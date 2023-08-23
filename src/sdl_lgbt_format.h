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
void lgbt_load_bmp(const char *s)
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
    new_lgbt.pixels[x+y*new_lgbt.width]=ssp[x+y*new_lgbt.width];
    x++;
   }
   y++;
  }
  
 }




 SDL_FreeSurface(temp_surface1);
}

