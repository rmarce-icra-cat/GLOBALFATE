from PyQt4.QtGui import QAction, QIcon, QMessageBox, QPushButton, QLineEdit, QFileDialog

def check_filename(text_file, name, filename, p=0):
    if filename=='':
        msgBox = QMessageBox()
        msgBox.setText(name + " is missing.\nTry again")
        msgBox.exec_()
        return -1
    else:
        if p==1:
            text_file.write(name + '\t')
        text_file.write(str(filename) + '\n')
    return 0

 
    
    