#ifndef FUNC_H
#define FUNC_H

#include <QWidget>
#include <QCamera>                  //摄像头类
#include <QCameraImageCapture>      //摄像头拍照
//#include <QNetworkAccessManager>    //网络管理
//#include <QNetworkReply>            //服务响应
//#include <QNetworkRequest>          //服务请求
//#include <QUrl>                     //超链接类
#include <QtNetwork>
#include <QJsonObject>              //JSON对象类
#include <QJsonArray>               //JSON数组类
#include <QJsonValue>               //JSON数值类
#include <QJsonDocument>            //JSON格式化
namespace Ui {
class func;
}

class func : public QWidget
{
    Q_OBJECT

public:
    explicit func(QWidget *parent = nullptr);
    ~func();

public slots:
    void displayImage(int index,QImage image);
    QJsonObject postRes(QString url,QJsonObject postData);
    QJsonObject getRes(QString url);
    void showResult(QString url,QJsonObject json);
    void getToken();

private slots:
    void on_btn_open_clicked();

    void on_btn_close_clicked();

    void on_btn_get_clicked();

    void on_btn_select_clicked();

    void on_btn_user_select_clicked();

    void on_btn_user_delete_clicked();

    void on_btn_face_register_clicked();

    void on_btn_face_update_clicked();

    void on_btn_user_search_clicked();

    void on_btn_face_list_clicked();

private:
    Ui::func *ui;
    QCamera *Camera;
    QCameraImageCapture *ImageCapture;
    QString token;
    QString faceBuf;
};

#endif // FUNC_H
