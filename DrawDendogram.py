import numpy as np
from scipy.cluster.hierarchy import dendrogram, linkage
from scipy.spatial.distance import squareform
import matplotlib.pyplot as plt
import io
import math

exec(open("C:\\MinGW\\bin\\linkageCPP.py").read())

dendrogram(mat)

plt.title("Single Link")
plt.show()