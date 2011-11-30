#include "../include/RPG_SoundManager.h"
#include "../include/RPG_SceneManager.h"
/* ��Ƶ��������ʼ���ļ�soundlist.sndl ��ʽԼ��
mapID(��ͼID��) musicFileName(�ַ��������ļ�)
��ЧID musicFileName(�ַ��������ļ�)
��������ID musicFileName(�ַ������������ļ�)
-1�������� END
*/

using std::string;

bool RPG_NS_SoundManager::RPG_SoundManager::bMusicON = false;
bool RPG_NS_SoundManager::RPG_SoundManager::bDisableMusic = false;
std::map<int, std::string> RPG_NS_SoundManager::RPG_SoundManager::musicList;
RPG_NS_MixSound::EffectSound* RPG_NS_SoundManager::RPG_SoundManager::effectSnd = NULL;
RPG_NS_MixSound::MusicSound* RPG_NS_SoundManager::RPG_SoundManager::musicSnd = NULL;
int RPG_NS_SoundManager::RPG_SoundManager::currentID = -1;
//bool RPG_NS_SoundManager::RPG_SoundManager::isFirst = true;

RPG_NS_SoundManager::RPG_SoundManager::RPG_SoundManager(void){

}

RPG_NS_SoundManager::RPG_SoundManager::~RPG_SoundManager(void){

}

void RPG_NS_SoundManager::RPG_SoundManager::AudioManager_Init(){//��Ƶ��������ʼ��

	//��ȡ soundlist.sndl �ļ�����ͼ(ID)��Ӧ�ı�������
	FILE *sndFile;
	string sndFileName = "data\\sound\\soundlist.sndl";
	sndFile = fopen(sndFileName.c_str(), "r");
	if(sndFile == NULL){
		throw "�Ҳ�����Ƶ��������ʼ���ļ���"+sndFileName;
		return;
	}
	string musicFileName;
	char str[100];
	int ID;
	while(true){
		
		fscanf(sndFile, "%d %s", &ID, str);
		if(ID == -1) break;
		musicFileName = str;
		musicList[ID] = musicFileName;
	}
	fclose(sndFile);
	currentID = RPG_NS_SceneManager::RPG_SceneManager::getCurrentMap();
	currentID = -1;
}

void RPG_NS_SoundManager::RPG_SoundManager::playSnd(const int musicID){
	//��ͼ������IDʱ�Ա�������ģʽ���ţ���������Чģʽ����
	//if(bMusicON && musicID<CLIMATE_BASE)
	//	stopMusic();
	//
	std::map<int, std::string>::iterator i = musicList.find(musicID);
    if(i==musicList.end()){
        return;
    }

	if(musicID >= CLIMATE_BASE){//��Ч
		if(effectSnd != NULL){
			delete effectSnd;
			effectSnd = NULL;
		}
		effectSnd = new EffectSound(musicList[musicID]);
		effectSnd->play();
	}
	else if(!bDisableMusic){	//��������
		if(currentID!=-1 && !musicList[currentID].compare(musicList[musicID]))
			return;
		stopMusic();
		musicSnd = new MusicSound(musicList[musicID]);
		musicSnd->play();
		currentID = musicID;
		bMusicON = true;
	}

}

void RPG_NS_SoundManager::RPG_SoundManager::playSnd(const std::string &musicFileName){
	//ͨ�������ļ������ű�������
	if(!bDisableMusic){
		stopMusic();
		musicSnd = new MusicSound(musicFileName);
		musicSnd->play();
		currentID = -1;
		bMusicON = true;
	}

}

void RPG_NS_SoundManager::RPG_SoundManager::stopMusic(){
	if(bMusicON){
		bMusicON = false;
		musicSnd->stop();
		delete musicSnd;
	}

}

void RPG_NS_SoundManager::RPG_SoundManager::clearEft(){
	if(effectSnd != NULL){
		delete effectSnd;
		effectSnd = NULL;
	}
}