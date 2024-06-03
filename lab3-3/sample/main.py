import abc
import enum
import parser_edsl as pe
import sys
import typing
from dataclasses import dataclass
from pprint import pprint


class Type(abc.ABC):
    pass


class Kind(enum.Enum):
    Int = 'int'


@dataclass
class ElementaryType(Type):
    kind: Kind


@dataclass
class TupleType(Type):
    types: list[Type]


@dataclass
class ListType(Type):
    type_: list[Type]


class Pattern(abc.ABC):
    pass


class Op(enum.Enum):
    Cons = ':'
    Add = '+'
    Sub = '-'
    Mul = '*'
    Div = '/'


@dataclass
class PatternBinary(Pattern):
    lhs: Pattern
    op: Op
    rhs: Pattern


@dataclass
class PatternTuple(Pattern):
    patterns: list[Pattern]


class Result(abc.ABC):
    pass


@dataclass
class ResultBinary(Result):
    lhs: Result
    op: Op
    rhs: Result


@dataclass
class ResultTuple(Result):
    results: list[Result]


@dataclass
class VarExpr(Pattern, Result):
    varname: str


@dataclass
class ConstExpr(Pattern, Result):
    value: typing.Any
    type_: Kind


class EmptyList(Pattern, Result):
    pass


@dataclass
class FuncCallExpr(Result):
    funcname: str
    argument: Result


@dataclass
class Statement:
    pattern: Pattern
    result: Result


@dataclass
class FuncType:
    input_: Type
    output: Type


@dataclass
class Func:
    name: str
    type_: FuncType
    body: list[Statement]


@dataclass
class Program:
    funcs: list[Func]


EMPTY_LIST = EmptyList()

IDENT = pe.Terminal('IDENT', '[A-Za-z_][A-Za-z_0-9]*', str)
INT_CONST = pe.Terminal('INT_CONST', '[0-9]+', int)


def make_keyword(image):
    return pe.Terminal(image, image, lambda _: None, priority=10)


KW_IS, KW_END, KW_INT = map(make_keyword, ['is', 'end', 'int'])

NProgram = pe.NonTerminal('Program')
NFuncs = pe.NonTerminal('Funcs')
NFunc = pe.NonTerminal('Func')

NFuncType = pe.NonTerminal('FuncType')
NType = pe.NonTerminal('Type')
NElementaryType = pe.NonTerminal('ElementaryType')
NListType = pe.NonTerminal('ListType')
NTupleType = pe.NonTerminal('TupleType')
NTupleTypeContent = pe.NonTerminal('TupleTypeContent')
NTupleTypeItems = pe.NonTerminal('TupleTypeItems')
NTupleTypeItem = pe.NonTerminal('TupleTypeItem')

NFuncBody = pe.NonTerminal('FuncBody')
NStatements = pe.NonTerminal('Statements')
NStatement = pe.NonTerminal('Statement')

NPattern = pe.NonTerminal('Pattern')
NConsOp = pe.NonTerminal('ConsOp')
NPatternTerm = pe.NonTerminal('PatternTerm')
NConst = pe.NonTerminal('Const')
NIdent = pe.NonTerminal('Ident')

NPatternList = pe.NonTerminal('PatternList')
NPatternListContent = pe.NonTerminal('PatternListContent')
NPatternListItems = pe.NonTerminal('PatternListItems')
NPatternListItem = pe.NonTerminal('PatternListItem')

NPatternTuple = pe.NonTerminal('PatternTuple')
NPatternTupleContent = pe.NonTerminal('PatternTupleContent')
NPatternTupleItems = pe.NonTerminal('PatternTupleItems')
NPatternTupleItem = pe.NonTerminal('PatternTupleItem')

NResult = pe.NonTerminal('Result')
NResultTerm = pe.NonTerminal('ResultTerm')

NExpr = pe.NonTerminal('Expr')
NAddOp = pe.NonTerminal('AddOp')
NTerm = pe.NonTerminal('Term')
NMulOp = pe.NonTerminal('MulOp')
NFactor = pe.NonTerminal('Factor')
NAtom = pe.NonTerminal('Atom')
NFuncCall = pe.NonTerminal('FuncCall')
NFuncArg = pe.NonTerminal('FuncArg')

NResultList = pe.NonTerminal('ResultList')
NResultListContent = pe.NonTerminal('ResultListContent')
NResultListItems = pe.NonTerminal('ResultListItems')
NResultListItem = pe.NonTerminal('ResultListItem')

NResultTuple = pe.NonTerminal('ResultTuple')
NResultTupleContent = pe.NonTerminal('ResultTupleContent')
NResultTupleItems = pe.NonTerminal('ResultTupleItems')
NResultTupleItem = pe.NonTerminal('ResultTupleItem')

NProgram |= NFuncs, Program

