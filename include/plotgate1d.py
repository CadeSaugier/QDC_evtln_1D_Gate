#Plot 1D Histogram File with Gate Lines and Save as PDF
#Program by: Cade Saugier, INPP EAL Ohio University

import sys
from matplotlib import pyplot

#Build Main
def main():
	###Open Data File and Pull
	dataFile=open('./'+sys.argv[1],'r')
	data=pull(dataFile)
	dataFile.close()
	
	###Get File Name
	k=0
	for i in range(len(sys.argv[1])):
		if sys.argv[1][i]=='_':
			k=i
	k+=1
	dataFileName=sys.argv[1][k:-3]
	
	###Get Upper and Lower Limits
	a=int(sys.argv[2])
	b=int(sys.argv[3])
	
	###Build 1D Hist. Plot
	print('\n>>> Generating Gate Plot <<<')
	pyplot.figure(1, figsize=(10,7))
	pyplot.title(dataFileName+' Gate',fontsize=24)
	pyplot.xlabel('Bin (arb)',fontsize=20)
	pyplot.ylabel('Counts',fontsize=20)
	pyplot.stairs(data[1],color='black')
	
	###Build Gate Lines
	top=max(data[1])
	pyplot.plot([a,a],[0,top],color='red',linestyle='dashed')
	pyplot.plot([b,b],[0,top],color='red',linestyle='dashed')
	
	###Save Figure
	pyplot.savefig('./gate_'+dataFileName+'.pdf',format='pdf')
	print('--->Saved Figure '+'gate_'+dataFileName+'.pdf')
	

############# FUNCTION LIST #############
def pull(data):
        #Pull Bin and Count
        read=data.readlines()
        binnum=[]
        countnum=[]
        for i in range(len(read)):
                l=0
                for k in read[i]:
                        if k!=' ':
                                l=l+1
                        else:
                                break
                l=l+1
                binnum+=[int(read[i][:l])]
                countnum+=[int(read[i][l:-1])]
        return [binnum,countnum]

#Run Main
main()
