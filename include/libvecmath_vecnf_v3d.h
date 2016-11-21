#define vec2d(x,y)		_vec2di(x,y)
#define vec3d(x,y,z)	_vec3di(x,y,z)
#define vec4d(x,y,z,w)	_vec4di(x,y,z,w)
#define quatd(w,x,y,z)	_quatdi(w,x,y,z)

#define vec2dvd(v)	_vec2dvd(v)
#define vec3dvd(v)	_vec3dvd(v)
#define vec4dvd(v)	_vec4dvd(v)
#define quatdvd(v)	_quatdvd(v)	//XYZW
#define quatdvd2(v)	_quatdvd2(v)	//WXYZ

#define pdvec2d(v)	_vdvec2(v)
#define pdvec3d(v)	_vdvec4(v)
#define pdvec4d(v)	_vdvec4(v)
#define pdquatd(v)	_vdvec4(v)	//XYZW
#define pdquatd2(v)	_vd2vec4(v)	//WXYZ

#define vdvec2d(p,v)	_vdavec2d(p,v)
#define vdvec3d(p,v)	_vdavec3d(p,v)
#define vdvec4d(p,v)	_vdavec4d(p,v)
#define vdquatd(p,v)	_vdavec4d(p,v)		//XYZW
#define vdquatd2(p,v)	_vda2vec4d(p,v)		//WXYZ

#define v4dshuf(a,sh)	_vd4_shuf(a,sh)

typedef struct {double x,y;} vec2d;
typedef struct {double x,y,z,w;} vec4d;
typedef vec4d vec3d;
typedef vec4d quatd;

static vec2d _vec2di(double x, double y)
	{ vec2d t; t.x=x; t.y=y; return(t); };
static vec3d _vec3di(double x, double y, double z)
	{ vec3d t; t.x=x; t.y=y; t.z=z; return(t); };
static vec4d _vec4di(double x, double y, double z, double w)
	{ vec4d t; t.x=x; t.y=y; t.z=z; t.w=w; return(t); };
static quatd _quatdi(double w, double x, double y, double z)
	{ quatd t; t.x=x; t.y=y; t.z=z; t.w=w; return(t); };

static vec2d _vec2dvd(double *x)
	{ return(_vec2di(x[0], x[1])); };
static vec3d _vec3dvd(double *x)
	{ return(_vec3di(x[0], x[1], x[2])); };
static vec4d _vec4dvd(double *x)
	{ return(_vec4di(x[0], x[1], x[2], x[3])); };
static quatd _quatdvd(double *x)
	{ return(_vec4di(x[3], x[0], x[1], x[2])); };
static quatd _quatdvd2(double *x)
	{ return(_quatdi(x[0], x[1], x[2], x[3])); };

static double *_vdvec2d(vec2d v)
{
	double *p;
	p=(double *)gcralloc(4*sizeof(double));
	p[0]=v.x; p[1]=v.y; p[2]=0; p[3]=0;
	return(p);
}

static double *_vdvec4d(vec4d v)
{
	double *p;
	p=(double *)gcralloc(4*sizeof(double));
	p[0]=v.x; p[1]=v.y; p[2]=v.z; p[3]=v.w;
	return(p);
}

static double *_vd2vec4d(vec4d v)
{
	double *p;
	p=(double *)gcralloc(4*sizeof(double));
	p[0]=v.w; p[1]=v.x; p[2]=v.y; p[3]=v.z;
	return(p);
}

static void _vdavec2d(double *p, vec2d v)
	{ p[0]=v.x; p[1]=v.y; }
static void _vdavec3d(double *p, vec4d v)
	{ p[0]=v.x; p[1]=v.y; p[2]=v.z; }
static void _vdavec4d(double *p, vec4d v)
	{ p[0]=v.x; p[1]=v.y; p[2]=v.z; p[3]=v.w; }
static void _vda2vec4d(double *p, vec4d v)
	{ p[0]=v.w; p[1]=v.x; p[2]=v.y; p[3]=v.z; }

static double _vd4_getidx(vec4d v, int i)
{
	double f;
	switch(i)
	{
	case 0: f=v.x; break; case 1: f=v.y; break;
	case 2: f=v.z; break; case 3: f=v.w; break;
	default: f=0; break;
	}
	return(f);
}

