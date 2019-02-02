import os
import sys
#sys.path.append(".")
from compareOutputs import *
#os.system("tcsh")

grades = 0.0
Section = 563  ## or 563

if (os.path.isdir("./YourTestRuns") == True):
    os.system("rm -rf ./YourTestRuns")

if (os.path.isdir("./YourTestRuns") == False):
    os.mkdir("YourTestRuns")

f = open ('transcripts.txt', 'w')
f.write("Transcripts for Project 2\n\n")

try:
    os.system("make clean; make")
except TypeError:
    f.write("Unable to compile....\nGrades: 0.00\n")
    f.close()
    exit()

if not os.path.exists("./sim"):
    f.write("Unable to compile....\nGrades: 0.00\n")
    f.write("Total Grades: ")
    f.write("0.0")
    f.write("\n")
    f.close()
    f = open('UnableToCompile.txt', 'w')
    f.write("Mark....")
    f.close()
    exit()

## ValidationRun
os.system("./sim bimodal 7 0 0 ./traces/gcc_trace.txt > ./YourTestRuns/YourTestRun1.txt")
os.system("./sim bimodal 10 0 0 ./traces/gcc_trace.txt > ./YourTestRuns/YourTestRun2.txt")
os.system("./sim bimodal 5 0 0 ./traces/jpeg_trace.txt > ./YourTestRuns/YourTestRun3.txt")
os.system("./sim bimodal 6 0 0 ./traces/perl_trace.txt > ./YourTestRuns/YourTestRun4.txt")
os.system("./sim gshare 10 4 0 0 ./traces/gcc_trace.txt > ./YourTestRuns/YourTestRun5.txt")

os.system("./sim gshare 14 9 0 0 ./traces/gcc_trace.txt > ./YourTestRuns/YourTestRun6.txt")
os.system("./sim gshare 11 5 0 0 ./traces/jpeg_trace.txt > ./YourTestRuns/YourTestRun7.txt")
os.system("./sim gshare 10 6 0 0 ./traces/perl_trace.txt > ./YourTestRuns/YourTestRun8.txt")
os.system("./sim hybrid 9 14 11 6 0 0 ./traces/gcc_trace.txt > ./YourTestRuns/YourTestRun9.txt")
os.system("./sim hybrid 5 10 8 5 0 0 ./traces/jpeg_trace.txt > ./YourTestRuns/YourTestRun10.txt")

os.system("./sim bimodal 7 2048 4 ./traces/gcc_trace.txt > ./YourTestRuns/YourTestRun11.txt")
os.system("./sim gshare 10 6 2048 4 ./traces/perl_trace.txt > ./YourTestRuns/YourTestRun12.txt")

## Grading Phase
## We have mystery runs, so your actual grades might vary if it fails in Mystery Run
## Also, for some students, your cache blocks may be correct but with wrong orders, or missing dirty bits. I'm yet to update the grading rubrics for those partial grades in this script. However, partial grades will be given for minor mistakes....
#os.system("diff -iw ./YourTestRuns/YourTestRun1.txt ./Validation_Runs/ValidationRun1.txt > ./YourTestRuns/diff1.txt")
#os.system("diff -iw ./YourTestRuns/YourTestRun2.txt ./Validation_Runs/ValidationRun2.txt > ./YourTestRuns/diff2.txt")
#os.system("diff -iw ./YourTestRuns/YourTestRun3.txt ./Validation_Runs/ValidationRun3.txt > ./YourTestRuns/diff3.txt")
#os.system("diff -iw ./YourTestRuns/YourTestRun4.txt ./Validation_Runs/ValidationRun4.txt > ./YourTestRuns/diff4.txt")
#os.system("diff -iw ./YourTestRuns/YourTestRun5.txt ./Validation_Runs/ValidationRun5.txt > ./YourTestRuns/diff5.txt")
totStats = []
totWrongStats = []
totContents = []
totWrongContents = []
totTags = []
totWrongTags = []
Grades = []
ValidationFiles = ["val_bimodal_1.txt", "val_bimodal_2.txt", "val_bimodal_3.txt", "val_bimodal_4.txt", "val_gshare_1.txt", "val_gshare_2.txt", "val_gshare_3.txt", "val_gshare_4.txt", "val_hybrid_1.txt", "val_hybrid_2.txt", "val_BTB_1.txt", "val_BTB_2.txt"]
for i in range(0,12):
    successFlag = True
    try:
        fileObj1 = "YourTestRuns/YourTestRun" + str(i+1) + ".txt"
        fileObj2 = "Validation_Runs/" + ValidationFiles[i]
        if (i < 10):
            [totStats1, totWrongStats1, totContents1, totWrongContents1] = gradeOutputFiles(fileObj1, fileObj2)
        else:
            [totStats1, totWrongStats1, totContents1, totWrongContents1, totTags1, totWrongTags1] = gradeBTBOutputFiles(fileObj1, fileObj2)
    except:
        successFlag = False
        totStats.append("NA")
        totWrongStats.append("NA")
        totContents.append("NA")
        totWrongContents.append("NA")
        totTags.append("NA")
        totWrongTags.append("NA")
        Grades.append(0)
        continue
    else:
        totStats.append(totStats1)
        totWrongStats.append(totWrongStats1)
        totContents.append(totContents1)
        totWrongContents.append(totWrongContents1)
        if (i < 10):
            totTags.append("NA")
            totWrongTags.append("NA")
        else:
            totTags.append(totTags1)
            totWrongTags.append(totWrongTags1)
    try:
        if (i<10):
            grades1 = gradingPolicy(totStats1, totWrongStats1, totContents1, totWrongContents1)
        else:
            grades1 = gradingBTBPolicy(totStats1, totWrongStats1, totContents1, totWrongContents1, totTags1, totWrongTags1)
    except:
        successFlag = False
        Grades.append(0)
    else:
        Grades.append(grades1)

print "Show Grades:"
for i in range(0,12):
    printGrades(f, ValidationFiles[i], Grades[i], totContents[i], totWrongContents[i], totStats[i], totWrongStats[i], totTags[i], totWrongTags[i])
    print ValidationFiles[i], ": ", Grades[i]

totGrades = 0.0
for i in range(0, 8):
    if Section is 563: 
        totGrades = totGrades + (20.0/4) * float(Grades[i]) / 100.0;
    else:
        totGrades = totGrades + (25.0/4) * float(Grades[i]) / 100.0;
if Section is 563:
    totGrades = totGrades + 10.0 * float(Grades[8]) / 100.0 + 10.0 * float(Grades[9]) / 100.0;

bonusGrades = 0.0
for i in range(10,12):
    if Section is 563: 
        bonusGrades = bonusGrades + 3.5 * float(Grades[i]) / 100.0;
    else:
        bonusGrades = bonusGrades + 4.0 * float(Grades[i]) / 100.0;

f.write("Total Coding Part Full Credit is 50+14 for ECE463 Students, and 60+7 for ECE563 Students")
f.write("\n")
f.write("Normal Grades: ")
#f.write(str(grades))
f.write(str(totGrades))
f.write("\n")
f.write("Bonus Grades: ")
f.write(str(bonusGrades))
f.write("\n")
f.write("Total Grades: ")
f.write(str(totGrades+bonusGrades))
f.close()
print "(Total Coding Part Full Credit is 50+14 for ECE463 Students, and 60+7 for ECE563 Students)"
print totGrades+bonusGrades, " (", totGrades, " + ", bonusGrades, ")"


