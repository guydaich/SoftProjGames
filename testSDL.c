
#include "SDL.h"
#include "SDL_ttf.h"

int main( int argc, char* args[] )
{
  /*  //The images
    SDL_Surface* hello = NULL;
    SDL_Surface* screen = NULL;
	SDL_Init( SDL_INIT_EVERYTHING );

    //Set up screen
    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );

    //Load image
    hello = SDL_LoadBMP( "hello.bmp" );
	  //Apply image to screen
    SDL_BlitSurface( hello, NULL, screen, NULL );

    //Update Screen
    SDL_Flip( screen );

    //Pause
    SDL_Delay( 2000 );
	 //Free the loaded image
    SDL_FreeSurface( hello );

    //Quit SDL
    SDL_Quit();
*/

/*	TTF_Font *font;
	SDL_Surface* display;
	SDL_Surface *text;
	SDL_Event event;
   SDL_Color text_color = {255, 255, 255};
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
   {
      exit(1);
   }

   // Set the video mode
   display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
   if (display == NULL)
   {    
      exit(1);
   }

   // Set the title bar
   SDL_WM_SetCaption("SDL Tutorial", "SDL Tutorial");

   // Initialize SDL_ttf library
   if (TTF_Init() != 0)
   {
      SDL_Quit();
      exit(1);
   }

   // Load a font
   font = TTF_OpenFont("FreeSans.ttf", 24);
   if (font == NULL)
   {
      TTF_Quit();
      SDL_Quit();
      exit(1);
   }

   // Write text to surface
   text = TTF_RenderText_Solid(font,
   "A journey of a thousand miles begins with a single step.",
   text_color);

   if (text == NULL)
   {
      TTF_Quit();
      SDL_Quit();
      exit(1);
   }

   // Main loop
   while(1)
   {
      // Check for messages
      if (SDL_PollEvent(&event))
      {
         // Check for the quit message
         if (event.type == SDL_QUIT)
         {
            // Quit the program
            break;
         }
      }

      // Clear the screen
      if (SDL_FillRect(display, 
                       NULL,
                       SDL_MapRGB( display->format, 0,0,0))
                       != 0)
      {
         break;
      }

      // Apply the text to the display
      if (SDL_BlitSurface(text, NULL, display, NULL) != 0)
      {
         break;
      }

        //Update the display
      SDL_Flip(display);

   }

   // Shutdown the TTF library
   TTF_Quit();

   // Tell the SDL to clean up and shut down
   SDL_Quit();
    
   return 0;    
   */

}