static vec4d _vd4_shuf(vec4d v, int sh)
{
	vec4d b;
	b.x=_vd4_getidx(v, sh&3);
	b.y=_vd4_getidx(v, (sh>>2)&3);
	b.z=_vd4_getidx(v, (sh>>4)&3);
	b.w=_vd4_getidx(v, (sh>>6)&3);
	return(b);
}


static double v2dx(vec2d a)	{ return(a.x); };
static double v2dy(vec2d a)	{ return(a.y); };

static double v3dx(vec3d a)	{ return(a.x); };
static double v3dy(vec3d a)	{ return(a.y); };
static double v3dz(vec3d a)	{ return(a.z); };

static double v4dx(vec4d a)	{ return(a.x); };
static double v4dy(vec4d a)	{ return(a.y); };
static double v4dz(vec4d a)	{ return(a.z); };
static double v4dw(vec4d a)	{ return(a.w); };

static vec2d v2dxy(vec2d a)	{ vec2d t; t.x=a.x; t.y=a.y; return(t); };
static vec2d v2dyx(vec2d a)	{ vec2d t; t.x=a.y; t.y=a.x; return(t); };

static vec2d v3dxy(vec3d a)	{ vec2d t; t.x=a.x; t.y=a.y; return(t); };
static vec2d v3dxz(vec3d a)	{ vec2d t; t.x=a.x; t.y=a.z; return(t); };
static vec2d v3dyx(vec3d a)	{ vec2d t; t.x=a.y; t.y=a.x; return(t); };
static vec2d v3dyz(vec3d a)	{ vec2d t; t.x=a.y; t.y=a.z; return(t); };
static vec2d v3dzx(vec3d a)	{ vec2d t; t.x=a.z; t.y=a.x; return(t); };
static vec2d v3dzy(vec3d a)	{ vec2d t; t.x=a.z; t.y=a.y; return(t); };

static vec3d v3dxyz(vec3d a) { vec3d t; t.x=a.x; t.y=a.y; t.z=a.z; return(t); };
static vec3d v3dxzy(vec3d a) { vec3d t; t.x=a.x; t.y=a.z; t.z=a.y; return(t); };
static vec3d v3dyxz(vec3d a) { vec3d t; t.x=a.y; t.y=a.x; t.z=a.z; return(t); };
static vec3d v3dyzx(vec3d a) { vec3d t; t.x=a.y; t.y=a.z; t.z=a.x; return(t); };
static vec3d v3dzxy(vec3d a) { vec3d t; t.x=a.z; t.y=a.x; t.z=a.y; return(t); };
static vec3d v3dzyx(vec3d a) { vec3d t; t.x=a.z; t.y=a.y; t.z=a.x; return(t); };

static vec2d v4dxy(vec4d a)	{ vec2d t; t.x=a.x; t.y=a.y; return(t); };
static vec2d v4dxz(vec4d a)	{ vec2d t; t.x=a.x; t.y=a.z; return(t); };
static vec2d v4dxw(vec4d a)	{ vec2d t; t.x=a.x; t.y=a.w; return(t); };
static vec2d v4dyx(vec4d a)	{ vec2d t; t.x=a.y; t.y=a.x; return(t); };
static vec2d v4dyz(vec4d a)	{ vec2d t; t.x=a.y; t.y=a.z; return(t); };
static vec2d v4dyw(vec4d a)	{ vec2d t; t.x=a.y; t.y=a.w; return(t); };
static vec2d v4dzx(vec4d a)	{ vec2d t; t.x=a.z; t.y=a.x; return(t); };
static vec2d v4dzy(vec4d a)	{ vec2d t; t.x=a.z; t.y=a.y; return(t); };
static vec2d v4dzw(vec4d a)	{ vec2d t; t.x=a.z; t.y=a.w; return(t); };
static vec2d v4dwx(vec4d a)	{ vec2d t; t.x=a.w; t.y=a.x; return(t); };
static vec2d v4dwy(vec4d a)	{ vec2d t; t.x=a.w; t.y=a.y; return(t); };
static vec2d v4dwz(vec4d a)	{ vec2d t; t.x=a.w; t.y=a.z; return(t); };

