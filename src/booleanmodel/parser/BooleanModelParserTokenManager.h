#ifndef BOOLEANMODELPARSERTOKENMANAGER_H
#define BOOLEANMODELPARSERTOKENMANAGER_H
#include "JavaCC.h"
#include "CharStream.h"
#include "Token.h"
#include "TokenManager.h"
#include "BooleanModelParserConstants.h"
class BooleanModelParser;

/** Token Manager. */
class BooleanModelParserTokenManager : public TokenManager {
   public:

  /** Debug output. */
  FILE *debugStream;
  /** Set debug output. */

void  setDebugStream(FILE *ds);

 int jjStopStringLiteralDfa_0(int pos, unsigned long long active0);

int  jjStartNfa_0(int pos, unsigned long long active0);

 int  jjStopAtPos(int pos, int kind);

 int  jjMoveStringLiteralDfa0_0();

 int  jjMoveStringLiteralDfa1_0(unsigned long long active0);

 int  jjMoveStringLiteralDfa2_0(unsigned long long old0, unsigned long long active0);

 int  jjMoveStringLiteralDfa3_0(unsigned long long old0, unsigned long long active0);

 int  jjMoveStringLiteralDfa4_0(unsigned long long old0, unsigned long long active0);

int jjStartNfaWithStates_0(int pos, int kind, int state);

int jjMoveNfa_0(int startState, int curPos);

Token * jjFillToken();

public: int curLexState;
public: int jjnewStateCnt;
public: int jjround;
public: int jjmatchedPos;
public: int jjmatchedKind;


Token * getNextToken();

void  TokenLexicalActions(Token *matchedToken);

void jjCheckNAdd(int state);

void jjAddStates(int start, int end);

void jjCheckNAddTwoStates(int state1, int state2);

void jjCheckNAddStates(int start, int end);
  private: BooleanModelParser*parser;
  private: void ReInitRounds();
  public: BooleanModelParserTokenManager(CharStream *stream, int lexState = 0, BooleanModelParser *parserArg = NULL);
  public: virtual ~BooleanModelParserTokenManager();
  void ReInit(CharStream *stream, int lexState = 0, BooleanModelParser *parserArg = NULL);
  void SwitchTo(int lexState);
  const JAVACC_SIMPLE_STRING jjKindsForBitVector(int i, unsigned long long vec);
  const JAVACC_SIMPLE_STRING jjKindsForStateVector(int lexState, int vec[], int start, int end);
    CharStream  *input_stream;
    int jjrounds[21];
    int jjstateSet[2 * 21];
    JAVACC_STRING_TYPE jjimage;
    JAVACC_STRING_TYPE image;
    int jjimageLen;
    int lengthOfMatch;
    JAVACC_CHAR_TYPE curChar;
};
#endif
