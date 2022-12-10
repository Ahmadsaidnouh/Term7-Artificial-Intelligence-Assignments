import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split

trainingSet = pd.read_csv('data/training.csv')  # read the data
testingSet = pd.read_csv('data/testing.csv')  # read the data

print(trainingSet.tail(5))  # displays the last 5 elements in training data
print(testingSet.tail(5))  # displays the last 5 elements in testing data

X_train = trainingSet.iloc[:, :-1]  # X_train contains the features only without the class
y_train = trainingSet.iloc[:, -1]  # Y_train contains the class only without the features
X_test = testingSet.iloc[:, :-1]  # X_test contains the features only without the class
y_test = testingSet.iloc[:, -1]  # Y_test contains the class only without the features

print(trainingSet.head(5))  # displays the first 5 elements in training data
print(X_train.head(5))  # displays the features of first 5 elements in training data
print(y_train.head(5))  # displays the class of first 5 elements in training data

print(testingSet.head(5))  # displays the first 5 elements in testing data
print(X_test.head(5))  # displays the features of first 5 elements in testing data
print(y_test.head(5))  # displays the class of first 5 elements in testing data

print(len(X_train))
print(len(X_test))
print(len(y_train))
print(len(y_test))

# start importing your model from sklearn library
# then, train your model using X_train and Y_train
# then, test your model using X_test and Y_test
# then, find all needed calculations like confusion matrix, precision ... (easily done with a module in sklearn)
