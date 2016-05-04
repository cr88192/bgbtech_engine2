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
