#include "tab_character.h"
#include <QCompleter>
#include "data/persona_repository.h"
#include "data/skill_repository.h"
#include "data/threshold_repository.h"
#include "models/persona.h"
#include "models/progression.h"
#include "models/skill.h"
#include "ui_tab_character.h"
#include <iostream>

TabCharacter::TabCharacter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TabCharacter)
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

TabCharacter::~TabCharacter()
{
    delete ui;
}

void TabCharacter::load_save_data(SaveData *save)
{
    save_data_ = save;
    if (!save_data_) {
        return;
    };

    const QString full_name = QString::fromStdString(save_data_->firstname + " "
                                                     + save_data_->surname);
    ui->full_name_txt->setText(full_name);
    const QSignalBlocker yen_block(ui->yen_spin);
    ui->yen_spin->setValue(save_data_->yen);
    const QSignalBlocker level_block(ui->mc_level_spin);
    ui->mc_level_spin->setValue(save_data_->mc_level);
    const QSignalBlocker mc_xp_block(ui->mc_xp_spin);
    ui->mc_xp_spin->setValue(save_data_->mc_xp);

    const QSignalBlocker academic_blocker(ui->academics_cmb);
    ui->academics_cmb->setCurrentIndex(
        progression::get_social_rank(progression::SocialStat::Academic, save_data_->academics));
    const QSignalBlocker charm_blocker(ui->charm_cmb);
    ui->charm_cmb->setCurrentIndex(
        progression::get_social_rank(progression::SocialStat::Charm, save_data_->charm));
    const QSignalBlocker courage_blocker(ui->courage_cmb);
    ui->courage_cmb->setCurrentIndex(
        progression::get_social_rank(progression::SocialStat::Courage, save_data_->courage));

    ui->persona_cmb->clear();
    for (const Persona &persona : save_data_->active_personas) {
        ui->persona_cmb->addItem(QString::fromStdString(persona.name), persona.id);
    }
    ui->persona_cmb->setCurrentIndex(0);
}

void TabCharacter::populate_static_data()
{
    populate_skill_combos();
    populate_social_combos();
}

void TabCharacter::populate_skill_combos()
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

void TabCharacter::populate_social_combos()
{
    const std::pair<QComboBox *, progression::SocialStat> stats[] = {
        {ui->academics_cmb, progression::SocialStat::Academic},
        {ui->charm_cmb, progression::SocialStat::Charm},
        {ui->courage_cmb, progression::SocialStat::Courage},
    };

    for (auto &[combo, stat] : stats) {
        const QSignalBlocker blocker(combo);
        combo->clear();
        for (const progression::Rank &rank : progression::get_social_table(stat))
            combo->addItem(QString::fromUtf8(rank.name), rank.threshold);
        combo->setCurrentIndex(0);
    }
}

void TabCharacter::on_persona_cmb_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }
    const Persona &persona = save_data_->active_personas[index];
    const QSignalBlocker level_block(ui->persona_lv_spin);
    ui->persona_lv_spin->setValue(persona.level);
    const QSignalBlocker xp_block(ui->persona_xp_spin);
    ui->persona_xp_spin->setValue(persona.total_xp);

    auto set_stat_pair = [](QSlider *slider, QSpinBox *spin, std::uint8_t value) {
        const QSignalBlocker slider_block(slider);
        const QSignalBlocker spin_block(spin);
        slider->setValue(value);
        spin->setValue(value);
    };

    set_stat_pair(ui->st_sld, ui->st_spin, persona.st);
    set_stat_pair(ui->ma_sld, ui->ma_spin, persona.ma);
    set_stat_pair(ui->en_sld, ui->en_spin, persona.en);
    set_stat_pair(ui->ag_sld, ui->ag_spin, persona.ag);
    set_stat_pair(ui->lu_sld, ui->lu_spin, persona.lu);

    for (std::size_t i = 0; i < skill_combos_.size(); ++i) {
        QComboBox *skill_cmb = skill_combos_[i];
        QSignalBlocker skill_block(skill_cmb);
        int skill_idx = skill_cmb->findData(persona.skills[i]);
        if (skill_idx == -1) {
            const Skill &skill = SkillRepository::instance().get_by_id(persona.skills[i]);
            const QString debug_label = QString::fromStdString(skill.name) + " [DEBUG]";
            skill_idx = skill_cmb->count();
            skill_cmb->addItem(debug_label, skill.id);
            skill_cmb->setItemData(skill_idx,
                                   QString::fromStdString(skill.description),
                                   Qt::ToolTipRole);
        }
        skill_cmb->setCurrentIndex(skill_idx);
    }
}

