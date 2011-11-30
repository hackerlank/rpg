/***************************************/
/*********** .map�ļ���ʽԼ�� **********/
/*
	mapID \n
	mapsize(width, height) \n
	tiledsize(width, height) \n
	map_front_layer(width*height ����)
	map_back_layer(width*height ����)
	map_logic_layer(width*height ����)
	NPC ����
	NPC ��Ϣ
		����NPC��Ϣ��ʽԼ����
		ID-UnicodeID-�̶�λ��/���λ��[<������> ���λ�ø��� ...]-�Ƿ��������[<��> ����]
					[<����̶�> �̶�λ��]
	��ͼ������ ����
	��ͼ��������Ϣ
		���е�ͼ��������ϢԼ����
		posX posY(����ͼ����������) nextMapID(���ŵ�ͼ��ID) n_posX n_posY(���ŵ�ͼ���������)
	��ͼ�������
*/
/**************************************/
#include "../include/RPG_map.h"
#include <cstdio>
#include <memory.h>
#include <time.h>

unsigned int RPG_NS_Map::RPG_Map::convertToIndex(int posX, int posY)	//����λ�õ���ͼ�������ŵ�ת��
{
	return posY*m_mapSize.width+posX;
}

RPG_NS_Map::PosType RPG_NS_Map::RPG_Map::convertToPos(unsigned int index) 	//��ͼ�������ŵ�����λ�õ�ת��
{
	int posX,posY;
	PosType t;
	posX = index % m_mapSize.width;
	posY = index / m_mapSize.width;
	t.posX = posX;
	t.posY = posY;
	return t;
}

bool RPG_NS_Map::RPG_Map::canMov(int posX, int posY)	//�жϵ�ͼ�Ƿ��ͨ��
{
	if(posX>=m_mapSize.width || posY>=m_mapSize.height ||
		posX<0 || posY<0)
		return false;
	return !(*(m_mapDataLogic+posY*m_mapSize.width+posX));
}

bool RPG_NS_Map::RPG_Map::canMov(unsigned int index)
{
	PosType t;
	t = convertToPos(index);
	return !(*(m_mapDataLogic+t.posY*m_mapSize.width+t.posX));
}

RPG_NS_Map::RPG_Map::RPG_Map(string mapFileName, bool isLoadNPC)//��ͼ�๹��
{
	FILE *mapFile;
	int numOfNPC;
	char mapName[100];
	Read_NPC t_readNpc;
	mapFile = fopen(mapFileName.c_str(), "r");
	if(mapFile == NULL)
		throw "�Ҳ�����ͼ�ļ���"+mapFileName;
	fscanf(mapFile, "%d", &m_mapID);	//����.map�ļ�Լ������
	fscanf(mapFile, "%s", mapName);	//�����ͼ����
	m_mapName = mapName;
	fscanf(mapFile, "%d %d", &m_mapSize.width, &m_mapSize.height);
	fscanf(mapFile, "%d %d", &m_tiledSize.width, &m_tiledSize.height);
	//�����ͼ�ڴ�ռ�
	m_mapDataFront = (int*)malloc(m_mapSize.width*m_mapSize.height*sizeof(int));
	m_mapDataBack = (int*)malloc(m_mapSize.width*m_mapSize.height*sizeof(int));
	m_mapDataLogic = (int*)malloc(m_mapSize.width*m_mapSize.height*sizeof(int));
	if(m_mapDataFront==NULL || m_mapDataBack==NULL || m_mapDataLogic==NULL)
		throw "�޷������ͼ�ڴ�";

	//�������
	for(int iLoop=0; iLoop<m_mapSize.height; iLoop++)
		for(int jLoop=0; jLoop<m_mapSize.width; jLoop++){
			fscanf(mapFile, "%d", m_mapDataFront+iLoop*m_mapSize.width+jLoop);
		}
	for(int iLoop=0; iLoop<m_mapSize.height; iLoop++)
		for(int jLoop=0; jLoop<m_mapSize.width; jLoop++){
			fscanf(mapFile, "%d", m_mapDataBack+iLoop*m_mapSize.width+jLoop);
		}
	for(int iLoop=0; iLoop<m_mapSize.height; iLoop++)
		for(int jLoop=0; jLoop<m_mapSize.width; jLoop++){
			fscanf(mapFile, "%d", m_mapDataLogic+iLoop*m_mapSize.width+jLoop);
		}

	//����NPC��Ϣ
	
	fscanf(mapFile, "%d", &numOfNPC);
	for(int npcIndex=0; npcIndex<numOfNPC; npcIndex++){
		fscanf(mapFile, "%d", &t_readNpc.npcID);
		fscanf(mapFile, "%d", &t_readNpc.npcUnicode);
		fscanf(mapFile, "%d", &t_readNpc.isRandom);
		if(t_readNpc.isRandom){	//���λ��
			fscanf(mapFile, "%d", &t_readNpc.numOfRND);
			t_readNpc.posRND = (PosType *)malloc(t_readNpc.numOfRND*sizeof(PosType));
			for(int iRND=0; iRND<t_readNpc.numOfRND; iRND++)
				fscanf(mapFile, "%d %d", &t_readNpc.posRND[iRND].posX,
										&t_readNpc.posRND[iRND].posY);
		}
		else{	//ȷ��λ��
			t_readNpc.posRND = (PosType *)malloc(sizeof(PosType));
			fscanf(mapFile, "%d %d", &t_readNpc.posRND[0].posX,
										&t_readNpc.posRND[0].posY);
		}
		fscanf(mapFile, "%d", &t_readNpc.isRndShow);
		if(t_readNpc.isRndShow){	//�������
			fscanf(mapFile, "%d", &t_readNpc.Posibility);
		}
		if(isLoadNPC)
			npcInit(t_readNpc);	//���NPC����
		
	}
	//
	fscanf(mapFile, "%d", &m_numOfIO);
	for(int iNum=0; iNum<m_numOfIO; iNum++){
		IO_Point t;
		fscanf(mapFile, "%d %d %d %d %d", &t.posX, &t.posY, &t.next_mapID, &t.n_posX, &t.n_posY);
		m_inoutPointList.push_back(t);
	}
	fscanf(mapFile, "%d", &m_tiledSplit);	//��ȡ��ͼ�������
	fclose(mapFile);
}

RPG_NS_Map::RPG_Map::~RPG_Map()//����
{
	//�ͷŵ�ͼ�ռ�
	free(m_mapDataFront);
	free(m_mapDataBack);
	free(m_mapDataLogic);
}

void RPG_NS_Map::RPG_Map::npcInit(Read_NPC t_npc)
{
	int rnd;
	NPC npc;
	rnd = rand()%100;	//���������
	if(t_npc.isRndShow){
		if(rnd>t_npc.Posibility)
			return;
	}
	npc.NPC_ID = t_npc.npcID;
	npc.NPC_Unicode = t_npc.npcUnicode;
	if(t_npc.isRandom){	//�������
		rnd = rand()%t_npc.numOfRND;
		npc.pos = t_npc.posRND[rnd];
	}
	else{
		npc.pos = t_npc.posRND[0];
	}
	m_npcList.push_back(npc);		//�����NPC����
}

RPG_NS_Map::SizeType RPG_NS_Map::RPG_Map::getMapSize_E(){

	SizeType t_size;
	t_size.width = m_mapSize.width * m_tiledSize.width;
	t_size.height = m_mapSize.height * m_tiledSize.height;
	return t_size;
}