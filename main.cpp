#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
// Changes...


const int width = 1000;
const int height = 700;


class Main
{
  public:
    bool init();
    bool loadImages();
    bool loadSounds();
    void spawnSomething();
    void gameloop();
    void close();
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Surface* player = NULL;
    SDL_Surface* something = NULL;
    SDL_Surface* _something = NULL;
    SDL_Surface* pewpew = NULL;
    SDL_Surface* gift = NULL;
    SDL_Surface* gameOver = NULL;
    SDL_Surface* yay = NULL;
    SDL_Event e;
    SDL_Rect pRect; // dst Rects
    SDL_Rect pSRect; // src Rects
    SDL_Rect pewRect;
    SDL_Rect tmpRect;
    SDL_Rect somethingRect1, somethingRect2, somethingRect3;
    SDL_Rect giftRect;
    Mix_Chunk * hit;
    Mix_Chunk * step;
    Mix_Chunk * shoot;
    bool quit = false;
    bool aPressed = false;
    bool dPressed = false;
    bool wPressed = false;
    bool sPressed = false;
    bool charging = false;
    bool pew = false;
    int mx;
    int my;
    int somethingTimer = (rand() % 1001) + 1000;
    double speed = 0.0;
    double initspeed = 5.0;
    int animationcycle = 0;
    int charge = 0;
    double angle[2];
    double dx;
    double dy;
    int hspeed;
    int vspeed;
    Main()
    {
      giftRect.x = rand() % width - 100;
      giftRect.y = ((height / 4) * 3) - 30;
      somethingRect1.x = width + 100;
      somethingRect2.x = width + 100;
      somethingRect3.x = width + 100;
      pRect.x = 0;
      pRect.y = (height - height / 4) - 150;
      pRect.w = 100;
      pRect.h = 100;
      pSRect.x = 150;
      pSRect.y = 0;
      pSRect.w = 75;
      pSRect.h = 230;
      if (!init())
      {
        std::cout << "Failed to initialize!\n";
      }
      else
      {
        if (!loadImages())
        {
          std::cout << "Failed to load images!\n";
        }
        else
        {
          if (!loadSounds())
          {
            std::cout << "Failed to load sounds!" << std::endl;
          }
        }
      }
      while (!quit)
      {
        gameloop();
      }
      close();
    }
};


bool Main::init()
{
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    std::cout << "Couldn't init SDL, :/, Err: " << SDL_GetError();
    success = false;
  }
  else
  {
    window = SDL_CreateWindow("Pour Mimi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
      std::cout << "Window failed at it's job and did not appear, this is why: " << SDL_GetError() << std::endl;
      success = false;
    }
    else
    {
      if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
      {
        std::cout << "Audio is mean with me :/ he doesn't wanna init! Error: " << Mix_GetError() << std::endl;
      }
      screenSurface = SDL_GetWindowSurface(window);
      SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    }
  }
  return success;
}


bool Main::loadImages()
{
  player = SDL_LoadBMP("images/Character-sprites.bmp");
  something = SDL_LoadBMP("images/something.bmp");
  _something = SDL_LoadBMP("images/something.bmp");
  pewpew = SDL_LoadBMP("images/bullet.bmp");
  gift = SDL_LoadBMP("images/gift.bmp");
  gameOver = SDL_LoadBMP("images/game-over.bmp");
  yay = SDL_LoadBMP("images/yay.bmp");
  return true;
}


bool Main::loadSounds()
{
  hit = Mix_LoadWAV("sounds/hit.mp3");
  step = Mix_LoadWAV("sounds/step.mp3");
  shoot = Mix_LoadWAV("sounds/shoot.wav");
  return true;
}


