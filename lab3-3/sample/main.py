import abc
import enum
import typing
import parser_edsl as pe
import sys
from dataclasses import dataclass


class SemanticError(pe.Error):
    pass


class FunctionRedefinition(SemanticError):
    def __init__(self, pos, funcname):
        self.pos = pos
        self.funcname = funcname

    @property
    def message(self):
        return f'Переопределение функции {self.funcname}'


class RepeatedPatternVariable(SemanticError):
    def __init__(self, pos, varname):
        self.pos = pos
        self.varname = varname

    @property
    def message(self):
        return f'Повторная переменная в образце: {self.varname}'


class TypeMismatch(SemanticError):
    def __init__(self, pos: pe.Position, expected: str, actual: str):
        self.pos = pos
        self.expected = expected
        self.actual = actual

    @property
    def message(self):
        return f'Ожидался {self.expected}, но указан {self.actual}'


class Op(enum.Enum):
    Cons = ':'
    Add = '+'
    Sub = '-'
    Mul = '*'
    Div = '/'


class Type(abc.ABC):
    @staticmethod
    @abc.abstractmethod
    def name() -> str:
        pass


class IntType(Type):
    @staticmethod
    def name():
        return 'целочисленный тип'


@dataclass
class TupleType(Type):
    types: list[Type]

    @staticmethod
    def name():
        return 'кортеж'


@dataclass
class ListType(Type):
    type_: Type

    @staticmethod
    def name():
        return 'список'


class Pattern(abc.ABC):
    @abc.abstractmethod
    def check(self, formal_type: Type, var_types: dict[str, Type]):
        pass


@dataclass
class PatternEmptyList(Pattern):
    coord: pe.Position

    @pe.ExAction
    def create(attrs, coords, res_coord):
        lcb_coord, rcb_coord = coords
        return PatternEmptyList(lcb_coord)

    def check(self, formal_type, var_types):
        actual_type = ListType
        if actual_type != type(formal_type):
            raise TypeMismatch(
                self.coord, formal_type.name(), actual_type.name())


@dataclass
class PatternConst(Pattern):
    value: typing.Any
    value_coord: pe.Position
    type_: Type

    @staticmethod
    def create(type_):
        @pe.ExAction
        def action(attrs, coords, res_coord):
            value, = attrs
            value_coord, = coords
            return PatternConst(value, value_coord, type_)

        return action

    def check(self, formal_type, var_types):
        if type(self.type_) != type(formal_type):
            raise TypeMismatch(
                self.value_coord, formal_type.name(), self.type_.name())


@dataclass
class PatternVar(Pattern):
    name: str
    name_coord: pe.Position

    @pe.ExAction
    def create(attrs, coords, res_coord):
        name, = attrs
        name_coord, = coords
        return PatternVar(name, name_coord)

    def check(self, formal_type, var_types):
        if self.name in var_types:
            raise RepeatedPatternVariable(self.name_coord, self.name)
        var_types[self.name] = formal_type


@dataclass
class PatternBinary(Pattern):
    lhs: Pattern
    op: Op
    op_coord: pe.Position
    rhs: Pattern

    @pe.ExAction
    def create_empty_list_cons(attrs, coords, res_coord):
        lhs, = attrs
        # TODO: handle coords
        return PatternBinary(lhs, Op.Cons, None, PatternEmptyList(None))

    @pe.ExAction
    def create_cons(attrs, coords, res_coord):
        lhs, rhs = attrs
        # TODO: handle coords
        return PatternBinary(lhs, Op.Cons, None, rhs)

    @pe.ExAction
    def create(attrs, coords, res_coord):
        lhs, op, rhs = attrs
        lhs_coord, op_coord, rhs_coord = coords
        return PatternBinary(lhs, op, op_coord, rhs)

    def check(self, formal_type, var_types):
        assert self.op is Op.Cons

        actual_type = ListType
        if actual_type != type(formal_type):
            raise TypeMismatch(
                self.op_coord, formal_type.name(), actual_type.name())

        self.lhs.check(formal_type.type_, var_types)
        self.rhs.check(formal_type, var_types)


