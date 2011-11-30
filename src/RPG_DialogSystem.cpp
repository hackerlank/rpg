#include "../include/RPG_DialogSystem.h"
#include "../include/RPG_SpriteManager.h"
#include "../include/RPG_SceneManager.h"
#include "../include/RPG_SoundManager.h"
#include "../include/RPG_MenuSurface.h"
#include "../include/RPG_NPC.h"
#include <sstream>
#include "../include/game.h"

using RPG_NS_SceneManager::RPG_SceneManager;
// *.say�ļ�λ��data\sayĿ¼�£�ȡ������,����map1_dialog.say

int RPG_NS_DialogSystem::RPG_DialogSystem::status = DLG_NULL;
RPG_NS_DialogSystem::RPG_DialogSystem* RPG_NS_DialogSystem::RPG_DialogSystem::instance = NULL;
std::string RPG_NS_DialogSystem::RPG_DialogSystem::saypath = "data\\say\\";
std::string RPG_NS_DialogSystem::RPG_DialogSystem::picturePath = "image\\picture\\";
bool RPG_NS_DialogSystem::RPG_DialogSystem::saySentence = false;

RPG_NS_DialogSystem::RPG_DialogSystem::RPG_DialogSystem(int mapID, int npcUnicodeID, std::string* heroName){

	std::string dlgFile;
	dlgFile = "map" + intostring(mapID) + "_dialog.say";
	dlgFile = saypath + dlgFile;
	instance = this; //���ʵ��
	instance->m_sayFile = fopen(dlgFile.c_str(), "r");	//�򿪶Ի��ļ�
	if(instance->m_sayFile == NULL){
		throw "can't find file" + dlgFile;
		return;
	}
	status = DLG_DEAD; //
	//
	int uid;
	char npcName[100];
	char reserve[256];
	while(fscanf(instance->m_sayFile, "%d %s %s", &uid, npcName, reserve) != EOF){
		if(uid == npcUnicodeID){	//������
			instance->m_npcName = new GUI::GUILabel("font\\simkai.ttf", 20, npcName, 128, 20, 20, SAYNAME_X, SAYNAME_Y);	
			status = DLG_LIVE; //����
			gamePause = true;//��ͣ��Ҷ�Ӣ�۵Ŀ��ƺ�AI
			instance->m_back = new RPG_NS_Image::RPG_Image(picturePath+"dlgBK.bmp");
			break;
		}
		status = DLG_DEAD;
	}
	for(int inum=0; inum<3; inum++)
		instance->m_heroName[inum] = heroName[inum];
	//instance->m_say = NULL;
	instance->m_picture = NULL;
	instance->m_currentSay = NULL;
	instance->m_nameOfCurrentSay = NULL;
	instance->m_flag = false;
	//����¼�����
	RPG_NS_EventManager::RPG_EventManager::addListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
	RPG_NS_EventManager::RPG_EventManager::addListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
}

RPG_NS_DialogSystem::RPG_DialogSystem::~RPG_DialogSystem(void){
	RPG_NS_EventManager::RPG_EventManager::delListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
	RPG_NS_EventManager::RPG_EventManager::delListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
}

void RPG_NS_DialogSystem::RPG_DialogSystem::createDialog(int mapID, int npcUnicodeID){
	if(gamePause) return;
	std::string heroName[3];
	heroName[0] = heroSet.heros[0].heroName;
	heroName[1] = heroSet.heros[1].heroName;
	heroName[2] = heroSet.heros[2].heroName;
	new RPG_NS_DialogSystem::RPG_DialogSystem(mapID, npcUnicodeID, heroName); 
}

void RPG_NS_DialogSystem::RPG_DialogSystem::Update(SDL_Surface* destSurf){
	
	if(status==DLG_NULL)return;//����ִ��
	if(status==DLG_DEAD){ //ɾ���Ѿ�������shop
		delete instance;
		instance = NULL;
		status = DLG_NULL;
		return;
	}
	//����״̬
	RPG_NS_MenuSurface::RPG_MenuSurface::enableSurface(false);//��ֹ�˵�
	update(destSurf);
}

