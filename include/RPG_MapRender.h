#ifndef HEADER_GAME_MAP_RENDERH
#define HEADER_GAME_MAP_RENDERH

#include "SDL\SDL.h" 
#include "RPG_Image.h"
#include "RPG_Map.h"
using namespace std;
namespace RPG_NS_MapRender{

	class RPG_MapRender	//��ͼ��Ⱦ��
	{	
	protected:
		RPG_NS_Map::RPG_Map* m_rpgMap; //��ͼ��
		SDL_Surface* m_backMapBuf;	 //ȫ��ͼ����-����
		SDL_Surface* m_frontMapBuf;   //ȫ��ͼ����-ǰ��
		int m_numOfTiled;	//������ʽ����
		RPG_NS_Image::RPG_Image* m_tiledSet; //������
	public:
		RPG_MapRender(RPG_NS_Map::RPG_Map* rpgMap,string tiledFile,
					  int x_tiled, int y_tiled);
		~RPG_MapRender();
		SDL_Surface* getBackMapBuf() {return m_backMapBuf;}
		SDL_Surface* getFrontMapBuf() {return m_frontMapBuf;}
		int getNumOfTiled() {return m_numOfTiled;}
		RPG_NS_Map::RPG_Map* getMap() {return m_rpgMap;}
		RPG_NS_Image::RPG_Image* getTiledSet() {return m_tiledSet;}
		void mapRender_Init(string tiledFile, int x_tiled, int y_tiled);
		void frontMap_Render();	//��Ⱦǰ��
		void backMap_Render();	//��Ⱦ����
	};
}
#endif
