#include "func.h"
#include "ui_func.h"
#include <QMessageBox>
func::func(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func)
{
    ui->setupUi(this);
    getToken();
}

func::~func()
{
    delete ui;
}
#include <QCameraInfo>
void func::on_btn_open_clicked()
{
    // 获取摄像头信息
    QCameraInfo Info = QCameraInfo::defaultCamera();
    if(Info.isNull()){
        QMessageBox::warning(this,"eror","Don't have camera!!!");
        return;
    }
    // 打开摄像头
    Camera = new QCamera(Info,this);
    // 图像获取
    ImageCapture =new QCameraImageCapture(Camera);
    // 保存图像
    ImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    connect(ImageCapture,SIGNAL(imageCaptured(int,QImage)),this,SLOT(displayImage(int,QImage)));
    // 视频预览
    Camera->setCaptureMode(QCamera::CaptureStillImage);
    Camera->setViewfinder(ui->video);
    Camera->start();

}
void func::getToken(){
    QJsonObject params;
    QString client_secret="APP_Secret";//填入自己百度智能云APP_Secret
    QString client_id="APP_ID";//填入自己百度智能云APP_ID
    QString url="https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id="+client_id+"&client_secret="+client_secret+"&";
    QJsonObject a = func::postRes(url,params);
    QString token = a.value("access_token").toString();
    func::token=token;
    qDebug() << token;
}

void func::on_btn_close_clicked()
{
    Camera->stop();
}


void func::on_btn_get_clicked()
{
    ImageCapture->capture("YedaTan.jpg");
}

void func::displayImage(int index,QImage image){
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label_photo_select->setPixmap(pixmap);
    ui->label_photo_resgister->setPixmap(pixmap);
    ui->label_photo_update->setPixmap(pixmap);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    image.save(&buffer,"png");
    faceBuf = bytes.toBase64();
}


// 封装GET请求
QJsonObject func::getRes(QString url) {
    QNetworkAccessManager manager;
    QNetworkRequest request(url);    // 构造网络请求对象
    QNetworkReply *reply = manager.get(request);    // 发送GET请求
    // 等待请求完成
    while (!reply->isFinished()) {
        qApp->processEvents();
    }
    QByteArray response_data = reply->readAll();    // 获取响应数据
    qDebug() << response_data;
    // 将响应数据解析为JSON对象
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    reply->deleteLater();    // 释放资源
    return json_obj;
}

// 封装POST请求
QJsonObject func::postRes(QString url, QJsonObject params) {
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

// 人脸搜索
void func::on_btn_select_clicked()
{
    QString group=ui->group_face_select->currentText();
    if(group=="学生")group="1";
    else group="2";
    QString url="https://aip.baidubce.com/rest/2.0/face/v3/search?access_token="+func::token;
    QJsonObject json;
    json["image"] = func::faceBuf;
    json["image_type"] = "BASE64";
    json["group_id_list"]=group;
    showResult(url,json);
}
// 获取用户列表
void func::on_btn_user_select_clicked()
{
    QString group=ui->group_user_select->currentText();
    if(group=="学生")group="1";
    else group="2";
    QString url="https://aip.baidubce.com/rest/2.0/face/v3/faceset/group/getusers?access_token="+func::token;
    QJsonObject json;
    json["group_id"]=group;
    showResult(url,json);
}
// 人脸删除
void func::on_btn_user_delete_clicked()
{
    QString group=ui->group_user_delete->currentText();
    QString user_id=ui->ipt_user_delete->text();
    if(group=="学生")group="1";
    else group="2";
    QString url="https://aip.baidubce.com/rest/2.0/face/v3/faceset/user/delete?access_token="+func::token;
    QJsonObject json;
    json["user_id"] = user_id;
    json["group_id"]=group;
    showResult(url,json);
}
// 人脸注册
void func::on_btn_face_register_clicked()
{
    QString group=ui->group_face_register->currentText();
    if(group=="学生")group="1";
    else group="2";
    QString url="https://aip.baidubce.com/rest/2.0/face/v3/faceset/user/add?access_token="+func::token;
    QJsonObject json;
    json["image"] = func::faceBuf;
    json["group_id"]=group;
    json["image_type"]="BASE64";
    json["user_id"]=ui->ipt_face_register->text();
    showResult(url,json);
}
// 人脸更新
void func::on_btn_face_update_clicked()
{
    QString group=ui->group_face_update->currentText();
    if(group=="学生")group="1";
    else group="2";
    QString url="https://aip.baidubce.com/rest/2.0/face/v3/faceset/user/update?access_token="+func::token;
    QJsonObject json;
    json["image"] = func::faceBuf;
    json["group_id"]=group;
    json["image_type"]="BASE64";
    json["user_id"]=ui->ipt_face_update->text();
    json["user_info"]=ui->ipt_user_info->toPlainText();
    showResult(url,json);
}
// 用户信息查询
void func::on_btn_user_search_clicked()
{
    QString group=ui->group_user_search->currentText();
    if(group=="学生")group="1";
    else group="2";
    QString url="https://aip.baidubce.com/rest/2.0/face/v3/faceset/user/get?access_token="+func::token;
    QJsonObject json;
    json["group_id"]=group;
    json["user_id"]=ui->ipt_user_search->text();
    showResult(url,json);
}
//　获取人脸列表
void func::on_btn_face_list_clicked()
{
    QString group=ui->group_face_list->currentText();
    if(group=="学生")group="1";
    else group="2";
    QString url="https://aip.baidubce.com/rest/2.0/face/v3/faceset/face/getlist?access_token="+func::token;
    QJsonObject json;
    json["group_id"]=group;
    json["user_id"]=ui->ipt_face_list->text();
    showResult(url,json);
}
// 展示结果
void func::showResult(QString url,QJsonObject json){
    QJsonObject res = postRes(url,json);
    qDebug()<<res["error_code"].toInt();
    if(res["error_code"].toInt())
    ui->result_box->setText("请求错误:"+res["error_msg"].toString());
    else
    ui->result_box->setText(QString(QJsonDocument(res["result"].toObject()).toJson()));
}
