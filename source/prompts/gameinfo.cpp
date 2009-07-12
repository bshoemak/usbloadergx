#include <gccore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "usbloader/wbfs.h"
#include "language/gettext.h"
#include "libwiigui/gui.h"
#include "../xml/xml.h"
#include "menu.h"
#include "filelist.h"
#include "sys.h"
#include "wpad.h"
#include "fatmounter.h"
#include "listfiles.h"
#include "gameinfo.h"


/*** Extern variables ***/
extern GuiWindow * mainWindow;
extern GuiSound * bgMusic;
extern u8 shutdown;
extern u8 reset;

/*** Extern functions ***/
extern void ResumeGui();
extern void HaltGui();


/****************************************************************************
* gameinfo
***************************************************************************/
int showGameInfo(char *ID)
{
    //load the xml shit
	bool databaseopened = true;
	OpenXMLDatabase(Settings.titlestxt_path, Settings.db_language, Settings.db_JPtoEN, true, false, true); // open file, do not load titles, keep in memory
    if(databaseopened) {

		LoadGameInfoFromXML(ID, Settings.db_language);
		CloseXMLDatabase();

		bool showmeminfo = false;
		
		int choice=-1;
		int titley=10;
		int marginY=titley+40;
		int indexy = marginY;
		int wifiY=0;
		int intputX=200, inputY=-30, txtXOffset=90;
		u8 nunchuk=0,
		classiccontroller=0,
		balanceboard=0,
		dancepad=0,
		guitar=0,
		gamecube=0,
		wheel=0,
		motionplus=0,
		drums=0,
		microphone=0;
		int newline=1;
		u8 page=1;
			
		GuiImageData * playersImgData = NULL;
		GuiImage * playersImg = NULL;

		GuiImageData * wifiplayersImgData = NULL;
		GuiImage * wifiplayersImg = NULL;
		GuiImage * ratingImg = NULL;

		GuiImage * classiccontrollerImg = NULL;
		GuiImage * nunchukImg = NULL;
		GuiImage * guitarImg = NULL;
		GuiImage * drumsImg = NULL;
		GuiImage * dancepadImg = NULL;
		GuiImage * motionplusImg = NULL;
		GuiImage * wheelImg = NULL;
		GuiImage * balanceboardImg = NULL;
		GuiImage * microphoneImg = NULL;
		GuiImage * gcImg = NULL;
		GuiImage * dialogBoxImg1 = NULL;
		GuiImage * dialogBoxImg2 = NULL;
		GuiImage * dialogBoxImg3 = NULL;
		GuiImage * dialogBoxImg4 = NULL;
		GuiImage * dialogBoxImg11 = NULL;
		GuiImage * dialogBoxImg22 = NULL;
		GuiImage * dialogBoxImg33 = NULL;
		GuiImage * dialogBoxImg44 = NULL;
		GuiImage * coverImg = NULL;
		GuiImage * coverImg2 = NULL;

		GuiImageData * classiccontrollerImgData = NULL;
		GuiImageData * nunchukImgData = NULL;
		GuiImageData * guitarImgData = NULL;
		GuiImageData * drumsImgData = NULL;
		GuiImageData * motionplusImgData = NULL;
		GuiImageData * wheelImgData = NULL;
		GuiImageData * balanceboardImgData = NULL;
		GuiImageData * dancepadImgData = NULL;
		GuiImageData * microphoneImgData = NULL;
		GuiImageData * gamecubeImgData = NULL;
		GuiImageData * ratingImgData = NULL;
		GuiImageData * cover = NULL;

		GuiText * releasedTxt = NULL;
		GuiText * publisherTxt = NULL;
		GuiText * developerTxt = NULL;
		GuiText * titleTxt = NULL;
		GuiText * synopsisTxt = NULL;
		GuiText ** genreTxt = NULL;
		GuiText ** wifiTxt = NULL;
		GuiText * betaTxt = NULL;
		GuiText * beta1Txt = NULL;
		GuiText * memTxt = NULL;

		GuiWindow gameinfoWindow(600,308);
		gameinfoWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		gameinfoWindow.SetPosition(0, -50);

		GuiWindow gameinfoWindow2(600,308);
		gameinfoWindow2.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		gameinfoWindow2.SetPosition(0, -50);

		GuiWindow txtWindow(350,270);
		txtWindow.SetAlignment(ALIGN_CENTRE, ALIGN_RIGHT);
		txtWindow.SetPosition(85, 50);

		GuiSound btnSoundOver(button_over_pcm, button_over_pcm_size, SOUND_PCM, Settings.sfxvolume);
		GuiSound btnClick(button_click2_pcm, button_click2_pcm_size, SOUND_PCM, Settings.sfxvolume);
		char imgPath[50];
		snprintf(imgPath, sizeof(imgPath), "%sbutton_dialogue_box.png", CFG.theme_path);
		GuiImageData btnOutline(imgPath, button_dialogue_box_png);
		snprintf(imgPath, sizeof(imgPath), "%sgameinfo1_png.png", CFG.theme_path);
		GuiImageData dialogBox1(imgPath, gameinfo1_png);
		snprintf(imgPath, sizeof(imgPath), "%sgameinfo1a_png.png", CFG.theme_path);
		GuiImageData dialogBox2(imgPath, gameinfo1a_png);
		snprintf(imgPath, sizeof(imgPath), "%sgameinfo2_png.png", CFG.theme_path);
		GuiImageData dialogBox3(imgPath, gameinfo2_png);
		snprintf(imgPath, sizeof(imgPath), "%sgameinfo2a_png.png", CFG.theme_path);
		GuiImageData dialogBox4(imgPath, gameinfo2a_png);

		GuiTrigger trigA;
		trigA.SetButtonOnlyTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
		GuiTrigger trigB;
		trigB.SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);

		GuiButton backBtn(0,0);
		backBtn.SetPosition(-20,-20);
		backBtn.SetTrigger(&trigB);
		gameinfoWindow.Append(&backBtn);

		GuiButton nextBtn(0,0);
		nextBtn.SetPosition(20,20);
		nextBtn.SetTrigger(&trigA);
		gameinfoWindow.Append(&nextBtn);
		
		char linebuf[1000] = "";
		char linebuf2[100] = "";

		// enable icons for required accessories
		for (int i=1;i<=XML_ELEMMAX;i++)
		{
			if (strcmp(gameinfo.accessoriesReq[i],"classiccontroller")==0)
				classiccontroller=1;
			if (strcmp(gameinfo.accessoriesReq[i],"nunchuk")==0)
				nunchuk=1;
			if (strcmp(gameinfo.accessoriesReq[i],"guitar")==0)
				guitar=1;
			if (strcmp(gameinfo.accessoriesReq[i],"drums")==0)
				drums=1;
			if (strcmp(gameinfo.accessoriesReq[i],"dancepad")==0)
				dancepad=1;
			if (strcmp(gameinfo.accessoriesReq[i],"motionplus")==0)
				motionplus=1;
			if (strcmp(gameinfo.accessoriesReq[i],"wheel")==0)
				wheel=1;
			if (strcmp(gameinfo.accessoriesReq[i],"balanceboard")==0)
				balanceboard=1;
			if (strcmp(gameinfo.accessoriesReq[i],"microphone")==0)
				microphone=1;
			if (strcmp(gameinfo.accessoriesReq[i],"gamecube")==0)
				gamecube=1;
		}
		
		// switch icons
		if (nunchuk) nunchukImgData = new GuiImageData(nunchukR_png);
		else nunchukImgData = new GuiImageData(nunchuk_png);
			
		if (classiccontroller) classiccontrollerImgData = new GuiImageData(classiccontrollerR_png);
		else classiccontrollerImgData = new GuiImageData(classiccontroller_png);
		
		if (guitar) guitarImgData = new GuiImageData(guitarR_png);
		else guitarImgData = new GuiImageData(guitar_png);
		
		if (gamecube) gamecubeImgData = new GuiImageData(gcncontrollerR_png);
		else gamecubeImgData = new GuiImageData(gcncontroller_png);
		
		if (wheel) wheelImgData = new GuiImageData(wheelR_png);
		else wheelImgData = new GuiImageData(wheel_png);
		
		if (motionplus) motionplusImgData = new GuiImageData(motionplusR_png);
		else motionplusImgData = new GuiImageData(motionplus_png);
		
		if (drums) drumsImgData = new GuiImageData(drumsR_png);
		else drumsImgData = new GuiImageData(drums_png);
		
		if (microphone) microphoneImgData = new GuiImageData(microphoneR_png);
		else microphoneImgData = new GuiImageData(microphone_png);
		
		if (balanceboard) balanceboardImgData = new GuiImageData(balanceboardR_png);
		else balanceboardImgData = new GuiImageData(balanceboard_png);
		
		if (dancepad) dancepadImgData = new GuiImageData(dancepadR_png);
		else dancepadImgData = new GuiImageData(dancepad_png);

		// look for optional accessories
		for (int i=1;i<=XML_ELEMMAX;i++)
		{
			if (strcmp(gameinfo.accessories[i],"classiccontroller")==0)
				classiccontroller=1;
			if (strcmp(gameinfo.accessories[i],"nunchuk")==0)
				nunchuk=1;
			if (strcmp(gameinfo.accessories[i],"guitar")==0)
				guitar=1;
			if (strcmp(gameinfo.accessories[i],"drums")==0)
				drums=1;
			if (strcmp(gameinfo.accessories[i],"dancepad")==0)
				dancepad=1;
			if (strcmp(gameinfo.accessories[i],"motionplus")==0)
				motionplus=1;
			if (strcmp(gameinfo.accessories[i],"wheel")==0)
				wheel=1;
			if (strcmp(gameinfo.accessories[i],"balanceboard")==0)
				balanceboard=1;
			if (strcmp(gameinfo.accessories[i],"microphone")==0)
				microphone=1;
			if (strcmp(gameinfo.accessories[i],"gamecube")==0)
				gamecube=1;
		}

		dialogBoxImg1 = new GuiImage(&dialogBox1);
		dialogBoxImg1->SetAlignment(0,3);
		dialogBoxImg1->SetPosition(-9,0);

		dialogBoxImg2 = new GuiImage(&dialogBox2);
		dialogBoxImg2->SetAlignment(0,3);
		dialogBoxImg2->SetPosition(145,0);

		dialogBoxImg3 = new GuiImage(&dialogBox3);
		dialogBoxImg3->SetAlignment(0,3);
		dialogBoxImg3->SetPosition(301,0);

		dialogBoxImg4 = new GuiImage(&dialogBox4);
		dialogBoxImg4->SetAlignment(0,3);
		dialogBoxImg4->SetPosition(457,0);

		gameinfoWindow.Append(dialogBoxImg1);
		gameinfoWindow.Append(dialogBoxImg2);
		gameinfoWindow.Append(dialogBoxImg3);
		gameinfoWindow.Append(dialogBoxImg4);

		snprintf(imgPath, sizeof(imgPath), "%s%s.png", Settings.covers_path, ID);
		cover = new GuiImageData(imgPath, 0); //load full id image
		if (!cover->GetImage()) {
			delete cover;
			snprintf(imgPath, sizeof(imgPath), "%snoimage.png", Settings.covers_path);
			cover = new GuiImageData(imgPath, nocover_png); //load no image
		}
		delete coverImg;
		coverImg = NULL;

		coverImg = new GuiImage(cover);
		coverImg->SetWidescreen(CFG.widescreen);
		coverImg->SetPosition(15,30);
		gameinfoWindow.Append(coverImg);

		// # of players
		if (strcmp(gameinfo.players,"") != 0) {
			playersImgData = new GuiImageData(Wiimote1_png);
			if (atoi(gameinfo.players)>1){
				playersImgData= new GuiImageData(Wiimote2_png);
			}
			if (atoi(gameinfo.players)>2){
				playersImgData= new GuiImageData(Wiimote4_png);
			}

			playersImg = new GuiImage(playersImgData);
			playersImg->SetWidescreen(CFG.widescreen);
			playersImg->SetPosition(intputX , inputY);
			playersImg->SetAlignment(0,4);
			gameinfoWindow.Append(playersImg);
			intputX += (CFG.widescreen ? playersImg->GetWidth() * .8 : playersImg->GetWidth())+5;
		}

		//draw the input types for this game
		if (motionplus==1){
			motionplusImg = new GuiImage(motionplusImgData);
			motionplusImg->SetWidescreen(CFG.widescreen);
			motionplusImg->SetPosition(intputX , inputY);
			motionplusImg->SetAlignment(0,4);
			gameinfoWindow.Append(motionplusImg);
			intputX += (CFG.widescreen ? motionplusImg->GetWidth() * .8 : motionplusImg->GetWidth())+5;
		}
		if (nunchuk==1){
			nunchukImg = new GuiImage(nunchukImgData);
			nunchukImg->SetWidescreen(CFG.widescreen);
			nunchukImg->SetPosition(intputX , inputY);
			nunchukImg->SetAlignment(0,4);
			gameinfoWindow.Append(nunchukImg);
			intputX += (CFG.widescreen ? nunchukImg->GetWidth() * .8 : nunchukImg->GetWidth())+5;
		}
		if (classiccontroller==1){
			classiccontrollerImg = new GuiImage(classiccontrollerImgData);
			classiccontrollerImg->SetWidescreen(CFG.widescreen);
			classiccontrollerImg->SetPosition(intputX , inputY);
			classiccontrollerImg->SetAlignment(0,4);
			gameinfoWindow.Append(classiccontrollerImg);
			intputX += (CFG.widescreen ? classiccontrollerImg->GetWidth() * .8 : classiccontrollerImg->GetWidth())+5;
		}
		if (gamecube==1){
			gcImg = new GuiImage(gamecubeImgData);
			gcImg->SetWidescreen(CFG.widescreen);
			gcImg->SetPosition(intputX , inputY);
			gcImg->SetAlignment(0,4);
			gameinfoWindow.Append(gcImg);
			intputX += (CFG.widescreen ? gcImg->GetWidth() * .8 : gcImg->GetWidth())+5;
		}
		if (wheel==1){
			wheelImg = new GuiImage(wheelImgData);
			wheelImg->SetWidescreen(CFG.widescreen);
			wheelImg->SetPosition(intputX , inputY);
			wheelImg->SetAlignment(0,4);
			gameinfoWindow.Append(wheelImg);
			intputX += (CFG.widescreen ? wheelImg->GetWidth() * .8 : wheelImg->GetWidth())+5;
		}
		if (guitar==1){
			guitarImg = new GuiImage(guitarImgData);
			guitarImg->SetWidescreen(CFG.widescreen);
			guitarImg->SetPosition(intputX , inputY);
			guitarImg->SetAlignment(0,4);
			gameinfoWindow.Append(guitarImg);
			intputX += (CFG.widescreen ? guitarImg->GetWidth() * .8 : guitarImg->GetWidth())+5;
		}
		if (drums==1){
			drumsImg = new GuiImage(drumsImgData);
			drumsImg->SetWidescreen(CFG.widescreen);
			drumsImg->SetPosition(intputX , inputY);
			drumsImg->SetAlignment(0,4);
			gameinfoWindow.Append(drumsImg);
			intputX += (CFG.widescreen ? drumsImg->GetWidth() * .8 : drumsImg->GetWidth())+5;
		}
		if (microphone==1){
			microphoneImg = new GuiImage(microphoneImgData);
			microphoneImg->SetWidescreen(CFG.widescreen);
			microphoneImg->SetPosition(intputX , inputY);
			microphoneImg->SetAlignment(0,4);
			gameinfoWindow.Append(microphoneImg);
			intputX += (CFG.widescreen ? microphoneImg->GetWidth() * .8 : microphoneImg->GetWidth())+5;
		}
		if (dancepad==1){
			dancepadImg = new GuiImage(dancepadImgData);
			dancepadImg->SetWidescreen(CFG.widescreen);
			dancepadImg->SetPosition(intputX , inputY);
			dancepadImg->SetAlignment(0,4);
			gameinfoWindow.Append(dancepadImg);
			intputX += (CFG.widescreen ? dancepadImg->GetWidth() * .8 : dancepadImg->GetWidth())+5;
		}
		if (balanceboard==1){
			balanceboardImg = new GuiImage(balanceboardImgData);
			balanceboardImg->SetWidescreen(CFG.widescreen);
			balanceboardImg->SetPosition(intputX , inputY);
			balanceboardImg->SetAlignment(0,4);
			gameinfoWindow.Append(balanceboardImg);
			intputX += (CFG.widescreen ? balanceboardImg->GetWidth() * .8 : balanceboardImg->GetWidth())+5;
		}

		// # online players
		if ((strcmp(gameinfo.wifiplayers,"") != 0) && (strcmp(gameinfo.wifiplayers,"0") != 0)){
			wifiplayersImgData = new GuiImageData(wifi1_png);
			if (atoi(gameinfo.wifiplayers)>1){
				wifiplayersImgData= new GuiImageData(wifi2_png);}
			if (atoi(gameinfo.wifiplayers)>2){
				wifiplayersImgData= new GuiImageData(wifi4_png);}
			if (atoi(gameinfo.wifiplayers)>4){
				wifiplayersImgData= new GuiImageData(wifi8_png);}
			/*if (atoi(gameinfo.wifiplayers)>8){       uncomment this when we actually have these images and it is needed
				wifiplayersImgData= new GuiImageData(wifi12_png);}
			if (atoi(gameinfo.wifiplayers)>12){
				wifiplayersImgData= new GuiImageData(wifi16_png);}
			if (atoi(gameinfo.wifiplayers)>16){
				wifiplayersImgData= new GuiImageData(wifi32_png);}*/
			wifiplayersImg = new GuiImage(wifiplayersImgData);
			wifiplayersImg->SetWidescreen(CFG.widescreen);
			wifiplayersImg->SetPosition(intputX , inputY);
			wifiplayersImg->SetAlignment(0,4);
			gameinfoWindow.Append(wifiplayersImg);
			intputX += (CFG.widescreen ? wifiplayersImg->GetWidth() * .8 : wifiplayersImg->GetWidth())+5;
		}

		// ratings
		if (strcmp(gameinfo.ratingtype,"") !=0){
		if (strcmp(gameinfo.ratingtype,"ESRB")==0) {
			if (strcmp(gameinfo.ratingvalueESRB,"EC")==0)
				ratingImgData = new GuiImageData(esrb_ec_png);
			else if (strcmp(gameinfo.ratingvalueESRB,"E")==0)
				ratingImgData = new GuiImageData(esrb_e_png);
			else if (strcmp(gameinfo.ratingvalueESRB,"E10+")==0)
				ratingImgData = new GuiImageData(esrb_eten_png);
			else if (strcmp(gameinfo.ratingvalueESRB,"T")==0)
				ratingImgData = new GuiImageData(esrb_t_png);
			else if (strcmp(gameinfo.ratingvalueESRB,"M")==0)
				ratingImgData = new GuiImageData(esrb_m_png);
			else if (strcmp(gameinfo.ratingvalueESRB,"AO")==0)
				ratingImgData = new GuiImageData(esrb_ao_png);
			else {ratingImgData = new GuiImageData(norating_png);}
			}											//there are 2 values here cause some countries are stupid and
		else if (strcmp(gameinfo.ratingtype,"PEGI")==0) {//can't use the same as everybody else
			if ((strcmp(gameinfo.ratingvaluePEGI,"3")==0)||(strcmp(gameinfo.ratingvaluePEGI,"4")==0))
				ratingImgData = new GuiImageData(pegi_3_png);
			else if ((strcmp(gameinfo.ratingvaluePEGI,"7")==0)||(strcmp(gameinfo.ratingvaluePEGI,"7")==0))
				ratingImgData = new GuiImageData(pegi_7_png);
			else if (strcmp(gameinfo.ratingvaluePEGI,"12")==0)
				ratingImgData = new GuiImageData(pegi_12_png);
			else if ((strcmp(gameinfo.ratingvaluePEGI,"16")==0)||(strcmp(gameinfo.ratingvaluePEGI,"15")==0))
				ratingImgData = new GuiImageData(pegi_16_png);
			else if (strcmp(gameinfo.ratingvaluePEGI,"18")==0)
				ratingImgData = new GuiImageData(pegi_18_png);
			else {ratingImgData = new GuiImageData(norating_png);}
			}
		else if (strcmp(gameinfo.ratingtype,"CERO")==0) {
			if (strcmp(gameinfo.ratingvalueCERO,"A")==0)
				ratingImgData = new GuiImageData(cero_a_png);
			else if (strcmp(gameinfo.ratingvalueCERO,"B")==0)
				ratingImgData = new GuiImageData(cero_b_png);
			else if (strcmp(gameinfo.ratingvalueCERO,"C")==0)
				ratingImgData = new GuiImageData(cero_c_png);
			else if (strcmp(gameinfo.ratingvalueCERO,"D")==0)
				ratingImgData = new GuiImageData(cero_d_png);
			else if (strcmp(gameinfo.ratingvalueCERO,"Z")==0)
				ratingImgData = new GuiImageData(cero_z_png);
			else {ratingImgData = new GuiImageData(norating_png);}
			}

		else {ratingImgData = new GuiImageData(norating_png);}
			ratingImg = new GuiImage(ratingImgData);
			ratingImg->SetWidescreen(CFG.widescreen);
			ratingImg->SetPosition(-25 , inputY);
			ratingImg->SetAlignment(1,4);
			gameinfoWindow.Append(ratingImg);
			intputX += (CFG.widescreen ? ratingImg->GetWidth() * .8 : ratingImg->GetWidth())+5;
		}

		// memory info
		if (showmeminfo) {
			char meminfotxt[200];
			strlcpy(meminfotxt,MemInfo(),sizeof(meminfotxt));
			snprintf(linebuf, sizeof(linebuf), "%s",meminfotxt);
			memTxt = new GuiText(linebuf, 18, (GXColor){0,0,0, 255});
			memTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP); memTxt->SetPosition(0,0);
			gameinfoWindow.Append(memTxt);
		}	
			
		// title
		int titlefontsize=25;
		if (strcmp(gameinfo.title,"") != 0) {
			snprintf(linebuf, sizeof(linebuf), "%s",gameinfo.title);
			titleTxt = new GuiText(linebuf, titlefontsize, (GXColor){0,0,0, 255});
			//while (titleTxt->GetWidth()>250) { titleTxt->SetFontSize(titlefontsize-=2); }
			titleTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP); titleTxt->SetPosition(txtXOffset,12+titley);
			gameinfoWindow.Append(titleTxt);
		}

		//publisher
		if (strcmp(gameinfo.publisher,"") != 0){
			snprintf(linebuf, sizeof(linebuf), "%s %s", tr("Published by"), gameinfo.publisher);
			publisherTxt = new GuiText(linebuf, 16, (GXColor){0,0,0, 255});
			if (publisherTxt->GetWidth()>250) newline=2;
			publisherTxt->SetMaxWidth(250,GuiText::WRAP);
			publisherTxt->SetAlignment(ALIGN_RIGHT, ALIGN_TOP); publisherTxt->SetPosition(-17,12+indexy);  indexy+=(20 * newline);newline=1;
			gameinfoWindow.Append(publisherTxt);
		}
		
		//developer
		if (strcmp(gameinfo.developer,"") != 0 && strcasecmp(gameinfo.developer,gameinfo.publisher) != 0)	{
			snprintf(linebuf, sizeof(linebuf), "%s %s", tr("Developed by"), gameinfo.developer);
			developerTxt = new GuiText(linebuf, 16, (GXColor){0,0,0, 255});
			if (developerTxt->GetWidth()>250) newline=2;
			developerTxt->SetMaxWidth(250,GuiText::WRAP);
			developerTxt->SetAlignment(ALIGN_RIGHT, ALIGN_TOP); developerTxt->SetPosition(-17,12+indexy); indexy+=(20 * newline);newline=1;
			gameinfoWindow.Append(developerTxt);
		}

		//date
		snprintf(linebuf2, sizeof(linebuf2), " ");
		if (strcmp(gameinfo.day,"") != 0)
		{snprintf(linebuf2, sizeof(linebuf2), "%s ", gameinfo.day);}
		if (strcmp(gameinfo.month,"") != 0){
			switch (atoi(gameinfo.month))
			{
				case 1:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Jan"));
					break;
				case 2:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Feb"));
					break;
				case 3:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Mar"));
					break;
				case 4:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Apr"));
					break;
				case 5:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("May"));
					break;
				case 6:
				   snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("June"));
					break;
				case 7:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("July"));
					break;
				case 8:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Aug"));
					break;
				case 9:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Sept"));
					break;
				case 10:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Oct"));
					break;
				case 11:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Nov"));
					break;
				case 12:
					snprintf(linebuf2, sizeof(linebuf2), "%s%s ", linebuf2, tr("Dec"));
					break;
			}
		}
		if (strcmp(gameinfo.year,"") != 0){
			snprintf(linebuf, sizeof(linebuf), "%s : %s%s", tr("Released"), linebuf2, gameinfo.year);
			releasedTxt = new GuiText(linebuf, 16, (GXColor){0,0,0, 255});
			if (releasedTxt->GetWidth()>300) newline=2;
			releasedTxt->SetAlignment(ALIGN_RIGHT, ALIGN_TOP); releasedTxt->SetPosition(-17,12+indexy);  indexy+=(20 * newline);newline=1;
			gameinfoWindow.Append(releasedTxt);
		}

		//genre
		int genreY = marginY;
		genreTxt = new GuiText * [gameinfo.genreCnt + 1];
		for (int i=1;i<=gameinfo.genreCnt;i++)
		{
			snprintf(linebuf, sizeof(linebuf), "%s", gameinfo.genresplit[i]);
			genreTxt[i] = new GuiText(linebuf, 16, (GXColor){0,0,0, 255});
			genreTxt[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP); genreTxt[i]->SetPosition(205,12+genreY); genreY+=20;
			gameinfoWindow.Append(genreTxt[i]);
		}
			
		//online
		wifiTxt = new GuiText * [gameinfo.wifiCnt + 1];
		for (int i=gameinfo.wifiCnt;i>=1;i--)
			{
				if (strcmp(gameinfo.wififeatures[i],"Nintendods") == 0) {
					snprintf(linebuf, sizeof(linebuf), "Nintendo DS");
				} else {
					snprintf(linebuf, sizeof(linebuf), "%s",gameinfo.wififeatures[i]);
				}
				wifiTxt[i] = new GuiText(linebuf, 16, (GXColor){0,0,0, 255});
				wifiTxt[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP); wifiTxt[i]->SetPosition(215,200+wifiY);  wifiY-=20;
				gameinfoWindow.Append(wifiTxt[i]);
			}
		if (strcmp(gameinfo.wififeatures[1],"") != 0){
			snprintf(linebuf, sizeof(linebuf), "%s:",tr("WiFi Features"));
			wifiTxt[0] = new GuiText(linebuf, 16, (GXColor){0,0,0, 255});
			wifiTxt[0]->SetAlignment(ALIGN_LEFT, ALIGN_TOP); wifiTxt[0]->SetPosition(205,200+wifiY);
			gameinfoWindow.Append(wifiTxt[0]);
		}
		
		//synopsis
		if (strcmp(gameinfo.synopsis,"") != 0)	{
			snprintf(linebuf, sizeof(linebuf), "%s", gameinfo.synopsis);
			synopsisTxt = new GuiText(linebuf, 16, (GXColor){0,0,0, 255});
			synopsisTxt->SetMaxWidth(350,GuiText::WRAP);
			synopsisTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP); synopsisTxt->SetPosition(0,0);
			dialogBoxImg11 = new GuiImage(&dialogBox1);
			dialogBoxImg11->SetAlignment(0,3);
			dialogBoxImg11->SetPosition(-9,0);

			dialogBoxImg22 = new GuiImage(&dialogBox2);
			dialogBoxImg22->SetAlignment(0,3);
			dialogBoxImg22->SetPosition(145,0);

			dialogBoxImg33 = new GuiImage(&dialogBox3);
			dialogBoxImg33->SetAlignment(0,3);
			dialogBoxImg33->SetPosition(301,0);

			dialogBoxImg44 = new GuiImage(&dialogBox4);
			dialogBoxImg44->SetAlignment(0,3);
			dialogBoxImg44->SetPosition(457,0);

			gameinfoWindow2.Append(dialogBoxImg11);
			gameinfoWindow2.Append(dialogBoxImg22);
			gameinfoWindow2.Append(dialogBoxImg33);
			gameinfoWindow2.Append(dialogBoxImg44);

			txtWindow.Append(synopsisTxt);
			coverImg2 = new GuiImage(cover);
			coverImg2->SetWidescreen(CFG.widescreen);
			coverImg2->SetPosition(15,30);
			gameinfoWindow2.Append(coverImg2);
			gameinfoWindow2.Append(&txtWindow);
		}
		
		//don't bother us txt
		snprintf(linebuf, sizeof(linebuf), "Don't bother the USB Loader GX Team about errors in this file.");
		betaTxt = new GuiText(linebuf, 14, (GXColor){0,0,0, 255});
		betaTxt->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM); betaTxt->SetPosition(-17,-20);
		gameinfoWindow.Append(betaTxt);
		snprintf(linebuf, sizeof(linebuf), "A site will be available in the near nuture to submit changes.");
		beta1Txt = new GuiText(linebuf, 14, (GXColor){0,0,0, 255});
		beta1Txt->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM); beta1Txt->SetPosition(-17,-10);
		gameinfoWindow.Append(beta1Txt);
		
		gameinfoWindow.SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_IN, 100);
		HaltGui();
		mainWindow->SetState(STATE_DISABLED);
		mainWindow->Append(&gameinfoWindow);
		mainWindow->ChangeFocus(&gameinfoWindow);
		ResumeGui();

		while(choice == -1)
		{
			VIDEO_WaitVSync();
			if(shutdown == 1)
			{
				wiilight(0);
				Sys_Shutdown();
			}
			if(reset == 1)
				Sys_Reboot();

			if ((backBtn.GetState()==STATE_CLICKED)||(backBtn.GetState()==STATE_HELD)){
				choice=1;
				synopsisTxt = NULL;
				break;
			}
			else if (((nextBtn.GetState()==STATE_CLICKED)||(nextBtn.GetState()==STATE_HELD))&&
			(strcmp(gameinfo.synopsis,"") != 0)){

				if (page==1){
					nextBtn.ResetState();
					HaltGui();
					gameinfoWindow.SetVisible(false);
					gameinfoWindow2.SetVisible(true);
					coverImg->SetPosition(15,30);

					backBtn.SetClickable(false);
					gameinfoWindow2.Append(&nextBtn);
					mainWindow->Append(&gameinfoWindow2);
					ResumeGui();
					page=2;
				}
				else {
					nextBtn.ResetState();
					HaltGui();
					backBtn.SetClickable(true);
					gameinfoWindow2.SetVisible(false);
					gameinfoWindow.SetVisible(true);
					gameinfoWindow.Append(&backBtn);
					gameinfoWindow.Append(&nextBtn);
					mainWindow->Remove(&gameinfoWindow2);
					ResumeGui();
					page=1;
				}
				nextBtn.ResetState();
			}
		}
		if (page==1){
			gameinfoWindow.SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 100);
			while(gameinfoWindow.GetEffect() > 0) usleep(50);
			HaltGui();
			mainWindow->Remove(&gameinfoWindow);
			mainWindow->SetState(STATE_DEFAULT);

			delete playersImgData;
			delete playersImg;

			delete wifiplayersImgData;
			delete wifiplayersImg;
			delete ratingImg;

			delete classiccontrollerImg;
			delete nunchukImg;
			delete guitarImg;
			delete drumsImg;
			delete dancepadImg;
			delete motionplusImg;
			delete wheelImg;
			delete balanceboardImg;
			delete microphoneImg;
			delete gcImg;
			delete dialogBoxImg1;
			delete dialogBoxImg2;
			delete dialogBoxImg3;
			delete dialogBoxImg4;
			delete dialogBoxImg11;
			delete dialogBoxImg22;
			delete dialogBoxImg33;
			delete dialogBoxImg44;
			delete coverImg;
			delete coverImg2;

			delete classiccontrollerImgData;
			delete nunchukImgData;
			delete guitarImgData;
			delete drumsImgData;
			delete motionplusImgData;
			delete wheelImgData;
			delete balanceboardImgData;
			delete dancepadImgData;
			delete microphoneImgData;
			delete gamecubeImgData;
			delete ratingImgData;
			delete cover;
			delete releasedTxt;
			delete publisherTxt;
			delete developerTxt;
			delete titleTxt;
			delete synopsisTxt;
			delete betaTxt;
			delete beta1Txt;
			delete memTxt;
			if (gameinfo.genreCnt>0){
				for(int i=1;i<=gameinfo.genreCnt;i++)
				{
					delete genreTxt[i];
				}
			}
			if (gameinfo.wifiCnt>0){
				for(int i=0;i<=gameinfo.wifiCnt;i++)
				{
					delete wifiTxt[i];
				}
			}
			ResumeGui();
		} else {
			gameinfoWindow2.SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 100);
			while(gameinfoWindow2.GetEffect() > 0) usleep(50);
			HaltGui();
			mainWindow->Remove(&gameinfoWindow2);
			mainWindow->SetState(STATE_DEFAULT);
			ResumeGui();
		}
		return choice;

    /* File not found */
    } else {
        return -1;
    }
}
