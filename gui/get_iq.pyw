#!/usr/bin/env python
import sys
from PySide import QtGui, QtCore
from PySide.QtCore import QProcess

class App (QtGui.QWidget):
    def __init__ (self):
        super (App, self).__init__ ()
        self.initUI ()

    def center (self):
        qr = self.frameGeometry ()
        cp = QtGui.QDesktopWidget ().availableGeometry ().center ()
        qr.moveCenter (cp)
        self.move(qr.topLeft ())

    def closeEvent (self, event):
        reply = QtGui.QMessageBox.question (self, 'Message',
            "Are you sure to quit?", QtGui.QMessageBox.Yes | 
            QtGui.QMessageBox.No, QtGui.QMessageBox.No)

        if reply == QtGui.QMessageBox.Yes:
            event.accept ()
        else:
            event.ignore ()

    def print_error (self):
        self.display_output.append (str(self.runner.readAllStandardError ()))

    def print_error_message (self,text):
        self.display_output.append (text)

    def print_output (self):
        self.display_output.append (str(self.runner.readAllStandardOutput ()))
        # print (str(self.runner.readAllStandardOutput ()))

    def start_get_iq (self):
        self.runner = QProcess (self)
        self.runner.readyReadStandardOutput.connect(self.print_output)
        self.runner.readyReadStandardError.connect(self.print_error)
        if not self.runner.start ('src/cpp2/bin/get_iq.exe'):
            self.print_error_message ('Unable to start get_iq.exe.')
        pass

    def initUI (self):
        # self.setGeometry(300, 300, 250, 150)
        self.resize (250, 150)
        self.center ()
        self.setWindowTitle ('GetIQ Frontend')
        # self.setWindowIcon(QtGui.QIcon('web.png'))

        # self.statusBar().showMessage ('Ready')

        self.display_output = QtGui.QTextEdit (self)

        self.command_path = QtGui.QLineEdit('Enter path')

        hbox_1 = QtGui.QHBoxLayout ()
        hbox_1.addWidget (self.command_path)
        # hbox_1.addWidget ()

        start_button = QtGui.QPushButton ('Start', self)
        start_button.clicked.connect (self.start_get_iq)

        quit_button = QtGui.QPushButton ('Quit', self)
        quit_button.clicked.connect (QtCore.QCoreApplication.instance().quit)

        hbox_2 = QtGui.QHBoxLayout ()
        hbox_2.addWidget (start_button)
        hbox_2.addStretch (1)
        hbox_2.addWidget (quit_button)

        vbox = QtGui.QVBoxLayout ()
        vbox.addWidget (self.display_output)
        # vbox.addStretch(1)
        vbox.addLayout (hbox_1)
        vbox.addLayout (hbox_2)
        
        self.setLayout (vbox)
        self.show ()
        
def main ():
    gui = QtGui.QApplication (sys.argv)
    app = App ()
    sys.exit (gui.exec_ ())

if __name__ == '__main__':
    main ()
