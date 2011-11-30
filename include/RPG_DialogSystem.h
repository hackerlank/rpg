#ifndef HEADER_DLGSYSTEM_H
#define HEADER_DLGSYSTEM_H

#include <string>
#include "GUI.h"
#include "RPG_Image.h"
#include <SDL\SDL.h>
#include "RPG_EventManager.h"

/*	�Ի�ϵͳ
	ÿ�ŵ�ͼ����һ��*.say�ĶԻ��ļ���
	....
	-255 NULL NULL
	NpcUnicodeID(int) npcName(string) NULL
	flag(˵���߱��->0:Ӣ��1��-1:Ӣ��2��-2��Ӣ��3��1��NPC) bmpFileName(ͷ���ļ�·��) saystring(˵������)
	-255 NULL NULL
*/
using RPG_NS_Image::RPG_Image;

namespace RPG_NS_DialogSystem{
	class RPG_DialogSystem : public RPG_NS_EventManager::RPG_EventHandleInterface
	{
	public:
		static const int DLG_NULL = 0; //��ʾ��ǰû�п����Ի�ϵͳ
		static const int DLG_LIVE = 1; //��ʾ��ǰ�����Ի�ϵͳ
		static const int DLG_DEAD = 2; //��ʾ�رնԻ�ϵͳ,��Ҫɾ��ʵ��
		//
		static const int BK_Y = 450; //�Ի��򱳾�
		static const int PICTURE_X = 40; //ͷ����ʾ����
		static const int PICTURE_Y = 470;
		static const int SAYNAME_X = 200; //˵����������ʾ����
		static const int SAYNAME_Y = 470; 
		static const int SAYCONTENT_X = 200; //�Ի�������ʾ����
		static const int SAYCONTENT_Y = 500; 
	private:
		static int status; //��ǰ��״̬(DLG_NULL, DLG_LIVE, DLG_DEAD)
		static RPG_DialogSystem* instance; //ʵ��
		//
		int m_npcID;	//npcΨһ���
		GUI::GUILabel *m_npcName; //npc����
		std::string m_heroName[3]; //��Ӣ������
		RPG_Image *m_picture;//ͷ��
		RPG_Image *m_back;//�Ի���ʾ����ͼ��
		GUI::GUILabel *m_currentSay; //��ǰ˵������
		std::string m_content;//����˵������
		GUI::GUILabel *m_nameOfCurrentSay;//��ǰ����������
		//
		bool bInDialog;//�Ƿ��ڶԻ�������
		FILE *m_sayFile;//�Ի��ļ�
		static bool saySentence;//�Ƿ���˵ĳ�仰�Ĺ�����
		bool m_flag;
		static std::string saypath;
		static std::string picturePath;
		static void update(SDL_Surface* destSurf);//aid function:�Ի������еĸ���
		static std::string getHZbyIndex(std::string str, int index);//ͨ������ֵȡ�ַ�����

	public:
		void setHeroName(int heroID, std::string name){m_heroName[heroID] = name;}
		static void createDialog(int mapID, int npcUnicodeID);
		static void Update(SDL_Surface* destSurf);//ÿ����Ϸѭ���е���
		static string intostring(int n);
		//
		//virtual void onRButtonDown(int mx, int my);//
		virtual void onKeyDown(SDLKey sym);
		virtual void onLButtonDown(int mx, int my);

	public:
		RPG_DialogSystem(int mapID, int npcUnicodeID, std::string *heroName);
		~RPG_DialogSystem(void);
	};
}
#endif