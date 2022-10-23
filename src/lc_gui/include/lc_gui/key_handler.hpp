#pragma once

#include "window.hpp"

namespace lc_gui
{
    class KeyHandler
    {
    public:
        KeyHandler(Window *window);

        void addKey(std::vector<GLenum> keyCombination, bool allowKeyPress, std::function<void(int)> callback, int id = -1);
        void addKey(std::vector<GLenum> keyCombination, std::function<void(int)> pressCallback, std::function<void(int)> releaseCallback, int id = -1);

        void update();

    private:
        static const double KEY_DOWN_PRESS_TIME;

        Window *window;

        struct Key
        {
            std::vector<GLenum> keyCombination;
            bool allowKeyPress{false}, pressAndRelease{false};
            int id;
            std::function<void(int)> callback, altCallback;

            bool keyDown{false};
            double keyDownTime;
        };
        std::vector<Key> keys;
    };
}