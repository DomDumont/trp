import sys
import os
import posixpath
import codecs
import re
import string

OSPJ = posixpath.join
OSPR = posixpath.relpath

out_html = ""
out_wordlist = set()

lang_id = ""

def AppendClass(_name):
    global lang_id
    global out_wordlist

    out_wordlist.add(_name)

    if (lang_id == "en"):    
        return '\n## Class '+ _name + '\n\nMembers:\n\n' 
    if (lang_id == "fr"):    
        return '\n## Classe '+ _name + '\n\nMembres:\n\n' 

def AppendFunc(_name):  
    global out_wordlist
    for item in re.findall('[%s]+' % string.ascii_letters, _name):
        if (len(item)> 2):
            out_wordlist.add(item)    

    return '\n**'+ _name + '**\n\n' 

def AppendRole(_name):    
    global lang_id
    if (lang_id == "en"):        
        return 'Role : '+ _name + '\n\n' 
    if (lang_id == "fr"):        
        return 'Rôle : '+ _name + '\n\n' 

def AppendArg(_name):    
    return '* '+ _name + '\n' 

def AppendSample(_name):    
    global lang_id
    if (lang_id == "en"):            
        return '\nSample : \n\n\t'+ _name.replace('\\n','\n\t') + '\n' 
    if (lang_id == "fr"):            
        return '\nExemple : \n\n\t'+ _name.replace('\\n','\n\t') + '\n' 

"""--------------------------------------------------

ApplyTemplate

--------------------------------------------------"""    

def ApplyTemplate(_path):

    global out_html

    with codecs.open(_path, 'w','utf8' ) as fOut:
        fOut.write(out_html.decode('utf-8'))

    return True
    
"""--------------------------------------------------

WriteWordList

--------------------------------------------------"""    

def WriteWordList(_path):

    global out_wordlist
    list = []

    for item in out_wordlist:
        list.append(item)

    list.sort()
    with codecs.open(_path, 'w','utf8' ) as fOut:
        for item in list:
            toto = item + '\n'
            fOut.write(toto.decode('utf-8'))

    return True

"""--------------------------------------------------

ScanFile

--------------------------------------------------"""    

def ScanFile(_path):
    global out_html
    with open(_path, 'r') as inF:
        for line in inF:
            if '///class:' in line:
                out_html += AppendClass(line[line.index('///class:')+9:-1])
            if '///func:' in line:
                out_html += AppendFunc(line[line.index('///func:')+8:-1])                
            if '///role:' in line:
                out_html += AppendRole(line[line.index('///role:')+8:-1])                
            if '///arg:' in line:
                out_html += AppendArg(line[line.index('///arg:')+7:-1])                
            if '///sample:' in line:
                out_html += AppendSample(line[line.index('///sample:')+10:-1])                

"""--------------------------------------------------

SortedListDir

--------------------------------------------------"""    

def SortedListDir(_path):
    """
    Returns the content of a directory ordering the names alphabetically
    """        
    return sorted([f for f in os.listdir(_path)])

"""--------------------------------------------------

ConvertDirectory

--------------------------------------------------"""    

def ConvertDirectory (_in_dir,_out_dir,_cur_dir) :
    for filename in SortedListDir(_cur_dir):
        #print 'DEBUG '+ filename
        filePath = OSPJ(_cur_dir,filename)
        rel = OSPR(filePath,_in_dir)
        filePathWithoutExt, fileExtension = os.path.splitext(rel)
        if os.path.isdir(OSPJ(_cur_dir,filename)):          
            #print 'DEBUG this is a directory'
            ConvertDirectory(_in_dir,_out_dir,OSPJ(_cur_dir,filename))
        else:
            #print 'DEBUG this is a file'
            if ((fileExtension == '.h') or (fileExtension == '.cpp')):
                print filename + ' => ' + fileExtension
                ScanFile(filePath)

"""--------------------------------------------------

BuildSite

--------------------------------------------------"""    
def BuildSite (_source_dir,_out_dir, _word_dir) :
    global out_html
    out_html = "# Script Reference \n\n"
    ConvertDirectory(_source_dir,_out_dir,_source_dir)
    if (ApplyTemplate(OSPJ(_out_dir,'generated-ref.md')) == False):
        return False
    if (WriteWordList(OSPJ(_word_dir,'wordlist.txt')) == False):
        return False
    return True

"""--------------------------------------------------

Main

--------------------------------------------------"""

def main(argv):
    global lang_id
    #print 'GCD = '+ GCD    
    if len(argv) != 4:
        print ('usage : python extract_api lang_id source_dir dest_dir word_dir')
        return

    lang_id = argv[0]

    if (BuildSite(argv[1],argv[2],argv[3])== True) :
        print('Done with grace and subtility')
    else:
        print('Failed with panache')


if __name__ == "__main__":    
    main(sys.argv[1:])