#ifndef SPIN_BOX_DELEGATE_H
#define SPIN_BOX_DELEGATE_H
#include <QSpinBox>
#include <QStyledItemDelegate>

class SpinBoxDelegate : public QStyledItemDelegate
{
public:
    explicit SpinBoxDelegate(int min_val = 0, int max_val = INT_MAX, QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

private:
    int min_val_;
    int max_val_;
};

#endif // SPIN_BOX_DELEGATE_H
