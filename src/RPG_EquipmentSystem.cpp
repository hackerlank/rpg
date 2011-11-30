#include <sstream>
#include "../include/RPG_EquipmentSystem.h"
using RPG_NS_EquipmentSystem::RPG_EquipmentSystem;
using std::stringstream;

RPG_EquipmentSystem* RPG_EquipmentSystem::instance = NULL;
int RPG_EquipmentSystem::status = RPG_EquipmentSystem::EQP_NULL;

string RPG_EquipmentSystem::equipLabel[] = {
				"ͷ��:    ", "�·�:    ", "Ь��:    ", "����:    ", "����:    ", "ͷ��:    ", 
				"����:    ", "����1:    ", "����2:    ", "����:    ","����1:    ", 
				"����2:    ", "��ָ:    ", "�����1:    ", "�����2:    ", "�����3:    "
};
string RPG_EquipmentSystem::statLabel[] = {
				"������:", "�������:", "ħ������:", "ħ������:", "�ٶ�:"
};

RPG_EquipmentSystem::RPG_EquipmentSystem(){
	//�����ļ�,��Ʒ����ϸ������Ϣ
	////////////////////
	FILE* fp;
	fp = fopen("data\\item\\all.item", "rb");
	if(fp==NULL)throw string("�޷����ļ�")+string("data\\item\\all.item");
	Item item;
	while(!feof(fp)){
		int nRead = fread(&item, sizeof(Item), 1, fp);
		if(nRead==0)break;
		m_itemList.push_back(item);
	}
	fclose(fp);
	////////////////////
	setNULL();
	showHero(0); //Ĭ����ʾ��һ��Ӣ��

	//���캯������GUI�в���Ĳ���
	m_image_back = new RPG_NS_Image::RPG_Image("image\\gui\\equip\\back.bmp");
	m_button_left = new GUI::GUIButton("image\\gui\\equip\\equipLeft1.bmp",
		"image\\gui\\equip\\equipLeft2.bmp", 40, 10, this);
	m_button_right = new GUI::GUIButton("image\\gui\\equip\\equipRight1.bmp",
		"image\\gui\\equip\\equipRight2.bmp", 200, 10, this);
	m_button_uninstall = new GUI::GUIButton("image\\gui\\equip\\equipUninstall1.bmp",
		"image\\gui\\equip\\equipUninstall2.bmp", 355, 420, this);
	m_button_install = new GUI::GUIButton("image\\gui\\equip\\equipInstall1.bmp",
		"image\\gui\\equip\\equipInstall2.bmp", 505, 420, this);
	m_button_drink = new GUI::GUIButton("image\\gui\\equip\\equipDrink1.bmp",
		"image\\gui\\equip\\equipDrink2.bmp", 655, 420, this);
	m_button_exit = new GUI::GUIButton("image\\gui\\equip\\equipExit1.bmp",
		"image\\gui\\equip\\equipExit2.bmp", 755, 20, this);
	status = EQP_LIVE; //����
	instance = this;
	gamePause = true; //��ͣ��Ϸ

	//����¼�
	m_button_left->setfClickEvent(onLeftHelper);
	m_button_right->setfClickEvent(onRightHelper);
	m_button_uninstall->setfClickEvent(onUninstallHelper);
	m_button_install->setfClickEvent(onInstallHelper);
	m_button_drink->setfClickEvent(onDrinkHelper);
	m_button_exit->setfClickEvent(onExitHelper);
}

RPG_EquipmentSystem::~RPG_EquipmentSystem(){
	delAllGUI();
	if(m_button_left!=NULL)delete m_button_left;
	if(m_button_right!=NULL)delete m_button_right;
	if(m_button_uninstall!=NULL)delete m_button_uninstall;
	if(m_button_install!=NULL)delete m_button_install;
	if(m_button_drink!=NULL)delete m_button_drink;
	if(m_button_exit!=NULL)delete m_button_exit;
}

