#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/Transform2D.h"
#include "lab_m1/Tema1/Object.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // start shape from left corner
    glm::vec3 corner = glm::vec3(0, 0, 0);
    square_side = 110;
    life_square = 70;
    diamond_size = 100;
    enemy_side = 80;
    i_enemy_side = 50;

    game_over = false;
    start = std::time(0);
    start_s = std::time(0);
    int_s = 0;
    p = 5;

    angularStep = 0;

    // attack stars' scale
    star_x = 7;
    star_y = 7;

    // cost stars' scale
    baby_star_x = 3;
    baby_star_y = 3;

    num_lives = 3;

    // Shape setup
    Mesh* green_square = Object::CreateSquare("square_g", corner, square_side, glm::vec3(0, 1, 0.5), true);
    AddMeshToList(green_square);

    Mesh* square = Object::CreateSquare("wall", corner, square_side, glm::vec3(1, 0, 0.1), true);
    AddMeshToList(square);

    Mesh* box = Object::CreateSquare("box", corner, square_side, glm::vec3(0, 0, 0));
    AddMeshToList(box);

    Mesh* life = Object::CreateHeart("life", corner, glm::vec3(1, 0, 0));
    AddMeshToList(life);

    // Flowers
    Mesh* dr = Object::CreateDiamond("red_diamond", corner, diamond_size, glm::vec3(1, 0.1, 0));
    AddMeshToList(dr);

    Mesh* db = Object::CreateDiamond("blue_diamond", corner, diamond_size, glm::vec3(0.1, 0, 1));
    AddMeshToList(db);

    Mesh* dy = Object::CreateDiamond("yellow_diamond", corner, diamond_size, glm::vec3(1, 0.7, 0));
    AddMeshToList(dy);

    Mesh* dm = Object::CreateDiamond("pink_diamond", corner, diamond_size, glm::vec3(0.5, 0, 0.5));
    AddMeshToList(dm);

    // Enemies
    Mesh* eb = Object::CreateEnemy("enemy_blue", corner, enemy_side, glm::vec3(0.1, 0, 1), 1);
    AddMeshToList(eb);

    Mesh* ieb = Object::CreateEnemy("i_enemy_blue", corner, i_enemy_side, glm::vec3(0.1, 1, 1), 2);
    AddMeshToList(ieb);

    Mesh* eo = Object::CreateEnemy("enemy_red", corner, enemy_side, glm::vec3(1, 0, 0), 1);
    AddMeshToList(eo);

    Mesh* ieo = Object::CreateEnemy("i_enemy_red", corner, i_enemy_side, glm::vec3(1, 0.5, 0), 2);
    AddMeshToList(ieo);

    Mesh* ep = Object::CreateEnemy("enemy_pink", corner, enemy_side, glm::vec3(0.5, 0, 0.5), 1);
    AddMeshToList(ep);

    Mesh* iep = Object::CreateEnemy("i_enemy_pink", corner, i_enemy_side, glm::vec3(0.8, 0, 0.8), 2);
    AddMeshToList(iep);

    Mesh* ey = Object::CreateEnemy("enemy_yellow", corner, enemy_side, glm::vec3(1, 0.7, 0), 1);
    AddMeshToList(ey);

    Mesh* iey = Object::CreateEnemy("i_enemy_yellow", corner, i_enemy_side, glm::vec3(1, 1, 0), 2);
    AddMeshToList(iey);

    // Stars
    Mesh *starb = Object::CreateStar("blue_star", corner, glm::vec3(0.1, 0, 1));
    AddMeshToList(starb);

    Mesh* staro = Object::CreateStar("red_star", corner, glm::vec3(1, 0, 0));
    AddMeshToList(staro);

    Mesh* stary = Object::CreateStar("yellow_star", corner, glm::vec3(1, 0.7, 0));
    AddMeshToList(stary);

    Mesh* starp = Object::CreateStar("pink_star", corner, glm::vec3(0.5, 0, 0.5));
    AddMeshToList(starp);

    Mesh *starg = Object::CreateStar("grey_star", corner, glm::vec3(0.5, 0.5, 0.5));
    AddMeshToList(starg);

    Mesh* star = Object::CreateStar("star", corner, glm::vec3(1, 0, 0.9));
    AddMeshToList(star);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    GameSetup();

    if (game_over) {
        exit(1);
    }

    // add random enemy every 4-8 seconds
    end = time(0);
    int r = rand() % 8 + 4;
    if (end - start >= r) {
        AddEnemy();
        start = end;
    }

    // add 3 stars at random moment
    end_s = time(0);
    if (end_s - start_s >= int_s) {
        AddStar();
        int_s = rand() % 7 + 4;
        start_s = end_s;
    }


    EnemySetup(deltaTimeSeconds);
    FlowerSetup(deltaTimeSeconds);
    StarSetup();
}

