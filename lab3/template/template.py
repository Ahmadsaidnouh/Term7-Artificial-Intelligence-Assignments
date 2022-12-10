import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split

dataset = pd.read_csv('data/allData.csv')  # read the data
print(dataset.head(10))  # displays the first 10 elements in our data
print(dataset.tail(10))  # displays the last 10 elements in out data

X = dataset.iloc[:, :-1]  # X contains the features only without the class
Y = dataset.iloc[:, -1]  # Y contains the class only without the features

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, shuffle=True, random_state=0)
print(X_train)
# print(len(X_train))
# print(len(X_test))
# print(len(Y_train))
# print(len(Y_test))

# from sklearn.naive_bayes import GaussianNB
