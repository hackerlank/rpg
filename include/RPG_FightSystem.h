#ifndef HEADER_FIGHTSYS_H
#define HEADER_FIGHTSYS_H
#include "game.h"
#include <vector>
#include "RPG_NPC.h"
#include "RPG_Map.h"
#include "SDL\SDL.h"
#include "GUI.h"
#include "RPG_EventManager.h"

#define MAX_GET_EQUIPMENT 10//NPC���ɵ�װ����
#define MAX_NPC_MAGIC 5//NPC����ʹ��ħ���� 
#define MAX_ATTIME 1000 //ATʱ���յ�
//#define MAP_WIDTH 10 //ս����ͼ��С
//#define MAP_HEIGHT 10 


using std::vector;

namespace RPG_NS_FightSys{//ս��ϵͳ

	
	typedef struct{
		int mgxs; //ħ��ϵ��
		int zdl;//�¶���
		int zdsh;//�¶��˺�
		int zsml;//��˯����
		int fml;//��ħ��
		int speeddown;//����
		int stringdown;//����
		int defdown;//����
	}Magic_attack;//ħ����������
	
	typedef struct{
		int hx;//��Ѫ��
		int speedup;//����
		int stringup;//�ӹ�
		int defup;//�ӷ�
	}Magic_def;//ħ����������
		
	typedef struct{
		char name[MAX_NAME];//ħ������
		int type;//ħ����������(0:�з����� 1:�з�Ⱥ�� 2:�ҷ����� 3:�ҷ�Ⱥ��)
		int length;//��������
		int slfw;//Ⱥ����ħ�����÷�Χ�������پ��룩
		int manacost;//ħ������
		Magic_attack attack;
		Magic_def defend;
	}Magic;//ħ��

	//
	typedef struct{
		int wg; //�﹥buf + -
		int wgCounter;//�﹥bufʣ��غ���
		int wf;//���
		int wfCounter;
		int dsh;//���˺�
		int dCounter;
		int speed;//�ٶ�buf
		int speedCounter;
		int sleepCounter;//˯��ʣ��غ���
		int fmCounter;//��ħʣ��غ���
	}Buf;//����״̬

	typedef struct{
		HeroStat state;
		int fid;//ս��id
		int attacklength;//�﹥��Χ
		int movelength;//�ƶ���Χ
		Buf buf;//����״̬
	}Hero;

	typedef struct{
		int fid;//ս��ID
		int hp;
		int wg;
		int wf;
		int mg;
		int mf;
		int speed;
		int attacklength;//�﹥��Χ
		int movelength;//�ƶ���Χ
		Buf buf;//����״̬
		char magic[MAX_NPC_MAGIC][MAX_NAME];//npcЯ����ħ��
		//
		int exp;//ɱ���ɵþ�����
		int money;//��Ǯ��
		char equipment[MAX_GET_EQUIPMENT][MAX_NAME];//�ɵ�װ��
		double rate[MAX_GET_EQUIPMENT];//��Ӧװ���ɵ�����
	}NPC;

	class RPG_FightSystem : public GUI::GUIObject, RPG_NS_EventManager::RPG_EventHandleInterface
	{
	public:
		static const int ATTACK = 0;
		static const int MOVE = 0;
		static const int MAGIC = 0;
		static const int MEDICIN = 0;

	private:
		vector<Magic> magic_list;//
		vector<Hero> hero_list;
		vector<NPC> npc_list;//ս��npc����
		vector<RPG_NS_NPC::RPG_NPC> npclist;//npc��ʾ����
		vector<int> action_list;//�ж�����
		int m_numOfP; //����ս��ϵͳ�еĹ�������
		RPG_NS_Map::RPG_Map *m_map;
		SDL_Rect m_area; //ս������
		SDL_Surface* m_pScreen;
		GUI::GUIButton* m_btnMove;//����ƶ���ť
		GUI::GUIButton* m_btnAttack;//������
		GUI::GUIButton* m_btnMagic;//ʹ��ħ��
		GUI::GUIButton* m_btnMedicine;//��ҩ
		//
		bool fight_over;//ս��ϵͳ�Ƿ���
		int action_sel;//���ѡ���ж����� 0:���� 1:�ƶ� 2��ħ�� 3:ҩ
		int current_npc;//��ǰ���npc,Ӣ�۱��Ϊ0~2��������3��ʼ���
		vector<int> npcATtime; //��������ATʱ��,��ʱ�䳤��Ϊ1000
		bool waitMove;//�ȴ����move����
		//
		void Internal_init();//�ڲ�������ʼ��
		int ATreach();//���ص���ʱ�̵�λ��ս��id
		void playerCtl(); //��ҿ���
		void NPC_Render();	
		void render();
		void onMove(); //�������ƽ���ġ��ƶ����¼�
		
		//
		static void move_Helper(GUI::GUIObject* object){
			((RPG_FightSystem*)object)->onMove();
		}
 	public:
		static void RPG_runFightSys(int npcID, int npcUni);//�����npcID������սϵͳ���npc ID����Ҫ
														//ͨ��ת���ļ� npcIDtrans.dat ����ת��
		void loadHero();
		void loadNPC(int npcID, int npcUni);//����npc
		void loadMagic();
		void fightCycle();//����ս��ϵͳ��ѭ��
		void initMap();

		RPG_FightSystem(int npcID, int npcUni);
		~RPG_FightSystem(void);
		virtual void onLButtonDown(int mx, int my);
		virtual void onRButtonDown(int mx, int my);
	};
}
#endif
