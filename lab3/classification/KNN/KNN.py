import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import mean_squared_error
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import StratifiedKFold, cross_val_score
from sklearn.metrics import classification_report, confusion_matrix, precision_score, recall_score, f1_score, \
    accuracy_score

trainingSet = pd.read_csv('../../finalData/training.csv')  # read the data
testingSet = pd.read_csv('../../finalData/testing.csv')  # read the data

# print(trainingSet.head(10))  # displays the first 5 elements in training data
# print(testingSet.head(10))  # displays the first 5 elements in testing data

# training data
X_train = trainingSet.iloc[:, :-1]  # X_train contains the features only without the class
y_train = trainingSet.iloc[:, -1]  # Y_train contains the class only without the features

# testing data
X_test = testingSet.iloc[:, :-1]  # X_test contains the features only without the class
y_test = testingSet.iloc[:, -1]  # Y_test contains the class only without the features

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

scaler = MinMaxScaler(copy=True, feature_range=(0, 1))
scaler.fit(X_train)
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

k_scores = []
stratifiedKF = StratifiedKFold(n_splits=10)
for k in range(1, 100):
    KNN = KNeighborsClassifier(n_neighbors=k)
    score = cross_val_score(KNN, X_train, y_train, cv=stratifiedKF, scoring='accuracy')
    k_scores.append(score.mean())
    print("K = " + str(k) + " and mean score from 10 folds is " + str(score.mean()))

plt.figure(figsize=(12, 8))
plt.plot(range(1, 100), k_scores, color='red', linestyle='dashed', marker='o',
         markerfacecolor='blue', markersize=10)
plt.title('Accuracy Rate for K Value')
plt.xlabel('K Value')
plt.ylabel('Accuracy')
plt.show()

maxAcc = max(k_scores)
kth = k_scores.index(maxAcc) + 1
print("Max accuracy at K = " + str(kth))

KNN = KNeighborsClassifier(n_neighbors=kth)
KNN.fit(X_train, y_train)

y_pred = KNN.predict(X_test)

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
