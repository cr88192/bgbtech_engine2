/*
Copyright (C) 2015-2016 by Brendan G Bohannon

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


BSVM2_Trace *BSVM2_TrRun_NULL(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	return(NULL);
}

BSVM2_Trace *BSVM2_TrRun_Def0(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def1(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode *op;
	op=tr->ops[0]; op->Run(frm, op);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def2(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def3(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def4(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def5(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def6(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def7(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_Def8(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	if((ops+4)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	if((ops+1)<opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	if(ops<opse)
		{ (*ops)->Run(frm, *ops); }
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN0(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN1(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN2(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN3(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN4(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN5(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN6(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_DefN7(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->top->Run(frm, tr->top));
}

BSVM2_Trace *BSVM2_TrRun_ThrowN(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+4)<=opse)
	{
		(*ops)->Run(frm, *ops); ops++;
		if(frm->ctx->status)goto exth;
		(*ops)->Run(frm, *ops); ops++;
		if(frm->ctx->status)goto exth;
		(*ops)->Run(frm, *ops); ops++;
		if(frm->ctx->status)goto exth;
		(*ops)->Run(frm, *ops); ops++;
		if(frm->ctx->status)goto exth;
	}
	if((ops+1)<opse)
	{
		(*ops)->Run(frm, *ops); ops++;
		if(frm->ctx->status)goto exth;
		(*ops)->Run(frm, *ops); ops++;
		if(frm->ctx->status)goto exth;
	}
	if(ops<opse)
	{
		(*ops)->Run(frm, *ops);
		if(frm->ctx->status)goto exth;
	}
	return(tr->top->Run(frm, tr->top));
	exth:
	frm->ctx->trace=tr;
	return(BSVM2_TrOp_ThrowStatusI(frm, frm->ctx->status));
//	return(NULL);
}

BSVM2_Trace *BSVM2_TrRun_Next0(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next1(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode *op;
	op=tr->ops[0]; op->Run(frm, op);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next2(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next3(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next4(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next5(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next6(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next7(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_Next8(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops;
	
	ops=tr->ops;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

#if 1
BSVM2_Trace *BSVM2_TrRun_NextN0(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_NextN1(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_NextN2(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_NextN3(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_NextN4(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_NextN5(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_NextN6(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}

BSVM2_Trace *BSVM2_TrRun_NextN7(BSVM2_Frame *frm, BSVM2_Trace *tr)
{
	BSVM2_Opcode **ops, **opse;
	
	ops=tr->ops; opse=ops+tr->n_ops;
	while((ops+8)<=opse)
	{	(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;
		(*ops)->Run(frm, *ops); ops++;	(*ops)->Run(frm, *ops); ops++;	}
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops); ops++;
	(*ops)->Run(frm, *ops);
	return(tr->jnext);
}
#endif

BSVM2_Trace *BSVM2_Interp_DecodeBlockSetupTraceDfl(
	BSVM2_CodeBlock *cblk, BSVM2_Opcode **ops, int n_ops,
	BSVM2_TailOpcode *top, int flag)
{
	BSVM2_Trace *tr;
	int i;

	for(i=0; i<n_ops; i++)
	{
		if(ops[i]->opfl&BSVM2_TRFL_CANTHROW)
			flag|=BSVM2_TRFL_CANTHROW;
//		tr->ops[i]=ops[i];
	}

	tr=BSVM2_Interp_AllocTrace(cblk);
	tr->trfl=flag;

#if 0
	if((n_ops>8) && !top)
	{
		top=BSVM2_Interp_AllocTailOpcode(cblk);
//		top->Run=BSVM2_TrOp_Default;
		top->Run=BSVM2_TrOp_JMP;
	}
#endif

	if(n_ops>6)
	{
		tr->ops=dtmAlloc("bsvm2_trops_t",
			n_ops*sizeof(BSVM2_Opcode *));
		tr->n_ops=n_ops;
		for(i=0; i<n_ops; i++)
			tr->ops[i]=ops[i];
		tr->top=top;
	}else
	{
		tr->ops=(BSVM2_Opcode **)(tr->t_ops);
		tr->n_ops=n_ops;
		for(i=0; i<n_ops; i++)
			tr->ops[i]=ops[i];
		tr->top=top;
	}
	
	if(flag&BSVM2_TRFL_CANTHROW)
	{
		tr->Run=BSVM2_TrRun_ThrowN;
	}else if(n_ops>8)
	{
		if(flag&1)
		{
			switch(n_ops&7)
			{
			case 0: tr->Run=BSVM2_TrRun_NextN0; break;
			case 1: tr->Run=BSVM2_TrRun_NextN1; break;
			case 2: tr->Run=BSVM2_TrRun_NextN2; break;
			case 3: tr->Run=BSVM2_TrRun_NextN3; break;
			case 4: tr->Run=BSVM2_TrRun_NextN4; break;
			case 5: tr->Run=BSVM2_TrRun_NextN5; break;
			case 6: tr->Run=BSVM2_TrRun_NextN6; break;
			case 7: tr->Run=BSVM2_TrRun_NextN7; break;
			default: break;
			}
		}else
		{
			switch(n_ops&7)
			{
			case 0: tr->Run=BSVM2_TrRun_DefN0; break;
			case 1: tr->Run=BSVM2_TrRun_DefN1; break;
			case 2: tr->Run=BSVM2_TrRun_DefN2; break;
			case 3: tr->Run=BSVM2_TrRun_DefN3; break;
			case 4: tr->Run=BSVM2_TrRun_DefN4; break;
			case 5: tr->Run=BSVM2_TrRun_DefN5; break;
			case 6: tr->Run=BSVM2_TrRun_DefN6; break;
			case 7: tr->Run=BSVM2_TrRun_DefN7; break;
//			default: tr->Run=BSVM2_TrRun_DefN; break;
			}
		}
	}else if(flag&1)
	{
		switch(n_ops)
		{
		case 0: tr->Run=BSVM2_TrRun_Next0; break;
		case 1: tr->Run=BSVM2_TrRun_Next1; break;
		case 2: tr->Run=BSVM2_TrRun_Next2; break;
		case 3: tr->Run=BSVM2_TrRun_Next3; break;
		case 4: tr->Run=BSVM2_TrRun_Next4; break;
		case 5: tr->Run=BSVM2_TrRun_Next5; break;
		case 6: tr->Run=BSVM2_TrRun_Next6; break;
		case 7: tr->Run=BSVM2_TrRun_Next7; break;
		case 8: tr->Run=BSVM2_TrRun_Next8; break;
		default: break;
		}
	}else
	{
		switch(n_ops)
		{
		case 0: tr->Run=BSVM2_TrRun_Def0; break;
		case 1: tr->Run=BSVM2_TrRun_Def1; break;
		case 2: tr->Run=BSVM2_TrRun_Def2; break;
		case 3: tr->Run=BSVM2_TrRun_Def3; break;
		case 4: tr->Run=BSVM2_TrRun_Def4; break;
		case 5: tr->Run=BSVM2_TrRun_Def5; break;
		case 6: tr->Run=BSVM2_TrRun_Def6; break;
		case 7: tr->Run=BSVM2_TrRun_Def7; break;
		case 8: tr->Run=BSVM2_TrRun_Def8; break;
		default: break;
		}
	}
	
//#ifdef BS2I_USE_BASM
//	BS2J_CheckSetupJitTrace(tr);
//#endif
	
	return(tr);
}



BS2VM_API BSVM2_Trace *BSVM2_TrOp_Default(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JMP(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	return(op->jmptrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JEQ(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(!frm->stack[op->t0].i)
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JNE(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i)
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JLT(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i<0)
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JGT(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i>0)
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JLE(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i<=0)
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JGE(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i>=0)
		return(op->jmptrace);
	return(op->nexttrace);
}


BS2VM_API BSVM2_Trace *BSVM2_TrOp_JEQNULL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JNENULL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(!dtvNullP(frm->stack[op->t0].a))
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JEQNULLL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(dtvNullP(frm->local[op->i0].a))
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JNENULLL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(!dtvNullP(frm->local[op->i0].a))
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_EQI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i==frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_NEI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i!=frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i<frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i>frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LEI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i<=frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GEI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i>=frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui<frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui>frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LEUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui<=frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GEUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui>=frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}


BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_EQL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l==frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_NEL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l!=frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l<frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l>frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LEL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l<=frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GEL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l>=frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTUL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul<frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTUL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul>frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LEUL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul<=frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GEUL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul>=frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}


BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_EQF(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f==frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_NEF(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f!=frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTF(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f<frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTF(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f>frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LEF(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f<=frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GEF(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f>=frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_EQD(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d==frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_NED(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d!=frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTD(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d<frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTD(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d>frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LED(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d<=frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GED(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d>=frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}


#if 1
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_EQIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i==op->v.i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_NEIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i!=op->v.i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i<op->v.i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i>op->v.i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LEIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i<=op->v.i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GEIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i>=op->v.i)
		return(op->jmptrace);
	return(op->nexttrace);	}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LTUIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui<op->v.ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GTUIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui>op->v.ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_LEUIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui<=op->v.ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMP_GEUIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui>=op->v.ui)
		return(op->jmptrace);
	return(op->nexttrace);	}

#endif



BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_EQI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].i==frm->local[op->i1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_NEI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].i!=frm->local[op->i1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_LTI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].i<frm->local[op->i1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_GTI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].i>frm->local[op->i1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_LEI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].i<=frm->local[op->i1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_GEI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].i>=frm->local[op->i1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_LTUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].ui<frm->local[op->i1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_GTUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].ui>frm->local[op->i1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_LEUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].ui<=frm->local[op->i1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BS2VM_API BSVM2_Trace *BSVM2_TrOp_JCMPLL_GEUI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->local[op->i0].ui>=frm->local[op->i1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}


BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETI(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;

	ctx=frm->ctx;
	frmb=frm->rnext;

	frmb->stack[frmb->rcsrv].i=frm->stack[op->t0].i;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].l=frm->stack[op->t0].l;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETF(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].f=frm->stack[op->t0].f;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETD(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].d=frm->stack[op->t0].d;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETA(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].a=frm->stack[op->t0].a;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETV(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}


BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETIC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;

	ctx=frm->ctx;
	frmb=frm->rnext;

	frmb->stack[frmb->rcsrv].i=op->v.i;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETLC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;

	ctx=frm->ctx;
	frmb=frm->rnext;

	frmb->stack[frmb->rcsrv].l=op->v.l;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETFC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;

	ctx=frm->ctx;
	frmb=frm->rnext;

	frmb->stack[frmb->rcsrv].f=op->v.f;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETDC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;

	ctx=frm->ctx;
	frmb=frm->rnext;

	frmb->stack[frmb->rcsrv].d=op->v.d;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETAC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;

	ctx=frm->ctx;
	frmb=frm->rnext;

	frmb->stack[frmb->rcsrv].a=op->v.a;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETIL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;

	ctx=frm->ctx;
	frmb=frm->rnext;

	frmb->stack[frmb->rcsrv].i=frm->local[op->i0].i;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETLL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].l=frm->local[op->i0].l;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETFL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].f=frm->local[op->i0].f;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETDL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].d=frm->local[op->i0].d;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETAL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	ctx=frm->ctx;	frmb=frm->rnext;
	frmb->stack[frmb->rcsrv].a=frm->local[op->i0].a;
	frm->rnext=ctx->freeframe;	ctx->freeframe=frm;
	ctx->frame=frmb;
	ctx->tstackref=frmb->tstkpos;
	return(frmb->rtrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_CALLG(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	BSVM2_ImageGlobal *vi;
	BSVM2_Trace *tr;
	BSVM2_Value *va, *vb;
	int i, j, k;
	
	ctx=frm->ctx;
	vi=op->v.p;
	vi->runcnt++;
	
	frmb=BSVM2_Interp_AllocFrame(ctx);

//	frmb->stack=ctx->tstack+ctx->tstackref;
//	frmb->local=frmb->stack+vi->cblk->stkdepth;
	frmb->local=ctx->tstack+ctx->tstackref;
	frmb->stack=frmb->local+vi->cblk->largs;

	ctx->tstackref=ctx->tstackref+vi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;

	frmb->rnext=frm;
	frm->rtrace=op->nexttrace;
	frm->rcsrv=op->t1;
	ctx->frame=frmb;

#if 0
	if(vi->nargs>1)
	{
		for(i=0; i<vi->nargs; i++)
		{
			frmb->local[vi->cblk->bargs+i]=
				frm->stack[op->t1+i];
		}
	}else if(vi->nargs)
	{
		frmb->local[vi->cblk->bargs]=
			frm->stack[op->t1];
	}
#endif

#if 1
	if(vi->nargs>1)
	{
		j=vi->cblk->bargs; k=op->t1;
//		va=frmb->local+j;
//		vb=frm->stack+k;
		switch(vi->nargs)
		{
#if 0
		case 8:	va[7]=vb[7];
		case 7:	va[6]=vb[6];
		case 6:	va[5]=vb[5];
		case 5:	va[4]=vb[4];
		case 4:	va[3]=vb[3];
		case 3:	va[2]=vb[2];
		case 2:	va[1]=vb[1];
		case 1:	va[0]=vb[0];
#endif

#if 1
		case 8:	frmb->local[j+7]=frm->stack[k+7];
		case 7:	frmb->local[j+6]=frm->stack[k+6];
		case 6:	frmb->local[j+5]=frm->stack[k+5];
		case 5:	frmb->local[j+4]=frm->stack[k+4];
		case 4:	frmb->local[j+3]=frm->stack[k+3];
		case 3:	frmb->local[j+2]=frm->stack[k+2];
		case 2:	frmb->local[j+1]=frm->stack[k+1];
		case 1:	frmb->local[j  ]=frm->stack[k  ];
#endif
		case 0:
			break;
		default:
			for(i=0; i<vi->nargs; i++)
			{
				frmb->local[vi->cblk->bargs+i]=
					frm->stack[op->t1+i];
			}
			break;
		}
	}else if(vi->nargs)
	{
		frmb->local[vi->cblk->bargs]=
			frm->stack[op->t1];
	}
#endif

	tr=BS2I_ImageGetFuncTrace(vi);
	return(tr);
}


BS2VM_API BSVM2_Trace *BSVM2_TrOp_CALLV(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	BSVM2_ImageGlobal *vi, *fvi;
	BSVM2_Trace *tr;
	dtVal obj, fcn;
	int i;
	
	ctx=frm->ctx;
	vi=op->v.p;
	
	obj=frm->stack[(op->t1)-1].a;
	fcn=dtcVaGetA(obj, vi->objinf);
	fvi=dtvUnwrapPtr(fcn);
	fvi->runcnt++;
	
	frmb=BSVM2_Interp_AllocFrame(ctx);

//	frmb->stack=ctx->tstack+ctx->tstackref;
//	frmb->local=frmb->stack+fvi->cblk->stkdepth;
	frmb->local=ctx->tstack+ctx->tstackref;
	frmb->stack=frmb->local+fvi->cblk->largs;

	ctx->tstackref=ctx->tstackref+fvi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;
	frmb->self=obj;

	frmb->rnext=frm;
	frm->rtrace=op->nexttrace;
	frm->rcsrv=op->t0;
	ctx->frame=frmb;

	for(i=0; i<fvi->nargs; i++)
	{
		frmb->local[fvi->cblk->bargs+i]=
			frm->stack[op->t1+i];
	}

	tr=BS2I_ImageGetFuncTrace(fvi);
	return(tr);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_CALLTH(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	BSVM2_ImageGlobal *vi, *fvi;
	BSVM2_Trace *tr;
	dtVal obj, fcn;
	int i;
	
	ctx=frm->ctx;
	vi=op->v.p;
	
	obj=frm->self;
	fcn=dtcVaGetA(obj, vi->objinf);
	fvi=dtvUnwrapPtr(fcn);
	fvi->runcnt++;
	
	frmb=BSVM2_Interp_AllocFrame(ctx);

//	frmb->stack=ctx->tstack+ctx->tstackref;
//	frmb->local=frmb->stack+fvi->cblk->stkdepth;
	frmb->local=ctx->tstack+ctx->tstackref;
	frmb->stack=frmb->local+fvi->cblk->largs;

	ctx->tstackref=ctx->tstackref+fvi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;
	frmb->self=obj;

	frmb->rnext=frm;
	frm->rtrace=op->nexttrace;
	frm->rcsrv=op->t0;
	ctx->frame=frmb;

	for(i=0; i<fvi->nargs; i++)
	{
		frmb->local[fvi->cblk->bargs+i]=
			frm->stack[op->t1+i];
	}

	tr=BS2I_ImageGetFuncTrace(fvi);
	return(tr);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_CALLA(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	BSVM2_ImageGlobal *vi, *fvi;
	BSVM2_Lambda *lfcn;
	BSVM2_Trace *tr;
	dtVal fcn;
	int i;
	
	ctx=frm->ctx;
	vi=op->v.p;

	fcn=frm->stack[(op->t1)-1].a;

	if(BSVM2_Interp_IsLambdaP(fcn))
	{
		lfcn=dtvUnwrapPtrF(fcn);
		fvi=lfcn->func;
	}else
	{
		lfcn=NULL;
		fvi=dtvUnwrapPtrF(fcn);
	}

	fvi->runcnt++;

	frmb=BSVM2_Interp_AllocFrame(ctx);

	frmb->local=ctx->tstack+ctx->tstackref;
	frmb->stack=frmb->local+fvi->cblk->largs;

	ctx->tstackref=ctx->tstackref+fvi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;

	frmb->rnext=frm;
	frm->rtrace=op->nexttrace;
	frm->rcsrv=op->t0;
	ctx->frame=frmb;

	if(lfcn)
		{ frmb->lxvar=lfcn->lxvar; }

	if(fvi->nargs>1)
	{
		for(i=0; i<fvi->nargs; i++)
		{
			frmb->local[fvi->cblk->bargs+i]=
				frm->stack[op->t1+i];
		}
	}else if(fvi->nargs)
	{
		frmb->local[fvi->cblk->bargs]=
			frm->stack[op->t1];
	}

	tr=BS2I_ImageGetFuncTrace(fvi);
	return(tr);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_CALLL(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	BSVM2_ImageGlobal *vi, *fvi;
	BSVM2_Lambda *lfcn;
	BSVM2_Trace *tr;
	dtVal fcn;
	int i;
	
	ctx=frm->ctx;
	vi=op->v.p;

	fcn=frm->local[op->i0].a;

	if(BSVM2_Interp_IsLambdaP(fcn))
	{
		lfcn=dtvUnwrapPtrF(fcn);
		fvi=lfcn->func;
	}else
	{
		lfcn=NULL;
		fvi=dtvUnwrapPtrF(fcn);
	}

	fvi->runcnt++;

	frmb=BSVM2_Interp_AllocFrame(ctx);

	frmb->local=ctx->tstack+ctx->tstackref;
	frmb->stack=frmb->local+fvi->cblk->largs;

	ctx->tstackref=ctx->tstackref+fvi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;

	frmb->rnext=frm;
	frm->rtrace=op->nexttrace;
	frm->rcsrv=op->t0;
	ctx->frame=frmb;

	if(lfcn)
		{ frmb->lxvar=lfcn->lxvar; }

	if(fvi->nargs>1)
	{
		for(i=0; i<fvi->nargs; i++)
		{
			frmb->local[fvi->cblk->bargs+i]=
				frm->stack[op->t1+i];
		}
	}else if(fvi->nargs)
	{
		frmb->local[fvi->cblk->bargs]=
			frm->stack[op->t1];
	}

	tr=BS2I_ImageGetFuncTrace(fvi);
	return(tr);
}

BSVM2_Trace *BSVM2_TrOp_ThrowUnwindI(BSVM2_Frame *frm)
{
	BSVM2_Context *ctx;
	BSVM2_Trace *tr1;
	BSVM2_Frame *fr1, *frmb;

	ctx=frm->ctx;
	fr1=ctx->ehstack_fr[ctx->ehstackpos-1];
	tr1=ctx->ehstack_tr[ctx->ehstackpos-1];

	while(frm && (frm!=fr1))
	{
		frmb=frm->rnext;
		frm->rnext=ctx->freeframe;
		ctx->freeframe=frm;
		frm=frmb;
	}

	ctx->frame=fr1;
	return(tr1);
}

BSVM2_Trace *BSVM2_TrOp_ThrowStatusI(BSVM2_Frame *frm, int status)
{
	if(frm->ctx->ehstackpos<=0)
	{
		frm->ctx->status=status;
		return(NULL);
	}

	frm->ctx->thrownex=dtvWrapInt(status);
	return(BSVM2_TrOp_ThrowUnwindI(frm));
}

BSVM2_Trace *BSVM2_TrOp_ThrowI(BSVM2_Frame *frm,
	BSVM2_TailOpcode *op, dtVal ex)
{
	if(dtvIsCharP(ex))
	{
		frm->ctx->thrownex=ex;
		if(frm->ctx->ehstackpos>0)
		{
//			return(frm->ctx->ehstack[frm->ctx->ehstackpos-1]);
			return(BSVM2_TrOp_ThrowUnwindI(frm));
		}
		frm->ctx->status=BSVM2_EXS_THROWLEC;
		return(NULL);
	}

	frm->ctx->thrownex=ex;
	if(frm->ctx->ehstackpos>0)
	{
//		return(frm->ctx->ehstack[frm->ctx->ehstackpos-1]);
		return(BSVM2_TrOp_ThrowUnwindI(frm));
	}
	frm->ctx->status=BSVM2_EXS_THROWNEX;
	return(NULL);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_THROW(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	dtVal ex;
	
	ex=frm->stack[op->t0].a;
	return(BSVM2_TrOp_ThrowI(frm, op, ex));
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_RETHROW(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	dtVal ex;
	
	ex=frm->stack[op->t0].a;
	if(dtvNullP(ex))
		{ return(op->nexttrace); }
	return(BSVM2_TrOp_ThrowI(frm, op, ex));
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_BEGTRY(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	int i;
	
	i=frm->ctx->ehstackpos++;
	frm->ctx->ehstack_fr[i]=frm;
	frm->ctx->ehstack_tr[i]=op->jmptrace;
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_ENDTRY(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	dtVal ex;
	ex=frm->ctx->thrownex;
	frm->ctx->thrownex=DTV_NULL;
	frm->ctx->ehstackpos--;
	frm->stack[op->t0].a=ex;
	if(dtvNullP(ex))
		return(op->jmptrace);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_CATCH(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	dtVal ex;
	int i;

	ex=frm->stack[op->t0].a;

	vi=op->v.p;
	if((vi->tag==BS2CC_ITCC_CL) ||
		(vi->tag==BS2CC_ITCC_IF))
	{
		i=BGBDTC_CheckObjvInstanceof(ex, vi->objinf);
		return(i?op->nexttrace:op->jmptrace);
	}

//	frm->stack[op->t0].i=0;
//	return(op->nexttrace);
	return(op->jmptrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_BEGLEC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	s32 v;
	int i;
	
	i=frm->ctx->ehstackpos++;
	frm->ctx->ehstack_fr[i]=frm;
	frm->ctx->ehstack_tr[i]=op->jmptrace;
	
	v=(s32)(((u64)(op->jmptrace->top)*((u64)2147483647))>>32);
	frm->local[op->i0].a=dtvWrapChar(v);
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_ENDLEC(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	dtVal ex;
	ex=frm->ctx->thrownex;
	frm->ctx->thrownex=DTV_NULL;
	frm->ctx->ehstackpos--;

	if(!dtvEqqP(ex, frm->local[op->i0].a))
		{ return(BSVM2_TrOp_ThrowI(frm, op, ex)); }
	return(op->nexttrace);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_ENDLEC2(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	dtVal ex;
	ex=frm->ctx->thrownex;
	frm->ctx->thrownex=DTV_NULL;
	frm->ctx->ehstackpos--;
	if(dtvEqqP(ex, frm->local[op->i0].a))
		ex=DTV_NULL;
	frm->stack[op->t0].a=ex;
	if(dtvNullP(ex))
		return(op->jmptrace);
	return(op->nexttrace);
}
