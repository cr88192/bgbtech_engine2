
s64 BGBDT_EntObj_ParseInt(char *s)
{
	s64 v;
	int sg;
	int i, j, k;

	if(*s=='-')
		{ s++; sg=-1; }
	else { sg=1; }

	if(*s=='0')
	{
		if(s[1]=='x')
		{
			v=0; s+=2;
			while(*s)
			{
				i=*s++; j=0;
				if(i=='_')
					continue;
				if((i>='0') && (i<='9'))
					{ j=i-'0'; }
				if((i>='A') && (i<='F'))
					{ j=10+(i-'A'); }
				if((i>='a') && (i<='f'))
					{ j=10+(i-'a'); }
				v=(v<<4)|j;
			}
			return(v*sg);
		}

		if(s[1]=='b')
		{
			v=0; s+=2;
			while(*s)
			{
				i=*s++; j=0;
				if(i=='_')
					continue;
				if((i>='0') && (i<='1'))
					{ j=i-'0'; }
				v=(v<<1)|j;
			}
			return(v*sg);
		}

		if(s[1]=='c')
		{
			v=0; s+=2;
			while(*s)
			{
				i=*s++; j=0;
				if(i=='_')
					continue;
				if((i>='0') && (i<='7'))
					{ j=i-'0'; }
				v=(v<<3)|j;
			}
			return(v*sg);
		}

		v=0; s+=2;
		while(*s)
		{
			i=*s++; j=0;
			if(i=='_')
				continue;
			if((i>='0') && (i<='7'))
				{ j=i-'0'; }
			v=(v<<3)|j;
		}
		return(v*sg);
	}

	v=0;
	while(*s)
	{
		i=*s++; j=0;
		if(i=='_')
			continue;
		if((i>='0') && (i<='9'))
			{ j=i-'0'; }
		v=(v*10)+j;
	}
	return(v*sg);
}

double BGBDT_EntObj_ParseFloat(char *s)
{
	return(atof(s));
}

char *BGBDT_EntObj_SigCommonSupertype(char *sa, char *sb)
{
	if(!sa)return(sb);
	if(!sb)return(sa);
	if(!strcmp(sa, sb))
		return(sa);
	
	if((*sa=='i') || (*sa=='b') ||
		(*sa=='a') || (*sa=='c') ||
		(*sa=='s') || (*sa=='t') ||
		(*sa=='h'))
	{
		if(	(*sa=='a') || (*sa=='b') ||
			(*sa=='c') || (*sa=='h'))
		{
			if(*sa!='h')
			{
				if(*sb=='a')	return("c");
				if(*sb=='b')	return("c");
				if(*sb=='c')	return("c");
			}

			if(*sb=='s')	return("s");
			if(*sb=='t')	return("t");
		}
	
		if(*sb=='i')	return("i");
		if(*sb=='x')	return("x");
		if(*sb=='f')	return("f");
		if(*sb=='d')	return("d");
		return("i");
	}

	if(*sa=='x')
	{
		if(*sb=='i')	return("x");
		if(*sb=='x')	return("x");
		if(*sb=='f')	return("d");
		if(*sb=='d')	return("d");
		return("x");
	}

	if(*sa=='f')
	{
		if(*sb=='i')	return("f");
		if(*sb=='x')	return("d");
		if(*sb=='f')	return("f");
		if(*sb=='d')	return("d");
		return("f");
	}

	if(*sa=='d')
	{
		if(*sb=='i')	return("d");
		if(*sb=='x')	return("d");
		if(*sb=='f')	return("d");
		if(*sb=='d')	return("d");
		return("d");
	}

	return(sb);
}

