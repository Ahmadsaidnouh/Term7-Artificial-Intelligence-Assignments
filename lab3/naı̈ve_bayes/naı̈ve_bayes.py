from sklearn.metrics import confusion_matrix
from sklearn.metrics import classification_report
from sklearn.naive_bayes import GaussianNB
import pandas as pd

trainingSet = pd.read_csv('../finalData/training.csv')
testingSet = pd.read_csv('../finalData/testing.csv')

# training data
x_train = trainingSet.iloc[:, :-1]  # features
y_train = trainingSet.iloc[:, -1]  # labels

# testing data
x_test = testingSet.iloc[:, :-1]  # features
y_test = testingSet.iloc[:, -1]  # labels

classifier = GaussianNB()
classifier.fit(x_train, y_train)

y_predicted = classifier.predict(x_test)

print(classification_report(y_test, y_predicted))
print(confusion_matrix(y_test, y_predicted))
