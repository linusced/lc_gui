#include "lc_gui/lc_gui.hpp"
#include "config.hpp"

class App : public lc_gui::DrawLoop
{
private:
    std::unique_ptr<lc_gui::TextRenderer::Font> font;
    std::unique_ptr<lc_gui::TextElement> text;

public:
    App(lc_gui::Window *window) : lc_gui::DrawLoop(window, lc_gui::textFileData(std::string(SOURCE_DIR) + "/assets/css/main.css"))
    {
        font = std::make_unique<lc_gui::TextRenderer::Font>(std::string(SOURCE_DIR) + "/assets/fonts/Arial.ttf");

        text = std::make_unique<lc_gui::TextElement>(font.get(), "Hello!\nWelcome to " + std::string(PROJECT_NAME) + " v." + std::string(PROJECT_VERSION), "text", true, false);
        guiRenderer->addElement(text.get());
    }
};

int main(int argc, char const *argv[])
{
    lc_gui::init(true);
    {
        lc_gui::Window window{-1, -1, lc_gui::Window::getMonitorWidth(), lc_gui::Window::getMonitorHeight(), std::string(PROJECT_NAME)};
        lc_gui::enableBlend();

        App appLoop{&window};
        appLoop.loop();
    }
    lc_gui::terminate();
    return 0;
}