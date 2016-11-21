/**
 * Skel Info
 *
 * layout
 *   skeleton
 *   [attached features]
 *   [animations]
 *
 * skeleton:
 * origin x y z
 *   relative origin of skeleton (subtracted from coords)
 * iscale x
 *   inverse scale of bones (maps from internal units to meters)
 *   default value will be 1000 (millimeter units)
 * bone boneid parentid name ( x y z )
 *   declare a bone, valid bone ids start at 1.
 * endbones
 *   end of skeleton
 *
 * animations:
 * anim id name frames
 *   begin an animation sequence
 * frame framenum
 *   begin a poses for a frame
 * pose tags*
 *   set pose for one or more bones.
 *   B#, set the current bone (to set pose for)
 *   X#/Y#/Z#, set the origin of the bone
 *     given here as absolute coords
 *   P#/Q#/R#, set angles for the bone
 *     rotations will be applied in the order they are given.
 *     P/Q/R will map to X/Y/Z rotation.
 * endanim
 *   end current animation sequence
 */

BGBDT_SkelInfo *Bt2Skel_AllocSkelInfo()
{
	BGBDT_SkelInfo *tmp;
	tmp=dtmAlloc("bgbdt_skelinfo_t", sizeof(BGBDT_SkelInfo));
	return(tmp);
}

BGBDT_SkelBone *Bt2Skel_AllocSkelBone()
{
	BGBDT_SkelBone *tmp;
	tmp=dtmAlloc("bgbdt_skelbone_t", sizeof(BGBDT_SkelBone));
	return(tmp);
}

BGBDT_SkelAnim *Bt2Skel_AllocSkelAnim()
{
	BGBDT_SkelAnim *tmp;
	tmp=dtmAlloc("bgbdt_skelanim_t", sizeof(BGBDT_SkelAnim));
	return(tmp);
}

BGBDT_SkelAnimPose *Bt2Skel_AllocSkelAnimPose()
{
	BGBDT_SkelAnimPose *tmp;
	tmp=dtmAlloc("bgbdt_animpose_t", sizeof(BGBDT_SkelAnimPose));
	return(tmp);
}

BGBDT_SkelInstance *Bt2Skel_AllocSkelInstance()
{
	BGBDT_SkelInstance *tmp;
	tmp=dtmAlloc("bgbdt_skelinstance_t", sizeof(BGBDT_SkelInstance));
	return(tmp);
}



