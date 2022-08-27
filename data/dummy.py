import pandas as pd
a=[[j for i in range(10000) ] for j in range(3-1)]
df=pd.DataFrame(a)
df.set_index(0,inplace=True)
df.columns=[3 for i in range(9999)]
df.to_csv("E.csv")

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