#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "models/save_data.h"
#include "tabs/tab_character.h"
#include "tabs/tab_compendium.h"
#include "tabs/tab_inventory.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_actionLoadSave_triggered();

    void on_actionLoad_PC_Save_triggered();

private:
    Ui::MainWindow *ui;

    TabCharacter *tab_character_{nullptr};
    TabInventory *tab_inventory_{nullptr};
    TabCompendium *tab_compendium_{nullptr};

    std::optional<SaveData> current_save_;
    void load_save_file(const QString &file_path, bool is_pc_save);
    void write_save();
    void populate_tabs();
    void open_file_selector(bool is_pc_save);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
#endif // MAINWINDOW_H
