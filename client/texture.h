#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL2/SDL_image.h>

#include <string>

#include "drawable.h"
#include "sdl_exception.h"

class Texture : public Drawable {
 private:
  SDL_Texture* texture;

 public:
  Texture();

  ~Texture();

  // Renderiza la textura en un x e y concreto
  virtual void render(SDL_Renderer*, SDL_Rect*, int, int);

  void loadTexture(std::string, SDL_Renderer*);

  void free();

    SDL_Texture* getTexture();

  virtual int getWidth() const;

  virtual int getHeight() const;

  virtual int getX() const;

  virtual int getY() const;

  virtual void setX(int);

  virtual void setY(int);
};

#endif
