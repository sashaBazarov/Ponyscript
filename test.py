import llvmlite.ir as llvm
import re

class LLVMIRGenerator:
    def __init__(self):
        self.builder = None
        self.module = llvm.Module()
        self.variables = {}

    def parse(self, code):
        tokens = re.findall(r'\b(?:void|int|float|char|if|else|while|for|return|[a-zA-Z_][a-zA-Z0-9_]*)\b|[-+]?[0-9]*\.?[0-9]+|!=|==|<=|>=|&&|\|\||[{}();]|[^\s]', code)
        main_index = -1
        for i, token in enumerate(tokens):
            if token == 'main':
                main_index = i
                break

        if main_index == -1:
            raise ValueError("No main function found")

        main_tokens = tokens[main_index + 1:]
        self.generate_ir(main_tokens)

    def parse_expression(self, tokens):
        if not tokens:
            raise ValueError("Empty expression")

        if len(tokens) == 1:
            if tokens[0].isdigit():
                return llvm.Constant(llvm.IntType(32), int(tokens[0]))
            else:
                return self.variables.get(tokens[0], llvm.Constant(llvm.IntType(32), 0))

        while len(tokens) > 1 and tokens[0] == '(' and tokens[-1] == ')':
            paren_count = 1
            for i in range(1, len(tokens) - 1):
                if tokens[i] == '(':
                    paren_count += 1
                elif tokens[i] == ')':
                    paren_count -= 1
                if paren_count == 0:
                    break
            else:
                raise ValueError("Unmatched parenthesis")

            if i == len(tokens) - 2:
                tokens = tokens[1:-1]
            else:
                break

        if tokens[0] in self.variables:
            return self.variables[tokens[0]]

        op = tokens.pop(1)  # Binary operator
        if tokens:
            left = self.parse_expression(tokens[:1])
            right = self.parse_expression(tokens[1:])
        
        if op == '+':
            return self.builder.add(left, right)
        elif op == '-':
            return self.builder.sub(left, right)
        elif op == '*':
            return self.builder.mul(left, right)
        elif op == '/':
            return self.builder.sdiv(left, right)
        else:
            raise ValueError("Unknown operator:", op)

    def generate_ir(self, tokens):
        function_type = llvm.FunctionType(llvm.IntType(32), [], False)
        function = llvm.Function(self.module, function_type, 'main')
        block = function.append_basic_block('entry')
        self.builder = llvm.IRBuilder(block)
        
        result = self.parse_expression(tokens)
        self.builder.ret(result)

        return str(self.module)

# Example usage
generator = LLVMIRGenerator()
code = """
int main() {
    int a = 5;
    int b = 10;
    int c = a + b;
    return c;
}
"""
generator.parse(code)
print(generator.module)
