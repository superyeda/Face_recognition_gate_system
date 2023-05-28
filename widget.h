#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QJsonObject postRes(QString url, QJsonObject params);

private slots:
    void on_check_rem_clicked(bool checked);

    void on_btn_login_clicked();

    void on_btn_register_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
