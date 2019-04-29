#include "ClassicMode.h"

using namespace std;

ClassicMode::ClassicMode()
{
    frame = 0;
    isHittingWall = false;
    status = GOING_RIGHT;
    score = 0;
//    background = new Background;
//    bird = new Bird;
//    spike = new Spike;
}

ClassicMode::~ClassicMode()
{
//    delete background;
//    delete bird;
//    delete spike;

//    background = NULL;
//    bird = NULL;
//    spike = NULL;
}

void ClassicMode::loadMedia(SDL_Renderer* renderer)
{
    background.loadMedia(renderer);
    bird.loadMedia(renderer);
    spike.loadMedia(renderer);
    s_score.loadMedia(renderer);
    item.loadMedia(renderer);

    sound.push_back(Mix_LoadWAV("assets/audio/jump.wav"));
    sound.push_back(Mix_LoadWAV("assets/audio/point.wav"));
    sound.push_back(Mix_LoadWAV("assets/audio/dead.wav"));

}

void ClassicMode::handleEvent(SDL_Event event, bool& end_loop, int &mode)
{
    if(status != DEATH)
    {
        while(SDL_PollEvent(&event))
        {
            bird.handleEvent(event, status, sound);
            switch(event.type)
            {
            case SDL_QUIT:
            {
                end_loop = true;
                break;
            }
            case SDL_KEYUP:
            {
                if(event.key.keysym.sym==SDLK_ESCAPE)
                {
                    status = DEATH;
                }
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }
    else if(status == DEATH)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
            {
                end_loop = true;
                break;
            }
            case SDL_KEYUP:
            {
                if(event.key.keysym.sym==SDLK_ESCAPE)
                {
                    status = DEATH;
                }
                else if(event.key.keysym.sym==SDLK_SPACE)
                {
                    mode = 0;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
                mode = 0;
            default:
            {
                break;
            }
            }
        }
    }
}

void ClassicMode::update(bool &end_loop, int &mode)
{
    bird.update(status, score, isHittingWall);
    spike.update(status, score, isHittingWall);
    item.update(status, isHittingWall);
    item.itemAnimation();
    item.checkIfEaten(bird, score);
    background.update(mode); //change background for each mode
    //cout << mode << endl;
    for(int i = 0; i < spike.getSpikeNumber(); i++)
    {
        if(status == GOING_RIGHT)
        {
            if((bird.getY(0)+24 >= spike.getY(i))
                    &&(bird.getY(0) <= spike.getY(i)+10)
                    &&(bird.getX(0)+34 >= spike.getX(i))
                    &&(bird.getX(0) <= spike.getX(i)+30))
            {
                status = DEATH;
            }
        }
        else if(status == GOING_LEFT)
        {
            if((bird.getY(0) <= spike.getY(i)+10)
                    &&(bird.getY(0)+24 >= spike.getY(i))
                    &&(bird.getX(0) <= spike.getX(i)+30)
                    &&(bird.getX(0)+30 >= spike.getX(i)))
            {
                status = DEATH;
            }
        }
    }

    if(bird.getY(0) == 0 || bird.getY(0) == SCREEN_HEIGHT-24)
    {
        status = DEATH;
    }
    if(status == DEATH)
    {
        bird.pause();
        Mix_PlayChannel( -1, sound.at(2), 0 );
    }

}

void ClassicMode::playSound()
{
    bird.playSound(sound, isHittingWall);
}

void ClassicMode::render(SDL_Renderer* renderer, bool end_loop)
{

    if(!end_loop)
    {
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        background.render(background.getMode(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, renderer, 0, NULL, SDL_FLIP_NONE);
        if(status == GOING_RIGHT)
        {


            bird.render(frame/3, bird.getX(0), bird.getY(0), bird.getWidth(0), bird.getHeight(0), renderer, 0, NULL,SDL_FLIP_NONE); //frame/3 for bird animation
            if(!item.getItemState()) item.render(item.getItemType(), item.getX(0), item.getY(0), item.getWidth(0), item.getHeight(0), renderer, 0, NULL, SDL_FLIP_NONE); //if item hasn't been eaten

            for (int i = 0; i< (spike.getSpikeNumber()); i++)
            {
                spike.render(0, spike.getX(i), spike.getY(i), spike.getWidth(0), spike.getHeight(0), renderer, 0, NULL, SDL_FLIP_HORIZONTAL);
            }
        }
        else if(status == GOING_LEFT)
        {
            //.render(0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, renderer, 0, NULL, SDL_FLIP_NONE);
            bird.render(frame/3, bird.getX(0), bird.getY(0), bird.getWidth(0), bird.getHeight(0), renderer, 0, NULL,SDL_FLIP_HORIZONTAL);
            if(!item.getItemState()) item.render(item.getItemType(), item.getX(0), item.getY(0), item.getWidth(0), item.getHeight(0), renderer, 0, NULL, SDL_FLIP_NONE); //if item hasn't been eaten
            for (int i = 0; i< spike.getSpikeNumber(); i++)
            {
                spike.render(0, spike.getX(i), spike.getY(i), spike.getWidth(0), spike.getHeight(0), renderer, 0, NULL, SDL_FLIP_NONE);
            }
        }
        else if(status >= DEATH)
        {
            //background.render(0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, renderer, 0, NULL, SDL_FLIP_NONE);
            background.render(GAME_OVER, (SCREEN_WIDTH-background.getWidth(GAME_OVER))/2, (SCREEN_HEIGHT-background.getHeight(GAME_OVER))/2, background.getWidth(GAME_OVER), background.getHeight(GAME_OVER), renderer, 0, NULL, SDL_FLIP_NONE); //render game over screen
        }

        s_score.renderScore(renderer, score);
        frame++;
        if(frame/3 >= 3)
            frame = 0;

        SDL_RenderPresent(renderer);
    }
}