void RPG_EquipmentSystem::showHero(int index){
	if(index<0 || index>=heroSet.heroCount)return;
	m_heroIndex = index;
	/******************ɾ����ʧ��GUI�ؼ�*********************/
	delAllGUI();
	
	/********************************************************/

	/*******************����µ�GUI�ؼ�***********************/
	string heroName = string(heroSet.heros[index].heroName);
	m_image_hero = new RPG_NS_Image::RPG_Image("image\\gui\\surface\\hero\\hero_"+heroName+".bmp");
	m_image_hero->setColorKey(255, 0, 255);

	//���
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 21, string(heroSet.heros[index].heroName),
		0, 0, 0, 95, 17);
	m_label_level = new GUI::GUILabel("font\\simkai.ttf", 15, "�ȼ�: "+intToString(heroSet.heros[index].level),
		0, 0, 0, 30, 55);
	string exp = "����: "+intToString(heroSet.heros[index].experience)
		+"  ��һ������: "+intToString(heroSet.heros[index].nextExperience);
	m_label_experience = new GUI::GUILabel("font\\simkai.ttf", 15, exp, 0, 0, 0, 30, 75);
	double blood_radio, mana_radio, cp_radio;
	blood_radio = (double)heroSet.heros[index].blood / heroSet.heros[index].attribute.zdxl; //Ѫ���ٷֱ�
	mana_radio = (double)heroSet.heros[index].mana / heroSet.heros[index].attribute.zdml; //ħ���ٷֱ�
	cp_radio = (double)heroSet.heros[index].cp / 100.0; //CP�ٷֱ�
	m_label_blood = new GUI::GUILabel("font\\simkai.ttf", 15, "Ѫ:"+intToString(heroSet.heros[index].blood)+
		"/"+intToString(heroSet.heros[index].attribute.zdxl), 0, 0, 0, 20, 445);
	m_label_mana = new GUI::GUILabel("font\\simkai.ttf", 15, "ħ:"+intToString(heroSet.heros[index].mana)+
		"/"+intToString(heroSet.heros[index].attribute.zdml), 0, 0, 0, 20, 465);
	m_label_cp = new GUI::GUILabel("font\\simkai.ttf", 15, "CP:"+intToString(heroSet.heros[index].cp)+
		"/100", 0, 0, 0, 20, 485);
	m_progressbar_blood = new GUI::GUIProgressBar(115, 445, 130, 12, 250, 40, 60);//��ɫ
	m_progressbar_blood->setRadio(blood_radio);
	m_progressbar_mana = new GUI::GUIProgressBar(115, 465, 130, 12, 70, 40, 240);//��ɫ
	m_progressbar_mana->setRadio(mana_radio);
	m_progressbar_cp = new GUI::GUIProgressBar(115, 485, 130, 12, 220, 240, 60);//��ɫ
	m_progressbar_cp->setRadio(cp_radio);
	int basex, basey;
	basex = 20;
	basey = 515;
	int value[5];
	value[0] = heroSet.heros[index].attribute.wg;
	value[1] = heroSet.heros[index].attribute.wf;
	value[2] = heroSet.heros[index].attribute.mg;
	value[3] = heroSet.heros[index].attribute.mf;
	value[4] = heroSet.heros[index].attribute.sudu;
	int ext[5] = {0};
	for(int i=0; i<heroSet.items[m_heroIndex].size(); i++){
		Item t = getItemInfo(string(heroSet.items[m_heroIndex][i].material));
		ext[0] += t.eqs.wg;
		ext[1] += t.eqs.wf;
		ext[2] += t.eqs.mg;
		ext[3] += t.eqs.mf;
		ext[4] += t.eqs.sudu;
	}
	for(int i=0; i<5; i++){//��������
		string str = statLabel[i]+intToString(value[i]);
		if(ext[i]>0) str += " +"+intToString(ext[i]);
		else if(ext[i]<0) str += " -"+intToString(ext[i]);
		m_label_attribute[i] = new GUI::GUILabel("font\\simkai.ttf", 15, str,
			0, 0, 0, basex+(i%2)*120, basey+(i/2)*20);
	}	
	//�Ҳ�
	for(int i=0; i<heroSet.items[index].size(); i++){
		//ö��Ӣ�����ϵ�װ����ʾ������λ��
		Item t = getItemInfo(string(heroSet.items[index][i].material));
		if(t.type!=0 && t.type!=2)continue; //ֻ��װ����Ʒ����װ
		if(m_hasEquip[t.eqpID]==""){
			m_hasEquip[t.eqpID] = string(t.material);//���װ������
		}
		else{//�쿴���������������
			if(t.eqpID==EQP_ERHUAN && m_hasEquip[EQP_ERHUAN+1]==""){//��������
				m_hasEquip[EQP_ERHUAN+1] = string(t.material);
			}
			else if(t.eqpID==EQP_SHOUZHUO && m_hasEquip[EQP_SHOUZHUO+1]==""){//��������
				m_hasEquip[EQP_SHOUZHUO+1] = string(t.material);
			}
			else if(t.eqpID==EQP_HUSHENFU){//�����
				if(m_hasEquip[EQP_HUSHENFU+1]==""){
					m_hasEquip[EQP_HUSHENFU+1] = string(t.material);
				}
				else if(m_hasEquip[EQP_HUSHENFU+2]==""){
					m_hasEquip[EQP_HUSHENFU+2] = string(t.material);
				}
			}
		}
	}
	basex = 370;
	basey = 50;
	for(int i=0; i<16; i++){
		string str = equipLabel[i]+m_hasEquip[i];
		m_label_equipLabel[i] = new GUI::GUILabel("font\\simkai.ttf", 16, str,
			0, 0, 0, basex+(i/8)*250, basey+(i%8)*20); //����װ����Ϣ
	}

	m_listbox_hequip = new GUI::GUIListBox(340, 240, 100, 10);
	m_listbox_pequip = new GUI::GUIListBox(490, 240, 100, 10);
	m_listbox_drug = new GUI::GUIListBox(640, 240, 100, 10);
	//add to listbox
	for(int i=0; i<heroSet.items[index].size(); i++)
		m_listbox_hequip->addItem(string(heroSet.items[index][i].material));
	for(int i=0; i<heroPackage.weapon.size(); i++){
		string str = string(heroPackage.weapon[i].material)
			+"("+intToString(heroPackage.weapon[i].num)+")";
		m_listbox_pequip->addItem(str);
	}
	for(int i=0; i<heroPackage.jewelry.size(); i++){
		string str = string(heroPackage.jewelry[i].material)
			+"("+intToString(heroPackage.jewelry[i].num)+")";
		m_listbox_pequip->addItem(str);
	}
	for(int i=0; i<heroPackage.story.size(); i++){
		string str = string(heroPackage.story[i].material)
			+"("+intToString(heroPackage.story[i].num)+")";
		m_listbox_pequip->addItem(str);
	}
	for(int i=0; i<heroPackage.drug.size(); i++){
		string str = string(heroPackage.drug[i].material)
			+"("+intToString(heroPackage.drug[i].num)+")";
		m_listbox_drug->addItem(str);
	}
	
	/********************************************************/
}

