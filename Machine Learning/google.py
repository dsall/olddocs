import pandas as pd
import quandl

df = quandl.get("SSE/GGQ1")

df = df[['Adj. Open','Adj. High','Adj. Close','Adj. Volume']]

df['HL_PCT'] = (df['Adj. High'] - df['Adj. Close'])/df['Adj. Close']*100.00
df['PCT_Change'] = (df['Adj. Close'] - df['Adj. Open'])/df['Adj.Open']*100.00

df = df[['Adj. Close', 'HL_PCT', 'PCT_Change', 'Adj. Volume']]

print(df.head())