BGBDT_EntValue BGBDT_EntObj_ParseValueArr(char **rstr)
{
	char tb1[256], tb2[256];
	BGBDT_EntValue tva[256];
	BGBDT_EntValue tv;
	BGBDT_EntArray *arr;
	char *sig, *tsig;
	char *s, *s1, *s2;
	int ntva;

	s=*rstr;

	s=BGBDT_BxON_ParseTokenBasic(s, tb1);
	s1=BGBDT_BxON_ParseTokenBasic(s, tb2);

	if(strcmp(tb1, "X["))
	{
		tv.tag=BGBDT_ENTBTY_INVALID;
		return(tv);
	}

	ntva=0;
	while(1)
	{
		s1=BGBDT_BxON_ParseTokenBasic(s, tb1);
		if(!strcmp(tb1, "X]"))
			{ s=s1; break; }
		tv=BGBDT_EntObj_ParseValue(&s);
		tva[ntva++]=tv;

		s1=BGBDT_BxON_ParseTokenBasic(s, tb2);
		if(!strcmp(tb2, "X,"))
			{ s=s1; }
	}
	
	if(ntva==3)
	{
		if(	(tva[0].tag==BGBDT_ENTBTY_FLOAT) &&
			(tva[1].tag==BGBDT_ENTBTY_FLOAT) &&
			(tva[2].tag==BGBDT_ENTBTY_FLOAT))
		{
			tv.fxv[0]=(tva[0].lf)*4096+0.5;
			tv.fxv[1]=(tva[1].lf)*4096+0.5;
			tv.fxv[2]=(tva[2].lf)*4096+0.5;
			tv.tag=BGBDT_ENTBTY_FXV3;
			*rstr=s;
			return(tv);
		}
	}
	
	sig=NULL;
	for(i=0; i<ntva; i++)
	{
		tv=tva[i]; tsig=NULL;
		if(tv.tag==BGBDT_ENTBTY_INT)
		{
			tsig="i";
			if((tv.li>=(-32768)) && (tv.li<=32767))
				tsig="s";
			if((tv.li>=0) && (tv.li<=65535))
				tsig="t";
			if((tv.li>=(-128)) && (tv.li<=127))
				tsig="c";
			if((tv.li>=0) && (tv.li<=255))
				tsig="h";
			if((tv.li>=(1LL<<31)) || (tv.li<=(-(1LL<<31))))
				tsig="x";
		}
		
		if(tv.tag==BGBDT_ENTBTY_FLOAT)
			{ tsig="f"; }
		if(tv.tag==BGBDT_ENTBTY_STRING)
			{ tsig="Pc"; }
		if(tv.tag==BGBDT_ENTBTY_BOOL)
			{ tsig="b"; }
		if(tv.tag==BGBDT_ENTBTY_OBJREF)
			{ tsig="Co"; }
		if(tv.tag==BGBDT_ENTBTY_ARRAYREF)
			{ tsig="Cq"; }
		if(tv.tag==BGBDT_ENTBTY_FXV3)
			{ tsig="Cb"; }
		sig=BGBDT_EntObj_SigCommonSupertype(sig, tsig);
	}
	
	arr=BGBDT_EntObj_NewArray(sig, ntva);
	for(i=0; i<ntva; i++)
	{
		tv=tva[i];
		switch(tv.tag)
		{
		case BGBDT_ENTBTY_INT:
			BGBDT_EntObj_SetArrayIndexI(arr, i, tv.li);
			break;
		case BGBDT_ENTBTY_FLOAT:
			BGBDT_EntObj_SetArrayIndexI(arr, i, tv.lf);
			break;
		case BGBDT_ENTBTY_STRING:
			BGBDT_EntObj_SetArrayIndexP(arr, i, tv.ls);
			break;
		case BGBDT_ENTBTY_BOOL:
			BGBDT_EntObj_SetArrayIndexI(arr, i, tv.li);
			break;
		case BGBDT_ENTBTY_OBJREF:
			BGBDT_EntObj_SetArrayIndexP(arr, i, tv.li);
			break;
		case BGBDT_ENTBTY_ARRAYREF:
			BGBDT_EntObj_SetArrayIndexP(arr, i, tv.li);
			break;
		case BGBDT_ENTBTY_FXV3:
			BGBDT_EntObj_SetArrayIndexFxV(arr, i, tv.fxv, 3);
			break;
		}
	}

	tv.ptr=arr;
	tv.tag=BGBDT_ENTBTY_ARRAYREF;
	*rstr=s;
	return(tv);
}

BGBDT_EntValue BGBDT_EntObj_ParseValue(char **rstr)
{
	char tb1[256], tb2[256];
	BGBDT_EntValue tv;
	s64 vi;
	double vf;
	char *s, *s1, *s2;

	s=*rstr;

	s1=BGBDT_BxON_ParseTokenBasic(s, tb1);
	s2=BGBDT_BxON_ParseTokenBasic(s1, tb2);

	if(tb1[0]=='|')
	{
		s=s1;
		vi=BGBDT_EntObj_ParseInt(tb1+1);
		tv.li=vi;
		tv.tag=BGBDT_ENTBTY_INT;
		*rstr=s;
		return(tv);
	}
		
	if(tb1[0]=='#')
	{
		s=s1;
		vf=BGBDT_EntObj_ParseFloat(tb1+1);
		tv.lf=vf;
		tv.tag=BGBDT_ENTBTY_FLOAT;
		*rstr=s;
		return(tv);
	}

	if(tb1[0]=='S')
	{
		s=s1;
		tv.ls=frgl_strdup(tb1+1);
		tv.tag=BGBDT_ENTBTY_STRING;
		*rstr=s;
		return(tv);
	}

	if(!strcmp(tb1, "Itrue"))
	{
		s=s1;
		tv.li=BGBDT_ENTBTY_BOOL;
		tv.tag=1;
		*rstr=s;
		return(tv);
	}

	if(!strcmp(tb1, "Ifalse"))
	{
		s=s1;
		tv.li=0;
		tv.tag=BGBDT_ENTBTY_BOOL;
		*rstr=s;
		return(tv);
	}

	if(!strcmp(tb1, "Inull"))
	{
		s=s1;
		tv.ptr=NULL;
		tv.tag=BGBDT_ENTBTY_OBJREF;
		*rstr=s;
		return(tv);
	}

	if(!strcmp(tb1, "X-"))
	{
		if(tb2[0]=='|')
		{
			s=s2;
			vi=BGBDT_EntObj_ParseInt(tb2+1);
			tv.li=-vi;
			tv.tag=BGBDT_ENTBTY_INT;
			*rstr=s;
			return(tv);
		}
		
		if(tb2[0]=='#')
		{
			s=s2;
			vf=BGBDT_EntObj_ParseFloat(tb2+1);
			tv.lf=-vf;
			tv.tag=BGBDT_ENTBTY_FLOAT;
			*rstr=s;
			return(tv);
		}
	}

	if(!strcmp(tb1, "X{"))
	{
		tv.ptr=BGBDT_EntObj_ParseEntObj(&s);
		tv.tag=BGBDT_ENTBTY_OBJREF;
		*rstr=s;
		return(tv);
	}

	if(!strcmp(tb1, "X["))
	{
		tv=BGBDT_EntObj_ParseValueArr(&s);
		*rstr=s;
		return(tv);
	}
	
	tv.tag=BGBDT_ENTBTY_INVALID;
	*rstr=s;
	return(tv);
}

