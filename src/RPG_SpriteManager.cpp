#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include "../include/RPG_SpriteManager.h"
#include "../include/game.h"
#include "../include/RPG_Config.h"
using std::string;
using std::vector;
using std::stringstream;

vector<RPG_NPC*> RPG_NS_SpriteManager::RPG_SpriteManager::m_spriteList;
vector<RPG_NPC*> RPG_NS_SpriteManager::RPG_SpriteManager::g_collisionList;
vector<RPG_NPC*> RPG_NS_SpriteManager::RPG_SpriteManager::g_collisionList_non;
vector<RPG_NPC*> RPG_NS_SpriteManager::RPG_SpriteManager::g_collisionList_saying;
RPG_Map* RPG_NS_SpriteManager::RPG_SpriteManager::m_pMap;
bool RPG_NS_SpriteManager::RPG_SpriteManager::m_doCollision;
RPG_NS_SpriteManager::RPG_SpriteManager* RPG_NS_SpriteManager::RPG_SpriteManager::instance;

int RPG_NS_SpriteManager::RPG_SpriteManager::addNPC(int NPCID, int NPCUnicode, RPG_Map* pMap, int mapID, int x, int y){
	stringstream stream;
	stream << NPCID;
	string imageFileName = "image\\npc\\npc" + stream.str() + ".bmp";
	string dataFileName = "data\\npc\\npc" + stream.str() + ".npc";

	FILE* fp = fopen(dataFileName.c_str(), "rb");
	if(fp==NULL)throw "���ܴ��ļ�:" + dataFileName;
	RPG_NS_SpriteManager::NPC_data npcData;
	int nCount = fread(&npcData, sizeof(npcData), 1, fp);
	if(nCount!=1)throw "�ļ� "+dataFileName+" �Ѿ���";

	RPG_NPC* npc;
	if(pMap!=NULL){
		npc = new RPG_NPC(imageFileName, pMap); //����һ��npc
		mapID = pMap->getMapID();
	}
	else
		npc = new RPG_NPC(imageFileName, mapID); 
	npc->setMoveSpeed(npcData.moveSpeed); //�����ٶ�
	npc->setBelongsToMap(mapID); //����npc�����ĵ�ͼ
	npc->setNPCID(NPCID); //����npc������
	if(NPCUnicode==-1){ //�����-1���Զ�����
		NPCUnicode = getNPCUnicode();	
	}
	npc->setNPCUnicode(NPCUnicode); //����npc��Ψһ���
	npc->setPos(x, y); //���ô�שλ��
	//npc->setPosE(x*pMap->getTiledSize().width, y*pMap->getTiledSize().height); //��������λ��
	npc->setPosE(x*32, y*32); //��������λ��
	npc->splitImage(npcData.splitX, npcData.splitY); //�и�λͼ
	//������϶���
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upLeft);
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upStop);
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upRight);
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upStop);
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upLeft);
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upStop);
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upRight);
	npc->addDirMotion(RPG_NPC::NPC_UP, npcData.upStop);
	//������¶���
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downLeft);
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downStop);
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downRight);
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downStop);
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downLeft);
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downStop);
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downRight);
	npc->addDirMotion(RPG_NPC::NPC_DOWN, npcData.downStop);
	//���������
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftLeft);
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftStop);
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftRight);
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftStop);
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftLeft);
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftStop);
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftRight);
	npc->addDirMotion(RPG_NPC::NPC_LEFT, npcData.leftStop);
	//������Ҷ���
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightLeft);
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightStop);
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightRight);
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightStop);
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightLeft);
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightStop);
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightRight);
	npc->addDirMotion(RPG_NPC::NPC_RIGHT, npcData.rightStop);

	//npc->setMoveStatus(RPG_NPC::NPC_UP); //Ĭ��npc������
	npc->setMoveStatus(npcData.dir);
	//npc->getImage()->setColorKey(255, 0, 255); //����͸��ɫ
	m_spriteList.push_back(npc); //��ӵ��ӻ������
	m_doCollision = true;

	fclose(fp);
	return NPCUnicode;
}

int RPG_NS_SpriteManager::RPG_SpriteManager::addNPC(int NPCID, int NPCUnicode, int mapID, int x, int y){
	return addNPC(NPCID, NPCUnicode, NULL, mapID, x, y);
}

