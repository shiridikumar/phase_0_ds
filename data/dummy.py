import pandas as pd
a=[[j for i in range(10) ] for j in range(7-1)]
df=pd.DataFrame(a)
df.set_index(0,inplace=True)
df.columns=[7 for i in range(9)]
df.to_csv("E.csv")
