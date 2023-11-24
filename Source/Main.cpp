// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <Box2D/Box2D.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

int main(int argc, char** argv)
{
    // Exemplo de uso box2d para verificar se a biblioteca está funcionando
    /*
    // Configuração do mundo
    b2Vec2 gravity(0.0f, -9.8f);
    b2World world(gravity);

    // Definição do corpo rígido (retângulo)
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 10.0f);  // Posição inicial

    b2Body* dynamicBody = world.CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(1.0f, 1.0f);  // Tamanho do retângulo

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    dynamicBody->CreateFixture(&fixtureDef);

    // Simulação do mundo por alguns passos
    for (int32 i = 0; i < 10; ++i) {
        world.Step(1.0f / 60.0f, 6, 2);
    }

    // Verificação se a importação da Box2D está ocorrendo corretamente
    if (dynamicBody) {
        std::cout << "Box2D importado corretamente. Corpo rigido criado!" << std::endl;
    } else {
        std::cerr << "Erro ao importar Box2D ou criar o corpo rígido." << std::endl;
        return 1;
    }
    */

    Game game = Game(SCREEN_WIDTH, SCREEN_HEIGHT);
    bool success = game.Initialize();
    if (success)
    {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}
