import os
import fileinput
import sys

def process_header(filename, filenameWithPath, objectList, rootDir):
    memberList = []
    currentParent = ''
    for line in fileinput.input(filenameWithPath):
        if 'VX_RF_DATA_BEGIN' in line and '#define VX_RF_DATA_BEGIN' not in line:
            memberList = []
        elif 'VX_RF_DATA_END' in line and '#define VX_RF_DATA_END' not in line:
            currentParent = line.split('(')[1].strip('\n')
            currentParent = currentParent.strip(')')
            _pair = currentParent, memberList
            if filenameWithPath in objectList:
                objectList[filenameWithPath].append(_pair)
            else:
                objectList[filenameWithPath] = []
                objectList[filenameWithPath].append(_pair)
                
        elif 'VX_RF_DATA' in line and '#define VX_RF_DATA' not in line:
            items = line.split(',')
            parent = items[0].split('(')[1]
            membertype = items[1].strip(' ')
            memberName = items[2].strip('\n')
            memberName = memberName.strip(')')
            memberName = memberName.strip(' ')
            _tuple = parent, membertype, memberName
            memberList.append(_tuple)
    return

def process_files(directory, objectList):
    for root, dirs, files in os.walk(directory, topdown=False):
        for name in files:
            extension = name.split('.')[-1]
            if extension is 'h' or 'cpp' in extension:
                filenameWithPath = os.path.join(root, name)
                process_header(name, filenameWithPath, objectList, root)

objectList = {}
rootDir = sys.argv[1]
rootDir = rootDir.replace('\\', '/')
rootDir = rootDir.strip('\"')
process_files(rootDir, objectList)

for key, values in objectList.items():
    filename = key.partition('.')[0]
    outfile = open(filename + '_reflection.cpp', 'w')
    for it in values:
        parentName = it[0]
        globalName = 'g_rf_' + parentName.replace(':', '_')
        outfile.write('const auto ' + globalName + '\n{\n')
        outfile.write('\t::vx::detail::createReflectionData( \"' + parentName +  '\", sizeof(' + parentName + '), __alignof(' + parentName + '), ::vx::murmurhash(\"' + parentName + '\")\n')
        for member in it[1]:
            memberName = member[2]
            memberValueType = member[1]
            outfile.write('\t, ::vx::ReflectionDataMember(\"' + memberValueType + '\", \"' + memberName + '\", sizeof('+memberValueType+'), __alignof('+memberValueType+'), offsetof('+parentName+', '+memberName+'), ::vx::murmurhash(\"'+memberValueType+'\"))\n')
        outfile.write(')};\n')
        outfile.write('const vx::hash_type '+parentName+'::s_reflectionId{ '+globalName+'.hash };\n')
    outfile.close()
    
