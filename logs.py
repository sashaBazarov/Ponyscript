from datetime import datetime
import colorama

log_path:str = 'lastest.log'

def print_log(log):
    
    if log != '':

        log = str(datetime.now().strftime('[%H:%M:%S]')) + ' ' + log

        print(log)
        with open(log_path, 'a') as f:
            f.write(log + '\n')
    
    elif log == '\n':
        with open(log_path, 'a') as f:
            f.write(log)

def print_error(log):
    if log != '':
        print_log(colorama.Fore.RED + log + colorama.Fore.RESET)
        with open(log_path, 'a') as f:
            f.write(log + '\n')

def clear_log():
    with open(log_path, 'w') as f:
        f.write('')
    return True