import numpy as np

def check_cross_transpose(matrix1, matrix2, output1, output2):
    if (matrix1.transpose() == output2).all() and (matrix2.transpose() == output1).all():
        return True
    return False


array1 = np.loadtxt(open("H1.csv"), delimiter=",")
array2 = np.loadtxt(open("I1.csv"), delimiter=",")

output1 = np.loadtxt(open("H.csv"), delimiter=",")
output2 = np.loadtxt(open("I.csv"), delimiter=",")

print(check_cross_transpose(array1, array2, output1, output2))
for i in range(len(array1)):
    for j in range(len(array1[i])):
        if(array1[i][j]!=output2[j][i]):
            print(i,j)

print()
print()

for i in range(len(array2)):
    for j in range(len(array2[i])):
        if(array2[i][j]!=output1[j][i]):
            print(i,j)

