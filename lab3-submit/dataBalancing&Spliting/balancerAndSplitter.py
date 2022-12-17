import random
from random import sample

sourceFile = open('data/magic04.data', 'r')

data = sourceFile.readlines()
sourceFile.close()
data1 = data[0:12332].copy()  # contains 12332 gamma elements
data2 = data[12332:].copy()  # contains 6688 hadron elements

# shuffle the 12332 gammas so that we can select 6688 samples randomly
random.shuffle(data1)
random.shuffle(data1)

# since data1 is shuffled twice, by taking the first 6688 samples,we can say we have randomly select 6688 elements from
# the data1 list to balance the data.
gammasTemp = data1[0:6688].copy()
hadronsTemp = data2.copy()

# represent the gammas (g) with (1)
# represent the hadrons (h) with (0)
gammas = [sub.replace('g', '1') for sub in gammasTemp]
hadrons = [sub.replace('h', '0') for sub in hadronsTemp]

# since gammas is shuffled triple, by taking the first 4682 samples,we can say we have randomly select 6688 elements
# from the data1 list to balance the training data.
random.shuffle(gammas)
random.shuffle(gammas)
random.shuffle(gammas)
gammas70 = gammas[0:4682]  # 6688*0.7 = 4682
gammas30 = gammas[4682:]

# since hadrons is shuffled triple, by taking the first 4682 samples,we can say we have randomly select 6688 elements
# from the data1 list to balance the training data.
random.shuffle(hadrons)
random.shuffle(hadrons)
random.shuffle(hadrons)
hadrons70 = hadrons[0:4682]  # 6688*0.7 = 4682
hadrons30 = hadrons[4682:]

trainList = gammas70 + hadrons70  # 4682(g) + 4682(h)
testList = gammas30 + hadrons30  # 2006(g) + 2006(h)
random.shuffle(trainList)  # training data must be shuffled
random.shuffle(testList)  # testing data needs to be shuffled

#############################################################################################
##################### Start storing data in files ###########################################
#############################################################################################

with open('data/allData.csv', 'w') as allDataFile:
    allDataFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in gammas + hadrons:
        allDataFile.write(line)

with open('data/gamma.csv', 'w') as gammaFile:
    gammaFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in gammas:
        gammaFile.write(line)

with open('data/hadron.csv', 'w') as hadronFile:
    hadronFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in hadrons:
        hadronFile.write(line)

with open('data/training.csv', 'w') as trainingFile:
    trainingFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in trainList:
        trainingFile.write(line)

with open('data/testing.csv', 'w') as testingFile:
    testingFile.write('fLength,fWidth,fSize,fConc,fConc1,fAsym,fM3Long,fM3Trans,fAlpha,fDist,class\n')
    for line in testList:
        testingFile.write(line)
