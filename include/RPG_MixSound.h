// RPG_Sound( 2009 )
//
//��Ϸ��Ƶ������
//


#ifndef MIX_SOUND_CLASS_H
#define MIX_SOUND_CLASS_H

#include <iostream>
#include <string>
#include "SDL\SDL.h"
#include "SDL\SDL_mixer.h"

namespace RPG_NS_MixSound{
	class BaseMixSound//��Ƶ���Ż���
	{
	private:
		static int MixNUM;
	protected:
		BaseMixSound();
	public:
		virtual ~BaseMixSound();
	};

	class EffectSound: public BaseMixSound	//��Ч��
	{
	private:
		Mix_Chunk* sound;
	public:
		EffectSound(const std::string& sound_fileName);
		~EffectSound();
		void play() const;
	};

	class MusicSound: public BaseMixSound	//����������
	{
	private:
		Mix_Music* music;
	public:
		MusicSound(const std::string& music_fileName);
		~MusicSound();
		void play() const;
		void stop() const;
	};
}
#endif