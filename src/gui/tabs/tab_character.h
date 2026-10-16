#ifndef TAB_CHARACTER_H
#define TAB_CHARACTER_H

#include <QComboBox>
#include <QWidget>
#include "models/save_data.h"

namespace Ui {
class TabCharacter;
}

class TabCharacter : public QWidget
{
    Q_OBJECT

public:
    explicit TabCharacter(QWidget *parent = nullptr);
    ~TabCharacter();

    void load_save_data(SaveData *save);
private slots:
    void on_persona_cmb_currentIndexChanged(int index);

    void on_yen_spin_valueChanged(int arg1);

    void on_mc_xp_spin_editingFinished();

    void on_mc_level_spin_valueChanged(int arg1);

    void on_persona_lv_spin_valueChanged(int arg1);

    void on_persona_xp_spin_editingFinished();

private:
    Ui::TabCharacter *ui;
    SaveData *save_data_ = nullptr;
    std::array<QComboBox *, 8> skill_combos_{};
    void populate_static_data();
    void populate_skill_combos();
    void populate_social_combos();
    void configure_ui();
};

#endif // TAB_CHARACTER_H
