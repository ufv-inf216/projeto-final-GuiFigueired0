//
// Created by Lucas N. Ferreira on 16/11/23.
//

#include <fstream>
#include "../../CSV.h"
#include "DrawTileComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawTileComponent::DrawTileComponent(class Actor* owner, const std::string &tileData, const std::string &tileImg, int width, int height, int tileSize, int drawOrder)
        :DrawSpriteComponent(owner, tileImg, width, width, drawOrder)
        ,mTileSize(tileSize)
{
    LoadTileCSV(tileData);
}

DrawTileComponent::~DrawTileComponent()
{

}

void DrawTileComponent::LoadTileCSV(const std::string& mapPath)
{
    std::ifstream file(mapPath);
    if (!file.is_open())
    {
        SDL_Log("Failed to load paths: %s", mapPath.c_str());
    }

    int row = 0;

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);

        if(!line.empty()) {
            auto tiles = CSVHelper::Split(line);

            mTiles.emplace_back();
            for(const auto& t : tiles) {
                mTiles[row].emplace_back(std::stoi(t));
            }

            row++;
        }
    }
}

void DrawTileComponent::Draw(SDL_Renderer* renderer)
{
    size_t numRows = mTiles.size();
    size_t numCols = mTiles[0].size();

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            Vector2 pos = mOwner->GetPosition();

            int tile = mTiles[i][j];

            if(tile == -1) {
                continue;
            }

            int tilesPerRow = mWidth/mTileSize;

            int x = (tile % tilesPerRow) * mTileSize;
            int y = (tile / tilesPerRow) * mTileSize;

            pos.x += j * mTileSize;
            pos.y += i * mTileSize;

            SDL_Rect clipRect =  {x, y, mTileSize, mTileSize};
            SDL_Rect renderQuad = {static_cast<int>(pos.x),
                                   static_cast<int>(pos.y),
                                   clipRect.w,
                                   clipRect.h};

            SDL_RendererFlip flip = SDL_FLIP_NONE;
            /*
            if(mOwner->GetDirection().y < .0f && mIsFlippedVertically) {
                flip = SDL_FLIP_VERTICAL;
            }
            else if (mOwner->GetDirection().x < .0f && mIsFlippedHorizontally) {
                flip = SDL_FLIP_HORIZONTAL;
            }
            */
            SDL_RenderCopyEx(renderer, mSpriteSheetSurface, &clipRect, &renderQuad, .0f, nullptr, flip);
        }
    }
}