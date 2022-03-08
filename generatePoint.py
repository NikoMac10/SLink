# seed the pseudorandom number generator
from random import seed
from random import random

f = open("point.csv", 'w', newline='\n')

for x in range(0,100):
    f.write(str(x) + "," + str(random()*10) + "," + str(random()*10)+ "\n")
    

f.close()