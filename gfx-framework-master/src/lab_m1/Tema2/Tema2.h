#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera_tema.h"
#include "lab_m1/Tema2/Tank.h"
#include "lab_m1/Tema2/Transform3D.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

        struct enemy_t {
            float x;
            float y;
            float z;
            // rotate full tank
            float radians_rotation;
            // check if tank is rotating
            bool rotate;
            // direction of movement
            int sign;
            // colors
            glm::vec3 color_body;
            glm::vec3 color_turret;
            glm::vec3 color_cannon;
            glm::vec3 color_tracks;
            // where to turn around
            int route;
            float num_hits;
            // type == 1 -> tank moves up-down
            // type == 2 -> tank moves left-right
            int type;
        };

        struct building_t {
			float x;
			float y;
			float z;
            glm::vec3 scale;
			glm::vec3 color;
			float num_hits;
            bool falling;
		};

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void CreateEnemies();
        void CreateBuildings();
        void SetupTank();
        void SetupScene(float deltaTimeSeconds);
        void SetupEnemies(float deltaTimeSeconds);
        void SetupProjectiles(float deltaTimeSeconds);

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float num_hits);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implement::Camera* camera;
        glm::mat4 projectionMatrix;
        Tank* tank;
        std::vector<enemy_t> enemies;
        time_t start;
        bool freeze;
        float r_tank;
        bool game_over;

        int num_kills;
        int b_hit;
        float speed;
        float enemy_speed;
        float zNear, zFar;
        float ground_side;
        float move_away;

        std::vector<building_t> buildings;
    };
}   // namespace m1
