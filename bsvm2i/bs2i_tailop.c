#include <bteifgl.h>

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

BSVM2_Trace *BSVM2_Interp_DecodeBlockSetupTraceDfl(
	BSVM2_CodeBlock *cblk, BSVM2_Opcode **ops, int n_ops,
	BSVM2_TailOpcode *top, int flag)
{
	BSVM2_Trace *tr;
	int i;
	
	tr=BSVM2_Interp_AllocTrace(cblk);

	if((n_ops>8) && !top)
	{
		top=BSVM2_Interp_AllocTailOpcode(cblk);
		top->Run=BSVM2_TrOp_Default;
	}

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
	
	if(n_ops>8)
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
//		case 8: tr->Run=BSVM2_TrRun_DefN8; break;
		default: tr->Run=BSVM2_TrRun_DefN; break;
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
	
	return(tr);
}



BSVM2_Trace *BSVM2_TrOp_Default(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	return(op->nexttrace);
}

BSVM2_Trace *BSVM2_TrOp_JMP(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	return(op->jmptrace);
}

BSVM2_Trace *BSVM2_TrOp_JEQ(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(!frm->stack[op->t0].i)
		return(op->jmptrace);
	return(op->nexttrace);
}

BSVM2_Trace *BSVM2_TrOp_JNE(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i)
		return(op->jmptrace);
	return(op->nexttrace);
}

BSVM2_Trace *BSVM2_TrOp_JLT(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i<0)
		return(op->jmptrace);
	return(op->nexttrace);
}

BSVM2_Trace *BSVM2_TrOp_JGT(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i>0)
		return(op->jmptrace);
	return(op->nexttrace);
}

BSVM2_Trace *BSVM2_TrOp_JLE(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i<=0)
		return(op->jmptrace);
	return(op->nexttrace);
}

BSVM2_Trace *BSVM2_TrOp_JGE(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	if(frm->stack[op->t0].i>=0)
		return(op->jmptrace);
	return(op->nexttrace);
}


BSVM2_Trace *BSVM2_TrOp_JCMP_EQI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i==frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_NEI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i!=frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LTI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i<frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GTI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i>frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LEI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i<=frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GEI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].i>=frm->stack[op->t1].i)
		return(op->jmptrace);
	return(op->nexttrace);	}

BSVM2_Trace *BSVM2_TrOp_JCMP_LTUI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui<frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GTUI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui>frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LEUI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui<=frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GEUI(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ui>=frm->stack[op->t1].ui)
		return(op->jmptrace);
	return(op->nexttrace);	}


BSVM2_Trace *BSVM2_TrOp_JCMP_EQL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l==frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_NEL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l!=frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LTL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l<frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GTL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l>frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LEL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l<=frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GEL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].l>=frm->stack[op->t1].l)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LTUL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul<frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GTUL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul>frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LEUL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul<=frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GEUL(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].ul>=frm->stack[op->t1].ul)
		return(op->jmptrace);
	return(op->nexttrace);	}


BSVM2_Trace *BSVM2_TrOp_JCMP_EQF(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f==frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_NEF(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f!=frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LTF(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f<frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GTF(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f>frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LEF(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f<=frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GEF(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].f>=frm->stack[op->t1].f)
		return(op->jmptrace);
	return(op->nexttrace);	}

BSVM2_Trace *BSVM2_TrOp_JCMP_EQD(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d==frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_NED(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d!=frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LTD(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d<frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GTD(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d>frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_LED(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d<=frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}
BSVM2_Trace *BSVM2_TrOp_JCMP_GED(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{	if(frm->stack[op->t0].d>=frm->stack[op->t1].d)
		return(op->jmptrace);
	return(op->nexttrace);	}


BSVM2_Trace *BSVM2_TrOp_JCMP_RETI(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETL(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETF(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETD(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETA(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETV(
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


BSVM2_Trace *BSVM2_TrOp_JCMP_RETIC(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETLC(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETFC(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETDC(
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

BSVM2_Trace *BSVM2_TrOp_JCMP_RETAC(
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

BSVM2_Trace *BSVM2_TrOp_CALLG(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Context *ctx;
	BSVM2_Frame *frmb;
	BSVM2_ImageGlobal *vi;
	BSVM2_Trace *tr;
	int i;
	
	ctx=frm->ctx;
	vi=op->v.p;
	
	frmb=BSVM2_Interp_AllocFrame(ctx);

	frmb->stack=ctx->tstack+ctx->tstackref;
	frmb->local=frmb->stack+vi->cblk->stkdepth;
	ctx->tstackref=ctx->tstackref+vi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;

	frmb->rnext=frm;
	frm->rtrace=op->nexttrace;
	frm->rcsrv=op->t1;
	ctx->frame=frmb;

	for(i=0; i<vi->nargs; i++)
	{
		frmb->local[vi->cblk->bargs+i]=
			frm->stack[op->t1+i];
	}

	tr=BS2I_ImageGetFuncTrace(vi);
	return(tr);
}
