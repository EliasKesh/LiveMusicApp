#!/usr/bin/python
import os

indexTextStart = """<!DOCTYPE html>
<html>
<head><title>Index of {folderPath}</title></head>
<body>
    <h2>Index of {folderPath}</h2>
    <hr>
    <ul>
        <li>
            <a href='../'>../</a>
        </li>
"""
indexTextEnd = """
    </ul>
</body>
</html>
"""

def index_folder(folderPath):
    print("Indexing: " + folderPath +'/')
    #Getting the content of the folder
    files = os.listdir(folderPath)
    files.sort()
    #If Root folder, correcting folder name
    root = folderPath
    if folderPath == '.':
        root = 'Root'

 #   print("** "+root)
    indexText = indexTextStart.format(folderPath=root)
    for file in files:
        #Avoiding index.html files
        if file != 'index_dir.html':
            # if it's a directory then add the index
            if os.path.isdir(folderPath+'/'+file):
                indexText += "\t\t<li>\n\t\t\t<a href='" + file + "/index_dir.html'>" + file + "</a>\n\t\t</li>\n"
            else:
                if (file.endswith("mscz")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:orange\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("mp3")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:red\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("mid")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:orange\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("Loops")):
                    pass
                elif (file.endswith("spec.png")):
                    pass
                elif (file[0] == '.'):
                    pass
#                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:yellow\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("pdf")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:blue\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                else:
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:black\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"


        #Recursive call to continue indexing
        if os.path.isdir(folderPath+'/'+file):
            index_folder(folderPath + '/' + file)

    indexText += indexTextEnd

    #Create or override previous index.html
    index = open(folderPath+'/index_dir.html', "w")

    #Save indexed content to file
    index.write(indexText)

#Indexing root directory (Script position)
index_folder('.')

# rename 's/\d+/sprintf("%05d", $&)/e' *.mp3

# sourcedir = "/path/to/files"; extensions = (".jpg", ".jpeg")
# files = [(f, f[f.rfind("."):], f[:f.rfind(".")]) for f in os.listdir(sourcedir)if f.endswith(extensions)]
# maxlen = len(max([f[2] for f in files], key = len))

# for item in files:
#     zeros = maxlen-len(item[2])
#     shutil.move(sourcedir+"/"+item[0], sourcedir+"/"+str(zeros*"0")+item[0])
