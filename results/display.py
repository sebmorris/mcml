import matplotlib.pyplot as plt
import numpy as np

import sys

print(sys.path[0] + '/test.csv')

myFile = np.genfromtxt(sys.path[0] + '/test.csv', delimiter=',')

print(myFile)

plt.imshow(np.log(myFile.T + 1e-9))

plt.show()