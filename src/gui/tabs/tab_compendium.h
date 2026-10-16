#ifndef TAB_COMPENDIUM_H
#define TAB_COMPENDIUM_H

#include <QComboBox>
#include <QWidget>
#include "models/save_data.h"
#include <QTableWidgetItem>

namespace Ui {
class TabCompendium;
}

class TabCompendium : public QWidget
{
    Q_OBJECT

public:
    explicit TabCompendium(QWidget *parent = nullptr);
    ~TabCompendium();
    void load_save_data(SaveData *save);
private slots:
    void on_persona_add_btn_clicked();

    void on_compendium_tbl_itemSelectionChanged();

    void on_compendium_tbl_itemChanged(QTableWidgetItem *item);

private:
    Ui::TabCompendium *ui;
    SaveData *save_data_ = nullptr;
    std::array<QComboBox *, 8> skill_combos_{};
    void populate_static_data();
    void populate_skill_combos();
    void populate_persona_cmb();
    void configure_ui();
};

#endif // TAB_COMPENDIUM_H
