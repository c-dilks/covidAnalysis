import csv
import sys
import datetime

filename=sys.argv[1]
countryFilter=sys.argv[2]
if(len(sys.argv)>3): debug=1
else: debug=0

date = filename.replace(".csv","").replace("data/","")
daynum = datetime.datetime.strptime(date,'%m-%d-%Y').timetuple().tm_yday

with open(filename) as csvFile:
    csvReader = csv.reader(csvFile, delimiter=',')
    cnt = 0
    nC=0
    nD=0
    nR=0
    for row in csvReader:
        if(daynum<83):
            region,country,update,nCstr,nDstr,nRstr = row[0],row[1],row[2],row[3],row[4],row[5]
        else:
            fips,admin2,region,country,update,lat,lon,nCstr,nDstr,nRstr,key =\
            row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7],row[8],row[9],row[10]
        if(cnt>0):
            if(nCstr==""): nCstr="0"
            if(nDstr==""): nDstr="0"
            if(nRstr==""): nRstr="0"
            if(debug==1): print(country,nCstr,nDstr,nRstr)
            if(countryFilter in country):
                nC+=int(nCstr,10)
                nD+=int(nDstr,10)
                nR+=int(nRstr,10)
                if(debug==1): print("---add")
        cnt+=1
    if(debug==1): print("TOTAL COUNTS:")
    print date,daynum,countryFilter,nC,nD,nR

