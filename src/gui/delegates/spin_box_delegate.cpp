#include "spin_box_delegate.h"
#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(int min_val, int max_val, QObject *parent)
    : QStyledItemDelegate(parent)
    , min_val_(min_val)
    , max_val_(max_val)
{}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &,
                                       const QModelIndex &) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setAutoFillBackground(true);
    editor->setRange(min_val_, max_val_);
    editor->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::DisplayRole).toInt();
    QSpinBox *spinbox = qobject_cast<QSpinBox *>(editor);
    spinbox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QSpinBox *spinbox = static_cast<QSpinBox *>(editor);
    spinbox->interpretText();
    int value = spinbox->value();

    model->setData(index, value, Qt::EditRole);
}