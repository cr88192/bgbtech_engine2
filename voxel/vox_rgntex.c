/*
 * RegionBox
 * 
 * Stores some RGB+Depth images of a region.
 * A region has 256x256 faces, representing chunk colors along each axis.
 * Will function similarly to depth-maps.
 */

BGBDT_VoxRegionBox *BGBDT_AllocRegionBox(BGBDT_VoxWorld *world)
{
	BGBDT_VoxRegionBox *tmp;
	
	if(world->freergnbox)
	{
		tmp=world->freergnbox;
		world->freergnbox=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_VoxRegionBox));
		return(tmp);
	}
	
	tmp=frgl_talloc("bgbdt_voxregionbox_t", sizeof(BGBDT_VoxRegionBox));
	return(tmp);
}

void BGBDT_FreeRegionBox(BGBDT_VoxWorld *world, BGBDT_VoxRegionBox *rgn)
{
	rgn->next=world->freergnbox;
	world->freergnbox=rgn;
}

BTEIFGL_API BGBDT_VoxRegionBox *BGBDT_WorldGetRegionBox(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	int bx, by, bz;

	bx=xyz.x>>BGBDT_XYZ_SHR_REGION_XY;
	by=xyz.y>>BGBDT_XYZ_SHR_REGION_XY;
	bz=xyz.z>>BGBDT_XYZ_SHR_REGION_Z;
	
	return(BGBDT_WorldGetRegionBoxBxyz(world, bx, by, bz, accfl));
}

BTEIFGL_API BGBDT_VoxRegionBox *BGBDT_WorldGetRegionBoxBxyz(
	BGBDT_VoxWorld *world,
	int bx, int by, int bz, int accfl)
{
	char tbuf[256];
	BGBDT_VoxRegionBox *rcur;
	byte *buf, *buf1;
	int szbuf, szbuf1;
	int bid;
	int i, j, k;

//	rcur=world->lastrgn;
//	if(rcur && (rcur->bx==bx) && (rcur->by==by) && (rcur->bz==bz))
//		return(rcur);

	rcur=world->rgnbox;
	while(rcur)
	{
		if((rcur->bx==bx) && (rcur->by==by) && (rcur->bz==bz))
		{
//			world->lastrgn=rcur;
			return(rcur);
		}
		rcur=rcur->next;
	}

	if(accfl&BGBDT_ACCFL_NOLOAD)
		return(NULL);
	if(accfl&BGBDT_ACCFL_NOLOADRGN)
		return(NULL);
	
	//load region.

	bid=bgbdt_xyz2rgnid(bx, by, bz);
	sprintf(tbuf, "region/%s/%08X.rgnb", world->worldname, bid);

	if(!bgbdt_voxrgn_enctobuf)
	{
		szbuf=1<<22;
		buf=frgl_malloc(szbuf);
		bgbdt_voxrgn_enctobuf=buf;
		bgbdt_voxrgn_szenctobuf=szbuf;
	}

	buf=bgbdt_voxrgn_enctobuf;
	szbuf=bgbdt_voxrgn_szenctobuf;
	i=VfLoadFile(tbuf, (void **)(&buf), &szbuf);
	if(i<0)
	{
		buf=NULL;
	}
//	buf=vf_loadfile(tbuf, &szbuf);

	if(buf && BGBDT_VoxRgn_CheckRegionLzMagic(buf))
	{
		buf1=NULL;
		BGBDT_VoxRgn_UnpackRgnLz(world, buf, szbuf, &buf1, &szbuf1);
//		frgl_free(buf);
		buf=buf1;
		szbuf=szbuf1;
	}else if(buf)
	{
		buf1=frgl_malloc(szbuf);
		memcpy(buf1, buf, szbuf);
		buf=buf1;
	}

	if(buf)
	{
		rcur=BGBDT_AllocRegionBox(world);
		rcur->next=world->rgnbox;
		world->rgnbox=rcur;
		rcur->world=world;
		rcur->bx=bx;
		rcur->by=by;
		rcur->bz=bz;

		rcur->rgnbuf=buf;
		rcur->szrgnbuf=szbuf;
		
		rcur->nrgncell=szbuf>>BGBDT_RGN_CELLSHR;
		rcur->rgnmap=frgl_malloc((rcur->nrgncell+3)/4);
	
		BGBDT_WorldDecodeRegionBoxData(world, rcur);

//		world->lastrgn=rcur;
		return(rcur);
	}

	if(accfl&BGBDT_ACCFL_ENNEWRGN)
	{
		rcur=BGBDT_AllocRegionBox(world);
		rcur->next=world->rgnbox;
		world->rgnbox=rcur;
		rcur->world=world;
		rcur->bx=bx;
		rcur->by=by;
		rcur->bz=bz;
		rcur->flags=BGBDT_RGNFL_NEWRGN;

		for(i=0; i<6; i++)
		{
			rcur->rgnbox[i]=frgl_malloc(256*256*4);
		}

//		world->lastrgn=rcur;
		return(rcur);
	}

	return(NULL);
}

