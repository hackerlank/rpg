#ifndef HEADER_RPG_SCENEMANAGER_H
#define HEADER_RPG_SCENEMANAGER_H
/*
	����������������Ⱦ������ӻ�����������ϣ�
				�����Ƶ�������ڵ�ͼ�л�ʱ������Ч
	��ͼ���л�:����ͼ�����NPC
			   ����NPC��ͼָ��
			   ��Ⱦ��������ͼ���ӻ��棬��Ч��
   ˢ�ַ��������ͼ֮���л�ʱ�����NPC����������
			 ��ͼ�֣����������е�ͼ�֣������ڵ�ͼ֮��
			 �Լ����������ͼ��ͼ�л�ʱֻ�е�һ�ν���
			 ��ͼʱ���أ����Ҳ����NPC���С�
*/
#include <map>
#include "RPG_SpriteManager.h"
#include "RPG_Map.h"
#include "RPG_MapRender.h"
#include "RPG_RenderEngine.h"
#include "RPG_NPC.h"
#include "GUI.h"

//�����зַ�ʽ
#define TILED_SPLIT_X 70//
#define TILED_SPLIT_Y 1//	

namespace RPG_NS_SceneManager{
	class RPG_SceneManager
	{
	private:
		static int m_currentMap;	//��ǰ��ͼID
		static RPG_NS_Map::RPG_Map* m_map;	//
		static RPG_NS_MapRender::RPG_MapRender* m_mapRender; //
		static RPG_NS_RenderEngine::RPG_RenderEngine* m_renderEngine;
		static bool m_SMisExist;	//�����������Ƿ��Ѿ���ʼ��
		static bool bChange; //�Ƿ���Ҫ�л���ͼ
		static std::map<int, bool> m_mapFlag;	//����Ƿ��һ�ν���õ�ͼ
		static SDL_Surface* m_pScreen;	//��Ļ�Դ�
		static bool m_ableChange;	//�Ƿ������л���ͼ
		static int m_climateType;	//��ǰ����
		//��������
		static const int CLIMATE_BASE = 0x100000;
		static const int SUN = CLIMATE_BASE+0x0000;//��
		static const int RAIN = CLIMATE_BASE+0x0001;//����
		static const int THUNDER = CLIMATE_BASE+0x0002;//����
		static const int DUSK = CLIMATE_BASE+0x0004;//�ƻ�
		static const int DARK = CLIMATE_BASE+0x0008;//����
		static const int WINDY = CLIMATE_BASE+0x00016;//�ҷ�
		//other
		static GUI::GUILabel *m_mapName;//�ڵ�ͼ�л�ʱ��ʾ�ĵ�ͼ����
		static GUI::GUILabel *m_mapName2;
		static int m_mapNameIndex; //��ʱ����
		static string oldmapname;
		static RPG_NS_Image::RPG_Image *m_smallMap;//С��ͼ

	public:	
		static SDL_Rect m_gameWindow;	//��Ϸ����
		
	public:
		static void RPG_SceneManager_Init(std::string mapName, std::string tiledName,
										 int hero_X, int hero_Y, SDL_Surface* pScreen);
		static void changeMap(std::string mapName, std::string tiledName, 
								int hero_X, int hero_Y);	//�����л���ͼ
		static void RPG_GameRender();	//��Ⱦ����
		static void On_detectChange();	//����Ƿ������л���ͼҪ��,����Ϣ�����е�����
		static int getCurrentMap() {return m_currentMap;}
		static RPG_NS_Map::RPG_Map* getMap() {return m_map;}
		static RPG_NS_MapRender::RPG_MapRender* getMapRender() {return m_mapRender;}
		static RPG_NS_RenderEngine::RPG_RenderEngine* getRenderEngine() {return m_renderEngine;}
		static void setCurrentMap(int cm) { m_currentMap = cm;}	
		static bool isLoadNPC(int mapID);
		static bool isInGameW(int posXE, int posYE);	//�ж�����Ƿ�����Ϸ����������
		static RPG_NS_Map::PosType convertCoordinate(int posXE, int posYE);//�������������ת��Ϊ��ͼ
																		   //��������
		static bool canChange() {return m_ableChange;}
		static bool ableChange(bool state) {m_ableChange = state;}
		static int getClimate() {return m_climateType;}
		static void setClimate(int climate) {m_climateType = climate;} 
		static void RPG_ShowMapname();

		static SDL_Surface* getScreenbuf() {return m_pScreen;}
	};

}
#endif

