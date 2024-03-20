from lexer import *

print(find_variables(lexical_analyzer(open("C:/pony/prog.psc", "r").read())))