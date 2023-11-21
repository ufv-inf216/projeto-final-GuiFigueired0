//
// Created by Lucas N. Ferreira on 16/11/23.
//

#pragma once

#include "DrawSpriteComponent.h"

class DrawTileComponent : public DrawSpriteComponent {
public:
    // (Lower draw order corresponds with further back)
    DrawTileComponent(class Actor* owner,
            const std::string &tileData, const std::string &tileImgs,
            int width, int height, int tileSize, int drawOrder = 50);
    ~DrawTileComponent() override;

    void Draw(SDL_Renderer* renderer) override;

private:
    void LoadTileCSV(const std::string& mapPath);

    std::vector<std::vector<int>> mTiles;
    int mTileSize;
};
