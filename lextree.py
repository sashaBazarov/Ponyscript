class Node:
    def __init__(self, data):
        self.data = data
        self.children = []

    def add_child(self, child):
        self.children.append(child)

def build_tree(tokens):
    root = Node("")
    current_node = root
    stack = [root]

    for token in tokens:
        if token == '{':
            new_node = Node(token)
            current_node.add_child(new_node)
            stack.append(new_node)
            current_node = stack[-1]
        elif token == '}':
            new_node = Node(token)
            current_node.add_child(new_node)
            stack.append(new_node)
            stack.pop()
            current_node = stack[-1]
        else:
            new_node = Node(token)
            current_node.add_child(new_node)
            stack.append(new_node)

    return root


def nested_lists(input_list):
    nested = []
    stack = []
    
    for token in input_list:
        if token == '{':
            if stack:
                stack[-1].append([])
                stack.append(stack[-1][-1])
            else:
                nested.append([])
                stack.append(nested[-1])
        elif token == '}':
            stack.pop()
        else:
            if stack:
                stack[-1].append(token)
            else:
                nested.append(token)
    
    return nested


def tree_to_list(node):
    result = [node.data]
    for child in node.children:

        result.extend(tree_to_list(child))
    return result


def assemble_tokens(treelist, indent=0):
    out = ''
    for token in treelist:
        if isinstance(token, list):
            if len(token) > 1:
                out += '{\n' + assemble_tokens(token, indent + 4) + '\n' + ' ' * indent + '}\n'
            else:
                out += '{\n' + assemble_tokens(token, indent) + '}\n'
        else:
            out += token + " "
    if indent == 0:
        return out.strip()
    else:
        return '\n' + ' ' * indent + out.strip()

    


from lexer import *
tokens = (lexical_analyzer(open("C:\pony\prog.psc", "r").read()))
variables = find_variables(tokens)
classes, tokens = find_classes(tokens)

translated = translate_tokens(tokens=tokens, variables=variables, classes=classes)
tree = build_tree(translated)
tree_list = tree_to_list(tree)

print(assemble_tokens(nested_lists(tree_list)))
# assemble_tokens(tree_list)

# print(nested_lists(translated))


