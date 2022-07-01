import numpy as np
from scipy.cluster.hierarchy import dendrogram, linkage
from scipy.spatial.distance import squareform
import matplotlib.pyplot as plt
import io
import math

f = io.open("C:\\MinGW\\bin\\dataset2.csv", "r", newline='\r\n')
allOfIt = f.readlines()
f.close()
arr = []

for i in range(0, len(allOfIt)):
    currentRow = []
    primoCoppia = allOfIt[i].split(",")
    for j in range(0, len(allOfIt)):
        if i == j:
            currentRow.append(0.0)
        else:
            secondoCoppia = allOfIt[j].split(",")
            currentRow.append(float(math.sqrt(pow((float(primoCoppia[1]) - float(secondoCoppia[1])), 2)
                                        + pow((float(primoCoppia[2]) - float(secondoCoppia[2])), 2)
                                        )))
    if i%1000==0 :
        print(i)
    arr.append(currentRow)

arr1 = np.asarray(arr)
dists = squareform(arr)


print("Calculating linkage matrix...")
linkage_matrix = linkage(dists, "single")
print("Done...")

print(linkage_matrix)