@dataclass
class PatternTuple(Pattern):
    patterns: list[Pattern]
    patterns_coord: pe.Position

    @pe.ExAction
    def create(attrs, coords, res_coord):
        patterns, = attrs
        lbr_coord, patterns_coord, rbr_coord = coords
        return PatternTuple(patterns, patterns_coord)

    def check(self, formal_type, var_types):
        actual_type = TupleType
        if actual_type != type(formal_type):
            raise TypeMismatch(self.patterns_coord,
                               formal_type.name(), actual_type.name())

        for pattern, formal_type in zip(self.patterns, formal_type.types):
            pattern.check(formal_type, var_types)


class Result(abc.ABC):
    pass


@dataclass
class ResultEmtpyList(Result):
    coord: pe.Position

    @pe.ExAction
    def create(attrs, coords, res_coord):
        coord, = coords
        return ResultEmtpyList(None)


@dataclass
class ResultConst(Result):
    value: typing.Any
    value_coord: pe.Position
    type_: Type

    @staticmethod
    def create(type_):
        @pe.ExAction
        def action(attrs, coords, res_coord):
            value, = attrs
            value_coord, = coords
            return ResultConst(value, value_coord, type_)

        return action


@dataclass
class ResultVar(Result):
    name: str
    name_coord: pe.Position

    @pe.ExAction
    def create(attrs, coords, res_coord):
        name, = attrs
        name_coord, = coords
        return ResultVar(name, name_coord)


@dataclass
class FuncCallExpr(Result):
    funcname: str
    argument: Result
    argument_coord: pe.Position

    @pe.ExAction
    def create(attrs, coords, res_coord):
        funcname, argument = attrs
        funcname_coord, argument_coord = coords
        return FuncCallExpr(funcname, argument, argument_coord)


@dataclass
class ResultBinary(Result):
    lhs: Result
    op: Op
    op_coord: pe.Position
    rhs: Result

    @pe.ExAction
    def create_empty_list_cons(attrs, coords, res_coord):
        lhs, = attrs
        # TODO: handle coord
        return ResultBinary(lhs, Op.Cons, None, ResultEmtpyList(None))

    @pe.ExAction
    def create_cons(attrs, coords, res_coord):
        lhs, rhs = attrs
        # TODO: handle coord
        return ResultBinary(lhs, Op.Cons, pe.Position(), rhs)

    @pe.ExAction
    def create(attrs, coords, res_coord):
        lhs, op, rhs = attrs
        lhs_coord, op_coord, rhs_coord = coords
        return ResultBinary(lhs, op, op_coord, rhs)


@dataclass
class ResultTuple(Result):
    results: list[Result]  # TODO: handle coord?


@dataclass
class FuncType:
    input_: Type
    output: Type


@dataclass
class Sentence:
    pattern: Pattern  # TODO: handle coord?
    result: Result  # TODO: handle coord?

    def check(self, func_types: dict[str, FuncType], funcname: str):
        functype = func_types[funcname]

        var_types = {}
        self.pattern.check(functype.input_, var_types)
        # TODO


@dataclass
class Func:
    name: str
    name_coord: pe.Position
    type_: FuncType
    body: list[Sentence]

    @pe.ExAction
    def create(attrs, coords, res_coord):
        name, func_type, func_body = attrs
        name_coord, type_coord, is_coord, body_coord, end_coord = coords
        return Func(name, name_coord, func_type, func_body)

    def check(self, func_types):
        for sentence in self.body:
            sentence.check(func_types, self.name)


@dataclass
class Program:
    funcs: list[Func]

    def check(self):
        funcs = {}
        for func in self.funcs:
            if func.name in funcs:
                raise FunctionRedefinition(func.name_coord, func.name)
            funcs[func.name] = func.type_

        for func in self.funcs:
            func.check(funcs)


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
NSentences = pe.NonTerminal('Sentences')
NSentence = pe.NonTerminal('Sentence')