void RPG_NS_DialogSystem::RPG_DialogSystem::update(SDL_Surface* destSurf){

	static int uid;//-2~0:��Ӣ�ۣ�����Ϊnpc
	char pictureName[100];//ͷ���ļ�·��
	char content[256];//˵������
	//static bool saySentence = false;//�Ƿ���˵ĳ�仰�Ĺ�����
	static std::string t_currentSay;
	static int nIndex;
	static int count=0;
	static bool oneDlgover = true;
	static bool firstIn = true;
	
	if(saySentence){//����˵ĳһ�仰
		count++;//�������ۼ�
		if(count < 3 && count != 1){
			//��ʾ�����Ի���Ϣ
			instance->m_back->drawImage(destSurf, 0, BK_Y);//���Ի��򱳾�
			instance->m_nameOfCurrentSay->draw(destSurf);//��ʾ˵��������
			instance->m_picture->setColorKey(255, 0, 255);
			if(instance->m_picture != NULL)
				instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//��ʾͷ��
			instance->m_currentSay->draw(destSurf);//��ʾ�Ի�����
		}
		else{
			count = 1;
			//
			oneDlgover = false;
			if(instance->m_currentSay != NULL){
				delete instance->m_currentSay;
				instance->m_currentSay = NULL;
			}
			if(getHZbyIndex(instance->m_content, nIndex).size()){
				t_currentSay = t_currentSay + getHZbyIndex(instance->m_content, nIndex);
				nIndex++;
				instance->m_currentSay = new GUI::GUILabel("font\\simkai.ttf", 20, t_currentSay, 
						64, 64, 64, SAYCONTENT_X, SAYCONTENT_Y);
				//
				//��ʾ�����Ի���Ϣ
				instance->m_back->drawImage(destSurf, 0, BK_Y);//���Ի��򱳾�
				instance->m_nameOfCurrentSay->draw(destSurf);//��ʾ˵��������
				instance->m_picture->setColorKey(255, 0, 255);
				if(instance->m_picture != NULL)
					instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//��ʾͷ��
				instance->m_currentSay->draw(destSurf);//��ʾ�Ի�����
			}
			else{//����һ�仰
				count = 0;
				saySentence = false;
				if(instance->m_currentSay != NULL){
					delete instance->m_currentSay;
					instance->m_currentSay = NULL;
				}
				instance->m_currentSay = new GUI::GUILabel("font\\simkai.ttf", 20, t_currentSay, 
							64, 64, 64, SAYCONTENT_X, SAYCONTENT_Y);
				instance->m_back->drawImage(destSurf, 0, BK_Y);//���Ի��򱳾�
				instance->m_nameOfCurrentSay->draw(destSurf);//��ʾ˵��������
				instance->m_picture->setColorKey(255, 0, 255);
				if(instance->m_picture != NULL)
					instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//��ʾͷ��
				instance->m_currentSay->draw(destSurf);//��ʾ�Ի�����
			}
		}	
	}
	else{//
		//��ʾ�����Ի���Ϣ
		if(!oneDlgover){
			if(instance->m_currentSay != NULL){
				delete instance->m_currentSay;
				instance->m_currentSay = NULL;
			}
			instance->m_currentSay = new GUI::GUILabel("font\\simkai.ttf", 20, t_currentSay, 
						64, 64, 64, SAYCONTENT_X, SAYCONTENT_Y);
			instance->m_back->drawImage(destSurf, 0, BK_Y);//���Ի��򱳾�
			instance->m_nameOfCurrentSay->draw(destSurf);//��ʾ˵��������
			instance->m_picture->setColorKey(255, 0, 255);
			if(instance->m_picture != NULL)
				instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//��ʾͷ��
			instance->m_currentSay->draw(destSurf);//��ʾ�Ի�����
		}
		if(firstIn) goto label;
		//˵��һ�仰,�ȴ���������ո���
		if(!instance->m_flag) return;
label:
		RPG_NS_SoundManager::RPG_SoundManager::playSnd(RPG_NS_SoundManager::RPG_SoundManager::WORDSND);//����˵����Ч
		firstIn = false;
		instance->m_flag = false;
		if(instance->m_picture != NULL){
			delete instance->m_picture;
			instance->m_picture = NULL;
		}
		if(uid<=0 && instance->m_nameOfCurrentSay!=NULL){
			delete instance->m_nameOfCurrentSay;
			instance->m_nameOfCurrentSay = NULL;
		}
		fscanf(instance->m_sayFile, "%d %s %s", &uid, pictureName, content);//��һ�仰
		if(uid == -255){	//�Ի�����
			delete instance->m_back;
			delete instance->m_npcName;
			fclose(instance->m_sayFile);
			status = DLG_DEAD;
			oneDlgover = true;
			gamePause = false;
			firstIn = true;
			RPG_NS_SoundManager::RPG_SoundManager::clearEft();//�����Ч����������Դ
			RPG_NS_MenuSurface::RPG_MenuSurface::enableSurface(true);//�����˵�
		}
		else{
			oneDlgover = false;
			t_currentSay = "";
			nIndex = 0;
			string temp = "NULL";
			count = 0;
			if(temp.compare(pictureName))//
				instance->m_picture = new RPG_NS_Image::RPG_Image(picturePath+pictureName);//����ͷ��
			instance->m_content = content;
			if(uid>0){//��npc
				instance->m_nameOfCurrentSay = instance->m_npcName;
			}
			else{//��Ӣ��
				instance->m_nameOfCurrentSay = new GUI::GUILabel("font\\simkai.ttf", 20, instance->m_heroName[-1*uid], 
					128, 20, 20, SAYNAME_X, SAYNAME_Y);//��������������
			}
			saySentence = true;
		}	
	}
}

string RPG_NS_DialogSystem::RPG_DialogSystem::intostring(int n){

	stringstream str;
	str << n;
	return str.str();
}

string RPG_NS_DialogSystem::RPG_DialogSystem::getHZbyIndex(std::string str, int index){
	
	int iNum = 0;
	std::string t_str;
	for(int i=0; i<str.size(); i++){
		if(str[i]>=0){
			t_str = str.substr(i, 1);
			if(index == iNum++) return t_str;
		}
		else{
			t_str = str.substr(i, 2);
			if(index == iNum++) return t_str;
			i++;
		}
	}
	t_str = "";
	return t_str;
}

//void RPG_NS_DialogSystem::RPG_DialogSystem::onRButtonDown(int mx, int my){
	
//}

void RPG_NS_DialogSystem::RPG_DialogSystem::onLButtonDown(int mx, int my){
	if(!saySentence){
		instance->m_flag = true;
	}
}

void RPG_NS_DialogSystem::RPG_DialogSystem::onKeyDown(SDLKey sym){
	if(!saySentence && sym==SDLK_SPACE){
		instance->m_flag = true;
	}
}