static vec3d v4dxyz(vec4d a) { vec3d t; t.x=a.x; t.y=a.y; t.z=a.z; return(t); };
static vec3d v4dxyw(vec4d a) { vec3d t; t.x=a.x; t.y=a.y; t.z=a.w; return(t); };
static vec3d v4dxzy(vec4d a) { vec3d t; t.x=a.x; t.y=a.z; t.z=a.y; return(t); };
static vec3d v4dxzw(vec4d a) { vec3d t; t.x=a.x; t.y=a.z; t.z=a.w; return(t); };
static vec3d v4dxwy(vec4d a) { vec3d t; t.x=a.x; t.y=a.w; t.z=a.y; return(t); };
static vec3d v4dxwz(vec4d a) { vec3d t; t.x=a.x; t.y=a.w; t.z=a.z; return(t); };
static vec3d v4dyxz(vec4d a) { vec3d t; t.x=a.y; t.y=a.x; t.z=a.z; return(t); };
static vec3d v4dyxw(vec4d a) { vec3d t; t.x=a.y; t.y=a.x; t.z=a.w; return(t); };
static vec3d v4dyzx(vec4d a) { vec3d t; t.x=a.y; t.y=a.z; t.z=a.x; return(t); };
static vec3d v4dyzw(vec4d a) { vec3d t; t.x=a.y; t.y=a.z; t.z=a.w; return(t); };
static vec3d v4dywx(vec4d a) { vec3d t; t.x=a.y; t.y=a.w; t.z=a.x; return(t); };
static vec3d v4dywz(vec4d a) { vec3d t; t.x=a.y; t.y=a.w; t.z=a.z; return(t); };
static vec3d v4dzxy(vec4d a) { vec3d t; t.x=a.z; t.y=a.x; t.z=a.y; return(t); };
static vec3d v4dzxw(vec4d a) { vec3d t; t.x=a.z; t.y=a.x; t.z=a.w; return(t); };
static vec3d v4dzyx(vec4d a) { vec3d t; t.x=a.z; t.y=a.y; t.z=a.x; return(t); };
static vec3d v4dzyw(vec4d a) { vec3d t; t.x=a.z; t.y=a.y; t.z=a.w; return(t); };
static vec3d v4dzwx(vec4d a) { vec3d t; t.x=a.z; t.y=a.w; t.z=a.x; return(t); };
static vec3d v4dzwy(vec4d a) { vec3d t; t.x=a.z; t.y=a.w; t.z=a.y; return(t); };
static vec3d v4dwxy(vec4d a) { vec3d t; t.x=a.w; t.y=a.x; t.z=a.y; return(t); };
static vec3d v4dwxz(vec4d a) { vec3d t; t.x=a.w; t.y=a.x; t.z=a.z; return(t); };
static vec3d v4dwyx(vec4d a) { vec3d t; t.x=a.w; t.y=a.y; t.z=a.x; return(t); };
static vec3d v4dwyz(vec4d a) { vec3d t; t.x=a.w; t.y=a.y; t.z=a.z; return(t); };
static vec3d v4dwzx(vec4d a) { vec3d t; t.x=a.w; t.y=a.z; t.z=a.x; return(t); };
static vec3d v4dwzy(vec4d a) { vec3d t; t.x=a.w; t.y=a.z; t.z=a.y; return(t); };

