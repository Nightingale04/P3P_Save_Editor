#include "mainwindow.h"
#include <QCryptographicHash>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QMimeData>
#include "io/save_file_handler.h"
#include "tabs/tab_character.h"
#include "tabs/tab_compendium.h"
#include "tabs/tab_inventory.h"
#include "ui_mainwindow.h"
#include <filesystem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setMinimumSize(700, 500);

    tab_character_ = new TabCharacter(this);
    tab_inventory_ = new TabInventory(this);
    tab_compendium_ = new TabCompendium(this);
    ui->tabWidget->addTab(tab_character_, "Character");
    ui->tabWidget->addTab(tab_inventory_, "Inventory");
    ui->tabWidget->addTab(tab_compendium_, "Compendium");

    QAction *aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::show_about);
    ui->menubar->addAction(aboutAction);

    ui->tabWidget->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        event->ignore();
        return;
    }
    const QFileInfo file_info(urls.constFirst().toLocalFile());
    if (urls.constFirst().isLocalFile()
        && file_info.suffix().compare("bin", Qt::CaseInsensitive) == 0) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;
    const QString file_path = urls.constFirst().toLocalFile();
    auto choice = QMessageBox::question(this,
                                        "Save Type",
                                        "Is this a PC save?",
                                        QMessageBox::Yes | QMessageBox::No);
    bool is_pc_save = (choice == QMessageBox::Yes);
    load_save_file(file_path, is_pc_save);
}

void MainWindow::on_actionLoadSave_triggered()
{
    open_file_selector(false);
}
void MainWindow::on_actionLoad_PC_Save_triggered()
{
    open_file_selector(true);
}

void MainWindow::open_file_selector(bool is_pc_save)
{
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     tr("Open P3P Save File"),
                                                     "",
                                                     tr("Save File .BIN (.bin);;All File (*)"));
    if (!file_path.isEmpty()) {
        load_save_file(file_path, is_pc_save);
    }
}

void MainWindow::load_save_file(const QString &save_path, bool is_pc_save)
{
    std::filesystem::path path = QFileInfo(save_path).filesystemFilePath();

    if (is_pc_save) {
        std::filesystem::path slot_path = path;
        slot_path += "slot";
        if (!std::filesystem::exists(slot_path)) {
            QMessageBox::warning(this,
                                 "Missing .BINslot file",
                                 "The companion .BINslot file wasn't found next to this save.\n\n"
                                 "You can still view and edit the save, but saving won't patch the "
                                 "MD5 hash, the edited file will be rejected by the game.");
        }
    }
    std::optional<SaveData> loaded_save = save_file_handler::read_save_file(path);
    if (loaded_save.has_value()) {
        current_save_ = std::move(loaded_save);
        current_save_->is_pc_save = is_pc_save;
        populate_tabs();
        ui->tabWidget->setEnabled(true);
    } else {
        QMessageBox::critical(this, "Error", "Save file couldn't be loaded");
    }
}

void MainWindow::write_save()
{
    if (!current_save_.has_value()) {
        return;
    }
    std::filesystem::path slot_path = current_save_->file_path;
    slot_path += "slot";
    if (current_save_->is_pc_save) {
        if (!std::filesystem::exists(slot_path)) {
            auto choice = QMessageBox::warning(this,
                                               "Missing .BINslot file",
                                               "Without the .BINslot file the MD5 hash can't be "
                                               "updated and the game might reject the save.\n\n"
                                               "Save anyway?",
                                               QMessageBox::Save | QMessageBox::Cancel);
            if (choice == QMessageBox::Cancel)
                return;
        }
    }
    if (!save_file_handler::write_save_file(*current_save_)) {
        QMessageBox::critical(this, "Error", "There was an error trying to write the save file.");
        return;
    }

    if (current_save_->is_pc_save) {
        std::uint8_t *raw = current_save_->raw_buffer.data();
        std::vector<std::uint8_t> md5(QCryptographicHash::hashLength(QCryptographicHash::Md5));
        QCryptographicHash::hashInto(QSpan<std::byte>(reinterpret_cast<std::byte *>(md5.data()),
                                                      md5.size()),
                                     QByteArrayView(reinterpret_cast<const char *>(raw),
                                                    qsizetype(current_save_->raw_buffer.size())),
                                     QCryptographicHash::Md5);
        if (!save_file_handler::write_bin_slot(slot_path, md5)) {
            QMessageBox::critical(this, "Error", "Failed to update the .BINslot file.");
            return;
        }
    }
    QMessageBox::information(this, "Success", "Changes saved successfully.");
}

void MainWindow::populate_tabs()
{
    if (!current_save_.has_value())
        return;

    SaveData *save_ptr = &current_save_.value();

    tab_character_->load_save_data(save_ptr);
    tab_inventory_->load_save_data(save_ptr);
    tab_compendium_->load_save_data(save_ptr);
}

void MainWindow::on_actionSave_triggered()
{
    write_save();
}

void MainWindow::show_about()
{
    QMessageBox::about(
        this,
        tr("About %1").arg(QCoreApplication::applicationName()),
        tr("<h3>%1</h3>"
           "<p>Version %2<br>"
           "Save editor for Persona 3 Portable.<br>"
           "Built with Qt %3</p>"
           "<p>Developed by <a href=\"https://github.com/Nightingale04\">Nightingale</a></p>")
            .arg(QCoreApplication::applicationName(),
                 QCoreApplication::applicationVersion(),
                 QLatin1String(QT_VERSION_STR)));
}
