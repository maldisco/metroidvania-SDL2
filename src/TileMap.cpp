#include "TileMap.h"
#include "Camera.h"
#include "iostream"
#include "fstream"
#include "sstream"

TileMap::TileMap(GameObject &associated, std::string file, TileSet *tileSet, bool background) : Component(associated), tileSet(tileSet), background(background)
{
    Load(file);
}

void TileMap::Load(std::string file)
{
    std::fstream map;
    std::vector<int> dims;

    map.open(file);
    if (map.is_open())
    {
        std::string line;

        getline(map, line);
        std::stringstream ss(line);

        // extract map dimension from 1st line
        for (int i; ss >> i;)
        {
            dims.push_back(i);
            if (ss.peek() == ',')
                ss.ignore();
        }
        mapWidth = dims[0];
        mapHeight = dims[1];
        mapDepth = dims[2];

        associated.box.w = mapWidth * tileSet->GetTileWidth();
        associated.box.h = mapHeight * tileSet->GetTileHeight();

        while (getline(map, line))
        {
            if (line != "")
            {
                std::stringstream ss(line);

                for (int i; ss >> i;)
                {
                    tileMatrix.push_back(i - 1);

                    if (ss.peek() == ',')
                    {
                        ss.ignore();
                    }
                }
            }
        }
    }
}

void TileMap::SetTileSet(TileSet *tileSet)
{
    this->tileSet = tileSet;
}

int &TileMap::At(int x, int y, int z)
{
    return tileMatrix[mapWidth * mapHeight * z + mapWidth * y + x];
}

bool TileMap::IsSolid(int x, int y)
{
    return At(x, y, 0) != -1;
}

int TileMap::ScanLeft(std::set<int> yAxis, int x)
{
    for(int i = x; i >= 0; i--)
    {
        for(auto y : yAxis)
        {
            if(IsSolid(i, y))
            {
                return i;
            }
        }
    }

    return 0;
}

int TileMap::ScanRight(std::set<int> yAxis, int x)
{
    for(int i = x; i < mapWidth; i++)
    {
        for(auto y : yAxis)
        {
            if(IsSolid(i, y))
            {
                return i;
            }
        }
    }

    return mapWidth;
}

float TileMap::ScanDown(std::set<int> xAxis, float height)
{
    for (int i = height; i < mapHeight; i++)
    {
        for (auto x : xAxis)
        {
            if (IsSolid(x, i))
            {
                return i;
            }
        }
    }

    return mapHeight;
}

float TileMap::ScanUp(std::set<int> xAxis, float height)
{
    for (int i = height; i >= 0; i--)
    {
        for (auto x : xAxis)
        {
            if (IsSolid(x, i))
            {
                return i;
            }
        }
    }

    return 0;
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY)
{
    for (int i = 0; i < mapWidth * mapHeight; i++)
    {

        // i % mapWidth = Posição X
        // i / mapHeight = Posição Y
        // Multiplica-se o tileWidth e tileHeight pois as tiles não tem tamanho 1
        int x, y;
        if (background)
        {
            float parallax = 1 - 1 / pow(2, layer + 1);
            x = (i % mapWidth) * tileSet->GetTileWidth() - (cameraX * parallax);
            y = (i / mapWidth) * tileSet->GetTileHeight() - (cameraY * parallax);
        }
        else
        {
            x = (i % mapWidth) * tileSet->GetTileWidth() - cameraX;
            y = (i / mapWidth) * tileSet->GetTileHeight() - cameraY;
        }
        int index = At(i % mapWidth, i / mapWidth, layer);

        tileSet->RenderTile(index, x, y);
    }
}

void TileMap::Render()
{
    for (int i = 0; i < mapDepth; i++)
    {
        RenderLayer(i, Camera::virtualPos.x, Camera::virtualPos.y);
    }
}

void TileMap::Update(float dt) {}

int TileMap::GetDepth() { return mapDepth; }
int TileMap::GetWidth() { return mapWidth; }
int TileMap::GetHeight() { return mapHeight; }