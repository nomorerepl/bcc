#pragma GCC diagnostic ignored "-Wempty-body"
#include"b.h"
#include"h.h"

#ifdef SYMS
C ID(CP p){R!!sc((S)"aNW0_",cl(p));} //!< after first 'a'-classed char, these classes are valid identifier chars
K nm(K x){R((B)xx)->k;}K*GG(K x){R&(((B)xx)->v);}
CP _nxt(ST st){S s=Ss;Ss=cp(Ss,&Pt);R Pn=Ss-s,Pt;}//! return next codepoint on tape and paste its length into n
K _tok(I a,ST st){S r=Ss;
  W(nxt()&&ID(Pt));Ss-=Pn;O("tok a=%d n=%d s=%.*s\n",a,Pn,(I)(  Ss-r),r);
  P(a&&(':'==Pt),O("bail on ':'\n"),Ss-=Ss-r,NL) //!< FIXME special case to support pcle() logic
  B b=hget(GT,r,Ss-r);K x=kS(1);R xu=Pn,xx=(K)b,x;}
#endif
K XXX(K*k,K y){R r0(*k),*k=y,NL;} //!< release an existing value at pointer x and replace it with y

ZS qt(){R sc(";})]",*Ss);} //<! test whether current char is an expression terminator
ZK Na(){S r=Ss;W(10u>*++Ss-'0'||'.'==*Ss){};I f=0;N(Ss-r,f|='.'==r[i])R f?kf(fp(r,Ss-r)):ki(ip(r,Ss-r));}//!< parse numbers, int|fp (floats as 1e6 must contain a dot)

// encode integers up to 127 as bytes with the highest bit set
// other numbers are appended at the end of z and return index
// (indices <16 correspond to registers, zn-3 is the index in
// the z is of K vals: z[0] is the source and z[1] is opcodes)
ZK n(K x){R kc(KI==Ax&&129u>1+xi?128+xi:(z=jk(z,x),16+zn-3));}

ZK pF(K x,I b,ST st){R KF-b||KF==t(x)?x:Ax&&126<xi?n(kf(xi-128)):u(KF,k2(kc('%'),x));}//!< promote rettype to float if needed
ZK pE(I a,I c,ST st){      //!< parse an expr: c cmd, a optional rettype
 K r=k1(kc(c)),x;          //!< store command as char in K array
 do r=jk(r,x=qt()          //!< append parse trees of subsequent exprs to r
      ?n(ki(0))            //<! terminate expr when reached ;})]
      :p());               //<! get the parse tree of the next expr
 W(';'==*Ss++);            //<! semicolon is the expression separator
 R u(a?a:t(x),r);}         //<! set the given rettype, or the rettype of last expr

#define f(x,b) pF(x,b,st)
#define E(a,c) pE(a,c,st)

#ifndef SYMS
//! class            !""#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
I cl(CP c){P(!c,0)R 128>c?"  \"+$++ ()++ + +0000000000+;+++  aaaaaaaaaaaaaNaaaaaaaaWaaa[+]+_`aaaaaaaaaaaaaaaaaaaaaaaaaa{+} "[c-32]:0;}
#else
//! class            !""#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
I cl(CP c){P(!c,0)R 128>c?"  \"+$++ ()++ + +0000000000+;+++  aaaaaaaaaaaaaNaaaaaaaaWaaa[+]+_`aaaaaaaaaaaaaaaaaaaaaaaaaa{+} "[c-32]:"\0aga++"[UC(c)];}
#endif

