#ifndef HEADER_GAME_MAPH
#define HEADER_GAME_MAPH
#include<vector>
#include<string>
using std::vector;
using std::string;
namespace RPG_NS_Map{

	typedef struct {
		int posX;
		int posY;
	}PosType;

	typedef struct {
		int width;
		int height; 
	}SizeType;

	typedef struct {
		int x;
		int y;
		int width;
		int height;
	}RectType;

	typedef struct {
		int NPC_Unicode;	//NPC��ͼȫ��Ψһ��ʶ
		int NPC_ID;	//NPC ID��
		PosType pos;
	}NPC;

	typedef struct {
		int npcUnicode;
		int npcID;
		int isRandom;
		int numOfRND;
		PosType *posRND;
		int isRndShow;
		int Posibility;
	}Read_NPC;	//NPC������Ϣ��ʽ

	typedef struct {	//��������Ϣ
		int posX;
		int posY;
		int next_mapID;
		int n_posX;
		int n_posY;
	}IO_Point;
	
	class RPG_Map	//��ͼ��	
	{
	protected:
		int m_mapID;					//��ͼ���
		SizeType m_tiledSize;			//�����С
		SizeType m_mapSize;				//��ͼ��С	
		int* m_mapDataFront;	//��ͼ���ֲ�(ǰ����)
		int* m_mapDataBack;	//��ͼ���ֲ�(�󱳾�)
		int* m_mapDataLogic;	//��ͼ�߼��㣨��ײ���㣩
		vector<NPC> m_npcList;			//NPC
		int m_numOfIO;	//��������
		vector<IO_Point> m_inoutPointList;				//�����ڵ����
		int m_tiledSplit;	//��ͼ�������
		string m_mapName; //��ͼ����

	public:
		RPG_Map(string mapFileName, bool isLoadNPC);
		~RPG_Map();
		void setMapID(int id) {m_mapID = id;}
		int getMapID() {return m_mapID;}
		int* getMapDataFront() {return m_mapDataFront;}	
		int* getMapDataBack() {return m_mapDataBack;}
		int* getMapDataLogic() {return m_mapDataLogic;}
		vector<IO_Point>* getIOPoint() {return &m_inoutPointList;}
		vector<NPC> getNpcList() {return m_npcList;}
		SizeType getMapSize() {return m_mapSize;}
		SizeType getTiledSize() {return m_tiledSize;}
		bool canMov(int posx, int posy);	//�жϵ�ͼ�Ƿ��ͨ��
		bool canMov(unsigned int index);	//
		unsigned int convertToIndex(int posX, int posY);	//����λ�õ���ͼ�������ŵ�ת��
		PosType convertToPos(unsigned int index);	//��ͼ�������ŵ�����λ�õ�ת��
		void npcInit(Read_NPC npc);	//��ͼ��ʼ��
		SizeType getMapSize_E(); //�õ���ͼ���سߴ�
		int getTiledSplit() {return m_tiledSplit;}
		string getMapname() {return m_mapName;}
	};
}
#endif