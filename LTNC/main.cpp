#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>


#define WIDTH_MAIN_OBJECT 80
#define HEIGHT_MAIN_OBJECT 46
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "SDL_test";

SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

class SDLCommonFunction {
public:
    static SDL_Texture* loadTexture(const string& path);
    static void logSDLError(ostream& os, const string& msg, bool fatal);
    static void initSDL();
    static void quitSDL();
    static void waitUntilKeyPressed();
};

class BaseObject {
protected:
    SDL_Rect rect_;
    SDL_Texture* p_object_;
public:
    BaseObject();
    ~BaseObject();
    void Show();
    bool LoadImg(const char* file_name);
    void SetRect(int x, int y) {
        rect_.x = x;
        rect_.y = y;
    }
    SDL_Rect GetRect() const { return rect_; }
    SDL_Texture* GetObject() { return p_object_; }
};

class MainObject : public BaseObject {
public:
    MainObject();
    ~MainObject();
    void HandleInputAction(SDL_Event events);
    void HandleMove();
private:
    int x_val_;
    int y_val_;
};

int main(int argc, char* argv[]) {
    SDLCommonFunction::initSDL();

    MainObject human_object;
    human_object.SetRect(300, 420);
    bool ret = human_object.LoadImg("plane.png");
    if (!ret)
        return 0;

    SDL_Texture* background = SDLCommonFunction::loadTexture("vutru.jpg");
    SDL_RenderCopy(g_renderer, background, nullptr, nullptr);

    human_object.Show();
    SDL_RenderPresent(g_renderer);
    SDLCommonFunction::waitUntilKeyPressed();

    SDL_DestroyTexture(background);
    SDLCommonFunction::quitSDL();

    return 0;
}

SDL_Texture* SDLCommonFunction::loadTexture(const string& path) {
    SDL_Texture* newTexture = IMG_LoadTexture(g_renderer, path.c_str());
    if (newTexture == nullptr)
        logSDLError(cout, "Unable to load texture " + path + " SDL_image Error: " + IMG_GetError(), false);
    return newTexture;
}

void SDLCommonFunction::logSDLError(ostream& os, const string& msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << endl;
    if (fatal) {
        quitSDL();
        exit(1);
    }
}

void SDLCommonFunction::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(cout, "SDL_Init", true);

    g_window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == nullptr)
        logSDLError(cout, "CreateWindow", true);

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == nullptr)
        logSDLError(cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(g_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void SDLCommonFunction::quitSDL() {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

void SDLCommonFunction::waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}

BaseObject::BaseObject() {
    rect_.x = 0;
    rect_.y = 0;
    p_object_ = nullptr;
}

BaseObject::~BaseObject() {
    if (p_object_ != nullptr) {
        SDL_DestroyTexture(p_object_);
    }
}

bool BaseObject::LoadImg(const char* file_name) {
    p_object_ = SDLCommonFunction::loadTexture(file_name);
    if (p_object_ == nullptr)
        return false;
    return true;
}

void BaseObject::Show() {
    SDL_RenderCopy(g_renderer, p_object_, nullptr, &rect_);
}

MainObject::MainObject() {
    rect_.w = WIDTH_MAIN_OBJECT;
    rect_.h = HEIGHT_MAIN_OBJECT;
    x_val_ = 0;
    y_val_ = 0;
}

MainObject::~MainObject() {
    ;
}

void MainObject::HandleInputAction(SDL_Event events) {
}

void MainObject::HandleMove() {
}
