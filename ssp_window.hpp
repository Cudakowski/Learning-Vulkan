#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace ssp{


class SspWindow
{
private:
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void initWindow();

    int width;
    int height;
    bool framebufferResized{false};

    std::string windowName;
    GLFWwindow *window;
public:
    SspWindow(int w, int h, std::string name);
    ~SspWindow();

    SspWindow(const SspWindow &)=delete;
    SspWindow &operator=(const SspWindow &)=delete;
    
    bool shouldClose(){ return glfwWindowShouldClose(window);}
    VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}
    bool wasWindowResized(){return framebufferResized;}
    void resetWindowResizedFlag(){ framebufferResized=false;}

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface );
};






} // namespace ssp
