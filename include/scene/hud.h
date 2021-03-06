#pragma once
#include <deque>

#include "scene/scene.h"
#include "draw/sprite.h"
#include "draw/font.h"
#include "world/player.h"

namespace ace { namespace scene {
    struct Message {
        std::string message;
        glm::vec3 color;
    };

    struct HUD;

    struct MapDisplay {
        MapDisplay(HUD &hud);

        void update(double dt);
        void draw();

        HUD &hud;
        draw::SpriteGroup *marker, *map;
        draw::Sprite big, mini;
        bool big_open{false};
    private:
        void draw_map_grid(glm::vec2 offset) const;
    };

    struct HUD {
        HUD(GameScene &s);

        void update(double dt);
        void draw();

        void on_key(SDL_Scancode scancode, int modifiers, bool pressed);
        void on_mouse_button(int button, bool pressed);
        void on_mouse_motion(int x, int y, int dx, int dy);

        bool on_text_typing(const std::string &text);
        void on_text_finished(bool cancelled);

        void on_window_resize(int ow, int oh);

        void add_chat_message(std::string message, glm::vec3 color);
        void add_killfeed_message(const world::DrawPlayer &killer, const world::DrawPlayer &victim, net::KILL kill_type);
        void set_big_message(std::string message);
        
        void set_hit(glm::vec3 source);

        void update_weapon(const std::string &sight);
        void update_tool(const std::string &ammo_icon);

        GameScene &scene;

        draw::SpriteManager &sprites;

        draw::Sprite reticle, pal, palret, hit_indicator, weapon_sight, ammo_icon;
        glm::mat4 projection;

        MapDisplay map_display;
        
        world::DrawPlayer ply;

        int color_index = 0;
        float respawn_time = 0.0f, big_message_time = 0.0f;

        glm::vec3 last_hit;
    private:
        enum class State {
            None,
            ChangeTeam,
            ChangeWeapon,
            Exit
        } state{ State::None };

        void update_color(SDL_Scancode key);

        void draw_chat();
        void draw_scoreboard();

        net::CHAT cur_chat_type{ net::CHAT::INVALID };
        draw::Font *sys48, *sys13, *sys15, *sys18;
        std::deque<Message> chat_messages, killfeed;
        std::string big_message;

        friend MapDisplay;
    };
}}
