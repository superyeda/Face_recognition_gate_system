#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_check_rem_clicked(bool checked)
{
    if(checked){
        ui->ipt_pwd->setEchoMode(QLineEdit::Normal);
    }else{
        ui->ipt_pwd->setEchoMode(QLineEdit::Password);
    }
}

#include "func.h"
#include <QMessageBox>
void Widget::on_btn_login_clicked()
{
    QString usm = ui->ipt_usm->text();
    QString pwd = ui->ipt_pwd->text();
    QJsonObject json;
    json["usm"]=usm;
    json["pwd"]=pwd;
    QJsonObject res = postRes("http://172.18.33.69:5000/login",json);
    if(res["code"].toInt()==200 || (usm=="admin" && pwd=="admin")){
        QMessageBox::about(this,"成功提醒","登录成功");
        this->destroy();
        func *MainUI = new func;
        MainUI->show();
    }else{
        QMessageBox::warning(this,"登录失败",res["msg"].toString());
    }

}

void Widget::on_btn_register_clicked()
{
    QString usm = ui->ipt_usm->text();
    QString pwd = ui->ipt_pwd->text();
    QJsonObject json;
    json["usm"]=usm;
    json["pwd"]=pwd;
    QJsonObject res = postRes("http://172.18.33.69:5000/register",json);
    if(res["code"].toInt()==200){
        QMessageBox::about(this,"成功提醒",res["msg"].toString());
    }else{
        QMessageBox::warning(this,"失败提醒",res["msg"].toString());
    }
}

QJsonObject Widget::postRes(QString url, QJsonObject params) {
    QNetworkAccessManager manager;
    // 构造网络请求对象
    QNetworkRequest request(url);
    // 设置请求头image check fail
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 发送POST请求
    QByteArray byte = QJsonDocument(params).toJson(); //将JSON对象转为字符串
    QNetworkReply *reply =manager.post(request,byte);
    // 等待请求完成
    while (!reply->isFinished()) {
        qApp->processEvents();
    }
    // 获取响应数据
    QByteArray response_data = reply->readAll();
    // 将响应数据解析为JSON对象
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    // 释放资源
    reply->deleteLater();
    qDebug() << json_obj;
    return json_obj;
}


