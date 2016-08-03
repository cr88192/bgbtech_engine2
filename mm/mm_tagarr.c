#include <bteifgl.h>

/* Note: BSVM Base Types, not Native/C Base Types */
int BGBDT_TagArr_BaseTypeSize(int bty)
{
	int sz;
	switch(bty)
	{
	case BGBDT_BASETY_INT:		sz=4; break;
	case BGBDT_BASETY_LONG:		sz=8; break;
	case BGBDT_BASETY_FLOAT:	sz=4; break;
	case BGBDT_BASETY_DOUBLE:	sz=8; break;
	case BGBDT_BASETY_ADDRESS:	sz=8; break;
	case BGBDT_BASETY_UINT:		sz=4; break;
	case BGBDT_BASETY_UBYTE:	sz=1; break;
	case BGBDT_BASETY_SHORT:	sz=2; break;
	case BGBDT_BASETY_SBYTE:	sz=1; break;
	case BGBDT_BASETY_USHORT:	sz=2; break;
	case BGBDT_BASETY_ULONG:	sz=8; break;

	case BGBDT_BASETY_NLONG:	sz=sizeof(long); break;
	case BGBDT_BASETY_UNLONG:	sz=sizeof(long); break;
	case BGBDT_BASETY_X128:		sz=16; break;

	default: sz=8; break;
	}
	return(sz);
}

BTEIFGL_API dtVal BGBDT_TagArr_NewArray(int size, int bty)
{
	BGBDT_TagArrHead *arh;
	dtVal arv;
	int str, sz, sz2, szh, stsh;
	
	str=BGBDT_TagArr_BaseTypeSize(bty);
	switch(str)
	{
	case 1: stsh=0; break;
	case 2: stsh=1; break;
	case 4: stsh=2; break;
	case 8: stsh=3; break;
	case 16: stsh=4; break;
	}
	
	sz=size*str;
	if(sz<=256)
	{
		szh=sizeof(BGBDT_TagArrHead);
		szh=(szh+(str-1))&(~(str-1));
		sz2=szh+sz;
	
		arh=dtmAlloc("bgbdt_tagarrhead_t", sz2);
		arh->sz=size;
		arh->str=str;
		arh->bty=bty;
		arh->strsh=stsh;
		arh->scsz=sz;

		arh->data=((byte *)arh)+szh;
	}else
	{
		arh=dtmAlloc("bgbdt_tagarrhead_t", sizeof(BGBDT_TagArrHead));
		arh->sz=size;
		arh->str=str;
		arh->bty=bty;
		arh->strsh=stsh;
		arh->scsz=sz;

		arh->data=dtmAlloc("bgbdt_tagarrdata_t", size*str);
	}
//	arv.lo=BGBDT_MM_GetObjIdForPtr(arh);
	arv.lo=DTV_GetObjIdForPtr(arh);
	arv.hi=0xC0000000UL;
	return(arv);
}

int BGBDT_TagArr_IsArrayP(dtVal objv)
{
	static int objty_arrhead=-1;
	if(objty_arrhead<0)
	{
		objty_arrhead=BGBDT_MM_GetIndexObjTypeName("bgbdt_tagarrhead_t");
	}
	return(dtvCheckPtrTagP(objv, objty_arrhead));
}

int BGBDT_TagArr_IsArrayBtyP(dtVal objv, int bty)
{
	static int objty_arrhead=-1;
	int bt;

	if(objty_arrhead<0)
	{
		objty_arrhead=BGBDT_MM_GetIndexObjTypeName("bgbdt_tagarrhead_t");
	}
	if(!dtvCheckPtrTagP(objv, objty_arrhead))
		return(0);

	bt=dtvArrayGetBaseType(objv);
	return(bt==bty);
}

dtVal BGBDT_TagArr_GetIndexVariant(dtVal arr, int idx)
{
	dtVal va;
	double vf;
	s64 vi;
	int bt;

	bt=dtvArrayGetBaseType(arr);

	switch(bt)
	{
	case BGBDT_BASETY_INT:
		vi=dtvArrayGetIndexInt(arr, idx);
		va=dtvWrapInt(vi);
		break;
	case BGBDT_BASETY_LONG:
		vi=dtvArrayGetIndexLong(arr, idx);
		va=dtvWrapLong(vi);
		break;
	case BGBDT_BASETY_FLOAT:
		vf=dtvArrayGetIndexFloat(arr, idx);
		va=dtvWrapFloat(vf);
		break;
	case BGBDT_BASETY_DOUBLE:
		vf=dtvArrayGetIndexDouble(arr, idx);
		va=dtvWrapDouble(vf);
		break;
	case BGBDT_BASETY_ADDRESS:
		va=dtvArrayGetIndexDtVal(arr, idx);
		break;
	case BGBDT_BASETY_UINT:
		vi=(u32)dtvArrayGetIndexInt(arr, idx);
		va=dtvWrapLong(vi);
		break;
	case BGBDT_BASETY_UBYTE:
		vi=dtvArrayGetIndexByte(arr, idx);
		va=dtvWrapInt(vi);
		break;
	case BGBDT_BASETY_SHORT:
		vi=dtvArrayGetIndexShort(arr, idx);
		va=dtvWrapInt(vi);
		break;
	case BGBDT_BASETY_SBYTE:
		vi=dtvArrayGetIndexSByte(arr, idx);
		va=dtvWrapInt(vi);
		break;
	case BGBDT_BASETY_USHORT:
		vi=dtvArrayGetIndexUShort(arr, idx);
		va=dtvWrapInt(vi);
		break;
	case BGBDT_BASETY_ULONG:
		vi=dtvArrayGetIndexLong(arr, idx);
		va=dtvWrapLong(vi);
		break;
	default:
		va=DTV_UNDEFINED;
		break;
	}
	return(va);
}

