#ifndef RPG_FIND_PATH_H
#define RPG_FIND_PATH_H

#include <queue>
#include <map>
#include <vector>
#include "RPG_Map.h"
#include "RPG_NPC.h"
using RPG_NS_Map::RPG_Map;
using std::priority_queue;
using std::map;
using std::vector;
using namespace RPG_NS_NPC;

namespace RPG_NS_FindPath
{
	class PQNode{
	public:	
		int x, y; //��ǰλ��
		int w, f; //����ֵ
		int father; //�������
		int moveStatus; //ǰ��һ���������ô������

		bool operator< (const PQNode& a) const
		{
			return w+f > a.w+a.f;
		}
	};

	/*bool operator< (const PQNode& pq1, const PQNode& pq2){
		return pq1.w+pq1.f > pq2.w+pq2.f;
	}*/

	class FindPath
	{
		private:
			static int m_eX;
			static int m_eY; //Ŀ���
			static RPG_Map* m_pMap;
			static map<int , bool> used;
			//static priority_queue<PQNode, vector<PQNode>, less<vector<PQNode>::value_type>> pq;
			static priority_queue<PQNode> pq;
			static vector<PQNode> pathList; //�Ѿ���չ���Ľ�����


		private:
			static int calculateF(int x, int y); //������������
			static bool reachGoal(int x, int y) { return x==m_eX&&y==m_eY;}
			static void setMap(RPG_Map* pMap) {m_pMap = pMap;}
			static bool canMove(int x, int y) //�ж�(x,y)�Ƿ�����
				{return m_pMap->canMov(x, y) && !used[m_pMap->getMapSize().width*y+x];}
			static void setUsed(int x, int y) {used[m_pMap->getMapSize().width*y+x]=true;}
			static void expandNode(int father, PQNode& node); //��չnode���,fatherָ�������pathList��λ��

		public:
			static bool findPath(RPG_Map* pMap, int sx, int sy, int ex, int ey, std::vector<int>* command);
			//�ڵ�ͼ���ҵ�һ��·��,·�������������command��, A* �㷨

			
	};
}


#endif