#ifndef HEADER_RPG_NPC_H
#define HEADER_RPG_NPC_H

/*
	�����ڽ���ש֮���߶�����: [����]->���,����,�ҽ�,����
	�����÷�:
		�ȹ���,��λͼ��Ƭ,�����ٶȣ���������λ�����꣨Ҫ��֤ƥ�䣩��
		[���ô�ש�Ŀ����](���캯�������pMap�Զ�����),����ĸ�����Ķ�������,
		����npc���,�����������ŵ�ͼ,���ó���
*/

#include <vector>
#include <string>
#include "RPG_Image.h"
#include "RPG_Map.h"
#include <SDL\SDL.h>
using std::vector;
using std::string;

namespace RPG_NS_NPC
{
	class RPG_NPC
	{
		protected:
			int m_moveSpeed; //��ʼ��NPC_UPDATE_DELAYһ��
			bool m_bHide; //�Ƿ���ʾ
			bool m_bDead; //�Ƿ�����
			int m_NPCID; //npc���ͱ��
			int m_NPCUnicode; //npcΨһ���
			int m_belongsToMap; //��npc�������ŵ�ͼ

		public:
			static const int NPC_UP = -1;
			static const int NPC_DOWN = 1;
			static const int NPC_LEFT = -2;
			static const int NPC_RIGHT = 2;
			static const int NPC_UPDATE_DELAY = 4; //update��������֡������һ��(�ܾ����ٶ�)

		protected:
			RPG_NS_Image::RPG_Image* m_image; //NPC�����λͼ
			RPG_NS_Map::RPG_Map* m_map;
			vector<int> m_upMotion; //���ÿ�������λͼ��Ƭ���
			vector<int> m_downMotion;
			vector<int> m_leftMotion;
			vector<int> m_rightMotion;
			int m_posX, m_posY; //��ͼλ��
			int m_oldX, m_oldY; //��������ִ��ǰ������
			int m_posXE, m_posYE; //����λ��,��ͼ�е�����λ��(���Ͻ�)
			bool m_bBusy; //��ǰ�Ƿ���������ִ��
			int m_moveStatus; //����ִ�е�����(NPC_UP, NPC_DOWN, NPC_LEFT, NPC_DOWN),Ҳ��ʾ����
			int m_currentFrame; //��ǰ������λͼ��Ƭ
			int m_tileW, m_tileH; //��ש�ĳߴ�
			//int m_mapID; //��ͼID,���������m_map
			int tc; //����update����
			int upFre; //�û�update�ӳ�

		public:
			RPG_NPC(string imageFileName, RPG_NS_Map::RPG_Map* pMap);
			RPG_NPC(string imageFileName, int mapID);
			RPG_NPC(string imageFileName);
			~RPG_NPC();
		public:
			void splitImage(int cX, int cY);
			void addDirMotion(int dir, int index); //�����,����,�ҽ�,����,���,����,�ҽ�,���� ��ӵ���������
			void setPos(int x, int y);
			void setPosE(int x, int y);
			void setMoveSpeed(int s);
			void setTileSize(int w, int h);
			void setHide(bool isHide);
			void setDead(bool isDead);
			void setMoveStatus(int status); //���ó���
			void setNPCID(int id);
			void setBelongsToMap(int map);
			void setNPCUnicode(int uni);
			void setMap(RPG_NS_Map::RPG_Map* map);
			int getPosX() {return m_posX;}
			int getPosY() {return m_posY;}
			int getPosXE() {return m_posXE;}
			int getPosYE() {return m_posYE;}
			int getTileW() {return m_tileW;}
			int getTileH() {return m_tileH;}
			int getMoveSpeed() {return m_moveSpeed;}
			int getMoveStatus() {return m_moveStatus;}
			int getNPCID() {return m_NPCID;}
			int getBelongsToMap() {return m_belongsToMap;}
			int getNPCUnicode() {return m_NPCUnicode;}
			int getMapID() {return m_belongsToMap;}
			bool isHide() {return m_bHide;}
			bool isDead() {return m_bDead;}
			void moveUp();
			void moveDown();
			void moveLeft();
			void moveRight();
			void move(int dir);
			bool isBusy() {return m_bBusy;}
			void update(); //��λ�ò�һ��ʱҪ�������������ϵ�λ��
			bool renderNPC(SDL_Surface* surfDest);
			RPG_NS_Image::RPG_Image* getImage() {return m_image;}
			void undoCommand();

	};
}

#endif

