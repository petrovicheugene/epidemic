# README # 

### How to create subdir project ### 
1. Create a project from the Qt subdir template 
2. Define project default path in Build&Run: 

./ Util.asciify("build/Name}-FileSystemName}-Name}")} 
 
and Projects Directory : Current directory  
 
3. Run subDirInit.bat script. It creates all extra subfolders according to the subdir project schema  
 
. 
├── bin 
│   ├── debug 
│   └── release 
├── build 
│   ├── debug 
│   └── release 
├── import 
├── include 
├── lib.linux 
├── lib.win32 
├── src 
│   ├── include 
│   ├── MyApp 
│   └── MyLib 
└── tests 
    └── MyLibTest 
 
4. Add a subproject in the src directory 
5. Modify default .pro file for application by inserting the following after  
"greaterThan(QT_MAJOR_VERSION, 4): QT += widgets" 
 
 
#PRO VARS 
TARGET = <SubAppName> 
#Application version 
#RC_ICONS = "ZImages/SRVLab-8.ico" 
 
VER_MAJ=0 
VER_MIN=0 
VER_PAT=0 
VER_BUILD=b 
 
QMAKE_TARGET_PRODUCT="Sub App 1" 
QMAKE_TARGET_DESCRIPTION="Sub App 1 app" 
QMAKE_TARGET_COMPANY="TechnoAnalyt" 
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2017, 2018.  All rights reserved." 
COMPANY_URL=tehnoanalit.com 
 
#------------------------------------------------- 
# in common.pri will be defined VERSION, TARGET, DEBUG SETTINGS 
#  global APP DEFINES 
#------------------------------------------------- 
include( ../../common.pri ) 
include( ../../app.pri ) 
 
#END 
 
 
 
 
6. Modify default .pro file by inserting the following after  
TARGET = <SubLibName> 
 
#PRO VARS 
#Application version 
#RC_ICONS = "ZImages/SRVLab-8.ico" 
 
VER_MAJ=1 
VER_MIN=2 
VER_PAT=3 
VER_BUILD=b 
 
 
QMAKE_TARGET_PRODUCT="SRV Lab" 
QMAKE_TARGET_DESCRIPTION="Chemical analysis of X-ray spectra" 
QMAKE_TARGET_COMPANY="TechnoAnalyt" 
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2017, 2018.  All rights reserved." 
COMPANY_URL=tehnoanalit.com 
LANGUAGES += en \ 
    ru \ 
    kk 
#------------------------------------------------- 
# in common.pri will be defined VERSION, TARGET, DEBUG SETTINGS 
#  global APP DEFINES 
#------------------------------------------------- 
include( ../../common.pri ) 
include( ../../lib.pri ) 
 
#END 
 
7. Modify main.cpp : 
 
//=============================================== 
#include "MainWindow.h" 
#include "ZTranslatorManager.h" 
 
#include <iostream> 
#include <windows.h> 
 