void Main::gameloop()
{
  while (SDL_PollEvent(&e) != 0)
  {
    if (e.type == SDL_QUIT)
    {
      quit = true;
    }
    else if (e.type == SDL_KEYDOWN)
    {
      switch(e.key.keysym.sym)
      {
        case SDLK_ESCAPE:
          quit = true;
          break;
        case SDLK_a:
          aPressed = true;
          break;
        case SDLK_d:
          dPressed = true;
          break;
        case SDLK_s:
          sPressed = true;
          break;
        case SDLK_w:
          wPressed = true;
          break;
        case SDLK_SPACE:
          charging = true;
          break;
      }
    }
    else if (e.type == SDL_KEYUP)
    {
      switch(e.key.keysym.sym)
      {
        case SDLK_a:
          aPressed = false;
          break;
        case SDLK_d:
          dPressed = false;
          break;
        case SDLK_s:
          sPressed = false;
          break;
        case SDLK_w:
          wPressed = false;
          break;
        case SDLK_SPACE:
          charging = false;
          break;
      }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      SDL_GetMouseState(&mx, &my);
      dx = (double)mx - ((double)pRect.x + pRect.w / 2);
      dy = (double)my - ((double)pRect.y + pRect.h / 2);
      if (!pew)
      {
        angle[0] = dx / pow(pow(dx, 2) + pow(dy, 2), 0.5);
        angle[1] = dy / pow(pow(dx, 2) + pow(dy, 2), 0.5);
        pewRect.x = pRect.x + pRect.w / 2;
        pewRect.y = pRect.y + pRect.h / 2;
        pew = true;
        Mix_PlayChannel(-1, shoot, 0);
      }
      /*
      SDL_GetMouseState(&mx, &my);
      dx = (double)mx - (double)pRect.x;
      dy = (double)my - (double)pRect.y;
      speed = pow(pow(abs(dx), 2) + pow(abs(dy), 2), 0.5) / (50 / initspeed);
      angle[0] = dx / pow(pow(dx, 2) + pow(dy, 2), 0.5);
      angle[1] = dy / pow(pow(dx, 2) + pow(dy, 2), 0.5);
      Mix_PlayChannel(-1, hit, 0);
      */
    }
  }

  if (aPressed)
  {
    pRect.x -= (int)initspeed;
  }
  if (dPressed)
  {
    pRect.x += (int)initspeed;
  }
  /*
  if (wPressed)
  {
    pRect.y -= (int)initspeed;
  }
  if (sPressed)
  {
    pRect.y += (int)initspeed;
  }
  */
  if (aPressed || dPressed || wPressed || sPressed)
  {
    animationcycle++;
    animationcycle = animationcycle % 16;
    if (animationcycle % 8 == 0)
    {
      Mix_PlayChannel(-1, step, 0);
    }
    if (animationcycle < 8)
    {
      pSRect.x = 0;
    }
    if (animationcycle >= 8)
    {
      pSRect.x = 75;
    }
  }
  else
  {
    pSRect.x = 150;
    animationcycle = 8;
  }
  if (somethingTimer > 0)
  {
    somethingTimer--;
  }
  else
  {
    spawnSomething();
    somethingTimer = (rand() % 1001) + 1000;
  }
  if (pRect.x < 0) {pRect.x = 0;}
  if (pRect.y < 0) {pRect.y = 0;}
  if (pRect.x > width - 75) {pRect.x = width - 75;}
  if (pRect.y > ((height / 4) * 3) - 150) {pRect.y = ((height / 4) * 3) - 150;}
  if (pew) {pewRect.x += (30 * angle[0]); pewRect.y += (30 * angle[1]);}
  if (pewRect.x < 0) {pew = false;}
  if (pewRect.y < 0) {pew = false;}
  if (pewRect.x > width) {pew = false;}
  if (pewRect.y > height) {pew = false;}
  if (pew)
  {
    if (pewRect.x < somethingRect1.x + somethingRect1.w && pewRect.x + pewRect.w > somethingRect1.x)
    {
      if (pewRect.y < somethingRect1.y + somethingRect1.h && pewRect.y + pewRect.h > somethingRect1.y)
      {
        // Bullet is colliding with something1
        somethingRect1.x = width + 100;
        somethingTimer = (rand() % 101) + 150;
        pew = false;
        giftRect.x = rand() % width - 100;
        Mix_PlayChannel(-1, hit, 0);
      }
    }
    if (pewRect.x < somethingRect2.x + somethingRect2.w && pewRect.x + pewRect.w > somethingRect2.x)
    {
      if (pewRect.y < somethingRect2.y + somethingRect2.h && pewRect.y + pewRect.h > somethingRect2.y)
      {
        // Bullet is colliding with something2
        somethingRect2.x = width + 100;
        somethingTimer = (rand() % 101) + 150;
        pew = false;
        giftRect.x = rand() % width - 100;
        Mix_PlayChannel(-1, hit, 0);
      }
    }
    if (pewRect.x < somethingRect3.x + somethingRect3.w && pewRect.x + pewRect.w > somethingRect3.x)
    {
      if (pewRect.y < somethingRect3.y + somethingRect3.h && pewRect.y + pewRect.h > somethingRect3.y)
      {
        // Bullet is colliding with something3
        somethingRect1.x = width + 100;
        somethingTimer = (rand() % 101) + 150;
        pew = false;
        giftRect.x = rand() % width - 100;
        Mix_PlayChannel(-1, hit, 0);
      }
    }
  }
  if (charging)
  {
    if (abs((pRect.x - giftRect.x) + (pRect.y - giftRect.y)) <= 200)
    {
      charge++;
    }
  }
  if (charge >= 5000)
  {
    SDL_BlitSurface(yay, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);
    quit = true;
  }
  std::cout << somethingRect1.x << ", " << somethingRect1.y << std::endl;
  std::cout << somethingRect2.x << ", " << somethingRect2.y << std::endl;
  std::cout << somethingRect3.x << ", " << somethingRect3.y << std::endl;
  std::cout << somethingTimer << std::endl;
  std::cout << charge << std::endl;
  /*
  SDL_GetMouseState(&mx, &my);
  pRect.x -= (pRect.x - mx) / (100 / speed);
  pRect.y -= (pRect.y - my) / (100 / speed);
  */
  /*
  pRect.x += speed * angle[0];
  pRect.y += speed * angle[1];
  if (speed > 0)
  {
    speed -= 50 / initspeed;
  }
  if (speed < 0) {speed=0;}
  if (pRect.x <= 0) {pRect.x=0; angle[0] = -angle[0];}
  if (pRect.y <= 0) {pRect.y=0; angle[1] = -angle[1];}
  if ((pRect.x + pRect.w) >= width) {pRect.x=width-pRect.w; angle[0] = -angle[0];}
    if ((pRect.y + pRect.h) >= height) {pRect.y=height-pRect.h; angle[1] = -angle[1];}
    */
  SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
  tmpRect.x = 0;
  tmpRect.y = ((height / 4) * 3) + 70;
  tmpRect.w = width;
  tmpRect.h = (height / 4) - 70;
  SDL_FillRect(screenSurface, &tmpRect, SDL_MapRGB(screenSurface->format, 0xA5, 0x4A, 0x2A));
  tmpRect.x = giftRect.x;
  tmpRect.y = giftRect.y - 50;
  tmpRect.w = 100;
  tmpRect.h = 5;
  SDL_FillRect(screenSurface, &tmpRect, SDL_MapRGB(screenSurface->format, 0x80, 0x80, 0x80));
  tmpRect.w = (100.0 / 5000.0) * charge;
  SDL_FillRect(screenSurface, &tmpRect, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));
  SDL_BlitSurface(something, NULL, screenSurface, &somethingRect1);
  SDL_BlitSurface(something, NULL, screenSurface, &somethingRect2);
  SDL_BlitSurface(something, NULL, screenSurface, &somethingRect3);
  SDL_BlitSurface(gift, NULL, screenSurface, &giftRect);
  SDL_BlitSurface(player, &pSRect, screenSurface, &pRect);
  if (pew)
  {
    SDL_BlitSurface(pewpew, NULL, screenSurface, &pewRect);
  }
  SDL_UpdateWindowSurface(window);
}


