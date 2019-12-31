with open('number.txt', 'r') as f:
    data = f.read().split('\n')[:-1]
    
    for x in range(3):
        n = 0
        for d_ind in range(len(data)):
            n += (int(data[d_ind][x])<<d_ind)
        print(hex(n))

