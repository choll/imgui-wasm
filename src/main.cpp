//
// This file is derived from imgui-highdpi-sapp.c (Copyright (c) 2017 Andre Weissflog)
//

//------------------------------------------------------------------------------
//  imgui-highdpi-sapp.c
//
//  This demonstrates Dear ImGui rendering via sokol_gfx.h and sokol_imgui.h,
//  with HighDPI rendering and a custom embedded font.
//------------------------------------------------------------------------------
#include "gui.hpp"

#include <imgui.h>
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_log.h>
#define SOKOL_IMGUI_IMPL
#include <util/sokol_imgui.h>

#include <cstddef>

void init()
{
    // setup sokol-gfx and sokol-time
    sg_desc desc = {};
    desc.context = sapp_sgcontext();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    // setup sokol-imgui, but provide our own font
    simgui_desc_t simgui_desc = {};
    simgui_desc.no_default_font = true;
    simgui_desc.logger.func = slog_func;
    simgui_setup(&simgui_desc);

    ImGui::StyleColorsLight();

    // configure Dear ImGui with our own embedded font
    auto& io = ImGui::GetIO();

    ImFontConfig cfg;

    cfg.OversampleH = 2;
    cfg.OversampleV = 2;
    // Fonts can be brightened by setting:
    //cfg.RasterizerMultiply = 1.5f;

    io.Fonts->AddFontFromFileTTF("fonts/NotoSans-Regular.ttf", 18.0f, &cfg);
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 16.0f, &cfg);
    io.Fonts->AddFontFromFileTTF("fonts/Silian-Rail.ttf", 16.0f, &cfg);

    // create font texture and linear-filtering sampler for the custom font
    // NOTE: linear filtering looks better on low-dpi displays, while
    // nearest-filtering looks better on high-dpi displays
    unsigned char* font_pixels;
    int font_width, font_height;
    io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
    sg_image_desc img_desc = {};
    img_desc.width = font_width;
    img_desc.height = font_height;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    img_desc.data.subimage[0][0].ptr = font_pixels;
    img_desc.data.subimage[0][0].size = std::size_t(font_width * font_height * 4);
    sg_image font_img = sg_make_image(&img_desc);
    sg_sampler_desc smp_desc = {};
    smp_desc.min_filter = SG_FILTER_LINEAR;
    smp_desc.mag_filter = SG_FILTER_LINEAR;
    smp_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    smp_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    sg_sampler font_smp = sg_make_sampler(&smp_desc);
    simgui_image_desc_t font_desc = {};
    font_desc.image = font_img;
    font_desc.sampler = font_smp;
    io.Fonts->TexID = simgui_imtextureid(simgui_make_image(&font_desc));
}

void frame()
{
    const int width = sapp_width();
    const int height = sapp_height();
    simgui_new_frame({width, height, sapp_frame_duration(), sapp_dpi_scale()});

    draw();

    static sg_pass_action pass_action;
    pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass_action.colors[0].clear_value = {0.45f, 0.55f, 0.60f, 1.0f}; // background color

    // The sokol_gfx draw pass
    sg_begin_default_pass(&pass_action, width, height);
    simgui_render();
    sg_end_pass();
    sg_commit();
}

void cleanup()
{
    simgui_shutdown();
    sg_shutdown();
}

void input(const sapp_event* event)
{
    if (event->type == SAPP_EVENTTYPE_QUIT_REQUESTED)
    {
        // This branch is taken if sapp_request_quit() is called, the quit
        // request can be called via sapp_cancel_quit() (then a dialog can
        // be displayed to ask for confirmation).
        sapp_quit();
    }
    else
    {
        simgui_handle_event(event);
    }
}

sapp_desc sokol_main(int, char*[])
{
    sapp_desc desc = {};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = input;
    desc.width = 1024;
    desc.height = 768;
    desc.fullscreen = true;
    desc.high_dpi = true;
    desc.html5_ask_leave_site = true;
    desc.ios_keyboard_resizes_canvas = false;
    desc.window_title = "Dear ImGui HighDPI";
    desc.icon.sokol_default = true;
    desc.enable_clipboard = true;
    desc.logger.func = slog_func;
    return desc;
}
