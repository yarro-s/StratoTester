import pandas as pd
import bt
import bt.algos as algos
import math
from dateutil.relativedelta import relativedelta
# import numpy as np


data_path = '../data/'

data = pd.read_csv(data_path + 'abc_prices+weights.csv', 
    index_col=0,
    parse_dates=True,
    infer_datetime_format=True)
# data = data.iloc[1:, :]

print("\n   Test data:\n", data.head(3), "\n...\n", data.tail(3))

prices = data.loc[:, ['p(A)', 'p(B)', 'p(C)']]
weights = data.loc[:, ['W(A)', 'W(B)', 'W(C)']]
npv = data.loc[:, 'V']

prices.columns = ['A', 'B', 'C']
weights.columns = prices.columns

theor_cagr = math.pow(npv[-1]/npv[0], 
    1/relativedelta(npv.index[-1], npv.index[0]).years) - 1
print("\n------\n   Theoretical CAGR: ", theor_cagr)

# Backtest
test = bt.Strategy('Custom Weight', 
    [bt.algos.RunOnce(),
     bt.algos.SelectAll(),
     bt.algos.WeighEqually(),
     bt.algos.Rebalance()])

res = bt.Backtest(test, prices).run()

print("\n   Actual CAGR: ", res) #.stats['cagr'])