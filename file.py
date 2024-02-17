i = 0
while True:
    i = i + 1
    x = i
    l = 0
    m = 0

    while x > 0:
        m = m + 1

        if x % 2 != 0:
            l = l + 1
        
        x = x // 2

    if l == 7 and m == 8:
        print(i)
