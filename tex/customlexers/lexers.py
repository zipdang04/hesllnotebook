import sys
from pygments.lexer import *
from pygments.lexers.c_cpp import CppLexer
from pygments.token import *
from pygments.token import Text, Comment, Operator, Keyword, Name, String, \
    Number, Punctuation, Whitespace

class CustomLexer(CppLexer):
    name = "Hesll's lexer for C++"
    filenames = ["*.cpp"]

    MACRO_BUILTIN = set((
        "FOR", "REV", 
        "testBit", "flipBit", "cntBit", "cntBitll", 
        "CURRENT_TIMESTAMP", "randomize"
    ))
    FUNCTION_BUILTIN = set((
        "sort", "lower_bound", "upper_bound", "iota", "memset",
        "minimize", "maximize", "read",
        "emplace_back", "push_back", "push", "push_front", "pop_back", "pop", "pop_front", "empty" # 
    ))
    CLASS_BUILTIN = set((
        "ll", "ld", "PQMax", "PQMin",
        "pair", "tuple", "vector", 
        "set", "map", "unordered_set", "unordered_map",
        "stack", "queue", "deque", "priority_queue",
        "less", "greater"
    ))
    
    MACRO_CREATED = set()
    CLASS_CREATED = set()
    FUNCTION_CREATED = set()
    
    def get_tokens_unprocessed(self, text, stack=('root',)):
        isConstant = False; isType = False
        names = dict()

        all_tokens = list(CppLexer.get_tokens_unprocessed(self, text, stack))
        for i in range(len(all_tokens)):
            index, token, value = all_tokens[i]
            if token == Token.Comment.Preproc:
                words = value.split()
                if len(words) > 1:
                    name = words[1].split('(')[0]
                    self.MACRO_CREATED.add(name)
            elif token == Name.Class:
                self.CLASS_CREATED.add(value)
            elif token == Name.Function:
                self.FUNCTION_CREATED.add(value)
            elif token == Name:
                if value in self.MACRO_BUILTIN:
                    token = Name.Function.Magic
                elif value in self.FUNCTION_BUILTIN:
                    token = Name.Function
                elif value in self.CLASS_BUILTIN:
                    token = Keyword.Type
                elif value in self.MACRO_CREATED:
                    token = Name.Function.Magic
                elif value in self.CLASS_CREATED:
                    token = Name.Class
                elif value in self.FUNCTION_CREATED:
                    token = Name.Function
                all_tokens[i] = (index, token, value)
        #     print(index, token, token in self.CLASS_BUILTIN, value, file = sys.stderr)
        # print(list(self.FUNCTION_CREATED), file = sys.stderr)
        return all_tokens