NPattern = pe.NonTerminal('Pattern')
NConsOp = pe.NonTerminal('ConsOp')
NPatternTerm = pe.NonTerminal('PatternTerm')
NConst = pe.NonTerminal('Const')

NPatternList = pe.NonTerminal('PatternList')
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

NFunc |= IDENT, NFuncType, KW_IS, NFuncBody, KW_END, Func.create

NFuncType |= NType, '::', NType, FuncType

NType |= NElementaryType
NType |= NListType
NType |= NTupleType

NElementaryType |= KW_INT, lambda: IntType()

NListType |= '*', NType, ListType

NTupleType |= '(', NTupleTypeContent, ')', TupleType

NTupleTypeContent |= lambda: []
NTupleTypeContent |= NTupleTypeItems

NTupleTypeItems |= NTupleTypeItem, lambda x: [x]
NTupleTypeItems |= NTupleTypeItems, ',', NTupleTypeItem, lambda xs, x: xs + [x]

NTupleTypeItem |= NType

NFuncBody |= NSentences

NSentences |= NSentence, lambda x: [x]
NSentences |= NSentences, ';', NSentence, lambda xs, x: xs + [x]

NSentence |= NPattern, '=', NResult, Sentence

NPattern |= NPatternTerm
NPattern |= NPatternTerm, NConsOp, NPattern, PatternBinary.create

NConsOp |= ':', lambda: Op.Cons

NPatternTerm |= IDENT, PatternVar.create
NPatternTerm |= NConst
NPatternTerm |= NPatternList,
NPatternTerm |= NPatternTuple,
NPatternTerm |= '[', NPattern, ']',

NConst |= INT_CONST, PatternConst.create(IntType())

NPatternList |= '{', '}', PatternEmptyList.create
NPatternList |= '{', NPatternListItems, '}'

NPatternListItems |= NPatternListItem, PatternBinary.create_empty_list_cons
NPatternListItems |= NPatternListItem, ',', NPatternListItems, PatternBinary.create_cons

NPatternListItem |= NPattern

NPatternTuple |= '(', NPatternTupleContent, ')', PatternTuple.create

NPatternTupleContent |= lambda: []
NPatternTupleContent |= NPatternTupleItems

NPatternTupleItems |= NPatternTupleItem, lambda x: [x]
NPatternTupleItems |= NPatternTupleItems, ',', NPatternTupleItem, lambda xs, x: xs + \
    [x]

NPatternTupleItem |= NPattern

NResult |= NResultTerm
NResult |= NResultTerm, NConsOp, NResult, ResultBinary.create

NResultTerm |= NExpr
NResultTerm |= NResultList,
NResultTerm |= NResultTuple,

NExpr |= NTerm
NExpr |= NExpr, NAddOp, NTerm, ResultBinary.create

NAddOp |= '+', lambda: Op.Add
NAddOp |= '-', lambda: Op.Sub

NTerm |= NFactor
NTerm |= NTerm, NMulOp, NFactor, ResultBinary.create

NMulOp |= '*', lambda: Op.Mul
NMulOp |= '/', lambda: Op.Div

NFactor |= NAtom
NFactor |= '[', NExpr, ']'

NAtom |= IDENT, ResultVar.create
NAtom |= NConst
NAtom |= NFuncCall

NFuncCall |= IDENT, NFuncArg, FuncCallExpr.create

NFuncArg |= NAtom
NFuncArg |= NResultList
NFuncArg |= NResultTuple
NFuncArg |= '[', NResult, ']'

NResultList |= '{', NResultListContent, '}'

NResultListContent |= lambda: ResultEmtpyList.create
NResultListContent |= NResultListItems

NResultListItems |= NResultListItem, ResultBinary.create_empty_list_cons
NResultListItems |= NResultListItem, ',', NResultListItems, ResultBinary.create_cons

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
                tree.check()
                print('Семантических ошибок не найдено')
        except pe.Error as e:
            print(f'Ошибка {e.pos}: {e.message}')