int BGBDT_WorldCheckRegionBoxMagic(byte *buf)
{
	if((buf[0]!=0xE4) || (buf[4]!='R') || (buf[5]!='G') ||
		(buf[6]!='B') || (buf[7]!='X'))
			return(0);
	return(1);
}

int BGBDT_WorldDecodeRegionBoxData(BGBDT_VoxWorld *world,
	BGBDT_VoxRegionBox *rgn)
{
	byte *cs, *cse, *cs1, *cs2, *cs3;
	int hsz;
	int tsz;
	int i, j, k;

	if(!rgn || !rgn->rgnbuf)
		return(-1);
	
	cs=rgn->rgnbuf;
//	if((cs[0]!=0xE4) || (cs[4]!='R') || (cs[5]!='G') ||
//		(cs[6]!='N') || (cs[7]!='0'))
	if(!BGBDT_WorldCheckRegionBoxMagic(cs))
	{
		FRGL_DBGBREAK
		return(-1);
	}

	hsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
	cse=cs+hsz;
	cs+=8;
	
	while(cs<cse)
	{
		if(cs[0]==0xE3)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			cs1=cs+tsz;
			//...
			cs=cs1;
			continue;
		}

		if(cs[0]==0xE4)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			cs1=cs+tsz;
			cs2=cs+8;

			if((cs[4]=='R') && (cs[5]=='G') &&
				(cs[6]=='B') && (cs[7]=='4'))
			{
				for(i=0; i<6; i++)
				{
					j=(cs2[i*4+0]<<24)|(cs2[i*4+1]<<16)|
						(cs2[i*4+2]<<8)|(cs2[i*4+3]);
					rgn->boxofs[i]=j;
				}
				cs=cs1;
				continue;
			}

#if 0
			if((cs[4]=='O') && (cs[5]=='F') &&
				(cs[6]=='S') && (cs[7]=='4'))
			{
				for(i=0; i<BGBDT_XYZ_TSZ_RGN_XYZ; i++)
				{
					j=(cs2[i*4+0]<<24)|(cs2[i*4+1]<<16)|
						(cs2[i*4+2]<<8)|(cs2[i*4+3]);
					rgn->chkofs[i]=j;
				}
				cs=cs1;
				continue;
			}

			if((cs[4]=='O') && (cs[5]=='F') &&
				(cs[6]=='S') && (cs[7]=='3'))
			{
				for(i=0; i<BGBDT_XYZ_TSZ_RGN_XYZ; i++)
				{
					j=(cs2[i*3+0]<<16)|
						(cs2[i*3+1]<<8)|(cs2[i*3+2]);
					rgn->chkofs[i]=j;
				}
				cs=cs1;
				continue;
			}

			if((cs[4]=='C') && (cs[5]=='E') &&
				(cs[6]=='B') && (cs[7]=='M'))
			{
				memcpy(rgn->rgnmap, cs2, tsz-8);
				cs=cs1;
				continue;
			}

			if((cs[4]=='C') && (cs[5]=='O') &&
				(cs[6]=='B') && (cs[7]=='M'))
			{
				j=(cs2[0]<<24)|(cs2[1]<<16)|
					(cs2[2]<<8)|(cs2[3]);
				k=(cs2[4]<<24)|(cs2[5]<<16)|
					(cs2[6]<<8)|(cs2[7]);
				memcpy(rgn->rgnmap, rgn->rgnbuf+j, k);
				cs=cs1;
				continue;
			}

			if((cs[4]=='S') && (cs[5]=='T') &&
				(cs[6]=='R') && (cs[7]=='S'))
			{
				k=tsz;
				if(k<1024)
					k=1024;
			
				rgn->strtab_buf=frgl_malloc(k);
				memcpy(rgn->strtab_buf, cs2, tsz-8);
				cs3=rgn->strtab_buf+(tsz-8);
				while(((cs3+2)>rgn->strtab_buf) && !cs3[0] && !cs3[-1])
					cs3--;
				cs3++;
				
				rgn->strtab_buf[0]=0;
				rgn->strtab_buf[1]=0;
				rgn->strtab_msz=k;
				rgn->strtab_nsz=cs3-rgn->strtab_buf;
				if(rgn->strtab_nsz<2)
					rgn->strtab_nsz=2;
				
				cs=cs1;
				continue;
			}

			if((cs[4]=='S') && (cs[5]=='T') &&
				(cs[6]=='I') && (cs[7]=='3'))
			{
				k=(tsz-8)/3;
				j=k;
				if(j<256)j=256;

				rgn->strix_ofs=frgl_malloc(j*sizeof(int));
				rgn->strix_num=k;
				rgn->strix_max=j;

				for(i=0; i<k; i++)
				{
					j=(cs2[i*3+0]<<16)|
						(cs2[i*3+1]<<8)|(cs2[i*3+2]);
					rgn->strix_ofs[i]=j;
				}
				cs=cs1;
				continue;
			}

			if((cs[4]=='S') && (cs[5]=='T') &&
				(cs[6]=='I') && (cs[7]=='2'))
			{
				k=(tsz-8)/2;
				j=k;
				if(j<256)j=256;

				rgn->strix_ofs=frgl_malloc(j*sizeof(int));
				rgn->strix_num=k;
				rgn->strix_max=j;

				for(i=0; i<k; i++)
				{
					j=(cs2[i*2+0]<<8)|(cs2[i*2+1]);
					rgn->strix_ofs[i]=j;
				}
				cs=cs1;
				continue;
			}
#endif

			cs=cs1;
			continue;
		}

		FRGL_DBGBREAK
	}
	return(0);
}


