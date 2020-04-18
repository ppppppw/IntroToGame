#include "Start.h"
#define Start_WIDTH 14
#define Start_HEIGHT 8


unsigned int start_data[] = {
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3
};

void Start::Initialize() {
    live = 3;
    
    state.nextScene = -2;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(Start_WIDTH, Start_HEIGHT, start_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("ac.png");
    
    state.player->height = 0.9f;
    state.player->width = 0.8f;
    
    state.player->jumpPower = 5.0f;


    
    state.font = new Entity();
    state.font->textureID = Util::LoadTexture("pixel_font.png");
    state.font->position = glm::vec3(1,-1,0);
    

}
    
void Start::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, 0, state.map);
}

void Start::Render(ShaderProgram *program) {
    state.map->Render(program);
    Util::DrawText(program, state.font->textureID, "PLATFORMER!", 0.6f, -0.05f, state.font->position);
    Util::DrawText(program, state.font->textureID, "press enter to start", 0.25f, -0.05f, glm::vec3(1,-3,0));
    state.player->Render(program);
    
    
}
