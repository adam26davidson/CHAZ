#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Texture
{
	public:
		Texture();
		~Texture();
		bool loadFromFile(string path);
		void free();
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void render(int x, int y, SDL_Rect* clip = NULL);
		int getWidth();
		int getHeight();
	private:
		SDL_Texture texture_;
		int width_;
		int height_;
};

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int RING_1_FRAMES = 10;
const int RING_2_FRAMES = 30;
const int RING_3_FRAMES = 4;

Texture gSpriteTexturesRing1[RING_1_FRAMES];
Texture gSpriteTexturesRing2[RING_2_FRAMES];
Texture gSpriteTexturesRing3[RING_3_FRAMES];

Texture::Texture() {
	texture_ = NULL;
	width_ = 0;
	height_ = 0;
}

Texture::~Texture() {
	free();
}

bool Texture::loadFromFile(string path) {

	free();
	
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {

		printf("unable to load image %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());

	} else {

		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

		if (newTexture == NULL) {

			printf("unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());

		} else {

			width_ = loadedSurface->w;
			height_ = loadedSurface->h;

		}

		SDL_FreeSurface(loadedSurface);

	}
	
	texture_ = newTexture();
	return texture != NULL;
}

void Texture::free() {
	

	if (texture_ != NULL) {
		
		SDL_DestroyTexture(texture_);
		texture_ = NULL;
		width_ = 0;
		height_ = 0;

	}

}
		
void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( texture_, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( texture_, blending );
}

void Texture::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( texture_, alpha );
}

void Texture::render( int x, int y, SDL_Rect* clip )
{

	SDL_Rect renderQuad = { x, y, width_, height_ };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy( gRenderer, texture_, clip, &renderQuad );
}

int Texture::getWidth()
{
	return width_;
}

int Texture::getHeight()
{
	return height_;
}

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled" );
		}

		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL )
		{
			printf( "Window could not be created SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;
	
	for (int i = 0; i < RING_1_FRAMES; i++) {
		if( !gSpriteTexturesRing1[i].loadFromFile("CHAZ/Sprites/ring_1_frames/ring_1_" + to_string(i) + ".png"))
		{
			printf( "Failed to load ring 1 texture\n" );
			success = false;
		} 
	}
	for (int i = 0; i < RING_2_FRAMES; i++) {
		if( !gSpriteTexturesRing2[i].loadFromFile("CHAZ/Sprites/ring_2_frames/ring_2_" + to_string(i) + ".png"))
		{
			printf( "Failed to load ring 2 texture\n" );
			success = false;
		} 
	}
	for (int i = 0; i < RING_3_FRAMES; i++) {
		if( !gSpriteTexturesRing3[i].loadFromFile("CHAZ/Sprites/ring_3_frames/ring_3_" + to_string(i) + ".png"))
		{
			printf( "Failed to load ring 3 texture\n" );
			success = false;
		} 
	}
	
	return success;
}

void close()
{

	for (int i = 0; i < RING_1_FRAMES; i++) {
		gSpriteTexturesRing1[i].free();
	}
	for (int i = 0; i < RING_2_FRAMES; i++) {
		gSpriteTexturesRing2[i].free();
	}
	for (int i = 0; i < RING_3_FRAMES; i++) {
		gSpriteTexturesRing3[i].free();
	}
	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}
	
int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;

			SDL_Event e;

			//Current animation frame
			int frame = 0;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current frame
				SDL_Rect space = {0, 0, gSpriteTexturesRing1[frame / 4].getWidth(), gSpriteTexturesRing1[frame / 4].getHeight()};
				gSpriteTexturesRing1[frame / 4].render(SCREEN_WIDTH - gSpriteTexturesRing1[frame / 4].getWidth()) / 2,SCREEN_HEIGHT - gSpriteTexturesRing1[frame / 4].geHeight()) / 2,  );

				//Update screen
				SDL_RenderPresent( gRenderer );

				//Go to next frame
				++frame;

				//Cycle animation
				if( frame / 4 >= RING_1_FRAMES )
				{
					frame = 0;
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}			
		
	