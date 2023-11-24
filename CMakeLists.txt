cmake_minimum_required(VERSION 3.8)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# INIT NO SCREEN BUGFIX
# Em alguns casos, o programa pode não renderizar a tela. Por isso temos
# as seguintes linhas. Caso encontre algum problema, comente-as.
set(CMAKE_SKIP_RPATH ON)
set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
# END NO SCREEN BUGFIX
####### DO NOT EDIT ABOVE

# Adicione o caminho para os arquivos do Box2D
add_subdirectory(box2d)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "fogo-e-agua")
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME}
        Source/Main.cpp
        Source/Math.cpp
        Source/Random.cpp
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Game.cpp
        Source/Components/DrawComponents/DrawComponent.cpp
        Source/Components/DrawComponents/DrawComponent.h
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/Components/ColliderComponents/CircleColliderComponent.cpp
        Source/Components/ColliderComponents/CircleColliderComponent.h
        Source/Json.h
        Source/Components/DrawComponents/DrawSpriteComponent.cpp
        Source/Components/DrawComponents/DrawSpriteComponent.h
        Source/Components/DrawComponents/DrawPolygonComponent.cpp
        Source/Components/DrawComponents/DrawPolygonComponent.h
        Source/Actors/Mario.cpp
        Source/Actors/Mario.h
        Source/Components/ColliderComponents/AABBColliderComponent.cpp
        Source/Components/ColliderComponents/AABBColliderComponent.h
        Source/Components/DrawComponents/DrawAnimatedComponent.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.h
        Source/Actors/Block.cpp
        Source/Actors/Block.h
        Source/Actors/Goomba.cpp
        Source/Actors/Goomba.h
        Source/Actors/Spawner.cpp
        Source/Actors/Spawner.h
        Source/CSV.h
        Source/Components/DrawComponents/DrawTileComponent.h
        Source/Components/DrawComponents/DrawTileComponent.cpp
        Source/AudioSystem.cpp
        Source/AudioSystem.h
)

####### DO NOT EDIT BELLOW
find_package(SDL2 2.28.2 EXACT REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer box2d)