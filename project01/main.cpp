/* Project 1 -- String Search
 *
 * Copyright 2016 Jonathan Glines
 * glinjona@isu.edu
 *
 * 1. Search a string of characters. 
 * 2. Report total characters and total words.
 * 3. Report the frequency of each
 */

#include <cctype>
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

void print_usage() {
  fprintf(stderr, "Usage: ./project01 [filepath]\n");
}

#define BUFFER_SIZE 4096

typedef std::map<char, size_t> CharFreqMap;
typedef std::map<std::string, size_t> WordFreqMap;

void addWord(char *word, size_t &total, WordFreqMap &wordFrequency) {
  total += 1;
  /* Convert the token string to lowercase */
  char *cp = word;
  while (*cp) {
    *cp = tolower(*cp);
    cp += 1;
  }
  /* Add the word to our frequency map */
  wordFrequency[std::string(word)] += 1;
}

void addCharFreq(
    char *buff, size_t size,
    size_t &total, CharFreqMap &charFrequency)
{
  for (int i = 0; i < size; ++i) {
    /* NOTE: The assignment did not specify, but I am counting the printable
     * characters only */
    if (isprint(buff[i])) {
      charFrequency[buff[i]] += 1;
      total += 1;
    }
  }
}

template <class FreqMap>
void printFreqMap(const FreqMap &frequency) {
  for (typename FreqMap::const_iterator i = frequency.begin();
      i != frequency.end();
      ++i)
  {
    std::cout << "  '" << i->first << "': " << i->second << std::endl;
  }
}

int main(int argc, char **argv) {
  const char *filepath;
  if (argc == 1) {
    filepath = "-";
  } else if (argc != 2) {
    print_usage();
    return EXIT_FAILURE;
  } else {
    filepath = argv[1];
  }
  CharFreqMap charFrequency;
  WordFreqMap wordFrequency;
  size_t charTotal, wordTotal;
  charTotal = wordTotal = 0;
  FILE *fp;
  if (strcmp(filepath, "-") == 0) {
    fp = stdin;
  } else {
    fp = fopen(filepath, "r");
  }
  char *buff, *tempBuff;
  buff = (char*)malloc(sizeof(char) * BUFFER_SIZE);
  tempBuff = (char*)malloc(sizeof(char) * BUFFER_SIZE * 2);

  size_t numRead;
  numRead = fread(buff, sizeof(char), BUFFER_SIZE - 1, fp);
  buff[numRead] = '\0';
  addCharFreq(buff, numRead, charTotal, charFrequency);
  const char *sep = " .,;:\"\n\t\r";
  while (numRead > 0) {
    /* NOTE: strtok() is not reentrant */
    char *token = strtok(buff, sep);
    while (token) {
      if (strlen(token) > 0) {
        /* Check for tokens that straddle buffers */
        if (&token[strlen(token)] == &buff[BUFFER_SIZE - 1]) {
          size_t oldLength = strlen(token);
          memcpy(tempBuff, token, oldLength);
          /* Read further in the file to get the rest of the token */
          numRead = fread(buff, sizeof(char), BUFFER_SIZE - 1, fp);
          buff[numRead] = '\0';
          addCharFreq(buff, numRead, charTotal, charFrequency);
          token = strtok(buff, sep);
          if (token != buff) {
            /* The token did not actually straddle buffers */
            tempBuff[oldLength] = '\0';
            addWord(token, wordTotal, wordFrequency);
          } else {
            memcpy(tempBuff + oldLength, token, strlen(token) + 1);
          }
          token = tempBuff;
        }
        /* We found a word; add it to the frequency map */
        addWord(token, wordTotal, wordFrequency);
      }
      /* Look for the next word */
      token = strtok(NULL, sep);
    }
    numRead = fread(buff, sizeof(char), BUFFER_SIZE - 1, fp);
    buff[numRead] = '\0';
    addCharFreq(buff, numRead, charTotal, charFrequency);
  }

  if (ferror(fp)) {
    fprintf(stderr, "Error reading file '%s'\n", filepath);
    return EXIT_FAILURE;
  }

  /* Free our resources */
  if (fp != stdin) {
    fclose(fp);
  }
  free(buff);

  /* Print our results */
  fprintf(stdout, "Character frequency results:\n");
  printFreqMap(charFrequency);
  fprintf(stdout, "Word frequency results:\n");
  printFreqMap(wordFrequency);
  fprintf(stdout,
      "Character total: %d\n"
      "Word total: %d\n",
      charTotal,
      wordTotal);

  return EXIT_SUCCESS;
}
