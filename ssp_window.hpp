#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace ssp{


class SspWindow
{
private:

    void initWindow();

    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
public:
    SspWindow(int w, int h, std::string name);
    ~SspWindow();

    SspWindow(const SspWindow &)=delete;
    SspWindow &operator=(const SspWindow &)=delete;
    
    bool shouldClose(){ return glfwWindowShouldClose(window);}
    VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface );
};






} // namespace ssp