void TabCharacter::configure_ui()
{
    auto connect_stat = [this](QSpinBox *spin, std::uint8_t Persona::*member) {
        connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, member](int idx) {
            const int persona_idx = ui->persona_cmb->currentIndex();
            save_data_->active_personas[persona_idx].*member = static_cast<std::uint8_t>(idx);
        });
    };

    auto connect_skill = [this](QComboBox *combo, std::size_t index) {
        connect(combo,
                QOverload<int>::of(&QComboBox::currentIndexChanged),
                this,
                [this, combo, index](int idx) {
                    if (idx < 0)
                        return;
                    const int persona_idx = ui->persona_cmb->currentIndex();
                    const std::uint16_t skill_id = static_cast<std::uint16_t>(
                        combo->currentData().toUInt());
                    save_data_->active_personas[persona_idx].skills[index] = skill_id;
                });
    };

    auto connect_social_stat = [this](QComboBox *combo, std::uint16_t SaveData::*member) {
        connect(combo,
                QOverload<int>::of(&QComboBox::currentIndexChanged),
                this,
                [this, combo, member](int idx) {
                    if (idx < 0)
                        return;
                    const std::uint16_t social_points = static_cast<std::uint8_t>(
                        combo->currentData().toUInt());
                    save_data_->*member = social_points;
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

    connect_stat(ui->st_spin, &Persona::st);
    connect_stat(ui->ma_spin, &Persona::ma);
    connect_stat(ui->en_spin, &Persona::en);
    connect_stat(ui->ag_spin, &Persona::ag);
    connect_stat(ui->lu_spin, &Persona::lu);

    connect_social_stat(ui->academics_cmb, &SaveData::academics);
    connect_social_stat(ui->charm_cmb, &SaveData::charm);
    connect_social_stat(ui->courage_cmb, &SaveData::courage);
}

void TabCharacter::on_yen_spin_valueChanged(int arg1)
{
    save_data_->yen = arg1;
    return;
}

void TabCharacter::on_mc_xp_spin_editingFinished()
{
    const int xp = ui->mc_xp_spin->value();
    const QSignalBlocker level_spin_block(ui->mc_level_spin);
    const std::uint8_t mc_level = progression::get_mc_level(xp);
    save_data_->mc_level = mc_level;
    save_data_->mc_xp = xp;
    ui->mc_level_spin->setValue(mc_level);
    return;
}

void TabCharacter::on_mc_level_spin_valueChanged(int arg1)
{
    const std::uint8_t level = static_cast<std::uint8_t>(arg1);
    const QSignalBlocker xp_spin_block(ui->mc_xp_spin);
    const std::uint32_t mc_xp = progression::get_mc_xp(level);
    save_data_->mc_level = static_cast<std::uint8_t>(level);
    save_data_->mc_xp = mc_xp;
    ui->mc_xp_spin->setValue(mc_xp);
    return;
}

void TabCharacter::on_persona_lv_spin_valueChanged(int arg1)
{
    const QSignalBlocker xp_spin_block(ui->persona_xp_spin);
    const std::uint8_t persona_level = static_cast<std::uint8_t>(arg1);
    const int persona_idx = ui->persona_cmb->currentIndex();
    Persona &persona = save_data_->active_personas[persona_idx];

    std::cerr << static_cast<int>(persona_level);
    const std::uint32_t persona_xp = ThresholdRepository::instance()
                                         .get_xp_by_level(persona.base_level, persona_level);
    persona.level = persona_level;
    persona.total_xp = persona_xp;
    ui->persona_xp_spin->setValue(persona_xp);
    return;
}

void TabCharacter::on_persona_xp_spin_editingFinished()
{
    const QSignalBlocker lv_spin_block(ui->persona_lv_spin);
    const std::uint32_t persona_xp = static_cast<std::uint32_t>(ui->persona_xp_spin->value());
    const int persona_idx = ui->persona_cmb->currentIndex();
    Persona &persona = save_data_->active_personas[persona_idx];

    const std::uint8_t persona_level = ThresholdRepository::instance()
                                           .get_level_by_xp(persona.base_level, persona_xp);
    persona.level = persona_level;
    persona.total_xp = persona_xp;
    ui->persona_lv_spin->setValue(persona_level);
    return;
}