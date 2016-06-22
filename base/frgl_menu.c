#include <bteifgl.h>

FRGL_MenuInfo *frgl_menu_root;

FRGL_MenuInfo *frgl_menu_active;
int frgl_menu_activepos;

BTEIFGL_API FRGL_MenuInfo *FRGL_LookupMenu(char *name)
{
	FRGL_MenuInfo *cur;
	
	cur=frgl_menu_root;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API FRGL_MenuInfo *FRGL_GetMenu(char *name)
{
	FRGL_MenuInfo *tmp;

	tmp=FRGL_LookupMenu(name);
	if(tmp)return(tmp);
	
	tmp=dtmAlloc("frgl_menuinfo_t", sizeof(FRGL_MenuInfo));
	tmp->name=frgl_strdup(name);
	
	tmp->next=frgl_menu_root;
	frgl_menu_root=tmp;
	return(tmp);
}

BTEIFGL_API FRGL_MenuItem *FRGL_GetMenuItem(
	FRGL_MenuInfo *menu, int idx, char *name)
{
	FRGL_MenuItem *itm;
	
	itm=menu->item[idx];
	if(itm)
	{
		if(strcmp(itm->name, name))
			return(NULL);
		return(itm);
	}
	
	itm=dtmAlloc("frgl_menuitem_t", sizeof(FRGL_MenuItem));
	itm->name=frgl_strdup(name);
	
	if((idx+1)>menu->n_item)
		menu->n_item=idx+1;
	
	menu->item[idx]=itm;
	return(itm);
}

BTEIFGL_API int FRGL_MenuActiveP(void)
{
	if(frgl_menu_active)
		return(1);
	return(0);
}

BTEIFGL_API int FRGL_SetMenuActive(char *name)
{
	FRGL_MenuInfo *menu;

	if(!name)
	{
		frgl_menu_active=NULL;
		frgl_menu_activepos=0;
		return(1);
	}

	menu=FRGL_LookupMenu(name);
	if(menu)
	{
		frgl_menu_active=menu;
		frgl_menu_activepos=0;
		return(1);
	}
	
	return(0);
}

BTEIFGL_API int FRGL_DrawMenu(void)
{
	FRGL_MenuInfo *menu;
	FRGL_MenuItem *mitm;
	char *s;
	int w, n;
	int i, j, k, l;

	menu=frgl_menu_active;
	if(!menu)
		return(0);
	
	if(menu->title)
		{ s=menu->title; }
	else
		{ s=menu->name; }
	w=strlen(s);

	for(i=0; i<menu->n_item; i++)
	{
		if(!menu->item[i])
			continue;
		if(menu->item[i]->title)
			{ s=menu->item[i]->title; }
		else
			{ s=menu->item[i]->name; }
		l=strlen(s);
		if(l>w)w=l;
	}
	
	n=menu->n_item+1;
	
	if(menu->title)
		{ s=menu->title; }
	else
		{ s=menu->name; }
	l=strlen(s);
	GfxFont_DrawString(s, -(l/2)*24, ((n/2)+2)*16, 24, 24, 0, 255, 0, 255);

	for(i=0; i<menu->n_item; i++)
	{
		mitm=menu->item[i];
		if(!mitm)
			continue;
		if(mitm->title)
			{ s=mitm->title; }
		else
			{ s=mitm->name; }
		l=strlen(s);

		GfxFont_DrawString(s,
			-(w/2)*16, ((n/2)-i)*16,
			16, 16, 0, 255, 0, 255);
		if(mitm->opt_str)
		{
			GfxFont_DrawString(mitm->opt_str,
				((w/2)+2)*16, ((n/2)-i)*16,
				16, 16, 0, 255, 0, 255);
		}
		if(frgl_menu_activepos==i)
		{
			GfxFont_DrawString("=>",
				-((w/2)+3)*16, ((n/2)-i)*16,
				16, 16, 0, 255, 0, 255);
		}
	}

	return(1);
}

BTEIFGL_API int FRGL_MenuHandleKeys(u16 *keys)
{
	FRGL_MenuInfo *menu, *menu2;
	FRGL_MenuItem *mitm;
	char *s;
	u16 *kcur;
	int w, n;
	int i, j, k, l;

	menu=frgl_menu_active;
	if(!menu)
		return(0);
	
	kcur=keys;
	while(*kcur)
	{
		switch(*kcur)
		{
		case K_DEL:
		case K_ESC:
			BGBDT_Sound_PlaySound("sound/hitwood0", 128, 0, 0);
//			FRGL_SetMenuActive(menu->back);
			FRGL_SetMenuActive(NULL);
//			isotest_diagbox=DTV_NULL;
			break;		
		case K_ENTER:
			BGBDT_Sound_PlaySound("sound/hitwood0", 128, 0, 0);
			mitm=menu->item[frgl_menu_activepos];
			
			if(mitm->Action)
			{
				mitm->Action(menu, mitm);
				break;
			}
			
			menu2=FRGL_LookupMenu(mitm->name);
			if(menu2)
			{
				frgl_menu_active=menu2;
				frgl_menu_activepos=0;
				break;
			}
//			hituse=1;
			break;
		case '0':	case '1':
		case '2':	case '3':
		case '4':	case '5':
		case '6':	case '7':
		case '8':	case '9':
			frgl_menu_activepos=(*kcur-'0');
//			hituse=1+(*kcur-'0');
			break;
		case K_UPARROW:
			frgl_menu_activepos--;
			if(frgl_menu_activepos<0)
				frgl_menu_activepos=menu->n_item-1;
			BGBDT_Sound_PlaySound("sound/hitwood0", 128, 0, 0);
			break;
		case K_DOWNARROW:
			frgl_menu_activepos++;
			if(frgl_menu_activepos>=menu->n_item)
				frgl_menu_activepos=0;
			BGBDT_Sound_PlaySound("sound/hitwood0", 128, 0, 0);
			break;

		default:
			break;
		}
		kcur++;
	}

	return(0);
}