BGBDT_EntObj *BGBDT_EntObj_ParseEntObj(char **rstr)
{
	char tb1[256], tb2[256];
	char *afn[256];
	char *afs[256];
	BGBDT_EntValue *afv[256];
	BGBDT_EntField *afi[256];
	BGBDT_EntValue tval;
	BGBDT_EntClass *cli;
	BGBDT_EntObj *obj;
	char *fn, *sig;
	char *s, *s1, *s2;
	int nfn;
	
	s=*rstr;
	s=BGBDT_BxON_ParseTokenBasic(s, tb1);
	BGBDT_BxON_ParseTokenBasic(s, tb2);
	
	if(strcmp(tb1, "X{"))
		return(NULL);
	
	nfn=0;
	while(1)
	{
		s=BGBDT_BxON_ParseTokenBasic(s, tb1);
		s1=BGBDT_BxON_ParseTokenBasic(s, tb2);

		if(!strcmp(tb1, "X}"))
			break;
		
		if((tb1[0]!='I') || strcmp(tb2, "X:"))
		{
			break;
		}
		
		fn=frgl_strdup(tb1+1);
		
		s=s1;

		tval=BGBDT_EntObj_ParseValue(&s);

		s1=BGBDT_BxON_ParseTokenBasic(s, tb2);
		if(!strcmp(tb2, "X,"))
			{ s=s1; }

		switch(tval.tag)
		{
		case BGBDT_ENTBTY_INT:
			sig="i"; break;
		case BGBDT_ENTBTY_FLOAT:
			sig="d"; break;
		case BGBDT_ENTBTY_STRING:
			sig="Pc"; break;
		case BGBDT_ENTBTY_BOOL:
			sig="b"; break;
		case BGBDT_ENTBTY_OBJREF:
			sig="r"; break;
		case BGBDT_ENTBTY_ARRAYREF:
			sig="Qr"; break;
		case BGBDT_ENTBTY_FXV3:
			sig="Cb"; break;
		default:
			sig="Pv"; break;
		}

		i=nfn++;
		afn[i]=fn;
		afs[i]=sig;
		afv[i]=tval;
		afi[i]=BGBDT_EntObj_GetField(fn, sig);
	}
	
	cli=BGBDT_EntObj_GetClass(afi, nfn);
	obj=BGBDT_EntObj_NewInstance(cli);
	
	for(i=0; i<nfn; i++)
	{
		switch(afv[i].tag)
		{
		case BGBDT_ENTBTY_INT:
			BGBDT_EntObj_SetObjFieldI(obj, afi[i], afv[i].li);
			break;
		case BGBDT_ENTBTY_FLOAT:
			BGBDT_EntObj_SetObjFieldF(obj, afi[i], afv[i].lf);
			break;
		case BGBDT_ENTBTY_STRING:
			BGBDT_EntObj_SetObjFieldP(obj, afi[i], afv[i].ls);
			break;
		case BGBDT_ENTBTY_BOOL:
			BGBDT_EntObj_SetObjFieldI(obj, afi[i], afv[i].li);
			break;
		case BGBDT_ENTBTY_OBJREF:
			BGBDT_EntObj_SetObjFieldP(obj, afi[i], afv[i].ptr);
			break;
		case BGBDT_ENTBTY_ARRAYREF:
			BGBDT_EntObj_SetObjFieldP(obj, afi[i], afv[i].ptr);
			break;
		case BGBDT_ENTBTY_FXV3:
			sig="Cb"; break;
		default:
			break;
		}
	}
	
	return(obj);
}