int BGBDT_RegionBoxCellGetTag(BGBDT_VoxRegionBox *rgn, int idx)
{
	int i, j, k;
	
	if((idx<0) || (idx>=rgn->nrgncell))
		FRGL_DBGBREAK
	
	i=(rgn->rgnmap[idx>>2]>>((idx&3)<<1))&3;
	return(i);
}

int BGBDT_RegionBoxCellSetTag(BGBDT_VoxRegionBox *rgn, int idx, int val)
{
	int i, j, k;

	if((idx<0) || (idx>=rgn->nrgncell))
		FRGL_DBGBREAK

	rgn->rgnmap[idx>>2]&=~(3<<((idx&3)<<1));
	rgn->rgnmap[idx>>2]|=(val&3)<<((idx&3)<<1);
	return(0);
}

int BGBDT_RegionBoxFindFreeCellData(BGBDT_VoxRegionBox *rgn, int len)
{
	int i, j, k, l, nc;
	
	if(!rgn->rgnmap)
		return(-1);
	
	nc=rgn->nrgncell;
	i=0;
	while(i<nc)
	{
		j=BGBDT_RegionBoxCellGetTag(rgn, i);
		if(j)
		{
			while(i<nc)
			{
				j=BGBDT_RegionBoxCellGetTag(rgn, i);
				if(!j)break;
				i++;
			}
			continue;
		}
		
		l=i+len;
		if(l>nc)
			break;
		for(j=i; j<l; j++)
		{
			k=BGBDT_RegionBoxCellGetTag(rgn, j);
			if(k)break;
		}

		if(j>=l)
			return(i);
		i=j+1;
	}
	return(-1);
}

