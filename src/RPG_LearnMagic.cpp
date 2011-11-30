#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "../include/RPG_LearnMagic.h"
using std::string;
using std::stringstream;

using RPG_NS_LearnMagic::RPG_LearnMagic;
using RPG_NS_LearnMagic::MagicItem;

RPG_LearnMagic* RPG_LearnMagic::instance = NULL;
int RPG_LearnMagic::status = LEARN_NULL;

RPG_LearnMagic::RPG_LearnMagic(int mapId, int unicode){
	//����һ��ħ���̵�
	string fileName;
	stringstream stream;
	stream << mapId;
	stream << "U";
	stream << unicode;
	fileName = "data\\learnMagic\\mapID"+stream.str()+".learn";
	FILE* fp = fopen(fileName.c_str(), "r");
	if(fp==NULL)throw "�޷����ļ�"+fileName;

	MagicItem t;
	while(!feof(fp)){
		fscanf(fp, "%s", t.magicName); //����
		if(!strcmp(t.magicName, "END"))break;
		fscanf(fp, "%s", t.magicDesc); //����
		fscanf(fp, "%d", &t.cost); //�۸�
		for(int i=0; i<4; i++){
			fscanf(fp, "%d", &t.soul[i]); //�Ļ�
		}
		magicList.push_back(t);
	}
	fclose(fp);

	m_currentHero = 0;
	instance = this;
	status = LEARN_LIVE;
	gamePause = true;

	/////////////////����Ľ���////////////////////
	m_back = new RPG_NS_Image::RPG_Image("image\\gui\\learnMagic\\back.bmp");
	m_message = NULL;

	m_label_name = NULL; //Ӣ������
	m_label_totSoul = NULL; //������ǰӢ���ܵ��Ļ�
	m_label_pointSoul = NULL; //ʣ��ĵ���
	m_label_soul[0] = NULL; //ʣ���1
	m_label_soul[1] = NULL; //ʣ���2
	m_label_soul[2] = NULL; //ʣ���3
	m_label_soul[3] = NULL; //ʣ���4
	m_listbox_heroMagic = NULL;
	
	m_label_desc = NULL;
	m_label_cost = NULL;
	m_label_needSoul = NULL;

	/////
	m_button_left = new GUI::GUIButton("image\\gui\\learnMagic\\learnLeft1.bmp",
		"image\\gui\\learnMagic\\learnLeft2.bmp", HEROBASEX+10, HEROBASEY+10, this);
	m_button_left->setfClickEvent(onLeftHelper);
	m_button_right = new GUI::GUIButton("image\\gui\\learnMagic\\learnRight1.bmp",
		"image\\gui\\learnMagic\\learnRight2.bmp", HEROBASEX+155, HEROBASEY+10, this);
	m_button_right->setfClickEvent(onRightHelper);
	m_button_add1 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+125, this);
	m_button_add1->setfClickEvent(onAdd1Helper);
	m_button_add2 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+150, this);
	m_button_add2->setfClickEvent(onAdd2Helper);
	m_button_add3 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+175, this);
	m_button_add3->setfClickEvent(onAdd3Helper);
	m_button_add4 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+200, this);
	m_button_add4->setfClickEvent(onAdd4Helper);

	m_label_zsh = new GUI::GUILabel("font\\simkai.ttf", 17, "�Ļ꣺",
		0, 0, 0, HEROBASEX+10, HEROBASEY+55);

	/////
	stream.str("");
	stream << "$";
	stream << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 15, stream.str(),
		255, 255, 128, LEARNBASEX+45, LEARNBASEY+342);
	m_button_exit = new GUI::GUIButton("image\\gui\\learnMagic\\learnExit1.bmp",
		"image\\gui\\learnMagic\\learnExit2.bmp", LEARNBASEX+472, LEARNBASEY+12, this);
	m_button_exit->setfClickEvent(onExitHelper);
	m_button_buy = new GUI::GUIButton("image\\gui\\learnMagic\\learnBuy1.bmp",
		"image\\gui\\learnMagic\\learnBuy2.bmp", LEARNBASEX+180, LEARNBASEY+342, this);
	m_button_buy->setfClickEvent(onBuyHelper);

	m_listbox_learnMagic = new GUI::GUIListBox(LEARNBASEX+30, LEARNBASEY+35, 165, 7);
	for(int i=0; i<magicList.size(); i++){
		m_listbox_learnMagic->addItem(magicList[i].magicName);
	}

	////////////////�仯�Ľ���/////////////////////
	createHeroInterface(0); //Ĭ�ϵ�һ��Ӣ��
}

