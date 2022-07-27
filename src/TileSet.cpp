#include "TileSet.h"

TileSet::TileSet(GameObject& associated, int tileWidth, int tileHeight, std::string file) : tileHeight(tileHeight), tileWidth(tileWidth),  tileSet(file, associated, 1, 1.0){
    this->columns = tileSet.GetWidth() / tileWidth;
    this->rows =  tileSet.GetHeight() / tileHeight;
}

void TileSet::RenderTile(unsigned index, float x, float y){
    if(index < (columns*rows)){
        // calculate clip address 
        // x = (index MOD columns) * tileWidth
        // y = (index / columns) * tileHeight
        tileSet.SetClip( (index%this->columns)*tileWidth, (index/this->columns)*tileHeight, tileWidth, tileHeight );
        tileSet.Render(x, y);
    }
}

int TileSet::GetTileHeight(){
    return tileHeight;
}

int TileSet::GetTileWidth(){
    return tileWidth;
}