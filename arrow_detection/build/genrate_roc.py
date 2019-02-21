import sklearn.metrics as mt
import csv
import matplotlib.pyplot as plt
import numpy as np

original = [];
actual = [];

with open("class_output_simple.csv", 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for d,y in spamreader:
		original.append(int(int(d)>=1))
		actual.append(int(int(y)>=1))

# print "Original : {0}".format(original);

fpr,tpr,th = mt.roc_curve(original,actual);

plt.plot(fpr,tpr);
plt.plot(np.arange(0,1,0.01),np.arange(0,1,0.01),'r.');
# plt.plot(np.arange(0,1,0.01),np.arange(1,0,-0.01),'g.');
plt.grid(b=True, which='major', color='k', linestyle='--')
plt.grid(b=True, which='minor', color='k', linestyle='--')
plt.title("Arrow detection ROC. Accuracy at equal error rate= 58.45%");
plt.xlabel("Flase Positive Rate");
plt.ylabel("True Positive Rate");
plt.show()