void RPG_EquipmentSystem::render(SDL_Surface* destSurf){
	if(status==EQP_NULL)return;
	if(status==EQP_DEAD){
		delete instance;
		instance = NULL;
		return;
	}
	//ͼƬ
	instance->m_image_back->drawImage(destSurf, 0, 0); //����ͼƬ
	instance->m_image_hero->drawImage(destSurf, 25, 90); //Ӣ�۵�ͼƬ

	//��ť
	instance->m_button_left->draw(destSurf); //����ť
	instance->m_button_right->draw(destSurf); //���Ұ�ť
	instance->m_button_uninstall->draw(destSurf); //ж��װ��
	instance->m_button_install->draw(destSurf); //װ����ȥ
	instance->m_button_drink->draw(destSurf); //��ҩ
	instance->m_button_exit->draw(destSurf); //�ص���Ϸ
	
	//������
	instance->m_progressbar_blood->draw(destSurf); //Ѫ
	instance->m_progressbar_mana->draw(destSurf); //ħ
	instance->m_progressbar_cp->draw(destSurf); //CP
	//�б��
	instance->m_listbox_hequip->draw(destSurf); //Ӣ�����ϵ�װ���б�
	instance->m_listbox_pequip->draw(destSurf); //�����е�װ���б�
	instance->m_listbox_drug->draw(destSurf); //�����е�ҩƷ�б�
	//��ǩ��
	/**************************************/
	instance->m_label_name->draw(destSurf); //��������
	instance->m_label_level->draw(destSurf); //����ȼ�
	instance->m_label_experience->draw(destSurf); //����,��һ������
	instance->m_label_blood->draw(destSurf); //Ѫ,�ٷֱ�
	instance->m_label_mana->draw(destSurf); //ħ,�ٷֱ�
	instance->m_label_cp->draw(destSurf); //CP,�ٷֱ�
	for(int i=0; i<5; i++)
		instance->m_label_attribute[i]->draw(destSurf); //������,�������,ħ������,ħ������,�ٶ���Ϣ
	/**************************************/
	for(int i=0; i<16; i++)
		instance->m_label_equipLabel[i]->draw(destSurf);//װ���������ǩ
	if(instance->m_listbox_hequip->getSelectItem()>=0){//"ж���б��"��ѡ��
		if(instance->desc1!=NULL)delete instance->desc1;
		string value = instance->m_listbox_hequip->getSelectValue();
		Item t = instance->getItemInfo(value);
		string str = newLine(t.desc, 6);
		instance->desc1 = new GUI::GUILabel("font\\simkai.ttf", 15, str, 0, 0, 0, 340, 460);
		instance->desc1->draw(destSurf);
	}
	if(instance->m_listbox_pequip->getSelectItem()>=0){//"װ���б��"��ѡ��
		if(instance->desc2!=NULL)delete instance->desc2;
		string value = instance->m_listbox_pequip->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = instance->getItemInfo(value);
		string str = newLine(t.desc, 6);
		instance->desc2 = new GUI::GUILabel("font\\simkai.ttf", 15, str, 0, 0, 0, 490, 460);
		instance->desc2->draw(destSurf);
	}
	if(instance->m_listbox_drug->getSelectItem()>=0){//"ʳ���б��"��ѡ��
		if(instance->desc3!=NULL)delete instance->desc3;
		string value = instance->m_listbox_drug->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = instance->getItemInfo(value);
		string str = newLine(t.desc, 6);
		instance->desc3 = new GUI::GUILabel("font\\simkai.ttf", 15, str, 0, 0, 0, 640, 460);
		instance->desc3->draw(destSurf);
	}
}

