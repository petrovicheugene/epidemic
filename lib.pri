#------------------------------------------------- 
DESTDIR = $${LIBS_PATH}/ 

win32: { 
DLLDESTDIR = $${BIN_PATH}/ 
CONFIG += skip_target_version_ext 
} 

unix: { 
CONFIG += unversioned_libname 
} 

preCopyFiles($${_PRO_FILE_PWD_}/*.h, $${EXPORT_PATH}/) 

CONFIG(release, debug|release) { 
 postCopyFiles($${TRANSLATIONS_PATH}/*.qm, $${LIBS_TRANSLATIONS_PATH}) 
} 