void Tema1::AddEnemy()
{
    float x = 1250;
    float y;
    int pos = rand() % 3 + 1;
    switch (pos) {
        case 1:
            y = 100;
            break;
        case 2:
            y = 240;
            break;
        case 3:
            y = 375;
            break;
    }

    int color = rand() % 4 + 1;
    switch (color) {
        case 1:
            enemies.push_back({ x, y, x + 18, y - 12, "blue", true, 0, pos, 1 });
            break;
        case 2:
            enemies.push_back({ x, y, x + 18, y - 12, "red", true, 0, pos, 1 });
            break;
        case 3:
            enemies.push_back({ x, y, x + 18, y - 12, "pink", true, 0, pos, 1 });
            break;
        case 4:
            enemies.push_back({ x, y, x + 18, y - 12, "yellow",true, 0, pos, 1 });
            break;
    }
}

void Tema1::EnemySetup(float deltaTimeSeconds)
{
    for (auto& enemy : enemies) {
        if (!enemy.alive) {
            continue;
        }

        // enemy destroyed, disappearing animation
        if (enemy.num_hits == 3) {
            enemy.scale -= deltaTimeSeconds;

            // outside hexagon
            modelMatrix = glm::mat3(1) * Transform2D::Translate(enemy.x, enemy.y);
            modelMatrix *= Transform2D::Rotate(30);
            modelMatrix *= Transform2D::Translate(enemy_side / 2, enemy_side / 2);
            modelMatrix *= Transform2D::Scale(enemy.scale, enemy.scale);
            modelMatrix *= Transform2D::Translate(-enemy_side / 2, -enemy_side / 2);

            RenderMesh2D(meshes["enemy_" + enemy.color], shaders["VertexColor"], modelMatrix);

            // inside hexagon
            modelMatrix = glm::mat3(1) * Transform2D::Translate(enemy.little_x, enemy.little_y);
            modelMatrix *= Transform2D::Rotate(30);
            modelMatrix *= Transform2D::Translate(i_enemy_side / 2, i_enemy_side / 2);
            modelMatrix *= Transform2D::Scale(enemy.scale, enemy.scale);
            modelMatrix *= Transform2D::Translate(-i_enemy_side / 2, -i_enemy_side / 2);
            RenderMesh2D(meshes["i_enemy_" + enemy.color], shaders["VertexColor"], modelMatrix);

            // enemy completely destroyed
            if (enemy.scale <= 0) {
                enemy.alive = false;
            }
            continue;
        }
        else {
            // outside hexagon
            modelMatrix = glm::mat3(1) * Transform2D::Translate(enemy.x, enemy.y);
            modelMatrix *= Transform2D::Rotate(30);
            RenderMesh2D(meshes["enemy_" + enemy.color], shaders["VertexColor"], modelMatrix);

            // inside hexagon (+18, -13)
            modelMatrix = glm::mat3(1) * Transform2D::Translate(enemy.little_x, enemy.little_y);
            modelMatrix *= Transform2D::Rotate(30);
            RenderMesh2D(meshes["i_enemy_" + enemy.color], shaders["VertexColor"], modelMatrix);

            // moving animation
            enemy.x -= deltaTimeSeconds * 100;
            enemy.little_x -= deltaTimeSeconds * 100;

            // enemy passed red rectangle
            if (enemy.x < 0) {
                if (enemy.alive) {
                    enemy.alive = false;
                    num_lives--;

                    if (num_lives == 0) {
                        game_over = 1;
                    }
                }
            }
        }
    }
}

