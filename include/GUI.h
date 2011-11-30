#ifndef HEADER_GUI
#define HEADER_GUI

#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <string>
#include <vector>
#include "RPG_EventManager.h"
#include "RPG_Image.h"
#include "font.h"
using RPG_NS_EventManager::RPG_EventHandleInterface;
using RPG_NS_EventManager::RPG_EventManager;
using RPG_NS_Image::RPG_Image;
using std::string;
using std::vector;

/****
GUIButton�����ⲿ�¼���ԭ��:
    
****/

namespace GUI
{
	void DrawPixel(SDL_Surface *screen,int x,int y,Uint8 R,Uint8 G,Uint8 B);
	void DrawLine (SDL_Surface* screen , int x1 , int y1 , int x2 , int y2 , Uint8 R, Uint8 G, Uint8 B);
	void DrawRect(SDL_Surface* screen, int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B, int thick);

	class GUIObject{
	public:virtual void draw(SDL_Surface* destSurf){};
	}; //������Ⱦ����
	typedef void (*FuncType)(GUIObject* object);
	typedef void (*FuncTypeEx)(GUIObject* object, int id);

	class GUIButton: public RPG_EventHandleInterface, public GUIObject
	{ 
		private:
			GUIObject* m_pParent;
			int m_id; //��ť���
			FuncType m_fClickEvent; //��갴�º��ɿ����¼�����
			FuncType m_fOverEvent; //����ƶ�����ť��ʱ���¼�����
			FuncType m_fOutEvent; //����뿪��ťʱ���¼�����
			FuncTypeEx m_fClickEventEx; //��갴�º��ɿ����¼�����
			FuncTypeEx m_fOverEventEx; //����ƶ�����ť��ʱ���¼�����
			FuncTypeEx m_fOutEventEx; //����뿪��ťʱ���¼�����
			void onClickEvent();
			void onOverEvent();
			void onOutEvent();
	
		private:
			int m_x, m_y; //��ť��λ��
			int m_w, m_h; //��ť�Ĵ�С
			RPG_Image* m_upImage; //����״̬�µİ�ťλͼ
			RPG_Image* m_downImage; //���°�ťʱ��λͼ
			int m_switch; //��ǰ��ͼʱҪ������λͼ
			bool m_isInButton; //��ǰ����Ƿ���Button��

		//Button����Ҫ�������¼�
		private:
			virtual void onLButtonDown(int mx, int my);
			virtual void onLButtonUp(int mx, int my);
			virtual void onMouseMove(int mx, int my);
			bool inside(int x, int y){
				return x>m_x && x<m_x+m_w && y>m_y && y<m_y+m_h;
			}

		public:
			GUIButton(string upImagef, string downImagef, int x, int y, GUIObject* pParent, int id);
			GUIButton(string upImagef, string downImagef, int x, int y, GUIObject* pParent);
			GUIButton(string upImagef, string downImagef, GUIObject* pParent);
			~GUIButton();

			void setfClickEvent(FuncType f) {m_fClickEvent = f;}
			void setfOverEvent(FuncType f) {m_fOverEvent = f;}
			void setfOutEvent(FuncType f) {m_fOutEvent = f;}
			void setfClickEventEx(FuncTypeEx f) {m_fClickEventEx = f;}
			void setfOverEventEx(FuncTypeEx f) {m_fOverEventEx = f;}
			void setfOutEventEx(FuncTypeEx f) {m_fOutEventEx = f;}
			void draw(SDL_Surface* destSurf);
	};

	class GUILabel: public GUIObject
	{
		private:
			static int LabelCount; //�����ʵ������
			int m_x, m_y; //��һ�����ֵ����Ͻ�
			int m_height; //���ֵĸ߶�
			TTF_Font* m_pFont; //����
			vector<SDL_Surface*> m_textSurface; //�������֣�ÿ�����ֵı���

		private:
			vector<string> splitString(string strSrc, string strSep);
			
		public:
			GUILabel(const string& font, int size, const string& message, 
				int r, int g, int b, int x, int y);
			GUILabel(const string& font, int size, const string& message, 
				int r, int g, int b);
			~GUILabel();
			
			int getHeight() {return m_height;}
			void setPos(int x, int y) {m_x = x; m_y = y;}
			void draw(SDL_Surface* destSurf);
	};

	class GUIMessageBox: public GUIObject
	{
		public:
			static const int MB_NULL = -1;
			static const int MB_YES = 0;
			static const int MB_NO = 1;

		private:
			RPG_Image* m_dlg; //�Ի�����ı���ͼƬ
			GUILabel* m_caption; //������
			GUILabel* m_message; //��Ϣ����
			GUIButton* m_yes; //yes��ť�����
			GUIButton* m_no; //no��ť�����
			int m_x, m_y;
			int m_return;
			void freeControls();
			void onYesClick(void);
			void onNoClick(void);
			static void onYesClickHelper(GUIObject* object) {((GUIMessageBox*)object)->onYesClick();}
			static void onNoClickHelper(GUIObject* object) {((GUIMessageBox*)object)->onNoClick();}

		public:
			GUIMessageBox(string caption, string message, string dlg, string yes1, 
				string yes2, string no1, string no2, int x, int y, string font);
			~GUIMessageBox();

			void draw(SDL_Surface* destSurf);
			int getSelection() {return m_return;}
	};

	class GUIListBox: public RPG_EventHandleInterface, public GUIObject
	{
		private:
			int m_x, m_y;
			int m_w, m_h;
			int m_num; //�б������ʾ���������
			int m_topNum; //�б���ʾʱ��ǰ��˵��б�����
			int m_fontH; //����ĸ߶�
			vector<GUILabel*> m_itemList; //����б���
			vector<string> m_itemValue; //����б��������
			GUIButton* m_toUp;
			GUIButton* m_toDown;
			int m_selectItem;

		private:
			static void onToUpClickHelper(GUIObject* object){
				((GUIListBox*)object)->onToUpClick();
			}
			static void onToDownClickHelper(GUIObject* object){
				((GUIListBox*)object)->onToDownClick();
			}
			void onToUpClick(); //����
			void onToDownClick(); //����
			void onLButtonDown(int mx, int my); //������ѡ����¼�
			void onKeyDown(SDLKey sym); //���̣��ϣ��¼�
			bool inside(int x, int y) {
				return x>m_x && x<m_x+m_w && y>m_y && y<m_y+m_h;
			}

		public:
			GUIListBox(int x, int y, int w, int num);
			~GUIListBox();

			int getSelectItem() {return m_selectItem;}
			string getSelectValue() {
				if(m_selectItem<0)return "";
				else return m_itemValue[m_selectItem];
			}
			void addItem(string value);
			void draw(SDL_Surface* destSurf);
	};

	class GUIProgressBar: public GUIObject
	{
		private:
			double m_radio; //��ǰ�������İٷֱ�
			int m_x, m_y;
			int m_w, m_h;
			Uint8 m_R, m_G, m_B;
		public:
			GUIProgressBar(int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B);
			~GUIProgressBar();

			void setRadio(double radio) {m_radio = radio;}
			void draw(SDL_Surface* destSurf);
	};

	class GUIRenderManager
	{
		private:
			static vector<GUIObject*> m_renderList; //��Ҫ��Ⱦ��GUI����

		public:
			static void guiRender(SDL_Surface* destSurf);
			static void addRender(GUIObject* object);
			static void delRender(GUIObject* object);
	};
}

#endif