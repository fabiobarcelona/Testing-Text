//Validación de parámetros
void checkCommand(int argc, char* argv[]);

//Lista de comandos
void cExtract(int argc, char* argv[]);
void cCountWordsExam(int argc, char* argv[]);
void cSyntax();
void cInfo();
void cCountWord(int argc, char* argv[]);
void cSearchWord(int argc, char* argv[]);
void cNumbers(int argc, char* argv[]);
void cDates(int argc, char* argv[]);
void cReplaceWord(int argc, char* argv[]);

//Funciones auxiliares
int getLongestLine(char fileDir[]);
int getFileLines(char fileDir[]);
void tokenGuestToHost(char* tokenGuest, char fileHost[], int maxLine);
short isNum(char* number);
short isDecimal(char* number);
short isNegative(char* number);
short isDate(char date[]);
char* getNewFileName(char fileName[]);
int replaceAll(char *str, const char *oldWord, const char *newWord, int maxLongLine);