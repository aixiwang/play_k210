f = open('semeion.data')
i = 0

while True:
    s2 = f.readline().rstrip('\n').rstrip('\r')
    #print(s2)
    
    #s = raw_input('any key...')
    s = ''
    if len(s2) > 0:
        s3 = s2.split(' ')
        k = 0
        #print '----------------------------'
        for d in s3:
            if len(d) > 0:
                #print k,d
                k += 1
                
                s += d + ','

        data_h = 'const float data_%d[%d] = {' % (i,k)
        s3 = s.rstrip(',')
        print data_h + s3 + '};'    
        i += 1        
    else:
        break
        
        
data_h = 'const float ** data_list[%d] = {' %(i)
s = ''
for m in range(0,i):
    s += 'data_' + str(m) + ',' + '\n'
s = s.rstrip(',\n') + '\n'
print data_h + s + '};'