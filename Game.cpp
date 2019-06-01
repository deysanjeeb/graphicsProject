#include "Game.hpp"

Game::Game()
{
}

bool Game::Initialize()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    SDL_Log("Error: %s", SDL_GetError());
    return false;
  }

  SDL_GetCurrentDisplayMode(0, &this->DM);
  this->screenHeight = this->DM.h;
  this->screenWidth = this->DM.w;

  this->mWindow = SDL_CreateWindow(
      "Chapter 1",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      this->screenWidth,
      this->screenHeight,
      0);

  if (!this->mWindow)
  {
    SDL_Log("Error: %s", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(
      this->mWindow,
      -1,
      SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);

  if (!mRenderer)
  {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  mBallPos.x = this->screenWidth / 2;
  mBallPos.y = this->screenHeight / 2;

  return true;
}

void Game::Shutdown()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::RunLoop()
{
  while (this->mIsRunning)
  {
    this->ProcessInput();
    this->UpdateGame();
    this->GenerateOutput();
  }
}

void Game::ProcessInput()
{
  this->event;
  while (SDL_PollEvent(&this->event))
  {
    switch (this->event.type)
    {
    case SDL_QUIT:
      SDL_Event event;
      mIsRunning = false;
      break;
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
      SDL_Event event;
      mIsRunning = false;
    }
  }
}

void Game::UpdateGame()
{
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->mTicksCount + 16))
    ;
  this->deltaTime = (SDL_GetTicks() - this->mTicksCount);
  this->mTicksCount = SDL_GetTicks();
  if (this->deltaTime > 0.05f)
  {
    this->deltaTime = 0.05f;
  }

  //update objects using deltaTime
  this->mBallPos.x += 50 * deltaTime;
}

void Game::GenerateOutput()
{
  //renderer, rgba; black
  SDL_SetRenderDrawColor(
      this->mRenderer,
      0,
      0,
      0,
      255);

  //draw scene

  //background
  SDL_RenderClear(this->mRenderer);

  //white for walls
  SDL_SetRenderDrawColor(
      this->mRenderer,
      255,
      255,
      255,
      255);
  SDL_Rect wall_left{
      0,
      0,
      this->THICKNESS,
      this->screenHeight};
  SDL_Rect wall_right{
      this->screenWidth - (this->THICKNESS),
      0,
      this->THICKNESS,
      this->screenHeight};
  SDL_Rect wall_top{
      0,
      0,
      this->screenWidth,
      this->THICKNESS};
  SDL_Rect wall_bottom{
      0,
      this->screenHeight - (this->THICKNESS),
      this->screenWidth,
      this->THICKNESS};
  SDL_RenderFillRect(this->mRenderer, &wall_left);
  SDL_RenderFillRect(this->mRenderer, &wall_right);
  SDL_RenderFillRect(this->mRenderer, &wall_top);
  SDL_RenderFillRect(this->mRenderer, &wall_bottom);

  SDL_Rect ball{
      static_cast<int>(mBallPos.x - this->THICKNESS / 2),
      static_cast<int>(mBallPos.y - this->THICKNESS / 2),
      this->THICKNESS,
      this->THICKNESS};
  SDL_RenderFillRect(this->mRenderer, &ball);

  SDL_RenderPresent(this->mRenderer);
}