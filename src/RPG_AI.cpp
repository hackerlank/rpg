#include <stdlib.h>
#include "../include/RPG_AI.h"
#include "../include/RPG_Config.h"
#include "../include/RPG_SceneManager.h"
#include "../include/RPG_FindPath.h"
#include "../include/RPG_Map.h"

vector<RPG_NS_AI::NPCAction> RPG_NS_AI::RPG_AI::m_npcActionList;
vector<RPG_NS_AI::AIItem> RPG_NS_AI::RPG_AI::m_aiItem;

void RPG_NS_AI::RPG_AI::RPG_AI_Init(){
	RPG_NS_AI::NPCAction t;
	int npcId, nCount, moveId;
	FILE* fp = fopen("data\\ai\\npcmove\\all.npcmove", "r");
	if(fp==NULL)throw "�򲻿��ļ���data\\ai\\npcmove\\all.npcmove";
	fscanf(fp, "%d", &npcId);
	while(npcId!=-1){
		t.npcId = npcId;
		t.actionLength = 0;
		t.huntLength = -1;
		fscanf(fp, "%d", &t.length); //�˶�������Χ
		fscanf(fp, "%d", &nCount); //�˶���������
		while(nCount--){
			fscanf(fp, "%d", &moveId);
			if(moveId==1){ //׷��
				fscanf(fp, "%d", &t.huntLength); //׷�Ϸ�Χ
			}
			else{
				t.actionType[t.actionLength++] = moveId;
			}
		}
		m_npcActionList.push_back(t);
		fscanf(fp, "%d", &npcId);
	}

	fclose(fp);
}

void RPG_NS_AI::RPG_AI::updateList(){
	vector<RPG_NPC*>* npclist = RPG_SpriteManager::getSpriteList(); //�ӻ������
	RPG_NS_AI::AIItem t;
	for(int i=0; i<npclist->size(); i++){
		if((*npclist)[i]->isDead())continue; //����
		if((*npclist)[i]->getNPCID()==NPC_HERO)continue; //Ӣ�۲���ҪAI
		int j;
		for(j=0; j<m_aiItem.size(); j++){//Ѱ���Ƿ����
			if(m_aiItem[j].mapId==(*npclist)[i]->getMapID()
				&&m_aiItem[j].npcUnicode==(*npclist)[i]->getNPCUnicode()){
				m_aiItem[j].nowX = (*npclist)[i]->getPosX();
				m_aiItem[j].nowY = (*npclist)[i]->getPosY();//����λ��
				break;
			}	
		}
		if(j>=m_aiItem.size()){ //�����ڣ����
			t.mapId = (*npclist)[i]->getMapID();
			t.npcId= (*npclist)[i]->getNPCID();
			t.npcUnicode = (*npclist)[i]->getNPCUnicode();
			t.orgX = (*npclist)[i]->getPosX();
			t.orgY = (*npclist)[i]->getPosY();
			t.nowX = t.orgX;
			t.nowY = t.orgY;
			t.npc = (*npclist)[i];
			t.inHunt = false;
			t.goHome = false;
			m_aiItem.push_back(t);
		}
	}
	vector<AIItem>::iterator iter = m_aiItem.begin();
	for(; iter!=m_aiItem.end();){
		int i;
		for(i=0; i<(*npclist).size(); i++){//Ѱ���Ƿ���Ҫɾ��
			if((*npclist)[i]->getMapID()==(*iter).mapId
				&& (*npclist)[i]->getNPCUnicode()==(*iter).npcUnicode){
				break;
			}
		}
		if(i>=(*npclist).size()){//û�ҵ�����Ҫɾ��
			iter = m_aiItem.erase(iter);
		}else iter++;
	}
}

RPG_NS_AI::NPCAction RPG_NS_AI::RPG_AI::getActionInfo(int npcId){
	NPCAction t;
	t.npcId = -1; //�Ҳ����ı��
	for(int i=0; i<m_npcActionList.size(); i++){
		if(m_npcActionList[i].npcId==npcId)
			return m_npcActionList[i];
	}
	return t;
}

