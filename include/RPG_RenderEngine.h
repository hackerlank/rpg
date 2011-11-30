#ifndef HEADER_RPG_RENDERENGINE_H
#define HEADER_RPG_RENDERENGINE_H
/*
	��ͼ��Ⱦ���棺
	��Ⱦ��ͼ�����Ӳ�->NPC�Ӳ�->��ͼǰ����->��Ч������
	֧�ֹ���

*/

#include "RPG_NPC.h"
#include "RPG_MapRender.h"
#include "SDL\SDL.h"
#include "GUI.h"

namespace RPG_NS_RenderEngine{
	class RPG_RenderEngine
	{
	private:
		SDL_Surface* m_scene;		//����������
		vector<RPG_NS_NPC::RPG_NPC*> *m_renderList;	//NPC��Ⱦ����
		RPG_NS_MapRender::RPG_MapRender *m_mapRender;	//��ͼ��Ⱦ��
		RPG_NS_Map::RectType m_visibleWindow;	//�Ӵ�
		RPG_NS_Map::RectType m_largeView;		//���Ӵ�(�����ڴ˴����ڵ�NPC��Ҫ����Ⱦ)
		RPG_NS_Map::RectType m_smallView;		//С�Ӵ�(�����ڴ˴������ƶ����ᴥ������)
		int m_currentMap; //Ŀǰ��ͼ���
		int m_special;	//��Ч�����������

	public:
		RPG_RenderEngine(vector<RPG_NS_NPC::RPG_NPC*> *renderList, RPG_NS_MapRender::RPG_MapRender *mapRender,
							RPG_NS_Map::RectType visibleWindow);
		~RPG_RenderEngine(void);
	public:
		void RPG_Render();
		void RPG_Sprite_Render();
		void setMapRender(RPG_NS_MapRender::RPG_MapRender *mapRender) {m_mapRender = mapRender;}
		void renderEngine_Init();	//��ʼ����Ⱦ����
		RPG_NS_Map::RectType getvisibleWindow() {return m_visibleWindow;}
		RPG_NS_Map::RectType getlargeView() {return m_largeView;}
		RPG_NS_Map::RectType getsmallView() {return m_smallView;}
		RPG_NS_MapRender::RPG_MapRender* getMapRender() {return m_mapRender;}
		SDL_Surface* getSceneBuf() {return m_scene;}
		void setvisibleWindow(RPG_NS_Map::RectType rect) { m_visibleWindow = rect;}
		void setlargeView(RPG_NS_Map::RectType rect) { m_largeView = rect;}
		void setsmallView(RPG_NS_Map::RectType rect) { m_smallView = rect;}
		void setCurrentMap(int id) { m_currentMap = id;}
		void adjustWindow();	//�����ӿڵ����Ӵ�
		void scrollScreen();	//��������λ�ù������д���
		void RPG_SpecialScene();	//��Ч��������
		void RPG_changMap(RPG_NS_MapRender::RPG_MapRender *mapRender, RPG_NS_Map::RectType vw);	//�л���ͼ
	};
}

#endif
