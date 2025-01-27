import math
#from ROOT import TLorentzVector
import ROOT

#############################################################################
## test

def printme( str ):
    "This prints a passed string into this function"
    print str
    return
#############################################################################


def getsumpt( collection1, collection2=None, collection3=None ):
    "scalar sum of pt of up to 3 arbitrary collections of objects"
    thesum=0.0
   
    for theobj in collection1:
        thesum += theobj.obj.Pt()
   
    if collection2 is not None:
        for theobj in collection2:
            thesum += theobj.obj.Pt()
   
    if collection3 is not None:
        for theobj in collection3:
            thesum += theobj.obj.Pt()

    return thesum

#############################################################################

def getsumEnergy( collection1, collection2=None, collection3=None ):
    "scalar sum of energy of up to 3 arbitrary collections of objects"
    thesum=0.0
   
    for theobj in collection1:
        thesum += theobj.obj.E()
   
    if collection2 is not None:
        for theobj in collection2:
            thesum += theobj.obj.E()
   
    if collection3 is not None:
        for theobj in collection3:
            thesum += theobj.obj.E()
  
    return thesum
#############################################################################

def getsumTLV( collection1, collection2=None, collection3=None ):
    "vector sum of TLVs in up to 3 arbitrary collections of objects"
    
    thesum = ROOT.ttH.Lepton() #<- doesn't matter that it's a lepton, we just want the obj
    
    thesum.obj.SetPxPyPzE(0.,0.,0.,0.)

    for theobj in collection1:
        thesum.obj += theobj.obj
   
    if collection2 is not None:
        for theobj in collection2:
            thesum.obj += theobj.obj
   
    if collection3 is not None:
        for theobj in collection3:
            thesum.obj += theobj.obj
  
    return thesum.obj

#############################################################################
def getdR( object1, object2 ):
    "get dR between two arbitrary objects"
    dR = (object1.obj.Eta() - object2.obj.Eta())*(object1.obj.Eta() - object2.obj.Eta())
   #dR += (object1.obj.Phi() - object2.obj.Phi())*(object1.obj.Phi() - object2.obj.Phi())     ## which one is correct?
    dR += (getdPhi(object1,object2))*(getdPhi(object1,object2))                    ## which one is correct? this one.
    dR = math.sqrt(dR)
   #print object1.obj
    return dR

#############################################################################

def getdPhi( object1, object2 ):
    "get dPhi between two arbitrary objects (since its -pi to pi)"
    dPhi = object2.obj.Phi() - object1.obj.Phi()
    pi = math.pi
    if (dPhi>=pi):
       #dPhi = -(2*math.pi - abs(object2.obj.Phi()) - abs(object1.obj.Phi()))
        dPhi = dPhi - 2*pi                            # from TVector2
    if (dPhi<(-pi)):
       #dPhi = 2*math.pi - abs(object2.obj.Phi()) - abs(object1.obj.Phi())
        dPhi = dPhi + 2*pi                            # from TVector2
    return dPhi
#############################################################################

def convertWPtoCSVvalue( tagtype=None):
    "twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating"
    tagcut=0.0
    if tagtype is None:
        return tagcut
    if tagtype is 'L':
        tagcut = 0.423 # 10.1716% DUSG mistag efficiency
    if tagtype is 'M':
        tagcut = 0.814 # 1.0623% DUSG mistag efficiency
    if tagtype is 'T':
        tagcut = 0.941 # 0.1144% DUSG mistag efficiency
    return tagcut

#############################################################################

def getAvgCSV( collection1, tagtype=None, isTag=True ):
    "return avg. csv of collection: (the collection, CSV working point ('L','M','T', or None), whether you are looking for tags (True) or non-tags (False)). In the case of leptons, the avg. csv of matched jets is returned."
    tagcut = convertWPtoCSVvalue(tagtype)
    tagsum = 0.0
    count = 0
    for theobj in collection1:
        if tagtype is not None:
            if isTag is True and (tagcut<theobj.csv):
                tagsum += theobj.csv
                count += 1
            if isTag is False and (tagcut>=theobj.csv):
                tagsum += theobj.csv
                count += 1
        else:
            tagsum += theobj.csv
            count += 1
    if (count>0):
        tagsum = tagsum / count

    return tagsum

#############################################################################

def keepTagged( collection1, tagtype=None):
    if tagtype is None:
        return collection1
    tagcut = convertWPtoCSVvalue(tagtype)
    keptobjs = []
    for theobj in collection1:
        if (tagcut<theobj.csv):
            keptobjs.append(theobj)
    return keptobjs
    
    
#############################################################################

def keepUnTagged( collection1, tagtype=None):
    if tagtype is None:
        return collection1
    tagcut = convertWPtoCSVvalue(tagtype)
    keptobjs = []
    for theobj in collection1:
        if (tagcut>=theobj.csv):
            keptobjs.append(theobj)
    return keptobjs
                

