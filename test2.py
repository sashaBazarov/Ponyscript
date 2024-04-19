from lexer import *

tokens = lexical_analyzer(open('C:/pony/prog.psc', 'r').read())

variables = find_variables(tokens)
classes, tokens = find_classes(tokens)

print(assemble_code(translate_tokens(tokens, variables, classes)))

