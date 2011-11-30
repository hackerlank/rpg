#ifndef HEADER_RPG_IMAGE_H
#define HEADER_RPG_IMAGE_H

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
using std::string;
using std::vector;

namespace RPG_NS_Image
{

	class RPG_Image
	{
		private:
			int m_width;
			int m_height;
			int m_bpp;
			SDL_Surface* m_surfImage;
			vector<SDL_Rect> m_spliters;
			int m_cX, m_cY;
			Uint32 m_colork;
	    
		public:
			RPG_Image(string imageFileName);
			~RPG_Image();
	        
		public:
			int getWidth() {return m_width;}
			int getHeight() {return m_height;}
			int getBpp() {return m_bpp;}
			SDL_Surface* surfImage() {return m_surfImage;}
			bool drawImage(SDL_Surface* surfDest, int destX, int destY); //����ͼƬblit������ָ����λ�� 
			bool drawImage(SDL_Surface* surfDest, int destX, int destY,
							int srcX, int srcY, int srcWSize, int srcHSize); //����ͼƬblit������ָ����λ��
			void splitImage(int cX, int cY); //��Image�и��cY��cX�е����Сͼ��
			bool drawSpliter(SDL_Surface* surfDest, int destX, int destY, int index);
			void setColorKey(Uint8 r, Uint8 g, Uint8 b); //����͸��ɫ
			void undoColorKey(); //ȡ��͸��ɫ
	};      
}

#endif