int RPG_NS_SpriteManager::RPG_SpriteManager::addNPC(int NPCID, int NPCUnicode, RPG_Map* pMap, int x, int y){
	return addNPC(NPCID, NPCUnicode, pMap, 0, x, y);
}

void RPG_NS_SpriteManager::RPG_SpriteManager::killNPC(int mapID){
	vector<RPG_NPC*>::iterator iter = m_spriteList.begin();
	for(; iter!=m_spriteList.end(); ){
		if((*iter)->getNPCUnicode()>=UNICODE_BASE){
			iter++;
			continue; //����ɾ���Զ����ӻ���
		}
		if((*iter)->getMapID()==mapID){
			delete (*iter);
			iter = m_spriteList.erase(iter);
		}
		else 
			iter++;
	}
}


void RPG_NS_SpriteManager::RPG_SpriteManager::killNPCByIDs(int mapID, int NPCUnicode){
	vector<RPG_NPC*>::iterator iter = m_spriteList.begin();
	for(; iter!=m_spriteList.end(); ){
		if((*iter)->getBelongsToMap()==mapID && (*iter)->getNPCUnicode()==NPCUnicode){
			delete (*iter);
			iter = m_spriteList.erase(iter);
		}
		else 
			iter++;
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::killAllNPC(){
	vector<RPG_NPC*>::iterator iter = m_spriteList.begin();
	for(; iter!=m_spriteList.end(); ){
		if((*iter)->getNPCUnicode()>=UNICODE_BASE){
			iter++;
			continue; //����ɾ���Զ����ӻ���
		}
		delete (*iter);
		iter = m_spriteList.erase(iter);
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::setMap(int mapID, RPG_NS_Map::RPG_Map* pMap){
	for(int i=0; i<m_spriteList.size(); i++){
		if(m_spriteList[i]->getBelongsToMap()==mapID){
			m_spriteList[i]->setMap(pMap); //����NPC��ͼ�����Ϣ
		}
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::setCurrentMap(RPG_NS_Map::RPG_Map* pMap){
	m_pMap = pMap;
}

int RPG_NS_SpriteManager::RPG_SpriteManager::getNPCUnicode(){
	int id = -1;
	for(int i=0; i<m_spriteList.size(); i++){
		if(id<m_spriteList[i]->getNPCUnicode()){
			id = m_spriteList[i]->getNPCUnicode();
		}
	}
	return UNICODE_BASE + id +1; //����һ������Unicode
}

RPG_NPC* RPG_NS_SpriteManager::RPG_SpriteManager::getNPC(int mapID, int NPCUnicode){
	for(int i=0; i<m_spriteList.size(); i++){
		if(m_spriteList[i]->getBelongsToMap()==mapID
			&& m_spriteList[i]->getNPCUnicode()==NPCUnicode)
			return m_spriteList[i];
	}
	return NULL;
}

void RPG_NS_SpriteManager::RPG_SpriteManager::loadNPC(RPG_NS_Map::RPG_Map* pMap){
	vector<RPG_NS_Map::NPC> npcList = pMap->getNpcList();
	for(int i=0; i<npcList.size(); i++){
		addNPC(npcList[i].NPC_ID, npcList[i].NPC_Unicode, pMap, npcList[i].pos.posX, npcList[i].pos.posY);
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::clearList(){
	for(int i=0; i<m_spriteList.size(); i++){
		delete m_spriteList[i];
	}
	m_spriteList.clear();
}

void RPG_NS_SpriteManager::RPG_SpriteManager::detectCollision(){
	g_collisionList.clear();
	g_collisionList_non.clear();
	if(!m_doCollision)return;
	RPG_NPC *npci, *npcj;
	for(int i=0; i<m_spriteList.size(); i++){
		//ֻ��⵱ǰ��ͼ�е��ӻ���
		if(m_spriteList[i]->getMapID()!=m_pMap->getMapID())continue;
		npci = m_spriteList[i];
		for(int j=i+1; j<m_spriteList.size(); j++){
			if(m_spriteList[j]->getMapID()!=m_pMap->getMapID())continue;
			npcj = m_spriteList[j];
			//���i��j����npc�Ƿ���ײ
			if( (npci->getPosX())==(npcj->getPosX()) 
				&& (npci->getPosY())==(npcj->getPosY()) ){
				//����ǿ��ײ
				if((npci->getNPCID())==NPC_HERO)
					g_collisionList.push_back(npcj);
				if((npcj->getNPCID())==NPC_HERO)
					g_collisionList.push_back(npci); //��Hero��ײ��NPC������
				if(!npci->isHide() && !npcj->isHide()){ //ֻ�з������ӻ��������ײ���
					adjustment(npci); //����λ��
					adjustment(npcj);
				}
			}
			if( abs(npci->getPosX()-npcj->getPosX()) + abs(npci->getPosY()-npcj->getPosY()) <= 4){
				//��������ײ
				if((npci->getNPCID())==NPC_HERO)
					g_collisionList_non.push_back(npcj);
				if((npcj->getNPCID())==NPC_HERO)
					g_collisionList_non.push_back(npci);
			}
		}
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::adjustment(RPG_NS_NPC::RPG_NPC *npc){
	npc->undoCommand();
}

void RPG_NS_SpriteManager::RPG_SpriteManager::updateSprites(){
	for(int i=0; i<m_spriteList.size(); i++){
		m_spriteList[i]->update(); //����ÿ���ӻ���
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::killDeadSprite(){
	vector<RPG_NPC*>::iterator iter = m_spriteList.begin();
	for(; iter!=m_spriteList.end(); ){
		if((*iter)->isDead()){ //���������ӻ��������ɾ��
			delete (*iter);
			iter = m_spriteList.erase(iter);
		}
		else
			iter++;
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::delCollisionList(int i){
	if(i<0 || i>=g_collisionList.size())return;
	vector<RPG_NPC*>::iterator iter = g_collisionList.begin();
	while(i>0){
		i--;
		iter++;
	}
	iter = g_collisionList.erase(iter);
}

void RPG_NS_SpriteManager::RPG_SpriteManager::delCollisionList_non(int i){
	if(i<0 || i>=g_collisionList_non.size())return;
	vector<RPG_NPC*>::iterator iter = g_collisionList_non.begin();
	while(i>0){
		i--;
		iter++;
	}
	iter = g_collisionList_non.erase(iter);
}

void RPG_NS_SpriteManager::RPG_SpriteManager::delCollisionList_saying(int i){
	if(i<0 || i>=g_collisionList_saying.size())return;
	vector<RPG_NPC*>::iterator iter = g_collisionList_saying.begin();
	while(i>0){
		i--;
		iter++;
	}
	iter = g_collisionList_saying.erase(iter);
}

void RPG_NS_SpriteManager::RPG_SpriteManager::onKeyDown(SDLKey sym){
	if(sym==SDLK_SPACE)
		onRButtonOrSpace();
}

void RPG_NS_SpriteManager::RPG_SpriteManager::onRButtonDown(int mx, int my){
	onRButtonOrSpace();
}

void RPG_NS_SpriteManager::RPG_SpriteManager::onRButtonOrSpace(){ //˵����ײ���
	if(gamePause)return;
	g_collisionList_saying.clear();
	if(!m_doCollision)return;
	RPG_NPC *npci, *npcj;
	for(int i=0; i<m_spriteList.size(); i++){
		//ֻ��⵱ǰ��ͼ�е��ӻ���
		if(m_spriteList[i]->getMapID()!=m_pMap->getMapID())continue;
		npci = m_spriteList[i];
		for(int j=i+1; j<m_spriteList.size(); j++){
			if(m_spriteList[j]->getMapID()!=m_pMap->getMapID())continue;
			npcj = m_spriteList[j];
			
			if( abs(npci->getPosX()-npcj->getPosX()) + abs(npci->getPosY()-npcj->getPosY()) <= 1
				&& (npci->getMoveStatus()+npcj->getMoveStatus()==0) ){ //�������
				//����˵����ײ
				if((npci->getNPCID())==NPC_HERO)
					g_collisionList_saying.push_back(npcj);
				if((npcj->getNPCID())==NPC_HERO)
					g_collisionList_saying.push_back(npci);
			}
		}
	}
}

void RPG_NS_SpriteManager::RPG_SpriteManager::init(){
	instance = new RPG_SpriteManager();
	RPG_EventManager::addListen(instance, RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
	RPG_EventManager::addListen(instance, RPG_EventManager::EVENTMANAGER_ONRBUTTONDOWN);
}