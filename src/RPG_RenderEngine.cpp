#include "../include/RPG_Config.h"
#include "../include/RPG_RenderEngine.h"
#include "../include/RPG_SceneManager.h"

RPG_NS_RenderEngine::RPG_RenderEngine::RPG_RenderEngine(vector<RPG_NS_NPC::RPG_NPC*> *renderList, RPG_NS_MapRender::RPG_MapRender *mapRender,
														RPG_NS_Map::RectType visibleWindow){

	//��Ա��ֵ
	m_scene = NULL;
	m_renderList = renderList;
	m_mapRender = mapRender;
	m_visibleWindow = visibleWindow;
	m_currentMap = mapRender->getMap()->getMapID();
	adjustWindow();
	renderEngine_Init();	//��Ⱦ�����ʼ��
}

RPG_NS_RenderEngine::RPG_RenderEngine::~RPG_RenderEngine(void){
	//�ͷŷ�����ڴ�
	SDL_FreeSurface(m_scene);
	delete m_mapRender;
}

void RPG_NS_RenderEngine::RPG_RenderEngine::renderEngine_Init(){
	//���䳡���������ռ�
	RPG_NS_Map::SizeType mapSize;
	RPG_NS_Map::SizeType tiledSize;
	mapSize = m_mapRender->getMap()->getMapSize();
	tiledSize = m_mapRender->getMap()->getTiledSize();
	if(m_scene != NULL){
		SDL_FreeSurface(m_scene);
		m_scene = NULL;
	}
	m_scene = SDL_CreateRGBSurface(SDL_SWSURFACE, mapSize.width*tiledSize.width, mapSize.height*tiledSize.height, 
						 				 32, 0, 0, 0, 0);
	if(m_scene == NULL)
		throw "�޷����䳡���������ڴ�";//

}

void RPG_NS_RenderEngine::RPG_RenderEngine::RPG_changMap(RPG_NS_MapRender::RPG_MapRender *mapRender,
														 RPG_NS_Map::RectType vw){

	SDL_FreeSurface(m_scene);	//ע������������
	if(m_mapRender != NULL){
		delete m_mapRender;
		m_mapRender = NULL;		//ɾ����ͼ��Ⱦ�ࣨ�ɵ�ͼ��Ⱦ��������������Զ�ɾ���˵�ͼ�ࣩ					
	}
	m_mapRender = mapRender;
	RPG_NS_Map::SizeType mapSize;
	RPG_NS_Map::SizeType tiledSize;
	mapSize = m_mapRender->getMap()->getMapSize();
	tiledSize = m_mapRender->getMap()->getTiledSize();
	setCurrentMap(m_mapRender->getMap()->getMapID());
	m_currentMap = mapRender->getMap()->getMapID();
	setvisibleWindow(vw);
	adjustWindow();
	m_scene = SDL_CreateRGBSurface(SDL_SWSURFACE, mapSize.width*tiledSize.width, mapSize.height*tiledSize.height, 
						 				 32, 0, 0, 0, 0);
	if(m_scene == NULL)
		throw "�޷����䳡���������ڴ�";//


}

void RPG_NS_RenderEngine::RPG_RenderEngine::scrollScreen(){	//����
	
	using RPG_NS_NPC::RPG_NPC;
	int s_posX, s_posY;
	int e_posX, e_posY;	//С�Ӵ��˵�
	int hero_X, hero_Y;	//�����������
	RPG_NS_NPC::RPG_NPC *npc;

	npc = RPG_NS_SpriteManager::RPG_SpriteManager::getNPC(m_currentMap, NPC_HERO);

	s_posX = m_smallView.x;
	s_posY = m_smallView.y;
	e_posX = s_posX + m_smallView.width;
	e_posY = s_posY + m_smallView.height;
	hero_X = npc->getPosXE();
	hero_Y = npc->getPosYE();
	
	//����Ļ������������Ϊ�ο���Խ��С���ڶ���Ļ��������
	if(npc->getPosYE()<s_posY && npc->getMoveStatus()==RPG_NPC::NPC_UP)
		s_posY = npc->getPosYE();
	if(npc->getPosYE()+npc->getTileH()>e_posY && npc->getMoveStatus()==RPG_NPC::NPC_DOWN)
		s_posY = npc->getPosYE() + npc->getTileH() - m_smallView.height;
	if(npc->getPosXE()<s_posX && npc->getMoveStatus()==RPG_NPC::NPC_LEFT)
		s_posX = npc->getPosXE();
	if(npc->getPosXE()+npc->getTileW()>e_posX && npc->getMoveStatus()==RPG_NPC::NPC_RIGHT)
		s_posX = npc->getPosXE() + npc->getTileW() - m_smallView.width;

	m_visibleWindow.x = s_posX - GAME_WINDOW_W/4;
	m_visibleWindow.y = s_posY - GAME_WINDOW_H/4;
	adjustWindow();	
}

void RPG_NS_RenderEngine::RPG_RenderEngine::adjustWindow(){
	RPG_NS_Map::SizeType size;
	size = m_mapRender->getMap()->getMapSize_E();
	//�н�
	if(m_visibleWindow.x<0) m_visibleWindow.x = 0;
	if(m_visibleWindow.x+m_visibleWindow.width>size.width)
		m_visibleWindow.x = size.width - m_visibleWindow.width;
	if(m_visibleWindow.y<0) m_visibleWindow.y = 0;
	if(m_visibleWindow.y+m_visibleWindow.height>size.height)
		m_visibleWindow.y = size.height - m_visibleWindow.height;
	//�������������Ӵ�
	m_largeView.x = m_visibleWindow.x - 50;
	m_largeView.y = m_visibleWindow.y - 50;
	m_largeView.height = m_visibleWindow.height + 100;
	m_largeView.width = m_visibleWindow.width + 100;

	m_smallView.x = m_visibleWindow.x + GAME_WINDOW_W/4;
	m_smallView.y = m_visibleWindow.y + GAME_WINDOW_H/4;
	m_smallView.height = m_visibleWindow.height - GAME_WINDOW_H/4 - 150;
	m_smallView.width = m_visibleWindow.width - GAME_WINDOW_W/4 - 150;
}


void RPG_NS_RenderEngine::RPG_RenderEngine::RPG_Render(){
	
	SDL_Surface* backFrame;
	SDL_Surface* frontFrame;
	
	backFrame = m_mapRender->getBackMapBuf();
	frontFrame = m_mapRender->getFrontMapBuf();
	//�������Ⱦ����
	SDL_BlitSurface(backFrame, NULL, m_scene, NULL); 
	RPG_Sprite_Render();
	SDL_BlitSurface(frontFrame, NULL, m_scene, NULL); 
	RPG_SpecialScene();	//���ⳡ������

}

void RPG_NS_RenderEngine::RPG_RenderEngine::RPG_Sprite_Render(){
	//��Ⱦ�ӻ���
	RPG_NS_NPC::RPG_NPC *npc;
	for(int iNPC=0; iNPC<m_renderList->size(); iNPC++){
		npc = (*m_renderList)[iNPC];
		if(npc->getPosXE()>=m_largeView.x && npc->getPosXE()<=m_largeView.x+m_largeView.width
			&& npc->getPosYE()>=m_largeView.y && npc->getPosYE()<=m_largeView.y+m_largeView.height){
				if(npc->getBelongsToMap()==m_currentMap && npc->isHide()==false){
				//�ӻ��������Ⱦ����
					npc->renderNPC(m_scene);
				}
		}
	}
}

void RPG_NS_RenderEngine::RPG_RenderEngine::RPG_SpecialScene(){//��Ч��������

}

