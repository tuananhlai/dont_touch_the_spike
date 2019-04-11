#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Bird.h"
#include "Spike.h"
#include "Background.h"
#include "MainControl.h"

using namespace std;

int main(int argc, char* atgv[]) {
    MainControl main_control = MainControl();
    main_control.initializeSDL(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    while(!main_control.isEndLoop()) {
        main_control.handleEvent();                 //get the user event
        main_control.update();                      //update data
        main_control.render();                      //render the components
    }
    cout << main_control.getScore();
    main_control.close();
    return 0;
    return 0;
}
