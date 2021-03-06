#include "Level3.h"
#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8

#define LEVEL3_ENEMY_COUNT 2

unsigned int level3_data[] = {
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    3, 2, 2, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 2
};

void Level3::Initialize() {
    ohno = Mix_LoadWAV("ohno.mp3");
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
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

    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("fish.png");
    state.enemies[0].textureID = enemyTextureID;

    state.enemies[0].position = glm::vec3(10, -5, 0);
    state.enemies[0].acceleration = glm::vec3(0, -1.81f, 0);
    
    state.enemies[0].height = 1.0f;
    state.enemies[0].width = 0.5f;
    state.enemies[0].height = 0.5f;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].aiState = JUMPING;
    state.enemies[0].isActive = true;
    
    
    GLuint enemyTextureID2 = Util::LoadTexture("tree.png");
    state.enemies[1].textureID = enemyTextureID2;

    state.enemies[1].position = glm::vec3(13, -5, 0);
    
    state.enemies[1].height = 1.0f;
    state.enemies[1].width = 0.5f;
    state.enemies[1].height = 0.5f;
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].isActive = true;
    
    state.font = new Entity();
    state.font->textureID = Util::LoadTexture("pixel_font.png");
    state.font->position = glm::vec3(1,-1,0);
    

}
    
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    
    state.enemies[0].Update(deltaTime, state.player, &state.enemies[0], LEVEL3_ENEMY_COUNT, state.map);
    state.enemies[1].Update(deltaTime, state.player, &state.enemies[0], LEVEL3_ENEMY_COUNT, state.map);
    
    state.player->CheckCollisionsY(&state.enemies[0], 1);
    state.player->CheckCollisionsX(&state.enemies[0], 1);
    

    
    if (state.player->position.x >= 12){
        state.nextScene = 4;
    }
}

void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    
    if (state.nextScene == 4){
        Util::DrawText(program, state.font->textureID, "You Win :>", 0.5f, -0.05f, glm::vec3(8,-1,0));
        endOfGame = true;
    }
    if (live == 0) {
        Util::DrawText(program, state.font->textureID, "You Lose :<", 0.5f, -0.05f, state.font->position);
        endOfGame = true;
    }
     else {
         Util::DrawText(program, state.font->textureID, "Live: " + std::to_string(live), 0.5f, -0.05f, state.font->position);
     }
    
    state.enemies[0].Render(program);
    state.enemies[1].Render(program);
    state.player->Render(program);
    
    
    if (state.player->collidedLeft == true || state.player->collidedRight == true || state.player->collidedTop == true){
            Mix_PlayChannel(-1, ohno, 0);
            live--;
            state.player->position = glm::vec3(5, 0, 0);
//        endOfGame = true;
    }
    
    
}






