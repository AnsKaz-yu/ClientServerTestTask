#pragma once

// ============================================================
// mainwindow.h
// ============================================================

#include "server_worker.h"
#include "client_config.h"
#include "client_handler.h"

#include <QMainWindow>
#include <QThread>
#include <QMap>
#include <QString>

// Forward declarations (avoids pulling heavy Qt headers into the .h)
class QTableWidget;
class QTextEdit;
class QPushButton;
class QLabel;
class QSplitter;
class QGroupBox;
class QStatusBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    // --- toolbar / button actions ---
    void onStartServer();
    void onStopServer();
    void onStartClients();
    void onStopClients();
    void onClearLog();
    void onConfigureClient();   ///< Opens the settings dialog for selected client.

    // --- signals from ServerWorker (via queued connection across threads) ---
    void onClientConnected(const QString& client_id,
                           const QString& ip,
                           quint16        port);
    void onClientDisconnected(const QString& client_id);
    void onDataReceived(const DataRecord& record);
    void onLogMessage(const QString& message);
    void onServerStarted(quint16 port);
    void onServerStopped();

private:
    // --- UI construction ---
    void buildUi();
    void buildToolBar();
    void connectSignals();

    // --- helpers ---
    void appendLog(const QString& message);
    void setServerRunning(bool running);
    int  findClientRow(const QString& client_id) const;

    // --- worker ---
    QThread*       worker_thread_{nullptr};
    ServerWorker*  worker_{nullptr};

    // --- widgets ---
    QTableWidget*  client_table_{nullptr};
    QTableWidget*  data_table_{nullptr};
    QTextEdit*     log_view_{nullptr};
    QPushButton*   btn_start_server_{nullptr};
    QPushButton*   btn_stop_server_{nullptr};
    QPushButton*   btn_start_clients_{nullptr};
    QPushButton*   btn_stop_clients_{nullptr};
    QPushButton*   btn_configure_{nullptr};
    QPushButton*   btn_clear_log_{nullptr};
    QLabel*        status_label_{nullptr};

    // --- state ---
    QMap<QString, ClientConfig> client_configs_;
    bool server_running_{false};
};
