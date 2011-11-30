#ifndef HEADER_RPG_SPRITEMANAGER_H
#define HEADER_RPG_SPRITEMANAGER_H

#include "RPG_Map.h"
#include "RPG_NPC.h"
#include "RPG_EventManager.h"

using RPG_NS_Map::RPG_Map;
using RPG_NS_NPC::RPG_NPC;
using RPG_NS_EventManager::RPG_EventHandleInterface;
using RPG_NS_EventManager::RPG_EventManager;

namespace RPG_NS_SpriteManager
{
/*
 npc?.npc�ĸ�ʽ��
   ���ֲ�struct:��������

   ս��ϵͳ��struct:�𴦶���
*/

	typedef struct {
		int moveSpeed; //�ٶ�
		int splitX;
		int splitY; //��ȸ��߶��ϵķֿ���
		int upLeft; //������ʱ���֡���ڱ��
		int upRight;
		int upStop;
		int downLeft;
		int downRight;
		int downStop;
		int leftLeft;
		int leftRight;
		int leftStop;
		int rightLeft;
		int rightRight;
		int rightStop;
		int dir; //��ʼ�ĳ���
	} NPC_data;

	class RPG_SpriteManager: RPG_EventHandleInterface
	{
		public:
			static const int UNICODE_BASE = 20000; //һ���ϴ������ֹ�Զ���������ͼ�е�ID�ظ�

		public:
			static vector<RPG_NPC*> g_collisionList; //ǿ��ײ���Ľ������
			static vector<RPG_NPC*> g_collisionList_non; //����ײ���Ľ������
			static vector<RPG_NPC*> g_collisionList_saying; //ֻ������������ͬ��NPC�����ڸ������ײ
			static void delCollisionList(int i);
			static void delCollisionList_non(int i);
			static void delCollisionList_saying(int i);
			void onRButtonDown(int mx, int my);
			void onKeyDown(SDLKey sym);
			void onRButtonOrSpace();

		private:
			static RPG_Map* m_pMap; //��ǰ��ͼ
			static vector<RPG_NPC*> m_spriteList; //�ӻ������
			static int getNPCUnicode(); //���һ�������������NPC��һ���ı��,cong 
			static void adjustment(RPG_NPC* npc); //��������ײʱnpcλ�õĵ��� 
			static bool m_doCollision; //�Ƿ�����ײ���
			static RPG_SpriteManager* instance;

		public:
			static void setCurrentMap(RPG_Map* pMap);
			static void setCollision(bool coll) {m_doCollision = coll;}
			static void loadNPC(RPG_Map* pMap); //�����ͼ�е�NPC
			static void killNPC(int mapID); //ɾ����ͼ���ΪmapID�е�����NPC
			static void killNPCByIDs(int mapID, int NPCUnicode);
			static void killAllNPC(); //ɾ�������Զ����ӻ�����������ӻ���
			static int addNPC(int NPCID, int NPCUnicode, int mapID, int x, int y);
			static int addNPC(int NPCID, int NPCUnicode, RPG_Map* pMap, int x, int y);
			static int addNPC(int NPCID, int NPCUnicode, RPG_Map* pMap, int mapID, int x, int y); //�������mapID��NPC,����NPCUnicode
			static void clearList(); //ɾ�������ӻ��棬��ն���
			static vector<RPG_NPC*>* getSpriteList() {return &m_spriteList;}
			static RPG_NPC* getNPC(int mapID, int NPCUnicode);
			static void setMap(int mapID, RPG_Map* pMap); //���ӻ����б���mapID���ӻ����ͼָ��ָ��pMap
			
		public:
			static void init();
			static void updateSprites(); //���µ�ǰ��ͼ�е��ӻ���״̬
			static void detectCollision(); //��ײ���
			static void killDeadSprite(); //ɾ���������ӻ���,һ��ս���󱻵���
	};
}



#endif