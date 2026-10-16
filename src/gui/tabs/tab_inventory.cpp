#include "tab_inventory.h"
#include "data/item_repository.h"
#include "gui/delegates/spin_box_delegate.h"
#include "models/item.h"
#include "ui_tab_inventory.h"
#include <iostream>

TabInventory::TabInventory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TabInventory)
{
    ui->setupUi(this);

    configure_ui();
    populate_category_cmb();
}

TabInventory::~TabInventory()
{
    delete ui;
}

void TabInventory::add_row_inventory(const Item &item)
{
    const int row = ui->inventory_tbl->rowCount();
    ui->inventory_tbl->insertRow(row);

    QTableWidgetItem *item_name = new QTableWidgetItem(QString::fromStdString(item.name));
    item_name->setData(Qt::UserRole, item.id);
    item_name->setToolTip(QString::fromStdString(item.description));
    item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);

    QTableWidgetItem *item_category = new QTableWidgetItem(
        QString::fromStdString(item.get_type_name()));
    item_category->setFlags(item_category->flags() & ~Qt::ItemIsEditable);

    QTableWidgetItem *item_amount = new QTableWidgetItem();
    item_amount->setData(Qt::DisplayRole, save_data_->inventory[item.id]);
    item_amount->setData(Qt::UserRole, item.id);
    ui->inventory_tbl->setItem(row, 0, item_name);
    ui->inventory_tbl->setItem(row, 1, item_category);
    ui->inventory_tbl->setItem(row, 2, item_amount);
}

void TabInventory::load_save_data(SaveData *save)
{
    save_data_ = save;
    if (!save_data_) {
        return;
    };

    const QSignalBlocker inventory_block(ui->inventory_tbl);

    ui->inventory_tbl->setUpdatesEnabled(false);
    ui->inventory_tbl->setSortingEnabled(false);
    ui->inventory_tbl->clearContents();
    ui->inventory_tbl->setRowCount(0);

    for (std::size_t i = 0; i < save_data_->inventory.size(); ++i) {
        if (save_data_->inventory[i] == 0) {
            continue;
        }
        const Item &item = ItemRepository::instance().get_by_id(static_cast<std::uint16_t>(i));
        add_row_inventory(item);
    }
    ui->inventory_tbl->setUpdatesEnabled(true);
    ui->inventory_tbl->setSortingEnabled(true);
    return;
}

void TabInventory::populate_category_cmb()
{
    ui->item_category_cmb->clear();
    ui->item_category_cmb->addItem("Weapons", static_cast<int>(ItemType::Weapon));
    ui->item_category_cmb->addItem("Armors", static_cast<int>(ItemType::Armor));
    ui->item_category_cmb->addItem("Boots", static_cast<int>(ItemType::Boot));
    ui->item_category_cmb->addItem("Accessories", static_cast<int>(ItemType::Accessory));
    ui->item_category_cmb->addItem("Consumables", static_cast<int>(ItemType::Consumable));
    ui->item_category_cmb->addItem("Fusion Items", static_cast<int>(ItemType::Fusion_Item));
    ui->item_category_cmb->addItem("Incense Cards", static_cast<int>(ItemType::Incense_Card));
    ui->item_category_cmb->addItem("Gifts", static_cast<int>(ItemType::Gift));
    ui->item_category_cmb->addItem("Gemstones", static_cast<int>(ItemType::Gemstone));
    ui->item_category_cmb->addItem("Quest Items", static_cast<int>(ItemType::Quest));
    ui->item_category_cmb->addItem("Valuables", static_cast<int>(ItemType::Valuable));
    ui->item_category_cmb->addItem("Key Items", static_cast<int>(ItemType::Key));
    ui->item_category_cmb->addItem("Fusion Spells", static_cast<int>(ItemType::Fusion_Spell));
    ui->item_category_cmb->addItem("Skill Cards", static_cast<int>(ItemType::Skill_Card));
    ui->item_category_cmb->addItem("Debug items", ui->item_category_cmb->count() + 1);
    ui->item_category_cmb->setCurrentIndex(0);
}

