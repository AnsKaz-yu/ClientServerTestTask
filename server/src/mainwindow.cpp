// ============================================================
// mainwindow.cpp
// ============================================================

#include "mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QTableWidget>
#include <QTextEdit>
#include <QThread>
#include <QToolBar>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QMessageBox>
#include <QFont>

// ---- column indices: client table ----
enum ClientCol {
    kColClientId = 0,
    kColClientIp,
    kColClientPort,
    kColClientStatus,
    kClientColCount
};

// ---- column indices: data table ----
enum DataCol {
    kColDataClientId = 0,
    kColDataType,
    kColDataContent,
    kColDataTime,
    kDataColCount
};

// ============================================================
// Constructor / Destructor
// ============================================================

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Device Monitor Server");
    resize(1200, 800);

    buildUi();
    buildToolBar();
    connectSignals();
    setServerRunning(false);

    appendLog("Application started. Press 'Start Server' to begin.");
}

MainWindow::~MainWindow() {
    if (worker_thread_ && worker_thread_->isRunning()) {
        worker_thread_->quit();
        worker_thread_->wait(3000);
    }
}

// ============================================================
// UI construction
// ============================================================

void MainWindow::buildUi() {
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* main_layout = new QVBoxLayout(central);
    main_layout->setContentsMargins(6, 6, 6, 6);
    main_layout->setSpacing(6);

    // ---- top splitter: left = client table, right = data table ----
    auto* h_splitter = new QSplitter(Qt::Horizontal, central);

    // Client table group
    auto* client_group = new QGroupBox("Connected Clients", h_splitter);
    auto* client_layout = new QVBoxLayout(client_group);

    client_table_ = new QTableWidget(0, kClientColCount, client_group);
    client_table_->setHorizontalHeaderLabels(
        {"Client ID", "IP Address", "Port", "Status"});
    client_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    client_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    client_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    client_table_->setAlternatingRowColors(true);
    client_layout->addWidget(client_table_);

    auto* btn_row = new QHBoxLayout;
    btn_start_clients_ = new QPushButton("▶  Start All Clients", client_group);
    btn_stop_clients_  = new QPushButton("■  Stop All Clients",  client_group);
    btn_configure_     = new QPushButton("⚙  Configure...",      client_group);
    btn_row->addWidget(btn_start_clients_);
    btn_row->addWidget(btn_stop_clients_);
    btn_row->addWidget(btn_configure_);
    client_layout->addLayout(btn_row);

    h_splitter->addWidget(client_group);

    // Data table group
    auto* data_group = new QGroupBox("Received Data", h_splitter);
    auto* data_layout = new QVBoxLayout(data_group);

    data_table_ = new QTableWidget(0, kDataColCount, data_group);
    data_table_->setHorizontalHeaderLabels(
        {"Client ID", "Type", "Content", "Time"});
    data_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    data_table_->horizontalHeader()->setStretchLastSection(false);
    data_table_->horizontalHeader()->resizeSection(kColDataContent, 420);
    data_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    data_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    data_table_->setAlternatingRowColors(true);
    data_layout->addWidget(data_table_);

    h_splitter->addWidget(data_group);
    h_splitter->setStretchFactor(0, 1);
    h_splitter->setStretchFactor(1, 3);

    main_layout->addWidget(h_splitter, 3);

    // ---- log panel ----
    auto* log_group = new QGroupBox("Event Log", central);
    auto* log_layout = new QVBoxLayout(log_group);

    log_view_ = new QTextEdit(log_group);
    log_view_->setReadOnly(true);
    log_view_->setFont(QFont("Consolas", 9));
    log_layout->addWidget(log_view_);

    auto* log_btn_row = new QHBoxLayout;
    btn_clear_log_ = new QPushButton("Clear Log", log_group);
    log_btn_row->addStretch();
    log_btn_row->addWidget(btn_clear_log_);
    log_layout->addLayout(log_btn_row);

    main_layout->addWidget(log_group, 1);

    // ---- status bar ----
    status_label_ = new QLabel("Server: Stopped");
    statusBar()->addPermanentWidget(status_label_);
}

void MainWindow::buildToolBar() {
    QToolBar* toolbar = addToolBar("Main");
    toolbar->setMovable(false);

    btn_start_server_ = new QPushButton("▶  Start Server", this);
    btn_start_server_->setFixedHeight(32);
    btn_stop_server_  = new QPushButton("■  Stop Server",  this);
    btn_stop_server_->setFixedHeight(32);

    toolbar->addWidget(btn_start_server_);
    toolbar->addWidget(btn_stop_server_);
}

