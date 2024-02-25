import lexer

variables = []

with open('prog.psc', 'r', encoding="utf-8") as f:
    tokens = lexer.lexical_analyzer(f.read())
    classes, updated_tokens = lexer.find_classes(tokens)
    variables = lexer.find_variables(updated_tokens)
    print(lexer.translate_tokens(tokens, variables, classes))