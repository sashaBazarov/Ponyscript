import re
from wordlist import wordlist

def check_func_format(input_string):
    pattern = r'^\w+\s*->>\s*\((?:"[^"]*"|[^,]+)(?:,\s*(?:"[^"]*"|[^,]+))*\)$'
    pattern2 = r'^\w+\s*->>\(\)$'
    return bool(re.match(pattern, input_string)) or bool(re.match(pattern2, input_string))

def remove_text_in_quotes(s):
    return re.sub(r'\".*?\"', '', s)

def split_string(s):
    parts = re.split(r'\s(?![^()]*\))', s)
    return parts

def split_by_dot(s):
    parts = re.split(r'\.(?![^()]*\))', s, maxsplit=1)
    return parts

def check_header(string):
    pattern = r'^\".+\.psh\"$'
    if re.match(pattern, string):
        return True
    else:
        return False

def translate_token(token:str):
    if check_func_format(token):
        if token.split("->>")[0] in wordlist:
            return wordlist[token.split("->>")[0]].replace("--arg--", token.split("->>")[1][1:-1])
        else:
            return f'{token.split("->>")[0]}({token.split("->>")[1][1:-1]})'
    else:
        if token in wordlist:
            return wordlist[token]
        else:
             if check_header(token):
                 token = token.replace('"', '')
                 token = f'"{token.replace(".psh", ".h")}"'
             return token
            
def analizetext(string:str):
    dotcomma = False
    try:
        if string[-1] == ";":
            string = string[:-1]
            dotcomma = True
    except IndexError:pass

    out = ""
    #print(split_string(string))
    for token in split_string(string):

        if len(split_by_dot(token)) > 1:

            if check_header(token):
                out = out + " " + translate_token(token)

            else:
                first = True
                for tokenpart in split_by_dot(token):
                    
                    if first:
                        out = out + translate_token(tokenpart)
                        first = False
                    else:
                        out = out + "." + translate_token(tokenpart)
        else:
            out = out + " " + translate_token(token)

    if dotcomma:
        out += ";"
    
    out = out.strip()

    return out

if __name__ == "__main__":
    print(analizetext(''))
                
    #print(len(split_by_dot('neigh->>(5 + 3)')))