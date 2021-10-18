#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "shellp.h"

char *strdup(const char *s);

// Check the bottom of this file to locate the  method you must implement.
// You do not need to read all provided code, but you are encouraged to
// do so if you want to experiment/understand more details about the program.

#define R_NONE 0    /* No redirections */
#define R_INP  1    /* input redirection bit */
#define R_OUTP 2    /* output redirection bit */
#define R_APPD 4    /* append redirection bit */

#define MAXRD 255

Stage* allocStage(int nba,char** args);
void freeStage(Stage*);
void printStage(Stage*);
Command* addCommandStage(Command* c,Stage* s);

int extractRedirect(char* buf,int* len,int* mode,char* input,char* output);

char* skipWS(char* arg);
char* cutWord(char* arg);
int trimString(char* buf,int len);

Command* allocCommand(char* c)
{
  Command* r = (Command*)calloc(1,sizeof(Command));
  r->_com = NULL;
  r->_kind = noCMD;
  r->_mode = R_NONE;
  return r;
}
void freeCommand(Command* c)
{
  if (c->_com) free(c->_com);
  if (c->_input) free(c->_input);
  if (c->_output) free(c->_output);
  for(int i=0;i < c->_nba;i++)
    if (c->_args[i]) free(c->_args[i]);
  free(c->_args);
  for(int j=0;j<c->_nbs;j++)
    freeStage(c->_stages[j]);
  if (c->_stages) free(c->_stages);
  free(c);
}

Command* setCommand(Command* c,enum Kind k,char* com)
{
  c->_kind = k;
  if (c->_com) free(c->_com);
  c->_com = strdup(com);
  return c;
}
Command* setCommandArgs(Command* c,int nb,char** args)
{
  c->_nba = nb + 1;
  c->_args = (char**)malloc(sizeof(char*)*c->_nba);
  for(int i=0;i<nb;i++)
    c->_args[i] = strdup(args[i]);
  c->_args[nb] = NULL;
  return c;
}

Command* addCommandStage(Command* c,Stage* s)
{
  printf("New stage:");
  printStage(s);

  c->_stages = realloc(c->_stages,sizeof(Stage*)*(c->_nbs + 1));
  c->_nbs += 1;
  c->_stages[c->_nbs - 1] = s;
  return c;
}

void printCommand(Command* c)
{
  if (c->_mode & R_INP)
    printf("<  [%s]\n",c->_input);
  if (c->_mode & R_OUTP)
    printf(">  [%s]\n",c->_output);
  if (c->_mode & R_APPD)
    printf(">> [%s]\n",c->_output);
  printf("CORE: %s\n",c->_com);
  for(int i=0;i<c->_nba;i++)
    printf("\targs[%d] = %s\n",i,c->_args[i]);
  printf("Stages:");
  for(int i=0;i < c->_nbs;i++)
    printStage(c->_stages[i]);
  printf("\n");
  fflush(stdout);
}


