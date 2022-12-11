import random
from random import sample

sourceFile = open('data/magic04.data', 'r')

data = sourceFile.readlines()
data1 = data[0:12332]  # contains 12332 gamma elements
data2 = data[12332:]  # contains 6688 hadron elements

# sample() will randomly select 6688 elements from the data1 list to balance the data.
gammas = sample(data1, 6688)
hadrons = data2

# sample() will randomly select 4682 elements from the gammas list to represent the training data.
gammas70 = sample(gammas, 4682)  # 6688*0.7 = 4682
# gammas30 will hold all remaining that are in gammas list but not in gammas70 list to represent the testing data.
gammas30 = gammas.copy()
for ele in gammas70:
    gammas30.remove(ele)

# sample() will randomly select 4682 elements from the hadrons list to represent the training data.
hadrons70 = sample(hadrons, 4682)  # 6688*0.7 = 4682
# hadrons30 will hold all remaining that are in hadrons list but not in hadrons70 list to represent the testing data.
hadrons30 = hadrons.copy()
for ele in hadrons70:
    hadrons30.remove(ele)

#############################################################################################
##################### Start storing data in files ###########################################
#############################################################################################
trainList = gammas70 + hadrons70
testList = gammas30 + hadrons30
random.shuffle(trainList)  # training data must be shuffled
random.shuffle(testList)  # testing data needs to be shuffled

with open('data/allData.data', 'w') as allDataFile:
    allDataFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in gammas + hadrons:
        allDataFile.write(line)

with open('data/gamma.data', 'w') as gammaFile:
    gammaFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in gammas:
        gammaFile.write(line)

with open('data/hadron.data', 'w') as hadronFile:
    hadronFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in hadrons:
        hadronFile.write(line)

with open('data/training.data', 'w') as trainingFile:
    trainingFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in trainList:
        trainingFile.write(line)

with open('data/testing.data', 'w') as testingFile:
    testingFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in testList:
        testingFile.write(line)
