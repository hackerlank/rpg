#ifndef HEADER_RPG_SHOPINGSYSTEM
#define HEADER_RPG_SHOPINGSYSTEM

/*******
****data\shop\*.shopĳ�����˵�������Ʒ
�ļ���ʽ:
����װ��
~
ҩƷ
~
����
END
******
ÿ������һ���ļ������캯����mapID��unicodeID�������ļ�����
����
mapID:5
UnicodeID:1
�ļ���:mapID5U1.shop
********/

#include <SDL\SDL.h>
#include <string>
#include "GUI.h"
#include "game.h"
#include "RPG_Config.h"
#include "RPG_Image.h"
using std::string;

namespace RPG_NS_ShopingSystem
{
	class RPG_ShopingSystem: public GUI::GUIObject
	{
		public:
			static const int SHOP_NULL = 0; //��ʾ��ǰû�п�������ϵͳ
			static const int SHOP_LIVE = 1; //��ʾ��ǰ��������ϵͳ
			static const int SHOP_DEAD = 2; //��ʾ�ر�����ϵͳ,��Ҫɾ��ʵ��
			static const int SHOP_STARTX = GAME_WINDOW_W/2-100;
			static const int SHOP_STARTY = GAME_WINDOW_H/2-200;
			static const int SHOP_TOP = 0; //��ʾ��ʾ���㰴ť
			static const int SHOP_BUY = 1; //��ʾ��ʾ��������
			static const int SHOP_SALE = 2; //��ʾ��ʾ����������
			static const int SHOP_SAY = 3; //��ʾ��ʾ˵������

		private:
			static int status; //��ǰ��״̬(SHOP_NULL, SHOP_LIVE, SHOP_DEAD)
			static RPG_ShopingSystem* instance; //ʵ��
			vector<Item> m_itemList; //������Ʒ���б�,������Ϣ��
			vector<string> m_weaponList; //��������װ��
			vector<string> m_drugList; //����ҩƷ
			vector<string> m_jewelryList; //��������
			int m_interface; //��ǰ��ʾ�Ľ���
			int m_wdj; //����ʱ��ǰ��"����","ҩƷ"����"����" 0 1 2
			int m_mapID;
			int m_npcUnicode;
			bool m_doExchanging; //��ʾ��ǰ�Ƿ����ڽ���
			void doBuying(); //���ȷ�Ϻ����붫��
			void doSaling(); //���ȷ�Ϻ���������

		private://GUI
			GUI::GUIButton* m_button_top_buy; //�����"����"��ť
			GUI::GUIButton* m_button_top_sale; //�����"������"��ť
			GUI::GUIButton* m_button_top_say; //�����"̸��"��ť
			RPG_NS_Image::RPG_Image* m_back; //����
			GUI::GUIButton* m_button_left; //����ѡ���
			GUI::GUIButton* m_button_right; //����ѡ���
			GUI::GUILabel* m_label_name; //��ʾ"����","ҩƷ","����"
			GUI::GUIListBox* m_listbox_items; //��ʾ��Ʒ�б�
			GUI::GUILabel* m_label_desc; //��ʾ����Ʒ������
			GUI::GUILabel* m_label_cost; //��ʾ��Ʒ�ļ۸�
			GUI::GUILabel* m_label_count; //��ʾ���ӵ����Ʒ������
			GUI::GUIButton* m_button_ack; //"��","��" ��ť
			GUI::GUIButton* m_button_exit; //�˳���ť
			GUI::GUIMessageBox* m_messagebox; //ȷ����Ϣ
			GUI::GUILabel* m_label_money;
			void onTopBuy();
			void onTopSale();
			void onTopSay();
			void onLeft();
			void onRight();
			void onAck();
			void onExit();

			static void TopBuyHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onTopBuy();
			}
			static void TopSaleHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onTopSale();
			}
			static void TopSayHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onTopSay();
			}
			static void LeftHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onLeft();
			}
			static void RightHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onRight();
			}
			static void AckHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onAck();
			}
			static void ExitHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onExit();
			}
				
			void initInterface(int interfaceID); //��ʼ����Ӧ�Ľ���

		public:
			RPG_ShopingSystem(int mapID, int unicode); //��������ϵͳ, ��Ʒ�ļ�ΪfileName
			//~RPG_ShopingSystem();
			static void render(SDL_Surface* destSurf); //ÿ����ִ��

	};
}



#endif