Command* makeCommand()
{
  char buffer[1024];
  int  i = 0;
  char ch,*ptr;
  printf("%% ");fflush(stdout);
  while(i < sizeof(buffer) && (ch = getchar()) != '\n' && ch != EOF)
    buffer[i++] = ch;

  buffer[i] = 0;
  ptr = buffer+i-1;
  while(ptr>=buffer && isspace(*ptr)) ptr--,i--;
  *++ptr = 0;
  Command* c = allocCommand(ptr);
  if(ch==EOF)
    return setCommand(c,exitCMD,"exit");
  else {
    int mode = R_NONE;
    char input[1024];
    char output[1024];
    *input = *output = 0;
    int len = strlen(buffer);
    int ok = extractRedirect(buffer,&len,&mode,input,output);
    c->_mode = mode;
    c->_input = strdup(input);
    c->_output = strdup(output);
    len = trimString(buffer,len);
    if (ok) {
      char* sc = skipWS(buffer);
      char* ec = cutWord(sc);
      if (strcmp(sc,"cd")==0) {
	char* a0 = skipWS(ec);
	char* a1 = cutWord(a0);
	char* args[1] = {a0};
	return setCommandArgs(setCommand(c,cdCMD,sc),1,args);
      } else if (strcmp(sc,"pwd")==0) {
	return setCommand(c,pwdCMD,sc);
      } else if (strcmp(sc,"ln") == 0) {
	char* a0 = skipWS(ec);
	char* a1 = skipWS(cutWord(a0));
	char* a2 = cutWord(a1);
	char* args[2] = {a0,a1};
	return setCommandArgs(setCommand(c,linkCMD,sc),2,args);
      } else if (strcmp(sc,"rm") == 0) {
	char* a0 = skipWS(ec);
	char* a1 = cutWord(a0);
	char* args[1] = {a0};
	return setCommandArgs(setCommand(c,rmCMD,sc),1,args);
      } else if (strcmp(sc,"exit") == 0) {
	return setCommand(c,exitCMD,sc);
      } else {
	if (*sc) {
	  if (strchr(ec,'|') != NULL) {
	    // This is a pipeline.
	    char*  args[512];
	    args[0] = sc;
	    char* arg = skipWS(ec);
	    int nba = 1;
	    setCommand(c,pipelineCMD,"");
	    while(arg && *arg) {
	      char* p = *arg == '|' ? arg : 0;
	      if (p) {
		*p = 0;
		addCommandStage(c,allocStage(nba,args));
		args[0] = arg = skipWS(p+1);
		arg = cutWord(arg);
		nba = 1;
	      } else {
		args[nba++] = arg;
		arg = skipWS(cutWord(arg));
	      }
	    }
	    return addCommandStage(c,allocStage(nba,args));
	  } else {
	    char*  args[1024];
	    args[0] = sc;
	    char* arg = ec;
	    int nba = 1;
	    while(arg && *arg) {
	      args[nba++] = arg;
	      arg = skipWS(cutWord(arg));
	      assert(nba < 1024);
	    }
	    return setCommandArgs(setCommand(c,basicCMD,args[0]),nba,args);
	  }
	}
      }
    }
    return c;
  }
}

#define IS_REDIR(ch) ((ch) == '<' || (ch)=='>')

/* The routine extracts any redirects and replaces those parts of the command
   by whitespaces
*/

int extractRedirect(char* buf,int* len,int* mode,char* input,char* output)
{
  int i = 0;
  char* ptr = buf;
  while(ptr && *ptr) {
    if (IS_REDIR(*ptr)) {
      if (ptr[0]== '<') {
	if (*mode & R_INP) {
	  printf("Ambiguous input redirect\n");
	  return 0;
	} else {
	  *ptr++ = ' ';
	  i = 0;
	  while(isspace(*ptr)) ptr++;
	  while(isalnum(*ptr) || ispunct(*ptr)) {
	    if (i>=MAXRD) {
	      printf("redirect filename too long\n");
	      return 0;
	    }
	    input[i++] = *ptr;
	    *ptr++ = ' ';
	  }
	  input[i] = 0;
	  *mode = *mode | R_INP;
	}
      } else if (ptr[0]=='>' && ptr[1]=='>') {
	if (*mode & (R_APPD | R_OUTP)) {
	  printf("Ambiguous output redirect\n");
	  return 0;
	} else {
	  ptr[0] = ' ';
	  ptr[1] = ' ';
	  ptr += 2;
	  i = 0;
	  while(isspace(*ptr)) ptr++;
	  while(isalnum(*ptr) || ispunct(*ptr)) {
	    if (i>=MAXRD) {
	      printf("redirect filename too long\n");
	      return 0;
	    }
	    output[i++] = *ptr;
	    *ptr++ = ' ';
	  }
	  output[i] = 0;
	  *mode = *mode | R_APPD;
	}
      } else {
	if (*mode & (R_APPD | R_OUTP)) {
	  printf("Ambiguous output redirect\n");
	  return 0;
	} else {
	  assert(ptr[0]=='>');
	  *ptr++ = ' ';
	  i = 0;
	  while(isspace(*ptr)) ptr++;
	  while(isalnum(*ptr) || ispunct(*ptr)) {
	    if (i>=MAXRD) {
	      printf("redirect filename too long\n");
	      return 0;
	    }
	    output[i++] = *ptr;
	    *ptr++ = ' ';
	  }
	  output[i] = 0;
	  *mode = *mode | R_OUTP;
	}
      }
    }
    else ptr++;
  }
  return 1;
}

char* skipWS(char* arg)
{
  while(arg && *arg && isspace(*arg)) arg++;
  if(arg && *arg=='\"') arg++;
  return arg;
}

char* cutWord(char* arg)
{
  while(arg && *arg && !isspace(*arg)) arg++;
  if(*(arg-1)=='\"') arg--;
  if (arg && *arg) *arg++ = 0;
  return arg;
}

