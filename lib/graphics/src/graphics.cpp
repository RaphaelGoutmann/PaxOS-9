//
// Created by Charlito33 on 23/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#include "graphics.hpp"

#include <Surface.hpp>

#ifdef ESP_PLATFORM

#include "platforms/LGFX_ESP32.hpp"

#else

#include "lgfx/v1/platforms/sdl/Panel_sdl.hpp"
#include "LGFX_AUTODETECT.hpp"

#endif

namespace
{
    bool running;
    graphics::ERenderScale renderScale;

    std::shared_ptr<LGFX> lcd;
}

void graphics::init(const ERenderScale scale)
{
#ifdef ESP_PLATFORM

    running = true; // It doesn't feel right to set this here...

    lcd = std::make_shared<LGFX>();

#else

    lcd = std::make_shared<LGFX>(getScreenWidth(), getScreenHeight());

#endif

    renderScale = scale;

    lcd->init();
    lcd->setBrightness(0xFF);
    lcd->setColorDepth(16);
    lcd->setTextColor(TFT_WHITE);
    lcd->fillScreen(TFT_BLACK);

    lcd->startWrite(); // Keep the SPI Bus busy ?
}

int graphics::getRenderScale()
{
    switch (renderScale)
    {
    case DEFAULT:
        return 1;
    case SCALE_X2:
        return 2;
    case SCALE_X4:
        return 4;
    case SCALE_X8:
        return 8;
    default:
        return -1; // That's gonna be fun
    }
}

graphics::ERenderScale graphics::getRenderScaleEnum()
{
    return renderScale;
}

bool graphics::isRunning()
{
    return running;
}

#ifndef ESP_PLATFORM

struct SDLUpdateData
{
    void (*appMain)();
};

static int SDLUpdate(void *data)
{
    const auto *updateData = static_cast<SDLUpdateData *>(data);

    updateData->appMain();

    return 0;
}

void graphics::SDLInit(void (*appMain)())
{
    lgfx::Panel_sdl::setup();

    SDLUpdateData updateData
    {
        appMain
    };

    running = true;

    // Multithreading can be an issue, be careful
    SDL_Thread *thread = SDL_CreateThread(SDLUpdate, "graphics_update", &updateData);
    if (thread == nullptr)
    {
        printf("Unable to create thread : %s\n", SDL_GetError());
        exit(1);
    }

    while (lgfx::Panel_sdl::loop() == 0)
    {};

    running = false;

    SDL_WaitThread(thread, nullptr);

    lgfx::Panel_sdl::close();
}

#endif

// You should only use this function with a "Canvas" (Surface that is the size of the screen)
void graphics::renderSurface(const Surface* surface)
{
    lgfx::LGFX_Sprite sprite = surface->m_sprite; // we are friends !
    sprite.pushRotateZoom(lcd.get(), 0, static_cast<float>(getRenderScale()), static_cast<float>(getRenderScale()));
}

void graphics::flip()
{
    lcd->endWrite(); // Push write data
    lcd->display();
    lcd->startWrite(); // Keep the SPI bus busy ? Faster ?
}