void TabInventory::on_item_category_cmb_currentIndexChanged(int index)
{
    if (index < 0)
        return;

    int selected_type = ui->item_category_cmb->itemData(index).toInt();
    std::vector<const Item *> items;
    if (selected_type == 15) {
        items = ItemRepository::instance().get_debug();
        ui->item_tbl->setColumnHidden(1, false);
    } else {
        items = ItemRepository::instance().get_by_type(static_cast<ItemType>(selected_type));
        ui->item_tbl->setColumnHidden(1, true);
    }
    const int row_count = static_cast<int>(items.size());
    const QSignalBlocker item_tbl_block(ui->item_tbl);
    ui->item_tbl->setUpdatesEnabled(false);
    ui->item_tbl->setSortingEnabled(false);
    ui->item_tbl->clearContents();
    ui->item_tbl->setRowCount(row_count);

    for (int row = 0; row < row_count; ++row) {
        const Item *item = items[row];
        QString description = QString::fromStdString(item->description);
        QTableWidgetItem *item_name = new QTableWidgetItem(QString::fromStdString(item->name));
        item_name->setData(Qt::UserRole, item->id);
        item_name->setToolTip(description);
        QTableWidgetItem *item_category = new QTableWidgetItem(
            QString::fromStdString(item->get_type_name()));
        ui->item_tbl->setItem(row, 0, item_name);
        ui->item_tbl->setItem(row, 1, item_category);
    }
    ui->item_tbl->setSortingEnabled(true);
    ui->item_tbl->setUpdatesEnabled(true);
}

void TabInventory::on_item_tbl_itemDoubleClicked(QTableWidgetItem *item)
{
    if (!item)
        return;
    const int row = item->row();
    QTableWidgetItem *cell_name = ui->item_tbl->item(row, 0);
    const std::uint16_t item_id = static_cast<std::uint16_t>(cell_name->data(Qt::UserRole).toUInt());
    add_item_to_inventory(item_id);
}

void TabInventory::configure_ui()
{
    QHeaderView *header_table = ui->inventory_tbl->horizontalHeader();
    header_table->setSectionResizeMode(0, QHeaderView::Stretch);
    header_table->setSectionResizeMode(1, QHeaderView::Fixed);
    header_table->resizeSection(1, 100);
    header_table->setSectionResizeMode(2, QHeaderView::Fixed);
    header_table->resizeSection(2, 80);
    ui->inventory_tbl->setItemDelegateForColumn(2, new SpinBoxDelegate(0, 99, this));

    QHeaderView *header_item_tbl = ui->item_tbl->horizontalHeader();
    header_item_tbl->setSectionResizeMode(0, QHeaderView::Stretch);
    header_item_tbl->setSectionResizeMode(1, QHeaderView::Fixed);
    header_item_tbl->resizeSection(1, 80);

    QPalette pal = ui->item_category_cmb->view()->palette();
    QColor menuGrey = QApplication::palette("QMenu").color(QPalette::Window);
    pal.setColor(QPalette::Base, menuGrey);
    pal.setColor(QPalette::Window, menuGrey);
    ui->item_category_cmb->view()->setPalette(pal);
}

void TabInventory::on_item_add_btn_clicked()
{
    const int row = ui->item_tbl->currentRow();
    if (row < 0)
        return;
    QTableWidgetItem *cell = ui->item_tbl->item(row, 0);
    const std::uint16_t item_id = static_cast<std::uint16_t>(cell->data(Qt::UserRole).toUInt());
    add_item_to_inventory(item_id);
}

int TabInventory::find_inventory_row(std::uint16_t item_id) const
{
    const int col = 0;
    for (int row = 0; row < ui->inventory_tbl->rowCount(); ++row) {
        QTableWidgetItem *cell = ui->inventory_tbl->item(row, col);
        if (cell && cell->data(Qt::UserRole).toUInt() == item_id)
            return row;
    }
    return -1;
}

void TabInventory::add_item_to_inventory(std::uint16_t item_id)
{
    if (save_data_->inventory[item_id] == 0) {
        const QSignalBlocker inventory_block(ui->inventory_tbl);
        ui->inventory_tbl->setUpdatesEnabled(false);
        ui->inventory_tbl->setSortingEnabled(false);

        save_data_->inventory[item_id] = 1;
        const Item &item = ItemRepository::instance().get_by_id(item_id);
        add_row_inventory(item);

        ui->inventory_tbl->setUpdatesEnabled(true);
        ui->inventory_tbl->setSortingEnabled(true);
        const int row = find_inventory_row(item_id);
        ui->inventory_tbl->selectRow(row);
    } else {
        const int row = find_inventory_row(item_id);
        if (row > 0)
            ui->inventory_tbl->selectRow(row);
    }
    return;
}
void TabInventory::on_inventory_tbl_itemChanged(QTableWidgetItem *item)
{
    if (item->column() != 2)
        return;
    const std::uint16_t item_id = static_cast<std::uint16_t>(item->data(Qt::UserRole).toUInt());
    save_data_->inventory[item_id] = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
}
