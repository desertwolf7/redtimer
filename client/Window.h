#pragma once

#include <functional>

#include <QEvent>
#include <QHash>
#include <QObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QString>

namespace redtimer {

// forward declaration
class MainWindow;

class Window : public QQuickView
{
    Q_OBJECT

private:
    /// Emit the closed signal upon closing
    bool emitClosedSignal_ = true;

    /// Close callback
    std::function<void()> closeCb_;

    /// Already displayed messages
    QHash<QString, bool> displayed_;

protected:
    /// Counter to ensure that there are no idle callbacks after deleting the object
    int callbackCounter_ = 0;

    /// Flag to determine whether deleteLater() has been called
    bool deleteLater_ = false;

    /// Main item
    QQuickItem* item_;

    /// Window context
    QQmlContext* ctx_;

    /// Main window
    MainWindow* mainWindow_;

public:
    /// Position and size of windows
    struct Data
    {
        /// Last window size
        QRect geometry;

        /// Last window position
        QPoint position;
    };

    /**
     * @brief Window constructor for a window from a QML file within a Qt resource file
     *
     * @param qml Path to the QML file within the Qt resouce file
     * @param mainWindow Main window object
     * @param closeCb Close callback
     */
    Window( QString qml, MainWindow* mainWindow, std::function<void()> closeCb = nullptr );

    /**
     * @brief Get the main window
     *
     * @return Main window object
     */
    MainWindow* mainWindow();

    /**
     * @brief Get a QML GUI item
     *
     * Fetches the root item if \c qmlItem is empty.
     *
     * @param qmlItem Name of the QML GUI item
     *
     * @return QML GUI item
     */
    QQuickItem* qml( QString qmlItem = "" );

protected:
    /**
     * @brief Filter Qt events
     *
     * Emits the canceled signal when window has been closed.
     *
     * @param event Received event
     *
     * @return true if event has been processed, false otherwise
     */
    bool event( QEvent* event );

    /**
     * @brief Filter Qt key events
     *
     * Close window with Esc key.
     *
     * @param event Received key event
     */
    void keyPressEvent( QKeyEvent* event );

    /**
     * @brief Get window data
     *
     * @param windowData Window data
     */
    Data getWindowData();

    /**
     * @brief Set window data
     *
     * @param windowData Window data
     */
    void setWindowData( Data windowData );

public slots:
    /**
     * @brief Delete the object after all callbacks have finished
     */
    void deleteLater();

    /**
     * @brief Display a message
     *
     * @param text Message to display
     * @param type Message type (one of \c QtInfoMsg, \c QtWarningMsg and \c QtCriticalMsg)
     * @param timer Timer to use to hide the message
     */
    QQuickItem* message( QString text, QTimer* timer, QtMsgType type = QtInfoMsg );

    /**
     * @brief Display a message
     *
     * @param text Message to display
     * @param type Message type (one of \c QtInfoMsg, \c QtWarningMsg and \c QtCriticalMsg)
     * @param timeout Duration in milliseconds that the message will be displayed
     */
    QQuickItem* message( QString text, QtMsgType type = QtInfoMsg, int timeout = 5000 );

signals:
    /**
     * @brief Signal that will be emitted when closing the window
     *
     * This signal will be emitted only once.
     */
    void closed();
};

/**
 * @brief QDebug stream operator for window data
 *
 * @param debug QDebug stream
 * @param item Window data
 *
 * @return QDebug stream
 */
inline QDebug
operator<<( QDebug debug, const Window::Data& item )
{
    QDebugStateSaver saver( debug );
    debug.nospace() << "[position: " << item.position << ", geometry: \"" << item.geometry << "\"]";

    return debug;
}

} // redtimer