#############################################################################
## this may be redundant now for some purposes (see pickFromSortedTwoObjKine)
def getTwoObjKineExtreme( collection1, collection2, extremetype='min', quantity='dR'): ##, tagtype=None, isTag=True ):
    "call keepTagged or keepUntagged beforehand to look at tags/non-tags. (collection 1, collection 2 (can be same), 'min' or 'max', 'dR' or 'dEta' or 'dPhi') "
    minvalue = 999999.
    maxvalue = 0.
    #testvalue = -99999.
    for obj1 in collection1:
        for obj2 in collection2:
            if not(obj1==obj2):
                if extremetype is 'min':
                    if quantity is 'dR':
                        testvalue = getdR(obj1,obj2)
                        if (testvalue<minvalue):
                            minvalue = testvalue
                    if quantity is 'dEta':
                        testvalue = abs(obj1.obj.Eta()-obj2.obj.Eta())
                        if (testvalue<minvalue):
                            minvalue = testvalue
                    if quantity is 'dPhi':
                        testvalue = abs(getdPhi(obj1,obj2)) ## <- really finds the closest one in phi
                        if (testvalue<minvalue):
                            minvalue = testvalue
                    if quantity is 'mass':
                        obj12 = obj1.obj + obj2.obj
                        testvalue = obj12.M()
                        if (testvalue<minvalue):
                            minvalue = testvalue
                if extremetype is 'max':
                    if quantity is 'dR':
                        testvalue = getdR(obj1,obj2)
                        if (testvalue>maxvalue):
                            maxvalue = testvalue
                    if quantity is 'dEta':
                        testvalue = abs(obj1.obj.Eta()-obj2.obj.Eta())
                        if (testvalue>maxvalue):
                            maxvalue = testvalue
                    if quantity is 'dPhi':
                        testvalue = abs(getdPhi(obj1,obj2)) ## <- really finds the furthest one in phi
                        if (testvalue>maxvalue):
                            maxvalue = testvalue
                    if quantity is 'mass':
                        obj12 = obj1.obj + obj2.obj
                        testvalue = obj12.M()
                        if (testvalue>maxvalue):
                            maxvalue = testvalue
            #else:
                #print '  '
                #print obj1.obj.Pt()
                #print obj2.obj.Pt()
                        
    if extremetype is 'min':
        return minvalue
    if extremetype is 'max':
        return maxvalue
    

#############################################################################

def getTwoObjKineRawCollection( collection1, collection2, quantity='dR'):
    size1 = len(collection1)
    size2 = len(collection2)
    kineRawCollecion = []
    
    if collection1 is not collection2:
        
        for obj1 in collection1:
            for obj2 in collection2:
                if not(obj1==obj2):
                    
                    if quantity is 'dR':
                        value = getdR(obj1,obj2)
                        kineRawCollecion.append(value)
                    elif quantity is 'dEta':
                        value = obj2.obj.Eta()-obj1.obj.Eta()
                        kineRawCollecion.append(value)
                    elif quantity is 'dPhi':
                        value = getdPhi(obj1,obj2)
                        kineRawCollecion.append(value)
                    elif quantity is 'mass':
                        obj12 = obj1.obj + obj2.obj
                        kineRawCollecion.append(obj12.M())
                    elif quantity is 'massSFOS' and obj1.pdgID/obj2.pdgID == -1:
                        obj12 = obj1.obj + obj2.obj
                        kineRawCollecion.append(obj12.M())

    else:
        if (size1>=2):
            for idx1,obj1 in enumerate(collection1):
                for idx2,obj2 in enumerate(collection2):
                    if idx2>idx1:
                        if quantity is 'dR':
                            value = getdR(obj1,obj2)
                            kineRawCollecion.append(value)
                        elif quantity is 'dEta':
                            value = obj2.obj.Eta()-obj1.obj.Eta()
                            kineRawCollecion.append(value)
                        elif quantity is 'dPhi':
                            value = getdPhi(obj1,obj2)
                            kineRawCollecion.append(value)
                        elif quantity is 'mass':
                            obj12 = obj1.obj + obj2.obj
                            kineRawCollecion.append(obj12.M())
                        elif quantity is 'massSFOS' and obj1.pdgID/obj2.pdgID == -1:
                            obj12 = obj1.obj + obj2.obj
                            kineRawCollecion.append(obj12.M())

    return kineRawCollecion
#############################################################################

def pickFromSortedTwoObjKine( collection1, collection2, quantity='mass', whichInOrder=1, comparisonValue=None):
    "if no comparison value, whichInOrder picks the ith quanity from the list (in decending order). If comparison value provided, whichInOrder picks the ith quanity closest to the value."
    thelist = getTwoObjKineRawCollection( collection1, collection2, quantity)
    #print thelist
    size = len(thelist)

    if size is 0: return -99999.
    if size<whichInOrder: whichInOrder = size

    if comparisonValue is None:
        thelist.sort(reverse=True)
        return thelist[whichInOrder-1]

    thelist = sorted(thelist, key=lambda i: abs(i-comparisonValue))
    #print thelist
    #print " "
    return thelist[whichInOrder-1]
    
#############################################################################

def getNumTwoObjKineInRange( collection1, collection2, quantity='mass', comparisonValue=125., withinrange=15.):
    thelist = getTwoObjKineRawCollection( collection1, collection2, quantity)
    count = 0
    for item in thelist:
        if (abs(item - comparisonValue)<withinrange):
            count += 1
    return count

#############################################################################

def getFinalBDTOSTwoLep():
    "still need to do this..."
    return 0.

#############################################################################