static vec4d v4dxyzw(vec4d a) { vec4d t; t.x=a.x; t.y=a.y; t.z=a.z; t.w=a.w; return(t); };
static vec4d v4dxywz(vec4d a) { vec4d t; t.x=a.x; t.y=a.y; t.z=a.w; t.w=a.z; return(t); };
static vec4d v4dxzyw(vec4d a) { vec4d t; t.x=a.x; t.y=a.z; t.z=a.y; t.w=a.w; return(t); };
static vec4d v4dxzwy(vec4d a) { vec4d t; t.x=a.x; t.y=a.z; t.z=a.w; t.w=a.y; return(t); };
static vec4d v4dxwyz(vec4d a) { vec4d t; t.x=a.x; t.y=a.w; t.z=a.y; t.w=a.z; return(t); };
static vec4d v4dxwzy(vec4d a) { vec4d t; t.x=a.x; t.y=a.w; t.z=a.z; t.w=a.y; return(t); };
static vec4d v4dyxzw(vec4d a) { vec4d t; t.x=a.y; t.y=a.x; t.z=a.z; t.w=a.w; return(t); };
static vec4d v4dyxwz(vec4d a) { vec4d t; t.x=a.y; t.y=a.x; t.z=a.w; t.w=a.z; return(t); };
static vec4d v4dyzxw(vec4d a) { vec4d t; t.x=a.y; t.y=a.z; t.z=a.x; t.w=a.w; return(t); };
static vec4d v4dyzwx(vec4d a) { vec4d t; t.x=a.y; t.y=a.z; t.z=a.w; t.w=a.x; return(t); };
static vec4d v4dywxz(vec4d a) { vec4d t; t.x=a.y; t.y=a.w; t.z=a.x; t.w=a.z; return(t); };
static vec4d v4dywzx(vec4d a) { vec4d t; t.x=a.y; t.y=a.w; t.z=a.z; t.w=a.x; return(t); };
static vec4d v4dzxyw(vec4d a) { vec4d t; t.x=a.z; t.y=a.x; t.z=a.y; t.w=a.w; return(t); };
static vec4d v4dzxwy(vec4d a) { vec4d t; t.x=a.z; t.y=a.x; t.z=a.w; t.w=a.y; return(t); };
static vec4d v4dzyxw(vec4d a) { vec4d t; t.x=a.z; t.y=a.y; t.z=a.x; t.w=a.w; return(t); };
static vec4d v4dzywx(vec4d a) { vec4d t; t.x=a.z; t.y=a.y; t.z=a.w; t.w=a.x; return(t); };
static vec4d v4dzwxy(vec4d a) { vec4d t; t.x=a.z; t.y=a.w; t.z=a.x; t.w=a.y; return(t); };
static vec4d v4dzwyx(vec4d a) { vec4d t; t.x=a.z; t.y=a.w; t.z=a.y; t.w=a.x; return(t); };
static vec4d v4dwxyz(vec4d a) { vec4d t; t.x=a.w; t.y=a.x; t.z=a.y; t.w=a.z; return(t); };
static vec4d v4dwxzy(vec4d a) { vec4d t; t.x=a.w; t.y=a.x; t.z=a.z; t.w=a.y; return(t); };
static vec4d v4dwyxz(vec4d a) { vec4d t; t.x=a.w; t.y=a.y; t.z=a.x; t.w=a.z; return(t); };
static vec4d v4dwyzx(vec4d a) { vec4d t; t.x=a.w; t.y=a.y; t.z=a.z; t.w=a.x; return(t); };
static vec4d v4dwzxy(vec4d a) { vec4d t; t.x=a.w; t.y=a.z; t.z=a.x; t.w=a.y; return(t); };
static vec4d v4dwzyx(vec4d a) { vec4d t; t.x=a.w; t.y=a.z; t.z=a.y; t.w=a.x; return(t); };

static double qdx(vec4d a)	{ return(a.x); };
static double qdy(vec4d a)	{ return(a.y); };
static double qdz(vec4d a)	{ return(a.z); };
static double qdw(vec4d a)	{ return(a.w); };

static double qdi(vec4d a)	{ return(a.x); };
static double qdj(vec4d a)	{ return(a.y); };
static double qdk(vec4d a)	{ return(a.z); };
static double qdr(vec4d a)	{ return(a.w); };

static double qdreal(vec4d a)	{ return(a.w); };

static quatd qdvec(vec4d a)	{ a.w=0; return(a); };


static double v2ddot(vec2d a, vec2d b)
	{ return(a.x*b.x+a.y*b.y); }
static double v3ddot(vec3d a, vec3d b)
	{ return(a.x*b.x+a.y*b.y+a.z*b.z); }
static double v4ddot(vec4d a, vec4d b)
	{ return(a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w); }
static double qddot(quatd a, quatd b)
	{ return(a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w); }

static double v2dcross(vec2d a, vec2d b)
	{ return(a.x*b.y-a.y*b.x); }

static vec3d v3dcross(vec3d a, vec3d b)
{
	vec3d c;
	c.x=(a.y*b.z)-(a.z*b.y);
	c.y=(a.z*b.x)-(a.x*b.z);
	c.z=(a.x*b.y)-(a.y*b.x);
	return(c);
}

static vec4d v4dcross(vec4d a, vec4d b)
{
	vec4d c;
	c.x=(a.y*b.z)-(a.z*b.y);
	c.y=(a.z*b.x)-(a.x*b.z);
	c.z=(a.x*b.y)-(a.y*b.x);
	c.w=0;
	return(c);
}

