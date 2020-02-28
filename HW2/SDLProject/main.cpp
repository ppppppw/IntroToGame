/*
 CS3113
 Assignment 2: Pong
 Peiwen Tang (pt1145)
 02/28/2020
 */
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool endOfGame = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, modelMatrix2, modelMatrix3, projectionMatrix;

float player_x = 0;
float player_rotate = 0;
float lastTicks = 0.0f;

//randomly generate two floats for ball direction (x and y)
float r1 = 1.0f + (rand()/(RAND_MAX/(2.0f)));
float r2 = 1.0f + (rand()/(RAND_MAX/(2.0f)));

glm::vec3 player1_position = glm::vec3(-4.5, 0, 0);
glm::vec3 player2_position = glm::vec3(4.5, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);

glm::vec3 player1_movement = glm::vec3(0, 0, 0);
glm::vec3 player2_movement = glm::vec3(0, 0, 0);

glm::vec3 ball_movement = glm::vec3(0, 0, 0);

float player_speed = 2.0f;
float ball_speed = 1.0f;

GLuint playerTextureID, playerTextureID2, playerTextureID3;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n"; assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("HW2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix2 = glm::mat4(2.0f);
    modelMatrix2 = glm::mat4(1.0f);
    
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //load texture for two pictures
    playerTextureID = LoadTexture("ctg.png");
    playerTextureID2 = LoadTexture("ctg.png");
    playerTextureID3 = LoadTexture("ball.png");
}




void ProcessInput() {
    
    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_A]) {
        player1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_Z]) {
        player1_movement.y = -1.0f;
    }
    
    if (keys[SDL_SCANCODE_UP]) {
        player2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        player2_movement.y = -1.0f;
    }
    
    if (glm::length(player1_movement) > 1.0f) {
        player1_movement = glm::normalize(player1_movement);
    }
    if (glm::length(player2_movement) > 1.0f) {
        player2_movement = glm::normalize(player2_movement);
    }
}

void Update() {
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    
    lastTicks = ticks;
    
    player1_position += player1_movement * player_speed * deltaTime;
    player2_position += player2_movement * player_speed * deltaTime;
    
    ball_position.x += r1 * deltaTime * ball_speed;
    ball_position.y += r2 * deltaTime * ball_speed;
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, player1_position);
    
    modelMatrix2 = glm::mat4(1.0f);
    modelMatrix2 = glm::translate(modelMatrix2, player2_position);
    
    modelMatrix3 = glm::mat4(1.0f);
    modelMatrix3 = glm::translate(modelMatrix3, ball_position);
    
    //players stop when hit the wall
    if(player1_position.y + 0.01f >= 3.0f){
        player1_position.y = 3.0f - 0.01f;
    }
    else if(player1_position.y - 0.5f <= -3.6f){
        player1_position.y = -3.6f + 0.5f;
    }
    
    if(player2_position.y + 0.01f >= 3.0f){
        player2_position.y = 3.0f - 0.01f;
    }
    else if(player2_position.y - 0.5f <= -3.6f){
        player2_position.y = -3.6f + 0.5f;
    }
    
    // win
    if (ball_position.x - 0.05f >= 5.0f) {
        std::cout << "Left Player Wins!\n";
        endOfGame = true;
    }
    else if (ball_position.x + 0.05f <= -5.0f) {
        std::cout << "Right Player Wins!\n";
        endOfGame = true;
    }
    // ball hits celling or floor
    else if (abs(ball_position.y) + 0.5f >= 3.75f) {
        r2 = -r2;
        r1 += deltaTime * ball_speed * r1;
        ball_position.y += deltaTime * ball_speed * r2;
    }
    // ball hits left player
    else if (abs(player1_position.y - ball_position.y) - 0.5f < 0
            && abs(player1_position.x - ball_position.x) - 0.5f < 0) {
        r1 = -r1;
        ball_position.x += deltaTime * ball_speed * r1;
        ball_position.y += deltaTime * ball_speed * r2;
    }
    // ball hits right player
    else if (abs(player2_position.y- ball_position.y) - 0.5f < 0 &&
              abs(player2_position.x - ball_position.x) - 0.5f < 0) {
        r1 = -r1;
        ball_position.x += deltaTime * ball_speed * r1;
        ball_position.y += deltaTime * ball_speed * r2;
    }
    
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(modelMatrix);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    
    program.SetModelMatrix(modelMatrix2);

    float vertices2[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords2[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, playerTextureID2);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    
    program.SetModelMatrix(modelMatrix3);

    float vertices3[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords3[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, playerTextureID3);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    

    SDL_GL_SwapWindow(displayWindow);
    
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        // stop updating when there's a winner
        if (endOfGame == false) Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