void RPG_NS_AI::RPG_AI::AIMove(){
	int currentMapId;
	currentMapId = RPG_NS_SceneManager::RPG_SceneManager::getCurrentMap(); //��ǰMapID
	RPG_NS_Map::RPG_Map* map;
	map = RPG_NS_SceneManager::RPG_SceneManager::getMap();

	/***************����***************/
	updateList(); 

	/*************������Ϊ*************/
	NPCAction t;
	for(int i=0; i<m_aiItem.size(); i++){
		if(m_aiItem[i].goHome)continue; 
		t = getActionInfo(m_aiItem[i].npcId);
		if(t.npcId == -1)continue; //����ҪAI��npc
		if( (abs(m_aiItem[i].nowX-m_aiItem[i].orgX)
			+abs(m_aiItem[i].nowY-m_aiItem[i].orgY)>t.length) 
			&& m_aiItem[i].inHunt==false){//����׷��������������Χ
				//�ص����
				m_aiItem[i].actionList.clear();
				RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
					m_aiItem[i].orgX, m_aiItem[i].orgY, &m_aiItem[i].actionList);//A*Ѱ·
				m_aiItem[i].goHome = true;
				continue;
		}
		if( m_aiItem[i].inHunt 
			&& (abs(m_aiItem[i].nowX-m_aiItem[i].orgX)
			+abs(m_aiItem[i].nowY-m_aiItem[i].orgY)>t.huntLength) ){
				//׷��ʱ����׷����Χ���ص����
				m_aiItem[i].actionList.clear();
				RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
					m_aiItem[i].orgX, m_aiItem[i].orgY, &m_aiItem[i].actionList);//A*Ѱ·
				m_aiItem[i].goHome = true;
				continue;
		}
		if(m_aiItem[i].inHunt)continue; //�����ǰ����׷��״̬���򲻲�ȡ�����Ϊ
		if(t.huntLength!=-1){//�����ͻ�׷��
			//�鿴����ײ����,�����Ƿ�׷��
			int j;
			for(j=0; j<RPG_SpriteManager::g_collisionList_non.size(); j++){
				if(RPG_SpriteManager::g_collisionList_non[j]->getMapID()==m_aiItem[i].mapId
					&& RPG_SpriteManager::g_collisionList_non[j]->getNPCUnicode()==m_aiItem[i].npcUnicode){
					break;
				}
			}
			if(j<RPG_SpriteManager::g_collisionList_non.size()){
				//��������ײ����Ҫ׷��
				m_aiItem[i].actionList.clear(); //��ʼ׷��ʱȡ������������Ϊ
				RPG_NPC* hero = RPG_SpriteManager::getNPC(currentMapId, 0); //Ӣ��
				RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
					hero->getPosX(), hero->getPosY(), &m_aiItem[i].actionList);//A*Ѱ·
				m_aiItem[i].inHunt = true; //��׷������
				m_aiItem[i].npc->setMoveSpeed(1);
				continue;
			}
		}
		//׷�����������Ϊ
		if(m_aiItem[i].actionList.size()>0)continue; //������Ϊ��ִ��
		if(t.actionLength==0)continue; //��������������
		int k = rand()%t.actionLength; //�������һ������
		int l, xl, yl , u;
		switch(t.actionType[k])
		{
		case 0: //����߶�
			l = rand()%t.length+1;
			xl = rand()%l+1;
			yl = l - xl;
			if(rand()%2==0)xl = -xl;
			if(rand()%2==0)yl = -yl;
			RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
				m_aiItem[i].orgX+xl, m_aiItem[i].orgY+yl, &m_aiItem[i].actionList);//A*Ѱ·
			break;
		case 2: //�����߶�
			u = rand()%2; //�������
			if(u==0)for(int j=0; j<5; j++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_LEFT);
			else for(int j=0; j<5; j++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_RIGHT);
			break;
		case 3: //�����߶�
			int u = rand()%2; //�������
			if(u==0)for(int i=0; i<5; i++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_UP);
			else for(int i=0; i<5; i++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_DOWN);
			break;
		}
	}

	/*****************ִ�ж���**************/
	for(int i=0; i<m_aiItem.size(); i++){
		if(m_aiItem[i].mapId!=currentMapId)continue; //ֻ�е�ǰ��ͼ��NPC�Ŵ���
		if(m_aiItem[i].npc->isBusy())continue;
		if(m_aiItem[i].actionList.size()==0)continue;
		vector<int>::iterator iter = m_aiItem[i].actionList.begin();
		m_aiItem[i].npc->move((*iter));
		iter = m_aiItem[i].actionList.erase(iter); //ɾ���ö���
		if(m_aiItem[i].actionList.size()==0){
			m_aiItem[i].inHunt = false;
			m_aiItem[i].goHome = false;
			m_aiItem[i].npc->setMoveSpeed(2);
		}
	}
}

