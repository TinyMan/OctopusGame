// level.cpp
// File handling the informations for a level

#include "stdafx.h"
#include <SDL.h>
#include "graphics.h"
#include "level.h"
#include "tinyxml2.h"
#include "utils.h"
#include "player.h"
#include "bullet.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <string>

using namespace tinyxml2;

Level::Level() {

}

Level::Level(std::string mapName, Vector2 spawnPoint, GraphicalOctopus &graphics) :
	_mapName(mapName),
	_size(Vector2(0,0)) 
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {

}

void Level::loadMap(std::string mapName, GraphicalOctopus &graphics) {
	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "Content/Maps/" << mapName << ".tmx"; //Pass in Map 1, we get maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str());
	 
	XMLElement* mapNode = doc.FirstChildElement("map");
	 
	//Get the width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);
	 
	//Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);
	 
	//Loading the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->Attribute("source");
			std::stringstream ss;
			ss << source;

			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = graphics.loadImage(ss.str());
			this->_tilesets.push_back(Tileset(tex, firstgid));

			// Get all the animations for that tileset
			XMLElement* pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.startTileId = pTileA->IntAttribute("id") + firstgid;
					ati.tilesetFirstGid = firstgid;
					XMLElement* pAnimation = pTileA->FirstChildElement("animation");
					if(pAnimation != NULL) {
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.tileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.duration = pFrame->FloatAttribute("duration");

									pFrame = pFrame->NextSiblingElement("frame");
								}
							}

							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfos.push_back(ati);
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	//Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			//Loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//Loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//Build each individual tile here
							//If gid is 0, no tile should be drawn. Continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < (int)(this->_tilesets.size()); i++) {
								if (this->_tilesets[i].firstGid <= gid) {
									if (this->_tilesets[i].firstGid > closest) {
										closest = this->_tilesets[i].firstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.firstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy = tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							//Calculate the position of the tile in the tileset

							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							//Build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < (int)this->_animatedTileInfos.size(); i++) {
								if (this->_animatedTileInfos.at(i).startTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}

							if (isAnimatedTile) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < (int)ati.tileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.tileIds.at(i),
										tileWidth, tileHeight));
									AnimatedTile tile(tilesetPositions, ati.duration, tls.texture,
										Vector2(tileWidth, tileHeight), finalTilePosition);
									this->_animatedTileList.push_back(tile);
								}
							}
							else {
								Tile tile(tls.texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	// Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "walls") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");

						this->_verticalCollisionRects.push_back(Rectangle(
							(int)(std::ceil(x) * globals::TILE_SCALE),
							(int)(std::ceil(y) * globals::TILE_SCALE),
							(int)(std::ceil(width) * globals::TILE_SCALE),
							(int)(std::ceil(height) * globals::TILE_SCALE)
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "platforms") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");

						this->_horizontalCollisionRects.push_back(Rectangle(
							(int)(std::ceil(x) * globals::TILE_SCALE),
							(int)(std::ceil(y) * globals::TILE_SCALE),
							(int)(std::ceil(width) * globals::TILE_SCALE),
							(int)(std::ceil(height) * globals::TILE_SCALE)
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			// Other objects group go here with an else if (ss.str() == "whatever")
			else if (ss.str() == "slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2((int)std::ceil(pObject->FloatAttribute("x")), (int)std::ceil(pObject->FloatAttribute("y")));

						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');

							// Now we have each of the pairs
							// Loop through the list and split them into Vector2s and store them in <points>
							for (int i = 0; i < (int)pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (int i = 0; i < (int)points.size(); i += 2) {
							this->_slopes.push_back(Slope(
								Vector2(((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::TILE_SCALE),
								((p1.y + points.at(i < 2 ? i : i - 1).y) * globals::TILE_SCALE)),
								Vector2(((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::TILE_SCALE),
								((p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::TILE_SCALE))
							));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "spawn points") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoints.push_back(Vector2((int)(std::ceil(x) * globals::TILE_SCALE),
								(int)(std::ceil(y) * globals::TILE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}


			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}

}

void Level::update(int elapsedTime) {
	for (int i = 0; i < (int)this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
	}
}

void Level::draw(GraphicalOctopus &graphics) {
	// Draws the background
	for (int i = 0; i < (int)(this->_tileList.size()); i++) {
		this->_tileList.at(i).draw(graphics);
	}

	for (int i = 0; i < (int)this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).draw(graphics);
	}
}

std::vector<Rectangle> Level::checkTileCollision(Player &player) {
	Rectangle other = player.getBoundingBox();
	std::vector<Rectangle> others;
	for (int i = 0; i < (int)this->_verticalCollisionRects.size(); i++) {
		if (this->_verticalCollisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_verticalCollisionRects.at(i));
		}
	}

	for (int i = 0; i < (int)this->_horizontalCollisionRects.size(); i++) {
		if (this->_horizontalCollisionRects.at(i).collidesWith(other)
			&& player.getPreviousY() + (player.getBoundingBox().getHeight()	 * globals::SPRITE_SCALE) < this->_horizontalCollisionRects.at(i).getTop()) {
			others.push_back(this->_horizontalCollisionRects.at(i));
		}
	}

	return others;
}

std::vector<Rectangle> Level::checkBulletCollision(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < (int)this->_verticalCollisionRects.size(); i++) {
		if (this->_verticalCollisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_verticalCollisionRects.at(i));;
		}
	}

	for (int i = 0; i < (int)this->_horizontalCollisionRects.size(); i++) {
		if (this->_horizontalCollisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_horizontalCollisionRects.at(i));
		}
	}

	return others;
}

std::vector<Slope> Level::checkSlopeCollision(const Rectangle &other) {
	std::vector<Slope> others;
	for (int i = 0; i < (int)this->_slopes.size(); i++) {
		if (this->_slopes.at(i).collidesWith(other)) {
			others.push_back(this->_slopes.at(i));
		}
	}

	return others;
}

const std::vector<Vector2> Level::getPlayerSpawnPoints() const {
	return this->_spawnPoints;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = (gid - 1) % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;
	int tsyy = 0;
	int amt = ((gid - tls.firstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}
