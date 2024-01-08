#pragma once

#include <SFML/Graphics/Image.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define HUD so::Hud::instance()

namespace so
{
    enum Status
    {
        WIN,
        LOSE,
        ONGOING,
        LOADING
    };
}