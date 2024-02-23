#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new implement::Camera();

    // tank's radius
    r_tank = 0.6f;
    // tank's speed
    speed = 7.5f;
    // enemy tank's speed
    enemy_speed = 4.5f;
    // coordinates for projection matrix
    zNear = 0.01f;
    zFar = 200.0f;
    // size for ground
    ground_side = 200.0f;
    // distance away from bulding after collision
    move_away = 5.0f;
    // start time of game
    start = time(0);
    // freeze <-> game over
    freeze = false;
    num_kills = 0;
    b_hit = 0;
    game_over = false;

    // init player's tank
    tank = new Tank();
    tank->Init();

    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tureta");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "tureta.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senila1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "senila1.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senila2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "senila2.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("proiectil");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "proiectil.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plan");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("building");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("TankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("DeformShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TurretShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // create vector of structs of enemy tanks
    CreateEnemies();

    // create vector of structs of buildings
    CreateBuildings();

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::CreateEnemies()
{
    // up-down
    enemies.push_back({ 40, -0.05f, 40, 0, false, -1, glm::vec3(0.55f, 0.3f, 0.7f), glm::vec3(0.65f, 0.4f, 0.6f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.5f, 0.5f, 0.5f), 40, 0, 1});
    enemies.push_back({ -40, -0.05f, -40, 180, false, 1, glm::vec3(0.6f, 0.1f, 0.6f), glm::vec3(0.75f, 0.3f, 0.75f), glm::vec3(0.5f, 0.54f, 5), glm::vec3(0.5f, 0.5f, 0.5f), -40, 0, 1 });
    enemies.push_back({ -20, -0.05f, -20, 180, false, 1, glm::vec3(0.3f, 0.1f, 0.3f), glm::vec3(0.8f, 0.2f, 0.8f), glm::vec3(0.7f, 0.74f, 7), glm::vec3(0.7f, 0.7f, 0.7f), -20, 0, 1 });
    enemies.push_back({ 10, -0.05f, -6, 180, false, 1, glm::vec3(0.15f, 0.1f, 0), glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.5f, 0.5f, 0.5f), -6, 0, 1 });
    // left-right
    enemies.push_back({ 30, -0.05f, -30, 90, false, -1, glm::vec3(0.3f, 0, 0.6f), glm::vec3(0.4f, 0, 0.6f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.5f, 0.5f, 0.5f), 30, 0, 2 });
    enemies.push_back({ -50, -0.05f, -50, -90, false, 1, glm::vec3(0.4f, 0, 0.5f), glm::vec3(0.45f, 0, 0.3f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.5f, 0.5f, 0.5f), 50, 0, 2 });
    enemies.push_back({ 60, -0.05f, -60, 90, false, -1, glm::vec3(0.4f, 0, 1), glm::vec3(0.5f, 0, 0.8f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.5f, 0.5f, 0.5f), 60, 0, 2 });
    enemies.push_back({ -30, -0.05f, 30, -90, false, 1, glm::vec3(0.45f, 0.2f, 0.5f), glm::vec3(0.55f, 0.3f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(0.5f, 0.5f, 0.5f), 30, 0, 2 });
}

void Tema2::CreateBuildings()
{
    // randomize 15 buldings on map
    for (int i = 0; i < 15; i++) {
        int sign = rand() % 2;
        if (sign == 0) {
			sign = -1;
        }
        else {
            sign = 1;
        }

        // randomize coordinates
        float rx = rand() % 50;
        rx *= sign;
        float rz = rand() % 60 + 20;
        rz = -rz;

        // randomize scale
        float sx = rand() % 8 + 5;
        float sy = rand() % 14 + 8;
        float sz = rand() % 8 + 4;

        // add buillding to vector
        buildings.push_back( { rx, tank->y, rz, glm::vec3(sx, sy, sz), glm::vec3(0.6f, 0.6f, 0.6f), 0, false} );
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    time_t t = time(0);
    // game over after 2min
    if (!game_over && (t - start == 120 || tank->num_hits == 3)) {
        cout << "Game over!" << endl;
        cout << "You survived " << t - start << " seconds!" << endl;
        cout << "You killed " << num_kills << " enemies!" << endl;
        cout << "You destroyed " << b_hit << " buildings!" << endl;
        cout << "You finished with " << num_kills * 10 + b_hit * 5 << " points!" << endl;
        freeze = true;
        game_over = true;
    }

    SetupTank();
    SetupScene(deltaTimeSeconds);
    SetupEnemies(deltaTimeSeconds);
    SetupProjectiles(deltaTimeSeconds);
}

void Tema2::SetupTank()
{
    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(tank->x, tank->y, tank->z);
        modelMatrix *= Transform3D::RotateOY(tank->radians_rotation);

        RenderSimpleMesh(meshes["corp"], shaders["TankShader"], modelMatrix, tank->color_body, tank->num_hits);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(tank->x, tank->y, tank->z);
        modelMatrix *= Transform3D::RotateOY(tank->radians_rotation);
        modelMatrix *= Transform3D::RotateOY(tank->radians_turret);

        RenderSimpleMesh(meshes["tureta"], shaders["TankShader"], modelMatrix, tank->color_cannon, tank->num_hits);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(tank->x, tank->y, tank->z);
        modelMatrix *= Transform3D::RotateOY(tank->radians_rotation);
        modelMatrix *= Transform3D::RotateOY(tank->radians_turret);

        RenderSimpleMesh(meshes["turela"], shaders["DeformShader"], modelMatrix, tank->color_turret, tank->num_hits);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(tank->x, tank->y, tank->z);
        modelMatrix *= Transform3D::RotateOY(tank->radians_rotation);

        RenderSimpleMesh(meshes["senila1"], shaders["TankShader"], modelMatrix, tank->color_tracks, tank->num_hits);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(tank->x, tank->y, tank->z);
        modelMatrix *= Transform3D::RotateOY(tank->radians_rotation);

        RenderSimpleMesh(meshes["senila2"], shaders["TankShader"], modelMatrix, tank->color_tracks, tank->num_hits);
    }

    // check collisions with buildings
    for (building_t& b : buildings) {
        if (b.falling) {
			// building already falling
			continue;
		}
        if (b.x - b.scale.x / 2 <= tank->x && tank->x <= b.x + b.scale.x / 2 && b.z - b.scale.z / 2 <= tank->z && tank->z <= b.z + b.scale.z / 2) {
			// building hit by tank -> we'll predent it was fast enought to destroy it
            // building down
            b.num_hits = 3;
            b.falling = true;
            b_hit++;
            
            // tank hit
            tank->num_hits++;
            // tank gets slower
            speed -= 1.0f;

            // move tank away from building
            camera->MoveForward(-move_away);
            tank->z += move_away * cos(tank->radians_rotation);
            tank->x += move_away * sin(tank->radians_rotation);
		}
	}

    // check collisions with enemy tanks
 //   for (enemy_t& enemy : enemies) {
 //       if (enemy.num_hits >= 3) {
	//		// enemy already dead
	//		continue;
	//	}
 //       if (enemy.x - r_tank <= tank->x && tank->x <= enemy.x + r_tank && enemy.z - r_tank <= tank->z && tank->z <= enemy.z + r_tank) {
	//		// enemy hit by tank
	//		enemy.num_hits++;
	//		// tank hit
	//		tank->num_hits++;
	//		// tank gets slower
	//		speed -= 1.0f;

	//		// move tank away from enemy
 //           enemy.x += move_away * (enemy.x - tank->x) * cos(enemy.radians_rotation);
 //           enemy.z += move_away * (enemy.z - tank->z) * sin(enemy.radians_rotation);
	//	}
	//}

}

void Tema2::SetupScene(float deltaTimeSeconds)
{
    // ground
    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(0, -0.1f, 0);
        modelMatrix *= Transform3D::Scale(ground_side, 1, ground_side);

        RenderSimpleMesh(meshes["plan"], shaders["TankShader"], modelMatrix, glm::vec3(0.35f, 0.45f, 0.30f), 0);
    }

    // sky - front
    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(0, -0.1f, -ground_side / 2);
        modelMatrix *= Transform3D::RotateOX(RADIANS(90));
        modelMatrix *= Transform3D::Scale(ground_side, 1, ground_side);

        RenderSimpleMesh(meshes["plan"], shaders["TankShader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.7f), 0);
    }

    // sky - right
    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(ground_side / 2, -0.1f, -ground_side / 2);
        modelMatrix *= Transform3D::RotateOX(RADIANS(90));
        modelMatrix *= Transform3D::RotateOZ(RADIANS(90));
        modelMatrix *= Transform3D::Scale(ground_side, 1, ground_side);

        RenderSimpleMesh(meshes["plan"], shaders["TankShader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.7f), 0);
    }

    // sky - left
    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(-ground_side / 2, -0.1f, ground_side / 2);
        modelMatrix *= Transform3D::RotateOX(RADIANS(90));
        modelMatrix *= Transform3D::RotateOZ(RADIANS(90));
        modelMatrix *= Transform3D::Scale(ground_side, 1, ground_side);

        RenderSimpleMesh(meshes["plan"], shaders["TankShader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.7f), 0);
    }

    // sky - back
    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(0, -0.1f, ground_side / 2);
        modelMatrix *= Transform3D::RotateOX(RADIANS(90));
        modelMatrix *= Transform3D::Scale(ground_side, 1, ground_side);

        RenderSimpleMesh(meshes["plan"], shaders["TankShader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.7f), 0);
    }

    // sky - up
    {
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(0, ground_side / 2, 0);
        modelMatrix *= Transform3D::Scale(ground_side, 1, ground_side);

        RenderSimpleMesh(meshes["plan"], shaders["TankShader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.7f), 0);
    }

    // buildings
    for (auto& b : buildings) {
        if (b.falling) {
            b.y -= deltaTimeSeconds;
        }

        if (b.y <= -b.scale.y) {
            b_hit++;
            // delete building after falling
            for (auto i = buildings.begin(); i != buildings.end(); ++i) {
                if ((*i).falling && (*i).y == b.y) {
                    buildings.erase(i);
                    break;
                }
            }
		}

        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(b.x, b.y, b.z);
        modelMatrix *= Transform3D::Scale(b.scale.x, b.scale.y, b.scale.z);

        RenderSimpleMesh(meshes["building"], shaders["DeformShader"], modelMatrix, b.color, b.num_hits);
    }
}

void Tema2::SetupEnemies(float deltaTimeSeconds)
{
    for (auto& enemy : enemies) {
        // place enemy tank
        glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(enemy.x, enemy.y, enemy.z);
        modelMatrix *= Transform3D::RotateOY(RADIANS(enemy.radians_rotation));
        RenderSimpleMesh(meshes["corp"], shaders["TankShader"], modelMatrix, enemy.color_body, enemy.num_hits);

        modelMatrix = glm::mat4(1) * Transform3D::Translate(enemy.x, enemy.y, enemy.z);
        modelMatrix *= Transform3D::RotateOY(RADIANS(enemy.radians_rotation));
        RenderSimpleMesh(meshes["tureta"], shaders["TankShader"], modelMatrix, enemy.color_cannon, enemy.num_hits);

        modelMatrix = glm::mat4(1) * Transform3D::Translate(enemy.x, enemy.y, enemy.z);
        modelMatrix *= Transform3D::RotateOY(RADIANS(enemy.radians_rotation));
        RenderSimpleMesh(meshes["turela"], shaders["DeformShader"], modelMatrix, enemy.color_turret, enemy.num_hits);

        modelMatrix = glm::mat4(1) * Transform3D::Translate(enemy.x, enemy.y, enemy.z);
        modelMatrix *= Transform3D::RotateOY(RADIANS(enemy.radians_rotation));
        RenderSimpleMesh(meshes["senila1"], shaders["TankShader"], modelMatrix, enemy.color_tracks, enemy.num_hits);

        modelMatrix = glm::mat4(1) * Transform3D::Translate(enemy.x, enemy.y, enemy.z);
        modelMatrix *= Transform3D::RotateOY(RADIANS(enemy.radians_rotation));
        RenderSimpleMesh(meshes["senila2"], shaders["TankShader"], modelMatrix, enemy.color_tracks, enemy.num_hits);

        // ------------------------ ANIMATION -----------------------------------------------
        if (enemy.num_hits >= 3) {
            // delete tank
            continue;
        }

        float speed_e = enemy_speed - enemy.num_hits;

        if (enemy.type == 1) {
            if (!enemy.rotate) {
                // enemy moves in straight line
                enemy.z += enemy.sign * speed_e * deltaTimeSeconds;

                if (enemy.z >= abs(enemy.route) || enemy.z <= -abs(enemy.route)) {
                    // imaginary line over, enemy starts rotating at 180 degrees
                    enemy.rotate = true;
                }
            }
            else {
                // enemy rotates 180 degrees
                enemy.radians_rotation += speed_e * 0.1f;

                // rotation done, enemy moves in straght line again
                if (enemy.z >= abs(enemy.route)) {
                    // enemy moves opposite to camera
                    enemy.sign = -1;
                    if (RADIANS(enemy.radians_rotation) >= RADIANS(360)) {
                        enemy.rotate = false;
                        enemy.radians_rotation = 0;
                    }
                }

                if (enemy.z <= -abs(enemy.route)) {
                    // enemy moves towards camera
                    enemy.sign = 1;
                    if (RADIANS(enemy.radians_rotation) >= RADIANS(180)) {
                        enemy.rotate = false;
                        enemy.radians_rotation = 180;
                    }
                }
            }
        }
        else if (enemy.type == 2) {
            if (!enemy.rotate) {
                // enemy moves in straight line
                enemy.x += enemy.sign * speed_e * deltaTimeSeconds;

                if (enemy.x >= abs(enemy.route) || enemy.x <= -abs(enemy.route)) {
                    // imaginary line over, enemy starts rotating at 180 degrees
                    enemy.rotate = true;
                }
            }
            else {
                // enemy rotates 180 degrees
                enemy.radians_rotation += speed_e * 0.1f;

                // rotation done, enemy moves in staright line again
                if (enemy.x >= abs(enemy.route)) {
                    // enemy moves to the left
                    enemy.sign = -1;
                    if (RADIANS(enemy.radians_rotation) >= RADIANS(90)) {
                        enemy.rotate = false;
                        enemy.radians_rotation = 90;
                    }
                }

                if (enemy.x <= -abs(enemy.route)) {
                    // enemy moves to the right
                    enemy.sign = 1;
                    if (RADIANS(enemy.radians_rotation) >= RADIANS(270)) {
                        enemy.rotate = false;
                        enemy.radians_rotation = -90;
                    }
                }
            }
        }   
    }
}

void Tema2::SetupProjectiles(float deltaTimeSeconds)
{
    float firing_speed = 10.0f;
    // height for projectile to go down
    float h = 0.2f;
    for (auto& p : tank->projectiles) {
        // coordinate calculated to touch the ground
        if (p.y >= -0.75f + tank->y && !p.hit) {
            glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(p.x, p.y, p.z);
            modelMatrix *= Transform3D::RotateOY(p.radians + p.radians_turret);

            RenderSimpleMesh(meshes["proiectil"], shaders["TankShader"], modelMatrix, p.color, 0);

            // move projectile straight
            p.z -= firing_speed * deltaTimeSeconds * cos(p.radians + p.radians_turret);
            p.x -= firing_speed * deltaTimeSeconds * sin(p.radians + p.radians_turret);
            // move projectile down
            p.y -= h * deltaTimeSeconds;

            // search for enemy tank to hit (collision)
            for (auto& enemy : enemies) {
                if (enemy.x - r_tank <= p.x && p.x <= enemy.x + r_tank && enemy.z - r_tank <= p.z && p.z <= enemy.z + r_tank) {
                    // tank hit
					p.hit = true;
                    if (enemy.num_hits < 3) {
                        enemy.num_hits++;

                        if (enemy.num_hits == 3) {
                            // enemy killed
							num_kills++;
                            enemy.y -= 0.05f;
						}
                    }
					
                    // delete projectile after collision
                    for (auto i = tank->projectiles.begin(); i != tank->projectiles.end(); ++i) {
                        if ((*i).hit) {
                            tank->projectiles.erase(i);
                            break;
                        }
                    }
				}
            }

            // search for bulding to hit
            for (auto& b : buildings) {
                if (b.falling) {
                    // building already falling
                    continue;
                }
                if (b.x - b.scale.x / 2 <= p.x && p.x <= b.x + b.scale.x / 2 && b.z - b.scale.z / 2 <= p.z && p.z <= b.z + b.scale.z / 2) {
					// building hit
					p.hit = true;
                    if (b.num_hits < 3) {
						b.num_hits++;

                        if (b.num_hits == 3) {
							// building destroyed - start falling
							b.falling = true;
						}
					}

					// delete projectile after collision
                    for (auto i = tank->projectiles.begin(); i != tank->projectiles.end(); ++i) {
                        if ((*i).hit) {
							tank->projectiles.erase(i);
							break;
						}
					}
				}   
            }
        }
        else if (p.y <= -0.75f + tank->y) {
            // after 12s from lauch delete projectile
            time_t t = time(0);
            if (t - p.start_time >= 12) {
                // delete projectile
                tank->projectiles.erase(tank->projectiles.begin());
            }
            else {
                // place projectile on scene
                glm::mat4 modelMatrix = glm::mat4(1) * Transform3D::Translate(p.x, p.y, p.z);
                modelMatrix *= Transform3D::RotateOY(p.radians + p.radians_turret);

                RenderSimpleMesh(meshes["proiectil"], shaders["TankShader"], modelMatrix, p.color, 0);
            }
        }
    }
}

void Tema2::FrameEnd()
{
   // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float hp)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    GLint obj_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(obj_color, 1, glm::value_ptr(color));

    // send damage to shader
    float hp_damage = glGetUniformLocation(shader->program, "object_damage");
    glUniform1f(hp_damage, hp);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move camera for debug reasons
   /* if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->TranslateForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->TranslateForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }*/

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivity = 0.1f;

        // game over, player can't move
        if (freeze) {
            return;
        }

        // move tank with camera following behind
        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(speed * deltaTime);
            tank->z -= speed * deltaTime * cos(tank->radians_rotation);
            tank->x -= speed * deltaTime * sin(tank->radians_rotation);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-speed * deltaTime);
            tank->z += speed * deltaTime * cos(tank->radians_rotation);
            tank->x += speed * deltaTime * sin(tank->radians_rotation);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            tank->radians_rotation += speed * deltaTime * sensivity;
            camera->RotateThirdPerson_OY(speed * deltaTime * sensivity);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            tank->radians_rotation -= speed * deltaTime * sensivity;
            camera->RotateThirdPerson_OY(-speed * deltaTime * sensivity);
        }

    }
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivity = 0.002f;
        // rotate turret after how much the mouse moved on Ox axis since last frame
        tank->radians_turret -= sensivity * deltaX;
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            // Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            // Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // start firing projectiles
    if (button == 1) {
        time_t t = time(0);
        // fire after at least 1s
        if (t - tank->start_firing > 1) {
            if (!freeze) {
                // add projectile to vector
                tank->projectiles.push_back({ tank->x, tank->y, tank->z, tank->radians_rotation, tank->radians_turret, false, time(0), glm::vec3(0, 0, 0) });
                tank->start_firing = time(0);
            }
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
