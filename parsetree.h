/* File Name: parsetree.h
 * Author: William Ferdinando
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
#include "tokens.h"

using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };
extern void RunTimeError (string);
extern int fileLineNum;

// The object holds boolean, integer, or string, and remembers which type it holds
class Value {
    bool bval;
    int ival;
    string sval;
    enum VT { isBool, isInt, isString, isTypeError } type;

public:
    Value() : bval(false), ival(0), type(isTypeError) {}
    Value(bool b) : bval(b), ival(0), type(isBool) {}
    Value(int i) : bval(false), ival(i), type(isInt) {}
    Value(string s) : bval(false), ival(0), sval(s), type(isString) {}
    
    // The Value object holds the error message
    Value(string sval, bool isError) : bval(false), ival(0), sval(sval), type(isTypeError) {}

    bool isBoolType() const 
    { 
        return type == VT::isBool;
    }
    bool isIntType() const
    { 
        return type == VT::isInt; 
    }
    bool isStringType() const 
    {
        return type == VT::isString; 
    }
    bool isError() const 
    { 
        return type == VT::isTypeError;
    }
    bool hasMessage() const 
    { 
        return isError() && sval.size() > 0; 
    }
    bool isTrue() const 
    { 
        return isBoolType() && bval; 
    }
    bool getBoolean() const 
    {
        if ( !isBoolType() )
        {
            // Throw "Not boolean valued" error message
            return 0;
        }
        return bval;
    }

    int getInteger() const
    {
        if ( !isIntType() )
        {
            // Throw "Not integer valued" error message
            return 0;
        }
        return ival;
    }

    string getString() const
    {
        if ( !isStringType() )
        {
            // Throw "Not string valued" error message
            return 0;
        }
        return sval;
    }

    string getMessage() const 
    {
        if ( !hasMessage() )
        {
            // Throw "No message" error message
            return 0;
        }
        return sval;
    }

    friend ostream& operator<<(ostream& out, const Value& v) 
    {
        if ( v.type == VT::isBool )
        {
            out << (v.bval ? "True" : "False");
        }
        else if ( v.type == VT::isInt )
        {
            out << v.ival;
        }
        else if ( v.type == VT::isString )
        {
            out << v.sval;
        }
        else if ( v.sval.size() > 0 )
        {
            out << "RUNTIME ERROR " << v.sval;
        }
        return out;
    }

    Value operator+(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival + v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval + v.sval);
        }
        
        RunTimeError("Cannot add these two values");
        return Value();
    }

    Value operator-(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival - v.ival);
        }

        RunTimeError("Cannot subtract these two values");
        return Value();
    }
    
    Value operator*(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival * v.ival);
        }

        if (type == isInt && v.type == isString)
        {
            if(getInteger() >= 0)
            {
                string s;
                for(int i = 0; i < ival; ++i)
                {
                    s = s + v.sval;
                }
                return s;
            }

            RunTimeError("Cannot multiply a negative integer and a string");
            return Value();
        }

        if (type == isString && v.type == isInt)
        {
            if(v.getInteger() >= 0)
            {
                string s;
                for(int i = 0; i < v.ival; ++i)
                {
                    s = s + sval;
                }
                return s;
            }

            RunTimeError("Cannot multiply a string by a negative integer");
            return Value();
        }
        
        if (type == isInt && v.type == isBool)
        {
            if (ival == -1)
            {
                bool ans = !v.bval;
                return (ans);
            }
            
            RunTimeError("Invalid logic, this is not a statement");
            return Value();
        }
        
        RunTimeError("Cannot multiply these two values");
        return Value();
    }

    Value operator/(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            if (v.ival != 0)
            {
                return Value(ival / v.ival);
            }
            
            RunTimeError("Cannot divide by 0");
        }

        RunTimeError("Cannot divide these two values");
        return Value();
    }

    Value operator<(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival < v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval < v.sval);
        }

        RunTimeError("Cannot use the less than sign for these two values");
        return Value();
    }

    Value operator<=(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival <= v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval <= v.sval);
        }

        RunTimeError("Cannot use the less than or equal to sign of these two values");
        return Value();
    }

    Value operator>(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival > v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval > v.sval);
        }

        RunTimeError("Cannot use the greater than sign for these two values");
        return Value();
    }

    Value operator>=(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival >= v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval >= v.sval);
        }

        RunTimeError("Cannot use the greater than or equal to sign for these two values");
        return Value();
    }

    Value operator==(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival == v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval == v.sval);
        }

        if (type == isBool && v.type == isBool)
        {
            return Value(bval == v.bval);
        }

        RunTimeError("Cannot compare the equality of these two values");
        return Value();
    }

    Value operator!=(const Value& v) 
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival != v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval != v.sval);
        }

        if (type == isBool && v.type == isBool)
        {
            return Value(bval != v.bval);
        }

        RunTimeError("Cannot compare the inequality of these two values");
        return Value();
    }
};

extern map<string, Value> symTblMap;

class ParseTree 
{
public:
    int         linenum;
    // Pointers of type ParseTree
    ParseTree   *left;
    ParseTree   *right;

public:
    //The constructor
    ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
        : linenum(linenum), left(l), right(r) {}

    // The destructor
    virtual ~ParseTree()
    {
        delete left;
        delete right;
    }

    int GetLinenum() const { return linenum; }

    virtual NodeType GetType() const { return ERRTYPE; } //ask any class which is a parsee tree. what its type is. nothing provided. then error
    virtual bool IAmAStringConst() const { return false; }
    virtual bool IAmAnIdent() const { return false; }
    virtual bool IAmABool() const { return false; }
    virtual string getID() const {return ""; }
    virtual bool getBool() const {return false; }
    virtual Value Eval(map<string, Value> &symTblMap) = 0;
};

class StmtList : public ParseTree { // class statement list inherits from class ParseTree. therefore a stmList object is also a Parsetree object

public:
    StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        left->Eval(symTblMap);
        if (right)
        {
            right->Eval(symTblMap);
        }

        return Value();
    }
};

// An IF statement whose Expr is not boolean typed is an error
class IfStatement : public ParseTree {
public:
    IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) 
    {
        fileLineNum = line;
    }
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        Value ans = left->Eval(symTblMap);
        if (ans.isBoolType()){
            if (ans.getBoolean() )
        {
            return right->Eval(symTblMap); 
        }
    }
        else
        {
            RunTimeError("if expression is not boolean typed");
        }

        return Value();
    }
};

class Assignment : public ParseTree
{
public:
    Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        if (left->IAmAnIdent())
        {
            // If the symbol table is not empty and I am able to put at least one symbol in the map, get the identifier
            if(not symTblMap.empty() and symTblMap.count(left->getID()))
            {
                Value rVal = right->Eval(symTblMap);
                string lVal = left->getID();
                symTblMap[lVal] = rVal;
            }
            
            else
            {
                Value rVal = right->Eval(symTblMap);
                string lVal = left->getID();
                symTblMap.insert (std::pair<string, Value>(lVal, rVal));
            }
        }
        
        else
        {
            RunTimeError("Left hand side must be an identifier");
        }
        return right->Eval(symTblMap);
    }
};

class PrintStatement : public ParseTree
{
public:
    PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        cout << left->Eval(symTblMap) << '\n';
        return Value();
    }
};

// The Type is determined by the operands so integer addition returns an int.
class PlusExpr : public ParseTree 
{
public:
    PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) + right->Eval(symTblMap);
    }
};

class MinusExpr : public ParseTree {
public:
    MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) - right->Eval(symTblMap);
    }
};

class TimesExpr : public ParseTree {
public:
    TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) * right->Eval(symTblMap);
    }
};

class DivideExpr : public ParseTree {
public:
    DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) / right->Eval(symTblMap);
    }
};

class LogicAndExpr : public ParseTree {//BOOLEAN TYPE RETURN
public:
    LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        Value lVal = left->Eval(symTblMap);
        Value rVal = right->Eval(symTblMap);
        if (lVal.isBoolType() && rVal.isBoolType())
        {
            return lVal.isTrue() && rVal.isTrue();
        }
        else
        {
            RunTimeError("first operand of || is not boolean typed");
        }
        return Value();
    }
};

class LogicOrExpr : public ParseTree {
public:
    LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        {
        Value lVal = left->Eval(symTblMap);
        Value rVal = right->Eval(symTblMap);
        if (lVal.isBoolType() || rVal.isBoolType())
        {
            return lVal.isTrue() || rVal.isTrue();
        }
            else
            {
                RunTimeError("first operand of || is not boolean typed");
            }
        return Value();
        }
    }
};

class EqExpr : public ParseTree {
public:
    EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) == right->Eval(symTblMap);
    }
};

class NEqExpr : public ParseTree {
public:
    NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) != right->Eval(symTblMap);
    }
};

class LtExpr : public ParseTree {
public:
    LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) < right->Eval(symTblMap);
    }
};

class LEqExpr : public ParseTree {
public:
    LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) <= right->Eval(symTblMap);
    }
};

class GtExpr : public ParseTree {
public:
    GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) > right->Eval(symTblMap);
    }
};

class GEqExpr : public ParseTree {
public:
    GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return left->Eval(symTblMap) >= right->Eval(symTblMap);
    }
};

class IConst : public ParseTree {
    int val;

public:
    IConst(int l, int i) : ParseTree(l), val(i) {}//nothing passed to the parsetree constructor cuz integer constant is a leaf
    IConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }

    NodeType GetType() const { return INTTYPE; }
    
    
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return Value(val);
    }
};

class BoolConst : public ParseTree {
    bool val;

public:
    BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}

    NodeType GetType() const { return BOOLTYPE; }
    
    bool IAmABool() const { return true; }
    bool getBool() const {return val; }
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return Value(val);
    }
};

class SConst : public ParseTree {
    string val;


public:
    SConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = t.GetLexeme();
    }

    NodeType GetType() const { return STRTYPE; }
    
    bool IAmAStringConst() const { return true; }
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        return Value(val);
    }
};

class Ident : public ParseTree {
    string id;

public:
    Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
    bool IAmAnIdent() const { return true; }
    string getID() const {return id; }
        
    virtual Value Eval(map<string, Value> &symTblMap)
    {
        if (not symTblMap.empty() and symTblMap.count(id))
        {
            return symTblMap[id];
        }
        else
        {
            RunTimeError("No identifier found");
        }
        return Value();
    }
};

#endif /* PARSETREE_H_ */