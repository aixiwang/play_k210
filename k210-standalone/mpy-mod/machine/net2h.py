f = open('saved.tinn')
count_line = f.readline().rstrip('\n').rstrip('\r')
c1 = count_line.split(' ')
print('const int net_1[3] = {' + c1[0] + ',' + c1[1] + ',' + c1[2] + '};')



i = 0
s = ''
while True:
    s2 = f.readline().rstrip('\n').rstrip('\r')
    if len(s2) > 0:
        s += s2 + ','
        i += 1
    else:
        break
net2_h = 'const float net_2[%d] = {' % (i)
s3 = s.rstrip(',')
print net2_h + s3 + '};'    
