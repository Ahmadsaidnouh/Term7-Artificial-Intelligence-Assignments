import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import StratifiedKFold, cross_val_score
from sklearn.metrics import classification_report, confusion_matrix, precision_score, recall_score, f1_score, \
    accuracy_score

trainingSet = pd.read_csv('../../finalData/training.csv')  # read the data
testingSet = pd.read_csv('../../finalData/testing.csv')  # read the data

# training data
X_train = trainingSet.iloc[:, :-1]  # X_train contains the features only without the class
y_train = trainingSet.iloc[:, -1]  # Y_train contains the class only without the features

# testing data
X_test = testingSet.iloc[:, :-1]  # X_test contains the features only without the class
y_test = testingSet.iloc[:, -1]  # Y_test contains the class only without the features

scaler = MinMaxScaler(copy=True, feature_range=(0, 1))
scaler.fit(X_train)
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

# try width range with large step
#################################################################################################
n_scores = []
stratifiedKF = StratifiedKFold(n_splits=10)
for n in range(1, 1000, 100):
    rnForest = RandomForestClassifier(n_estimators=n)
    score = cross_val_score(rnForest, X_train, y_train, cv=stratifiedKF, scoring='accuracy')
    n_scores.append(score.mean())
    print("N = " + str(n) + " and mean score from 10 folds is " + str(score.mean()))

plt.figure(figsize=(12, 8))
plt.plot(range(1, 1000, 100), n_scores, color='red', linestyle='dashed', marker='o',
         markerfacecolor='blue', markersize=10)
plt.title('Accuracy Rate for N-Estimators Value')
plt.xlabel('N-Estimators Value')
plt.ylabel('Accuracy')
plt.show()

# try smaller range with smaller step
#################################################################################################
n_scores = []
stratifiedKF = StratifiedKFold(n_splits=10)
for n in range(1, 300, 20):
    rnForest = RandomForestClassifier(n_estimators=n)
    score = cross_val_score(rnForest, X_train, y_train, cv=stratifiedKF, scoring='accuracy')
    n_scores.append(score.mean())
    print("N = " + str(n) + " and mean score from 10 folds is " + str(score.mean()))

plt.figure(figsize=(12, 8))
plt.plot(range(1, 300, 20), n_scores, color='red', linestyle='dashed', marker='o',
         markerfacecolor='blue', markersize=10)
plt.title('Accuracy Rate for N-Estimators Value')
plt.xlabel('N-Estimators Value')
plt.ylabel('Accuracy')
plt.show()

maxAcc = max(n_scores)
max_index = n_scores.index(maxAcc)
n_est = 1 + 20 * max_index
print(n_est, n_scores[max_index])

rnForest = RandomForestClassifier(n_estimators=n_est)
rnForest.fit(X_train, y_train)

y_pred = rnForest.predict(X_test)

print(classification_report(y_test, y_pred))

cm = confusion_matrix(y_test, y_pred)

labels = ['h', 'g']
columns = [f'Predicted {label}' for label in labels]
index = [f'Actual {label}' for label in labels]
table = pd.DataFrame(cm, columns=columns, index=index)
table

acc = accuracy_score(y_test, y_pred)
prec = precision_score(y_test, y_pred)
recall = recall_score(y_test, y_pred)
specificity = cm[0, 0] / (cm[0, 0] + cm[0, 1])
f1 = f1_score(y_test, y_pred)

print('model_accuracy = ', acc)
print('model_precision = ', prec)
print('model_recall = ', recall)
print('model_specificity = ', specificity)
print('model_f1 = ', f1)
