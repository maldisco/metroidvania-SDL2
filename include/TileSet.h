#ifndef TILESET_H
#define TILESET_H

#include "Sprite.h"

/**
 * @brief Class that holds every map texture
 * 
 */
class TileSet{
    public:
        /**
         * @brief Construct a new Tile Set object
         * 
         * @param associated 
         * @param tileWidth 
         * @param tileHeight 
         * @param file 
         */
        TileSet(GameObject& associated, int tileWidth, int tileHeight, std::string file);

        /**
         * @brief Render a tile on a given position
         * 
         * @param index 
         * @param x 
         * @param y 
         */
        void RenderTile(unsigned index, float x, float y);

        int GetTileWidth();
        int GetTileHeight();
    
    private:
        Sprite tileSet;
        int rows, columns;
        int tileHeight, tileWidth;
};
#endif