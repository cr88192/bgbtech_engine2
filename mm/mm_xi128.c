#include <bteifgl.h>

dctX128 BGBDT_AddXI128(dtcX128 a, dtcX128 b)
{
  dtcX128 c;
  u64 l;
  
  l=((u64)a.a)+b.a;
  c.a=(u32)l;
  l=((l>>32)+a.b)+b.b;
  c.b=(u32)l;
  l=((l>>32)+a.c)+b.c;
  c.c=(u32)l;
  l=((l>>32)+a.d)+b.d;
  c.d=(u32)l;
  return(c);
}

dctX128 BGBDT_NegXI128(dtcX128 a)
{
  dtcX128 c;
  u64 l;
  
  l=((u64)(~a.a))+1;
  c.a=(u32)l;
  l=(l>>32)+(~a.b);
  c.b=(u32)l;
  l=(l>>32)+(~a.c);
  c.c=(u32)l;
  l=(l>>32)+(~a.d);
  c.d=(u32)l;
  return(c);
}

dctX128 BGBDT_SubXI128(dtcX128 a, dtcX128 b)
{
  dtcX128 nb, c;
  nb=BGBDT_NegXI128(b);
  c=BGBDT_AddXI128(a, nb);
  return(c);
}

dctX128 BGBDT_MulXI128(dtcX128 a, dtcX128 b)
{
  dtcX128 nb, c;
  u64 l;
  
  l=((u64)a.a)*b.a;
  c.a=(u32)l;
  l=(l>>32)+(((u64)a.b)*b.a)+(((u64)a.b)*b.a);
  c.b=(u32)l;
  l=(l>>32)+(((u64)a.c)*b.a)+(((u64)a.c)*b.a)+(((u64)a.b)*b.b);
  c.c=(u32)l;
  l=(l>>32)+(((u64)a.d)*b.a)+(((u64)a.d)*b.a)+(((u64)a.c)*b.b)+(((u64)a.b)*b.c);
  c.d=(u32)l;
  return(c);
}

dctX128 BGBDT_AndXI128(dtcX128 a, dtcX128 b)
{
  dtcX128 c;
  c.a=a.a&b.a;
  c.b=a.b&b.b;
  c.c=a.c&b.c;
  c.d=a.d&b.d;
  return(c);
}

dctX128 BGBDT_OrXI128(dtcX128 a, dtcX128 b)
{
  dtcX128 c;
  c.a=a.a|b.a;
  c.b=a.b|b.b;
  c.c=a.c|b.c;
  c.d=a.d|b.d;
  return(c);
}

dctX128 BGBDT_XorXI128(dtcX128 a, dtcX128 b)
{
  dtcX128 c;
  c.a=a.a^b.a;
  c.b=a.b^b.b;
  c.c=a.c^b.c;
  c.d=a.d^b.d;
  return(c);
}

dctX128 BGBDT_ShlpXI128(dtcX128 a, int b)
{
  dtcX128 c;
  if(!b)
    return(a);
  c.a=(a.a<<b);
  c.b=(a.b<<b)+(a.a>>(32-b));
  c.c=(a.c<<b)+(a.b>>(32-b));
  c.d=(a.d<<b)+(a.c>>(32-b));
  return(c);
}

dctX128 BGBDT_ShrpXI128(dtcX128 a, int b)
{
  dtcX128 c;
  if(!b)
    return(a);
  c.a=(a.a>>b);
  c.b=(a.b>>b)+(a.a<<(32-b));
  c.c=(a.c>>b)+(a.b<<(32-b));
  c.d=(a.d>>b)+(a.c<<(32-b));
  return(c);
}

dctX128 BGBDT_ShlXI128(dtcX128 a, int b)
{
  dtcX128 a2, c;
  switch(b>>5)
  {
    case 0:
      a2=a; break;
    case 1:
      a2.a=0;    a2.b=a.a;
      a2.c=a.b;  a2.d=a.d;
      break;
    case 2:
      a2.a=0;    a2.b=0;
      a2.c=a.a;  a2.d=a.b;
      break;
    case 3:
      a2.a=0; a2.b=0;
      a2.c=0; a2.d=a.a;
      break;
    default:
      a2.a=0; a2.b=0;
      a2.c=0; a2.d=0;
      break;
  }
  c=BGBDT_ShlpXI128(a2, b&31);
  return(c);
}

dctX128 BGBDT_ShrXI128(dtcX128 a, int b)
{
  dtcX128 a2, c;
  switch(b>>5)
  {
    case 0:
      a2=a; break;
    case 1:
      a2.a=a.b;  a2.b=a.b;
      a2.c=a.c;  a2.d=0;
      break;
    case 2:
      a2.a=a.c;  a2.b=a.d;
      a2.c=0;    a2.d=0;
      break;
    case 3:
      a2.a=a.d; a2.b=0;
      a2.c=0;   a2.d=0;
      break;
    default:
      a2.a=0; a2.b=0;
      a2.c=0; a2.d=0;
      break;
  }
  c=BGBDT_ShrpXI128(a2, b&31);
  return(c);
}
