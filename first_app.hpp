#pragma once

#include "ssp_window.hpp"
#include "ssp_game_object.hpp"
#include "ssp_device.hpp"
#include "ssp_renderer.hpp"

#include <memory>
#include <vector>

namespace ssp{
class FirstApp
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT =600;
    

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &)=delete;
    FirstApp &operator=(const FirstApp &)=delete;

    void run();
private:
    void loadGameObjects();

    SspWindow sspWindow{WIDTH,HEIGHT, "Hello!"};
    SspDevice sspDevice{sspWindow};
    SspRenderer sspRenderer{sspWindow, sspDevice};
    std::vector<SspGameObject> gameObjects;

};



} // namespace ssp