RPG_LearnMagic::~RPG_LearnMagic(){
	if(m_button_exit!=NULL)delete m_button_exit;
	if(m_button_buy!=NULL)delete m_button_buy;
	if(m_button_left!=NULL)delete m_button_left;
	if(m_button_right!=NULL)delete m_button_right;
	if(m_button_add1!=NULL)delete m_button_add1;
	if(m_button_add2!=NULL)delete m_button_add2;
	if(m_button_add3!=NULL)delete m_button_add3;
	if(m_button_add4!=NULL)delete m_button_add4;

	if(m_listbox_learnMagic!=NULL)delete m_listbox_learnMagic;
	if(m_listbox_heroMagic!=NULL)delete m_listbox_heroMagic;
	
	if(m_label_desc!=NULL)delete m_label_desc;
	if(m_label_cost!=NULL)delete m_label_cost;
	if(m_label_needSoul!=NULL)delete m_label_needSoul;
	if(m_label_money!=NULL)delete m_label_money;
	if(m_label_name!=NULL)delete m_label_name;
	if(m_label_zsh!=NULL)delete m_label_zsh;
	if(m_label_totSoul!=NULL)delete m_label_totSoul;
	if(m_label_pointSoul!=NULL)delete m_label_pointSoul;
	for(int i=0; i<4; i++)if(m_label_soul[i]!=NULL)
		delete m_label_soul[i];

	gamePause = false;
}



void RPG_LearnMagic::createHeroInterface(int index){

	//ɾ����ʱ��GUI
	m_currentHero = index;
	if(m_label_name!=NULL)delete m_label_name; //Ӣ������
	if(m_label_totSoul!=NULL)delete m_label_totSoul; //������ǰӢ���ܵ��Ļ�
	if(m_label_pointSoul!=NULL)delete m_label_pointSoul; //ʣ��ĵ���
	for(int i=0; i<4; i++)if(m_label_soul[i]!=NULL)
		delete m_label_soul[i];

	if(m_listbox_heroMagic!=NULL)delete m_listbox_heroMagic; //Ӣ�۵�����ħ��

	//�½�GUI
	stringstream stream;
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 19, string(heroSet.heros[index].heroName),
		0, 0, 0, HEROBASEX+60, HEROBASEY+20);
	for(int i=1; i<=4 ;i++){ //ÿ������ܵ���
		stream << "��";
		stream << i;
		stream << ": ";
		stream << heroSet.heros[index].totSoul[i-1];
		stream << " ";
	}
	m_label_totSoul = new GUI::GUILabel("font\\simkai.ttf", 12, stream.str(), 
		0, 0, 0, HEROBASEX+10, HEROBASEY+80);
	stream.str("");
	stream << "ʣ�������";
	stream << heroSet.heros[index].pointSoul;
	m_label_pointSoul = new GUI::GUILabel("font\\simkai.ttf", 17, stream.str(),
		0, 0, 0, HEROBASEX+10, HEROBASEY+100);
	stream.str("");
	for(int i=1; i<=4; i++){ //ÿ�����ʣ�����
		stream << "ʣ���";
		stream << i;
		stream << ": ";
		stream << heroSet.heros[index].soul[i-1];
		m_label_soul[i-1] = new GUI::GUILabel("font\\simkai.ttf", 15, stream.str(),
			0, 0, 0, HEROBASEX+10, HEROBASEY+125+(i-1)*25);
		stream.str("");
	}
	m_listbox_heroMagic = new GUI::GUIListBox(HEROBASEX+10, HEROBASEY+230, 165, 7);
	//add magic to listbox
	for(int i=0; i<heroSet.heros[index].magicList.size(); i++){
		m_listbox_heroMagic->addItem(heroSet.heros[index].magicList[i]);
	}
}

MagicItem RPG_LearnMagic::getMagicItem(std::string magicName){
	for(int i=0; i<magicList.size(); i++){
		if(!strcmp(magicName.c_str(), magicList[i].magicName))
			return magicList[i];
	}
}

