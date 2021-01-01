import numpy as np
import sympy as sp
from sympy import *
def eigenvalue():
    x = symbols("x")
    A = np.array([[3, 4, -1], [-1, -2, 1], [3, 9, 0]])
    I = np.identity(len(A))
    H = A - I * x
    M = sp.Matrix(H)
    equation = M.det()
    final=solve(equation, x)
    return math.abs(final[0]) > 4 * math.abs(final[1])
eigenvalue()