BTEIFGL_API int Bt2Skel_CheckLoadInfo(
	BGBDT_SkelInfo *inf, char *name)
{
	BGBDT_SkelBone *t_bone[256];
	BGBDT_SkelAnim *t_anim[256];
	BGBDT_SkelAnimPose **t_pose;
	BGBDT_SkelAnimPose *cpos;
	vec3 tporg;
	quat trot;
	float cporg[3];
	char tb[256];
	byte tbp[64];
	char *buf;
	char *cs, *lcs;
	char **a;
	double f, g, h;
	int sz, nb, na, nf, ntpb, mtf;
	int ca, cf, cb;
	int i, j, k, l;
	
	sprintf(tb, "%s.txt", name);
	buf=vf_loadfile(tb, &sz);
	if(!buf)
		return(-1);
	
	for(i=0; i<256; i++)
		t_bone[i]=NULL;
	
	inf->scale=1.0/1000;
	
	nb=0;
	cs=buf;
	while(*cs)
	{
		lcs=cs;
		cs=frgl_rsgets(tb, 255, cs);
		a=frgl_rsplit(tb);
		if(!a || !a[0])
			continue;
		if(a[0][0]=='/')continue;
		if(a[0][0]==';')continue;
		if(a[0][0]=='#')continue;

		if(!strcmp(a[0], "origin"))
		{
			inf->origin=vec3(atof(a[1]), atof(a[2]), atof(a[3]));
			continue;
		}

		if(!strcmp(a[0], "scale"))
		{
			inf->scale=1.0/atof(a[1]);
			continue;
		}

		if(!strcmp(a[0], "bone"))
		{
			i=atoi(a[1]);
			if(i>=n)n=i+1;
			t_bone[i]=Bt2Skel_AllocSkelBone();
			t_bone[i]->id=i;
			t_bone[i]->parid=atoi(a[2]);
			t_bone[i]->name=BGBDT_TagStr_Strdup(a[3]);
			t_bone[i]->absorg=vec3(atof(a[5]), atof(a[6]), atof(a[7]));
			continue;
		}

		if(!strcmp(a[0], "anim"))
		{
			cs=lcs;
			break;
		}

		if(!strcmp(a[0], "endbones"))
		{
			break;
		}
	}
	
	if(nb<=32)
	{
		inf->bone=inf->t_bone;
	}else
	{
		inf->bone=inf->t_bone;
		tmp=dtmAlloc("bgbdt_skelbonearr_t",
			bn*sizeof(BGBDT_SkelBone *));
	}
	
	for(i=0; i<nb; i++)
		{ inf->bone[i]=t_bone[i]; }
	inf->n_bone=nb;
	
	for(i=0; i<nb; i++)
	{
		j=inf->bone[i]->parid;
		if(j>0)
		{
			inf->bone[i]->relorg=v3scale(
				v3sub(inf->bone[i]->absorg, inf->bone[j]->absorg),
				inf->scale);
		}else
		{
			inf->bone[i]->relorg=v3scale(
				v3sub(inf->bone[i]->absorg, inf->origin),
				inf->scale);
		}
	}
	
	na=0;
	ca=0; cf=0; cb=0;
	ntpb=0;
	t_pose=NULL;
	
	while(*cs)
	{
		cs=frgl_rsgets(tb, 255, cs);
		a=frgl_rsplit(tb);
		if(!a || !a[0])
			continue;
		if(a[0][0]=='/')continue;
		if(a[0][0]==';')continue;
		if(a[0][0]=='#')continue;

		if(!strcmp(a[0], "endanim"))
		{
			k=ntbp*t_anim[ca]->n_frame;
			t_anim[ca]->pose=dtmAlloc("bgbdt_skelanimposes_t",
				k*sizeof(BGBDT_SkelAnimPose *));
			if(ntpb<=16)
				{ t_anim[ca]->pbone=t_anim[ca]->t_pbone; }
			else
				{ t_anim[ca]->pbone=dtmAlloc("bgbdt_skelanimpbone_t", ntpb); }
			t_anim[ca]->n_pbone=ntpb;
			memcpy(t_anim[ca]->pbone, tbp, ntpb);
			
			for(i=0; i<nf; i++)
				for(j=0; j<ntpb; j++)
					{ t_anim[ca]->pose[i*ntpb+j]=t_pose[i*nb+j]; }
			continue;
		}

		if(!strcmp(a[0], "anim"))
		{
			ca=atoi(a[1]);
			nf=atoi(a[3]);
			t_anim[ca]=Bt2Skel_AllocSkelAnim();
			t_anim[ca]->name=BGBDT_TagStr_Strdup(a[2]);
			t_anim[ca]->n_frame=nf;
			ntpb=0;
			
			k=nf;
			if(k<64)
				k=64;
			if(!t_pose)
			{
				t_pose=frgl_malloc(
					k*nb*sizeof(BGBDT_SkelAnimPose *));
				mtf=k;
			}
			if(k>mtf)
			{
				t_pose=frgl_realloc(t_pose,
					k*nb*sizeof(BGBDT_SkelAnimPose *));
				mtf=k;
			}
			for(i=0; i<k*nb; i++)
				t_pose[i]=NULL;
			continue;
		}
		
		if(!strcmp(a[0], "frame"))
		{
			cf=atoi(a[1]);
			continue;
		}

		if(!strcmp(a[0], "pose"))
		{
			for(i=1; a[i]; i++)
			{
				if(a[i][0]=='B')
				{
					k=atoi(a[i]+1);
					for(j=0; j<ntpb; j++)
						if(tbp[j]==k)
							break;
					if(j>=ntpb)
						{ j=ntpb++; tbp[j]=k; }
					cb=j;
					
					if(t_bone[k]->parid>0)
						{ tporg=t_bone[t_bone[k]->parid]->absorg; }
					else
						{ tporg=inf->origin; }

					cporg[0]=v3x(tporg);
					cporg[1]=v3y(tporg);
					cporg[2]=v3z(tporg);

					if(!t_pose[cf*nb+j])
						t_pose[cf*nb+j]=Bt2Skel_AllocSkelAnimPose();
					cpos=t_pose[cf*nb+j];
					cpos->org=t_bone[k]->relorg;
					cpos->rot=quat(1, 0, 0, 0);
					continue;
				}
				
				if(a[i][0]=='X')
				{
					f=atof(a[i]+1);
					f=(f-cporg[0])*inf->scale;
					cpos->org=vec3(f, v3y(cpos->org), v3z(cpos->org));
					continue;
				}

				if(a[i][0]=='Y')
				{
					f=atof(a[i]+1);
					f=(f-cporg[1])*inf->scale;
					cpos->org=vec3(v3x(cpos->org), f, v3z(cpos->org));
					continue;
				}

				if(a[i][0]=='Z')
				{
					f=atof(a[i]+1);
					f=(f-cporg[2])*inf->scale;
					cpos->org=vec3(v3x(cpos->org), v3y(cpos->org), f);
					continue;
				}

				if(a[i][0]=='P')
				{
					f=atof(a[i]+1)*(M_PI/180);
					g=sin(f); h=cos(f);
					trot=quat(h, g, 0, 0);
					cpos->rot=qmul(cpos->rot, trot);
					continue;
				}

				if(a[i][0]=='Q')
				{
					f=atof(a[i]+1)*(M_PI/180);
					g=sin(f); h=cos(f);
					trot=quat(h, 0, g, 0);
					cpos->rot=qmul(cpos->rot, trot);
					continue;
				}

				if(a[i][0]=='R')
				{
					f=atof(a[i]+1)*(M_PI/180);
					g=sin(f); h=cos(f);
					trot=quat(h, 0, 0, g);
					cpos->rot=qmul(cpos->rot, trot);
					continue;
				}
			}
			continue;
		}
	}
	
	if(t_pose)
		frgl_free(t_pose);

	frgl_free(buf);
	return(0);
}
