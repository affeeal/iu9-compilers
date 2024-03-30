import abc
import enum
import parser_edsl as pe
import sys
import typing
from dataclasses import dataclass
from pprint import pprint


class Type(abc.ABC):
    pass


class TypeEnum(enum.Enum):
    Int = 'int'


@dataclass
class ElementaryType(Type):
    type_: TypeEnum


@dataclass
class TupleType(Type):
    types: list[Type]


@dataclass
class ListType(Type):
    types: list[Type]


class Pattern(abc.ABC):
    pass


@dataclass
class PatternBinary(Pattern):
    lhs: Pattern
    op: str
    rhs: Pattern


@dataclass
class PatternList(Pattern):
    patterns: list[Pattern]


@dataclass
class PatternTuple(Pattern):
    patterns: list[Pattern]


class Result(abc.ABC):
    pass


@dataclass
class ResultBinary(Result):
    lhs: Result
    op: str
    rhs: Result


@dataclass
class ResultList(Result):
    results: list[Result]


@dataclass
class ResultTuple(Result):
    results: list[Result]


@dataclass
class VarExpr(Pattern, Result):
    varname: str


@dataclass
class ConstExpr(Pattern, Result):
    value: typing.Any
    type_: TypeEnum


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
NTupleTypeItems = pe.NonTerminal('TupleTypeContent')

NFuncBody = pe.NonTerminal('FuncBody')
NStatements = pe.NonTerminal('Statements')
NStatement = pe.NonTerminal('Statement')

NPattern = pe.NonTerminal('Pattern')
NConsOp = pe.NonTerminal('ConsOp')
NPatternUnit = pe.NonTerminal('PatternUnit')
NConst = pe.NonTerminal('Const')

NPatternList = pe.NonTerminal('PatternList')
NPatternListContent = pe.NonTerminal('PatternListContent')
NPatternListItems = pe.NonTerminal('PatternListItems')
NPatternListItem = pe.NonTerminal('PatternListItem')

NPatternTuple = pe.NonTerminal('PatternTuple')
NPatternTupleContent = pe.NonTerminal('PatternTupleContent')
NPatternTupleItems = pe.NonTerminal('PatternTupleItems')
NPatternTupleItem = pe.NonTerminal('PatternTupleItem')

NResult = pe.NonTerminal('Result')
NResultUnit = pe.NonTerminal('ResultUnit')

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
NFuncs |= NFuncs, NFunc, lambda fs, f: fs + [f]

NFunc |= IDENT, NFuncType, KW_IS, NFuncBody, KW_END, Func

NFuncType |= NType, '::', NType, FuncType

NType |= NElementaryType
NType |= NListType
NType |= NTupleType

NElementaryType |= KW_INT, lambda: ElementaryType(TypeEnum.Int)

NListType |= '*', NType, ListType

NTupleType |= '(', NTupleTypeContent, ')', TupleType

NTupleTypeContent |= lambda: []
NTupleTypeContent |= NTupleTypeItems

NTupleTypeItems |= NType, lambda t: [t]
NTupleTypeItems |= NTupleTypeItems, ',', NType, lambda ts, t: ts + [t]

NFuncBody |= NStatements

NStatements |= NStatement, lambda s: [s]
NStatements |= NStatements, ';', NStatement, lambda ss, s: ss + [s]

NStatement |= NPattern, '=', NResult, Statement

NPattern |= NPatternUnit
NPattern |= NPatternUnit, NConsOp, NPattern, PatternBinary

NConsOp |= ':', lambda: ':'

NPatternUnit |= IDENT, VarExpr
NPatternUnit |= NConst
NPatternUnit |= NPatternList,
NPatternUnit |= NPatternTuple,
NPatternUnit |= '[', NPattern, ']',

NConst |= INT_CONST, lambda value: ConstExpr(value, TypeEnum.Int)

NPatternList |= '{', NPatternListContent, '}', PatternList

NPatternListContent |= lambda: []
NPatternListContent |= NPatternListItems

NPatternListItems |= NPatternListItem, lambda pli: [pli]
NPatternListItems |= NPatternListItems, ',', NPatternListItem, \
    lambda plis, pli: plis + [pli]

NPatternListItem |= NPattern

NPatternTuple |= '(', NPatternTupleContent, ')', PatternTuple

NPatternTupleContent |= lambda: []
NPatternTupleContent |= NPatternTupleItems

NPatternTupleItems |= NPatternTupleItem, lambda pti: [pti]
NPatternTupleItems |= NPatternTupleItems, ',', NPatternTupleItem, \
    lambda ptis, pti: ptis + [pti]

NPatternTupleItem |= NPattern

NResult |= NResultUnit
NResult |= NResultUnit, NConsOp, NResult, ResultBinary

NResultUnit |= NExpr
NResultUnit |= NResultList,
NResultUnit |= NResultTuple,

NExpr |= NTerm
NExpr |= NExpr, NAddOp, NTerm, ResultBinary

NAddOp |= '+', lambda: '+'
NAddOp |= '-', lambda: '-'

NTerm |= NFactor
NTerm |= NTerm, NMulOp, NFactor, ResultBinary

NMulOp |= '*', lambda: '*'
NMulOp |= '/', lambda: '/'

NFactor |= NAtom
NFactor |= '[', NExpr, ']'

NAtom |= IDENT, VarExpr
NAtom |= NConst
NAtom |= NFuncCall

NFuncCall |= IDENT, NFuncArg, FuncCallExpr

NFuncArg |= NAtom
NFuncArg |= NResultList
NFuncArg |= NResultTuple
NFuncArg |= '[', NResult, ']'

NResultList |= '{', NResultListContent, '}', ResultList

NResultListContent |= lambda: []
NResultListContent |= NResultListItems

NResultListItems |= NResultListItem, lambda rli: [rli]
NResultListItems |= NResultListItems, ',', NResultListItem, \
    lambda rlis, rli: rlis + [rli]

NResultListItem |= NResult

NResultTuple |= '(', NResultTupleContent, ')', ResultTuple

NResultTupleContent |= lambda: []
NResultTupleContent |= NResultTupleItems

NResultTupleItems |= NResultTupleItem, lambda rti: [rti]
NResultTupleItems |= NResultTupleItems, ',', NResultTupleItem, \
    lambda rtis, rti: rtis + [rti]

NResultTupleItem |= NResult

if __name__ == "__main__":
    p = pe.Parser(NProgram)
    assert p.is_lalr_one()

    p.add_skipped_domain('\\s')
    p.add_skipped_domain('@[^\\n]*')

    for filename in sys.argv[1:]:
        try:
            with open(filename) as f:
                pass
                tree = p.parse(f.read())
                pprint(tree)
        except pe.Error as e:
            print(f'Ошибка {e.pos}: {e.message}')
        except Exception as e:
            print(e)
