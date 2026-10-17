#include <gui/mainwindow.h>

#include <QApplication>
#include <QMessageBox>

#include <QStyleFactory>
#include "data/arcana_repository.h"
#include "data/item_repository.h"
#include "data/persona_repository.h"
#include "data/skill_repository.h"
#include "data/threshold_repository.h"
#include "utils/combo_box_style.h"

bool load_all_databases()
{
    return ArcanaRepository::instance().load() && ItemRepository::instance().load()
           && PersonaRepository::instance().load() && SkillRepository::instance().load()
           && ThresholdRepository::instance().load();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral(APP_NAME));
    QCoreApplication::setApplicationVersion(QStringLiteral(APP_VERSION));
    a.setWindowIcon(QIcon(":/resources/p3p_save.svg"));
    QStyle *fusion = QStyleFactory::create("Fusion");
    if (fusion) {
        a.setStyle(new Utils::ComboBoxStyle(fusion, &a));
        //a.setStyle(fusion);
    }
    if (!load_all_databases()) {
        QMessageBox::critical(nullptr, "Error", "Failed to load the databases");
        return 1;
    }
    MainWindow w;
    w.show();
    return QApplication::exec();
}
