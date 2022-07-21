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
        /**
         * @brief Set the class of the tile
         * 
         * @param index 
         * @param class
         */
        void SetTileClass(int index, std::string tileClass);
        std::string GetTileClass(int index);
    
    private:
        Sprite tileSet;
        std::vector<std::string> tileSetClasses;
        int rows, columns;
        int tileHeight, tileWidth;
};
#endif