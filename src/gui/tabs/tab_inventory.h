#ifndef TAB_INVENTORY_H
#define TAB_INVENTORY_H

#include <QTableWidgetItem>
#include <QWidget>
#include "models/save_data.h"
#include "models/item.h"

namespace Ui {
class TabInventory;
}

class TabInventory : public QWidget
{
    Q_OBJECT

public:
    explicit TabInventory(QWidget *parent = nullptr);
    ~TabInventory();
    void load_save_data(SaveData *save);

private slots:
    void on_item_category_cmb_currentIndexChanged(int index);

    void on_item_tbl_itemDoubleClicked(QTableWidgetItem *item);

    void on_item_add_btn_clicked();

    void on_inventory_tbl_itemChanged(QTableWidgetItem *item);

private:
    Ui::TabInventory *ui;
    SaveData *save_data_ = nullptr;
    int find_inventory_row(std::uint16_t item_id) const;
    void add_item_to_inventory(std::uint16_t item_id);
    void add_row_inventory(const Item &item);
    void populate_category_cmb();
    void configure_ui();
};

#endif // TAB_INVENTORY_H
