/**********
*******data/ai/npcmove/all.npcmove
������NPC�ڵ�ͼ�еĻ���
����
1 ��npcId����
5 ; �˶�������Χ
4 ��4���˶�
0 ; 0��ʾ��������Χ������ƶ�����������Χ�����һ���ص�
1 10; 1��ʾ��׷�ϣ�����ײʱһ����ȡ����Ϊ�� 10��ʾ����׷�Ϸ�Χ
2 ; �����˶����ߵ���ߵ�������Χ���ص�ԭ�㣬�������ٻ���
3 ; �����˶���������
-1 ; �ļ�����

***********/



#ifndef HEADER_RPG_AI_H
#define HEADER_RPG_AI_H

#include "RPG_NPC.h"
#include "RPG_SpriteManager.h"
#include <vector>

using std::vector;
using RPG_NS_NPC::RPG_NPC;
using RPG_NS_SpriteManager::RPG_SpriteManager;

namespace RPG_NS_AI
{
	typedef struct{
		int npcId;
		int length; //��NPC���˶���������Χ
		int actionLength; //�ж������˶�����
		int actionType[4]; //�˶������ͣ�������׷��
		int huntLength; //�����׷����Ϊ����ô׷�ϵ�������Χ������Ϊ-1
	}NPCAction; //����ĳ��NPCID����Ϊ

	typedef struct{
		int mapId;
		int npcId; 
		int npcUnicode; //��NPC�ĵ�ͼ��Ψһ����
		RPG_NPC* npc; //��NPC��ָ��
		int orgX, orgY; //��NPC��һ�γ���ʱ��λ��
		int nowX, nowY; //��NPC��ǰλ��
		vector<int> actionList; //��������
		bool inHunt; //��ǰ����Ϊ�Ƿ���׷����Ϊ
		bool goHome; //��ǰ����Ϊ�ǻ�ԭ�����Ϊ
	}AIItem;

	class RPG_AI
	{
		private:
			static vector<NPCAction> m_npcActionList; //����NPC����Ϊ
			static vector<AIItem> m_aiItem; //ά���ŵ�ǰnpc�ľ�����Ϊ, ��Ҫÿ֡����

			static void updateList(); //����m_aiItem���У����ӻ�����б���һ��
			static NPCAction getActionInfo(int npcId); //����npcId�õ������͵���Ϊ

		public:
			static void RPG_AI_Init(); //��ʼ����������Ϊ�ļ���
			static void AIMove(); //ÿ֡���ã�ʵ�֣���
	};
}


#endif HEADER_RPG_AI_H