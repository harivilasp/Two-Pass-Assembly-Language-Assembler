#include<stdio.h>
#include<string.h>
#include<conio.h>
#define MAXLINE 79
#define SIZE 80

struct pse_table
{
  char ps_do[10];
}p_table[2]={{"ORG"},{"END"}};

struct mot_table
{
  char mnemo[10];
  int len;
  int op;
}table[SIZE]=
  {
    {"MOV",1,0X40},
    {"MVI",2,6},
    {"LXI",3,1},
    {"LDA",3,0X3A},
    {"STA",3,0X32},
    {"LHLD",3,0X2A},
    {"SHLD",3,0X22},
    {"LDAX",1,0X0A},
    {"STAX",1,2},
    {"XCHG",1,0XEB},
    {"ADD",1,0X80},
    {"ADI",2,0XC6},
    {"ADC",1,0X88},
    {"ACI",2,0XCE},
    {"SUB",1,0X90},
    {"SUI",2,0XD6},
    {"SBB",1,0X98},
    {"SBI",2,0XDE},
    {"INR",1,4},
    {"DCR",1,5},
    {"INX",1,3},
    {"DCX",1,0X0B},
    {"DAD",2,9},
    {"DAA",1,0X27},
    {"ANA",1,0XA0},
    {"ANI",2,0XE6},
    {"XRA",1,0XA8},
    {"XRI",2,0XEE},
    {"ORA",1,0XB0},
    {"ORI",2,0XF6},
    {"CMP",1,0XB8},
    {"CPI",2,0XFE},
    {"RLC",1,7},
    {"RRC",1,0X0F},
    {"RAL",1,0X17},
    {"RAR",1,0X1F},
    {"CMA",1,0X2F},
    {"CMC",1,0X3F},
    {"STC",1,0X37},
    {"PCHL",1,0XE9},
    {"RST",1,0XC7},
    {"PUSH",1,0XC5},
    {"POP",1,0XC1},
    {"OUT",2,0XD3},
    {"IN",2,0XDB},
    {"XTHL",1,0XE3},
    {"SPHL",1,0XF9},
    {"RIM",1,0X20},
    {"SIM",1,0X30},
    {"NOP",1,0},
    {"HLT",1,0X76},
    {"DI",1,0XF3},
    {"EI",1,0XFB},
    {"JMP",3,0XC3},
    {"JNZ",3,0XC2},
    {"JZ",3,0XCA},
    {"JNC",3,0XD2},
    {"JC",3,0XDA},
    {"JPO",3,0XE2},
    {"JPE",3,0XEA},
    {"JP",3,0XF2},
    {"JM",3,0XFA},
    {"RET",1,0XC9},
    {"RNZ",1,0XC0},
    {"RZ",1,0XC8},
    {"RNC",1,0XD0},
    {"RC",1,0XD8},
    {"RPO",1,0XE0},
    {"RPE",1,0XE8},
    {"RP",1,0XF0},
    {"CALL",3,0XCD},
    {"CNZ",3,0XC4},
    {"CZ",3,0XCC},
    {"CNC",3,0XD4},
    {"CC",3,0XDC},
    {"CPO",3,0XE4},
    {"CPE",3,0XEC},
    {"CP",3,0XF4},
    {"CM",3,0XFC},
  };

struct symbol_table
{
  char label[10];
  int loc;
}stable[20];

FILE *fp1,*fp2,*fopen();
int addr[8]={7,0,1,2,3,4,5,6};
int addr1[8]={0,1,2,3,4,5,6,7};
int lc=0;
char s0[20]={NULL},s1[20]={NULL},s2[20]={NULL},s3[20]={NULL};
char fn[30],fn1[30];
int i;
int op_code;
int n;
char d;
int m;
int count=0;
char s4[10];

void token(char *s);
int stlbl();
int pseudo();
int atoi(char s[]);
void pass2();
void abs();
void mov_op();
void ana_op();
void mvi_op();
void lxi_op();
void rst_op();
void fun();
void other();

void main(int argc,char *argv[])
{
  int i;
  int k,c,prin;
  char s[80],name[13];
  clrscr();

  //if(argc!=3)
  //{
  //  printf("\n error: i/p & o/p file name missing");
  //  exit();
  //}

  if((fp1=fopen("assem.asm","r"))==NULL)
  {
    printf("\n error: cant open file");
    exit();
  }

  strcpy(fn1,"test.txt");
  strcpy(fn,"assem.asm");

  while(fgets(s,MAXLINE,fp1) && s[0]!='\n')
  {
    token(s);
    printf("\n\nhello");

    for(k=0;k<count;k++)
    {
      if(strcmp(stable[k].label,s1)==0)
	printf("\n error: same label used more time\n");
    }

    if((c=pseudo())!=2)
    {
      switch(c)
      {
	case 0:
	  lc=atoi(s3);
	  break;
	case 1:
	  fclose(fp1);
	  pass2();
	default:
	  break;
      }
    }

    if(stlbl())
      count++;

    for(i=0;i<SIZE;i++)
    {
      if(strcmp(table[i].mnemo,s2)==0)
      {
	lc=lc+table[i].len;
	break;
      }
    }

    if(strcmp(s2,"ORG")!=0 && strcmp(s2,"END")!=0 && i>=SIZE)
    {
      printf("\n mnemonic missing");
      exit();
    }
  }

  for(prin=0;prin<20;prin++)
  {
    printf("\n%s %d",stable[prin].label,stable[prin].loc);
  }
  fclose(fp1);
  fclose(fp2);
}


