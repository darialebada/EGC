#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about FrameStart, Update, FrameEnd
 *  and the order in which they are called, see world.cpp.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "moon.jpg").c_str(), GL_REPEAT);
        mapTextures["moon"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass.jpg").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "water5.jpg").c_str(), GL_REPEAT);
        mapTextures["water"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wall.jpg").c_str(), GL_REPEAT);
        mapTextures["wall"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "bamboo.jpg").c_str(), GL_REPEAT);
        mapTextures["bamboo"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "sky.png").c_str());
        mapTextures["sky"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "white.png").c_str(), GL_REPEAT);
        mapTextures["white"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "nirvana.png").c_str());
        mapTextures["nirvana"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pirate.png").c_str());
        mapTextures["pirate"] = texture;
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LightShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("TextureShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "TextureVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "TextureFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        lightPosition = glm::vec3(-9, 3, -10);
        lightDirection = glm::vec3(0, -1, 0);
        lightColor = glm::vec3(0.5f, 0.5f, 0.5f);
        materialShininess = 20;
        materialKd = 0.5;
        materialKs = 1;
    }
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    // water
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderSimpleMesh(meshes["plane"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["water"]);

    }

    // sky
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, -20));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.7f, 1));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["plane"], shaders["LightShader"], modelMatrix, 1, glm::vec3(1), mapTextures["sky"]);

    }

    // moon
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f));
        RenderSimpleMesh(meshes["sphere"], shaders["TextureShader"], modelMatrix, 1, glm::vec3(1), mapTextures["moon"]);
    }

    // island
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, -1.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, 1));
        RenderSimpleMesh(meshes["sphere"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["grass"]);
    }

    // lighthouse
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, -1.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 1.5f, 0.4f));
        RenderSimpleMesh(meshes["cylinder"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["wall"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.65f, -1.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.2f, 0.5f));
        RenderSimpleMesh(meshes["cylinder"], shaders["TextureShader"], modelMatrix, 1, glm::vec3(1), mapTextures["white"]);
    }

    // boat 1
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 0.5f, -0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.2f, 0.4f));
        RenderSimpleMesh(meshes["box"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["bamboo"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 0.5f, -0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.4f, 0.05f));
        RenderSimpleMesh(meshes["cylinder"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["wall"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.975f, 0.9f, -0.7f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.15f, 0.25f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["quad"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["pirate"]);
    }


    // boat 2
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0.5f, -0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.2f, 0.4f));
        RenderSimpleMesh(meshes["box"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["bamboo"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0.5f, -0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.4f, 0.05f));
        RenderSimpleMesh(meshes["cylinder"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["wall"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.975f, 0.9f, -0.7f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.15f, 0.25f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["quad"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["nirvana"]);
    }

    // boat 3
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 0.5f, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.2f, 0.4f));
        RenderSimpleMesh(meshes["box"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["bamboo"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 0.5f, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.4f, 0.05f));
        RenderSimpleMesh(meshes["cylinder"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["wall"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.975f, 0.9f, 0.3f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.15f, 0.25f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["quad"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["nirvana"]);
    }


    // boat 4
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0.5f, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.2f, 0.4f));
        RenderSimpleMesh(meshes["box"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["bamboo"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0.5f, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.4f, 0.05f));
        RenderSimpleMesh(meshes["cylinder"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["wall"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.975f, 0.9f, 0.3f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.15f, 0.25f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["quad"], shaders["LightShader"], modelMatrix, 0, glm::vec3(1), mapTextures["pirate"]);
    }



}


void Tema3::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int lightType, const glm::vec3& color, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // TODO(student): Set any other shader uniforms that you need
    GLint loc_type = glGetUniformLocation(shader->program, "light_type");
    glUniform1i(loc_type, lightType);

    int light_color = glGetUniformLocation(shader->program, "light_color");
    glUniform3f(light_color, lightColor.x, lightColor.y, lightColor.z);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    // texture
    float time_elapsed = Engine::GetElapsedTime();
    int loc_time_elapsed = glGetUniformLocation(shader->program, "time_elapsed");
    glUniform1f(loc_time_elapsed, time_elapsed);

    int rotate_texture = 0;
    if (texture->GetTextureID() == mapTextures["water"]->GetTextureID()) {
        rotate_texture = 1;
    }
    int loc_rotate_texture = glGetUniformLocation(shader->program, "rotate_texture");
    glUniform1i(loc_rotate_texture, rotate_texture);

    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see input_controller.h.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
  
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}