int BGBDT_RegionBoxExpandCellData(BGBDT_VoxRegionBox *rgn, int len)
{
	int nc, nc1, nl;
	int nb, nb1;

	nc=rgn->nrgncell;
	nc1=nc+(nc>>2);
	nl=nc+len;
	while(nc1<=nl)
		nc1=nc1+(nc1>>2);
	
	nc1=((nc1+15)>>4)<<4;
	
	nb=(nc+3)>>2;
	nb1=(nc1+3)>>2;
	
	rgn->rgnbuf=frgl_realloc(rgn->rgnbuf, (nc1<<BGBDT_RGN_CELLSHR)+4096);
	rgn->rgnmap=frgl_realloc(rgn->rgnmap, nb1+256);
	memset(rgn->rgnmap+nb, 0, nb1-nb);
	
	rgn->szrgnbuf=nc1<<BGBDT_RGN_CELLSHR;
	rgn->nrgncell=nc1;
	return(0);
}

int BGBDT_RegionBoxAllocCellData(BGBDT_VoxRegionBox *rgn, int sz)
{
	int i, j, k, n;
	
	n=(sz+BGBDT_RGN_CELLPAD)>>BGBDT_RGN_CELLSHR;
	i=BGBDT_RegionBoxFindFreeCellData(rgn, n);
	if(i>=0)
	{
		BGBDT_RegionBoxCellSetTag(rgn, i, 1);
		memset(rgn->rgnbuf+(i<<BGBDT_RGN_CELLSHR),
			0, 1<<BGBDT_RGN_CELLSHR);

		for(j=1; j<n; j++)
		{
			BGBDT_RegionBoxCellSetTag(rgn, i+j, 2);
			memset(rgn->rgnbuf+((i+j)<<BGBDT_RGN_CELLSHR),
				0, 1<<BGBDT_RGN_CELLSHR);
		}
		return(i);
	}
	
	BGBDT_RegionBoxExpandCellData(rgn, n);
	return(BGBDT_RegionBoxAllocCellData(rgn, sz));
}

int BGBDT_RegionBoxFreeCellData(BGBDT_VoxRegionBox *rgn, int ix)
{
	int i, j, k;

	j=BGBDT_RegionBoxCellGetTag(rgn, ix);
	if(j!=1)
		return(-1);

	BGBDT_RegionBoxCellSetTag(rgn, ix, 0);
	memset(rgn->rgnbuf+(ix<<BGBDT_RGN_CELLSHR),
		0, 1<<BGBDT_RGN_CELLSHR);
	for(i=ix+1; i<rgn->nrgncell; i++)
	{
		j=BGBDT_RegionBoxCellGetTag(rgn, i);
		if(j!=2)break;
		BGBDT_RegionBoxCellSetTag(rgn, i, 0);
		memset(rgn->rgnbuf+(i<<BGBDT_RGN_CELLSHR),
			0, 1<<BGBDT_RGN_CELLSHR);
	}
	return(0);
}

