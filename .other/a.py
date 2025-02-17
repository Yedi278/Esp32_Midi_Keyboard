import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os

os.chdir(os.path.dirname(__file__))

data_path = 'data.txt'

with open(data_path, 'r') as f:
    data = f.readlines()

data = [float(x.strip()) for x in data]


plt.plot(data)
plt.show()
