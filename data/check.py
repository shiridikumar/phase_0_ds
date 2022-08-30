import numpy as np

def check_cross_transpose(matrix1, output2):
    if (matrix1.transpose() == output2).all():
        return True
    return False


array1 = np.loadtxt(open("H.csv"), delimiter=", ")
# array2 = np.loadtxt(open("input2.csv"), delimiter=", ")

# output1 = np.loadtxt(open("output1.csv"), delimiter=", ")
output2 = np.loadtxt(open("H1.csv"), delimiter=",")


print(check_cross_transpose(array1,output2),output2.shape,array1.shape)
for i in range(len(array1)):
    for j in range(len(array1[i])):
        if(array1[i][j]!=output2[j][i]):
            print(i,j)