NFuncs |= lambda: []
NFuncs |= NFuncs, NFunc, lambda xs, x: xs + [x]

NFunc |= IDENT, NFuncType, KW_IS, NFuncBody, KW_END, Func

NFuncType |= NType, '::', NType, FuncType

NType |= NElementaryType
NType |= NListType
NType |= NTupleType

NElementaryType |= KW_INT, lambda: ElementaryType(Kind.Int)

NListType |= '*', NType, ListType

NTupleType |= '(', NTupleTypeContent, ')', TupleType

NTupleTypeContent |= lambda: []
NTupleTypeContent |= NTupleTypeItems

NTupleTypeItems |= NTupleTypeItem, lambda x: [x]
NTupleTypeItems |= NTupleTypeItems, ',', NTupleTypeItem, lambda xs, x: xs + [x]

NTupleTypeItem |= NType

NFuncBody |= NStatements

NStatements |= NStatement, lambda x: [x]
NStatements |= NStatements, ';', NStatement, lambda xs, x: xs + [x]

NStatement |= NPattern, '=', NResult, Statement

NPattern |= NPatternTerm
NPattern |= NPatternTerm, NConsOp, NPattern, PatternBinary

NConsOp |= ':', lambda: Op.Cons

NPatternTerm |= NIdent
NPatternTerm |= NConst
NPatternTerm |= NPatternList,
NPatternTerm |= NPatternTuple,
NPatternTerm |= '[', NPattern, ']',

NIdent |= IDENT, VarExpr

NConst |= INT_CONST, lambda x: ConstExpr(x, Kind.Int)

NPatternList |= '{', NPatternListContent, '}'

NPatternListContent |= lambda: EMPTY_LIST
NPatternListContent |= NPatternListItems

NPatternListItems |= NPatternListItem, lambda x: PatternBinary(
    x, Op.Cons, EMPTY_LIST)
NPatternListItems |= NPatternListItem, ',', NPatternListItems, lambda x, xs: PatternBinary(
    x, Op.Cons, xs)

NPatternListItem |= NPattern

NPatternTuple |= '(', NPatternTupleContent, ')', PatternTuple

NPatternTupleContent |= lambda: []
NPatternTupleContent |= NPatternTupleItems

NPatternTupleItems |= NPatternTupleItem, lambda x: [x]
NPatternTupleItems |= NPatternTupleItems, ',', NPatternTupleItem, lambda xs, x: xs + \
    [x]

NPatternTupleItem |= NPattern

NResult |= NResultTerm
NResult |= NResultTerm, NConsOp, NResult, ResultBinary

NResultTerm |= NExpr
NResultTerm |= NResultList,
NResultTerm |= NResultTuple,

NExpr |= NTerm
NExpr |= NExpr, NAddOp, NTerm, ResultBinary

NAddOp |= '+', lambda: Op.Add
NAddOp |= '-', lambda: Op.Sub

NTerm |= NFactor
NTerm |= NTerm, NMulOp, NFactor, ResultBinary

NMulOp |= '*', lambda: Op.Mul
NMulOp |= '/', lambda: Op.Div

NFactor |= NAtom
NFactor |= '[', NExpr, ']'

NAtom |= NIdent
NAtom |= NConst
NAtom |= NFuncCall

NFuncCall |= IDENT, NFuncArg, FuncCallExpr

NFuncArg |= NAtom
NFuncArg |= NResultList
NFuncArg |= NResultTuple
NFuncArg |= '[', NResult, ']'

NResultList |= '{', NResultListContent, '}'

NResultListContent |= lambda: EMPTY_LIST
NResultListContent |= NResultListItems

NResultListItems |= NResultListItem, lambda x: ResultBinary(
    x, Op.Cons, EMPTY_LIST)
NResultListItems |= NResultListItem, ',', NResultListItems, lambda x, xs: ResultBinary(
    x, Op.Cons, xs)

NResultListItem |= NResult

NResultTuple |= '(', NResultTupleContent, ')', ResultTuple

NResultTupleContent |= lambda: []
NResultTupleContent |= NResultTupleItems

NResultTupleItems |= NResultTupleItem, lambda x: [x]
NResultTupleItems |= NResultTupleItems, ',', NResultTupleItem, lambda xs, x: xs + \
    [x]

NResultTupleItem |= NResult

if __name__ == "__main__":
    p = pe.Parser(NProgram)
    assert p.is_lalr_one()

    p.add_skipped_domain('\\s')
    p.add_skipped_domain('@[^\\n]*')

    for filename in sys.argv[1:]:
        try:
            with open(filename) as f:
                tree = p.parse(f.read())
                pprint(tree)
        except pe.Error as e:
            print(f'Ошибка {e.pos}: {e.message}')
        except Exception as e:
            print(e)
