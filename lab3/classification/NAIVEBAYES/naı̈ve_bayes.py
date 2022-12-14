import pandas as pd
from sklearn.preprocessing import MinMaxScaler
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import classification_report, confusion_matrix, precision_score, recall_score, f1_score, accuracy_score

trainingSet = pd.read_csv('../../finalData/training.csv')
testingSet = pd.read_csv('../../finalData/testing.csv')

# training data
x_train = trainingSet.iloc[:, :-1]  # features
y_train = trainingSet.iloc[:, -1]  # labels

# testing data
x_test = testingSet.iloc[:, :-1]  # features
y_test = testingSet.iloc[:, -1]  # labels

scaler = MinMaxScaler(copy=True, feature_range=(0,1))
scaler.fit(x_train)
x_train = scaler.transform(x_train)
x_test = scaler.transform(x_test)

classifier = GaussianNB()
classifier.fit(x_train, y_train)

y_predicted = classifier.predict(x_test)

print(classification_report(y_test, y_predicted))
print(confusion_matrix(y_test, y_predicted))