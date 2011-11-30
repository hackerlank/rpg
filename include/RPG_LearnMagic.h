#ifndef HEADER_RPG_LEARNMAGIC
#define HEADER_RPG_LAERNMAGIC

/******************
******data/learnMagic/*.learn ĳ��npc���Խ��ڵ�ħ��
�ļ�����mapID*U*.learn
�ļ���ʽ:
ħ������
ħ������
�۸�
��1 ��2 ��3 ��4 ;ѧϰ��ħ��Ӣ����Ҫ���ٵ���
END
******************/

#include <vector>
#include <SDL\SDL.h>
#include "game.h"
#include "GUI.h"
#include "RPG_Image.h"
using std::vector;

namespace RPG_NS_LearnMagic
{
	typedef struct{
		char magicName[MAX_NAME]; 
		char magicDesc[256];
		int cost;
		int soul[4];
	}MagicItem;

	class RPG_LearnMagic : public GUI::GUIObject
	{
		public:
			static const int LEARN_NULL = 0;
			static const int LEARN_LIVE = 1;
			static const int LEARN_DEAD = 2;

			static const int HEROBASEX = 430;
			static const int HEROBASEY = 115;
			static const int LEARNBASEX = 150;
			static const int LEARNBASEY = 100;

		private:
			static RPG_LearnMagic* instance;
			static int status;
			vector<MagicItem> magicList; //��NPC����ħ���б�
			int m_currentHero;

			MagicItem getMagicItem(string magiccName);

		private: //GUI
			RPG_NS_Image::RPG_Image* m_back;
			GUI::GUIMessageBox* m_message;
			////////////////Ӣ����ؽ���/////////////////
			GUI::GUIButton* m_button_left; //����ť
			GUI::GUIButton* m_button_right; //���Ұ�ť	
			GUI::GUIButton* m_button_add1; //�ӻ�1
			GUI::GUIButton* m_button_add2; //�ӻ�2
			GUI::GUIButton* m_button_add3; //�ӻ�3
			GUI::GUIButton* m_button_add4; //�ӻ�4

			GUI::GUILabel* m_label_name; //Ӣ������
			GUI::GUILabel* m_label_zsh; //"���Ļ�:"
			GUI::GUILabel* m_label_totSoul; //������ǰӢ���ܵ��Ļ�
			GUI::GUILabel* m_label_pointSoul; //ʣ��ĵ���
			GUI::GUILabel* m_label_soul[4]; //ʣ���

			GUI::GUIListBox* m_listbox_heroMagic; //Ӣ�۵�����ħ��

			////////////////������ؽ���//////////////////
			GUI::GUIButton* m_button_buy; //����ť
			GUI::GUIButton* m_button_exit; //�˳���ť

			GUI::GUIListBox* m_listbox_learnMagic; //����ħ��

			GUI::GUILabel* m_label_desc;
			GUI::GUILabel* m_label_cost;
			GUI::GUILabel* m_label_needSoul;
			GUI::GUILabel* m_label_money; //��ҽ�Ǯ


			/////////////////��ť�¼�////////////////////
			void onBuy();
			void onExit();
			void onLeft();
			void onRight();
			void onAdd1();
			void onAdd2();
			void onAdd3();
			void onAdd4();
			static void onBuyHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onBuy();
			}
			static void onExitHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onExit();
			}
			static void onLeftHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onLeft();
			}
			static void onRightHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onRight();
			}
			static void onAdd1Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd1();
			}
			static void onAdd2Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd2();
			}
			static void onAdd3Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd3();
			}
			static void onAdd4Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd4();
			}


			////////////////////////////////////////////
			void createHeroInterface(int index); //�ع�Ӣ����ص�GUI

		public:
			RPG_LearnMagic(int mapId, int unicode); //��ʶ��һ��npc
			~RPG_LearnMagic();
			static void render(SDL_Surface* destSurf);
			
	};
}

#endif