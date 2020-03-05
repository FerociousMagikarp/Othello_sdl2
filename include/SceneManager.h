#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <SDL.h>

class Scene;

class SceneManager
{
    public:
        ~SceneManager();
        static SceneManager* get_instance();

        void run();
        void quit();

    protected:

    private:
        SceneManager();
        SceneManager(const SceneManager&);
        SceneManager& operator=(const SceneManager&);

        bool m_run;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        Scene* scene;
};

#endif // SCENEMANAGER_H
