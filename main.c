#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#if defined(__GNUC__)
    #include <stdint-gcc.h>
#else
    #include <stdint.h>
#endif



#define FOUNDWORD_COUNT_DEFAULT 8

typedef struct FoundWord {
    uint16_t lineNumber;
    char* lineContent;
} FoundWord;

typedef struct FoundResult {
    size_t number;
    FoundWord* foundWords;
} FoundResult;

#define IS_LETTER(c) (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
#define TO_UPPER(c)  (c - (c >= 'a') * 32)
bool compareWordsAfterSecondLetter(const size_t length, char a[], char b[]);
void checkAlloc(const void *allocated);

FoundResult* seek(char text[], const size_t textLength, char search[], const size_t searchLength);


inline bool compareWordsAfterSecondLetter(const size_t length, char a[], char b[]) {
    for (size_t i = 1; i < length; i++) {
        if (TO_UPPER(a[i]) != TO_UPPER(b[i])) {
            return false;
        }
    }
    return true;
}

void checkAlloc(const void *allocated) {
    if (allocated == NULL) {
        perror("UNABLE TO ALLOCATE MEMORY");
        exit(1);
    }
}

FoundResult* seek(char text[], const size_t textLength, char search[], const size_t searchLength) {
    size_t wordTuplesCount = 0;
    size_t wordTuplesMax = FOUNDWORD_COUNT_DEFAULT;
    FoundWord *wordTuples = malloc(FOUNDWORD_COUNT_DEFAULT * sizeof(FoundWord));
    checkAlloc(wordTuples);
    
    uint16_t lineNumber = 0;
    for (size_t i = 0; i < textLength; i++) {
        if (text[i] == '\n') {
            lineNumber++;
            continue;
        }

        uint16_t wordLength = 0;
        while (IS_LETTER(text[i + wordLength])) {
            wordLength++;
        }

        if (searchLength == wordLength && TO_UPPER(text[i]) == TO_UPPER(search[0]) && compareWordsAfterSecondLetter(searchLength, search, text + i * sizeof(char))) {
            if (wordTuplesCount == wordTuplesMax) {
                wordTuplesMax *= 2;
                wordTuples = realloc(wordTuples, wordTuplesMax * sizeof(FoundWord));
                checkAlloc(wordTuples);
            }
            wordTuples[wordTuplesCount].lineNumber = lineNumber;

            wordTuples[wordTuplesCount].lineContent = malloc(wordLength * sizeof(char));
            checkAlloc(wordTuples[wordTuplesCount].lineContent);
            memcpy(
                wordTuples[wordTuplesCount].lineContent,
                text + i * sizeof(char),
                wordLength * sizeof(char)
            );
            wordTuplesCount++;
        } else {
            i += wordLength;
        }
    }

    FoundResult *result = malloc(sizeof(FoundResult));
    result->number = wordTuplesCount;
    result->foundWords = wordTuples;
    return result;
}

int main() {
    // read file -- change path if necessary @everyone
    FILE *file = fopen("../res/sentences.txt", "r");
    if (file == NULL) {
        perror("UNABLE TO OPEN FILE");
        exit(1);
    }
    fseek(file, 0L, SEEK_END);
    const size_t length = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char text[length + 1];
    fread(text, sizeof(char), length, file);
    text[length] = '\0';
    fclose(file);

    // actual function call
    FoundResult* wordTuples = seek(text, length, "python", strlen("python"));

    // show output
    for (size_t i = 0; i < wordTuples->number; i++) {
        printf("\n Found on line %d: %s", wordTuples->foundWords[i].lineNumber, wordTuples->foundWords[i].lineContent);
    }

    // free memory
    for (size_t i = 0; i < wordTuples->number; i++) {
        free(wordTuples->foundWords[i].lineContent);
    }
    free(wordTuples);

    return 0;
}
