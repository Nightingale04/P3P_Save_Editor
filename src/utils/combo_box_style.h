#ifndef COMBOBOXSTYLE_H
#define COMBOBOXSTYLE_H

#include <QProxyStyle>
#include <QStyleOption>

namespace Utils {
class ComboBoxStyle : public QProxyStyle
{
public:
    explicit ComboBoxStyle(QStyle *baseStyle = nullptr, QObject *parent = nullptr)
        : QProxyStyle(baseStyle)
    {
        if (parent) {
            setParent(parent);
        }
    }
    int styleHint(StyleHint hint,
                  const QStyleOption *option = nullptr,
                  const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const override
    {
        if (hint == QStyle::SH_ComboBox_Popup) {
            return 0;
        }
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

} // namespace Utils

#endif // COMBOBOXSTYLE_H
