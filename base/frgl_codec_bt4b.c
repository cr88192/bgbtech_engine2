/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


int bt4b_decompress_frame(FRGL_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz)
{
	BTIC4B_Context *info;
	int xs, ys;
	
	info=ctx->data;
	xs=info->xs;
	ys=info->ys;

//	return(BTIC4B_DecodeCtx(
//		info, src, dst, ssz, dsz,
//		&xs, &ys, info->clrs));

	return(BTIC4B_DecodeImgBufferCtx(
		info, src, ssz, dst,
		xs, ys, info->clrs));
}

int bt4b_decompress_frame_clrs(FRGL_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
	BTIC4B_Context *info;
	int xs, ys, clrs1;
	
	info=ctx->data;
	xs=info->xs;
	ys=info->ys;

	switch(clrs)
	{
	case BTIC1H_PXF_RGBA: clrs1=BTIC4B_CLRS_RGBA; break;
	case BTIC1H_PXF_BGRA: clrs1=BTIC4B_CLRS_BGRA; break;
	case BTIC1H_PXF_RGBX: clrs1=BTIC4B_CLRS_RGBX; break;
	case BTIC1H_PXF_BGRX: clrs1=BTIC4B_CLRS_BGRX; break;
	case BTIC1H_PXF_RGB: clrs1=BTIC4B_CLRS_RGB; break;
	case BTIC1H_PXF_BGR: clrs1=BTIC4B_CLRS_BGR; break;

	case BTIC1H_PXF_BC1: clrs1=BTIC4B_CLRS_BC1; break;
	case BTIC1H_PXF_BC3: clrs1=BTIC4B_CLRS_BC3; break;
	case BTIC1H_PXF_BC6: clrs1=BTIC4B_CLRS_BC6; break;
	case BTIC1H_PXF_BC7: clrs1=BTIC4B_CLRS_BC7; break;
	default: clrs1=clrs;
	}

//	return(BTIC1H_DecodeCtx(
//		info, src, dst, ssz, dsz,
//		&xs, &ys, clrs));
	return(BTIC4B_DecodeImgBufferCtx(
		info, src, ssz, dst,
		xs, ys, clrs1));
}

#if 0
int bt4b_compress_frame(FRGL_VidCodecCTX *ctx,
	void *src, void *dst, int dsz, int qfl, int clrs, int *rfl)
{
	BTIC4B_Context *info;
	int sz;
	
	info=ctx->data;

	if(clrs==-1)
		{ clrs=info->clrs; }

	sz=BTIC1H_EncodeCtx(info, src, dst, dsz,
		info->xs, info->ys, qfl, clrs);

	if(info->bits_total>=(1<<27))
	{
		BTIC1H_DumpEncodeStats(info);
		BTIC1H_ClearEncodeStats(info);
	}
	
	*rfl=qfl;
	return(sz);
}

int bt4b_end_compress(FRGL_VidCodecCTX *ctx)
{
	BTIC1H_Work_KillWorkers();
	BTIC1H_DestroyContext(ctx->data);
}
#endif

int bt4b_end_decompress(FRGL_VidCodecCTX *ctx)
{
//	BTIC1H_Work_KillWorkers();
//	BTIC4B_DestroyContext(ctx->data);
	BTIC4B_FreeContext(ctx->data);
}

FRGL_VidCodecCTX *bt4b_begin_decompress(int fcc,
	FRGL_BITMAPINFOHEADER *in, FRGL_BITMAPINFOHEADER *out)
{
	FRGL_VidCodecCTX *ctx;
	BTIC4B_Context *info;
	int err;

	if((fcc!=BTIC1H_FCC_bt4b) && (in->biCompression!=BTIC1H_FCC_bt4b))
		return(NULL);

	ctx=FRGL_VidCodecCTX_New();
//	info=gcalloc(sizeof(vfw_ctxinfo));
	info=BTIC4B_AllocContext();
	ctx->data=info;

//	info->width=abs(in->biWidth);
//	info->height=abs(in->biHeight);
	info->xs=abs(in->biWidth);
	info->ys=abs(in->biHeight);

	if(out)
	{
		info->flip=((short)(out->biHeight))<0;

		info->clrs=(out->biBitCount==24)?
			BTIC4B_CLRS_RGB:BTIC4B_CLRS_RGBA;

//		if(out->biCompression==BTIC1H_FCC_BGRA)
//			info->clrs=BTIC4B_CLRS_BGRA;

		if(out->biCompression==BTIC1H_FCC_RGBX)
		{
			info->clrs=(out->biBitCount==24)?
				BTIC4B_CLRS_RGB:BTIC4B_CLRS_RGBX;
		}

		if(out->biCompression==BTIC1H_FCC_RGBA)
		{
			info->clrs=(out->biBitCount==24)?
				BTIC4B_CLRS_RGB:BTIC4B_CLRS_RGBA;
		}

		if(out->biCompression==BTIC1H_FCC_BGRX)
		{
			info->clrs=(out->biBitCount==24)?
				BTIC4B_CLRS_BGR:BTIC4B_CLRS_BGRX;
		}

		if(out->biCompression==BTIC1H_FCC_BGRA)
		{
			info->clrs=(out->biBitCount==24)?
				BTIC4B_CLRS_BGR:BTIC4B_CLRS_BGRA;
		}

#if 0
		if(out->biCompression==BTIC1H_FCC_YUY2)
		{
			info->clrs=BTIC4B_CLRS_YUYV;
//			info->flip=!info->flip;
		}

		if(out->biCompression==BTIC1H_FCC_UYVY)
		{
			info->clrs=BTIC4B_CLRS_UYVY;
//			info->flip=!info->flip;
		}
#endif

		if(out->biCompression==BTIC1H_FCC_DXT1)
			{ info->clrs=BTIC4B_CLRS_BC1; }
		if(out->biCompression==BTIC1H_FCC_DXT5)
			{ info->clrs=BTIC4B_CLRS_BC3; }
		if(out->biCompression==BTIC1H_FCC_BC1)
			{ info->clrs=BTIC4B_CLRS_BC1; }
		if(out->biCompression==BTIC1H_FCC_BC3)
			{ info->clrs=BTIC4B_CLRS_BC3; }
//		if(out->biCompression==BTIC1H_FCC_BC6H)
//			{ info->clrs=BTIC4B_CLRS_BC6H; }
		if(out->biCompression==BTIC1H_FCC_BC7)
			{ info->clrs=BTIC4B_CLRS_BC7; }
	}
	else
		{ info->clrs=BTIC4B_CLRS_RGBA; }

	ctx->decompress_frame=&bt4b_decompress_frame;
	ctx->decompress_frame_clrs=&bt4b_decompress_frame_clrs;
	ctx->end_decompress=bt4b_end_decompress;

	return(ctx);
}

