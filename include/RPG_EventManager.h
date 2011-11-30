#ifndef HEADER_RPG_EVENTMANAGER_H
#define HEADER_RPG_EVENTMANAGER_H

#include <SDL\SDL.h>
#include <vector>
using std::vector;

namespace RPG_NS_EventManager
{
	class RPG_EventHandleInterface{
		public:
			RPG_EventHandleInterface() {}
			~RPG_EventHandleInterface() {}
			virtual void handleKeyState(Uint8 *keystate){}
			virtual void onKeyDown(SDLKey sym) {}
			virtual void onLButtonDown(int mx, int my) {}
			virtual void onRButtonDown(int mx, int my) {}
			virtual void onLButtonUp(int mx, int my) {}
			virtual void onRButtonUp(int mx, int my) {}
			virtual void onMouseMove(int mx, int my) {}
			virtual void onExit() {}
	};

	class RPG_EventManager
	{
		public:
			static const int EVENTMANAGER_HANDLEKEYSTATE = 0;
			static const int EVENTMANAGER_ONKEYDOWN = 1;
			static const int EVENTMANAGER_ONLBUTTONDOWN = 2;
			static const int EVENTMANAGER_ONRBUTTONDOWN = 3;
			static const int EVENTMANAGER_ONLBUTTONUP = 4;
			static const int EVENTMANAGER_ONRBUTTONUP = 5;
			static const int EVENTMANAGER_ONMOUSEMOVE = 6;
			static const int EVENTMANAGER_ONEXIT = 7;
		private:
			//�¼�����
			static vector<RPG_EventHandleInterface*> m_handleKeyState_list;
			static vector<RPG_EventHandleInterface*> m_onKeyDown_list;
			static vector<RPG_EventHandleInterface*> m_onLButtonDown_list;
			static vector<RPG_EventHandleInterface*> m_onRButtonDown_list;
			static vector<RPG_EventHandleInterface*> m_onLButtonUp_list;
			static vector<RPG_EventHandleInterface*> m_onRButtonUp_list;
			static vector<RPG_EventHandleInterface*> m_onMouseMove_list;
			static vector<RPG_EventHandleInterface*> m_onExit_list;
			static vector<RPG_EventHandleInterface*>& getByIndex(int sel);

		public:
			static void init(); //��ʼ��
			static void listen(); //�¼�������
			static void addListen(RPG_EventHandleInterface* handler, int sel); //��ӽ��¼�����
			static void delListen(RPG_EventHandleInterface* handler, int sel); //���¼�������ɾ��
			static void clearListen(int sel); //���ĳ���¼�����
	};
}


#endif