import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import AdaBoostClassifier
from sklearn.metrics import mean_squared_error
from sklearn.metrics import classification_report, confusion_matrix


trainingSet = pd.read_csv('../../finalData/training.csv')  # read the data
testingSet = pd.read_csv('../../finalData/testing.csv')  # read the data

X_train = trainingSet.iloc[:, :-1]  # X_train contains the features only without the class
y_train = trainingSet.iloc[:, -1]  # Y_train contains the class only without the features
X_test = testingSet.iloc[:, :-1]  # X_test contains the features only without the class
y_test = testingSet.iloc[:, -1]  # Y_test contains the class only without the features

scaler = StandardScaler()
scaler.fit(X_train)
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

X_Ktrain = []
y_Ktrain = []
k = 4
step = len(X_train) / k
step = np.ceil(step)
step = int(step)
for i in range(0, len(X_train), step):
    X_Ktrain.append(X_train[i:i + step])
    y_Ktrain.append(y_train[i:i + step])

# try width range with large step
#################################################################################################
# error = []
# for i in range(1, 5000, 1000):
#     for j in range(1, len(X_Ktrain)):
#         err1 = 0
#         for m in range(1, len(X_Ktrain)):
#             if m == j:
#                 continue
#             ada = AdaBoostClassifier(n_estimators=i)
#             ada.fit(X_Ktrain[m], y_Ktrain[m])
#             y_true = y_Ktrain[j]
#             y_pred = ada.predict(X_Ktrain[j])
#             err = mean_squared_error(y_true, y_pred)
#             err1 = err1 + err
#     error.append(err1)
#     print('n_est = ', i, 'err = ', err1)
#     print("######################################\n")
# print(error)
#
# plt.figure(figsize=(12,8))
# plt.plot(range(1,5000,1000), error, color='red', linestyle='dashed', marker='o',
#         markerfacecolor='blue', markersize=10)
# plt.title('Error Rate for N-Estimators Value')
# plt.xlabel('N-Estimators Value')
# plt.ylabel('Mean_Squared_Error')
#################################################################################################

# try smaller range with smaller step
#################################################################################################
# error = []
# for i in range(1, 1000, 30):
#     for j in range(1, len(X_Ktrain)):
#         err1 = 0
#         for m in range(1, len(X_Ktrain)):
#             if m == j:
#                 continue
#             ada = AdaBoostClassifier(n_estimators=i)
#             ada.fit(X_Ktrain[m], y_Ktrain[m])
#             y_true = y_Ktrain[j]
#             y_pred = ada.predict(X_Ktrain[j])
#             err = mean_squared_error(y_true, y_pred)
#             err1 = err1 + err
#     error.append(err1)
#     print('n_est = ', i, 'err = ', err1)
#     print("######################################\n")
# print(error)
#
# plt.figure(figsize=(12,8))
# plt.plot(range(1,1000,30), error, color='red', linestyle='dashed', marker='o',
#         markerfacecolor='blue', markersize=10)
# plt.title('Error Rate for N-Estimators Value')
# plt.xlabel('N-Estimators Value')
# plt.ylabel('Mean_Squared_Error')
#################################################################################################

# try smaller, more precise range with smaller, more precise step
error = []
for i in range(100, 400, 20):
    for j in range(1, len(X_Ktrain)):
        err1 = 0
        for m in range(1, len(X_Ktrain)):
            if m == j:
                continue
            ada = AdaBoostClassifier(n_estimators=i)
            ada.fit(X_Ktrain[m], y_Ktrain[m])
            y_true = y_Ktrain[j]
            y_pred = ada.predict(X_Ktrain[j])
            err = mean_squared_error(y_true, y_pred)
            err1 = err1 + err
    error.append(err1)
#     print('n_est = ', i, 'err = ', err1)
#     print("######################################\n")
# print(error)

plt.figure(figsize=(12, 8))
plt.plot(range(100, 400, 20), error, color='red', linestyle='dashed', marker='o',
         markerfacecolor='blue', markersize=10)
plt.title('Error Rate for N-Estimators Value')
plt.xlabel('N-Estimators Value')
plt.ylabel('Mean_Squared_Error')
plt.show()

minErr = min(error)
min_index = error.index(minErr)
n_est = 100 + 20 * min_index
print(n_est, error[min_index])

ada = AdaBoostClassifier(n_estimators=n_est)
ada.fit(X_train, y_train)

y_pred = ada.predict(X_test)

print(classification_report(y_test, y_pred))
print(confusion_matrix(y_test, y_pred))