static vec4d qdcross(quatd a, quatd b)
	{ return(v4dcross(a, b)); }

static double v2dlen(vec2d a)
	{ return(sqrt(a.x*a.x+a.y*a.y)); }
static double v3dlen(vec3d a)
	{ return(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)); }
static double v4dlen(vec4d a)
	{ return(sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w)); }
static double qdlen(quatd a)
	{ return(sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w)); }

static vec2d v2dnorm(vec2d a)
{	vec2d c; double l;
	l=v2dlen(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l;
	return(c); }
static vec3d v3dnorm(vec3d a)
{	vec3d c; double l;
	l=v3dlen(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l; c.z=a.z*l;
	return(c); }
static vec4d v4dnorm(vec4d a)
{	vec4d c; double l;
	l=v4dlen(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l; c.z=a.z*l; c.w=a.w*l;
	return(c); }
static quatd qdnorm(quatd a)
{	vec4d c; double l;
	l=v4dlen(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l; c.z=a.z*l; c.w=a.w*l;
	return(c); }

static vec2d v2dneg(vec2d a)
	{ vec2d c; c.x=-a.x; c.y=-a.y; return(c); }
static vec3d v3dneg(vec3d a)
	{ vec3d c; c.x=-a.x; c.y=-a.y; c.z=-a.z; return(c); }
static vec4d v4dneg(vec4d a)
	{ vec4d c; c.x=-a.x; c.y=-a.y; c.z=-a.z; c.w=-a.w; return(c); }

static quatd qdneg(quatd a)
	{ quatd c; c.x=-a.x; c.y=-a.y; c.z=-a.z; c.w=-a.w; return(c); }
static quatd qdconj(quatd a)
	{ quatd c; c.x=-a.x; c.y=-a.y; c.z=-a.z; c.w=a.w; return(c); }

static quatd qdrcp(quatd a)
{
	quatd c; double f;
	f=(a.w*a.w)+(a.x*a.x)+(a.y*a.y)+(a.z*a.z);
	c.x=-a.x/f; c.y=-a.y/f; c.z=-a.z/f; c.w=a.w/f;
	return(c);
}


static vec2d v2dadd(vec2d a, vec2d b)
	{ vec2d c; c.x=a.x+b.x; c.y=a.y+b.y; return(c); }
static vec2d v2dsub(vec2d a, vec2d b)
	{ vec2d c; c.x=a.x-b.x; c.y=a.y-b.y; return(c); }
static vec2d v2dmul(vec2d a, vec2d b)
	{ vec2d c; c.x=a.x*b.x; c.y=a.y*b.y; return(c); }
static vec2d v2ddiv(vec2d a, vec2d b)
	{ vec2d c; c.x=a.x/b.x; c.y=a.y/b.y; return(c); }

static vec3d v3dadd(vec3d a, vec3d b)
	{ vec3d c; c.x=a.x+b.x; c.y=a.y+b.y; c.z=a.z+b.z; return(c); }
static vec3d v3dsub(vec3d a, vec3d b)
	{ vec3d c; c.x=a.x-b.x; c.y=a.y-b.y; c.z=a.z-b.z; return(c); }
static vec3d v3dmul(vec3d a, vec3d b)
	{ vec3d c; c.x=a.x*b.x; c.y=a.y*b.y; c.z=a.z*b.z; return(c); }
static vec3d v3ddiv(vec3d a, vec3d b)
	{ vec3d c; c.x=a.x/b.x; c.y=a.y/b.y; c.z=a.z/b.z; return(c); }

static vec4d v4dadd(vec4d a, vec4d b)
	{vec4d c;c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;c.w=a.w+b.w;return(c);}
static vec4d v4dsub(vec4d a, vec4d b)
	{vec4d c;c.x=a.x-b.x;c.y=a.y-b.y;c.z=a.z-b.z;c.w=a.w-b.w;return(c);}
static vec4d v4dmul(vec4d a, vec4d b)
	{vec4d c;c.x=a.x*b.x;c.y=a.y*b.y;c.z=a.z*b.z;c.w=a.w*b.w;return(c);}
static vec4d v4ddiv(vec4d a, vec4d b)
	{vec4d c;c.x=a.x/b.x;c.y=a.y/b.y;c.z=a.z/b.z;c.w=a.w/b.w;return(c);}

static quatd qdadd(quatd a, quatd b)
	{vec4d c;c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;c.w=a.w+b.w;return(c);}
static quatd qdsub(quatd a, quatd b)
	{vec4d c;c.x=a.x-b.x;c.y=a.y-b.y;c.z=a.z-b.z;c.w=a.w-b.w;return(c);}

static quatd qdmul(quatd a, quatd b)
{
	quatd c;
	c.x=(a.w*b.x)+(a.x*b.w)+(a.y*b.z)-(a.z*b.y);
	c.y=(a.w*b.y)+(a.y*b.w)+(a.z*b.x)-(a.x*b.z);
	c.z=(a.w*b.z)+(a.z*b.w)+(a.x*b.y)-(a.y*b.x);
	c.w=(a.w*b.w)-(a.x*b.x)-(a.y*b.y)-(a.z*b.z);
	return(c);
}

static quatd qddiv(quatd a, quatd b)
	{ return(qdmul(a, qdrcp(b))); }
static quatd qdrdiv(quatd a, quatd b)
	{ return(qdmul(qdrcp(b), a)); }


static double v2ddist(vec2d a, vec2d b) { return(v2dlen(v2dsub(a, b))); }
static double v3ddist(vec3d a, vec3d b) { return(v3dlen(v3dsub(a, b))); }
static double v4ddist(vec4d a, vec4d b) { return(v4dlen(v4dsub(a, b))); }
static double qddist(quatd a, quatd b) { return(qdlen(qdsub(a, b))); }

static vec2d v2dscale(vec2d a, double f)
	{ vec2d c; c.x=a.x*f; c.y=a.y*f; return(c); }
static vec3d v3dscale(vec3d a, double f)
	{ vec3d c; c.x=a.x*f; c.y=a.y*f; c.z=a.z*f; return(c); }
static vec4d v4dscale(vec4d a, double f)
	{ vec4d c; c.x=a.x*f; c.y=a.y*f; c.z=a.z*f; c.w=a.w*f; return(c); }
static quatd qdscale(quatd a, double f)
	{ quatd c; c.x=a.x*f; c.y=a.y*f; c.z=a.z*f; c.w=a.w*f; return(c); }


static int v2deqp(vec2d a, vec2d b)
	{ return((a.x==b.x) && (a.y==b.y)); }
static int v3deqp(vec3d a, vec3d b)
	{ return((a.x==b.x) && (a.y==b.y) && (a.z==b.z)); }
static int v4deqp(vec4d a, vec4d b)
	{ return((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)); }
static int qdeqp(quatd a, quatd b)
	{ return((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)); }

static int v2dneqp(vec2d a, vec2d b)
	{ return((a.x!=b.x) || (a.y!=b.y)); }
static int v3dneqp(vec3d a, vec3d b)
	{ return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z)); }
static int v4dneqp(vec4d a, vec4d b)
	{ return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) || (a.w!=b.w)); }
