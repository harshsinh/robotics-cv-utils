import sys
import os
import glob
import csv

time_off = 0
frame_off= 0
csv_path = "/home/harsh/"

if(len(sys.argv) < 3):
    print("[ERROR!] give at least 3 inputs\n \
    \t frame_offset : number of frame offset, default is 0 \n \
    \t csv_file_path: full path of the csv file for height")
    sys.exit()

else:
    csv_path = sys.argv[2]
    frame_off= int(sys.argv[1])
    print("frame offset given: " + str(frame_off) + ";\n\
csv_path : " + csv_path)

    heights = []
    times   = []

    csv_file = open(csv_path, 'r')
    csvreader= csv.reader(csv_file)

    fields   = csvreader.next()

    for row in csvreader:
        times.append(float(row[1]))
        heights.append(float(row[7]))

    time_off = times[0]

def tff(f_num, offset=frame_off):
    '''Time from Frame number'''
    secs = (f_num  + offset)/ 30.
    mins = int(secs / 60.)
    secs = secs - 60 * mins
    secs = int((secs * 10) + 0.05)/10.0
    return mins, secs

def crn(mins, secs):
    '''CSV Row Number from time'''
    curr_secs = 60*mins + secs
    row_num   = int((curr_secs - time_off) * 10)
    return row_num

files = glob.glob("frame*.png")
print(len(files))

for name in files:
    print(name)
    frame_num = name[5:-4]
    m, s = tff(float(frame_num))
    h    = heights[crn(m, s)]
    dst  = str(h) + "m" + name[-4:]
    print(str(m) + " mins and " + str(s) + " seconds and height : " + str(h))

    i = 1
    while(os.path.exists(dst)):
        dst = str(h) + "m" + "_" + str(i) + name[-4:]
        i = i + 1

    os.rename(name, dst)
    print("Renaming " + name + " to " + dst)
