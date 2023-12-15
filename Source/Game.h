// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "Math.h"
#include "CSV.h"
#include "Scenes/Level.h"
#include <Box2D/Box2D.h>
#include "Transform.h"
#include "Actors/Actor.h"
#include "Actors/Player.h"

class Game
{
public:
    const float SCENE_TRANSITION_TIME = 0.25f;

    enum class GameScene
    {
        Menu,
        Level
    };

    enum class FadeState
    {
        FadeIn,
        FadeOut,
        None
    };

    Game(int windowWidth, int windowHeight);

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    // Actor functions
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    // Draw functions
    void AddDrawable(class DrawComponent* drawable);
    void RemoveDrawable(class DrawComponent* drawable);

    // Collider functions
    void AddCollider(class AABBColliderComponent* collider);
    void RemoveCollider(class AABBColliderComponent* collider);
    std::vector<class AABBColliderComponent*>& GetColliders() { return mColliders; }

    void SetScene(GameScene gameState);
    void SetIsPaused(bool paused) { mIsPaused = paused; }

    SDL_Renderer* GetRenderer() { return mRenderer; }

    class Font* GetFont(const std::string& fileName);

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }
    bool GetStatePaused() const { return mIsPaused; }

    SDL_Texture* LoadTexture(const std::string& texturePath);

    // Level
    class Level* GetLevel() { return mLevels[mCurrentLevel-1]; }
    class AudioSystem* GetSound() { return mAudio; }
    bool showColliders() const { return mShowColliders; }
    void changeShowColliders() { mShowColliders = !mShowColliders; }
    bool GetStateWin() { return mWin.first && mWin.second; }
    void SetWinWaterGirl(bool status) {mWin.first = status; }
    void SetWinFireBoy(bool status) {mWin.second = status; }

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Unload actors for scene transition
    void InitializeScene();
    void UnloadActors();

    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;

    // All the collision components
    std::vector<class AABBColliderComponent*> mColliders;

    std::map<std::string, class Font*> mFonts;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    AudioSystem *mAudio;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mLevelRunning;
    bool mUpdatingActors;
    bool mIsPaused;
    float timePaused;

    // Scene transition effect
    FadeState mFadeState;
    float mSceneTransitionTime;

    GameScene mGameState;
    class Scene *mScene;

    // Levels
    std::vector<Level*> mLevels;
    int mCurrentLevel;
    bool mShowColliders;
    std::pair<bool, bool> mWin;
};