string RPG_EquipmentSystem::intToString(int n){
	stringstream stream;
	stream << n;
	return stream.str();
}

Item RPG_EquipmentSystem::getItemInfo(string material){
	for(int i=0; i<m_itemList.size(); i++){
		if(strcmp(m_itemList[i].material, material.c_str())==0){
			//�ҵ�����Ʒ
			return m_itemList[i];
		}
	}
	return Item();
}

void RPG_EquipmentSystem::setNULL(){
	m_image_hero = NULL;

	m_progressbar_blood = NULL;
	m_progressbar_mana = NULL;
	m_progressbar_cp = NULL;

	m_listbox_hequip = NULL;
	m_listbox_pequip = NULL;
	m_listbox_drug = NULL;

	m_label_name = NULL;
	m_label_level = NULL;
	m_label_experience = NULL;
	m_label_blood = NULL;
	m_label_mana = NULL;
	m_label_cp = NULL;
	desc1 = desc2 = desc3 = NULL;
	for(int i=0; i<5; i++){
		m_label_attribute[i] = NULL;
	}
	for(int i=0; i<16; i++){
		m_label_equipLabel[i] = NULL;
	}
}

void RPG_EquipmentSystem::delAllGUI(){
	if(m_image_hero!=NULL)delete m_image_hero;

	if(m_progressbar_blood!=NULL)delete m_progressbar_blood;
	if(m_progressbar_mana!=NULL)delete m_progressbar_mana;
	if(m_progressbar_cp!=NULL)delete m_progressbar_cp;

	if(m_listbox_hequip!=NULL)delete m_listbox_hequip;
	if(m_listbox_pequip!=NULL)delete m_listbox_pequip;
	if(m_listbox_drug!=NULL)delete m_listbox_drug;

	if(m_label_name!=NULL)delete m_label_name;
	if(m_label_level!=NULL)delete m_label_level;
	if(m_label_experience!=NULL)delete m_label_experience;
	if(m_label_blood!=NULL)delete m_label_blood;
	if(m_label_mana!=NULL)delete m_label_mana;
	if(m_label_cp!=NULL)delete m_label_cp;
	for(int i=0; i<5; i++){
		if(m_label_attribute[i]!=NULL)delete m_label_attribute[i];
	}
	for(int i=0; i<16; i++){
		if(m_label_equipLabel[i]!=NULL)delete m_label_equipLabel[i];
		m_hasEquip[i] = "";
	}
	setNULL();
}

/****************************��ť����**********************************/
void RPG_EquipmentSystem::onLeft(){
	if(m_heroIndex==0)return;
	m_heroIndex--;
	showHero(m_heroIndex);
}

void RPG_EquipmentSystem::onRight(){
	if(m_heroIndex==heroSet.heroCount-1)return;
	m_heroIndex++;
	showHero(m_heroIndex);
}

