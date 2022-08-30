import pandas as pd
def gen_matrix(n,m):
    a=[[j*(m)+i for i in range(m) ] for j in range(n-1)]
    df=pd.DataFrame(a)
    df[0]=[0 for i in range(n-1)]
    df.set_index(0,inplace=True)
    df.columns=[n for i in range(m-1)]
    df.to_csv("I.csv")

gen_matrix(100,100)

# def solution(ans,k):
#     a=[]
#     for i in range(0,len(ans)-k+1):
#         l=ans[i:i+k]
#         l.sort()
#         if(k%2==0):
#             a.append((l[k//2]+l[(k//2)-1])/2)
#         else:
#             a.append(l[(k//2)])
#     return a




# n=[int(i) for i in input().split()]
# k=int(input())
# ans=solution(n,k)
# print(ans)