void token(char s[])
{
  int i,j=0;
  for(i=0;s[i]!=':' && s[i];i++,j++);
  if(s[i]==0)
  {
    s0[0]=0;
    j=0;
  }
  else
  {
    for(i=0;i<j;i++)
      s0[i]=s[i];
    j++;
    s0[i]=NULL;
  }

  while(s[j]==' ' || s[j]=='\t')
    j++;
  for(i=0; s[j]!=' ' && s[j]!='\n' && s[j] && s[j]!='\t';i++,j++)
    s2[i]=s[j];
  s2[i]=NULL;

  while(s[j]==' ' || s[j]=='\t' || s[j]=='\n')
    j++;
  for(i=0;s[j]!='\n' && s[j]!=';' && s[j];i++,j++)
    s3[i]=s[j];
  s3[i]=NULL;
  i=0;

  while(s0[i]==' ' || s0[i]=='\t')
    i++;
  for(j=0;s0[i];i++,j++)
    s1[j]=s0[i];
  s1[j]=NULL;
}

int stlbl()
{
  static int j=0;
  if(*s1)
  {
    strcpy(stable[j].label,s1);
    stable[j].loc=lc;
    j++;
    return(1);
  }
  else
    return(0);
}

int pseudo()
{
  register int i;
  for(i=0;i<=2;i++)
  {
    if(strcmp(p_table[i].ps_do,s2)==0)
      return(i);
  }
  return(i);
}

int atoi(char s[])
{
  int i,v;
  v=0;
  for(i=0;s[i]!=NULL;i++)
    v=v*16+((s[i]>='A' && s[i]<='F')? (s[i]=s[i]-55):(s[i]=s[i]-0x30));
  return(v);
}

void pass2()
{
  char s[80],sh[10],sl[10];
  char line[70];
  int k;
  unsigned int c,l,h,temp;
  fp2=fopen(fn1,"w");
  fp1=fopen(fn,"r");
  if(fp1==0)
  {
    puts("fp1 is null");
    exit();
  }
  fprintf(fp2,"Address \t Instruction \t Oprand \t Opcode");
  while(fgets(s,MAXLINE,fp1)!=NULL && s[0]!='\n')
  {
    token(s);
    if((c=pseudo())!=2)
    {
      switch(c)
      {
	case 0:
	  lc=atoi(s3);
	  break;
	case 1:
	  fclose(fp2);
	  break;
	default:
	  break;
      }
    }

    for(i=0;i<=SIZE;i++)
    {
      if(strcmp(table[i].mnemo,s2)==0)
      {
	if(strcmp(s2,"MOV")==0)
	  mov_op();
	else if(strcmp(s2,"MVI")==0)
	  mvi_op();
	else if(strcmp(s2,"LXI")==0)
	  lxi_op();
	else if(strcmp(s2,"LDAX")==0)
	  lxi_op();
	else if(strcmp(s2,"STAX")==0)
	  lxi_op();
	else if(strcmp(s2,"ADD")==0)
	  ana_op();
	else if(strcmp(s2,"ADC")==0)
	  ana_op();
	else if(strcmp(s2,"SUB")==0)
	  ana_op();
	else if(strcmp(s2,"SBB")==0)
	  ana_op();
	else if(strcmp(s2,"INR")==0)
	  mvi_op();
	else if(strcmp(s2,"DCR")==0)
	  mvi_op();
	else if(strcmp(s2,"INX")==0)
	  lxi_op();
	else if(strcmp(s2,"DAD")==0)
	  lxi_op();
	else if(strcmp(s2,"DCX")==0)
	  lxi_op();
	else if(strcmp(s2,"ANA")==0)
	  ana_op();
	else if(strcmp(s2,"XRA")==0)
	  ana_op();
	else if(strcmp(s2,"ORA")==0)
	  ana_op();
	else if(strcmp(s2,"CMP")==0)
	  ana_op();
	else if(strcmp(s2,"PUSH")==0)
	  lxi_op();
	else if(strcmp(s2,"POP")==0)
	  lxi_op();
	else if(strcmp(s2,"RST")==0)
	  rst_op();
	else
	  other();
      }
    }

    for(n=0;n<count;n++)
    {
      if(strcmp(stable[n].label,s3)==0)
      {
	h=stable[n].loc & 0xFF00;
	h=h >> 8;
	l=stable[n].loc & 0x00FF;
	fprintf(fp2,"\n %04x \t \t \t %02x",lc++,l);
	fprintf(fp2,"\n %04x \t \t \t %02x",lc++,h);
	break;
      }
    }

    if(n>=count)
    {
      if( strcmp(s2,"JMP")==0 || strcmp(s2,"JNZ")==0 ||
	  strcmp(s2,"JZ")==0 || strcmp(s2,"JNC")==0 ||
	  strcmp(s2,"JC")==0 || strcmp(s2,"JPO")==0 ||
	  strcmp(s2,"JPE")==0 || strcmp(s2,"JP")==0 ||
	  strcmp(s2,"JM")==0 || strcmp(s2,"CALL")==0 ||
	  strcmp(s2,"CNZ")==0 || strcmp(s2,"CZ")==0 ||
	  strcmp(s2,"CNC")==0 || strcmp(s2,"CC")==0 ||
	  strcmp(s2,"CPO")==0 || strcmp(s2,"CPE")==0 ||
	  strcmp(s2,"CP")==0 || strcmp(s2,"CM")==0)
	abs();
    }

    if(s2[2]=='I')
    {
      if(strcmp(s2,"MVI")==0)
      {
	sl[0]=s3[2];
	sl[1]=s3[3];
	sl[2]=0;
	l=atoi(sl);
	fprintf(fp2,"\n %04x \t \t \t %02x",lc++,l);
      }
      else if(strcmp(s2,"LXI")==0)
      {
	for(i=0;s3[i]!=',';i++);
	k=0;
	i++;
	while(s3[i]!='\0' && s[i]!='\n')
	  sl[k++]=s3[i++];
	sl[k]=0;
	temp=atoi(sl);
	l=temp & 0x00FF;
	h=temp & 0xFF00;
	h=h>>8;
	fprintf(fp2,"\n %04x \t \t \t %02x",lc++,l);
	fprintf(fp2,"\n %04x \t \t \t %02x",lc++,h);
      }
      else
      {
	l=atoi(s3);
	fprintf(fp2,"\n %04x \t \t \t %02x",lc++,l);
      }
      if(n>=count)
      {
	if(strcmp(s2,"LDA")==0 || strcmp(s2,"STA")==0 ||
	   strcmp(s2,"LHLD")==0 || strcmp(s2,"SHLD")==0)
	  abs();
      }
      if(strcmp(s2,"IN")==0 || strcmp(s2,"OUT")==0)
      {
	l=atoi(s3);
	fprintf(fp2,"\n %04x \t \t \t %02x",lc++,l);
      }
    }
  }
}

