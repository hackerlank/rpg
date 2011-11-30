#ifndef HEADER_SOUNDMANAGER_H
#define HEADER_SOUNDMANAGER_H
#include <map>
#include <string>
#include "SDL\SDL.h"
#include "SDL\SDL_mixer.h"
#include "RPG_MixSound.h"
using RPG_NS_MixSound::EffectSound;
using RPG_NS_MixSound::MusicSound;

namespace RPG_NS_SoundManager{
	class RPG_SoundManager
	{
	private:
		static EffectSound* effectSnd;//��Ч������
		static MusicSound* musicSnd;//�������ֲ�����
		static bool bMusicON;	//���ֲ����Ƿ������
		static bool bDisableMusic; //�Ƿ��ֹ��������
		static int currentID;//��ǰ���ֲ���ID��
		//static bool isFirst;
	public:
		static bool isMusicON() {return bMusicON;}
		static void setMusicON(bool on) {bMusicON = on;}
		static bool isDisableMusic() {return bDisableMusic;}
		static void disableMusic(bool disable) {bDisableMusic = disable;}

		static std::map<int, std::string> musicList;	//��ͼ��Ӧ�������ļ��б�
	//
		static void AudioManager_Init();	//��Ƶ��������ʼ��	
		static void playSnd(const int musicID);//��������
		static void playSnd(const std::string &musicFileName);
		static void stopMusic();//ֹͣ��������
		static void clearEft();//�����Ч���Ų���
	public:
		/*
		==============����ID����
		*/
		//��������
		static const int CLIMATE_BASE = 0x100000;
		static const int SUN = CLIMATE_BASE+0x0000;//��
		static const int RAIN = CLIMATE_BASE+0x0001;//����
		static const int THUNDER = CLIMATE_BASE+0x0002;//����
		static const int DUSK = CLIMATE_BASE+0x0004;//�ƻ�
		static const int DARK = CLIMATE_BASE+0x0008;//����
		static const int WINDY = CLIMATE_BASE+0x00016;//�ҷ�
		//��ЧID
		static const int EFFECT_BASE = 0x1000000;
		static const int WORDSND = EFFECT_BASE + 0x0000;//�Ի�ϵͳ˵����Ч
		static const int BTN1 = EFFECT_BASE + 0x0001;//��ť1
		static const int BTN2 = EFFECT_BASE + 0x0002;//

	public:
		RPG_SoundManager(void);
	public:
		~RPG_SoundManager(void);
	};
}
#endif