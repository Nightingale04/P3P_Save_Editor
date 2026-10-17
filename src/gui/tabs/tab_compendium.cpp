#include "tab_compendium.h"
#include <QCompleter>
#include <QMenu>
#include <QMessageBox>
#include "data/arcana_repository.h"
#include "data/persona_repository.h"
#include "data/skill_repository.h"
#include "data/threshold_repository.h"
#include "gui/delegates/spin_box_delegate.h"
#include "models/skill.h"
#include "ui_tab_compendium.h"
#include <iostream>

namespace {
QString format_persona_label(const Persona *persona)
{
    if (persona->id == 0) {
        return QString::fromStdString(persona->name);
    }
    const QString arcana = QString::fromStdString(
        ArcanaRepository::instance().get_by_id(persona->arcana).name);
    return QString("%1 [%2]").arg(QString::fromStdString(persona->name), arcana);
}

QTableWidgetItem *create_item(const QVariant &value, std::uint16_t id, bool editable = true)
{
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole, value);
    item->setData(Qt::UserRole, id);
    if (!editable) {
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        return item;
    }
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return item;
}
} // namespace

TabCompendium::TabCompendium(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TabCompendium)
{
    ui->setupUi(this);

    skill_combos_ = {ui->skill_cmb_1,
                     ui->skill_cmb_2,
                     ui->skill_cmb_3,
                     ui->skill_cmb_4,
                     ui->skill_cmb_5,
                     ui->skill_cmb_6,
                     ui->skill_cmb_7,
                     ui->skill_cmb_8};

    configure_ui();
    populate_static_data();
}

TabCompendium::~TabCompendium()
{
    delete ui;
}

void TabCompendium::add_row_compendium(const Persona &persona)
{
    const int row = ui->compendium_tbl->rowCount();
    ui->compendium_tbl->insertRow(row);
    const Arcana &arcana = ArcanaRepository::instance().get_by_id(persona.arcana);

    ui->compendium_tbl->setItem(row,
                                0,
                                create_item(QString::fromStdString(arcana.name), persona.id, false));

    ui->compendium_tbl
        ->setItem(row, 1, create_item(QString::fromStdString(persona.name), persona.id, false));

    ui->compendium_tbl->setItem(row, 2, create_item(persona.level, persona.id));
    ui->compendium_tbl->setItem(row, 3, create_item(persona.total_xp, persona.id));
    ui->compendium_tbl->setItem(row, 4, create_item(persona.st, persona.id));
    ui->compendium_tbl->setItem(row, 5, create_item(persona.ma, persona.id));
    ui->compendium_tbl->setItem(row, 6, create_item(persona.en, persona.id));
    ui->compendium_tbl->setItem(row, 7, create_item(persona.ag, persona.id));
    ui->compendium_tbl->setItem(row, 8, create_item(persona.lu, persona.id));
}

void TabCompendium::load_save_data(SaveData *save)
{
    save_data_ = save;
    if (!save_data_) {
        return;
    };

    ui->compendium_tbl->setUpdatesEnabled(false);
    ui->compendium_tbl->setSortingEnabled(false);
    ui->compendium_tbl->clearContents();
    ui->compendium_tbl->setRowCount(0);

    {
        const QSignalBlocker table_block(ui->compendium_tbl);
        for (std::size_t i = 0; i < save_data_->compendium.size(); ++i) {
            const Persona &persona = save_data_->compendium[i];
            if (persona.id != 0) {
                add_row_compendium(persona);
            }
        }
    }

    ui->compendium_tbl->setUpdatesEnabled(true);
    ui->compendium_tbl->setSortingEnabled(true);
    ui->compendium_tbl->selectRow(0);

    return;
}

void TabCompendium::populate_static_data()
{
    populate_skill_combos();
    populate_persona_cmb();
}

void TabCompendium::populate_persona_cmb()
{
    ui->persona_cmb->clear();
    const std::vector<const Persona *> personas = PersonaRepository::instance()
                                                      .get_obtainable_personas();

    for (const Persona *persona : personas) {
        ui->persona_cmb->addItem(format_persona_label(persona),
                                 static_cast<unsigned int>(persona->id));
    }
    ui->persona_cmb->setCurrentIndex(0);
}

void TabCompendium::populate_skill_combos()
{
    const std::vector<const Skill *> skills = SkillRepository::instance().get_obtainable_skills();
    for (QComboBox *skill_cmb : skill_combos_) {
        const QSignalBlocker blocker(skill_cmb);
        skill_cmb->clear();
        for (const Skill *skill : skills) {
            const int row = skill_cmb->count();
            skill_cmb->addItem(QString::fromStdString(skill->name), skill->id);
            skill_cmb->setItemData(row, QString::fromStdString(skill->description), Qt::ToolTipRole);
        }
        skill_cmb->setCurrentIndex(0);
    }
}

