mkdir bin
cd bin
mkdir debug
mkdir release
cd..
mkdir build
cd build
mkdir debug
mkdir release
cd..

mkdir import
mkdir export
mkdir lib.linux
mkdir lib.win

mkdir src
cd src
mkdir common
cd.. 

mkdir test

#app.pri
@echo #------------------------------------------------- > app.pri
@echo DESTDIR = $${BIN_PATH}/ >> app.pri
@echo linux-g++: QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../../lib.$${OS_SUFFIX}/ >> app.pri
@echo.>> app.pri
@echo CONFIG(debug, debug^|release) { >> app.pri
@echo     TARGET=$${TARGET}-$${VERSION} >> app.pri
@echo } else { >> app.pri
@echo     TARGET=$${TARGET} >> app.pri
@echo.>> app.pri
@echo     isEmpty(QMAKE_POST_LINK){ >> app.pri
@echo     QMAKE_POST_LINK += $$(QTDIR)/bin/windeployqt $${BIN_PATH} >> app.pri
@echo     }else{ >> app.pri
@echo     QMAKE_POST_LINK += ^& $$(QTDIR)/bin/windeployqt $${BIN_PATH} >> app.pri
@echo     } >> app.pri
@echo.>> app.pri
@echo     postCopyFiles($${TRANSLATIONS_PATH}/*.qm, $${BIN_PATH}/translations) >> app.pri
@echo } >> app.pri
@echo.>> app.pri

#lib.pri
@echo #------------------------------------------------- > lib.pri
@echo DESTDIR = $${LIBS_PATH}/ >> lib.pri
@echo.>> lib.pri
@echo win32: { >> lib.pri
@echo DLLDESTDIR = $${BIN_PATH}/ >> lib.pri
@echo CONFIG += skip_target_version_ext >> lib.pri
@echo } >> lib.pri
@echo.>> lib.pri
@echo unix: { >> lib.pri
@echo CONFIG += unversioned_libname >> lib.pri
@echo } >> lib.pri
@echo.>> lib.pri
@echo preCopyFiles($${_PRO_FILE_PWD_}/*.h, $${EXPORT_PATH}/) >> lib.pri
@echo.>> lib.pri
@echo CONFIG(release, debug^|release) { >> lib.pri
@echo  postCopyFiles($${TRANSLATIONS_PATH}/*.qm, $${LIBS_TRANSLATIONS_PATH}) >> lib.pri
@echo } >> lib.pri
@echo.>> lib.pri

#common.pri
@echo #------------------------------------------------- > common.pri
@echo LANGUAGES += en \ >> common.pri
@echo ru\ >> common.pri
@echo kk >> common.pri
@echo.>> common.pri
@echo PROJECT_ROOT_PATH = $${PWD}/ >> common.pri
@echo.>> common.pri
@echo win32: {OS_SUFFIX = win_$${QT_ARCH}} >> common.pri
@echo linux:!android: {OS_SUFFIX = linux_$${QT_ARCH}} >> common.pri
@echo linux:android: {OS_SUFFIX = android_$${QT_ARCH}} >> common.pri
@echo.>> common.pri
@echo CONFIG += c++11 >> common.pri
@echo CONFIG += c++14 >> common.pri
@echo CONFIG += c++17 >> common.pri
@echo.>> common.pri
@echo VERSION=$${VER_MAJ}.$${VER_MIN}.$${VER_PAT} >> common.pri
@echo.>> common.pri
@echo CONFIG(debug, debug^|release) { >> common.pri
@echo     BUILD_FLAG = debug >> common.pri
@echo     LIB_SUFFIX = d >> common.pri
@echo } else { >> common.pri
@echo     BUILD_FLAG = release >> common.pri
@echo } >> common.pri
@echo.>> common.pri
@echo #Define the preprocessor macro to get the application version in the application. >> common.pri
@echo DEFINES += APP_DISPLAY_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\" >> common.pri
@echo DEFINES += APP_PRODUCT=\"\\\"$${TARGET}\\\"\" >> common.pri
@echo DEFINES += APP_VERSION=\"\\\"$${VERSION}.$${VER_RELEASE}\\\"\" >> common.pri
@echo DEFINES += APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\" >> common.pri
@echo DEFINES += APP_COMPANY_URL=\"\\\"$${COMPANY_URL}\\\"\" >> common.pri
@echo DEFINES += APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\" >> common.pri
@echo DEFINES += APP_ICON=\"\\\"$${RC_ICONS}\\\"\" >> common.pri
@echo DEFINES += APP_DESCRIPTION=\"\\\"$${QMAKE_TARGET_DESCRIPTION}\\\"\" >> common.pri
@echo.>> common.pri
@echo #DEBUG SETTINGS >> common.pri
@echo CONFIG(release, debug^|release):DEFINES += QT_NO_DEBUG_OUTPUT >> common.pri
@echo #by default defined: in Debug mode QT_DEBUG, in Release mode QT_NO_DEBUG >> common.pri
@echo.>> common.pri
@echo SRC_PATH = $${PROJECT_ROOT_PATH}/src/ >> common.pri
@echo LIBS_PATH = $${PROJECT_ROOT_PATH}/lib.$${OS_SUFFIX}/ >> common.pri
@echo EXPORT_PATH = $${PROJECT_ROOT_PATH}/export/ >> common.pri
@echo IMPORT_PATH = $${PROJECT_ROOT_PATH}/import/ >> common.pri
@echo COMMON_PATH = $${SRC_PATH}/common/ >> common.pri
@echo TRANSLATIONS_PATH = $${PROJECT_ROOT_PATH}/translations/$${TARGET}/ >> common.pri
#@echo BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${BUILD_FLAG}/$${TARGET}/ >> common.pri
@echo BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${TARGET}_$${OS_SUFFIX}_$${BUILD_FLAG}/ >> common.pri
@echo mkpath($${SRC_PATH}) >> common.pri
@echo mkpath($${LIBS_PATH}) >> common.pri
@echo mkpath($${EXPORT_PATH}) >> common.pri
@echo mkpath($${IMPORT_PATH}) >> common.pri
@echo mkpath($${COMMON_PATH}) >> common.pri
@echo mkpath($${TRANSLATIONS_PATH}) >> common.pri
@echo mkpath($${BIN_PATH}) >> common.pri
@echo.>> common.pri
#@echo BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${BUILD_FLAG}/$${TARGET}/ >> common.pri
@echo BUILD_PATH = $${OUT_PWD} >> common.pri
@echo RCC_DIR = $${BUILD_PATH}/rcc/ >> common.pri
@echo UI_DIR = $${BUILD_PATH}/ui/ >> common.pri
@echo MOC_DIR = $${BUILD_PATH}/moc/ >> common.pri
@echo OBJECTS_DIR = $${BUILD_PATH}/obj/ >> common.pri
@echo mkpath($${BUILD_PATH}) >> common.pri
@echo mkpath($${RCC_DIR}) >> common.pri
@echo mkpath($${UI_DIR}) >> common.pri
@echo mkpath($${MOC_DIR}) >> common.pri
@echo mkpath($${OBJECTS_DIR}) >> common.pri
@echo.>> common.pri
@echo LIBS += -L$${LIBS_PATH} >> common.pri
@echo.>> common.pri
@echo INCLUDEPATH += $${EXPORT_PATH}/ >> common.pri
@echo INCLUDEPATH += $${IMPORT_PATH}/ >> common.pri
@echo INCLUDEPATH += $${COMMON_PATH}/ >> common.pri
@echo.>> common.pri
@echo win32 { >> common.pri
@echo     SHARED_LIB_FILES = $$files($${LIBS_PATH}/*.dll) >> common.pri
@echo     for(FILE, SHARED_LIB_FILES) { >> common.pri
@echo         BASENAME = $$basename(FILE) >> common.pri
@echo         LIBS += -l$$replace(BASENAME,.dll,) >> common.pri
@echo     } >> common.pri
@echo } >> common.pri
@echo unix { >> common.pri
@echo     SHARED_LIB_FILES = $$files($${LIBS_PATH}/*.so) >> common.pri
@echo     for(FILE, SHARED_LIB_FILES) { >> common.pri
@echo         BASENAME = $$basename(FILE) >> common.pri
@echo         LIBS += -l$$replace(BASENAME,.so,) >> common.pri
@echo     } >> common.pri
@echo } >> common.pri
@echo.>> common.pri
@echo QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter >> common.pri
@echo.>> common.pri
@echo # FUNCS >> common.pri
@echo # creates translation file names for languages from LANGUAGES variable >> common.pri
@echo defineReplace(prependAll) { >> common.pri
@echo  for(a,$$1):result += $$2$${a}$$3 >> common.pri
@echo  return($$result) >> common.pri
@echo } >> common.pri
@echo.>> common.pri
@echo # Copies the given files to the given directory >> common.pri
@echo defineTest(postCopyFiles) { >> common.pri
@echo     files = $$1 >> common.pri
@echo     DDIR = $$2 >> common.pri
@echo.>> common.pri
@echo     for(FILE, files) { >> common.pri
@echo         # Replace slashes in paths with backslashes for Windows >> common.pri
@echo         win32:FILE ~= s,/,\\,g >> common.pri
@echo         win32:DDIR ~= s,/,\\,g >> common.pri
@echo.>> common.pri
@echo         isEmpty(QMAKE_POST_LINK){ >> common.pri
@echo         QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t) >> common.pri
@echo         }else{ >> common.pri
@echo         QMAKE_POST_LINK += ^& $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t) >> common.pri
@echo         } >> common.pri
@echo     } >> common.pri
@echo     export(QMAKE_POST_LINK) >> common.pri
@echo } >> common.pri
@echo.>> common.pri
@echo defineTest(preCopyFiles) { >> common.pri
@echo     files = $$1 >> common.pri
@echo     DDIR = $$2 >> common.pri
@echo.>> common.pri
@echo     for(FILE, files) { >> common.pri
@echo         # Replace slashes in paths with backslashes for Windows >> common.pri
@echo         win32:FILE ~= s,/,\\,g >> common.pri
@echo         win32:DDIR ~= s,/,\\,g >> common.pri
@echo.>> common.pri
@echo         isEmpty(QMAKE_PRE_LINK){ >> common.pri
@echo         QMAKE_PRE_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t) >> common.pri
@echo         }else{ >> common.pri
@echo         QMAKE_PRE_LINK += ^& $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t) >> common.pri
@echo         } >> common.pri
@echo     } >> common.pri
@echo     export(QMAKE_PRE_LINK) >> common.pri
@echo } >> common.pri
@echo !isEmpty(LANGUAGES){ >> common.pri
@echo TRANSLATIONS = $$prependAll(LANGUAGES, $${TRANSLATIONS_PATH}/$${TARGET}_, .ts) >> common.pri
@echo QMAKE_PRE_LINK += $$(QTDIR)/bin/lupdate-pro $${_PRO_FILE_} >> common.pri
@echo.>> common.pri
@echo CONFIG(release, debug^|release) { >> common.pri
@echo     QMAKE_PRE_LINK += ^& $$(QTDIR)/bin/lrelease-pro $${_PRO_FILE_} >> common.pri
@echo }} >> common.pri
@echo.>> common.pri
@echo linux-g++: QMAKE_CXXFLAGS += -std=c++11 >> common.pri
@echo linux-g++: QMAKE_CXXFLAGS += -std=c++14 >> common.pri
@echo linux-g++: QMAKE_CXXFLAGS += -std=c++17 >> common.pri
@echo.>> common.pri

#README.md
@echo # README # > README.md
@echo.>> README.md
@echo This README would normally document whatever steps are necessary to get your application up and running. >> README.md
@echo.>> README.md
@echo ### What is this repository for? ### >> README.md
@echo.>> README.md
@echo * Quick summary >> README.md
@echo * Version >> README.md
@echo * [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo) >> README.md
@echo.>> README.md
@echo ### How do I get set up? ### >> README.md
@echo.>> README.md
@echo * Summary of set up >> README.md
@echo * Configuration >> README.md
@echo * Dependencies >> README.md
@echo * Database configuration >> README.md
@echo * How to run tests >> README.md
@echo * Deployment instructions >> README.md
@echo.>> README.md
@echo ### Contribution guidelines ### >> README.md
@echo.>> README.md
@echo * Writing tests >> README.md
@echo * Code review >> README.md
@echo * Other guidelines >> README.md
@echo.>> README.md
@echo ### Who do I talk to? ### >> README.md
@echo.>> README.md
@echo * Repo owner or admin >> README.md
@echo * Other community or team contact >> README.md
@echo.>> README.md

#gitignore
@echo # C++ objects and libs >> .gitignore
@echo *.slo >> .gitignore
@echo *.lo >> .gitignore
@echo *.o >> .gitignore
@echo *.a >> .gitignore
@echo *.la >> .gitignore
@echo *.lai >> .gitignore
@echo *.so >> .gitignore
@echo *.so.* >> .gitignore
@echo *.dll >> .gitignore
@echo *.dylib >> .gitignore
@echo. >> .gitignore
@echo # Qt-es >> .gitignore
@echo object_script.*.Release >> .gitignore
@echo object_script.*.Debug >> .gitignore
@echo *_plugin_import.cpp >> .gitignore
@echo /.qmake.cache >> .gitignore
@echo /.qmake.stash >> .gitignore
@echo *.pro.user >> .gitignore
@echo *.pro.user.* >> .gitignore
@echo *.qbs.user >> .gitignore
@echo *.qbs.user.* >> .gitignore
@echo *.moc >> .gitignore
@echo moc_*.cpp >> .gitignore
@echo moc_*.h >> .gitignore
@echo qrc_*.cpp >> .gitignore
@echo ui_*.h >> .gitignore
@echo *.qmlc >> .gitignore
@echo *.jsc >> .gitignore
@echo Makefile* >> .gitignore
@echo *build-* >> .gitignore
@echo *.qm >> .gitignore
@echo *.prl >> .gitignore
@echo. >> .gitignore
@echo # Qt unit tests >> .gitignore
@echo target_wrapper.* >> .gitignore
@echo. >> .gitignore
@echo # QtCreator >> .gitignore
@echo *.autosave >> .gitignore
@echo. >> .gitignore
@echo # QtCreator Qml >> .gitignore
@echo *.qmlproject.user >> .gitignore
@echo *.qmlproject.user.* >> .gitignore
@echo. >> .gitignore
@echo # QtCreator CMake >> .gitignore
@echo CMakeLists.txt.user* >> .gitignore
@echo. >> .gitignore
@echo # QtCreator 4.8< compilation database >> .gitignore 
@echo compile_commands.json >> .gitignore
@echo. >> .gitignore
@echo # QtCreator local machine specific files for imported projects >> .gitignore
@echo *creator.user* >> .gitignore
@echo. >> .gitignore
@echo *~ >> .gitignore
@echo *.autosave >> .gitignore
@echo *.a >> .gitignore
@echo *.core >> .gitignore
@echo *.moc >> .gitignore
@echo *.o >> .gitignore
@echo *.obj >> .gitignore
@echo *.orig >> .gitignore
@echo *.rej >> .gitignore
@echo *.so >> .gitignore
@echo *.so.* >> .gitignore
@echo *_pch.h.cpp >> .gitignore
@echo *_resource.rc >> .gitignore
@echo *.qm >> .gitignore
@echo .#* >> .gitignore
@echo *.*# >> .gitignore
@echo core >> .gitignore
@echo !core/ >> .gitignore
@echo tags >> .gitignore
@echo .DS_Store >> .gitignore
@echo .directory >> .gitignore
@echo *.debug >> .gitignore
@echo Makefile* >> .gitignore
@echo *.prl >> .gitignore
@echo *.app >> .gitignore
@echo moc_*.cpp >> .gitignore
@echo ui_*.h >> .gitignore
@echo qrc_*.cpp >> .gitignore
@echo Thumbs.db >> .gitignore
@echo *.res >> .gitignore
@echo *.rc >> .gitignore
@echo /.qmake.cache >> .gitignore
@echo /.qmake.stash >> .gitignore
@echo temp/*.* >> .gitignore
@echo temp/ >> .gitignore
@echo # qtcreator generated files >> .gitignore
@echo *.pro.user* >> .gitignore
@echo.>> .gitignore
@echo # xemacs temporary files >> .gitignore
@echo *.flc >> .gitignore
@echo.>> .gitignore
@echo # Vim temporary files >> .gitignore
@echo .*.swp >> .gitignore
@echo.>> .gitignore
@echo # Visual Studio generated files >> .gitignore
@echo *.ib_pdb_index >> .gitignore
@echo *.idb >> .gitignore
@echo *.ilk >> .gitignore
@echo *.pdb >> .gitignore
@echo *.sln >> .gitignore
@echo *.suo >> .gitignore
@echo *.vcproj >> .gitignore
@echo *vcproj.*.*.user >> .gitignore
@echo *.ncb >> .gitignore
@echo *.sdf >> .gitignore
@echo *.opensdf >> .gitignore
@echo *.vcxproj >> .gitignore
@echo *vcxproj.* >> .gitignore
@echo.>> .gitignore
@echo # MinGW generated files >> .gitignore
@echo *.Debug >> .gitignore
@echo *.Release >> .gitignore
@echo.>> .gitignore
@echo # Python byte code >> .gitignore
@echo *.pyc >> .gitignore
@echo.>> .gitignore
@echo # Binaries >> .gitignore
@echo # -------- >> .gitignore
@echo *.dll >> .gitignore
@echo *.exe >> .gitignore
@echo.>> .gitignore

#SUBDIRS_README.md
@echo # README # > SUBDIRS_README.md
@echo.>> SUBDIRS_README.md
@echo ### How to create subdir project ### >> SUBDIRS_README.md
@echo 1. Create a project from the Qt subdir template >> SUBDIRS_README.md
@echo 2. Define project default path in Build^&Run: >> SUBDIRS_README.md
@echo.>> SUBDIRS_README.md
#@echo old version - ../../%{JS: Util.asciify("build/%{CurrentKit:FileSystemName}-%{CurrentBuild:Name}/%{CurrentProject:Name}")} >> SUBDIRS_README.md
#@echo ../../%{JS: Util.asciify("build/%{CurrentBuild:Name}/%{CurrentKit:FileSystemName}-%{CurrentProject:Name}")} >> SUBDIRS_README.md
@echo ./%{JS: Util.asciify("build/%{CurrentBuild:Name}-%{CurrentKit:FileSystemName}-%{CurrentProject:Name}")} >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo and Projects Directory : Current directory  >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo 3. Run subDirInit.bat script. It creates all extra subfolders according to the subdir project schema  >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo . >> SUBDIRS_README.md
@echo ├── bin >> SUBDIRS_README.md
@echo │   ├── debug >> SUBDIRS_README.md
@echo │   └── release >> SUBDIRS_README.md
@echo ├── build >> SUBDIRS_README.md
@echo │   ├── debug >> SUBDIRS_README.md
@echo │   └── release >> SUBDIRS_README.md
@echo ├── import >> SUBDIRS_README.md
@echo ├── include >> SUBDIRS_README.md
@echo ├── lib.linux >> SUBDIRS_README.md
@echo ├── lib.win32 >> SUBDIRS_README.md
@echo ├── src >> SUBDIRS_README.md
@echo │   ├── include >> SUBDIRS_README.md
@echo │   ├── MyApp >> SUBDIRS_README.md
@echo │   └── MyLib >> SUBDIRS_README.md
@echo └── tests >> SUBDIRS_README.md
@echo     └── MyLibTest >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo 4. Add a subproject in the src directory >> SUBDIRS_README.md
@echo 5. Modify default .pro file for application by inserting the following after  >> SUBDIRS_README.md
@echo "greaterThan(QT_MAJOR_VERSION, 4): QT += widgets" >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #PRO VARS >> SUBDIRS_README.md
@echo TARGET = ^<SubAppName^> >> SUBDIRS_README.md
@echo #Application version >> SUBDIRS_README.md
@echo #RC_ICONS = "ZImages/SRVLab-8.ico" >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo VER_MAJ=0 >> SUBDIRS_README.md
@echo VER_MIN=0 >> SUBDIRS_README.md
@echo VER_PAT=0 >> SUBDIRS_README.md
@echo VER_BUILD=b >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo QMAKE_TARGET_PRODUCT="Sub App 1" >> SUBDIRS_README.md
@echo QMAKE_TARGET_DESCRIPTION="Sub App 1 app" >> SUBDIRS_README.md
@echo QMAKE_TARGET_COMPANY="TechnoAnalyt" >> SUBDIRS_README.md
@echo QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2017, 2018.  All rights reserved." >> SUBDIRS_README.md
@echo COMPANY_URL=tehnoanalit.com >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #------------------------------------------------- >> SUBDIRS_README.md
@echo # in common.pri will be defined VERSION, TARGET, DEBUG SETTINGS >> SUBDIRS_README.md
@echo #  global APP DEFINES >> SUBDIRS_README.md
@echo #------------------------------------------------- >> SUBDIRS_README.md
@echo include( ../../common.pri ) >> SUBDIRS_README.md
@echo include( ../../app.pri ) >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #END >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo 6. Modify default .pro file by inserting the following after  >> SUBDIRS_README.md
@echo TARGET = ^<SubLibName^> >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #PRO VARS >> SUBDIRS_README.md
@echo #Application version >> SUBDIRS_README.md
@echo #RC_ICONS = "ZImages/SRVLab-8.ico" >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo VER_MAJ=1 >> SUBDIRS_README.md
@echo VER_MIN=2 >> SUBDIRS_README.md
@echo VER_PAT=3 >> SUBDIRS_README.md
@echo VER_BUILD=b >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo QMAKE_TARGET_PRODUCT="SRV Lab" >> SUBDIRS_README.md
@echo QMAKE_TARGET_DESCRIPTION="Chemical analysis of X-ray spectra" >> SUBDIRS_README.md
@echo QMAKE_TARGET_COMPANY="TechnoAnalyt" >> SUBDIRS_README.md
@echo QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2017, 2018.  All rights reserved." >> SUBDIRS_README.md
@echo COMPANY_URL=tehnoanalit.com >> SUBDIRS_README.md
@echo LANGUAGES += en \ >> SUBDIRS_README.md
@echo     ru \ >> SUBDIRS_README.md
@echo     kk >> SUBDIRS_README.md
@echo #------------------------------------------------- >> SUBDIRS_README.md
@echo # in common.pri will be defined VERSION, TARGET, DEBUG SETTINGS >> SUBDIRS_README.md
@echo #  global APP DEFINES >> SUBDIRS_README.md
@echo #------------------------------------------------- >> SUBDIRS_README.md
@echo include( ../../common.pri ) >> SUBDIRS_README.md
@echo include( ../../lib.pri ) >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #END >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo 7. Modify main.cpp : >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo //=============================================== >> SUBDIRS_README.md
@echo #include "MainWindow.h" >> SUBDIRS_README.md
@echo #include "ZTranslatorManager.h" >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #include ^<iostream^> >> SUBDIRS_README.md
@echo #include ^<windows.h^> >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #include ^<QApplication^> >> SUBDIRS_README.md
@echo #include ^<QDir^> >> SUBDIRS_README.md
@echo #include ^<QOperatingSystemVersion^> >> SUBDIRS_README.md
@echo #include ^<QPixmap^> >> SUBDIRS_README.md
@echo #include ^<QSet^> >> SUBDIRS_README.md
@echo #include ^<QSplashScreen^> >> SUBDIRS_README.md
@echo #include ^<QTextCodec^> >> SUBDIRS_README.md
@echo #include ^<QTranslator^> >> SUBDIRS_README.md
@echo #include ^<iostream^> >> SUBDIRS_README.md
@echo //====================================================== >> SUBDIRS_README.md
@echo /*! >> SUBDIRS_README.md
@echo \brief  This module contains a function "messageHandler" for receiving standard >> SUBDIRS_README.md
@echo  messages qDebug, qInfo, qWarning, qCritical and qFatal. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo  Further these messages are redirected to MainWindow that should have a receive function >> SUBDIRS_README.md
@echo  "zp_handleStandardLogMessage(QtMsgType type, >> SUBDIRS_README.md
@echo                          const QMessageLogContext ^&context, >> SUBDIRS_README.md
@echo                          const QString ^&msg)". >> SUBDIRS_README.md
@echo  At the same time the messages are outputing in standard output stream >> SUBDIRS_README.md
@echo  std and are displayed in Qt Creator. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo  In class MainWindow the messages may be handled as log messages. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo  In main function qApp object gets new properties that hold the defines >> SUBDIRS_README.md
@echo  created in .pro file. These defines becomes accessable from every point >> SUBDIRS_README.md
@echo  of the code. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo */ >> SUBDIRS_README.md
@echo //====================================================== >> SUBDIRS_README.md
@echo namespace >> SUBDIRS_README.md
@echo { >> SUBDIRS_README.md
@echo // main window pointer >> SUBDIRS_README.md
@echo static MainWindow* pMainWindow = nullptr; >> SUBDIRS_README.md
@echo static QSet^<QtMsgType^> msgTypesToHandleInMainWindowSet; >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo // settings of format of debug output mssages to stderr >> SUBDIRS_README.md
@echo enum DebugOutputOption { >> SUBDIRS_README.md
@echo     DO_MsgOnly = 0x0, >> SUBDIRS_README.md
@echo     DO_ShowFile = 0x1, >> SUBDIRS_README.md
@echo     DO_ShowFunction = 0x2, >> SUBDIRS_README.md
@echo     DO_ShowLine = 0x4, >> SUBDIRS_README.md
@echo     DO_ShowVersion = 0x8}; >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo Q_DECLARE_FLAGS(DebugOutputOptions, DebugOutputOption) >> SUBDIRS_README.md
@echo Q_DECLARE_OPERATORS_FOR_FLAGS(DebugOutputOptions) >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo static DebugOutputOptions debugOutputOption = >> SUBDIRS_README.md
@echo        DO_MsgOnly >> SUBDIRS_README.md
@echo         ^| DO_ShowFile >> SUBDIRS_README.md
@echo         ^| DO_ShowFunction >> SUBDIRS_README.md
@echo         ^| DO_ShowLine >> SUBDIRS_README.md
@echo         // ^| DO_ShowVersion >> SUBDIRS_README.md
@echo         ; >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo } >> SUBDIRS_README.md
@echo //====================================================== >> SUBDIRS_README.md
@echo void initMsgTypesToHandleInMainWindow() >> SUBDIRS_README.md
@echo { >> SUBDIRS_README.md
@echo #ifndef QT_NO_DEBUG_OUTPUT >> SUBDIRS_README.md
@echo     msgTypesToHandleInMainWindowSet.insert(QtDebugMsg); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo     msgTypesToHandleInMainWindowSet.insert(QtInfoMsg); >> SUBDIRS_README.md
@echo     msgTypesToHandleInMainWindowSet.insert(QtWarningMsg); >> SUBDIRS_README.md
@echo     msgTypesToHandleInMainWindowSet.insert(QtCriticalMsg); >> SUBDIRS_README.md
@echo     msgTypesToHandleInMainWindowSet.insert(QtFatalMsg); >> SUBDIRS_README.md
@echo } >> SUBDIRS_README.md
@echo //====================================================== >> SUBDIRS_README.md
@echo void messageHandler(QtMsgType type, const QMessageLogContext ^&context, const QString ^&msg) >> SUBDIRS_README.md
@echo { >> SUBDIRS_README.md
@echo     if(pMainWindow != nullptr ^&^& (msgTypesToHandleInMainWindowSet.contains(type))) >> SUBDIRS_README.md
@echo     { >> SUBDIRS_README.md
@echo         // let the application handles message (for example for saving in log) >> SUBDIRS_README.md
@echo         pMainWindow-^>zp_handleStandardLogMessage(type, context, msg); >> SUBDIRS_README.md
@echo     } >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     // Standard output >> SUBDIRS_README.md
@echo     QString outputMsg = msg; >> SUBDIRS_README.md
@echo     // it msg type is debug add context to message >> SUBDIRS_README.md
@echo     if(type == QtDebugMsg) >> SUBDIRS_README.md
@echo     { >> SUBDIRS_README.md
@echo         QString contextString; >> SUBDIRS_README.md
@echo         if(debugOutputOption ^& DO_ShowFile) >> SUBDIRS_README.md
@echo         { >> SUBDIRS_README.md
@echo             contextString += QString(context.file); >> SUBDIRS_README.md
@echo         } >> SUBDIRS_README.md
@echo         if(debugOutputOption ^& DO_ShowFunction) >> SUBDIRS_README.md
@echo         { >> SUBDIRS_README.md
@echo             if(!contextString.isEmpty()) >> SUBDIRS_README.md
@echo             { >> SUBDIRS_README.md
@echo                 contextString += "; "; >> SUBDIRS_README.md
@echo             } >> SUBDIRS_README.md
@echo             contextString += QString(context.function); >> SUBDIRS_README.md
@echo         } >> SUBDIRS_README.md
@echo         if(debugOutputOption ^& DO_ShowLine) >> SUBDIRS_README.md
@echo         { >> SUBDIRS_README.md
@echo             if(!contextString.isEmpty()) >> SUBDIRS_README.md
@echo             { >> SUBDIRS_README.md
@echo                 contextString += "; "; >> SUBDIRS_README.md
@echo             } >> SUBDIRS_README.md
@echo             contextString += "line:" + QString::number(context.line); >> SUBDIRS_README.md
@echo         } >> SUBDIRS_README.md
@echo         if(debugOutputOption ^& DO_ShowVersion) >> SUBDIRS_README.md
@echo         { >> SUBDIRS_README.md
@echo             if(!contextString.isEmpty()) >> SUBDIRS_README.md
@echo             { >> SUBDIRS_README.md
@echo                 contextString += "; "; >> SUBDIRS_README.md
@echo             } >> SUBDIRS_README.md
@echo             contextString +=  "ver:" + QString::number(context.version); >> SUBDIRS_README.md
@echo         } >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo         if(!contextString.isEmpty()) >> SUBDIRS_README.md
@echo         { >> SUBDIRS_README.md
@echo             outputMsg += " ^| " + contextString; >> SUBDIRS_README.md
@echo         } >> SUBDIRS_README.md
@echo     } >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     std::cerr ^<^< outputMsg.toStdString() ^<^< std::endl; >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     if(type == QtFatalMsg) >> SUBDIRS_README.md
@echo     { >> SUBDIRS_README.md
@echo         abort(); >> SUBDIRS_README.md
@echo     } >> SUBDIRS_README.md
@echo } >> SUBDIRS_README.md
@echo //====================================================== >> SUBDIRS_README.md
@echo int main(int argc, char *argv[]) >> SUBDIRS_README.md
@echo { >> SUBDIRS_README.md
@echo     QTextCodec* codec = QTextCodec::codecForName("windows-1251"); >> SUBDIRS_README.md
@echo     QTextCodec::setCodecForLocale(codec); >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     QApplication a(argc, argv); >> SUBDIRS_README.md
@echo     // >> SUBDIRS_README.md
@echo     initMsgTypesToHandleInMainWindow(); >> SUBDIRS_README.md
@echo     // custom message handler for logging via qInfo qWarning qCritical >> SUBDIRS_README.md
@echo     qInstallMessageHandler(messageHandler); >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     // create qApp properties and set .pro defines into them >> SUBDIRS_README.md
@echo #ifdef APP_DISPLAY_NAME >> SUBDIRS_README.md
@echo     QApplication::setApplicationDisplayName(APP_DISPLAY_NAME); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #ifdef APP_PRODUCT >> SUBDIRS_README.md
@echo     QApplication::setApplicationName(APP_PRODUCT); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #ifdef APP_VERSION >> SUBDIRS_README.md
@echo     QApplication::setApplicationVersion(APP_VERSION); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #ifdef APP_COMPANY >> SUBDIRS_README.md
@echo     QApplication::setOrganizationName(APP_COMPANY); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #ifdef APP_COMPANY_URL >> SUBDIRS_README.md
@echo     QApplication::setOrganizationDomain(APP_COMPANY_URL); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #ifdef APP_COPYRIGHT >> SUBDIRS_README.md
@echo     qApp-^>setProperty("appCopyright", QString(APP_COPYRIGHT)); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo #ifdef APP_ICON >> SUBDIRS_README.md
@echo     qApp-^>setProperty("appIcon", QString(APP_ICON)); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo #ifdef APP_DESCRIPTION >> SUBDIRS_README.md
@echo     qApp-^>setProperty("appDescription", QString(APP_DESCRIPTION)); >> SUBDIRS_README.md
@echo #endif >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     //ZTranslatorManager languageManager; >> SUBDIRS_README.md
@echo     //languageManager.zp_installTranslatorsToApplication(); >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     // set dots on the splitter handle >> SUBDIRS_README.md
@echo     qApp-^>setStyleSheet( >> SUBDIRS_README.md
@echo                 "QSplitter::handle:vertical {height: 4px; image: url(:/images/ZImages/vSplitterHandler.png);}" >> SUBDIRS_README.md
@echo                 "QSplitter::handle:horizontal {width: 4px; image: url(:/images/ZImages/hSplitterHandler.png);}" >> SUBDIRS_README.md
@echo                 ); >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     // horizontal lines on table header views on win10 >> SUBDIRS_README.md
@echo     QOperatingSystemVersion currentOS = QOperatingSystemVersion::current(); >> SUBDIRS_README.md
@echo     if (currentOS ^>= QOperatingSystemVersion(QOperatingSystemVersion::Windows10)) >> SUBDIRS_README.md
@echo     { >> SUBDIRS_README.md
@echo         qApp-^>setStyleSheet("QHeaderView::section{" >> SUBDIRS_README.md
@echo                             "border-top:0px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "border-left:0px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "border-right:1px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "border-bottom: 1px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "background-color:white;" >> SUBDIRS_README.md
@echo                             "padding:4px;" >> SUBDIRS_README.md
@echo                             "}" >> SUBDIRS_README.md
@echo                             "QTableCornerButton::section{" >> SUBDIRS_README.md
@echo                             "border-top:0px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "border-left:0px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "border-right:1px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "border-bottom: 1px solid #D8D8D8;" >> SUBDIRS_README.md
@echo                             "background-color:white;" >> SUBDIRS_README.md
@echo                             "}"); >> SUBDIRS_README.md
@echo     } >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     // launch app >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     // main window >> SUBDIRS_README.md
@echo     MainWindow w; >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo     // pointer to main window for message handler >> SUBDIRS_README.md
@echo     pMainWindow = ^&w; >> SUBDIRS_README.md
@echo     w.show(); >> SUBDIRS_README.md
@echo     return a.exec(); >> SUBDIRS_README.md
@echo } >> SUBDIRS_README.md
@echo //=============================================== >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo 8. Add a new subproject for Qt Installer >> SUBDIRS_README.md
@echo. >> SUBDIRS_README.md
@echo. >> FINAL_MARK