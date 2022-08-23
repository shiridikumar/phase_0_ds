import pandas as pd
a=[[1 for i in range(10000) ] for j in range(10)]

df=pd.DataFrame(a)
df.set_index(0,inplace=True)
df.columns=[1 for i in range(9999)]
df.to_csv("dummy.csv")