void RPG_EquipmentSystem::onUninstall(){
	if(m_listbox_hequip->getSelectItem()>=0){//"ж���б��"��ѡ��
		string value = m_listbox_hequip->getSelectValue();
		Item t = getItemInfo(value);
	
		//��Ӣ������ж��װ��
		delFromHero(m_heroIndex, value);
		//��ӵ�������
		addToPackage(t.type, value);
		/////����Ӣ�۵Ĳ���//////

		///////////////////////
		showHero(m_heroIndex);
	}
}

void RPG_EquipmentSystem::onInstall(){
	if(m_listbox_pequip->getSelectItem()>=0){//"װ���б��"��ѡ��
		string value = m_listbox_pequip->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = getItemInfo(value);
		if(t.type!=0 && t.type!=2)return;; //ֻ��װ����Ʒ����װ
		bool bFixed = false; //�Ƿ��п�λװ
		if(m_hasEquip[t.eqpID]==""){
			//m_hasEquip[t.eqpID] = string(t.material);//���װ������
			bFixed = true;
		}
		else{//�쿴���������������
			if(t.eqpID==EQP_ERHUAN && m_hasEquip[EQP_ERHUAN+1]==""){//��������
				//m_hasEquip[EQP_ERHUAN+1] = string(t.material);
				bFixed = true;
			}
			else if(t.eqpID==EQP_SHOUZHUO && m_hasEquip[EQP_SHOUZHUO+1]==""){//��������
				//m_hasEquip[EQP_SHOUZHUO+1] = string(t.material);
				bFixed = true;
			}
			else if(t.eqpID==EQP_HUSHENFU){//�����
				if(m_hasEquip[EQP_HUSHENFU+1]==""){
					//m_hasEquip[EQP_HUSHENFU+1] = string(t.material);
					bFixed = true;
				}
				else if(m_hasEquip[EQP_HUSHENFU+2]==""){
					//m_hasEquip[EQP_HUSHENFU+2] = string(t.material);
					bFixed = true;
				}
			}
		}
		if(bFixed){//����װ��
			//��ӵ�Ӣ������
			addToHero(m_heroIndex, value);
			//�Ӱ�����ɾ��һ��
			delFromPackage(t.type, value);
			/////����Ӣ�۵Ĳ���//////

			///////////////////////
			showHero(m_heroIndex);

		}
	}
}

void RPG_EquipmentSystem::onDrink(){
	if(instance->m_listbox_drug->getSelectItem()>=0){//"ʳ���б��"��ѡ��
		string value = instance->m_listbox_drug->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = instance->getItemInfo(value);
		if(t.dgs.canUse==false)return; //��ҩƷ������װ��ϵͳ��ʹ��
		int add;
		//����Ӣ�۵�Ѫ,ħ,��CP
		add = t.dgs.addBlood;
		if(add>=RADIO_BASE){//Ѫ���ٷֱȼ�
			add = heroSet.heros[m_heroIndex].attribute.zdxl * ((add-RADIO_BASE)/100.0);
		}
		heroSet.heros[m_heroIndex].blood += add;
		if(heroSet.heros[m_heroIndex].blood>heroSet.heros[m_heroIndex].attribute.zdxl){
			heroSet.heros[m_heroIndex].blood = heroSet.heros[m_heroIndex].attribute.zdxl;//���ܳ������Ѫ��
		}

		add = t.dgs.addMana;
		if(add>=RADIO_BASE){//ħ���ٷֱȼ�
			add = heroSet.heros[m_heroIndex].attribute.zdml * ((add-RADIO_BASE)/100.0);
		}
		heroSet.heros[m_heroIndex].mana += add;
		if(heroSet.heros[m_heroIndex].mana>heroSet.heros[m_heroIndex].attribute.zdml){
			heroSet.heros[m_heroIndex].mana = heroSet.heros[m_heroIndex].attribute.zdml;//���ܳ������ħ��
		}

		add = t.dgs.addCp;
		if(add>=RADIO_BASE){//CP���ٷֱȼ�
			add = 100 * ((add-RADIO_BASE)/100.0);
		}
		heroSet.heros[m_heroIndex].cp += add;
		if(heroSet.heros[m_heroIndex].cp>100){
			heroSet.heros[m_heroIndex].cp = 100;//���ܳ������cp��
		}
		//ɾ��һ����ҩƷ
		delFromPackage(1, value);
		showHero(m_heroIndex);
	}
}

void RPG_EquipmentSystem::onExit(){
	status = EQP_DEAD;
	gamePause = false;
}