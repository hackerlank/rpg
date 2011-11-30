#include <algorithm>
#include <cmath>
#include "../include/RPG_FindPath.h"
using RPG_NS_FindPath::PQNode;
map<int, bool> RPG_NS_FindPath::FindPath::used;
/*priority_queue< PQNode, vector<PQNode>, less<vector<PQNode>::value_type> >
	RPG_NS_FindPath::FindPath::pq;
	*/
priority_queue<PQNode> RPG_NS_FindPath::FindPath::pq;
RPG_Map* RPG_NS_FindPath::FindPath::m_pMap;
int RPG_NS_FindPath::FindPath::m_eX;
int RPG_NS_FindPath::FindPath::m_eY;
vector<RPG_NS_FindPath::PQNode> RPG_NS_FindPath::FindPath::pathList;

bool RPG_NS_FindPath::FindPath::findPath(RPG_NS_Map::RPG_Map *pMap, 
			int sx, int sy, int ex, int ey, std::vector<int> *command){

	command->clear(); //�����ǰ������
	m_eX = ex;
	m_eY = ey;
	setMap(pMap);
	used.clear();
	while(!pq.empty())pq.pop(); //������ȶ���
	pathList.clear();


	PQNode s;
	s.x = sx;
	s.y = sy;
	s.w = 0;
	s.f = calculateF(s.x, s.y); 
	s.father = -1;
	pq.push(s); //������
	bool bFind = false;
	while( !pq.empty() ){
		PQNode top = pq.top(); //ȡ�����ȶ������Ͻ���С�Ľ��
		int father = pathList.size();
		pathList.push_back(pq.top()); //�����߹��Ķ�����
		pq.pop(); //����������
		if(reachGoal(top.x, top.y)){ //�ҵ�Ŀ�ĵ�
			bFind = true;
			//�γ�command����
			while( pathList[father].father!=-1 ){
				command->push_back(pathList[father].moveStatus);
				father = pathList[father].father;
			}
			reverse(command->begin(), command->end()); //��ת·��
			break;
		}
		expandNode(father, top);
	}
	return bFind;
}

int RPG_NS_FindPath::FindPath::calculateF(int x, int y){
	return abs(m_eX-x) + abs(m_eY-y);
}

void RPG_NS_FindPath::FindPath::expandNode(int father, RPG_NS_FindPath::PQNode& node){
	static int dx[] = {0, 0, -1, 1};
	static int dy[] = {-1, 1, 0, 0};
	static int cmd[] = {RPG_NPC::NPC_UP, RPG_NPC::NPC_DOWN, RPG_NPC::NPC_LEFT, RPG_NPC::NPC_RIGHT};
	for(int i=0; i<4; i++){
		int x = node.x + dx[i];
		int y = node.y + dy[i];
		if(canMove(x, y)){ //��������
			PQNode t;
			t.x = x;
			t.y = y;
			t.father = father;
			t.moveStatus = cmd[i];
			t.w = node.w + 1;
			t.f = calculateF(t.x, t.y);
			setUsed(t.x, t.y); //����Ϊ�Ѿ��߹�
			pq.push(t);
		}
	}
}