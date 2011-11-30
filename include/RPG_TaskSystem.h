/*****************
**********data/task/all.task
chapter1_main_1 ; ��ʾ��һ�����ߵ�һ������
����
~
��ϸ����
~
chapter1_branch_1 ; ��ʾ��һ��֧�ߵ�һ������
����
~
��ϸ����
~
END ;��ʾ����
*****************/

#ifndef HEADER_RPG_TASKSYSTEM
#define HEADER_RPG_TASKSYSTEM

#include <vector>
#include <SDL/SDL.h>
#include "GUI.h"
#include "RPG_Image.h"
#include "game.h"
using std::vector;

namespace RPG_NS_TaskSystem
{
	typedef struct{
		string id; //����id
		string desc; //�������
		string detail; //������ϸ����
	}TaskInfo; //�������Ϣ

	typedef struct{
		vector<string> id; //�����Ѿ����ܵ������б�
		vector<bool> comp; //��Ӧ������������
	}ChapterInfo; //ÿ�µĵ�ǰ������Ϣ

	class RPG_TaskSystem: public GUI::GUIObject
	{
		public:
			static const int TASK_NULL = 0;
			static const int TASK_LIVE = 1;
			static const int TASK_DEAD = 2;

		private://GUI
			GUI::GUILabel* m_label_main; //��ʾ���������񡰵ı�ǩ
			GUI::GUILabel* m_label_branch; //��ʾ��֧�����񡱵ı�ǩ
			GUI::GUILabel* m_label_chapterName; //�½�����
			GUI::GUIButton* m_button_Left; //�½ڵ�ѡ�ť,����
			GUI::GUIButton* m_button_Right; //�½ڵ�ѡ�ť������
			GUI::GUIButton* m_button_Exit; //�Ƴ���ť
			vector<GUI::GUILabel*> m_label_list; //��ǰ��ʾ������Ϣ�ı�ǩ
			vector<GUI::GUILabel*> m_label_comp; //��Ӧ������������
			vector<GUI::GUIButton*> m_button_detail; //��ϸ����鿴��ť
			RPG_NS_Image::RPG_Image* m_back; //����ͼƬ
			GUI::GUIMessageBox* m_message; //��ϸ������
			bool m_bMessageOn; //��Ϣ���Ƿ����

			void onLeft();
			void onRight();
			void onExit();
			void onDetail(int id); //��ϸ���
			static void onLeftHelper(GUIObject* object){
				((RPG_TaskSystem*)object)->onLeft();
			}
			static void onRightHelper(GUIObject* object){
				((RPG_TaskSystem*)object)->onRight();
			}
			static void onExitHelper(GUIObject* object){
				((RPG_TaskSystem*)object)->onExit();
			}
			static void onDetailHelper(GUIObject* object, int id){
				((RPG_TaskSystem*)object)->onDetail(id);
			}	

		private:
			static vector<TaskInfo> m_taskList; //���������������
			static vector<ChapterInfo> m_chapterList; //ÿ�µ��������
			static TaskInfo getTaskInfo(string id); //����id�õ�����ľ������
			static int m_chapterCount; //��ǰ�½�����
			static int m_currentChapter; //��ǰ�½�
			static int status; //��ǰ��״̬

			static RPG_TaskSystem* instance;

		public:
			RPG_TaskSystem(int chapter);
			~RPG_TaskSystem();
			static void RPG_TaskSystem_Init(); //��Ҫ������������
			void createInterface(int chapter); //�����½�chapter�Ĳ���
			static void render(SDL_Surface* destSurf);

			static void receive(string id); //���յ�һ������δ��ɡ�״̬
			static void complete(string id); //���һ�������Ѿ���ɡ�״̬
			static void addChapter(); //����һ��
	};
}

#endif HEADER_RPG_TASKSYSTEM