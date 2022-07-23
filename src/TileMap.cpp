#include "TileMap.h"
#include "Camera.h"
#include "iostream"
#include "fstream"
#include "sstream"


TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) : Component(associated), tileSet(tileSet){
    Load(file);
}

void TileMap::Load(std::string file){
    std::fstream solidBlocks;
    int tileId = 0;
    solidBlocks.open("assets/map/solidblocks.txt");
    if(solidBlocks.is_open()){
        std::string line;
        while(getline(solidBlocks, line)){
            tileId = std::stoi(line);
            tileSet->SetTileClass(tileId, "solid");
        }
    }

    std::fstream map;
    std::vector<int> dims;

    map.open(file);
    if(map.is_open()){
        std::string line;
        
        getline(map, line);
        std::stringstream ss(line);

        // extract map dimension from 1st line
        for(int i; ss >> i;){
            dims.push_back(i);
            if(ss.peek() == ',')
                ss.ignore();
        }
        mapWidth = dims[0];
        mapHeight = dims[1];
        mapDepth = dims[2];

        while(getline(map, line)){
            if(line != ""){
                std::stringstream ss(line);

                for(int i; ss >> i;){
                    tileMatrix.push_back(i);

                    if(ss.peek()==','){
                        ss.ignore();
                    }
                
                }
            
            }

        }

    }
}

void TileMap::SetTileSet(TileSet* tileSet){
    this->tileSet = tileSet;
}

int& TileMap::At(int x, int y, int z){
    return tileMatrix[mapWidth*mapHeight*z + mapWidth*y + x];
}

bool TileMap::IsSolid(int x, int y){
    std::string class1 = tileSet->GetTileClass(At(x, y, 1));
    std::string class2 = tileSet->GetTileClass(At(x, y, 2));
    return (class1.compare("solid") == 0) or (class2.compare("solid") == 0);
}

float TileMap::ScanX(std::set<int> yAxis){

}

float TileMap::ScanY(std::set<int> xAxis, float y){
    for(int i = y; i < mapHeight; i++){
        for(auto x : xAxis){
            if(IsSolid(x, i)){
                return i;
            }
        }
    }

    return mapHeight;
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY){
    for(int i = 0; i < mapWidth*mapHeight; i++){
        
        // i % mapWidth = Posição X  
        // i / mapHeight = Posição Y 
        // Multiplica-se o tileWidth e tileHeight pois as tiles não tem tamanho 1
        int x = (i % mapWidth)*tileSet->GetTileWidth() - (cameraX * (1));
        int y = (i / mapWidth)*tileSet->GetTileHeight() - (cameraY * (1));
        int index = At(i%mapWidth, i/mapWidth, layer);

        tileSet->RenderTile(index, x, y);
    }
}

void TileMap::Render(){
    for(int i = 0; i < mapDepth; i++){
        RenderLayer(i, Camera::pos.x, Camera::pos.y);
    }
}

void TileMap::Update(float dt){}
bool TileMap::Is(std::string type){ 
    if(type.compare("Tile map") == 0){
        return true;
    }

    return false;
 }
void TileMap::NotifyCollision(GameObject& other){}

int TileMap::GetDepth(){ return mapDepth; }
int TileMap::GetWidth(){ return mapWidth; }
int TileMap::GetHeight(){ return mapHeight; }