#if 0
FRGL_VidCodecCTX *bt4b_begin_compress(int fcc,
	FRGL_BITMAPINFOHEADER *in, FRGL_BITMAPINFOHEADER *out)
{
	FRGL_VidCodecCTX *ctx;
	BTIC4B_Context *info;
	int err;

	if((fcc!=BTIC1H_FCC_bt1h) && (out->biCompression!=BTIC1H_FCC_bt1h))
		return(NULL);

	ctx=FRGL_VidCodecCTX_New();
	info=BTIC1H_AllocContext();
	ctx->data=info;

	info->xs=abs(in->biWidth);
	info->ys=abs(in->biHeight);
	info->clrs=BTIC4B_CLRS_RGBA;
	info->flip=((short)(in->biHeight))<0;

//	if(in && (in->biBitCount==24))
//		{ info->clrs=BTIC4B_CLRS_RGB; }

	if(in)
	{
		info->clrs=(in->biBitCount==24)?
			BTIC4B_CLRS_RGB:BTIC4B_CLRS_RGBA;

//		if(out->biCompression==BTIC1H_FCC_BGRA)
//			info->clrs=BTIC4B_CLRS_BGRA;

		if(in->biCompression==BTIC1H_FCC_RGBX)
		{
			info->clrs=(in->biBitCount==24)?
				BTIC4B_CLRS_RGB:BTIC4B_CLRS_RGBX;
		}

		if(in->biCompression==BTIC1H_FCC_RGBA)
		{
			info->clrs=(in->biBitCount==24)?
				BTIC4B_CLRS_RGB:BTIC4B_CLRS_RGBA;
		}

		if(in->biCompression==BTIC1H_FCC_BGRX)
		{
			info->clrs=(in->biBitCount==24)?
				BTIC4B_CLRS_BGR:BTIC4B_CLRS_BGRX;
		}

		if(in->biCompression==BTIC1H_FCC_BGRA)
		{
			info->clrs=(in->biBitCount==24)?
				BTIC4B_CLRS_BGR:BTIC4B_CLRS_BGRA;
		}

		if(in->biCompression==BTIC1H_FCC_YUY2)
		{
			info->clrs=BTIC4B_CLRS_YUYV;
			info->flip=!info->flip;
		}

		if(in->biCompression==BTIC1H_FCC_UYVY)
		{
			info->clrs=BTIC4B_CLRS_UYVY;
			info->flip=!info->flip;
		}
	}
	else
		{ info->clrs=BTIC4B_CLRS_RGBA; }

	ctx->compress_frame=&bt4b_compress_frame;
	ctx->end_compress=bt4b_end_compress;

	return(ctx);
}
#endif

int bt4b_decompress_query(int fcc,
	FRGL_BITMAPINFOHEADER *in, FRGL_BITMAPINFOHEADER *out)
{
	int err;

//	if(fcc!=RIFF_TAG_BTIC)
//		return(0);

	if((fcc!=BTIC1H_FCC_bt1h) && (in->biCompression!=BTIC1H_FCC_bt1h))
		return(0);
	return(1);
}

#if 0
int bt4b_compress_query(int fcc,
	FRGL_BITMAPINFOHEADER *in, FRGL_BITMAPINFOHEADER *out)
{
	int err;

//	if(fcc!=RIFF_TAG_BTIC)
//		return(0);

	if((fcc!=BTIC1H_FCC_bt1h) && (out->biCompression!=BTIC1H_FCC_bt1h))
		return(0);
	return(1);
}
#endif

int FRGL_CodecBT4B_Init()
{
	FRGL_VidCodec *codec;
	int i, j;
	short *sw;
	char *t;

	printf("BT4B: Init\n");

	codec=FRGL_VidCodec_New();
	codec->begin_decompress=&bt4b_begin_decompress;
//	codec->begin_compress=&bt4b_begin_compress;

	codec->decompress_query=&bt4b_decompress_query;
//	codec->compress_query=&bt4b_compress_query;
	
	return(0);
}