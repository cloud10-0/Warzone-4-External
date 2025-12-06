#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <chrono>

struct Color {
    float r, g, b, a;
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

namespace settings
{
    // Window settings
    inline int width = 1920;
    inline bool Menu = false;
    inline int height = 1080;
    inline int screen_center_x = width / 2;
    inline int screen_center_y = height / 2;
    inline bool show_menu = true;
    inline int current_tab = 0;

    namespace aim
    {
        inline bool mouse_aim = false;
        inline bool prediction = false;
        inline bool visible_only = true;
        inline bool show_fov = false;
        inline float fov = 150.0f;
        inline float smoothness = 5.0f;
        inline int aim_key = VK_RBUTTON;
    }

    namespace visuals
    {
        inline bool box = false;
        inline bool skeleton = true;
        inline bool snaplines = false;
        inline bool distance = false;
        inline bool indication_arrows = false;
        inline bool name = false;
        inline bool VSync = false;


        inline bool filled = false;
        inline bool corner = false;
        inline bool box_3d = false;
        inline int box_type = 0;
        inline float thickness = 1.5f;
        inline float max_distance = 250.0f;

        // Colors
        inline Color visible_color = Color(0.0f, 1.0f, 0.0f, 1.0f);
        inline Color hidden_color = Color(1.0f, 0.0f, 0.0f, 1.0f);
        inline Color box_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        inline Color skeleton_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        inline Color snapline_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    namespace misc
    {
        // Misc settings
        inline bool ignore_team = true;
        inline bool stream_proof = false;
    }
}

// Hotkey status variables
inline bool keystatus = false;
inline bool keystatus2 = false;
inline bool ChangeKeyAim = false;
inline bool ChangeKeyTrig = false;