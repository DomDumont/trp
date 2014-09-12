import sys
import shutil
import os
import webbrowser
import posixpath

OSPJ = posixpath.join

lang_id = ""

"""--------------------------------------------------

BuildDoc

--------------------------------------------------"""    
def BuildDoc () :

    savedPath = os.getcwd()
    os.chdir('../../doc/'+lang_id)

    SRC_LIST = [
    'Home.md',
    'Revisions.md',
    'Installation.md',
    'tutorial.md',
    'tuto1.md',
    'tuto2.md',
    'tuto3-animation.md',
    'tuto4-sound.md',
    'tuto5-music.md',
    'tuto6-button.md',
    'tuto7-listbox.md',
    'tuto8-io.md',
    'android.md',
    'ios.md',
    'generated-ref.md'
    ]

    OUTPUT = 'trp-manual'

    destination = open('temp.md','wt')
    for item in SRC_LIST:
        try:
            shutil.copyfileobj(open(item,'rt'), destination)
        except Exception, e:
            destination.write('* error '+ item + ' file missing \n')
        
    destination.close()


    os.system('pandoc -s -S --variable=geometry:a4paper --toc temp.md -o ./out/{0}.pdf'.format(OUTPUT))
    print "PDF conversion done"
    os.system('pandoc temp.md -o ./out/{0}.docx'.format(OUTPUT))
    print "Docx conversion done"
    os.system('pandoc -s -S --toc --toc-depth=2 --self-contained -N -c pandoc.css -A footer.html temp.md -o ./out/{0}.html'.format(OUTPUT))
    print "HTML conversion done"

    os.remove('temp.md')

    os.chdir(savedPath)

    return True

#webbrowser.open('file://'+OSPJ(os.getcwd(),'out')+'/trp-manual.html')

#os.system("open "+OSPJ(os.getcwd(),'out')+'/trp-manual.pdf')





#'./blog.md','rt'), destination)
#'./tasks.md','rt'), destination)

"""--------------------------------------------------

Main

--------------------------------------------------"""

def main(argv):
    global lang_id
    #print 'GCD = '+ GCD    
    if len(argv) != 1:
        print ('usage : python build_doc.py lang_id')
        return

    lang_id = argv[0]

    if (BuildDoc()== True) :
        print('Done with grace and subtility')
    else:
        print('Failed with panache')

if __name__ == "__main__":    
    main(sys.argv[1:])
