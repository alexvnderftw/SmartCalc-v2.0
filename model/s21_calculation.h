#ifndef S21_CALCULATION_H
#define S21_CALCULATION_H

// TO ADD:
// 1. Changing comma to dot, and so in output
// 2. Calculate X

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#define _USE_MATH_DEFINES

namespace s21 {
class Calculation {
 public:
  Calculation() = default;
  ~Calculation() = default;

  enum Status {
    READY,
    NEW_EXPRESSION,
    PARSED,
    COMPLETED,
    PARSE_ERROR,
    CALCULATE_ERROR,
    EMPTY
  };

  enum TrigType { RAD, DEG };

  void SetX(double x);
  void SetX(const std::string x_str);
  void SetExpression(const std::string input);
  void SetRadian();
  void SetDegree();

  double GetX();
  const std::string GetExpression();
  TrigType GetTrigValue();
  Status GetStatus();
  double GetResult();
  double GetResult(double x);
  double GetResult(const std::string input);
  double GetResult(const std::string input, double x);
  double GetResult(const std::string input, const std::string x);
  // std::string GetResult(const std::string input, const std::string x);

 private:
  enum TokenType {
    /* Unary functions */
    SQRT,
    LN,
    LOG,
    SIN,
    COS,
    TAN,
    ASIN,
    ACOS,
    ATAN,
    /* Unary operators */
    PLUS,
    MINUS,
    MINUS_ALT,
    /* Binary operators */
    SUM,
    SUB,
    MULT,
    DIV,
    POW,
    MOD,
    /* Other */
    UNDEF,
    NUM,
    LEFT_PAR,
    RIGHT_PAR,
    X
  };

  enum FuncType { REGULAR, TRIGONOMICAL_ARC, TRIGONOMICAL_STRAIGHT };

  struct Token {
    TokenType type = UNDEF;
    double number = NAN;
  };

  Status status_ = READY;
  std::string expr_{};
  double x_ = NAN;
  double result_ = NAN;
  TrigType trig_value_ = RAD;

  /* Parsing variables */
  std::string::iterator iter_;
  std::stack<TokenType> stack_{};
  TokenType prev_ = UNDEF;

  std::vector<Token> output_queue_{};
  std::stack<double> calc_stack_{};

  void Reset();
  void Parse();
  void Calculate();
  void ParseToken();
  void TrimSpaces(std::string& str);

  void CheckHiddenMultiplication();
  bool CheckNumber(std::string::iterator input);
  bool CheckX(std::string::iterator input);
  bool CheckFunction(std::string::iterator input);
  bool CheckLeftParenthesis(std::string::iterator input);
  bool CheckRightParenthesis(std::string::iterator input);
  bool CheckUnarOperator(std::string::iterator input);
  bool CheckOperator(std::string::iterator input);

  TokenType ParseFunction(std::string::iterator input);

  typedef double (*UnaryCallback)(double);
  typedef double (*BinaryCallback)(double, double);

  int GetPriority(TokenType value);
  const std::string GetString(TokenType value);
  UnaryCallback GetUnaryCallback(TokenType value);
  BinaryCallback GetBinaryCallback(TokenType value);
  FuncType GetFunctionType(TokenType value);
  bool IsFunction(TokenType value);
  bool IsBinaryOperator(TokenType value);
  bool IsUnaryOperator(TokenType value);

  static double sum(double a, double b) { return a + b; };
  static double sub(double a, double b) { return a - b; };
  static double mult(double a, double b) { return a * b; };
  static double fdiv(double a, double b) { return a / b; };
  static double plus(double a) { return +a; };
  static double minus(double a) { return -a; };

  /* Key - {Parse pattern, Callback function, Function type} */
  const std::map<TokenType, std::tuple<std::string, UnaryCallback, FuncType>>
      functions = {{SQRT, {"sqrt", std::sqrt, REGULAR}},
                   {LN, {"ln", std::log, REGULAR}},
                   {LOG, {"log", std::log10, REGULAR}},
                   {SIN, {"sin", std::sin, TRIGONOMICAL_STRAIGHT}},
                   {COS, {"cos", std::cos, TRIGONOMICAL_STRAIGHT}},
                   {TAN, {"tan", std::tan, TRIGONOMICAL_STRAIGHT}},
                   {ASIN, {"asin", std::asin, TRIGONOMICAL_ARC}},
                   {ACOS, {"acos", std::acos, TRIGONOMICAL_ARC}},
                   {ATAN, {"atan", std::atan, TRIGONOMICAL_ARC}}};

  /* Key - {Parse pattern, Callback function} */
  const std::map<TokenType, std::pair<std::string, UnaryCallback>>
      unary_operators = {{PLUS, {"+", plus}},
                         {MINUS, {"-", minus}},
                         {MINUS_ALT, {"~", minus}}};

  /* Key - {Parse pattern, Callback function, Priority} */
  const std::map<TokenType, std::tuple<std::string, BinaryCallback, int>>
      operators = {{SUM, {"+", sum, 1}},      {SUB, {"-", sub, 1}},
                   {MULT, {"*", mult, 2}},    {DIV, {"/", fdiv, 2}},
                   {POW, {"^", std::pow, 3}}, {MOD, {"mod", std::fmod, 2}}};
};
}  // namespace s21

#endif  // S21_CALCULATION_H