void TabCompendium::add_persona_compendium(std::uint16_t persona_id)
{
    if (save_data_->compendium[persona_id - 1].id == 0) {
        const QSignalBlocker compendium_block(ui->compendium_tbl);
        ui->compendium_tbl->setUpdatesEnabled(false);
        ui->compendium_tbl->setSortingEnabled(false);

        const Persona &persona = PersonaRepository::instance().get_by_id(persona_id);
        save_data_->compendium[persona_id - 1] = persona;
        save_data_->compendium[persona_id - 1].total_xp = ThresholdRepository::instance()
                                                              .get_xp_by_level(persona.base_level,
                                                                               persona.base_level);
        add_row_compendium(save_data_->compendium[persona_id - 1]);

        ui->compendium_tbl->setUpdatesEnabled(true);
        ui->compendium_tbl->setSortingEnabled(true);
        const int row = find_compendium_row(persona_id);
        ui->compendium_tbl->selectRow(row);
    } else {
        const int row = find_compendium_row(persona_id);
        if (row > -1)
            ui->compendium_tbl->selectRow(row);
    }
    return;
}

void TabCompendium::on_persona_add_btn_clicked()
{
    const int idx = ui->persona_cmb->findText(ui->persona_cmb->currentText().trimmed(),
                                              Qt::MatchExactly);
    const std::uint16_t persona_id = (idx > 0) ? static_cast<std::uint16_t>(
                                                     ui->persona_cmb->itemData(idx).toUInt())
                                               : 0;
    if (persona_id == 0) {
        QMessageBox::warning(this,
                             "Invalid Selection",
                             "Please select a valid Persona to add to the Compendium.");
        return;
    }
    add_persona_compendium(persona_id);
}

void TabCompendium::configure_ui()
{
    QHeaderView *header_table = ui->compendium_tbl->horizontalHeader();
    header_table->setSectionResizeMode(0, QHeaderView::Fixed);
    header_table->resizeSection(0, 100);
    header_table->setSectionResizeMode(1, QHeaderView::Stretch);
    header_table->setSectionResizeMode(2, QHeaderView::Fixed);
    header_table->resizeSection(2, 30);
    header_table->setSectionResizeMode(3, QHeaderView::Fixed);
    header_table->resizeSection(3, 100);
    header_table->setSectionResizeMode(4, QHeaderView::Fixed);
    header_table->resizeSection(4, 30);
    header_table->setSectionResizeMode(5, QHeaderView::Fixed);
    header_table->resizeSection(5, 30);
    header_table->setSectionResizeMode(6, QHeaderView::Fixed);
    header_table->resizeSection(6, 30);
    header_table->setSectionResizeMode(7, QHeaderView::Fixed);
    header_table->resizeSection(7, 30);
    header_table->setSectionResizeMode(8, QHeaderView::Fixed);
    header_table->resizeSection(8, 30);

    for (int col = 2; col < header_table->count(); ++col) {
        const int max_val = col == 3 ? INT_MAX : 99;
        const int min_val = col == 2 ? 1 : 0;
        ui->compendium_tbl->setItemDelegateForColumn(col,
                                                     new SpinBoxDelegate(min_val, max_val, this));
    }

    QCompleter *completer = new QCompleter(ui->persona_cmb->model(), ui->persona_cmb);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->persona_cmb->setCompleter(completer);

    auto connect_skill = [this](QComboBox *combo, std::size_t index) {
        connect(combo,
                QOverload<int>::of(&QComboBox::currentIndexChanged),
                this,
                [this, combo, index](int idx) {
                    if (idx < 0)
                        return;
                    const int row = ui->compendium_tbl->currentRow();
                    if (row < 0)
                        return;
                    QTableWidgetItem *cell = ui->compendium_tbl->item(row, 1);
                    const int persona_id = cell->data(Qt::UserRole).toInt();
                    const std::uint16_t skill_id = static_cast<std::uint16_t>(
                        combo->currentData().toUInt());
                    save_data_->compendium[persona_id - 1].skills[index] = skill_id;
                });
    };

    for (std::size_t i = 0; i < skill_combos_.size(); ++i) {
        QComboBox *skill_cmb = skill_combos_[i];
        QCompleter *cmpl = new QCompleter(skill_cmb->model(), skill_cmb);
        cmpl->setFilterMode(Qt::MatchContains);
        cmpl->setCaseSensitivity(Qt::CaseInsensitive);
        skill_cmb->setCompleter(cmpl);
        connect_skill(skill_cmb, i);
    }
}

