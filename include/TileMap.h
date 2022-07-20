#ifndef TILEMAP_H
#define TILEMAP_H

#include "TileSet.h"

/**
 * @brief Component responsible for map textures
 * 
 */
class TileMap : public Component{
    public:
        /**
         * @brief Construct a new Tile Map object
         * 
         * @param associated 
         * @param file 
         * @param tileSet 
         */
        TileMap(GameObject& associated, std::string file, TileSet* tileSet);

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
        void SetTileSet(TileSet* tileSet);

        /**
         * @brief Return tile in the position X, Y, Z
         * 
         * @param x 
         * @param y 
         * @param z 
         * @return int& 
         */
        int& At(int x, int y, int z = 0);

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
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);
    
    private:
        /**
         * @brief Matrix that represents game map
         * 
         */
        std::vector<int> tileMatrix;
        TileSet* tileSet;
        int mapWidth;
        int mapHeight;
        int mapDepth;
};
#endif