#include "ssp_window.hpp"

#include <stdexcept>

namespace ssp{
SspWindow::SspWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
{
    initWindow();
}

void SspWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_TRUE);

    window = glfwCreateWindow(width, height ,windowName.c_str(),nullptr,nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}


SspWindow::~SspWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void SspWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface )
{
    if(glfwCreateWindowSurface(instance, window, nullptr, surface )!= VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface");
    }
}

void SspWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto sspWindow = reinterpret_cast<SspWindow *>(glfwGetWindowUserPointer(window));
    sspWindow->framebufferResized=true;
    sspWindow->width=width;
    sspWindow->height=height;
}


} // namespace ssp