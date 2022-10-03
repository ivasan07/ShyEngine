#include "RendererManager.h"

#include <SDL_image.h>

#define COLOREXP(color) \
	color.r, color.g, color.b, color.a

namespace RendererManager {

    RendererManager::RendererManager() : RendererManager("SDL Window", 600, 400) {}

    RendererManager::RendererManager(const std::string& title, int w, int h) {
        windowTitle = title;
        width = w;  height = h;

        initSDL();
    }

    RendererManager::~RendererManager() {
        closeSDL();
    }

    void RendererManager::initSDL()
    {
        // initialise SDL
        int sdlInit_ret = SDL_Init(SDL_INIT_EVERYTHING);
        assert(sdlInit_ret == 0);

        // Create window
        window = SDL_CreateWindow(windowTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        assert(window != nullptr);

        // Create the renderer
        renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        assert(renderer != nullptr);

        // initialize SDL_image
        int imgInit_ret = IMG_Init(
            IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
        assert(imgInit_ret != 0);
    }

    void RendererManager::closeSDL() {
        SDL_DestroyWindow(window); window = NULL;
        SDL_DestroyRenderer(renderer); renderer = NULL;
        SDL_Quit();
        IMG_Quit();
    }

    SDL_Window* RendererManager::getWindow() {
        return window;
    }

    SDL_Renderer* RendererManager::getRenderer() {
        return renderer;
    }

    int RendererManager::getWidth() {
        return width;
    }

    int RendererManager::getHeight() {
        return height;
    }

    void RendererManager::toggleFullScreen() {
        auto flags = SDL_GetWindowFlags(window);
        if (flags & SDL_WINDOW_FULLSCREEN) {
            SDL_SetWindowFullscreen(window, 0);
        }
        else {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
    }

    void RendererManager::resizeWindow(int w, int h) {
        SDL_SetWindowSize(window, w, h);
    }

    void RendererManager::renameWindow(const std::string& name) {
        SDL_SetWindowTitle(window, name.c_str());
    }

    void RendererManager::repositionWindow(int x, int y) {
        SDL_SetWindowPosition(window, x, y);
    }

    SDL_Color RendererManager::createColor(unsigned long num) {
        return
        {
             static_cast<Uint8>((num >> 24) & 0xff),
             static_cast<Uint8>((num >> 16) & 0xff),
             static_cast<Uint8>((num >> 8) & 0xff),
             static_cast<Uint8>(num & 0xff)
        };
    }

    SDL_Color RendererManager::createColor(std::string strnum) {
        return createColor(std::stoul(strnum.erase(0, 2), nullptr, 16));
    }

    SDL_Rect RendererManager::createRect(Vector2D& pos, float w, float h) {
        return
        {
            static_cast<int>(pos.getX()),
            static_cast<int>(pos.getY()),
            static_cast<int>(w),
            static_cast<int>(h) 
        };
    }

    SDL_Rect RendererManager::createRect(float x, float y, float w, float h) {
        return
        {
            static_cast<int>(x),
            static_cast<int>(y),
            static_cast<int>(w),
            static_cast<int>(h)
        };
    }

    void RendererManager::showCursor(bool show) {
        SDL_ShowCursor(show);
    }

    void RendererManager::clearRenderer(SDL_Color bg) {
        SDL_SetRenderDrawColor(renderer, COLOREXP(bg));
        SDL_RenderClear(renderer);
    }

    void RendererManager::presentRenderer() {
        SDL_RenderPresent(renderer);
    }
}