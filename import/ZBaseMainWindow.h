//======================================================
/*!
 * \brief The base main window provides the most common functionality
 *
 * The main window class provides the following functionality:
 * For receiving and further handling messages sent via function
 * qDebug, qWarning, qCritical and qFatal.
 * Help and about Actions. Help file should be in HTML format
 * and have the name EXE_BASE_NAME.
 * Saving and restoring geometry and state.
 * Loading app translator .qm by means ZTranslatorManager.
 * Provides Application Language menu.
 * Provides dockWidget Visible menu. The menu holds eventFilter for catching 
 * mouseButtonRelese  and preventing closing of the menu after 
 * an dockWidgetVisible action being toggled  
 * 
 *  qApp properties:
 *  appIcon
 *  author
 *  email
 * 
*/
//======================================================
#ifndef ZMAINWINDOWBASE_H
#define ZMAINWINDOWBASE_H
//======================================================
#include <QMainWindow>
#include <QWidget>
//======================================================
class QSettings;
class ZHelpBrowser;
//======================================================
class ZBaseMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ZBaseMainWindow(QWidget* parent = nullptr);

    virtual void zp_handleStandardLogMessage(QtMsgType type,
                                             const QMessageLogContext& context,
                                             const QString& msg) const;
    enum MenuIdentifier
    {
        MI_FILE = 0,
        MI_VIEW = 1,
        MI_EDIT = 2,
        MI_ACTIONS = 3,
        MI_HELP = 4
    };

signals:

    void zg_standardLogMessage(QtMsgType type, const QString& msg) const;

private slots:

    virtual void zh_help();
    virtual void zh_about();
    virtual void zh_exit();

protected:
    // VARS
    const QString zv_appGeometryString = "appGeometry";
    const QString zv_appStateString = "appState";

    ZHelpBrowser* zv_helpBrowser;

    QAction* zv_helpAction;
    QAction* zv_aboutAction;
    QAction* zv_aboutQtAction;
    QAction* zv_exitAction;
    QMenu* zv_languageMenu;
    QMenu* zv_dockWidgetMenu;

    // FUNCS
    bool eventFilter(QObject* obj, QEvent* event) override;
    void zh_createBaseActions();
    void zh_createBaseConnections();

    virtual void zh_saveSettings() const;
    virtual void zh_restoreSettings();
    void zh_appendActionsToMenu(QMenu* menu, int menuIdentifier);
    void zh_fillLanguageMenu();
    void zh_appLanguageControl();
    //    virtual bool zh_openSettingsGroup(QSettings* settings);
    //    virtual bool zh_closeSettingsGroup(QSettings* settings);
};
//======================================================
// MACROSES:
#ifndef ZMAIN_STYLES
#define ZMAIN_STYLES
#endif // ZMAIN_STYLES
#endif // ZMAINWINDOWBASE_H
