import re

def check_func_format(input_string):
    pattern = r'^\w+\s*->>\s*\((?:"[^"]*"|[^,]+)(?:,\s*(?:"[^"]*"|[^,]+))*\)$'
    return bool(re.match(pattern, input_string))

