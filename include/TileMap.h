#ifndef TILEMAP_H
#define TILEMAP_H

#include "TileSet.h"

/**
 * @brief Component responsible for map textures
 *
 */
class TileMap : public Component
{
public:
    /**
     * @brief Construct a new Tile Map object
     *
     * @param associated
     * @param file
     * @param tileSet
     */
    TileMap(GameObject &associated, std::string file, TileSet *tileSet, bool background = false);

    /**
     * @brief Load a txt file representing game map and parse it into a matrix
     *
     * @param file
     */
    void Load(std::string file);

    /**
     * @brief Set the Tile Set object
     *
     * @param tileSet
     */
    void SetTileSet(TileSet *tileSet);

    /**
     * @brief Return tile in the position X, Y, Z
     *
     * @param x
     * @param y
     * @param z
     * @return int&
     */
    int &At(int x, int y, int z = 0);

    /**
     * @brief Return if tile in position X, Y is solid or not
     *
     * @param x
     * @param y
     * @return true
     * @return false
     */
    bool IsSolid(int x, int y);

    /**
     * @brief Scan xAxis (righ-left) for obstacle
     *
     * @param yAxis
     * @return int
     */
    int ScanLeft(std::set<int> yAxis, int x);

    /**
     * @brief Scan xAxis (left-right) for obstacle
     *
     * @param yAxis
     * @return int
     */
    int ScanRight(std::set<int> yAxis, int x);

    /**
     * @brief scan yAxis (top-down) for obstacle
     *
     * @param xAxis
     * @param height
     * @return float
     */
    float ScanDown(std::set<int> xAxis, float height);

    /**
     * @brief scan yAxis (down-top) for obstacle
     *
     * @param xAxis
     * @param height
     * @return float
     */
    float ScanUp(std::set<int> xAxis, float height);

    /**
     * @brief Render game map
     *
     */
    void Render();

    /**
     * @brief Render a layer of game map
     *
     * @param layer
     * @param cameraX
     * @param cameraY
     */
    void RenderLayer(int layer, int cameraX = 0, int cameraY = 0);

    int GetWidth();
    int GetHeight();
    int GetDepth();

    void Update(float dt);
    
    TileSet *tileSet;
private:
    /**
     * @brief Matrix that represent game map
     *
     */
    std::vector<int> tileMatrix;
    int mapWidth;
    int mapHeight;
    int mapDepth;
    bool background;
};
#endif