//! parse next codepoint on tape
K _p(ST st){K x,y;CP a;I b;      //!< a operator, x/y operands, b return type
 //O("p Ss=%s\n",Ss);
 C qn='0'-cl('-'==(a=*Ss++)?Ss['.'==*Ss]:'.'==a?*Ss:a); //<!special case: if expr starts with a minus, dot or a minus-dot, it may be a number
 C cls=qn?cl(a):'0';

#ifdef SYMS
 cls=LP(cls)&&'('-Ss[1]?'a':cls; //<!special case: if N|W is not followed by (, force class to identifier
#endif

 switch(cls){                //!< current char class:
  case'N':T[sN++]=KI;        //!< 'for' loop, declare a loop variable and fallthrough to W
  C('W',R                    //!< W|N(cnd){body}
   ++Ss,x=p(),++Ss,          //<! parse cnd expr into x
   x=k3(kc(a),x,p()),        //<! x is (W|N,cnd,body)
   sN-='N'==a,x)             //<! undeclare loop var for N
  case'$':++Ss;              //!< $[ctf], fallthrough
  C('{',R E(0,a))            //!< inner scope, return enclosed expression
  C('+',R x=p(),             //!< an operator
   //!rettype: #x is int (count), x%y is float (div), *x is list type (first), others retain right operand type
   u('#'==a?KI:'%'==a?KF:t(x)-8*('*'==a),k2(kc(a),x)))
  C('[',R E(X2,a))           //!< parse a dyadic expression inside []
  C('(',x=p(),++Ss)          //!< parse fenced expression
  C('0',                     //!< numeral
    P('2'==a&&'*'==*Ss,++Ss,x=p(),u(t(x),k2(kc('\\'),x)))//!< override 2*x as monadic left shift (\x) and return
    --Ss;x=n(Na()))          //<! parse number
  case'a':                   //<! identifier:
#ifdef SYMS
    {--Ss;K y=tok(0);//o(y); //<! tok() parses the complete identifier and stores it in the hash table
#endif
    x='['==*Ss?++Ss,pE(      //<! a) if followed by [exp], it is an array indexing or a function call:
     T[b=a-'a']?T[b]-8:      //<! if varname has no type, it is a func call; for arrays, unset high bit
#ifndef SYMS
      (x=GGG[b],x=xy,        //<! xx is the string, xy is the code
#else
      (x=*GG(y),x=xy,        //<! xx is the string, xy is the code
#endif
       D0=MX(D0,xC[xn-2]),   //<! D[0] and D[1] are stored after RET
       D1=MX(D1,xC[xn-1]),xu),
#ifdef SYMS
     nm(y),st               //<! op is the array|function name
    ):nm(y);};break;        //<! b) it is variable reference.
#else
     a,st                    //<! op is the array|function name
    ):kc(a);break;           //<! b) it is variable reference.
#endif
  default:AB(Ss-1);}         //<! bail on unmapped class or whitespace

 P(qt(),x)                   //<! if reached expr end, return the parse tree
 if('+'-cl(a=*Ss++))AB(Ss-1);//<! otherwise next char should be an operator, bail if not
 if(':'==*Ss)++Ss,a+=128;    //<! for assignment, set high bit of op char byte
 y=p();b=t(y);               //<! parse right operand into y and get its type into b
 $(':'==a&&Ax,T[xi-'a']=b)   //<! for assignment, set result type to the type of the right operand
 b='%'-a?MX(b,t(x)):KF;      //<! for div, force it to float, for the rest, use the widest one (KF>KJ>KI>KC)

 R u(U('<')<=U(a)?KI:b,      //<! if operator is a comparison (<=>), force return type to int
 k3(kc(a),f(x,b),f(y,b)));}  //<! return (op,left,right)

//S('0'-c('-'==(a=*s++)?s['.'==*s]:'.'==a?*s:a)?c(a):'0',case'N':T[sN++]=KI;
// C('W',R++s,x=p(st),++s,x=k3(kc(a),x,p(st)),sN-='N'==a,x)case'$':++s;C('{',R Ee(0,a,st))
// C('+',R x=p(st),u('#'==a?KI:'%'==a?KF:t(x)-8*('*'==a),k2(kc(a),x)))C('[',R Ee(12,a,st))C('(',x=p(st),++s)C('0',P('2'==a&&'*'==*s,++s,x=p(st),u(t(x),k2(kc('\\'),x)))--s;x=n(Na()))
// C('a',x='['==*s?++s,Ee(T[b=a-'a']?T[b]-8:(x=G[b],x=xy,D0=MX(D0,x[xn-2]),D1=MX(D1,x[xn-1]),xu),a,st):kc(a)),AB(s-1))

//:~
