#ifndef BOOLEANMODELPARSER_H
#define BOOLEANMODELPARSER_H
#include "JavaCC.h"
#include "CharStream.h"
#include "Token.h"
#include "TokenManager.h"
#include "BooleanModelParserConstants.h"
#include "ErrorHandler.h"

#include "../BooleanModel.h"

#include <string>
#include <list>

  struct JJCalls {
    int gen;
    Token *first;
    int arg;
    JJCalls *next;
    ~JJCalls() { if (next) delete next; }
    JJCalls() { next = NULL; arg = 0; gen = -1; first = NULL; }
  };

class BooleanModelParser {
   public:

list<NextStateAssignment*>* equation_list();

NextStateAssignment* assignment();

list<double>* number_list();

Formula* formula();

Formula* iff_expression();

Formula* impl_expression();

Formula* or_expression();

Formula* and_expression();

Formula* prefix_expression();

Formula* basic_expression();
  public: TokenManager *token_source;
  public: CharStream *jj_input_stream;
  /** Current token. */
  public: Token *token;
  /** Next token. */
  public: Token *jj_nt;
  private: int jj_ntk;
  private: JJCalls jj_2_rtns[1];
  private: bool jj_rescan;
  private: int jj_gc;
  private: Token *jj_scanpos, *jj_lastpos;
  private: int jj_la;
  /** Whether we are looking ahead. */
  private: bool jj_lookingAhead;
  private: bool jj_semLA;
  private: int jj_gen;
  private: int jj_la1[9];
  private: ErrorHandler *errorHandler;
  private: bool errorHandlerCreated;
  public: void setErrorHandler(ErrorHandler *eh) {
    if (errorHandlerCreated) delete errorHandler;
    errorHandler = eh;
    errorHandlerCreated = false;
  }
 Token *head; 
 public: 

 BooleanModelParser(TokenManager *tm);
   public: virtual ~BooleanModelParser();

void ReInit(TokenManager *tm);

Token * jj_consume_token(int kind);

Token * getNextToken();

Token * getToken(int index);

int jj_ntk_f();
 private: int jj_kind;
  int **jj_expentries;
  int *jj_expentry;

  /** Generate ParseException. */

protected: virtual void  parseError();
  private: int trace_indent;
  private: bool trace_enabled;
  /** Enable tracing. */

public: void enable_tracing();

public: void disable_tracing();
private: list<NextStateAssignment*>* assignments;
private: bool jj_done;

};
#endif
