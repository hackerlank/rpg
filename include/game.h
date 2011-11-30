#ifndef HEADER_GAME
#define HEADER_GAME


/******
****data\item\all.item������Ʒ������
typede struct{
	char material[MAX_NAME]; ����
	int cost; �۸�
	char desc[256]; ����
	....
}Item;
*******/

#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

#define MAX_NAME 20


/**********************��Ʒ�Ĵ洢��Ϣ************************/
#define RADIO_BASE 10000 //����Ϊ10000�Ժ���ǰٷֱȵĵ�λ
typedef struct{
	int zdxl; //���Ѫ��
	int zdml; //���ħ��
	
	int wg; //�﹥
	int wf; //���
	int mg; //ħ��
	int mf; //ħ��
	int sudu; //�ٶ�

	int bjl; //������
	int zdl; //�¶���
	int zdsh; //�¶��˺�
	int zsml; //��˯����
	int yjbsl; //һ����ɱ��
	int fjl; //�⼼��
	int fml; //��ħ��

	int fdl; //������
	int fsml; //��˯����
	int fyjbsl; //��һ����ɱ��
	int ffjl; //���⼼��
	int ffml; //����ħ��
	int jdsh; //�����˺�
	int hhhx; //�غϻ�Ѫ
	int hhhm; //�غϻ�ħ
	int hhhcp; //�غϻ�CP
} EquipmentStat; //װ����ص�������

typedef struct{
	int canUse; //��װ��ϵͳ���ܷ�ʹ��
	int addBlood; //��Ѫ
	int addMana; //��ħ
	int addCp; //��CP
} DrugStat; //ҩ����Ϣ

typedef struct{
	char material[MAX_NAME];//��Ʒ����
	int cost;//��Ʒ�ļ۸�
	char desc[256];//����Ʒ������
	int type; //0��ʾ����,2��ʾ����(0,2����װ����),1��ʾҩƷ��,3��ʾ������Ʒ
	int eqpID; //ֻ��Ϊװ��ʱ����Ч,��ʾ������ʲô���
	EquipmentStat eqs; //װ������ϸ��Ϣ
	DrugStat dgs; //ҩƷ�����ϸ��Ϣ
} Item; //��Ʒ��ϸ��Ϣ(���մ洢�ṹ)

/***********************************************************/


/*********************Ӣ�۵�״̬�����************************/
	
typedef struct{
	char material[MAX_NAME]; //Heroӵ�е���Ʒ����
	int num; //����Ʒ������
} Mate; //�������ϵ���Ʒ

typedef struct{
	char heroName[MAX_NAME];//Ӣ�۵�����
	int level;//Ӣ�۵ĵȼ�
	int experience; //��ǰ����
	int nextExperience; //��һ���ľ���
	int blood;//��ǰ��Ѫ��
	int mana;//��ǰ��ħ��
	int cp;//��ǰ��CP
	int soul[4]; //ʣ���Ļ��ֵ
	int totSoul[4]; //�ܹ��Ļ��ֵ
	int pointSoul; //���ٵ���û�м�
	vector<string> magicList; //Ӣ�����ϵ�ħ���б�
	EquipmentStat attribute;//Ӣ�۵���ϸ��Ϣ
} HeroStat; //Ӣ�۵�״̬,����Ѫ��ħ��������...
typedef struct{
	HeroStat heros[3];//�������Ӣ��
	vector<Mate> items[3]; //Ӣ�����ϵ�װ��
	int heroCount;
} HeroSet;
extern HeroSet heroSet;//Ӣ�ۼ���

typedef struct{
	int finance; //��ҵ�Ǯ��
	vector<Mate> weapon; //����װ��
	vector<Mate> drug; //ҩƷ
	vector<Mate> jewelry; //����
	vector<Mate> story; //������Ʒ
}Package; //�����еĶ���
extern Package heroPackage; //��game.cpp�ж����ȫ�ְ���

/***********************************************************/

/***********************��Ϸ��״̬��Ϣ************************/

extern bool gamePause; //��Ϸ��ͣ���
extern bool gameOver; //��Ϸֹͣ���
extern int gameChapter; //��ǰ���½�,��1��ʼ���
extern map<string, int> sema; //�����ź�����
void story_tot(); //��Ϸ�����


/*******************ȫ�ֵĸ�������************************/
string newLine(string str, int n); //��str���ÿ�����n�����ֵ����ַ���
void addToPackage(int witch, string material); //���һ������Ϊmaterial����Ʒ,0��weapon��,1��drug...
void delFromPackage(int witch, string material); //ɾ��һ������Ϊmaterial����Ʒ
void addToHero(int nameid, string material); //���װ��material��nameidӢ������
void delFromHero(int nameid, string material); //ɾ��Ӣ��nameid���ϵ�material
bool hasGood(string material); //���԰������Ƿ��и���Ʒ

#endif


/*********************************************************/