int trimString(char* buf,int len)
{
  char* ptr = buf + len - 1;
  while(ptr>=buf && isspace(*ptr))
    ptr--,len--;
  *++ptr = 0;
  return len;
}

// ================================================================================
// This part of the file contains the PipelineCommand object that you must modify
// ================================================================================
Stage* allocStage(int nba,char** args)
{
  Stage* s = (Stage*)calloc(1,sizeof(Stage));
  s->_nba = nba + 1;
  s->_args = (char**)calloc(s->_nba,sizeof(char*));
  s->_fdin = s->_fdout = -1;
  for(int i=0;i<nba;i++)
    s->_args[i] = strdup(args[i]);
  return s;
}

void freeStage(Stage* s)
{
  for(int i=0;i<s->_nba;i++)
    if (s->_args[i] != NULL) free(s->_args[i]);
  free(s->_args);
  free(s);
}

void printStage(Stage* s)
{
  printf("\t(%d)[",s->_nba);
  for(int i=0;i<s->_nba;i++)
    if (s->_args[i] != NULL)
      printf("%s ",s->_args[i]);
    else printf("null ");
  printf("]\n");
}

Stage* setStageInput(Stage* s,int fd)
{
  s->_fdin = fd;
  return s;
}
Stage* setStageOutput(Stage* s,int fd)
{
  s->_fdout = fd;
  return s;
}

// ================================================================================
// Write the pipelining logic here.
// ================================================================================

void spawnStage(Stage* s)
{
  s->_child = fork();
  switch(s->_child) {
  case 0: { // inside child
    if (s->_fdin != -1) {
      close(0);
      dup(s->_fdin);
      close(s->_fdin);
    }
    if (s->_fdout != -1) {
      close(1);
      dup(s->_fdout);
      close(s->_fdout);
    }
    int rv = execvp(s->_args[0],s->_args);
    printf("execvp on [%s] failed with status %d ->[%s]\n",s->_args[0],errno,strerror(errno));
  }break;
  case -1: printf("spawn failed in stage");printStage(s);break;
  default: {   // inside parent. close the ends of the two pipes (in/out)
    if (s->_fdout!=-1)  close(s->_fdout);
    if (s->_fdin != -1) close(s->_fdin);
  }break;
  }
}

void waitStage(Stage* s)
{                         // wait for the pipeline stage to complete.
  int st;
  int rv = waitpid(s->_child,&st,0);
}


int setupCommandPipeline(Command* c)
{
  // TODO: Implement the pipeline command
  // There are a number of helper functions given to you.
  // setStageInput: sets the input attribute for a stage in s->_fdin
  // setStageOutput: sets the output attribute for a stage in s->_fdout
  // spawnStage: forks a child process, replaces the stdin and stdout with fdin and fdout respectively, and executes the command for the pipeline stage
  // waitStage: waits for the pipeline stage to complete
  // You may use the helper functions or write your own code to implement the pipeline command. 
  return 1;
}

// ================================================================================
// Write the basic command logic here.
// ================================================================================

int basicExecute(char* com,int mode,char* input,char* output,char** args)
{
  //TODO: implement this command.
  return 1;
}


int executeCommand(Command* c)
{
  switch(c->_kind) {
  case cdCMD: {
    int st = chdir(c->_args[0]);
    if (st==-1)
      printf("cd error: [%s]\n",strerror(errno));
    return TRUE;
  }break;
  case pwdCMD: {
    char buf[1024];
    char* ptr = getcwd(buf,sizeof(buf));
    if (ptr)
      printf("cwd: %s\n",buf);
    return TRUE;
  }break;
  case exitCMD:
    return FALSE;
  case linkCMD: {
    int rv = link(c->_args[0],c->_args[1]);
    if (rv!=0)
      printf("link error [%s]\n",strerror(errno));
    return TRUE;
  }break;
  case rmCMD: {
    int rv = unlink(c->_args[0]);
    if (rv!=0)
      printf("rm error: [%s]\n",strerror(errno));
    return TRUE;
  }break;
  case basicCMD: {
    return basicExecute(c->_com,c->_mode,c->_input,c->_output,c->_args);
  }break;
  case pipelineCMD: {
    setupCommandPipeline(c);
    return TRUE;
  }break;
  default:
    printf("oops....\n");
    return TRUE;
  }
}

int main(int argc,char* argv[])
{
  int loop = 1;
  while(loop) {
    Command* com  = makeCommand();
    printCommand(com);
    loop = executeCommand(com);
    freeCommand(com);
  }
  return 0;
}