void Tema1::FlowerSetup(float deltaTimeSeconds)
{
    for (auto& flower : flowers) {
        if (!flower.alive && flower.star_alive) {
            flower.star.sx += deltaTimeSeconds * 250;
            flower.star.angularStep += deltaTimeSeconds * 4;

            modelMatrix = glm::mat3(1) * Transform2D::Translate(flower.star.sx, flower.star.sy);
            modelMatrix *= Transform2D::Scale(star_x, star_y);
            // (18, 10.37) = star center - from geogebra
            modelMatrix *= Transform2D::Translate(18, 10.37);
            modelMatrix *= Transform2D::Rotate(-flower.star.angularStep);
            modelMatrix *= Transform2D::Translate(-18, -10.37);

            RenderMesh2D(meshes[flower.color + "_star"], shaders["VertexColor"], modelMatrix);

            continue;
        }
        if (!flower.alive) {
            continue;
        }
        // check time - send new star after 1 second
        flower.end = time(0);

        // flower destroyed - disappearing animation
        if (flower.bye) {
            flower.scale -= deltaTimeSeconds;

            modelMatrix = glm::mat3(1) * Transform2D::Translate(flower.x, flower.y);
            modelMatrix *= Transform2D::Translate(diamond_size / 2, diamond_size / 2);
            modelMatrix *= Transform2D::Scale(flower.scale, flower.scale);
            modelMatrix *= Transform2D::Translate(-diamond_size / 2, -diamond_size / 2);

            RenderMesh2D(meshes[flower.color + "_diamond"], shaders["VertexColor"], modelMatrix);

            // flower completely destroyed
            if (flower.scale <= 0) {
                flower.alive = false;
            }
            continue;
        }

        // flower
        modelMatrix = glm::mat3(1) * Transform2D::Translate(flower.x, flower.y);
        RenderMesh2D(meshes[flower.color + "_diamond"], shaders["VertexColor"], modelMatrix);

        // moving star animation
        if (flower.star_alive) {
            flower.star.sx += deltaTimeSeconds * 250;
            flower.star.angularStep += deltaTimeSeconds * 3;

            modelMatrix = glm::mat3(1) * Transform2D::Translate(flower.star.sx, flower.star.sy);
            modelMatrix *= Transform2D::Scale(star_x, star_y);
            // (18, 10.37) = star center - from geogebra
            modelMatrix *= Transform2D::Translate(18, 10.37);
            modelMatrix *= Transform2D::Rotate(-flower.star.angularStep);
            modelMatrix *= Transform2D::Translate(-18, -10.37);

            RenderMesh2D(meshes[flower.color + "_star"], shaders["VertexColor"], modelMatrix);
        }

        for (auto& enemy : enemies) {
            // enemy is still attacking
            if (enemy.alive && enemy.num_hits < 3) {
                if (enemy.row == flower.row) {
                    if (enemy.x < flower.square && enemy.x > (flower.square - 110)) {
                        // flower destroyed
                        flower.bye = true;
                    }
                    if (flower.star_alive && flower.color == enemy.color) {
                        // star attack
                        float sX = flower.star.sx + star_x * 6.1;
                        float sY = flower.star.sy + star_y * 6.1;
                        float sum = 45.0f + 6.1 * star_x * star_y / 2;
                        float eX = enemy.x + 45;
                        float eY = enemy.y + 45;
                        // collision
                        if (sum >= glm::distance(glm::vec2(sX, sY), glm::vec2(eX, eY))
                            && flower.x < enemy.x) {
                            // add a new star after 1 second
                            flower.start = time(0);
                            // star destroyed
                            flower.star_alive = false;
                            flower.star.sx = flower.x;
                            // enemy hit
                            enemy.num_hits++;

                            break;
                        }
                    }
                }
            }
        }

        // 1 second passed, a new star can be created
        if (!flower.star_alive && (flower.end - flower.start >= 1)) {
            for (auto& enemy : enemies) {
                if (flower.row == enemy.row && enemy.alive && enemy.num_hits < 3
                    && flower.color ==  enemy.color && enemy.x > flower.square) {
                    // enemy of same color on row, star can attack
                    flower.star_alive = true;
                    flower.star.sx = flower.x;
                }
            }
        }

    }
}

void Tema1::AddStar()
{
    glm::ivec2 resolution = window->GetResolution();
    int coordX = resolution.x - 2 * square_side;
    int coordY = resolution.y - 3 * square_side;
    
    for (int i = 0; i < 3; i++) {
        // random position for star
        float x = rand() % coordX;
        float y = rand() % coordY;

        for (auto& star : stars) {
            // cannot have 2 stars on same coordinates
            if (star.sx == x && star.sy == y) {
                i--;
                continue;
            }
        }
        stars.push_back({ x, y, true });
    }
}