/*********************��ť�¼�����****************************/
void RPG_LearnMagic::onBuy(){
	if(m_message!=NULL)return;
	if(m_listbox_learnMagic->getSelectItem()==-1)return;
	MagicItem t;
	t = getMagicItem(m_listbox_learnMagic->getSelectValue());
	if(heroPackage.finance < t.cost){ //��Ǯ����
		m_message = new GUI::GUIMessageBox("��ʾ:", "��Ǯ����", "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
		return;
	}
	stringstream stream;
	int lose[4];
	for(int i=0; i<4; i++){
		lose[i] = heroSet.heros[m_currentHero].soul[i] - t.soul[i];
	}
	if( lose[0]<0 || lose[1]<0 || lose[2]<0 ||lose[3]<0){ //ȱ������һ�ֻ�
		stream << "ȱ��:\n\n";
		if(lose[0]<0){
			stream << "    ";
			stream << -lose[0];
			stream << " ���1\n";
		}
		if(lose[1]<0){
			stream << "    ";
			stream << -lose[1];
			stream << " ���2\n";
		}
		if(lose[2]<0){
			stream << "    ";
			stream << -lose[2];
			stream << " ���3\n";
		}
		if(lose[3]<0){
			stream << "    ";
			stream << -lose[3];
			stream << " ���4\n";
		}
		m_message = new GUI::GUIMessageBox("��ʾ:", stream.str(), "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
		return;
	}
	int i;
	for(i=0; i<heroSet.heros[m_currentHero].magicList.size(); i++){
		if(!strcmp(t.magicName, heroSet.heros[m_currentHero].magicList[i].c_str())){//�ҵ���
			break;
		}
	}
	if(i<heroSet.heros[m_currentHero].magicList.size()){//�Ѿ�ѧ����ħ��
		m_message = new GUI::GUIMessageBox("��ʾ:", "�����ظ�ѧͬһ��ħ��!", "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
		return;
	}
	//����ѧϰ
	for(int i=0; i<4; i++){
		heroSet.heros[m_currentHero].soul[i] -= t.soul[i];
	}
	heroPackage.finance -= t.cost;
	m_message = new GUI::GUIMessageBox("��ʾ:","���Ѿ�ѧ���� "+string(t.magicName), "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
	heroSet.heros[m_currentHero].magicList.push_back(string(t.magicName));
	//��ˢGUI
	createHeroInterface(m_currentHero);
	delete m_label_money;
	stream.str("");
	stream << "$";
	stream << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 15, stream.str(),
		255, 255, 128, LEARNBASEX+45, LEARNBASEY+342);

}
void RPG_LearnMagic::onExit(){
	if(m_message!=NULL)return;
	status = LEARN_DEAD;
}
void RPG_LearnMagic::onLeft(){
	if(m_message!=NULL)return;
	if(m_currentHero==0)return;
	m_currentHero--;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onRight(){
	if(m_message!=NULL)return;
	if(m_currentHero>=heroSet.heroCount-1)return;
	m_currentHero++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd1(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[0]++;
	heroSet.heros[m_currentHero].totSoul[0]++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd2(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[1]++;
	heroSet.heros[m_currentHero].totSoul[1]++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd3(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[2]++;
	heroSet.heros[m_currentHero].totSoul[2]++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd4(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[3]++;
	heroSet.heros[m_currentHero].totSoul[3]++;
	createHeroInterface(m_currentHero);
}

///////////////////////////////////////////////////////////////
void RPG_LearnMagic::render(SDL_Surface* destSurf){
	if(status==LEARN_NULL)return;
	if(status==LEARN_DEAD){
		delete instance;
		status = LEARN_NULL;
		return;
	}
	//����״̬
	instance->m_back->drawImage(destSurf, LEARNBASEX, LEARNBASEY);
	/*************���ҽ���****************/
	instance->m_button_exit->draw(destSurf);
	instance->m_button_buy->draw(destSurf);
	instance->m_listbox_learnMagic->draw(destSurf);
	instance->m_label_money->draw(destSurf);

	if(instance->m_listbox_learnMagic->getSelectItem()!=-1){//ѡ��ĳ��ħ��
		if(instance->m_label_desc!=NULL){
			delete instance->m_label_desc;
			delete instance->m_label_cost;
			delete instance->m_label_needSoul;
		}
		MagicItem t;
		t = instance->getMagicItem(instance->m_listbox_learnMagic->getSelectValue());
		stringstream stream;
		string tempStr;

		stream << "������\n";
		tempStr = string(t.magicDesc);
		tempStr = newLine(tempStr, 12); //����
		stream << tempStr;
		instance->m_label_desc = new GUI::GUILabel("font\\simkai.ttf", 16, stream.str(),
			0, 0, 0, LEARNBASEX+20, LEARNBASEY+165);

		stream.str("");
		stream << "�۸�";
		stream << t.cost;
		instance->m_label_cost = new GUI::GUILabel("font\\simkai.ttf", 16, stream.str(),
			0, 0, 0, LEARNBASEX+20, LEARNBASEY+280);

		stream.str("");
		stream << "��Ҫ��";
		for(int i=0; i<4; i++){
			stream << t.soul[i];
			stream << "���";
			stream << i+1;
			stream << " ";
		}
		instance->m_label_needSoul = new GUI::GUILabel("font\\simkai.ttf", 12, stream.str(),
			0, 0, 0, LEARNBASEX+20, LEARNBASEY+300);
		
		instance->m_label_desc->draw(destSurf);
		instance->m_label_cost->draw(destSurf);
		instance->m_label_needSoul->draw(destSurf);
	}

	/****************Ӣ����ؽ���****************/
	instance->m_button_left->draw(destSurf);
	instance->m_button_right->draw(destSurf);
	instance->m_button_add1->draw(destSurf);
	instance->m_button_add2->draw(destSurf);
	instance->m_button_add3->draw(destSurf);
	instance->m_button_add4->draw(destSurf);

	instance->m_listbox_heroMagic->draw(destSurf);

	instance->m_label_zsh->draw(destSurf);
	instance->m_label_name->draw(destSurf);
	instance->m_label_totSoul->draw(destSurf);
	instance->m_label_pointSoul->draw(destSurf);
	for(int i=0; i<4; i++)instance->m_label_soul[i]->draw(destSurf);

	////////////////////////////////////////
	if(instance->m_message!=NULL){
		instance->m_message->draw(destSurf);
		if(instance->m_message->getSelection()!=GUI::GUIMessageBox::MB_NULL){//ѡ����
			delete instance->m_message;
			instance->m_message = NULL;
		}
	}

}