void abs()
{
  unsigned int temp,l,h;
  temp=atoi(s3);
  l=temp & 0x00FF;
  h=temp & 0xFF00;
  h=h >> 8;
  fprintf(fp2,"\n %04x \t \t \t %02x",lc++,l);
  fprintf(fp2,"\n %04x \t \t \t %02x",lc++,h);
}

void mov_op()
{
  fprintf(fp2,"\n %04x \t %s \t %s \t ",lc++,s2,s3);
  fun();
  op_code = table[i].op;
  for(n=0,d='A';d!=s3[0];d++,n++);
  m=addr[n];
  m=m << 3;
  op_code= op_code | m;
  for(n=0,d='A';d!=s3[2];d++,n++);
  op_code=op_code | addr[n];
  fprintf(fp2,"%02x",op_code);
}

void ana_op()
{
  fprintf(fp2,"\n %04x \t %s \t %s \t ",lc++,s2,s3);
  fun();
  op_code=table[i].op;
  for(n=0,d='A';d!=s3[0];d++,n++);
  op_code = op_code | addr[n];
  fprintf(fp2,"%02x",op_code);
}

void mvi_op()
{
  fprintf(fp2,"\n %04x \t %s \t %s \t ",lc++,s2,s3);
  fun();
  op_code=table[i].op;
  for(n=0,d='A';d!=s3[0];d++,n++);
  m=addr[n];
  m=m << 3;
  op_code=op_code | m;
  fprintf(fp2,"%02x",op_code);
}

void lxi_op()
{
  op_code = table[i].op;
  if(s3[0]=='B')
    op_code=op_code | 0x00;
  if(s3[0]=='D')
    op_code=op_code | 0x10;
  if(s3[0]=='H')
    op_code=op_code | 0x20;
  if(s3[0]=='S' || s3[0]=='P')
    op_code=op_code | 0x30;
  fprintf(fp2,"\n %04x \t %s \t %s \t %02x",lc++,s2,s3,op_code);
}

void rst_op()
{
  op_code = table[i].op;
  for(n=0,d='0';d!=s3[0];d++,n++);
  m = addr1[n];
  m = m<<3;
  op_code = op_code | m;
  fprintf(fp2,"\n %04x \t %s \t %s \t %02x",lc++,s2,s3,op_code);
}

void fun()
{
  if(s3[0]=='H')
    s3[0]='F';
  if(s3[0]=='L')
    s3[0]='G';
  if(s3[0]=='M')
    s3[0]='H';
  if(s3[2]=='H')
    s3[2]='F';
  if(s3[2]=='L')
    s3[2]='G';
  if(s3[2]=='M')
    s3[2]='H';
}

void other()
{
  op_code = table[i].op;
  fprintf(fp2,"\n %04x \t %s \t %s \t %02x",lc++,s2,s3,op_code);
}