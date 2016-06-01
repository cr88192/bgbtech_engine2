#include <bteifgl.h>

BGBDT_SndMixChan *bgbdt_snd_mixchan=NULL;
BGBDT_SndMixChan *bgbdt_snd_freechan=NULL;
BGBDT_SndListen *bgbdt_snd_listen=NULL;
double bgbdt_snd_abstime;

u32 bgbdt_snd_chanmask[128];
int bgbdt_snd_chanrov=1;

BTEIFGL_API int BGBDT_Sound_AllocChanId(void)
{
	int i, n;
	
	n=128;
	while(n--)
	{
		i=bgbdt_snd_chanrov;
		bgbdt_snd_chanrov=(i+1)&4095;
		if(!i)continue;
		if(!(bgbdt_snd_chanmask[i>>5]&(1U<<(i&31))))
		{
			bgbdt_snd_chanmask[i>>5]|=(1U<<(i&31));
			return(i);
		}
	}
	return(-1);
}

BTEIFGL_API int BGBDT_Sound_FreeChanId(int id)
{
	bgbdt_snd_chanmask[id>>5]&=~(1U<<(id&31));
	return(0);
}

BTEIFGL_API BGBDT_SndMixChan *BGBDT_Sound_AllocMixChan(void)
{
	BGBDT_SndMixChan *cur;
	
	cur=bgbdt_snd_freechan;
	if(cur)
	{
		bgbdt_snd_freechan=cur->next;
		memset(cur, 0, sizeof(BGBDT_SndMixChan));
		return(cur);
	}

	cur=dtmAlloc("bgbdt_sndmixchan_t", sizeof(BGBDT_SndMixChan));
	return(cur);
}

BTEIFGL_API int BGBDT_Sound_FreeMixChan(BGBDT_SndMixChan *cur)
{
	cur->next=bgbdt_snd_freechan;
	bgbdt_snd_freechan=cur;
	return(0);
}