static int qdneqp(quatd a, quatd b)
	{ return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) || (a.w!=b.w)); }


static vec2d v2daddscale(vec2d a, vec2d b, double f)
	{ return(v2dadd(a, v2dmul(b, vec2d(f, f)))); }
static vec3d v3daddscale(vec3d a, vec3d b, double f)
	{ return(v3dadd(a, v3dmul(b, vec3d(f, f, f)))); }
static vec4d v4daddscale(vec4d a, vec4d b, double f)
	{ return(v4dadd(a, v4dmul(b, vec4d(f, f, f, f)))); }
static quatd qdaddscale(quatd a, quatd b, double f)
	{ return(v4dadd(a, v4dmul(b, vec4d(f, f, f, f)))); }

static vec2d v2dscaleaddscale(vec2d a, double f, vec2d b, double g)
	{ return(v2dadd(v2dmul(a, vec2d(f, f)),
		v2dmul(b, vec2d(g, g)))); }
static vec3d v3dscaleaddscale(vec3d a, double f, vec3d b, double g)
	{ return(v3dadd(v3dmul(a, vec3d(f, f, f)),
		v3dmul(b, vec3d(g, g, g)))); }
static vec4d v4dscaleaddscale(vec4d a, double f, vec4d b, double g)
	{ return(v4dadd(v4dmul(a, vec4d(f, f, f, f)),
		v4dmul(b, vec4d(g, g, g, g)))); }
