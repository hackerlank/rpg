/**********************************/
/***        ��ͼ��Ⱦ�ࣺ     ******/
/* ����ͼ������ǰ������ֱ���
   ���ڴ���������				 **/
/**********************************/
#include "../include/RPG_mapRender.h"
#include "../include/RPG_Config.h"
void RPG_NS_MapRender::RPG_MapRender::mapRender_Init(string tiledFile,
													  int x_tiled ,int y_tiled){	//��ͼ��Ⱦ����ʼ��
	RPG_NS_Map::SizeType mapSize;
	RPG_NS_Map::SizeType tiledSize;
	mapSize = m_rpgMap->getMapSize();
	tiledSize = m_rpgMap->getTiledSize();
	m_tiledSet = new RPG_NS_Image::RPG_Image(tiledFile); //��������
	if(m_tiledSet == NULL)
		throw "��������ʧ��";
	m_tiledSet->splitImage(x_tiled, y_tiled); //�����ļ��з�Ϊx_tiled*y_tiled��С����
	m_numOfTiled = x_tiled * y_tiled;

	m_backMapBuf = SDL_CreateRGBSurface(SDL_SWSURFACE, mapSize.width*tiledSize.width, mapSize.height*tiledSize.height, 
						 				 32, 0, 0, 0, 0);
	if(m_backMapBuf == NULL)
		throw "�޷������ͼ�����ڴ�";
	m_frontMapBuf = SDL_CreateRGBSurface(SDL_SWSURFACE, mapSize.width*tiledSize.width, mapSize.height*tiledSize.height, 
										 32, 0, 0, 0, 0);
	if(m_frontMapBuf == NULL)
		throw "�޷������ͼǰ���ڴ�";

	SDL_FillRect(m_frontMapBuf, NULL, SDL_MapRGB(m_frontMapBuf->format, 0xff, 0, 0xff)); //���͸��ɫ
	SDL_SetColorKey(m_frontMapBuf, SDL_SRCCOLORKEY, SDL_MapRGB(m_frontMapBuf->format, 0xff, 0, 0xff)); //����͸��ɫ


	//��Ⱦ
	frontMap_Render();
	backMap_Render();
	
}
void RPG_NS_MapRender::RPG_MapRender::frontMap_Render()//ǰ����Ⱦ
{
	int xTiled,yTiled;
	int* mapDataFront;
	RPG_NS_Map::SizeType tiledSize,mapSize;
	mapDataFront = m_rpgMap->getMapDataFront();
	tiledSize = m_rpgMap->getTiledSize();
	mapSize = m_rpgMap->getMapSize();
	for(yTiled=0; yTiled<mapSize.height; yTiled++)
		for(xTiled=0; xTiled<mapSize.width; xTiled++){
			if(*(mapDataFront+yTiled*mapSize.width+xTiled) != -1)
				m_tiledSet->drawSpliter(m_frontMapBuf, tiledSize.width*xTiled,
				tiledSize.height*yTiled, *(mapDataFront+yTiled*mapSize.width+xTiled));
		}
}

void RPG_NS_MapRender::RPG_MapRender::backMap_Render()//������Ⱦ
{
	int xTiled,yTiled;
	int* mapDataBack;
	RPG_NS_Map::SizeType tiledSize,mapSize;
	mapDataBack = m_rpgMap->getMapDataBack();
	tiledSize = m_rpgMap->getTiledSize();
	mapSize = m_rpgMap->getMapSize();
	for(yTiled=0; yTiled<mapSize.height; yTiled++)
		for(xTiled=0; xTiled<mapSize.width; xTiled++){
			if(*(mapDataBack+yTiled*mapSize.width+xTiled) != -1)
			m_tiledSet->drawSpliter(m_backMapBuf, tiledSize.width*xTiled,
				tiledSize.width*yTiled, *(mapDataBack+yTiled*mapSize.width+xTiled));
		}
}

RPG_NS_MapRender::RPG_MapRender::RPG_MapRender(class RPG_NS_Map::RPG_Map* rpgMap,
											   string tiledFile, int x_tiled, int y_tiled){//���캯��
	m_rpgMap = rpgMap;
	mapRender_Init(tiledFile, x_tiled, y_tiled);	//��ʼ����ͼ��Ⱦ��
}

RPG_NS_MapRender::RPG_MapRender::~RPG_MapRender()
{
	SDL_FreeSurface(m_backMapBuf);
	SDL_FreeSurface(m_frontMapBuf);
	delete m_tiledSet;
	delete m_rpgMap;
}