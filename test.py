from lexer import *
classes, tokens = find_classes(lexical_analyzer(open("D:\\Users\\ccart\\source\\repos\\Ponyscript\\lib\\random\\random.h", "r").read()))
print(classes)