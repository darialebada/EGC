#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        struct enemy_t
        {
            float x;
            float y;
            float little_x;
            float little_y;
            std::string color;
            bool alive;
            int num_hits;
            int row;
            float scale;
        };

        struct star_t
        {
            float sx;
            float sy;
            float angularStep;
        };

        struct star_cost_t
        {
            float sx;
            float sy;
            bool is_alive;
        };

        struct flower_t
        {
            float x;
            float y;
            std::string color;
            bool alive;
            bool star_alive;
            bool bye;
            star_t star;
            int row;
            int square;
            float scale;
            time_t start;
            time_t end;
        };

        struct flower_pick
        {
            float x;
            float y;
            std::string color;
            bool click;
        };

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        // My functions
        void GameSetup();
        void EnemySetup(float deltaTimeSeconds);
        void FlowerSetup(float deltaTimeSeconds);
        void StarSetup();
        void AddStar();
        void AddEnemy();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat3 modelMatrix;
        float angularStep;

        // sizes for different shapes
        float diamond_size, enemy_side, i_enemy_side, life_square, square_side;
        float star_x, star_y;
        float baby_star_x, baby_star_y;

        // game logic variables
        bool game_over;
        int num_lives;
        int p;

        // time variables
        time_t start, end;
        time_t start_s, end_s;
        // when stars appear
        int int_s;

        // diamond for drag and drop
        flower_pick flower_d;

        std::vector<enemy_t> enemies;
        std::vector<flower_t> flowers;
        std::vector<star_cost_t> stars;
    };
}   // namespace m1
