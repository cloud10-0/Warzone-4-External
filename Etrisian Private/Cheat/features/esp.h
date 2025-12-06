#pragma once
#include "../../Util/engine.h"

inline void ActorLoop() {
    auto actors = engine::get_actor_list(globals::g_baseAddress, 0);

    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    for (auto& actor : actors) {
        if (!actor.is_valid)
            continue;

        
        Vector2 top = actor.bones_screen[0];     
        Vector2 bottom = actor.bones_screen[13]; 
        float height = fabs(bottom.y - top.y);
        float width = height / 2.0f;

        draw_list->AddRect(
            ImVec2(top.x - width / 2, top.y),
            ImVec2(top.x + width / 2, bottom.y),
            IM_COL32(255, 0, 0, 255),
            0.0f, 0, settings::visuals::thickness
        );

        // Skeleton
        if (settings::visuals::skeleton) {
            for (size_t i = 0; i < actor.bones_screen.size() - 1; i++) {
                draw_list->AddLine(
                    ImVec2(actor.bones_screen[i].x, actor.bones_screen[i].y),
                    ImVec2(actor.bones_screen[i + 1].x, actor.bones_screen[i + 1].y),
                    IM_COL32(0, 255, 0, 255),
                    1.0f
                );
            }
        }

        // Distance
        if (settings::visuals::distance) {
            auto dist = actor.position.distance_to(engine::player::get_position(globals::g_baseAddress));
            char buf[32];
            sprintf_s(buf, "%.1fm", dist);
            draw_list->AddText(
                ImVec2(top.x, top.y - 15),
                IM_COL32(255, 255, 255, 255),
                buf
            );
        }
    }
}
