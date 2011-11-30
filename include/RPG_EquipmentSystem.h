#ifndef HEADER_RPG_EQUIPMENTSYSTEM
#define HEADER_RPG_EQUIPMENTSYSTEM

/***********
*******����װ�����༰����װ��������
������:
ͷ��:1�� ���0
�·�:1�� ���1
Ь��:1�� ���2
����:1�� ���3
����:1�� ���4
ͷ��:1�� ���5
����:1�� ���6

������:
����:2�� ���7,8
����:1�� ���9
����:2�� ���10,11
��ָ:1�� ���12
�����:3�� ���13,14,15

***********/
#include <vector>
#include <string>
#include <SDL\SDL.h>
#include "game.h"
#include "GUI.h"
using std::vector;
using std::string;

namespace RPG_NS_EquipmentSystem
{
	class RPG_EquipmentSystem:public GUI::GUIObject
	{
		public:
			static const int EQP_TOUKUI = 0;
			static const int EQP_YIFU = 1;
			static const int EQP_XIEZI = 2;
			static const int EQP_WUQI = 3;
			static const int EQP_DUNPAI = 4;
			static const int EQP_TOUJING = 5;
			static const int EQP_YAODAI = 6;
			static const int EQP_ERHUAN = 7;
			static const int EQP_XIANGLIAN = 9;
			static const int EQP_SHOUZHUO = 10;
			static const int EQP_JIEZHI = 12;
			static const int EQP_HUSHENFU = 13;

			static const int EQP_NULL = 0; //û��ʵ��
			static const int EQP_LIVE = 1; //���ʵ��
			static const int EQP_DEAD = 2; //����,��Ҫɾ��
		
		private:
			static RPG_EquipmentSystem* instance;
			static int status;
			int m_heroIndex; //��ǰ��ʾ��Ӣ��
			vector<Item> m_itemList; //������Ʒ����ϸ��Ϣ
			

			string intToString(int n);
			/*******************��ť����**********************/
			void onLeft();
			void onRight();
			void onUninstall();
			void onInstall();
			void onDrink();
			void onExit();
			static void onLeftHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onLeft();
			}
			static void onRightHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onRight();
			}
			static void onUninstallHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onUninstall();
			}
			static void onInstallHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onInstall();
			}
			static void onDrinkHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onDrink();
			}
			static void onExitHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onExit();
			}
			/*************************************************/
		private://GUI
			static string equipLabel[16];
			static string statLabel[5];
			string m_hasEquip[16]; //16��װ��λ�õĸ���װ������
			
			//��ť
			GUI::GUIButton* m_button_left; //����ť
			GUI::GUIButton* m_button_right; //���Ұ�ť
			GUI::GUIButton* m_button_uninstall; //ж��װ��
			GUI::GUIButton* m_button_install; //װ����ȥ
			GUI::GUIButton* m_button_drink; //��ҩ
			//ͼƬ
			RPG_NS_Image::RPG_Image* m_image_back; //����ͼƬ
			RPG_NS_Image::RPG_Image* m_image_hero; //Ӣ�۵�ͼƬ
			//������
			GUI::GUIProgressBar* m_progressbar_blood; //Ѫ
			GUI::GUIProgressBar* m_progressbar_mana; //ħ
			GUI::GUIProgressBar* m_progressbar_cp; //CP
			//�б��
			GUI::GUIListBox* m_listbox_hequip; //Ӣ�����ϵ�װ���б�
			GUI::GUIListBox* m_listbox_pequip; //�����е�װ���б�
			GUI::GUIListBox* m_listbox_drug; //�����е�ҩƷ�б�
			//��ǩ��
			/**************************************/
			GUI::GUILabel* m_label_name; //��������
			GUI::GUILabel* m_label_level; //����ȼ�
			GUI::GUILabel* m_label_experience; //����,��һ������
			GUI::GUILabel* m_label_blood; //Ѫ,�ٷֱ�
			GUI::GUILabel* m_label_mana; //ħ,�ٷֱ�
			GUI::GUILabel* m_label_cp; //CP,�ٷֱ�
			GUI::GUILabel* m_label_attribute[5]; //������,�������,ħ������,ħ������,�ٶ���Ϣ
			GUI::GUILabel* desc1, *desc2, *desc3;
			/**************************************/
			GUI::GUILabel* m_label_equipLabel[16];//װ���������ǩ
			/**************************************/
			GUI::GUIButton* m_button_exit; //�ص���Ϸ����İ�ť


			void showHero(int index); //��ʾӢ�۵������Ϣ	
			Item getItemInfo(string material);//�������Ϊmaterial����Ʒ��ϸ��Ϣ
			void delAllGUI(); //ɾ�����˲������Щ�ؼ�֮���GUI�ؼ�
			void setNULL();

		public:
			RPG_EquipmentSystem();
			~RPG_EquipmentSystem();
			static void render(SDL_Surface* destSurf);
		
	};
}


#endif