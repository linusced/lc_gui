#include "lc_gui/basic_elements/text_input_element.hpp"

lc_gui::TextInputElement::TextInputElement(TextRenderer::Font *font, const std::string &text, const std::string &className, std::function<void(TextInputElement *, std::string &)> callback)
    : TextElement(font, text, className, false, true), callback(callback) {}

bool lc_gui::TextInputElement::inputs(const glm::ivec2 &relativeCursorPosition)
{
    window->setCursor(GLFW_IBEAM_CURSOR);
    unsigned int charCodePoint = window->getCharCodePoint();
    std::string text = getText();

    if (backspaceKeyDown && !glfwGetKey(*window, GLFW_KEY_BACKSPACE))
        backspaceKeyDown = false;
    if (arrowLeftKeyDown && !glfwGetKey(*window, GLFW_KEY_LEFT))
        arrowLeftKeyDown = false;
    if (arrowRightKeyDown && !glfwGetKey(*window, GLFW_KEY_RIGHT))
        arrowRightKeyDown = false;

    double currentTime = window->getCurrentTime();
    if (window->isLmbDown())
        requireInputs = true;
    else
        lmbDown = false;

    if (requireInputs)
    {
        if (prevText.empty())
            prevText = text;

        if (window->isLmbDown())
        {
            if (relativeCursorPosition.x < 0 || relativeCursorPosition.y < 0 || relativeCursorPosition.x >= prevLayout.width || relativeCursorPosition.y >= prevLayout.height)
            {
                requireInputs = false;
                updateCursorPos(-1);
                if (callback && !onlyCallbackOnEnterPress)
                {
                    callback(this, text);
                    setText(text);
                }
                else if (resetTextOnEscape)
                    setText(prevText);

                prevText.clear();
            }
            else if (!lmbDown)
            {
                lmbDown = true;

                int textOffsetX = (prevLayout.width - textRenderer.textureWidth) / 2;
                if (textRenderer.textureWidth > prevLayout.width)
                    textOffsetX = 0;

                if (relativeCursorPosition.x < textOffsetX)
                    textCursorOffset = text.size();
                else if (relativeCursorPosition.x > textOffsetX + textRenderer.textureWidth)
                    textCursorOffset = 0;
                else
                {
                    int cursorX = relativeCursorPosition.x;
                    if (textRenderer.textureWidth > prevLayout.width)
                        cursorX *= textRenderer.textureWidth / (float)prevLayout.width;

                    for (int i = 0; i < textRenderer.characterLayouts.size(); i++)
                        if (textOffsetX + textRenderer.characterLayouts[i].x + (textRenderer.characterLayouts[i].width / 2) >= cursorX)
                        {
                            textCursorOffset = text.size() - i;
                            break;
                        }
                }

                prevChangeTime = currentTime;
                updateCursorPos(text.size() - textCursorOffset);
            }
        }
        else if (glfwGetKey(*window, GLFW_KEY_ESCAPE))
        {
            requireInputs = false;
            updateCursorPos(-1);
            if (callback && !onlyCallbackOnEnterPress)
            {
                callback(this, text);
                setText(text);
            }
            else if (resetTextOnEscape)
                setText(prevText);

            prevText.clear();
        }
        else if (glfwGetKey(*window, GLFW_KEY_ENTER) || glfwGetKey(*window, GLFW_KEY_KP_ENTER))
        {
            requireInputs = false;
            updateCursorPos(-1);
            if (callback)
            {
                callback(this, text);
                setText(text);
            }

            prevText.clear();
        }
        else if (glfwGetKey(*window, GLFW_KEY_LEFT))
        {
            if ((!arrowLeftKeyDown || currentTime - arrowLeftKeyDownTime > .5) && textCursorOffset < text.size())
            {
                textCursorOffset++;
                if (!arrowLeftKeyDown)
                    arrowLeftKeyDownTime = currentTime;
            }
            arrowLeftKeyDown = true;

            prevChangeTime = currentTime;
            updateCursorPos(text.size() - textCursorOffset);
        }
        else if (glfwGetKey(*window, GLFW_KEY_RIGHT))
        {
            if ((!arrowRightKeyDown || currentTime - arrowRightKeyDownTime > .5) && textCursorOffset > 0)
            {
                textCursorOffset--;
                if (!arrowRightKeyDown)
                    arrowRightKeyDownTime = currentTime;

                prevChangeTime = currentTime;
                updateCursorPos(text.size() - textCursorOffset);
            }
            arrowRightKeyDown = true;
        }
        else if (glfwGetKey(*window, GLFW_KEY_BACKSPACE))
        {
            if (!backspaceKeyDown || currentTime - backspaceKeyDownTime > .5)
            {
                if (text.size() - textCursorOffset > 0)
                {
                    if (textCursorOffset == 0)
                        text = text.substr(0, text.size() - 1);
                    else
                        text = text.substr(0, text.size() - 1 - textCursorOffset) + text.substr(text.size() - textCursorOffset);

                    if (!backspaceKeyDown)
                        backspaceKeyDownTime = currentTime;

                    prevChangeTime = currentTime;
                    updateCursorPos(text.size() - textCursorOffset);
                    setText(text);
                }
                else
                    updateCursorPos(text.size() - textCursorOffset);
            }
            backspaceKeyDown = true;
        }
        else if (glfwGetKey(*window, GLFW_KEY_DELETE))
        {
            textCursorOffset = 0;

            prevChangeTime = currentTime;
            updateCursorPos(0);
            setText("");
        }
        else if (charCodePoint != 0 && charCodePoint < 128)
        {
            if (textCursorOffset == 0)
                text += (char)charCodePoint;
            else
                text = text.substr(0, text.size() - textCursorOffset) + (char)charCodePoint + text.substr(text.size() - textCursorOffset);

            prevChangeTime = currentTime;
            updateCursorPos(text.size() - textCursorOffset);
            setText(text);
        }
        else if (currentTime - prevChangeTime > 0.5)
            updateCursorPos(fmod(currentTime, 1) < 0.5 ? text.size() - textCursorOffset : -1);
    }
    return requireInputs;
}