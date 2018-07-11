/* BooleanModelParserTokenManager.cc */
#include "./BooleanModelParserTokenManager.h"
static const unsigned long long jjbitVec0[] = {
   0x0ULL, 0x0ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL
};
static const int jjnextStates[] = {
   12, 14, 16, 17, 19, 
};
static JAVACC_CHAR_TYPE jjstrLiteralChars_0[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_1[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_2[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_3[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_4[] = {0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_5[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_6[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_7[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_8[] = {0x7e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_9[] = {0x26, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_10[] = {0x7c, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_11[] = {0x3d, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_12[] = {0x2d, 0x3e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_13[] = {0x28, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_14[] = {0x29, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_15[] = {0x5b, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_16[] = {0x5d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_17[] = {0x2e, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_18[] = {0x2c, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_19[] = {0x3a, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_20[] = {0x3b, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_21[] = {0x3a, 0x3d, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_22[] = {0x2a, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_23[] = {0x74, 0x72, 0x75, 0x65, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_24[] = {0x66, 0x61, 0x6c, 0x73, 0x65, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_25[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_26[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_27[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_28[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_29[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_30[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_31[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_32[] = {0};
static const JAVACC_STRING_TYPE jjstrLiteralImages[] = {
jjstrLiteralChars_0, 
jjstrLiteralChars_1, 
jjstrLiteralChars_2, 
jjstrLiteralChars_3, 
jjstrLiteralChars_4, 
jjstrLiteralChars_5, 
jjstrLiteralChars_6, 
jjstrLiteralChars_7, 
jjstrLiteralChars_8, 
jjstrLiteralChars_9, 
jjstrLiteralChars_10, 
jjstrLiteralChars_11, 
jjstrLiteralChars_12, 
jjstrLiteralChars_13, 
jjstrLiteralChars_14, 
jjstrLiteralChars_15, 
jjstrLiteralChars_16, 
jjstrLiteralChars_17, 
jjstrLiteralChars_18, 
jjstrLiteralChars_19, 
jjstrLiteralChars_20, 
jjstrLiteralChars_21, 
jjstrLiteralChars_22, 
jjstrLiteralChars_23, 
jjstrLiteralChars_24, 
jjstrLiteralChars_25, 
jjstrLiteralChars_26, 
jjstrLiteralChars_27, 
jjstrLiteralChars_28, 
jjstrLiteralChars_29, 
jjstrLiteralChars_30, 
jjstrLiteralChars_31, 
jjstrLiteralChars_32, 
};

/** Lexer state names. */
static const JAVACC_CHAR_TYPE lexStateNames_arr_0[] = 
{0x44, 0x45, 0x46, 0x41, 0x55, 0x4c, 0x54, 0};
static const JAVACC_STRING_TYPE lexStateNames[] = {
lexStateNames_arr_0, 
};
static const unsigned long long jjtoToken[] = {
   0x1ffffff01ULL, 
};
static const unsigned long long jjtoSkip[] = {
   0xfeULL, 
};

void  BooleanModelParserTokenManager::setDebugStream(FILE *ds){ debugStream = ds; }

 int BooleanModelParserTokenManager::jjStopStringLiteralDfa_0(int pos, unsigned long long active0){
   switch (pos)
   {
      case 0:
         if ((active0 & 0x20000ULL) != 0L)
            return 4;
         if ((active0 & 0x1800000ULL) != 0L)
         {
            jjmatchedKind = 27;
            jjmatchedPos = 0;
            return -1;
         }
         if ((active0 & 0x30ULL) != 0L)
            return 9;
         return -1;
      case 1:
         if ((active0 & 0x1800000ULL) != 0L)
         {
            if (jjmatchedPos == 0)
            {
               jjmatchedKind = 27;
               jjmatchedPos = 0;
            }
            return -1;
         }
         return -1;
      case 2:
         if ((active0 & 0x1800000ULL) != 0L)
         {
            if (jjmatchedPos == 0)
            {
               jjmatchedKind = 27;
               jjmatchedPos = 0;
            }
            return -1;
         }
         return -1;
      case 3:
         if ((active0 & 0x1800000ULL) != 0L)
         {
            if (jjmatchedPos == 0)
            {
               jjmatchedKind = 27;
               jjmatchedPos = 0;
            }
            return -1;
         }
         return -1;
      default :
         return -1;
   }
}

int  BooleanModelParserTokenManager::jjStartNfa_0(int pos, unsigned long long active0){
   return jjMoveNfa_0(jjStopStringLiteralDfa_0(pos, active0), pos + 1);
}

 int  BooleanModelParserTokenManager::jjStopAtPos(int pos, int kind){
   jjmatchedKind = kind;
   jjmatchedPos = pos;
   return pos + 1;
}

 int  BooleanModelParserTokenManager::jjMoveStringLiteralDfa0_0(){
   switch(curChar)
   {
      case 9:
         return jjStopAtPos(0, 2);
      case 10:
         return jjStopAtPos(0, 3);
      case 13:
         {
         jjmatchedKind = 4;
         jjmatchedPos = 0;
         }
         return jjMoveStringLiteralDfa1_0(0x20ULL);
      case 32:
         return jjStopAtPos(0, 1);
      case 38:
         return jjStopAtPos(0, 9);
      case 40:
         return jjStopAtPos(0, 13);
      case 41:
         return jjStopAtPos(0, 14);
      case 42:
         return jjStopAtPos(0, 22);
      case 44:
         return jjStopAtPos(0, 18);
      case 45:
         return jjMoveStringLiteralDfa1_0(0x1000ULL);
      case 46:
         return jjStartNfaWithStates_0(0, 17, 4);
      case 58:
         {
         jjmatchedKind = 19;
         jjmatchedPos = 0;
         }
         return jjMoveStringLiteralDfa1_0(0x200000ULL);
      case 59:
         return jjStopAtPos(0, 20);
      case 61:
         return jjStopAtPos(0, 11);
      case 91:
         return jjStopAtPos(0, 15);
      case 93:
         return jjStopAtPos(0, 16);
      case 102:
         return jjMoveStringLiteralDfa1_0(0x1000000ULL);
      case 116:
         return jjMoveStringLiteralDfa1_0(0x800000ULL);
      case 124:
         return jjStopAtPos(0, 10);
      case 126:
         return jjStopAtPos(0, 8);
      default :
         return jjMoveNfa_0(0, 0);
   }
}

 int  BooleanModelParserTokenManager::jjMoveStringLiteralDfa1_0(unsigned long long active0){
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(0, active0);
      return 1;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 10:
         if ((active0 & 0x20ULL) != 0L)
            return jjStopAtPos(1, 5);
         break;
      case 61:
         if ((active0 & 0x200000ULL) != 0L)
            return jjStopAtPos(1, 21);
         break;
      case 62:
         if ((active0 & 0x1000ULL) != 0L)
            return jjStopAtPos(1, 12);
         break;
      case 97:
         return jjMoveStringLiteralDfa2_0(active0, 0x1000000ULL);
      case 114:
         return jjMoveStringLiteralDfa2_0(active0, 0x800000ULL);
      default :
         break;
   }
   return jjStartNfa_0(0, active0);
}

 int  BooleanModelParserTokenManager::jjMoveStringLiteralDfa2_0(unsigned long long old0, unsigned long long active0){
   if (((active0 &= old0)) == 0L)
      return jjStartNfa_0(0, old0);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(1, active0);
      return 2;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 108:
         return jjMoveStringLiteralDfa3_0(active0, 0x1000000ULL);
      case 117:
         return jjMoveStringLiteralDfa3_0(active0, 0x800000ULL);
      default :
         break;
   }
   return jjStartNfa_0(1, active0);
}

 int  BooleanModelParserTokenManager::jjMoveStringLiteralDfa3_0(unsigned long long old0, unsigned long long active0){
   if (((active0 &= old0)) == 0L)
      return jjStartNfa_0(1, old0);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(2, active0);
      return 3;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 101:
         if ((active0 & 0x800000ULL) != 0L)
            return jjStopAtPos(3, 23);
         break;
      case 115:
         return jjMoveStringLiteralDfa4_0(active0, 0x1000000ULL);
      default :
         break;
   }
   return jjStartNfa_0(2, active0);
}

 int  BooleanModelParserTokenManager::jjMoveStringLiteralDfa4_0(unsigned long long old0, unsigned long long active0){
   if (((active0 &= old0)) == 0L)
      return jjStartNfa_0(2, old0);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(3, active0);
      return 4;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 101:
         if ((active0 & 0x1000000ULL) != 0L)
            return jjStopAtPos(4, 24);
         break;
      default :
         break;
   }
   return jjStartNfa_0(3, active0);
}

int BooleanModelParserTokenManager::jjStartNfaWithStates_0(int pos, int kind, int state){
   jjmatchedKind = kind;
   jjmatchedPos = pos;
   if (input_stream->endOfInput()) { return pos + 1; }
   curChar = input_stream->readChar();
   return jjMoveNfa_0(state, pos + 1);
}

int BooleanModelParserTokenManager::jjMoveNfa_0(int startState, int curPos){
   int startsAt = 0;
   jjnewStateCnt = 21;
   int i = 1;
   jjstateSet[0] = startState;
   int kind = 0x7fffffff;
   for (;;)
   {
      if (++jjround == 0x7fffffff)
         ReInitRounds();
      if (curChar < 64)
      {
         unsigned long long l = 1ULL << curChar;
         if (l == 1);
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if ((0x3ff000000000000ULL & l) != 0L)
                  {
                     if (kind > 26)
                        kind = 26;
                     jjCheckNAddTwoStates(20, 3);
                  }
                  else if ((0x100002600ULL & l) != 0L)
                  {
                     if (kind > 31)
                        kind = 31;
                  }
                  else if (curChar == 47)
                     jjAddStates(0, 1);
                  else if (curChar == 46)
                  {
                     if (kind > 26)
                        kind = 26;
                     jjCheckNAdd(4);
                  }
                  if ((0x3fe000000000000ULL & l) != 0L)
                  {
                     if (kind > 29)
                        kind = 29;
                  }
                  else if (curChar == 13)
                     jjstateSet[jjnewStateCnt++] = 9;
                  if ((0x3000000000000ULL & l) != 0L)
                  {
                     if (kind > 30)
                        kind = 30;
                  }
                  break;
               case 1:
                  if ((0x3fe000000000000ULL & l) == 0L)
                     break;
                  if (kind > 25)
                     kind = 25;
                  jjCheckNAdd(2);
                  break;
               case 2:
                  if ((0x3ff000000000000ULL & l) == 0L)
                     break;
                  if (kind > 25)
                     kind = 25;
                  jjCheckNAdd(2);
                  break;
               case 3:
                  if (curChar != 46)
                     break;
                  if (kind > 26)
                     kind = 26;
                  jjCheckNAdd(4);
                  break;
               case 4:
                  if ((0x3ff000000000000ULL & l) == 0L)
                     break;
                  if (kind > 26)
                     kind = 26;
                  jjCheckNAdd(4);
                  break;
               case 6:
                  if ((0x3fe000000000000ULL & l) != 0L && kind > 29)
                     kind = 29;
                  break;
               case 7:
                  if ((0x3000000000000ULL & l) != 0L && kind > 30)
                     kind = 30;
                  break;
               case 8:
                  if ((0x100002600ULL & l) != 0L && kind > 31)
                     kind = 31;
                  break;
               case 9:
                  if (curChar == 10 && kind > 31)
                     kind = 31;
                  break;
               case 10:
                  if (curChar == 13)
                     jjstateSet[jjnewStateCnt++] = 9;
                  break;
               case 11:
                  if (curChar == 47)
                     jjAddStates(0, 1);
                  break;
               case 12:
                  if (curChar != 47)
                     break;
                  if (kind > 6)
                     kind = 6;
                  jjCheckNAdd(13);
                  break;
               case 13:
                  if ((0xffffffffffffdbffULL & l) == 0L)
                     break;
                  if (kind > 6)
                     kind = 6;
                  jjCheckNAdd(13);
                  break;
               case 14:
                  if (curChar == 42)
                     jjCheckNAddTwoStates(15, 16);
                  break;
               case 15:
                  if ((0xfffffbffffffffffULL & l) != 0L)
                     jjCheckNAddTwoStates(15, 16);
                  break;
               case 16:
                  if (curChar == 42)
                     jjCheckNAddStates(2, 4);
                  break;
               case 17:
                  if ((0xffff7bffffffffffULL & l) != 0L)
                     jjCheckNAddTwoStates(18, 16);
                  break;
               case 18:
                  if ((0xfffffbffffffffffULL & l) != 0L)
                     jjCheckNAddTwoStates(18, 16);
                  break;
               case 19:
                  if (curChar == 47 && kind > 7)
                     kind = 7;
                  break;
               case 20:
                  if ((0x3ff000000000000ULL & l) == 0L)
                     break;
                  if (kind > 26)
                     kind = 26;
                  jjCheckNAddTwoStates(20, 3);
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else if (curChar < 128)
      {
         unsigned long long l = 1ULL << (curChar & 077);
         if (l == 1);
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if ((0x7fffffe07fffffeULL & l) != 0L)
                  {
                     if (kind > 27)
                        kind = 27;
                  }
                  if (curChar == 120)
                     jjstateSet[jjnewStateCnt++] = 1;
                  break;
               case 5:
                  if ((0x7fffffe07fffffeULL & l) != 0L && kind > 27)
                     kind = 27;
                  break;
               case 13:
                  if (kind > 6)
                     kind = 6;
                  jjstateSet[jjnewStateCnt++] = 13;
                  break;
               case 15:
                  jjCheckNAddTwoStates(15, 16);
                  break;
               case 17:
               case 18:
                  jjCheckNAddTwoStates(18, 16);
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else
      {
         int i2 = (curChar & 0xff) >> 6;
         unsigned long long l2 = 1ULL << (curChar & 077);
         do
         {
            switch(jjstateSet[--i])
            {
               case 13:
                  if ((jjbitVec0[i2] & l2) == 0L)
                     break;
                  if (kind > 6)
                     kind = 6;
                  jjstateSet[jjnewStateCnt++] = 13;
                  break;
               case 15:
                  if ((jjbitVec0[i2] & l2) != 0L)
                     jjCheckNAddTwoStates(15, 16);
                  break;
               case 17:
               case 18:
                  if ((jjbitVec0[i2] & l2) != 0L)
                     jjCheckNAddTwoStates(18, 16);
                  break;
               default : break; //if (i1 == 0 || l1 == 0 || i2 == 0 || l2 == 0) break; else break;
            }
         } while(i != startsAt);
      }
      if (kind != 0x7fffffff)
      {
         jjmatchedKind = kind;
         jjmatchedPos = curPos;
         kind = 0x7fffffff;
      }
      ++curPos;
      if ((i = jjnewStateCnt), (jjnewStateCnt = startsAt), (i == (startsAt = 21 - startsAt)))
         return curPos;
      if (input_stream->endOfInput()) { return curPos; }
      curChar = input_stream->readChar();
   }
}

/** Token literal values. */

Token * BooleanModelParserTokenManager::jjFillToken(){
   Token *t;
   JAVACC_STRING_TYPE curTokenImage;
   int beginLine;
   int endLine;
   int beginColumn;
   int endColumn;
   if (jjmatchedPos < 0)
   {
       curTokenImage = image.c_str();
      beginLine = endLine = input_stream->getBeginLine();
      beginColumn = endColumn = input_stream->getBeginColumn();
   }
   else
   {
      JAVACC_STRING_TYPE im = jjstrLiteralImages[jjmatchedKind];
      curTokenImage = (im.length() == 0) ? input_stream->GetImage() : im;
      beginLine = input_stream->getBeginLine();
      beginColumn = input_stream->getBeginColumn();
      endLine = input_stream->getEndLine();
      endColumn = input_stream->getEndColumn();
   }
   t = Token::newToken(jjmatchedKind, curTokenImage);
   t->specialToken = NULL;
   t->next = NULL;

   t->beginLine = beginLine;
   t->endLine = endLine;
   t->beginColumn = beginColumn;
   t->endColumn = endColumn;

   return t;
}
const int defaultLexState = 0;
/** Get the next Token. */

Token * BooleanModelParserTokenManager::getNextToken(){
  Token *matchedToken;
  int curPos = 0;

  for (;;)
  {
   EOFLoop: 
   if (input_stream->endOfInput())
   {
      jjmatchedKind = 0;
      jjmatchedPos = -1;
      matchedToken = jjFillToken();
      return matchedToken;
   }
   curChar = input_stream->BeginToken();
   image = jjimage;
   image.clear();
   jjimageLen = 0;

   jjmatchedKind = 26;
   jjmatchedPos = -1;
   curPos = 0;
   curPos = jjMoveStringLiteralDfa0_0();
   if (jjmatchedPos < 0 || (jjmatchedPos == 0 && jjmatchedKind > 32))
   {
      jjmatchedKind = 32;
      jjmatchedPos = 0;
   }
   if (jjmatchedKind != 0x7fffffff)
   {
      if (jjmatchedPos + 1 < curPos)
         input_stream->backup(curPos - jjmatchedPos - 1);
      if ((jjtoToken[jjmatchedKind >> 6] & (1ULL << (jjmatchedKind & 077))) != 0L)
      {
         matchedToken = jjFillToken();
         TokenLexicalActions(matchedToken);
         return matchedToken;
      }
      else
      {
         goto EOFLoop;
      }
   }
   int error_line = input_stream->getEndLine();
   int error_column = input_stream->getEndColumn();
   JAVACC_STRING_TYPE error_after;
   bool EOFSeen = false;
   if (input_stream->endOfInput()) {
      EOFSeen = true;
      error_after = curPos <= 1 ? EMPTY : input_stream->GetImage();
      if (curChar == '\n' || curChar == '\r') {
         error_line++;
         error_column = 0;
      }
      else
         error_column++;
   }
   if (!EOFSeen) {
      error_after = curPos <= 1 ? EMPTY : input_stream->GetImage();
   }
   lexicalError();
  }
}


void  BooleanModelParserTokenManager::TokenLexicalActions(Token *matchedToken){
   switch(jjmatchedKind)
   {
      default :
         break;
   }
}

void BooleanModelParserTokenManager::jjCheckNAdd(int state){
   if (jjrounds[state] != jjround)
   {
      jjstateSet[jjnewStateCnt++] = state;
      jjrounds[state] = jjround;
   }
}

void BooleanModelParserTokenManager::jjAddStates(int start, int end){
   do {
      jjstateSet[jjnewStateCnt++] = jjnextStates[start];
   } while (start++ != end);
}

void BooleanModelParserTokenManager::jjCheckNAddTwoStates(int state1, int state2){
   jjCheckNAdd(state1);
   jjCheckNAdd(state2);
}


void BooleanModelParserTokenManager::jjCheckNAddStates(int start, int end){
   do {
      jjCheckNAdd(jjnextStates[start]);
   } while (start++ != end);
}

  /** Reinitialise parser. */
  void BooleanModelParserTokenManager::ReInit(CharStream *stream, int lexState, BooleanModelParser *parserArg) {
    if (input_stream) delete input_stream;
    jjmatchedPos = jjnewStateCnt = 0;
    curLexState = lexState;
    input_stream = stream;
    ReInitRounds();
    debugStream = stdout; // init
    SwitchTo(lexState);
    parser = parserArg;
  }

  void BooleanModelParserTokenManager::ReInitRounds() {
    int i;
    jjround = 0x80000001;
    for (i = 21; i-- > 0;)
      jjrounds[i] = 0x80000000;
  }

  /** Switch to specified lex state. */
  void BooleanModelParserTokenManager::SwitchTo(int lexState) {
    if (lexState >= 1 || lexState < 0)
      assert(false);
      //throw 1;//new TokenMgrError("Error: Ignoring invalid lexical state : " + lexState + ". State unchanged.", TokenMgrError.INVALID_LEXICAL_STATE);
    else
      curLexState = lexState;
  }

  /** Constructor. */
  BooleanModelParserTokenManager::BooleanModelParserTokenManager (CharStream *stream, int lexState, BooleanModelParser *parserArg){
    input_stream = NULL;
    ReInit(stream, lexState, parserArg);
  }

  // Destructor
  BooleanModelParserTokenManager::~BooleanModelParserTokenManager () {
    if (input_stream) delete input_stream;
  }