int BGBDT_WorldSaveRegionBoxData(BGBDT_VoxWorld *world,
	BGBDT_VoxRegionBox *rgn)
{
	char tbuf[256];
//	BGBDT_VoxChunk *chk;
	byte *tsbuf;
	byte *tobuf;
	byte *ct;
	int bid;
	int bx, by, bz, bn, bo;
	int n, sz0, sz1, sz2, sz3;
	int i, j, k;

#ifdef __EMSCRIPTEN__
	return(0);
#endif
	
	if(!rgn->rgnbuf)
	{
//		sz0=1<<22;
		sz0=1<<20;
		rgn->rgnbuf=frgl_malloc(sz0);
		rgn->szrgnbuf=sz0;
		rgn->nrgncell=sz0>>BGBDT_RGN_CELLSHR;
		rgn->rgnmap=frgl_malloc((rgn->nrgncell+3)/4);
		
		k=262144/64;
		for(i=0; i<k; i++)
		{
			BGBDT_RegionBoxCellSetTag(rgn, i, 3);
		}
	}

	tsbuf=frgl_malloc(1<<20);
	for(i=0; i<6; i++)
	{
		if(rgn->boxofs[i])
		{
			BGBDT_RegionBoxFreeCellData(rgn,
				rgn->boxofs[i]>>BGBDT_RGN_CELLSHR);
			rgn->boxofs[i]=0;
		}
	
		j=BGBBTJ_BufPNG_EncodeFast(tsbuf, 1<<20, rgn->rgnbox[i], 256, 256);
		if(j<0)
			continue;

		k=BGBDT_RegionBoxAllocCellData(rgn, j);
		bo=k<<BGBDT_RGN_CELLSHR;
		rgn->boxofs[i]=bo;
		
		memcpy(rgn->rgnbuf+bo, tsbuf, j);
	}

#if 0
	n=BGBDT_XYZ_TSZ_RGN_XYZ;
	for(i=0; i<n; i++)
	{
		chk=rgn->chkptr[i];
		if(!chk)
			continue;

		if(!(chk->flags&BGBDT_CHKFL_SAVEDIRTY) && rgn->chkofs[i])
			continue;

		j=rgn->chkofs[i];
		if(j)
		{
			BGBDT_RegionFreeCellData(rgn, j>>BGBDT_RGN_CELLSHR);
			rgn->chkofs[i]=0;
		}
		
		j=BGBDT_WorldEncodeChunk(world, rgn, chk, 0);
		rgn->chkofs[i]=j;
		chk->flags&=~BGBDT_CHKFL_SAVEDIRTY;
		
//		printf("Saved Chk %p %04X\n", chk, chk->flags);
		
		if(!j)
		{
			bid=bgbdt_xyz2rgnid(rgn->bx, rgn->by, rgn->bz);
			printf("BGBDT_WorldSaveRegionData: Fail Save %d,%d,%d Rgn=%08F\n",
				chk->bx, chk->by, chk->bz, bid);
		}
	}
#endif

#if 1
	for(i=0; i<rgn->nrgncell; i++)
	{
		j=BGBDT_RegionBoxCellGetTag(rgn, i);
		if(!j)
		{
			memset(rgn->rgnbuf+(i<<BGBDT_RGN_CELLSHR),
				0, 1<<BGBDT_RGN_CELLSHR);
		}
	}
#endif
	
	ct=rgn->rgnbuf;
	ct[0]=0xE4;
	ct[4]='R';	ct[5]='G';
	ct[6]='B';	ct[7]='X';
	ct+=8;


#if 1
	n=6;
	sz0=8+n*4;
	ct[0]=0xE4;		ct[1]=sz0>>16;
	ct[2]=sz0>> 8;	ct[3]=sz0    ;
	ct[4]='B';		ct[5]='O';
	ct[6]='X';		ct[7]='4';
	
	for(i=0; i<n; i++)
	{
		j=rgn->boxofs[i];
		ct[8+i*4+0]=j>>24;
		ct[8+i*4+1]=j>>16;
		ct[8+i*4+2]=j>> 8;
		ct[8+i*4+3]=j    ;
	}
	ct+=sz0;
#endif

#if 0
	n=BGBDT_XYZ_TSZ_RGN_XYZ;
	if(rgn->szrgnbuf>=(1<<24))
	{
		sz0=8+n*4;
		ct[0]=0xE4;		ct[1]=sz0>>16;
		ct[2]=sz0>> 8;	ct[3]=sz0    ;
		ct[4]='O';		ct[5]='F';
		ct[6]='S';		ct[7]='4';
		
		for(i=0; i<n; i++)
		{
			j=rgn->chkofs[i];
			ct[8+i*4+0]=j>>24;
			ct[8+i*4+1]=j>>16;
			ct[8+i*4+2]=j>> 8;
			ct[8+i*4+3]=j    ;
		}
		ct+=sz0;
	}else
	{
		sz0=8+n*3;
		ct[0]=0xE4;		ct[1]=sz0>>16;
		ct[2]=sz0>> 8;	ct[3]=sz0    ;
		ct[4]='O';	ct[5]='F';
		ct[6]='S';	ct[7]='3';
		
		for(i=0; i<n; i++)
		{
			j=rgn->chkofs[i];
			ct[8+i*3+0]=j>>16;
			ct[8+i*3+1]=j>> 8;
			ct[8+i*3+2]=j    ;
		}
		ct+=sz0;
	}

	sz0=8+((rgn->nrgncell+3)/4);
	ct[0]=0xE4;		ct[1]=sz0>>16;
	ct[2]=sz0>> 8;	ct[3]=sz0    ;
	ct[4]='C';		ct[5]='E';
	ct[6]='B';		ct[7]='M';
	memcpy(ct+8, rgn->rgnmap, sz0-8);
	ct+=sz0;

	if(rgn->strtab_buf)
	{
		sz0=8+((rgn->strtab_nsz+3)/4)*4;
		ct[0]=0xE4;		ct[1]=sz0>>16;
		ct[2]=sz0>> 8;	ct[3]=sz0    ;
		ct[4]='S';		ct[5]='T';
		ct[6]='R';		ct[7]='S';
		memcpy(ct+8, rgn->strtab_buf, sz0-8);
		ct+=sz0;
		
		n=rgn->strix_num;
		
		if(rgn->strtab_nsz>65530)
		{
			sz0=8+(rgn->strix_num*3);
			ct[0]=0xE4;		ct[1]=sz0>>16;
			ct[2]=sz0>> 8;	ct[3]=sz0    ;
			ct[4]='S';		ct[5]='T';
			ct[6]='I';		ct[7]='3';
			for(i=0; i<n; i++)
			{
				j=rgn->strix_ofs[i];
				ct[8+i*3+0]=j>>16;
				ct[8+i*3+1]=j>> 8;
				ct[8+i*3+2]=j    ;
			}
			ct+=sz0;
		}else
		{
			sz0=8+(rgn->strix_num*2);
			ct[0]=0xE4;		ct[1]=sz0>>16;
			ct[2]=sz0>> 8;	ct[3]=sz0    ;
			ct[4]='S';		ct[5]='T';
			ct[6]='I';		ct[7]='2';
			for(i=0; i<n; i++)
			{
				j=rgn->strix_ofs[i];
				ct[8+i*2+0]=j>> 8;
				ct[8+i*2+1]=j    ;
			}
			ct+=sz0;
		}
	}
#endif

	sz1=ct-rgn->rgnbuf;
	ct=rgn->rgnbuf;
	ct[0]=0xE4;		ct[1]=sz1>>16;
	ct[2]=sz1>> 8;	ct[3]=sz1    ;

	bid=bgbdt_xyz2rgnid(rgn->bx, rgn->by, rgn->bz);
	sprintf(tbuf, "region/%s/%08X.rgnb", world->worldname, bid);

	printf("Saving RegionBox %08X\n", bid);

#if 1
	tobuf=bgbdt_voxrgn_enctobuf;
	sz3=bgbdt_voxrgn_szenctobuf;

	sz2=rgn->szrgnbuf*1.25;

	if(!tobuf || (sz2>sz3))
	{
		frgl_free(tobuf);
		tobuf=frgl_malloc(sz2);
		bgbdt_voxrgn_szenctobuf=sz2;
	}
	
//	sz3=BGBDT_WorldEncodeRegionLZ4(world, rgn,
//		rgn->rgnbuf, rgn->szrgnbuf, tobuf, sz2);

//	sz3=BGBDT_WorldEncodeRegionBLZ(world, rgn,
//		rgn->rgnbuf, rgn->szrgnbuf, tobuf, sz2);

	sz3=BGBDT_WorldEncodeRegionFeLz32(world, NULL,
		rgn->rgnbuf, rgn->szrgnbuf, tobuf, sz2);

	if(sz3>0)
	{
		vf_storefile(tbuf, tobuf, sz3);
	}else
	{
		vf_storefile(tbuf, rgn->rgnbuf, rgn->szrgnbuf);
	}
#else
	vf_storefile(tbuf, rgn->rgnbuf, rgn->szrgnbuf);
#endif

	return(0);
}


