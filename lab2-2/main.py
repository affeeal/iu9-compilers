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


class PatternUnit(abc.ABC):
    pass


@dataclass
class Pattern:
    pattern_units: list[PatternUnit]


@dataclass
class PatternList(PatternUnit):
    patterns: list[Pattern]


@dataclass
class PatternTuple(PatternUnit):
    patterns: list[Pattern]


class ResultUnit(abc.ABC):
    pass


@dataclass
class Result:
    result_units: list[ResultUnit]


@dataclass
class ResultList(ResultUnit):
    results: list[Result]


@dataclass
class ResultTuple(ResultUnit):
    results: list[Result]


class Expr(ResultUnit, abc.ABC):
    pass


@dataclass
class VarExpr(Expr, PatternUnit):
    varname: str


@dataclass
class ConstExpr(Expr, PatternUnit):
    value: typing.Any
    type_: TypeEnum


@dataclass
class FuncCallExpr(Expr):
    funcname: str
    argument: Result


@dataclass
class BinaryExpr(Expr):
    lhs: Expr
    op: str
    rhs: Expr


@dataclass
class Statement:
    pattern: Pattern
    result: Result


@dataclass
class Func:
    funcname: str
    input_: Type
    output: Type
    body: list[Statement]


@dataclass
class Program:
    funcs: list[Func]


# Лексическая структура и грамматика


IDENT = pe.Terminal('IDENT', '[A-Za-z_][A-Za-z_0-9]*', str)
INT_CONST = pe.Terminal('INT_CONST', '[0-9]+', int)


def make_keyword(image):
    return pe.Terminal(image, image, lambda _: None, priority=10)


KW_IS, KW_END, KW_INT = map(make_keyword, ['is', 'end', 'int'])

NProgram, NFuncs, NFunc, NType, NElementaryType = \
    map(pe.NonTerminal, ['Program', 'Funcs', 'Func', 'Type', 'ElementaryType'])

NListType, NTupleType, NTupleTypeContent = \
    map(pe.NonTerminal, ['ListType', 'TupleType', 'TupleTypeContent'])

NTupleTypeItems, NStatements, NStatement = \
    map(pe.NonTerminal, ['TupleTypeItems', 'Statements', 'Statement'])

NPattern, NPatternUnit, NConst, NPatternList = \
    map(pe.NonTerminal, ['Pattern', 'PatternUnit', 'Const', 'PatternList'])

NPatternListContent, NPatternListItems = \
    map(pe.NonTerminal, ['PatternListContent', 'PatternListItems'])

NPatternListItem, NPatternTuple = \
    map(pe.NonTerminal, ['PatternListItem', 'PatternTuple'])

NPatternTupleContent, NPatternTupleItems = \
    map(pe.NonTerminal, ['PatternTupleContent', 'PatternTupleItems'])

NPatternTupleItem, NResult, NResultUnit = \
    map(pe.NonTerminal, ['PatternTupleItem', 'Result', 'ResultUnit'])

NExpr, NAddOp, NTerm, NMulOp, NFactor = \
    map(pe.NonTerminal, ['Expr', 'AddOp', 'Term', 'MulOp', 'Factor'])

NFuncCall, NFuncArg, NResultList = \
    map(pe.NonTerminal, ['FuncCall', 'FuncArg', 'ResultList'])

NResultListContent, NResultListItems = \
    map(pe.NonTerminal, ['ResultListContent', 'ResultListItems'])

NResultListItem, NResultTuple = \
    map(pe.NonTerminal, ['ResultListItem', 'ResultTuple'])

NResultTupleContent, NResultTupleItems = \
    map(pe.NonTerminal, ['ResultTupleContent', 'ResultTupleItems'])

NResultTupleItem = pe.NonTerminal('ResultTupleItem')

NProgram |= NFuncs, Program

NFuncs |= lambda: []
NFuncs |= NFuncs, NFunc, lambda fs, f: fs + [f]

NFunc |= IDENT, NType, '::', NType, KW_IS, NStatements, KW_END, Func

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

NStatements |= NStatement, lambda s: [s]
NStatements |= NStatements, ';', NStatement, lambda ss, s: ss + [s]

NStatement |= NPattern, '=', NResult, Statement

NPattern |= NPatternUnit, lambda pu: [pu]
NPattern |= NPatternUnit, ':', NPattern, lambda pu, p: [pu] + p

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

NResult |= NResultUnit, lambda ru: [ru]
NResult |= NResultUnit, ':', NResult, lambda ru, r: [ru] + r

NResultUnit |= NExpr
NResultUnit |= NResultList,
NResultUnit |= NResultTuple,

NExpr |= NTerm
NExpr |= NExpr, NAddOp, NTerm, BinaryExpr

NAddOp |= '+', lambda: '+'
NAddOp |= '-', lambda: '-'

NTerm |= NFactor
NTerm |= NTerm, NMulOp, NFactor, BinaryExpr

NMulOp |= '*', lambda: '*'
NMulOp |= '/', lambda: '/'

NFactor |= IDENT, VarExpr
NFactor |= NConst
NFactor |= '[', NExpr, ']'
NFactor |= NFuncCall

NFuncCall |= IDENT, NFuncArg, FuncCallExpr

NFuncArg |= '[', NResult, ']'
NFuncArg |= NResultUnit

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
    # TODO: разобраться, почему не проходит. Проблема в определении NFuncArg
    # assert p.is_lalr_one()

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
