#include "texture.h"

Texture::Texture() {
  texture = NULL;
  x = 0;
  y = 0;
  width = 0;
  height = 0;
}

Texture::~Texture() { free(); }

void Texture::loadTexture(std::string path_file, SDL_Renderer* renderer) {
    // Libera la textura si ya tenia algo cargado
    free();

    // La textura final
    SDL_Texture* new_texture = NULL;

    SDL_Surface* new_surface = IMG_Load(path_file.c_str());
    // texture = IMG_LoadTexture(renderer, filename.c_str());

    if (!new_surface)
        throw SdlException("Error al cargar la textura", SDL_GetError());
    else {
        // Color key image
        SDL_SetColorKey(new_surface, SDL_TRUE,
                        SDL_MapRGB(new_surface->format, 0, 0, 0));

        // Create texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface(renderer, new_surface);
        if (!new_texture) {
            throw SdlException("Error al cargar la textura", SDL_GetError());
        } else {
            // Cargamos las dimensiones de la imagen
            width = new_surface->w;
            height = new_surface->h;
        }
        // Liberamos la surface
        SDL_FreeSurface(new_surface);
    }
    // Liberamos la surface
    SDL_FreeSurface(new_surface);
  }
  texture = new_texture;
}

void Texture::render(SDL_Renderer* renderer, SDL_Rect* clip, int x_dest,
                     int y_dest) {
  SDL_Rect quad_dest = {x_dest, y_dest, width, height};
  // Checkeamos si queremos renderizar toda la textura o una porción

<<<<<<< HEAD
    if (clip != NULL) {
        quad_dest.h = clip->h;
        quad_dest.w = clip->w;
    }

    if (SDL_RenderCopy(renderer, texture, clip, &quad_dest))
        throw SdlException("Error al renderizar la textura", SDL_GetError());
}

void Texture::free() {
    // Free texture if it exists
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
        x = 0;
        y = 0;
    }
=======
  if (clip != NULL) {
    quad_dest.h = clip->h;
    quad_dest.w = clip->w;
  }

  if (SDL_RenderCopy(renderer, texture, clip, &quad_dest))
    throw SdlException("Error al renderizar la textura", SDL_GetError());
}

void Texture::free() {
  // Free texture if it exists
  if (texture != NULL) {
    SDL_DestroyTexture(texture);
    texture = NULL;
    width = 0;
    height = 0;
    x = 0;
    y = 0;
  }
>>>>>>> master
}

SDL_Texture* Texture::getTexture() { return texture; }

int Texture::getWidth(void) const { return width; }

int Texture::getHeight(void) const { return height; }

int Texture::getX(void) const { return x; }

int Texture::getY(void) const { return y; }

void Texture::setX(int new_x) { x = new_x; }

void Texture::setY(int new_y) { y = new_y; }