void Main::spawnSomething()
{
  if (somethingRect1.x == width + 100)
  {
    somethingRect1.x = rand() % width;
    somethingRect1.y = rand() % height;
    std::cout << somethingRect1.x << ", " << somethingRect1.y << std::endl;
    std::cout << "Spawning something1" << std::endl;
  }
  else
  {
    if (somethingRect2.x == width + 100)
    {
      somethingRect2.x = rand() % width;
      somethingRect2.y = rand() % height;
      std::cout << somethingRect2.x << ", " << somethingRect2.y << std::endl;
      std::cout << "Spawning something2" << std::endl;
    }
    else
    {
      if (somethingRect3.x == width + 100)
      {
        somethingRect3.x = rand() % width;
        somethingRect3.y = rand() % height;
        std::cout << somethingRect3.x << ", " << somethingRect3.y << std::endl;
        std::cout << "Spawning something3" << std::endl;
      }
      else
      {
        std::cout << "3 Somethings have already spawned!" << std::endl;
        SDL_BlitSurface(gameOver, NULL, screenSurface, NULL);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(5000);
        quit=true;
      }
    }
  }
}


void Main::close()
{
  SDL_DestroyWindow(window);
  SDL_FreeSurface(player);
  SDL_Quit();
}


int main(int argc, char *args[])
{
  Main mainclass;
}
