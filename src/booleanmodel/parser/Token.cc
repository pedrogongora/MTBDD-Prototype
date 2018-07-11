/* Generated By:JavaCC: Do not edit this line. Token.cc Version 6.0 */
/* JavaCCOptions:STATIC=false,SUPPORT_CLASS_VISIBILITY_PUBLIC=true */
#include "Token.h"


/**
 * Describes the input token stream.
 */

  /**
   * An optional attribute value of the Token.
   * Tokens which are not used as syntactic sugar will often contain
   * meaningful values that will be used later on by the compiler or
   * interpreter. This attribute value is often different from the image.
   * Any subclass of Token that actually wants to return a non-NULL value can
   * override this method as appropriate.
   */
  void * Token::getValue() {
    return NULL;
  }

  /**
   * No-argument constructor
   */
  Token::Token() {
    this->next = NULL;
    this->specialToken = NULL;
  }

  /**
   * Constructs a new token for the specified Image.
   */
  Token::Token(int kind)
  {
    this->kind = kind;
    this->next = NULL;
    this->specialToken = NULL;
  }

  /**
   * Constructs a new token for the specified Image and Kind.
   */
  Token::Token(int kind, JAVACC_STRING_TYPE image)
  {
    this->kind = kind;
    this->image = image;
    this->next = NULL;
    this->specialToken = NULL;
  }

  /**
   * Returns the image.
   */
  JAVACC_STRING_TYPE Token::toString()
  {
    return image;
  }

  /**
   * Returns a new Token object, by default. However, if you want, you
   * can create and return subclass objects based on the value of ofKind.
   * Simply add the cases to the switch for all those special cases.
   * For example, if you have a subclass of Token called IDToken that
   * you want to create if ofKind is ID, simply add something like :
   *
   *    case MyParserConstants.ID : return new IDToken(ofKind, image);
   *
   * to the following switch statement. Then you can cast matchedToken
   * variable to the appropriate type and use sit in your lexical actions.
   */
  Token *Token::newToken(int ofKind, JAVACC_STRING_TYPE image)
  {
    switch(ofKind)
    {
      default : return new Token(ofKind, image);
    }
  }

  Token *Token::newToken(int ofKind)
  {
    return newToken(ofKind, JAVACC_STRING_TYPE((JAVACC_CHAR_TYPE*)""));
  }

  Token::~Token() {
    if (specialToken) delete specialToken;
  }

/* JavaCC - OriginalChecksum=1c0cf7fb02a88684a2075db5ecd0ac37 (do not edit this line) */