#include <QApplication> 
#include <QDir> 
#include <QOperatingSystemVersion> 
#include <QPixmap> 
#include <QSet> 
#include <QSplashScreen> 
#include <QTextCodec> 
#include <QTranslator> 
#include <iostream> 
//====================================================== 
/*! 
\brief  This module contains a function "messageHandler" for receiving standard 
 messages qDebug, qInfo, qWarning, qCritical and qFatal. 
 
 Further these messages are redirected to MainWindow that should have a receive function 
                         const QMessageLogContext &context, 
 At the same time the messages are outputing in standard output stream 
 std and are displayed in Qt Creator. 
 
 In class MainWindow the messages may be handled as log messages. 
 
 In main function qApp object gets new properties that hold the defines 
 created in .pro file. These defines becomes accessable from every point 
 of the code. 
 
*/ 
//====================================================== 
namespace 
{ 
// main window pointer 
static MainWindow* pMainWindow = nullptr; 
static QSet<QtMsgType> msgTypesToHandleInMainWindowSet; 
 
// settings of format of debug output mssages to stderr 
enum DebugOutputOption { 
    DO_MsgOnly = 0x0, 
    DO_ShowFile = 0x1, 
    DO_ShowFunction = 0x2, 
    DO_ShowLine = 0x4, 
    DO_ShowVersion = 0x8}; 
 
Q_DECLARE_FLAGS(DebugOutputOptions, DebugOutputOption) 
Q_DECLARE_OPERATORS_FOR_FLAGS(DebugOutputOptions) 
 
static DebugOutputOptions debugOutputOption = 
       DO_MsgOnly 
        | DO_ShowFile 
        | DO_ShowFunction 
        | DO_ShowLine 
        // | DO_ShowVersion 
        ; 
 
} 
//====================================================== 
void initMsgTypesToHandleInMainWindow() 
{ 
#ifndef QT_NO_DEBUG_OUTPUT 
    msgTypesToHandleInMainWindowSet.insert(QtDebugMsg); 
#endif 
    msgTypesToHandleInMainWindowSet.insert(QtInfoMsg); 
    msgTypesToHandleInMainWindowSet.insert(QtWarningMsg); 
    msgTypesToHandleInMainWindowSet.insert(QtCriticalMsg); 
    msgTypesToHandleInMainWindowSet.insert(QtFatalMsg); 
} 
//====================================================== 
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) 
{ 
    if(pMainWindow != nullptr && (msgTypesToHandleInMainWindowSet.contains(type))) 
    { 
        // let the application handles message (for example for saving in log) 
        pMainWindow->zp_handleStandardLogMessage(type, context, msg); 
    } 
 
    // Standard output 
    QString outputMsg = msg; 
    // it msg type is debug add context to message 
    if(type == QtDebugMsg) 
    { 
        QString contextString; 
        if(debugOutputOption & DO_ShowFile) 
        { 
            contextString += QString(context.file); 
        } 
        if(debugOutputOption & DO_ShowFunction) 
        { 
            if(!contextString.isEmpty()) 
            { 
                contextString += "; "; 
            } 
            contextString += QString(context.function); 
        } 
        if(debugOutputOption & DO_ShowLine) 
        { 
            if(!contextString.isEmpty()) 
            { 
                contextString += "; "; 
            } 
            contextString += "line:" + QString::number(context.line); 
        } 
        if(debugOutputOption & DO_ShowVersion) 
        { 
            if(!contextString.isEmpty()) 
            { 
                contextString += "; "; 
            } 
            contextString +=  "ver:" + QString::number(context.version); 
        } 
 
        if(!contextString.isEmpty()) 
        { 
            outputMsg += " ^| " + contextString; 
        } 
    } 
 
    std::cerr << outputMsg.toStdString() << std::endl; 
 
    if(type == QtFatalMsg) 
    { 
        abort(); 
    } 
} 
//====================================================== 
int main(int argc, char *argv[]) 
{ 
    QTextCodec* codec = QTextCodec::codecForName("windows-1251"); 
    QTextCodec::setCodecForLocale(codec); 
 
    QApplication a(argc, argv); 
    // 
    initMsgTypesToHandleInMainWindow(); 
    // custom message handler for logging via qInfo qWarning qCritical 
    qInstallMessageHandler(messageHandler); 
 
 
    // create qApp properties and set .pro defines into them 
#ifdef APP_DISPLAY_NAME 
    QApplication::setApplicationDisplayName(APP_DISPLAY_NAME); 
#endif 
 
#ifdef APP_PRODUCT 
    QApplication::setApplicationName(APP_PRODUCT); 
#endif 
 
#ifdef APP_VERSION 
    QApplication::setApplicationVersion(APP_VERSION); 
#endif 
 
#ifdef APP_COMPANY 
    QApplication::setOrganizationName(APP_COMPANY); 
#endif 
 
#ifdef APP_COMPANY_URL 
    QApplication::setOrganizationDomain(APP_COMPANY_URL); 
#endif 
 
#ifdef APP_COPYRIGHT 
    qApp->setProperty("appCopyright", QString(APP_COPYRIGHT)); 
#endif 
 
#ifdef APP_ICON 
    qApp->setProperty("appIcon", QString(APP_ICON)); 
#endif 
#ifdef APP_DESCRIPTION 
    qApp->setProperty("appDescription", QString(APP_DESCRIPTION)); 
#endif 
 
    //ZTranslatorManager languageManager; 
    //languageManager.zp_installTranslatorsToApplication(); 
 
 
    // set dots on the splitter handle 
    qApp->setStyleSheet( 
                "QSplitter::handle:vertical {height: 4px; image: url(:/images/ZImages/vSplitterHandler.png);}" 
                "QSplitter::handle:horizontal {width: 4px; image: url(:/images/ZImages/hSplitterHandler.png);}" 
                ); 
 
    // horizontal lines on table header views on win10 
    QOperatingSystemVersion currentOS = QOperatingSystemVersion::current(); 
    if (currentOS >= QOperatingSystemVersion(QOperatingSystemVersion::Windows10)) 
    { 
        qApp->setStyleSheet("QHeaderView::section{" 
                            "border-top:0px solid #D8D8D8;" 
                            "border-left:0px solid #D8D8D8;" 
                            "border-right:1px solid #D8D8D8;" 
                            "border-bottom: 1px solid #D8D8D8;" 
                            "background-color:white;" 
                            "padding:4px;" 
                            "}" 
                            "QTableCornerButton::section{" 
                            "border-top:0px solid #D8D8D8;" 
                            "border-left:0px solid #D8D8D8;" 
                            "border-right:1px solid #D8D8D8;" 
                            "border-bottom: 1px solid #D8D8D8;" 
                            "background-color:white;" 
                            "}"); 
    } 
 
    // launch app 
 
    // main window 
    MainWindow w; 
 
    // pointer to main window for message handler 
    pMainWindow = &w; 
    w.show(); 
    return a.exec(); 
} 
//=============================================== 
 
 
8. Add a new subproject for Qt Installer 
 
