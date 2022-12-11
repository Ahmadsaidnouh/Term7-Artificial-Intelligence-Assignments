import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import mean_squared_error
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report, confusion_matrix

trainingSet = pd.read_csv('../../finalData/training.csv')  # read the data
testingSet = pd.read_csv('../../finalData/testing.csv')  # read the data

# print(trainingSet.head(10))  # displays the first 5 elements in training data
# print(testingSet.head(10))  # displays the first 5 elements in testing data

X_train = trainingSet.iloc[:, :-1]  # X_train contains the features only without the class
y_train = trainingSet.iloc[:, -1]  # Y_train contains the class only without the features
X_test = testingSet.iloc[:, :-1]  # X_test contains the features only without the class
y_test = testingSet.iloc[:, -1]  # Y_test contains the class only without the features

X_train = trainingSet.iloc[:, :-1]  # X_train contains the features only without the class
y_train = trainingSet.iloc[:, -1]  # Y_train contains the class only without the features
X_test = testingSet.iloc[:, :-1]  # X_test contains the features only without the class

# print(trainingSet.head(5))  # displays the first 5 elements in training data
# print(X_train.head(5))  # displays the features of first 5 elements in training data
# print(y_train.head(5))  # displays the class of first 5 elements in training data
#
# print(testingSet.head(5))  # displays the first 5 elements in testing data
# print(X_test.head(5))  # displays the features of first 5 elements in testing data
# print(y_test.head(5))  # displays the class of first 5 elements in testing data
#
# print(len(X_train))
# print(len(X_test))
# print(len(y_train))
# print(len(y_test))

scaler = StandardScaler()
scaler.fit(X_train)
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

# print(type(X_train))
# print(type(X_test))
# print(type(y_train))
# print(type(y_test))

X_Ktrain = []
y_Ktrain = []
k = 4
step = len(X_train) / k
step = np.ceil(step)
step = int(step)
for i in range(0, len(X_train), step):
    X_Ktrain.append(X_train[i:i + step])
    y_Ktrain.append(y_train[i:i + step])

error = []
for i in range(1, 60):
    for j in range(1, len(X_Ktrain)):
        err1 = 0
        for m in range(1, len(X_Ktrain)):
            if m == j:
                continue
            KNN = KNeighborsClassifier(n_neighbors=i)
            KNN.fit(X_Ktrain[m], y_Ktrain[m])
            y_true = y_Ktrain[j]
            y_pred = KNN.predict(X_Ktrain[j])
            err = mean_squared_error(y_true, y_pred)
            err1 = err1 + err
    error.append(err1)
    print('k = ', i, 'err = ', err1)
    print("######################################\n")
print(error)

minErr = min(error)
kth = error.index(minErr) + 1
print(kth)

plt.figure(figsize=(12, 8))
plt.plot(range(1, 60), error, color='red', linestyle='dashed', marker='o',
         markerfacecolor='blue', markersize=10)
plt.title('Error Rate for K Value')
plt.xlabel('K Value')
plt.ylabel('Mean_Squared_Error')
plt.show()

KNN = KNeighborsClassifier(n_neighbors=kth)
KNN.fit(X_train, y_train)

y_pred = KNN.predict(X_test)

print(classification_report(y_test, y_pred))
print(confusion_matrix(y_test, y_pred))