void Tema1::StarSetup()
{
    for (auto& star : stars) {
        if (star.is_alive) {
            modelMatrix = glm::mat3(1) * Transform2D::Translate(star.sx, star.sy);
            modelMatrix *= Transform2D::Scale(star_x, star_y);
            RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Tema1::GameSetup()
{
    // Green Squares
    modelMatrix = glm::mat3(1) * Transform2D::Translate(100, 15);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(235, 15);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(370, 15);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(100, 150);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(235, 150);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(370, 150);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(100, 285);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(235, 285);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(370, 285);
    RenderMesh2D(meshes["square_g"], shaders["VertexColor"], modelMatrix);

    // Red Rectangle (wall for enemy -> -life)
    modelMatrix = glm::mat3(1) * Transform2D::Translate(15, 15);
    modelMatrix *= Transform2D::Scale(0.5, 3.45);
    RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);

    // Home for Diamonds (black boxes)
    // Box 1
    modelMatrix = glm::mat3(1) * Transform2D::Translate(40, 560);
    RenderMesh2D(meshes["box"], shaders["VertexColor"], modelMatrix);

    // Red diamond
    modelMatrix = glm::mat3(1) * Transform2D::Translate(40, 568);
    RenderMesh2D(meshes["red_diamond"], shaders["VertexColor"], modelMatrix);

    // Cost star
    modelMatrix = glm::mat3(1) * Transform2D::Translate(0, 510);
    modelMatrix *= Transform2D::Scale(baby_star_x, baby_star_y);
    RenderMesh2D(meshes["grey_star"], shaders["VertexColor"], modelMatrix);

    // Box 2
    modelMatrix = glm::mat3(1) * Transform2D::Translate(200, 560);
    RenderMesh2D(meshes["box"], shaders["VertexColor"], modelMatrix);

    // Cost stars
    modelMatrix = glm::mat3(1) * Transform2D::Translate(160, 510);
    modelMatrix *= Transform2D::Scale(baby_star_x, baby_star_y);
    RenderMesh2D(meshes["grey_star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(200, 510);
    modelMatrix *= Transform2D::Scale(baby_star_x, baby_star_y);
    RenderMesh2D(meshes["grey_star"], shaders["VertexColor"], modelMatrix);

    // Blue diamond
    modelMatrix = glm::mat3(1) * Transform2D::Translate(200, 568);
    RenderMesh2D(meshes["blue_diamond"], shaders["VertexColor"], modelMatrix);

    // Box 3
    modelMatrix = glm::mat3(1) * Transform2D::Translate(360, 560);
    RenderMesh2D(meshes["box"], shaders["VertexColor"], modelMatrix);

    // Cost stars
    modelMatrix = glm::mat3(1) * Transform2D::Translate(320, 510);
    modelMatrix *= Transform2D::Scale(baby_star_x, baby_star_y);
    RenderMesh2D(meshes["grey_star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1) * Transform2D::Translate(360, 510);
    modelMatrix *= Transform2D::Scale(baby_star_x, baby_star_y);
    RenderMesh2D(meshes["grey_star"], shaders["VertexColor"], modelMatrix);

    // Yellow diamond
    modelMatrix = glm::mat3(1) * Transform2D::Translate(360, 568);
    RenderMesh2D(meshes["yellow_diamond"], shaders["VertexColor"], modelMatrix);

    // Box 4
    modelMatrix = glm::mat3(1) * Transform2D::Translate(520, 560);
    RenderMesh2D(meshes["box"], shaders["VertexColor"], modelMatrix);

    // Cost stars
    for (float x = 480; x <= 600; x += 40) {
        modelMatrix = glm::mat3(1) * Transform2D::Translate(x, 510);
        modelMatrix *= Transform2D::Scale(baby_star_x, baby_star_y);
        RenderMesh2D(meshes["grey_star"], shaders["VertexColor"], modelMatrix);
    }

    // Magenta diamond
    modelMatrix = glm::mat3(1) * Transform2D::Translate(520, 568);
    RenderMesh2D(meshes["pink_diamond"], shaders["VertexColor"], modelMatrix);

    // Red Squares - lives
    for (int i = 0, l = 565; i < num_lives; i++, l += 90) {
        modelMatrix = glm::mat3(1) * Transform2D::Translate(l, 510) * Transform2D::Scale(17, 17);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

    // Cost stars
    for (float x = 670, i = 1; i <= p; x += 40, i++) {
        modelMatrix = glm::mat3(1) * Transform2D::Translate(x, 520);
        modelMatrix *= Transform2D::Scale(baby_star_x, baby_star_y);
        RenderMesh2D(meshes["grey_star"], shaders["VertexColor"], modelMatrix);
    }

    // Drag and Drop flower
    if (flower_d.click) {
        modelMatrix = glm::mat3(1) * Transform2D::Translate(flower_d.x, flower_d.y);
        RenderMesh2D(meshes[flower_d.color + "_diamond"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // move flower around screen while pressing left mouse btn
    if (flower_d.click) {
        flower_d.x = mouseX - diamond_size / 2;
        flower_d.y = window->GetResolution().y - mouseY - diamond_size / 2;
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    glm::ivec2 resolution = window->GetResolution();
    float y = resolution.y;

    // add a flower with drag and drop
    if (button == 1) {
        // drop flower
        float coordY = 568;

        if ((y - mouseY) > coordY && (y - mouseY) < coordY + square_side) {
            flower_d.y = y - mouseY - diamond_size / 2;

            if (mouseX > 40 && mouseX < 40 + square_side && p >= 1) {
                flower_d.x = mouseX - diamond_size / 2;
                flower_d.click = true;
                flower_d.color = "red";
            }
            else if (mouseX > 200 && mouseX < 200 + square_side && p >= 2) {
                flower_d.x = mouseX - diamond_size / 2;
                flower_d.click = true;
                flower_d.color = "blue";
            }
            else if (mouseX > 360 && mouseX < 360 + square_side && p >= 2) {
                flower_d.x = mouseX - diamond_size / 2;
                flower_d.click = true;
                flower_d.color = "yellow";
            }
            else if (mouseX > 520 && mouseX < 520 + square_side && p >= 4) {
                flower_d.x = mouseX - diamond_size / 2;
                flower_d.click = true;
                flower_d.color = "pink";
            }
        }

        // take star
        for (auto& star : stars) {
            if (star.is_alive) {
                if (mouseX > star.sx + 12 * star_x && mouseX < star.sx + 24 * star_x
                    && (y - mouseY) > star.sy + 6 * star_y && (y - mouseY) < star.sy + 16 * star_y) {
                    star.is_alive = false;
                    p++;
                    break;
                }
            }
        }
    }

    // remove flower
    if (button == 2) {
        float fx, fy;
        for (float coordX = 100; coordX <= 370; coordX += 135) {
            if (mouseX > coordX && mouseX < coordX + square_side) {
                fx = coordX;
                for (float coordY = 15; coordY <= 285; coordY += 135) {
                    if ((y - mouseY) > coordY && (y - mouseY) < coordY + square_side) {
                        fy = coordY;
                    }
                }
            }
        }

        for (auto& flower : flowers) {
            if (flower.alive && flower.x == fx && flower.y == fy) {
                flower.alive = false;
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // add flower to the game
    if (button == 1) {
       // no drag and drop
       if (!flower_d.click) {
            return;
       }
       flower_d.click = false;

       glm::ivec2 resolution = window->GetResolution();
       float y = resolution.y;

       float fx, fy;
       bool add = false;
       int row;
       int square;

       // check if the flower was placed on a valid square
       for (float coordX = 100; coordX <= 370; coordX += 135) {
            if (mouseX > coordX && mouseX < coordX + square_side) {
                fx = coordX;
                for (float coordY = 15, i = 1; coordY <= 285; coordY += 135, i++) {
                    if ((y - mouseY) > coordY && (y - mouseY) < coordY + square_side) {
                        fy = coordY;
                        row = i;
                        square = fx + 100;
                        add = true;
                    }
                }
            }
       }
 
       // valid position, flower can be added
       if (add) {
           // check if square is empty
           for (auto& flower : flowers) {
               if (flower.alive) {
                   if (fx == flower.x && fy == flower.y) {
                       return;
                   }
               }
           }

           float sy = fy - 20;
           star_t star = { fx, sy, 0 };
           flowers.push_back({ fx, fy, flower_d.color, true, false, false, star, row, square, 1, time(0), time(0) });

           if (flower_d.color == "red") {
               p -= 1;
           }
           else if (flower_d.color == "blue" || flower_d.color == "yellow") {
               p -= 2;
           }
           else if (flower_d.color == "pink") {
               p -= 4;
           }
       }

    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