void MainWindow::connectSignals() {
    // --- buttons ---
    connect(btn_start_server_,  &QPushButton::clicked, this, &MainWindow::onStartServer);
    connect(btn_stop_server_,   &QPushButton::clicked, this, &MainWindow::onStopServer);
    connect(btn_start_clients_, &QPushButton::clicked, this, &MainWindow::onStartClients);
    connect(btn_stop_clients_,  &QPushButton::clicked, this, &MainWindow::onStopClients);
    connect(btn_configure_,     &QPushButton::clicked, this, &MainWindow::onConfigureClient);
    connect(btn_clear_log_,     &QPushButton::clicked, this, &MainWindow::onClearLog);

    // --- worker thread ---
    worker_thread_ = new QThread(this);
    worker_        = new ServerWorker();
    worker_->moveToThread(worker_thread_);

    // Worker signals -> GUI slots (all queued across thread boundary automatically).
    connect(worker_, &ServerWorker::clientConnected,
            this,    &MainWindow::onClientConnected);
    connect(worker_, &ServerWorker::clientDisconnected,
            this,    &MainWindow::onClientDisconnected);
    connect(worker_, &ServerWorker::dataReceived,
            this,    &MainWindow::onDataReceived);
    connect(worker_, &ServerWorker::logMessage,
            this,    &MainWindow::onLogMessage);
    connect(worker_, &ServerWorker::serverStarted,
            this,    &MainWindow::onServerStarted);
    connect(worker_, &ServerWorker::serverStopped,
            this,    &MainWindow::onServerStopped);

    // Thread lifecycle.
    connect(worker_thread_, &QThread::finished,
            worker_,        &QObject::deleteLater);

    worker_thread_->start();
}

// ============================================================
// Toolbar / button slots
// ============================================================

void MainWindow::onStartServer() {
    // Invoke the worker's start() method in the worker thread.
    QMetaObject::invokeMethod(worker_, [this]() {
        worker_->start(12345);
    }, Qt::QueuedConnection);
}

void MainWindow::onStopServer() {
    QMetaObject::invokeMethod(worker_, [this]() {
        worker_->stop();
    }, Qt::QueuedConnection);
}

void MainWindow::onStartClients() {
    QMetaObject::invokeMethod(worker_, [this]() {
        worker_->startAllClients();
    }, Qt::QueuedConnection);
}

void MainWindow::onStopClients() {
    QMetaObject::invokeMethod(worker_, [this]() {
        worker_->stopAllClients();
    }, Qt::QueuedConnection);
}

void MainWindow::onClearLog() {
    log_view_->clear();
}

void MainWindow::onConfigureClient() {
    const auto selected = client_table_->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "Configure Client",
            "Please select a client row first.");
        return;
    }

    const int row = client_table_->row(selected.first());
    const QString client_id =
        client_table_->item(row, kColClientId)->text();

    // Fetch existing config or create a default one.
    ClientConfig cfg = client_configs_.value(client_id, ClientConfig(client_id));

    // ---- Settings Dialog ----
    QDialog dlg(this);
    dlg.setWindowTitle(QString("Configure %1").arg(client_id));
    dlg.setMinimumWidth(380);

    auto* form = new QFormLayout(&dlg);

    auto* sb_bw  = new QDoubleSpinBox(&dlg);
    sb_bw->setRange(0, 100000); sb_bw->setSuffix(" Mbps");
    sb_bw->setValue(cfg.thresholds().max_bandwidth);

    auto* sb_lat = new QDoubleSpinBox(&dlg);
    sb_lat->setRange(0, 10000); sb_lat->setSuffix(" ms");
    sb_lat->setValue(cfg.thresholds().max_latency);

    auto* sb_loss = new QDoubleSpinBox(&dlg);
    sb_loss->setRange(0, 100); sb_loss->setSuffix(" %");
    sb_loss->setDecimals(3);
    sb_loss->setValue(cfg.thresholds().max_packet_loss);

    auto* sb_cpu = new QSpinBox(&dlg);
    sb_cpu->setRange(0, 100); sb_cpu->setSuffix(" %");
    sb_cpu->setValue(cfg.thresholds().max_cpu_usage);

    auto* sb_mem = new QSpinBox(&dlg);
    sb_mem->setRange(0, 100); sb_mem->setSuffix(" %");
    sb_mem->setValue(cfg.thresholds().max_memory_usage);

    form->addRow("Max Bandwidth:",    sb_bw);
    form->addRow("Max Latency:",      sb_lat);
    form->addRow("Max Packet Loss:",  sb_loss);
    form->addRow("Max CPU Usage:",    sb_cpu);
    form->addRow("Max Memory Usage:", sb_mem);

    auto* btns = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btns);

    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    cfg.thresholds().max_bandwidth    = sb_bw->value();
    cfg.thresholds().max_latency      = sb_lat->value();
    cfg.thresholds().max_packet_loss  = sb_loss->value();
    cfg.thresholds().max_cpu_usage    = sb_cpu->value();
    cfg.thresholds().max_memory_usage = sb_mem->value();

    client_configs_.insert(client_id, cfg);

    // Forward to the worker (queued = thread-safe).
    const ClientConfig cfg_copy = cfg;
    QMetaObject::invokeMethod(worker_, [this, cfg_copy]() {
        worker_->updateClientConfig(cfg_copy);
    }, Qt::QueuedConnection);
}

