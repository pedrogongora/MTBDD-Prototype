#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include <sstream>

/* BooleanModelParser.cc */
#include "./BooleanModelParser.h"
  unsigned int jj_la1_0[] = {
0x2000000,0x40000,0x800,0x1000,0x400,0x200,0x3802100,0x3802000,};
  unsigned int jj_la1_1[] = {
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,};

  /** Constructor with user supplied TokenManager. */



double parseDouble(string* s) {
	stringstream ss(*s);
	double d;
	ss >> d;
	return d;
}


list<NextStateAssignment*>* BooleanModelParser::equation_list() {NextStateAssignment* assign;
assignments = new list<NextStateAssignment*>();
    while (true) {
      assign = assignment();
assignments->push_back(assign);
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case IDENTIFIER:{
        ;
        break;
        }
      default:
        jj_la1[0] = jj_gen;
        goto end_label_1;
      }
    }
    end_label_1: ;
    jj_consume_token(0);
return assignments;
assert(false);
  }


NextStateAssignment* BooleanModelParser::assignment() {Token* t;
        Formula* f1;
        list<double>* costs;
    t = jj_consume_token(IDENTIFIER);
    jj_consume_token(ASSIGN);
    f1 = formula();
    jj_consume_token(COLON);
    costs = number_list();
    jj_consume_token(SEMICOLON);
return new NextStateAssignment(new Variable(new string((t->image).c_str())), f1, costs);
assert(false);
  }


list<double>* BooleanModelParser::number_list() {Token* t;
        list<double>* l = new list<double>();
    jj_consume_token(LBRACKET);
    t = jj_consume_token(NUMBER);
l->push_back(parseDouble(new string((t->image).c_str())));
    while (true) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case COMMA:{
        ;
        break;
        }
      default:
        jj_la1[1] = jj_gen;
        goto end_label_2;
      }
      jj_consume_token(COMMA);
      t = jj_consume_token(NUMBER);
l->push_back(parseDouble(new string((t->image).c_str())));
    }
    end_label_2: ;
    jj_consume_token(RBRACKET);
return l;
assert(false);
  }


Formula* BooleanModelParser::formula() {Formula* f;
    f = iff_expression();
return f;
assert(false);
  }


Formula* BooleanModelParser::iff_expression() {Formula* f1; Formula* f2;
        Token* t;
    f1 = impl_expression();
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case IFF:{
      t = jj_consume_token(IFF);
      f2 = iff_expression();
f1 = new Iff(f1, f2);
      break;
      }
    default:
      jj_la1[2] = jj_gen;
      ;
    }
return f1;
assert(false);
  }


Formula* BooleanModelParser::impl_expression() {Formula* f1; Formula* f2;
        Token* t;
    f1 = or_expression();
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case IMPLIES:{
      t = jj_consume_token(IMPLIES);
      f2 = impl_expression();
f1 = new Implication(f1, f2);
      break;
      }
    default:
      jj_la1[3] = jj_gen;
      ;
    }
return f1;
assert(false);
  }


Formula* BooleanModelParser::or_expression() {Formula* f1; Formula* f2;
        Token* t;
    f1 = and_expression();
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case OR:{
      t = jj_consume_token(OR);
      f2 = or_expression();
f1 = new Or(f1, f2);
      break;
      }
    default:
      jj_la1[4] = jj_gen;
      ;
    }
return f1;
assert(false);
  }


Formula* BooleanModelParser::and_expression() {Formula* f1; Formula* f2;
        Token* t;
    f1 = prefix_expression();
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case AND:{
      t = jj_consume_token(AND);
      f2 = and_expression();
f1 = new And(f1, f2);
      break;
      }
    default:
      jj_la1[5] = jj_gen;
      ;
    }
return f1;
assert(false);
  }


Formula* BooleanModelParser::prefix_expression() {Formula* f;
        Token*   t;
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case NOT:{
      t = jj_consume_token(NOT);
      f = prefix_expression();
return new Not(f);
      break;
      }
    case LPAR:
    case TRUE:
    case FALSE:
    case IDENTIFIER:{
      f = basic_expression();
return f;
      break;
      }
    default:
      jj_la1[6] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this);
    }
assert(false);
  }


Formula* BooleanModelParser::basic_expression() {Formula* f;
        Token* t,v;
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case IDENTIFIER:{
      t = jj_consume_token(IDENTIFIER);
return new Variable(new string((t->image).c_str()));
      break;
      }
    case TRUE:{
      t = jj_consume_token(TRUE);
return new BooleanConstant(true);
      break;
      }
    case FALSE:{
      t = jj_consume_token(FALSE);
return new BooleanConstant(false);
      break;
      }
    case LPAR:{
      jj_consume_token(LPAR);
      f = iff_expression();
      jj_consume_token(RPAR);
return f;
      break;
      }
    default:
      jj_la1[7] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this);
    }
assert(false);
  }


 BooleanModelParser::BooleanModelParser(TokenManager *tm){
    head = NULL;
    ReInit(tm);
}
   BooleanModelParser::~BooleanModelParser()
{
  if (token_source) delete token_source;
  if (head) {
    Token *next, *t = head;
    while (t) {
      next = t->next;
      delete t;
      t = next;
    }
  }
  if (errorHandlerCreated) {
    delete errorHandler;
  }
}

void BooleanModelParser::ReInit(TokenManager *tm){
    if (head) delete head;
    errorHandler = new ErrorHandler();
    errorHandlerCreated = true;
    token_source = tm;
    head = token = new Token();
    token->kind = 0;
    token->next = NULL;
    jj_lookingAhead = false;
    jj_rescan = false;
    jj_done = false;
    jj_scanpos = jj_lastpos = NULL;
    jj_gc = 0;
    jj_kind = -1;
    trace_indent = 0;
    trace_enabled = false;
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 8; i++) jj_la1[i] = -1;
  }


Token * BooleanModelParser::jj_consume_token(int kind)  {
    Token *oldToken;
    if ((oldToken = token)->next != NULL) token = token->next;
    else token = token->next = token_source->getNextToken();
    jj_ntk = -1;
    if (token->kind == kind) {
      jj_gen++;
      return token;
    }
    token = oldToken;
    jj_kind = kind;
    JAVACC_STRING_TYPE image = kind >= 0 ? tokenImage[kind] : tokenImage[0];
    errorHandler->handleUnexpectedToken(kind, image.substr(1, image.size() - 2), getToken(1), this);
    return token;
  }


/** Get the next Token. */

Token * BooleanModelParser::getNextToken(){
    if (token->next != NULL) token = token->next;
    else token = token->next = token_source->getNextToken();
    jj_ntk = -1;
    jj_gen++;
    return token;
  }

/** Get the specific Token. */

Token * BooleanModelParser::getToken(int index){
    Token *t = token;
    for (int i = 0; i < index; i++) {
      if (t->next != NULL) t = t->next;
      else t = t->next = token_source->getNextToken();
    }
    return t;
  }


int BooleanModelParser::jj_ntk_f(){
    if ((jj_nt=token->next) == NULL)
      return (jj_ntk = (token->next=token_source->getNextToken())->kind);
    else
      return (jj_ntk = jj_nt->kind);
  }


 void  BooleanModelParser::parseError()   {
      fprintf(stderr, "Parse error at: %d:%d, after token: %s encountered: %s\n", token->beginLine, token->beginColumn, addUnicodeEscapes(token->image).c_str(), addUnicodeEscapes(getToken(1)->image).c_str());
   }


 void BooleanModelParser::enable_tracing()  {
  }

  /** Disable tracing. */

 void BooleanModelParser::disable_tracing()  {
  }