void BGBDT_TagArr_SetIndexVariant(dtVal arr, int idx, dtVal val)
{
	int bt;

	bt=dtvArrayGetBaseType(arr);
	switch(bt)
	{
	case BGBDT_BASETY_INT:
		dtvArraySetIndexInt(arr, idx, dtvUnwrapInt(val));
		break;
	case BGBDT_BASETY_LONG:
		dtvArraySetIndexLong(arr, idx, dtvUnwrapLong(val));
		break;
	case BGBDT_BASETY_FLOAT:
		dtvArraySetIndexFloat(arr, idx, dtvUnwrapFloat(val));
		break;
	case BGBDT_BASETY_DOUBLE:
		dtvArraySetIndexDouble(arr, idx, dtvUnwrapDouble(val));
		break;
	case BGBDT_BASETY_ADDRESS:
		dtvArraySetIndexDtVal(arr, idx, val);
		break;
	case BGBDT_BASETY_UINT:
		dtvArraySetIndexInt(arr, idx, dtvUnwrapInt(val));
		break;
	case BGBDT_BASETY_UBYTE:
		dtvArraySetIndexByte(arr, idx, dtvUnwrapInt(val));
		break;
	case BGBDT_BASETY_SHORT:
		dtvArraySetIndexShort(arr, idx, dtvUnwrapInt(val));
		break;
	case BGBDT_BASETY_SBYTE:
		dtvArraySetIndexByte(arr, idx, dtvUnwrapInt(val));
		break;
	case BGBDT_BASETY_USHORT:
		dtvArraySetIndexShort(arr, idx, dtvUnwrapInt(val));
		break;
	case BGBDT_BASETY_ULONG:
		dtvArraySetIndexLong(arr, idx, dtvUnwrapLong(val));
		break;
	default:
		break;
	}
}

BTEIFGL_API dtVal BGBDT_TagTy_EncodeRotLong(s64 value)
{
	dtVal val;
	u64 v;
	int i, j;

	//try for an easy case first
	v=(u64)value;
	for(i=0; i<4; i++)
	{
		j=v>>48;
		if((j==0x0000)|(j==0xFFFF))
//		if(!(j&(j+1)))
//			break;
		v=(v>>16)|(v<<48);
	}
	if(i<4)
	{
		val.vi=v&0x0000FFFFFFFFFFFFULL;
		val.hi=val.hi|0x10000000|
			((i*16)<<16)|((j&3)<<22);
		return(val);
	}
	
	//gonna try a little harder
	v=(u64)value;
	for(i=0; i<8; i++)
	{
		j=v>>48;
//		if((j==0x0000)|(j==0xFFFF)|(j==0x5555)|(j==0xAAAA))
//		if((j==0x0000)|(j==0xFFFF)|(j==0x5555)|(j==0x3333))
		if((j==0x0000)|(j==0xFFFF)|(j==0x5555)|(j==0x00FF))
			break;
		v=(v>>8)|(v<<56);
	}
	if(i<8)
	{
//		if(j==0x3333)j=2;
		if(j==0x00FF)j=2;
		val.vi=v&0x0000FFFFFFFFFFFFULL;
		val.hi=val.hi|0x10000000|
			((i*8)<<16)|((j&3)<<22);
		return(val);
	}

	//ok, gonna try extra hard
	v=(u64)value;
	for(i=0; i<64; i++)
	{
		j=v>>48;
//		if((j==0x0000)|(j==0xFFFF)|(j==0x5555)|(j==0xAAAA))
//		if((j==0x0000)|(j==0xFFFF)|(j==0x5555)|(j==0x3333))
		if((j==0x0000)|(j==0xFFFF)|(j==0x5555)|(j==0x00FF))
			break;
		v=(v>>1)|(v<<63);
	}
	if(i<64)
	{
//		if(j==0x3333)j=2;
		if(j==0x00FF)j=2;
		val.vi=v&0x0000FFFFFFFFFFFFULL;
		val.hi=val.hi|0x10000000|
			(i<<16)|((j&3)<<22);
		return(val);
	}
	
	//well then...
	return(DTV_NULL);
}

BTEIFGL_API s64 BGBDT_TagTy_DecodeRotLong(dtVal val)
{
//	static u16 fill[4]={0x0000, 0x5555, 0xAAAA, 0xFFFF};
//	static u16 fill[4]={0x0000, 0x5555, 0x3333, 0xFFFF};
	static u16 fill[4]={0x0000, 0x5555, 0x00FF, 0xFFFF};
	int r;
	u64 v;
	
	v=val.vi&0x0000FFFFFFFFFFFFULL;
	v=v|(((u64)fill[(val.hi>>22)&3])<<48);
	
	r=(val.hi>>16)&63;
	if(r!=0)
		{ v=(v<<r)|(v>>(64-r)); }
	return(v);
}
