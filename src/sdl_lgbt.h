/*
LGBT: Light Graphics Binary Text

This file is one implementation of a new idea involving a rewrite of the font library previously used in my video games: Chaste Tris, Chaste Puyo, Chaste Panel.

The idea is a small self contained example of loading an image as a font file and then drawing to the screen based on fast blits of the image. But this library is not just a copy but includes a few additional utility functions for converting to or from a new format called LGBT.

And yes, the name is a joke because LGBT also stands for Lesbian, Gay, Bisexual, and Transgender.
*/


/*
texture font structure
*/
struct chaste_font
{
 int char_width; /*width of a char*/
 int char_height; /*height of a char*/
 SDL_Surface *surface; /*the surface of the image of loaded font*/
 SDL_Texture *texture; /*the texture of this font which will be loaded from surface*/
};



/*global fonts that will be reused many times*/
struct chaste_font main_font,font_8,font_16,font_32,font_64,font_128,font_pico8;

/*function to load a font and return a structure with the needed data to draw later*/
struct chaste_font chaste_font_load(const char *s)
{
 int w,h;
 struct chaste_font new_font;
 SDL_Surface *temp_surface;
 printf("Loading font: %s\n",s);

 /*load bitmap to temporary surface*/
 temp_surface=SDL_LoadBMP(s);

 /*convert to BGRA32 pixel format for easy handling for later functions*/
 new_font.surface=SDL_ConvertSurfaceFormat(temp_surface, SDL_PIXELFORMAT_BGRA32, 0);
 SDL_FreeSurface(temp_surface);

 if(new_font.surface==NULL){printf( "SDL could not load image! SDL_Error: %s\n",SDL_GetError());return new_font;}
 new_font.texture=SDL_CreateTextureFromSurface(renderer,new_font.surface);
 if(new_font.texture==NULL){printf( "SDL could not load texture from surface! SDL_Error: %s\n",SDL_GetError());return new_font;}
 else
 {
  SDL_QueryTexture(new_font.texture,NULL,NULL,&w,&h);
  printf("texture width==%d height==%d\n",w,h);
 }
 
 /*
  by default,font height is detected by original image height
  but the font width is the width of the image divided by 95
  because there are exactly 95 characters in the font format that I created.
 */
 new_font.char_width=new_font.surface->w/95; /*there are 95 characters in my font files*/
 new_font.char_height=new_font.surface->h;

 if(new_font.char_height==0)
 {
  printf("Something went horribly wrong loading the font from file:\n%s\n",s);
 }
 else
 {
  /*printf("Font loaded correctly\n");*/
  printf("Size of loaded font is %d,%d\n",new_font.char_width,new_font.char_height);
 }

 return new_font;
}


/*
 this function successfully draws a string of characters from the loaded font
 arguments are: character pointer, character x pos, character y pos
*/
void chaste_font_draw_string(char *s,int cx,int cy)
{
 int x,y,i,c,cx_start=cx;
 SDL_Rect rect_source,rect_dest;
 i=0;
 while(s[i]!=0)
 {
  c=s[i];
  if(c=='\n'){ cx=cx_start; cy+=main_font.char_height;}
  else
  {
   x=(c-' ')*main_font.char_width;
   y=0*main_font.char_height;

   rect_source.x=x;
   rect_source.y=y;
   rect_source.w=main_font.char_width;
   rect_source.h=main_font.char_height;

   rect_dest=rect_source;
   rect_dest.x=cx;
   rect_dest.y=cy;

   if(SDL_RenderCopy(renderer,main_font.texture,&rect_source,&rect_dest))
   {
    printf( "RenderCopy Error SDL_Error: %s\n",SDL_GetError());
   }
   cx+=main_font.char_width;
  }
  i++;
 }
}





/*
Draw a string with characters multiplied by an additional scale value passed to this function
*/
void chaste_font_draw_string_scaled(char *s,int cx,int cy,int scale)
{
 int x,y,i,c,cx_start=cx;
 SDL_Rect rect_source,rect_dest;
 i=0;
 while(s[i]!=0)
 {
  c=s[i];
  if(c=='\n'){ cx=cx_start; cy+=main_font.char_height*scale;}
  else
  {
   x=(c-' ')*main_font.char_width;
   y=0*main_font.char_height;

   rect_source.x=x;
   rect_source.y=y;
   rect_source.w=main_font.char_width;
   rect_source.h=main_font.char_height;

   rect_dest=rect_source;
   rect_dest.x=cx;
   rect_dest.y=cy;
   rect_dest.w=main_font.char_width*scale;
   rect_dest.h=main_font.char_height*scale;

   if(SDL_RenderCopy(renderer,main_font.texture,&rect_source,&rect_dest))
   {
    printf( "RenderCopy Error SDL_Error: %s\n",SDL_GetError());
   }
   cx+=main_font.char_width*scale;
  }
  i++;
 }
}


void chaste_font_free(struct chaste_font font)
{
 SDL_FreeSurface(font.surface);
 SDL_DestroyTexture(font.texture);
}



int text_x; /*the x position of where text will go*/


/*
 this function is now the official demo function for the LGBT library
 it shows text based on the loaded main font
*/
void lgbt_demo()
{
 int scale=8;
 main_font=font_8;
 text_x=width*1/6;

 delay=1000/fps;

 loop=1;
 while(loop)
 {
  sdl_time = SDL_GetTicks();
  sdl_time1 = sdl_time+delay;

  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);

  scale=16;
  chaste_font_draw_string_scaled("LGBT",text_x,main_font.char_height*1*scale,scale);

  scale=8;
  chaste_font_draw_string_scaled("Light\nGraphics\nBinary\nText",text_x,main_font.char_height*5*scale,scale);

  chaste_font_draw_string("This text was drawn with a program written by Chastity White Rose\nSimilar methods were used in her games Chaste Tris, Chaste Puyo, and Chaste Panel",text_x,main_font.char_height*10*scale);
 
  SDL_RenderPresent(renderer);

  /*time loop used to slow the game down so users can see it*/
  while(sdl_time<sdl_time1)
  {
   sdl_time=SDL_GetTicks();
  }

  /*test for events and only process if they exist*/
  while(SDL_PollEvent(&e))
  {
   if(e.type == SDL_QUIT){loop=0;}
   if(e.type == SDL_KEYUP)
   {
    if(e.key.keysym.sym==SDLK_ESCAPE){loop=0;}
   }
  }
  
 }
}


