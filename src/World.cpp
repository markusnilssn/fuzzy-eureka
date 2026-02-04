#include "World.h"
#include "PearlyNoise.hpp"
#include <vector>
#include "Engine/Components.h"
#include "GameComponents.h"
#include "Common/Random.h"

World::World(Content &content, Engine &engine, Grid &grid, int width, int height, const sf::Vector2i &nodeSize)
    : content(content), engine(engine), grid(grid), width(width), height(height), nodeSize(nodeSize)
{
    LoadFromPearlyNoise(width, height);
}

void World::LoadFromPearlyNoise(int width, int height)
{
    constexpr int tileSize = 16;
    Random random;

    float threshold = 0.0f;
    float scale = 0.01f;

    const siv::PerlinNoise::seed_type seed = 12345u;
    const siv::PerlinNoise perlin{seed};

    auto isDeepWater = [](float noise)
    { return noise < 0.455; };
    auto isNormalWater = [](float noise)
    { return noise < 0.465; };
    auto isShallowWater = [](float noise)
    { return noise < 0.475; };
    auto isShore = [](float noise)
    { return noise < 0.480; };
    auto isSand = [](float noise)
    { return noise < 0.51; };
    auto isShallowGrass = [](float noise)
    { return noise < 0.6; };
    auto isDeepGrass = [](float noise)
    { return noise < 0.7; };
    auto isMountain = [](float noise)
    { return noise >= 0.7f; };

    auto isTree = [&](float noise)
    { return noise > 0.6 && noise < 0.65; };

    constexpr float Invalid = -1.0f;

    auto isGrass = [&](float noise) -> bool
    {
        return isShallowGrass(noise) || isDeepGrass(noise);
    };

    auto isEdge = [&](float noise) -> bool
    {
        return (isGrass(noise) && noise > Invalid); // Anything not mountain
    };

    auto isWater = [&](float noise) -> bool
    {
        return isShallowWater(noise) || isNormalWater(noise) || isDeepWater(noise);
    };

    std::vector<std::vector<float>> noises;
    std::vector<std::vector<float>> noises2;

    noises.resize(width);
    noises2.resize(width);

    // Generate
    for (int_fast32_t x = 0; x < width; x++)
    {
        noises[x].resize(height);
        noises2[x].resize(height);

        for (int_fast32_t y = 0; y < height; y++)
        {
            float xpos = x * 0.01f;
            float ypos = y * 0.01f;
            float octaves = 6;
            float persistence = 0.5;
            // float lacunarity = 2.0;

            noises[x][y] = perlin.octave2D_01(xpos, ypos, octaves, persistence);
            noises2[x][y] = perlin.octave2D_01(xpos, ypos, octaves, persistence);
        }
    }

    // Cleanup

    auto Smooth = [&]()
    {
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; y++)
            {
                // fix mountains
                if (isMountain(noises[x][y]))
                {
                    auto GetTile = [=](int dx, int dy) -> float
                    {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx < 0 || nx >= width || ny < 0 || ny >= height)
                            return Invalid;
                        return noises[nx][ny];
                    };

                    auto up = GetTile(0, -1);
                    auto down = GetTile(0, 1);
                    auto left = GetTile(-1, 0);
                    auto right = GetTile(1, 0);

                    auto upLeft = GetTile(-1, -1);
                    auto upRight = GetTile(1, -1);
                    auto downLeft = GetTile(-1, 1);
                    auto downRight = GetTile(1, 1);

                    if (isEdge(up) && isEdge(down) && isEdge(left) && isEdge(right) && isEdge(upLeft) && isEdge(upRight) && isEdge(downLeft) && isEdge(downRight))
                        noises[x][y] = 0.6f;

                    if (isEdge(up) && isEdge(left) && isEdge(right) && !isEdge(down))
                        noises[x][y] = 0.6f;
                    if (isEdge(up) && isEdge(left) && !isEdge(right) && isEdge(down))
                        noises[x][y] = 0.6f;
                    if (isEdge(up) && !isEdge(left) && isEdge(right) && isEdge(down))
                        noises[x][y] = 0.6f;
                    if (!isEdge(up) && isEdge(left) && isEdge(right) && isEdge(down))
                        noises[x][y] = 0.6f;
                }
            }
        }
    };

    int_fast32_t amountToSmooth = 3;
    for (int_fast32_t i = 0; i < amountToSmooth; i++)
        Smooth();

    // Finalize
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; y++)
        {
            auto GetTile = [&](int dx, int dy) -> float
            {
                int nx = x + dx;
                int ny = y + dy;
                if (nx < 0 || nx >= width || ny < 0 || ny >= height)
                    return Invalid;
                return noises[nx][ny];
            };

            auto up = GetTile(0, -1);
            auto down = GetTile(0, 1);
            auto left = GetTile(-1, 0);
            auto right = GetTile(1, 0);

            auto upLeft = GetTile(-1, -1);
            auto upRight = GetTile(1, -1);
            auto downLeft = GetTile(-1, 1);
            auto downRight = GetTile(1, 1);

            auto isMountainEdge = [&](float noise)
            {
                auto occupy = [&](float noise)
                {
                    return isGrass(noise) && noise != Invalid;
                };

                return isMountain(noise) && (occupy(up) || occupy(down) || occupy(left) || occupy(right) || occupy(upLeft) || occupy(upRight) || occupy(downRight) || occupy(downLeft));
            };

            auto createEnvironment = [&](float noise) -> sf::Sprite
            {
                if (isDeepWater(noise))
                    return content.GetTextureSheet("Shore").Clip(4, 0);
                if (isNormalWater(noise))
                    return content.GetTextureSheet("Shore").Clip(3, 0);
                if (isShallowWater(noise))
                    return content.GetTextureSheet("Shore").Clip(2, 0);
                if (isShore(noise))
                    return content.GetTextureSheet("Shore").Clip(1, 0);
                if (isSand(noise))
                    return content.GetTextureSheet("Shore").Clip(0, 0);
                if (isShallowGrass(noise))
                    return content.GetTextureSheet("Grass").Clip(1, 0);
                if (isDeepGrass(noise))
                    return content.GetTextureSheet("Grass").Clip(2, 0);
                else
                {
                    auto background = content.GetTextureSheet("Grass").Clip(2, 0);

                    // Interface map (direction to sprite index)
                    if (isEdge(up) && !isEdge(down) && !isEdge(left) && !isEdge(right))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(1, 0, background); // top
                    if (isEdge(down) && !isEdge(up) && !isEdge(left) && !isEdge(right))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(1, 2, background); // bottom
                    if (isEdge(left) && !isEdge(up) && !isEdge(down) && !isEdge(right))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(0, 1, background); // left
                    if (isEdge(right) && !isEdge(up) && !isEdge(down) && !isEdge(left))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(2, 1, background); // right

                    if (isEdge(up) && isEdge(left))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(0, 0, background);
                    if (isEdge(up) && isEdge(right))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(2, 0, background);
                    if (isEdge(down) && isEdge(left))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(0, 2, background);
                    if (isEdge(down) && isEdge(right))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(2, 2, background);

                    if (isEdge(upLeft))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(3, 2, background); // top-left edge
                    if (isEdge(upRight))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(4, 2, background); // top-right edge
                    if (isEdge(downLeft))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(3, 3, background); // bottom-left edge
                    if (isEdge(downRight))
                        return content.GetTextureSheet("Cliff").ClipWithBackground(4, 3, background); // bottom-right edge

                    return content.GetTextureSheet("Cliff").Clip(1, 1); // Default mountain
                }
            };

            sf::Vector2f position(x * nodeSize.x, y * nodeSize.y);
            if (isTree(noises2[x][y]))
            {
                auto entity = engine.CreateEntity();
                engine.AddComponent(entity, TransformComponent{
                                                .position = position,
                                            });

                // engine.AddComponent(entity, SpriteComponent{
                //                                 .texture = content.GetTextureSheet("Trees").Clip(random.Next(1, 3), 0),
                //                             });

                engine.AddComponent(entity, ResourceComponent{
                                                .type = Resource::Wood,
                                                .amount = 50,
                                            });
                // engine.AddComponent(entity, ObjectComponent{});&

                grid.Lock(grid.GetNodeAt(x, y));
            }

            auto noise = noises[x][y];

            if (isWater(noise) || isMountainEdge(noise))
                grid.Lock(grid.GetNodeAt(x, y));

            auto sprite = createEnvironment(noise);
            sprite.setPosition(sf::Vector2f(x * nodeSize.x, y * nodeSize.y));
            world.emplace_back(std::move(sprite));
        }
    }
}

void World::Render(sf::RenderWindow &window)
{
    const sf::View &view = window.getView();
    sf::Vector2f offset(nodeSize.x, nodeSize.y);
    sf::Vector2f size = view.getSize() + offset;
    sf::Vector2f center = (view.getCenter() - size / 2.0f) - offset;
    sf::FloatRect viewport(center, size);

    int length = width * height;
    for (int index = 0; index < length; ++index)
    {
        int x = index / height;
        int y = index % height;

        if (!viewport.contains(world[index].getPosition()))
        {
            continue;
        }

        window.draw(world[index]);
    }
}
