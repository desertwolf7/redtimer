#pragma once

#include "MainWindow.h"
#include "Models.h"
#include "Window.h"

#include "qtredmine/SimpleRedmineClient.h"

#include <QObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QSettings>
#include <QSortFilterProxyModel>

namespace redtimer {

/**
 * @brief A settings window and IO access for RedTimer
 */
class Settings : public Window
{
    Q_OBJECT

public:
    /// Settings data structure for the currently loaded profile
    struct Data
    {
        /// @name GUI settings
        /// @{

        /// Redmine API key
        QString apiKey;

        /// Manually check the network connection
        bool checkConnection;

        /// Ignore SSL errors
        bool ignoreSslErrors;

        /// Maximum number of recently opened issues
        int numRecentIssues;

        /// Shortcuts
        QString shortcutCreateIssue;
        QString shortcutSelectIssue;
        QString shortcutStartStop;
        QString shortcutToggle;

        /// Redmine base URL
        QString url;

        /// Use custom fields
        bool useCustomFields;

        /// Use system tray icon
        bool useSystemTrayIcon;

        /// Close to tray
        bool closeToTray;

        /// Issue status to switch after tracking time
        int workedOnId;

        /// Default tracker to use in the Issue Creator
        int defaultTrackerId;

        /// ID of the time entry custom field for the start time
        int startTimeFieldId;

        /// ID of the time entry custom field for the end time
        int endTimeFieldId;

        /// @}

        /// @name Internal settings
        /// @{

        /// Last used activity
        int activityId;

        /// Last opened issue
        int issueId;

        /// Last opened project
        int projectId;

        /// Recently opened issues
        qtredmine::Issues recentIssues;

        /// @}

        /// @name Window data
        /// @{

        /// Window data of the Issue Creator
        WindowData issueCreator;

        /// Window data of the Issue Selector
        WindowData issueSelector;

        /// Window data of the main window
        WindowData mainWindow;

        /// Window data of the settings dialog
        WindowData settings;

        /// @}
    };

    /// Settings data
    Data data;

    /// Not applied settings data
    Data temp;

private:
    /// Redmine connection object
    qtredmine::SimpleRedmineClient* redmine_;

    /// Application settings
    QSettings settings_;

    /// Cached issue statuses
    SimpleModel issueStatusModel_;

    /// Cached trackers
    SimpleModel trackerModel_;

    /// Time entry custom fields for the start time
    SimpleModel startTimeModel_;

    /// Time entry custom fields for the end time
    SimpleModel endTimeModel_;

    /// GUI profiles
    SimpleModel profilesModel_;
    QSortFilterProxyModel profilesProxyModel_;

    /// Current profile ID
    int profileId_;

    /// Current profile hash
    QString profileHash_;

    /// List of initially loaded profiles
    QSet<int> loadedProfiles_;

private:
    /**
     * @brief Display a message box to get a profile name
     *
     * @param[out] name Profile name
     * @param[in]  title Title
     * @param[in]  initText Initial text
     *
     * @return true if valid profile name was specified, false otherwise
     */
    bool getProfileName( QString& name, QString title, QString initText );

public:
    /**
     * @brief Constructor for a Settings object
     *
     * @param mainWindow Main window object
     */
    explicit Settings( MainWindow* mainWindow );

    /**
     * @brief Load settings from settings file
     *
     * @param profile Load this profile instead of the last loaded
     */
    void load( const QString profile = QString() );

    /**
     * @brief Load profile-dependent settings from settings file
     */
    void loadProfileData();

    /**
     * @brief Save settings to settings file
     */
    void save();

public slots:
    /**
     * @brief Store the settings from the settings dialog in this class
     */
    void apply();

    /**
     * @brief Store the settings and close
     */
    void applyAndClose();

    /**
     * @brief Close the settings dialog
     */
    void close();

    /**
     * @brief Create a new profile
     *
     * @return true if profile was created successfully, false otherwise
     */
    bool createProfile();

    /**
     * @brief Cancel and close
     */
    void cancel();

    /**
     * @brief Delete the currently selected profile
     */
    void deleteProfile();

    /**
     * @brief Display the settings dialog
     *
     * @param loadData Load data from settings file before displaying
     */
    void display( bool loadData = true );

    /**
     * @brief A profile has been selected
     *
     * @param profileIndex Selected profile index
     */
    void profileSelected( int profileIndex );

    /**
     * @brief Rename the currently selected profile
     */
    void renameProfile();

    /**
     * @brief Update issue statuses
     */
    void updateIssueStatuses();

    /**
     * @brief Update trackers
     */
    void updateTrackers();

    /**
     * @brief Update time entry custom fields
     */
    void updateTimeEntryCustomFields();

    /**
     * @brief Toggle custom fields
     */
    void toggleCustomFields();

signals:
    /**
     * @brief Emitted when data have been applied in this GUI
     */
    void applied();
};

} // redtimer
