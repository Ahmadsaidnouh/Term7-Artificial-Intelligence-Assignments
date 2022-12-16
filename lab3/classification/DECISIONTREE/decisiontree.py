import graphviz
import pandas as pd
from sklearn.preprocessing import MinMaxScaler
from sklearn import tree
from sklearn.metrics import classification_report, confusion_matrix, precision_score, recall_score, f1_score, \
    accuracy_score

trainingSet = pd.read_csv('../../finalData/training.csv')
testingSet = pd.read_csv('../../finalData/testing.csv')

# training data
x_train = trainingSet.iloc[:, :-1]  # features
y_train = trainingSet.iloc[:, -1]  # labels

# testing data
x_test = testingSet.iloc[:, :-1]  # features
y_test = testingSet.iloc[:, -1]  # labels

scaler = MinMaxScaler(copy=True, feature_range=(0, 1))
scaler.fit(x_train)
x_train = scaler.transform(x_train)
x_test = scaler.transform(x_test)

classifier = tree.DecisionTreeClassifier()  # default is gini which gives better results if data is binary
classifier.fit(x_train, y_train)

y_predicted = classifier.predict(x_test)

cm = confusion_matrix(y_test, y_predicted)

labels = ['h', 'g']
columns = [f'Predicted {label}' for label in labels]
index = [f'Actual {label}' for label in labels]
table = pd.DataFrame(cm, columns=columns, index=index)
table

acc = accuracy_score(y_test, y_predicted)
prec = precision_score(y_test, y_predicted)
recall = recall_score(y_test, y_predicted)
specificity = cm[0, 0] / (cm[0, 0] + cm[0, 1])
f1 = f1_score(y_test, y_predicted)

print('model_accuracy = ', acc)
print('model_precision = ', prec)
print('model_recall = ', recall)
print('model_specificity = ', specificity)
print('model_f1 = ', f1)

# import graphviz
# dot_data = tree.export_graphviz(classifier, out_file=None)
# graph = graphviz.Source(dot_data)
# graph.render("decisiontree")