// ============================================================
// ServerWorker signal handlers (run on GUI thread)
// ============================================================

void MainWindow::onClientConnected(const QString& client_id,
                                    const QString& ip,
                                    quint16        port) {
    // Add a new row to the client table.
    const int row = client_table_->rowCount();
    client_table_->insertRow(row);
    client_table_->setItem(row, kColClientId,     new QTableWidgetItem(client_id));
    client_table_->setItem(row, kColClientIp,     new QTableWidgetItem(ip));
    client_table_->setItem(row, kColClientPort,   new QTableWidgetItem(QString::number(port)));
    client_table_->setItem(row, kColClientStatus, new QTableWidgetItem("Connected"));

    // Create a default config entry.
    if (!client_configs_.contains(client_id)) {
        client_configs_.insert(client_id, ClientConfig(client_id));
    }

    appendLog(QString("[+] Client %1 connected (%2:%3)").arg(client_id, ip).arg(port));
}

void MainWindow::onClientDisconnected(const QString& client_id) {
    const int row = findClientRow(client_id);
    if (row >= 0) {
        auto* status_item = client_table_->item(row, kColClientStatus);
        if (status_item) {
            status_item->setText("Disconnected");
            status_item->setForeground(Qt::red);
        }
    }
    appendLog(QString("[-] Client %1 disconnected").arg(client_id));
}

void MainWindow::onDataReceived(const DataRecord& record) {
    // Append a row to the data table (newest first for readability).
    const int row = 0;
    data_table_->insertRow(row);
    data_table_->setItem(row, kColDataClientId, new QTableWidgetItem(record.client_id));
    data_table_->setItem(row, kColDataType,     new QTableWidgetItem(record.data_type));
    data_table_->setItem(row, kColDataContent,  new QTableWidgetItem(record.content));
    data_table_->setItem(row, kColDataTime,
        new QTableWidgetItem(record.timestamp.toString("hh:mm:ss.zzz")));

    // Colour-code by type for quick visual scanning.
    QColor row_color;
    if (record.data_type == "NetworkMetrics")   row_color = QColor(220, 240, 255);
    else if (record.data_type == "DeviceStatus") row_color = QColor(220, 255, 220);
    else if (record.data_type == "Log") {
        const QString sev = record.log_entry.severity;
        if (sev == "ERROR")        row_color = QColor(255, 200, 200);
        else if (sev == "WARNING") row_color = QColor(255, 240, 200);
        else                       row_color = QColor(255, 255, 255);
    }

    if (row_color.isValid()) {
        for (int col = 0; col < kDataColCount; ++col) {
            if (auto* itm = data_table_->item(row, col)) {
                itm->setBackground(row_color);
            }
        }
    }

    // Cap the table at 2000 rows to avoid unbounded memory growth.
    constexpr int kMaxRows = 2000;
    if (data_table_->rowCount() > kMaxRows) {
        data_table_->removeRow(data_table_->rowCount() - 1);
    }
}

void MainWindow::onLogMessage(const QString& message) {
    appendLog(message);
}

void MainWindow::onServerStarted(quint16 port) {
    setServerRunning(true);
    appendLog(QString("Server started on port %1").arg(port));
}

void MainWindow::onServerStopped() {
    setServerRunning(false);
    appendLog("Server stopped");
}

// ============================================================
// Overrides
// ============================================================

void MainWindow::closeEvent(QCloseEvent* event) {
    // Stop the worker thread gracefully before closing.
    if (worker_thread_ && worker_thread_->isRunning()) {
        QMetaObject::invokeMethod(worker_, [this]() {
            worker_->stop();
        }, Qt::BlockingQueuedConnection);
        worker_thread_->quit();
        worker_thread_->wait(3000);
    }
    event->accept();
}

// ============================================================
// Helpers
// ============================================================

void MainWindow::appendLog(const QString& message) {
    const QString ts = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
    log_view_->append(ts + message);
}

void MainWindow::setServerRunning(bool running) {
    server_running_ = running;
    btn_start_server_->setEnabled(!running);
    btn_stop_server_->setEnabled(running);
    btn_start_clients_->setEnabled(running);
    btn_stop_clients_->setEnabled(running);
    btn_configure_->setEnabled(running);

    status_label_->setText(running ? "Server: Running on port 12345"
                                   : "Server: Stopped");
}

int MainWindow::findClientRow(const QString& client_id) const {
    for (int row = 0; row < client_table_->rowCount(); ++row) {
        const auto* item = client_table_->item(row, kColClientId);
        if (item && item->text() == client_id) {
            return row;
        }
    }
    return -1;
}
