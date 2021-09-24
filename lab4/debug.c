#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct TokenArray{
  char** tokens;
  size_t numTokens;
} TokenArray;
/*
 * dupString should allocate enough memory to hold a copy of s
 * including the null terminator, and copy over each character
 * from s to the duplicate string, and return the duplicate string. 
 */
char* dupString(char* s, size_t len){
  char* duplicate = malloc(len*sizeof(char));
  duplicate[len] = 0;
  for(size_t i = 0; i < len; i++){
    duplicate[i] = s[i];
  }
  return duplicate;
}

/*
Input: A string s, char delim -- the delimiter
Output: The function returns the number of non-empty tokens in the string. 

* note that a sequence of several delimiters is akin to a single delimiter
 * For instance: "Darth-Vader" has 2 tokens and 1 delimiter
 * "-Darth-Vader" is the same (2 tokens)
 *  "Darth---Vader" is still 2 tokens
 *  "Darth- -Vader" is three tokens, 
 *the middle one being a string with a single blank
 * Do make sure you handle all corner cases
 */
int countTokens(char* s,char delim)
{
   int cnt = 0;
   int inToken = *s != delim;
   while (*s) {
      if (*s == delim)
	cnt += inToken;
      inToken = *s != delim;
      s++;
   }
   return cnt;
}

char* tokenEnd(char* from,char delim)
{
   while (*from && *from != delim) from++;
   return from - 1;
}

char* skipDelim(char* from,char delim)
{
   while(*from && *from == delim) from++;
   return from;
}

/*
 *  Break the string into tokens. 
 * As with countTokens, we don't include empty tokens. 

 * For example, "Darth-Vader", "-Darth-Vader" and "Darth---Vader" 
 * are all tokenized to the two tokens: "Darth" and "Vader". 


 * "Darth-- -Vader", however, is broken into: "Darth", " ", and "Vader". 
 */
TokenArray tokenize(char* string, char delim)
{
  size_t numTok = countTokens(string,delim);
  char** tokens = malloc(sizeof(char*)*numTok);  
  size_t tokenIndex = 0;
  char* ts = string;
  while(tokenIndex < numTok) {
     char* te = tokenEnd(ts,delim);
     tokens[tokenIndex] = dupString(ts,te-ts+1);
     ts = skipDelim(te,delim);
     tokenIndex++;
  }
  TokenArray ta;
  ta.tokens = tokens;
  ta.numTokens = numTok;
  return ta;
}

int main(int argc, char* argv[])
{
  printf("Enter string:\n");
  char* string = NULL;
  size_t n = 0;
  ssize_t lineSize = getline(&string, &n, stdin);
  string[lineSize - 1] = '\0';
  printf("Enter delimiter: ");
  char delim;
  assert(scanf("%c", &delim) == 1);

  TokenArray ta = tokenize(string, delim);
  printf("Num tokens: %lu\n", ta.numTokens);
  for(size_t i = 0; i < ta.numTokens; i++){
    printf("token: %s\n", ta.tokens[i]);
    free(ta.tokens[i]);
  }
  free(ta.tokens);
  free(string);
  
  return 0;
}
 
