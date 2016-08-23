#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

void print_usage() {
  fprintf(stderr, "Usage: ./project01 [filepath]\n");
}

#define BUFFER_SIZE 4096

typedef std::map<std::string, size_t> FreqMap;

void addWord(char *word, FreqMap &wordFrequency) {
  /* Convert the token string to lowercase */
  char *cp = word;
  while (*cp) {
    *cp = tolower(*cp);
    cp += 1;
  }
  /* Add the word to our frequency map */
  wordFrequency[std::string(word)] += 1;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    print_usage();
  }
  FreqMap wordFrequency;
  const char *filepath = argv[1];
  FILE *fp = fopen(filepath, "r");
  char *buff, *tempBuff;
  buff = (char*)malloc(sizeof(char) * BUFFER_SIZE);
  tempBuff = (char*)malloc(sizeof(char) * BUFFER_SIZE * 2);

  size_t numRead;
  numRead = fread(buff, sizeof(char), BUFFER_SIZE - 1, fp);
  buff[numRead] = '\0';
  const char *sep = " .,;:\"\n\t\r";
  while (numRead > 0) {
    /* NOTE: strtok() is not reentrant */
    char *token = strtok(buff, sep);
    while (token) {
      if (strlen(token) > 0) {
        /* Check for tokens that straddle buffers */
        if (&token[strlen(token)] == &buff[BUFFER_SIZE - 1]) {
          fprintf(stderr, "\e[1mFound token straddling buffer\e[0m\n");
          fprintf(stderr, "\e[1mOld token: '%s'\e[0m\n",
              token);
          size_t oldLength = strlen(token);
          memcpy(tempBuff, token, oldLength);
          /* Read further in the file to get the rest of the token */
          numRead = fread(buff, sizeof(char), BUFFER_SIZE - 1, fp);
          buff[numRead] = '\0';
          token = strtok(buff, sep);
          if (token != buff) {
            /* The token did not actually straddle buffers */
            fprintf(stderr, "\e[1mThe token did not actually straddle the buffer!\e[0m\n");
            tempBuff[oldLength] = '\0';
            fprintf(stderr, "\e[1mSecond word: '%s'\e[0m\n",
                token);
            addWord(token, wordFrequency);
          } else {
            memcpy(tempBuff + oldLength, token, strlen(token) + 1);
          }
          token = tempBuff;
          fprintf(stderr, "\e[1mNew token: '%s'\e[0m\n",
              token);
        }
        /* We found a word; add it to the frequency map */
        addWord(token, wordFrequency);
      }
      /* Look for the next word */
      token = strtok(NULL, sep);
    }
    numRead = fread(buff, sizeof(char), BUFFER_SIZE - 1, fp);
    buff[numRead] = '\0';
  }
  if (ferror(fp)) {
  }

  fclose(fp);

  free(buff);

  fprintf(stdout, "Frequency results:\n");
  for (FreqMap::iterator i = wordFrequency.begin();
      i != wordFrequency.end();
      ++i)
  {
    fprintf(stdout,
        "  '%s': %d\n",
        i->first.c_str(),
        i->second);
  }
}
