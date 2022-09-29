#pragma once

#include "../element.hpp"

namespace lc_gui
{
    class ButtonElement : public Element
    {
    public:
        ButtonElement(const std::string &className, const std::string &clickClassStr, std::function<void(ButtonElement *)> clickCallback);

        bool inputs(const glm::ivec2 &relativeCursorPosition) override;

        void click();

        std::function<void(ButtonElement *)> clickCallback;
        std::string clickClassStr;
    };
}