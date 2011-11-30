#include <string.h>
#include <stdio.h>
#include <sstream>
#include "../include/RPG_ShopingSystem.h"
#include "../include/RPG_DialogSystem.h"
#include "../include/game.h"
using RPG_NS_ShopingSystem::RPG_ShopingSystem;
using RPG_NS_DialogSystem::RPG_DialogSystem;
using std::stringstream;
using GUI::GUIMessageBox;

int RPG_NS_ShopingSystem::RPG_ShopingSystem::status = RPG_NS_ShopingSystem::RPG_ShopingSystem::SHOP_NULL;
RPG_NS_ShopingSystem::RPG_ShopingSystem* RPG_NS_ShopingSystem::RPG_ShopingSystem::instance = NULL;

RPG_NS_ShopingSystem::RPG_ShopingSystem::RPG_ShopingSystem(int mapID, int unicode){
	string fileName;
	stringstream stream;
	stream << mapID;
	stream << "U";
	stream << unicode;
	m_mapID = mapID;
	m_npcUnicode = unicode;
	//��fileName�ļ��ж�ȡ������Ʒ�б�
	fileName = "data\\shop\\mapID"+stream.str()+".shop";
	FILE* fp = fopen(fileName.c_str(), "r");
	if(fp==NULL)throw "�޷����ļ�"+fileName;
	char material[MAX_NAME];
	int k = 0;
	while(!feof(fp)){
		fscanf(fp, "%s", material);
		if(strcmp(material, "END")==0)break;
		if(strcmp(material, "~")==0){k++;continue;}
		string name(material);
		if(k==0)m_weaponList.push_back(name);
		if(k==1)m_drugList.push_back(name);
		if(k==2)m_jewelryList.push_back(name);
	}
	fclose(fp);
	//��data\item\all.item�ж�ȡ������Ʒ��Ϣ
	fp = fopen("data\\item\\all.item", "rb");
	if(fp==NULL)throw string("�޷����ļ�")+string("data\\item\\all.item");
	Item item;
	while(!feof(fp)){
		int nRead = fread(&item, sizeof(Item), 1, fp);
		if(nRead==0)break;
		m_itemList.push_back(item);
	}
	fclose(fp);
	instance = this; //���ʵ��
	status = SHOP_LIVE; //����
	m_interface = SHOP_TOP; //Ĭ����ʾ���㰴ť
	gamePause = true; //��ͣ��Ϸ

	//
	m_button_top_buy = new GUI::GUIButton("image\\gui\\shop\\shopTopBuy1.bmp", "image\\gui\\shop\\shopTopBuy2.bmp",
		SHOP_STARTX+60, SHOP_STARTY+50, this);
	m_button_top_sale = new GUI::GUIButton("image\\gui\\shop\\shopTopSale1.bmp", "image\\gui\\shop\\shopTopBuy2.bmp",
		SHOP_STARTX+60, SHOP_STARTY+85, this);
	m_button_top_say = new GUI::GUIButton("image\\gui\\shop\\shopTopSay1.bmp", "image\\gui\\shop\\shopTopSay2.bmp",
		SHOP_STARTX+60, SHOP_STARTY+120, this);
	m_button_top_buy->setfClickEvent(TopBuyHelper);
	m_button_top_sale->setfClickEvent(TopSaleHelper);
	m_button_top_say->setfClickEvent(TopSayHelper);
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::render(SDL_Surface* destSurf){ //static
	if(status==SHOP_NULL)return;//����ִ��
	if(status==SHOP_DEAD){ //ɾ���Ѿ�������shop
		delete instance;
		instance = NULL;
		status = SHOP_NULL;
		return;
	}
	//����״̬
	if(instance->m_interface==SHOP_TOP){
		instance->m_button_top_buy->draw(destSurf);
		instance->m_button_top_sale->draw(destSurf);
		instance->m_button_top_say->draw(destSurf);
	}else if(instance->m_interface==SHOP_BUY||instance->m_interface==SHOP_SALE){
		instance->m_back->drawImage(destSurf, SHOP_STARTX, SHOP_STARTY);
		instance->m_button_left->draw(destSurf);
		instance->m_button_right->draw(destSurf);
		instance->m_label_name->draw(destSurf);
		instance->m_listbox_items->draw(destSurf);
		instance->m_button_ack->draw(destSurf);
		instance->m_button_exit->draw(destSurf);
		instance->m_label_money->draw(destSurf);
		if(instance->m_listbox_items->getSelectItem()!=-1){ //ѡ��ĳ����Ʒ
			if(instance->m_label_desc!=NULL)delete instance->m_label_desc;
			if(instance->m_label_cost!=NULL)delete instance->m_label_cost;
			string name = instance->m_listbox_items->getSelectValue(); //��Ʒ����
			string desc = "";
			int cost = 0;
			for(int i=0; i<instance->m_itemList.size(); i++){
				if(strcmp(instance->m_itemList[i].material, name.c_str())==0){
					desc = string(instance->m_itemList[i].desc);
					cost = instance->m_itemList[i].cost;
					break;
				}
			}
			if(instance->m_interface==SHOP_SALE)cost = cost*0.3; //��30%������Ʒ
			stringstream stream;
			stream << cost;
			string strCost = "�۸�:"+stream.str();
			//desc��Ҫ�ʵ�����(20�����һ�)
			desc = newLine(desc, 10);
			instance->m_label_desc = new GUI::GUILabel("font\\simkai.ttf", 
				15, desc, 0,0,0, SHOP_STARTX+35, SHOP_STARTY+185);
			instance->m_label_cost = new GUI::GUILabel("font\\simkai.ttf",
				15, strCost, 0,0,0, SHOP_STARTX+35, SHOP_STARTY+315);
			instance->m_label_desc->draw(destSurf);
			instance->m_label_cost->draw(destSurf);
			
			int nCount = 0; //ӵ�е�����
			if(instance->m_interface==SHOP_SALE){
				if(instance->m_label_count!=NULL)delete instance->m_label_count;
				//����������,��ʾӵ�е�����
				if(instance->m_wdj==0){//����
					for(int i=0; i<heroPackage.weapon.size(); i++){
						if(strcmp(heroPackage.weapon[i].material, name.c_str())==0){
							nCount = heroPackage.weapon[i].num;
						}
					}
				}
				else if(instance->m_wdj==1){//ҩƷ
					for(int i=0; i<heroPackage.drug.size(); i++){
						if(strcmp(heroPackage.drug[i].material, name.c_str())==0){
							nCount = heroPackage.drug[i].num;
						}
					}
				}else{
					for(int i=0; i<heroPackage.jewelry.size(); i++){
						if(strcmp(heroPackage.jewelry[i].material, name.c_str())==0){
							nCount = heroPackage.jewelry[i].num;
						}
					}
				}
				stringstream strCount;
				strCount << nCount;
				string lcount = "����:"+strCount.str();
				instance->m_label_count = new GUI::GUILabel("font\\simkai.ttf",
				15, lcount, 0,0,0, SHOP_STARTX+35, SHOP_STARTY+300);
				instance->m_label_count->draw(destSurf);
			}
		}
		if(instance->m_messagebox!=NULL){//������Ϣ��
			if(instance->m_messagebox->getSelection()==GUIMessageBox::MB_NULL){//��һ�û����ѡ��
				instance->m_messagebox->draw(destSurf);
			}else{//���ѡ����
				if(instance->m_doExchanging){ //������
					if(instance->m_messagebox->getSelection()==GUIMessageBox::MB_YES){//���ȷ��
						delete instance->m_messagebox;
						instance->m_messagebox = NULL;
						if(instance->m_interface==SHOP_BUY)
							instance->doBuying();
						else if(instance->m_interface==SHOP_SALE)
							instance->doSaling();
					}
					else{//���ȡ��
						instance->m_doExchanging = false; //ȡ������
						delete instance->m_messagebox;
						instance->m_messagebox = NULL;
					}
				}
				else{ //��ʾ,�ǽ���
					delete instance->m_messagebox;
					instance->m_messagebox = NULL;
				}
			}
		}
	}else if(instance->m_interface==SHOP_SAY){
	
	}
	
}

/****************************���а�ť�¼��Ĵ���*****************************/
void RPG_NS_ShopingSystem::RPG_ShopingSystem::initInterface(int interfaceID){
	m_interface = interfaceID; //ѡ�����
	//ɾ�����㰴ť
	delete m_button_top_buy;
	m_button_top_buy = NULL;
	delete m_button_top_sale;
	m_button_top_sale = NULL;
	delete m_button_top_say;
	m_button_top_say = NULL;

	m_wdj = 0; //Ĭ������"����"
	m_doExchanging = false;

	//��ʼ��"��������"����
	m_back = new RPG_NS_Image::RPG_Image("image\\gui\\shop\\shopBack.bmp");
	m_button_left = new GUI::GUIButton("image\\gui\\shop\\shopLeft1.bmp", "image\\gui\\shop\\shopLeft2.bmp",
		SHOP_STARTX+35, SHOP_STARTY+5, this);
	m_button_right = new GUI::GUIButton("image\\gui\\shop\\shopRight1.bmp", "image\\gui\\shop\\shopRight2.bmp",
		SHOP_STARTX+183, SHOP_STARTY+5, this);
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 20, "����",0,0,0, SHOP_STARTX+100, SHOP_STARTY+15);
	m_listbox_items = new GUI::GUIListBox(SHOP_STARTX+50, SHOP_STARTY+60, 150, 7);
	if(interfaceID==SHOP_BUY){//"��"
		m_button_ack = new GUI::GUIButton("image\\gui\\shop\\shopAckBuy1.bmp", "image\\gui\\shop\\shopAckBuy2.bmp",
		SHOP_STARTX+180, SHOP_STARTY+340, this);
	}
	else{//"��"
		m_button_ack = new GUI::GUIButton("image\\gui\\shop\\shopAckSale1.bmp", "image\\gui\\shop\\shopAckSale2.bmp",
		SHOP_STARTX+180, SHOP_STARTY+340, this);
	}
	m_button_exit = new GUI::GUIButton("image\\gui\\shop\\shopExit1.bmp", "image\\gui\\shop\\shopExit2.bmp", 
		SHOP_STARTX+223, SHOP_STARTY+12, this);
	stringstream total;
	total << "$";
	total << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 16, total.str(),
		255, 255, 128, SHOP_STARTX+45, SHOP_STARTY+340);
	m_label_desc = NULL;
	m_label_cost = NULL;
	m_label_count = NULL;
	m_messagebox = NULL;
	//����¼�
	m_button_left->setfClickEvent(LeftHelper);
	m_button_right->setfClickEvent(RightHelper);
	m_button_ack->setfClickEvent(AckHelper);
	m_button_exit->setfClickEvent(ExitHelper);
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onTopBuy(){
	initInterface(SHOP_BUY);
	//���"����"��Ʒ
	for(int i=0; i<m_weaponList.size(); i++){
		m_listbox_items->addItem(m_weaponList[i]);
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onTopSale(){
	initInterface(SHOP_SALE);
	for(int i=0; i<heroPackage.weapon.size(); i++){//������ʱ�������������е�����
		m_listbox_items->addItem(string(heroPackage.weapon[i].material));
	}

}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onTopSay(){
	delete m_button_top_buy;
	m_button_top_buy = NULL;
	delete m_button_top_sale;
	m_button_top_sale = NULL;
	delete m_button_top_say;
	m_button_top_say = NULL;
	status = SHOP_DEAD; //����ShopSystem
	gamePause = false;

	RPG_DialogSystem::createDialog(m_mapID, m_npcUnicode);//�����Ի�ϵͳ

}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onLeft(){ //�л�"����","ҩƷ","����"
	if(instance->m_wdj==0)return;
	//����Ϣ��,ûȷ������Ӧ������Ϣ
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	instance->m_wdj--;
	//ɾ����ʱ�Ŀؼ�
	delete instance->m_label_name;
	delete instance->m_listbox_items;
	if(instance->m_label_desc!=NULL){
		delete instance->m_label_desc;
		instance->m_label_desc = NULL;
	}
	if(instance->m_label_cost!=NULL){
		delete instance->m_label_cost;
		instance->m_label_cost = NULL;
	}
	if(instance->m_label_count!=NULL){
		delete instance->m_label_count;
		instance->m_label_count = NULL;
	}
	//������
	string name;
	if(m_wdj==0)name = "����";
	else if(m_wdj==1)name = "ҩƷ";
	else if(m_wdj==2)name = "����";
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 20, name,0,0,0, SHOP_STARTX+100, SHOP_STARTY+15);
	m_listbox_items = new GUI::GUIListBox(SHOP_STARTX+50, SHOP_STARTY+60, 150, 7);
	//�����ǡ��򡱻��ǡ����������Ӧ��Ʒ
	if(m_interface==SHOP_BUY){ //���򡱣���Ӵ��ļ���������Ʒ
		if(m_wdj==0){
			for(int i=0; i<m_weaponList.size(); i++){
				m_listbox_items->addItem(m_weaponList[i]);
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<m_drugList.size(); i++){
				m_listbox_items->addItem(m_drugList[i]);
			}
		}
		else{
			for(int i=0; i<m_jewelryList.size(); i++){
				m_listbox_items->addItem(m_jewelryList[i]);
			}
		}
	}
	else{//�������������������е���Ʒ
		if(m_wdj==0){
			for(int i=0; i<heroPackage.weapon.size(); i++){//������ʱ�������������е�����
				m_listbox_items->addItem(string(heroPackage.weapon[i].material));
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<heroPackage.drug.size(); i++){
				m_listbox_items->addItem(string(heroPackage.drug[i].material));
			}
		}
		else{
			for(int i=0; i<heroPackage.jewelry.size(); i++){
				m_listbox_items->addItem(string(heroPackage.jewelry[i].material));
			}
		}
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onRight(){
	if(instance->m_wdj==2)return;
	//����Ϣ��,ûȷ������Ӧ������Ϣ
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	instance->m_wdj++;
	//ɾ����ʱ�Ŀؼ�
	delete instance->m_label_name;
	delete instance->m_listbox_items;
	if(instance->m_label_desc!=NULL){
		delete instance->m_label_desc;
		instance->m_label_desc = NULL;
	}
	if(instance->m_label_cost!=NULL){
		delete instance->m_label_cost;
		instance->m_label_cost = NULL;
	}
	if(instance->m_label_count!=NULL){
		delete instance->m_label_count;
		instance->m_label_count = NULL;
	}
	//������
	string name;
	if(m_wdj==0)name = "����";
	else if(m_wdj==1)name = "ҩƷ";
	else if(m_wdj==2)name = "����";
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 20, name,0,0,0, SHOP_STARTX+100, SHOP_STARTY+15);
	m_listbox_items = new GUI::GUIListBox(SHOP_STARTX+50, SHOP_STARTY+60, 150, 7);
	//�����ǡ��򡱻��ǡ����������Ӧ��Ʒ
	if(m_interface==SHOP_BUY){ //���򡱣���Ӵ��ļ���������Ʒ
		if(m_wdj==0){
			for(int i=0; i<m_weaponList.size(); i++){
				m_listbox_items->addItem(m_weaponList[i]);
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<m_drugList.size(); i++){
				m_listbox_items->addItem(m_drugList[i]);
			}
		}
		else{
			for(int i=0; i<m_jewelryList.size(); i++){
				m_listbox_items->addItem(m_jewelryList[i]);
			}
		}
	}
	else{//�������������������е���Ʒ
		if(m_wdj==0){
			for(int i=0; i<heroPackage.weapon.size(); i++){//������ʱ�������������е�����
				m_listbox_items->addItem(string(heroPackage.weapon[i].material));
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<heroPackage.drug.size(); i++){
				m_listbox_items->addItem(string(heroPackage.drug[i].material));
			}
		}
		else{
			for(int i=0; i<heroPackage.jewelry.size(); i++){
				m_listbox_items->addItem(string(heroPackage.jewelry[i].material));
			}
		}
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onAck(){
	//����Ϣ��,ûȷ������Ӧ������Ϣ
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	if(m_listbox_items->getSelectItem()<0){ //��ûѡ����Ʒ
		if(m_messagebox!=NULL)delete m_messagebox;
		m_messagebox = new GUI::GUIMessageBox("", "û��ѡ����Ʒ!", "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		return;
	}
	
	if(m_interface==SHOP_BUY){//����Ʒ
		string msg = "ȷʵҪ�� "+m_listbox_items->getSelectValue()+" ��?";
		msg = newLine(msg, 10);
		m_messagebox = new GUI::GUIMessageBox("", msg, "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		m_doExchanging = true; //���뽻����
		return;
	}
	if(m_interface==SHOP_SALE){//����Ʒ
		string msg = "ȷʵҪ�� "+m_listbox_items->getSelectValue()+" ��?";
		msg = newLine(msg, 10);
		m_messagebox = new GUI::GUIMessageBox("", msg, "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		m_doExchanging = true; //���뽻����
		return;
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onExit(){
	//����Ϣ��,ûȷ������Ӧ������Ϣ
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	delete m_back;
	delete m_button_left;
	delete m_button_right;
	delete m_label_name;
	delete m_listbox_items;
	if(m_label_desc!=NULL)delete m_label_desc;
	if(m_label_count!=NULL)delete m_label_count;
	delete m_button_ack;
	delete m_button_exit;
	delete m_label_money;
	status = SHOP_DEAD; //����ShopSystem
	gamePause = false;
}

/****************************************************************************/
void RPG_NS_ShopingSystem::RPG_ShopingSystem::doBuying(){
	int cost = 0;
	string name = m_listbox_items->getSelectValue();
	for(int i=0; i<m_itemList.size(); i++){
		if(strcmp(m_itemList[i].material, name.c_str())==0){
			cost = m_itemList[i].cost;
			break;
		}
	}
	if(cost>heroPackage.finance){
		m_messagebox = new GUI::GUIMessageBox("", "�ʽ���!", "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		m_doExchanging = false;//ȡ������
		return;
	}

	heroPackage.finance -= cost;
	addToPackage(m_wdj, name.c_str()); //��ӽ�����
	m_doExchanging = false;
	if(m_wdj==0){ //Ϊ�˸����б���Ϣ
		onRight();
		onLeft();
	}else{
		onLeft();
		onRight();
	}
	delete m_label_money;
	stringstream total;
	total << "$";
	total << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 16, total.str(),
		255, 255, 128, SHOP_STARTX+45, SHOP_STARTY+340);
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::doSaling(){
	int cost = 0;
	string name = m_listbox_items->getSelectValue();
	for(int i=0; i<m_itemList.size(); i++){
		if(strcmp(m_itemList[i].material, name.c_str())==0){
			cost = m_itemList[i].cost;
			break;
		}
	}
	heroPackage.finance += cost*0.3;
	delFromPackage(m_wdj, name.c_str()); //��ӽ�����
	m_doExchanging = false;
	if(m_wdj==0){ //Ϊ�˸����б���Ϣ
		onRight();
		onLeft();
	}else{
		onLeft();
		onRight();
	}
	delete m_label_money;
	stringstream total;
	total << "$";
	total << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 16, total.str(),
		255, 255, 128, SHOP_STARTX+45, SHOP_STARTY+340);
}