void BGBDT_TickVoxRegionBox(BGBDT_VoxWorld *world,
	BGBDT_VoxRegionBox *rgn)
{
	if(rgn->flags&BGBDT_RGNFL_RGNDIRTY)
	{
		BGBDT_WorldSaveRegionBoxData(world, rgn);

		rgn->flags&=~BGBDT_RGNFL_RGNDIRTY;
//		rgn->lastsave=t0;
	}
}

int BGBDT_RgnBox_UpdateChunkMap(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	BGBDT_VoxData td, td1;
	BGBDT_VoxDataStatus tds;
	BGBDT_VoxCoord xyz0;
	BGBDT_VoxRegionBox *rbox;
	BGBDT_VoxTypeInfo *tyi;
	u32 px;
	int x, y, z;
	int ax, ay, az;
	int ix, tix, vty;
	int i, j, k;

	rbox=BGBDT_WorldGetRegionBoxBxyz(world,
		chk->rgn->bx, chk->rgn->by, chk->rgn->bz,
		BGBDT_ACCFL_ENNEWRGN);

	for(z=0; z<16; z++)
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
	{
		BGBDT_WorldGetChunkVoxelData(world,
			x, y, z, chk, &td, &tds,
			BGBDT_ACCFL_NOLOAD|BGBDT_ACCFL_CHKADJ);

		vty=td.vtypel|(td.vtypeh<<8);
		tix=vty&4095;
		tyi=world->voxtypes[tix];
		if(!tyi)continue;
		if(tyi->flags&BGBDT_VOXFL_TRANSPARENT)
			continue;
			
		ax=(chk->bx<<4)|x;
		ay=(chk->by<<4)|y;
		az=(chk->bz<<4)|z;
		
		if((ax|ay|az)>>8)
			__debugbreak();
		
		for(i=0; i<6; i++)
		{
			switch(i)
			{
			case 0: ix=az*256+ay; k=255-ax; break;
			case 1: ix=az*256+ay; k=ax; break;
			case 2: ix=az*256+ax; k=255-ay; break;
			case 3: ix=az*256+ax; k=ay; break;
			case 4: ix=ay*256+ax; k=255-az; break;
			case 5: ix=ay*256+ax; k=az; break;
			}

			if(k>rbox->rgnbox[i][ix*4+3])
			{
				xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, x, y, z);
				px=BGBDT_WorldVoxel_GetVoxColorID(world, xyz0, td);
				rbox->rgnbox[i][ix*4+0]=px;
				rbox->rgnbox[i][ix*4+1]=px>>8;
				rbox->rgnbox[i][ix*4+2]=px>>16;
				rbox->rgnbox[i][ix*4+3]=k;
			}
		}
	}
}
