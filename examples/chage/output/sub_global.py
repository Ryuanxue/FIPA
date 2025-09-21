#!/usr/bin/python

import sys

SymTbl = dict()

GlobRanges = list()

GlobObjs = dict()

def process_symtbl(ObjFile):
    with open(ObjFile+".symtbl", 'r') as f:
        for line in f:
            if 'OBJECT' in line:
                ObjName = line.strip().split()[7]
                ObjLopc = int(line.strip().split()[1], 16)
                ObjSize = 0
                if "0x" in line.strip().split()[2]:
                    ObjSize = int(line.strip().split()[2], 16)
                else:
                    ObjSize = int(line.strip().split()[2], 10)
                GlobObjs[ObjLopc] = (ObjName, ObjLopc+ObjSize)
                GlobRanges.append((ObjLopc,ObjSize))

def sub_addr_with_name(ObjFile):
    with open(ObjFile+".pinout0", 'r') as fin:
        with open(ObjFile+".pinout_new", 'w') as fout:
            for line in fin:
                From = line.strip().split()[0]
                To = line.strip().split()[1]
                if "0x" in From:
                    FromValue = int(From,16)
                    if FromValue in GlobObjs:
                        From = GlobObjs[FromValue][0]
                    else:
                        for (start, size) in GlobRanges:
                            if FromValue > start and FromValue < start+size:
                                From = GlobObjs[start][0]
                                break
                    if not "0x" in From:
                        fout.write('$'+From+' '+To+'\n')
                elif "0x" in To:
                    ToValue = int(To,16)
                    if ToValue in GlobObjs:
                        To = GlobObjs[ToValue][0]
                    else:
                        for (start, size) in GlobRanges:
                            if ToValue > start and ToValue < start+size:
                                To = GlobObjs[start][0]
                                break
                    if not "0x" in To:
                        fout.write(From+' $'+To+'\n')
                else:
                    fout.write(line)
                    
if __name__=="__main__":
    process_symtbl(sys.argv[1])
    sub_addr_with_name(sys.argv[1])
    