static quatd qdscaleaddscale(quatd a, double f, quatd b, double g)
	{ return(v4dadd(v4dmul(a, vec4d(f, f, f, f)),
		v4dmul(b, vec4d(g, g, g, g)))); }

static vec2d v2dscaleadd2(vec2d a, double f, vec2d b, double g)
	{ return(v2dadd(v2dmul(a, vec2d(f, f)),
		v2dmul(b, vec2d(g, g)))); }
static vec3d v3dscaleadd2(vec3d a, double f, vec3d b, double g)
	{ return(v3dadd(v3dmul(a, vec3d(f, f, f)),
		v3dmul(b, vec3d(g, g, g)))); }
static vec4d v4dscaleadd2(vec4d a, double f, vec4d b, double g)
	{ return(v4dadd(v4dmul(a, vec4d(f, f, f, f)),
		v4dmul(b, vec4d(g, g, g, g)))); }
static quatd qdscaleadd2(quatd a, double f, quatd b, double g)
	{ return(v4dadd(v4dmul(a, vec4d(f, f, f, f)),
		v4dmul(b, vec4d(g, g, g, g)))); }

static vec2d v2dscaleadd3(vec2d a, double f,
	vec2d b, double g, vec2d c, double h)
{
	return(v2dadd(
		v2dmul(a, vec2d(f, f)),
		v2dadd(	v2dmul(b, vec2d(g, g)),
			v2dmul(c, vec2d(h, h)))
		));
}

static vec3d v3dscaleadd3(vec3d a, double f,
	vec3d b, double g, vec3d c, double h)
{
	return(v3dadd(
		v3dmul(a, vec3d(f, f, f)),
		v3dadd(	v3dmul(b, vec3d(g, g, g)),
			v3dmul(c, vec3d(h, h, h)))
		));
}
static vec4d v4dscaleadd3(vec4d a, double f,
	vec4d b, double g, vec4d c, double h)
{
	return(v4dadd(
		v4dmul(a, vec4d(f, f, f, f)),
		v4dadd(	v4dmul(b, vec4d(g, g, g, g)),
			v4dmul(c, vec4d(h, h, h, h)))
		));
}

static quatd qdscaleadd3(quatd a, double f,
	quatd b, double g, quatd c, double h)
{
	return(v4dadd(
		v4dmul(a, vec4d(f, f, f, f)),
		v4dadd(	v4dmul(b, vec4d(g, g, g, g)),
			v4dmul(c, vec4d(h, h, h, h)))
		));
}

static vec2d v2dscaleadd4(vec2d a, double f,
	vec2d b, double g, vec2d c, double h, vec2d d, double i)
{
	return(v2dadd(
		v2dadd(	v2dmul(a, vec2d(f, f)),
			v2dmul(b, vec2d(g, g))),
		v2dadd(	v2dmul(c, vec2d(h, h)),
			v2dmul(d, vec2d(i, i)))
		));
}

static vec3d v3dscaleadd4(vec3d a, double f,
	vec3d b, double g, vec3d c, double h, vec3d d, double i)
{
	return(v3dadd(
		v3dadd(	v3dmul(a, vec3d(f, f, f)),
			v3dmul(b, vec3d(g, g, g))),
		v3dadd(	v3dmul(c, vec3d(h, h, h)),
			v3dmul(d, vec3d(i, i, i)))
		));
}

static vec4d v4dscaleadd4(vec4d a, double f,
	vec4d b, double g, vec4d c, double h, vec4d d, double i)
{
	return(v4dadd(
		v4dadd(	v4dmul(a, vec4d(f, f, f, f)),
			v4dmul(b, vec4d(g, g, g, g))),
		v4dadd(	v4dmul(c, vec4d(h, h, h, h)),
			v4dmul(d, vec4d(i, i, i, i)))
		));
}

static quatd qdscaleadd4(quatd a, double f,
	quatd b, double g, quatd c, double h, quatd d, double i)
{
	return(v4dadd(
		v4dadd(	v4dmul(a, vec4d(f, f, f, f)),
			v4dmul(b, vec4d(g, g, g, g))),
		v4dadd(	v4dmul(c, vec4d(h, h, h, h)),
			v4dmul(d, vec4d(i, i, i, i)))
		));
}