BTEIFGL_API BGBDT_SndMixChan *BGBDT_Sound_LookupMixChan(int id)
{
	BGBDT_SndMixChan *cur;
	
	cur=bgbdt_snd_mixchan;
	while(cur)
	{
		if(cur->id==id)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API BGBDT_SndMixChan *BGBDT_Sound_GetMixChan(int id)
{
	BGBDT_SndMixChan *cur;
	
	cur=BGBDT_Sound_LookupMixChan(id);
	if(cur)
		return(cur);
	
//	cur=dtmAlloc("bgbdt_sndmixchan_t", sizeof(BGBDT_SndMixChan));
//	cur->id=id;

	cur=BGBDT_Sound_AllocMixChan();
	cur->id=id;

	cur->next=bgbdt_snd_mixchan;
	bgbdt_snd_mixchan=cur;
	return(cur);
}

BTEIFGL_API int BGBDT_Sound_DeleteMixChan(int id)
{
	BGBDT_SndMixChan *cur, *lst;
	
	cur=bgbdt_snd_mixchan; lst=NULL;
	while(cur)
	{
		if(cur->id==id)
		{
			if(lst)
				{ lst->next=cur->next; }
			else
				{ bgbdt_snd_mixchan=cur->next; }
			BGBDT_Sound_FreeMixChan(cur);
			return(1);
		}
		lst=cur;
		cur=cur->next;
	}
	return(0);
}


BTEIFGL_API BGBDT_SndListen *BGBDT_Sound_LookupListener(int id)
{
	BGBDT_SndListen *cur;
	
	cur=bgbdt_snd_listen;
	while(cur)
	{
		if(cur->id==id)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API BGBDT_SndListen *BGBDT_Sound_GetListener(int id)
{
	BGBDT_SndListen *cur;
	
	cur=BGBDT_Sound_LookupListener(id);
	if(cur)
		return(cur);
	
	cur=dtmAlloc("bgbdt_sndlisten_t", sizeof(BGBDT_SndListen));
	cur->id=id;

	cur->next=bgbdt_snd_listen;
	bgbdt_snd_listen=cur;
	return(cur);
}


BTEIFGL_API int BGBDT_Sound_MixChannel(
	BGBDT_SndListen *listen, BGBDT_SndMixChan *chan, double dt)
{
	vec3 dir, rvel;
	double t0, d0, ts, hz;
	float dist, dx, dy, dz, dlr, dvel, dv0, dv1;
	float f, g;
	int nsmps, fhz, sofs, att;
	int i, j, k, l;
	
	if(chan->donemsk&(1<<listen->id))
		{ return(0); }
	
	if(!chan->samp)
	{
		chan->donemsk|=1<<listen->id;
		return(0);
	}

	nsmps=dt*listen->rate;
	
	t0=listen->ctime-chan->stime;
	hz=listen->rate; fhz=hz*16;
//	att=256;
	att=chan->vol;
	d0=0;
	
	if(!(chan->flag&BGBDT_SNDFL_LOOP))
	{
		i=t0*chan->samp->sbrate;
		if(i>chan->samp->len)
		{
			chan->donemsk|=1<<listen->id;
			return(0);
		}
	}
	
	if((chan->att==BGBDT_SNDATT_LINEAR) ||
		(chan->att==BGBDT_SNDATT_SQRT) ||
		(chan->att==BGBDT_SNDATT_POW075))
	{
		dir=v3sub(chan->org, listen->org);
		dist=v3len(dir);

		if(chan->att==BGBDT_SNDATT_LINEAR)
			att=att/(dist+1)+0.5;
		else if(chan->att==BGBDT_SNDATT_SQRT)
			att=att/sqrt(dist+1)+0.5;
		else if(chan->att==BGBDT_SNDATT_POW075)
			att=att/pow(dist+1, 0.75)+0.5;
		
		dx=v3dot(dir, listen->vld);
		dy=v3dot(dir, listen->vfw);
		dz=v3dot(dir, listen->vup);
		
		f=1.0/sqrt(dx*dx+dy*dy+dz*dz+1);
		dx=dx*f; dy=dy*f; dz=dz*f;
		
		if(dy>0)
			{ dlr=dx/(dy+1); }
		else
			{ dlr=dx/(4*(-dy+1)); }
		dlr=dx/(fabs(dz)+1);
		
		att=att*(1.0+0.5*dlr);
		
//		d0=(dist/64.0)/771.0;
		d0=dist/(64.0*771.0);

//		dvel=v3dot(chan->vel, listen->vel);
//		d0=dvel/(64.0*771.0);

		dv0=v3dot(listen->vel, dir);
		dv1=v3dot(chan->vel, dir);
		f=(64.0*771.0);
		dvel=((f+dv0)/(f-dv1));

		fhz=fhz*(1.0/dvel);
//		fhz=fhz*dvel;

//		rvel=v3sub(chan->vel, listen->vel);
	}
	
	if(att<1)
	{
		return(0);
	}
	
	for(i=0; i<(2*nsmps); i++)
	{
//		sofs=(t0*hz+i)*16;
//		sofs=((t0-d0)*hz+i)*16;
		sofs=((t0-d0)*fhz)+(i*(fhz/hz));

		if(chan->flag&BGBDT_SNDFL_LOOP)
		{
			j=chan->samp->GetSampleMonoFqMod(chan->samp, fhz, sofs);
//			j=chan->samp->GetSampleMonoMod(chan->samp, sofs>>4);
		}else
		{
			j=chan->samp->GetSampleMonoFqClz(chan->samp, fhz, sofs);
//			j=chan->samp->GetSampleMono(chan->samp, sofs>>4);
		}
		listen->mixbuf[i]+=j*att;
	}
	return(1);
}

BTEIFGL_API int BGBDT_Sound_CheckListenBuffer(
	BGBDT_SndListen *listen, double dt)
{
	int i, j, k, l;

	if(!listen->mixbuf)
	{
		k=2*dt*listen->rate;
		l=2*0.25*listen->rate;
		if(k>l)l=k;
		listen->mixbuf=frgl_malloc(l*sizeof(s32));
		listen->szmixbuf=l;
	}else
	{
		k=2*dt*listen->rate;
		if(k>listen->szmixbuf)
		{
			frgl_free(listen->mixbuf);
			listen->mixbuf=frgl_malloc(k*sizeof(s32));
			listen->szmixbuf=k;
		}
		
		for(i=0; i<k; i++)
		{
			listen->mixbuf[i]=0;
		}
	}
	return(0);
}


BTEIFGL_API int BGBDT_Sound_SetListenOrg(vec3 org)
{
	BGBDT_SndListen *mixl, *mixr;

	mixl=BGBDT_Sound_LookupListener(0);
	mixr=BGBDT_Sound_LookupListener(1);
	if(!mixl || !mixr)
		return(-1);
	
	mixl->lorg=mixl->org;
	mixr->lorg=mixr->org;
	mixl->lotime=mixl->otime;
	mixr->lotime=mixr->otime;
	mixl->org=org;
	mixr->org=org;
//	mixl->otime=mixl->ctime;
//	mixr->otime=mixr->ctime;
	mixl->otime=bgbdt_snd_abstime;
	mixr->otime=bgbdt_snd_abstime;
	
	mixl->vel=v3scale(v3sub(mixl->org, mixl->lorg),
		1.0/(mixl->otime-mixl->lotime+0.00001));
	mixr->vel=v3scale(v3sub(mixr->org, mixr->lorg),
		1.0/(mixr->otime-mixr->lotime+0.00001));
	return(0);
}

BTEIFGL_API int BGBDT_Sound_SetListenVecRt(vec3 v)
{
	BGBDT_SndListen *mixl, *mixr;

	mixl=BGBDT_Sound_LookupListener(0);
	mixr=BGBDT_Sound_LookupListener(1);
	if(!mixl || !mixr)
		return(-1);
	mixl->vrt=v;
	mixr->vrt=v;
	mixl->vld=v3scale(v, -1.0);
	mixr->vld=v3scale(v,  1.0);
	return(0);
}

BTEIFGL_API int BGBDT_Sound_SetListenVecFw(vec3 v)
{
	BGBDT_SndListen *mixl, *mixr;

	mixl=BGBDT_Sound_LookupListener(0);
	mixr=BGBDT_Sound_LookupListener(1);
	if(!mixl || !mixr)
		return(-1);
	mixl->vfw=v;
	mixr->vfw=v;
	return(0);
}

BTEIFGL_API int BGBDT_Sound_SetListenVecUp(vec3 v)
{
	BGBDT_SndListen *mixl, *mixr;

	mixl=BGBDT_Sound_LookupListener(0);
	mixr=BGBDT_Sound_LookupListener(1);
	if(!mixl || !mixr)
		return(-1);
	mixl->vup=v;
	mixr->vup=v;
	return(0);
}

BTEIFGL_API int BGBDT_Sound_MixTime(double dt)
{
	static s16 sbuf[1<<16];
	static double accDt;

	BGBDT_SndMixChan *cur, *nxt, *lst;
	BGBDT_SndListen *mixl, *mixr;
	double dt1;
	int i, j, k, l;
	
	mixl=BGBDT_Sound_LookupListener(0);
	mixr=BGBDT_Sound_LookupListener(1);
	if(!mixl || !mixr)
		return(-1);
	
	if(dt>0.25)
		return(-1);
	
	bgbdt_snd_abstime+=dt;
	
	accDt+=dt;
	if(accDt<(1.0/32))
		return(0);
	
	dt1=0;
	if(accDt>=(1.0/32))
	{
		dt1+=1.0/32;
		accDt-=(1.0/32);
	}

	BGBDT_Sound_CheckListenBuffer(mixl, dt1);
	BGBDT_Sound_CheckListenBuffer(mixr, dt1);

	mixl->ctime+=dt1;
	mixr->ctime+=dt1;
	
	cur=bgbdt_snd_mixchan; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		BGBDT_Sound_MixChannel(mixl, cur, dt1);
		BGBDT_Sound_MixChannel(mixr, cur, dt1);
		
		if(((cur->donemsk&3)==3) && 
			!(cur->flag&BGBDT_SNDFL_KEEPDONE))
		{
			if(lst)
				{ lst->next=nxt; }
			else
				{ bgbdt_snd_mixchan=nxt; }
			BGBDT_Sound_FreeChanId(cur->id);
			BGBDT_Sound_FreeMixChan(cur);
			cur=nxt;
			continue;
		}
		
		lst=cur;
		cur=nxt;
	}

	l=2*dt1*mixl->rate;
	for(i=0; i<l; i++)
	{
		j=mixl->mixbuf[i];
		j=(j+127)>>8;
		j=(j>32767)?32767:((j<(-32767))?(-32767):j);
		sbuf[i*2+0]=j;

		j=mixr->mixbuf[i];
		j=(j+127)>>8;
		j=(j>32767)?32767:((j<(-32767))?(-32767):j);
		sbuf[i*2+1]=j;
	}
	
	k=dt1*mixl->rate;
//	SoundDev_WriteSamples(sbuf, k);
	SoundDev_WriteStereoSamples(sbuf, k);
	
	return(0);
}

BTEIFGL_API int BGBDT_Sound_PlaySound(
	char *name, int vol, int att, int flag)
{
	BGBDT_SndMixChan *mixchn;
	BGBDT_SndSampler *mixsmp;
	int id;
	
	mixsmp=BGBDT_Snd_GetLoadSampler(name);

	id=BGBDT_Sound_AllocChanId();
	mixchn=BGBDT_Sound_GetMixChan(id);
	mixchn->samp=mixsmp;
	mixchn->vol=vol;
	mixchn->att=att;
	mixchn->flag=flag;
	mixchn->stime=bgbdt_snd_abstime;
	return(id);
}

BTEIFGL_API int BGBDT_Sound_ChanSetOrigin(int id, vec3 org)
{
	BGBDT_SndMixChan *mixc;

	mixc=BGBDT_Sound_LookupMixChan(id);
	if(!mixc)
		return(-1);
	
	mixc->lorg=mixc->org;
	mixc->lotime=mixc->otime;
	mixc->org=org;
	mixc->otime=bgbdt_snd_abstime;

	mixc->vel=v3scale(v3sub(mixc->org, mixc->lorg),
		1.0/(mixc->otime-mixc->lotime+0.00001));
	return(0);
}

BTEIFGL_API int BGBDT_Sound_ChanSetOriginF(
	int id, float x, float y, float z)
{
	return(BGBDT_Sound_ChanSetOrigin(id, vec3(x, y, z)));
}