void TabCompendium::on_compendium_tbl_itemSelectionChanged()
{
    const int row = ui->compendium_tbl->currentRow();
    if (row < 0)
        return;

    QTableWidgetItem *persona_cell = ui->compendium_tbl->item(row, 1);
    const std::uint16_t id = static_cast<std::uint16_t>(persona_cell->data(Qt::UserRole).toInt());
    const std::array<std::uint16_t, 8> skills = save_data_->compendium[id - 1].skills;

    for (std::size_t i = 0; i < skills.size(); ++i) {
        QComboBox *skill_cmb = skill_combos_[i];
        QSignalBlocker skill_block(skill_cmb);
        int skill_idx = skill_cmb->findData(skills[i]);
        if (skill_idx == -1) {
            const Skill &skill = SkillRepository::instance().get_by_id(skills[i]);
            const QString debug_label = QString::fromStdString(skill.name) + " [DEBUG]";
            skill_idx = skill_cmb->count();
            skill_cmb->addItem(debug_label, skill.id);
            skill_cmb->setItemData(skill_idx,
                                   QString::fromStdString(skill.description),
                                   Qt::ToolTipRole);
        }
        skill_cmb->setCurrentIndex(skill_idx);
    }
    return;
}

void TabCompendium::on_compendium_tbl_itemChanged(QTableWidgetItem *item)
{
    const std::uint16_t persona_id = static_cast<std::uint16_t>(item->data(Qt::UserRole).toUInt());
    switch (item->column()) {
    case 2: {
        const QSignalBlocker table_block(ui->compendium_tbl);

        const std::uint8_t level = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
        const std::uint8_t base_level = save_data_->compendium[persona_id - 1].base_level;
        const std::uint32_t persona_xp = ThresholdRepository::instance().get_xp_by_level(base_level,
                                                                                         level);
        save_data_->compendium[persona_id - 1].level = level;
        save_data_->compendium[persona_id - 1].total_xp = persona_xp;
        ui->compendium_tbl->item(item->row(), 3)->setData(Qt::DisplayRole, persona_xp);
        break;
    }
    case 3: {
        const QSignalBlocker table_block(ui->compendium_tbl);

        const std::uint32_t xp = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
        const std::uint8_t base_level = save_data_->compendium[persona_id - 1].base_level;
        const std::uint8_t persona_level = ThresholdRepository::instance()
                                               .get_level_by_xp(base_level, xp);

        save_data_->compendium[persona_id - 1].level = persona_level;
        save_data_->compendium[persona_id - 1].total_xp = xp;
        ui->compendium_tbl->item(item->row(), 2)->setData(Qt::DisplayRole, persona_level);
        break;
    }
    case 4: {
        const std::uint8_t st = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
        save_data_->compendium[persona_id - 1].st = st;
        break;
    }
    case 5: {
        const std::uint8_t ma = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
        save_data_->compendium[persona_id - 1].ma = ma;
        break;
    }
    case 6: {
        const std::uint8_t en = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
        save_data_->compendium[persona_id - 1].en = en;
        break;
    }
    case 7: {
        const std::uint8_t ag = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
        save_data_->compendium[persona_id - 1].ag = ag;
        break;
    }
    case 8: {
        const std::uint8_t lu = static_cast<std::uint8_t>(item->data(Qt::DisplayRole).toUInt());
        save_data_->compendium[persona_id - 1].lu = lu;
        break;
    }
    default:
        break;
    }
}

int TabCompendium::find_compendium_row(std::uint16_t persona_id) const
{
    const int col = 1;
    for (int row = 0; row < ui->compendium_tbl->rowCount(); ++row) {
        QTableWidgetItem *cell = ui->compendium_tbl->item(row, col);
        if (cell && cell->data(Qt::UserRole).toUInt() == persona_id)
            return row;
    }
    return -1;
}
void TabCompendium::on_compendium_tbl_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *cell = ui->compendium_tbl->itemAt(pos);
    if (!cell)
        return;

    QMenu menu(this);
    QAction *removeAction = menu.addAction(tr("Remove persona"));

    if (menu.exec(ui->compendium_tbl->viewport()->mapToGlobal(pos)) == removeAction) {
        const std::uint16_t persona_id = static_cast<std::uint16_t>(
            cell->data(Qt::UserRole).toUInt());
        save_data_->compendium[persona_id - 1].id = 0;
        save_data_->compendium[persona_id - 1].flag = 0;
        ui->compendium_tbl->removeRow(cell->row());
    }
}
